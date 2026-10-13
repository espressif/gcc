/* Subroutines for the Espressif XESPV vendor extension.
   Copyright (C) 2026 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This file is Espressif-specific and is not part of upstream GCC.  Keep
   XESPV code here rather than in riscv.cc or riscv-string.cc, which only
   call into it, so that updating to a new GCC release does not conflict
   with upstream changes to those files.  */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "memmodel.h"
#include "tm_p.h"
#include "insn-config.h"
#include "emit-rtl.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "varasm.h"
#include "explow.h"
#include "expr.h"
#include "optabs.h"
#include "predict.h"
#include "tree-pass.h"
#include "rtl-iter.h"
#include "langhooks.h"

/* Return true if RCLASS is a non-empty class of XESPV q registers.  */

bool
riscv_xespv_q_class_p (reg_class_t rclass)
{
  return rclass != NO_REGS && reg_class_subset_p (rclass, Q_REGS);
}

/* Return true if INSN is one of the XESPV q-register instructions used
   by the -mxespv-memcpy block move expansion.  */

static bool
riscv_xespv_memcpy_insn_p (rtx_insn *insn)
{
  switch (INSN_CODE (insn))
    {
    case CODE_FOR_xespv_vld128_ip:
    case CODE_FOR_xespv_vst128_ip:
    case CODE_FOR_xespv_vld_l64_ip:
    case CODE_FOR_xespv_vld_h64_ip:
    case CODE_FOR_xespv_vst_l64_ip:
    case CODE_FOR_xespv_vst_h64_ip:
    case CODE_FOR_xespv_src_q:
      return true;
    default:
      return false;
    }
}

/* Implement INSN_BASE_REG_CLASS.  */

enum reg_class
riscv_insn_base_reg_class (rtx_insn *insn)
{
  if (insn
      && (INSN_CODE (insn) == CODE_FOR_movti_xespv
	  || riscv_xespv_memcpy_insn_p (insn)))
    return ESP_GR_REGS;

  return BASE_REG_CLASS;
}

/* Return true if PAT is the pattern of an instruction accepted by
   riscv_xespv_memcpy_insn_p.  This does not recognize the instruction, so
   it can be used from insn conditions.  */

static bool
riscv_xespv_memcpy_pattern_p (rtx pat)
{
  subrtx_iterator::array_type array;
  FOR_EACH_SUBRTX (iter, array, pat, NONCONST)
    {
      const_rtx x = *iter;
      if (GET_CODE (x) == UNSPEC)
	switch (XINT (x, 1))
	  {
	  case UNSPEC_XESPV_VLD128:
	  case UNSPEC_XESPV_VST128:
	  case UNSPEC_XESPV_VLD_L64:
	  case UNSPEC_XESPV_VLD_H64:
	  case UNSPEC_XESPV_VST_L64:
	  case UNSPEC_XESPV_VST_H64:
	  case UNSPEC_XESPV_ANDQ:
	  case UNSPEC_XESPV_NOTQ:
	  case UNSPEC_XESPV_ORQ:
	  case UNSPEC_XESPV_XORQ:
	  case UNSPEC_XESPV_ZERO_Q:
	  case UNSPEC_XESPV_MOVI32:
	  case UNSPEC_XESPV_MOVI8:
	    return true;
	  default:
	    break;
	  }
      else if (GET_CODE (x) == UNSPEC_VOLATILE
	       && XINT (x, 1) == UNSPECV_XESPV_SRC_Q)
	return true;
    }
  return false;
}

/* Return true if OP is a valid pointer increment for an XESPV .ip load or
   store of SIZE bytes: a multiple of SIZE in the signed 8-bit range.  */

bool
riscv_xespv_ip_offset_p (rtx op, int size)
{
  HOST_WIDE_INT val = INTVAL (op);
  return val % size == 0 && IN_RANGE (val / size, -128, 127);
}

/* Return true if OP is a memory operand that XESPV q-register loads and
   stores can access: a 16-byte aligned MEM whose address is a register
   that is (or can still become) one of x8-x15/x24-x31.  */

bool
riscv_xespv_q_mem_p (rtx op)
{
  if (!MEM_P (op) || MEM_ALIGN (op) < 128)
    return false;

  rtx addr = XEXP (op, 0);
  if (!REG_P (addr))
    return false;

  return !HARD_REGISTER_P (addr) || ESP_GR_REG_P (REGNO (addr));
}

/* Return true if X is an XESPV q register.  */

static bool
riscv_q_reg_rtx_p (rtx x)
{
  return REG_P (x) && Q_REG_P (REGNO (x));
}

/* Return true if X is a TImode memory operand that is not aligned well
   enough for XESPV q-register loads and stores.  */

static bool
riscv_misaligned_ti_mem_p (rtx x)
{
  return MEM_P (x) && MEM_ALIGN (x) < 128;
}

/* Emit a TImode move from SRC to DEST as word-sized moves.  Used before
   register allocation.  */

static void
riscv_emit_ti_word_moves (rtx dest, rtx src)
{
  unsigned int nwords = GET_MODE_SIZE (TImode) / UNITS_PER_WORD;

  src = avoid_constant_pool_reference (src);
  if (reg_overlap_mentioned_p (dest, src))
    {
      rtx tmp = gen_reg_rtx (TImode);
      riscv_emit_ti_word_moves (tmp, src);
      src = tmp;
    }

  if (REG_P (dest) && !HARD_REGISTER_P (dest))
    emit_clobber (dest);

  for (unsigned int i = 0; i < nwords; i++)
    riscv_emit_move (operand_subword (dest, i, 1, TImode),
		     operand_subword (src, i, 1, TImode));
}

/* Expand a TImode move from SRC to DEST when XESPV q registers are
   available.  Moves that cannot use movti_xespv directly are emitted
   as word-sized moves, going through a pseudo when a q register is
   involved.  */

void
riscv_expand_movti_xespv (rtx dest, rtx src)
{
  if (!can_create_pseudo_p ())
    {
      emit_insn (gen_rtx_SET (dest, src));
      return;
    }

  if (riscv_q_reg_rtx_p (dest) || riscv_q_reg_rtx_p (src))
    {
      rtx other = riscv_q_reg_rtx_p (dest) ? src : dest;
      if (riscv_misaligned_ti_mem_p (other))
	{
	  rtx tmp = gen_reg_rtx (TImode);
	  if (other == src)
	    {
	      riscv_emit_ti_word_moves (tmp, src);
	      emit_insn (gen_rtx_SET (dest, tmp));
	    }
	  else
	    {
	      emit_insn (gen_rtx_SET (tmp, src));
	      riscv_emit_ti_word_moves (dest, tmp);
	    }
	  return;
	}

      if (CONSTANT_P (src) && src != CONST0_RTX (TImode))
	{
	  rtx mem = force_const_mem (TImode, src);
	  if (mem)
	    src = validize_mem (mem);
	  else
	    {
	      rtx tmp = gen_reg_rtx (TImode);
	      riscv_emit_ti_word_moves (tmp, src);
	      src = tmp;
	    }
	}

      if (MEM_P (src) && !REG_P (XEXP (src, 0)))
	src = replace_equiv_address (src, force_reg (Pmode, XEXP (src, 0)));
      if (MEM_P (dest) && !REG_P (XEXP (dest, 0)))
	dest = replace_equiv_address (dest, force_reg (Pmode, XEXP (dest, 0)));

      emit_insn (gen_rtx_SET (dest, src));
      return;
    }

  if ((REG_P (dest) && HARD_REGISTER_P (dest))
      || (REG_P (src) && HARD_REGISTER_P (src))
      || riscv_misaligned_ti_mem_p (dest) || riscv_misaligned_ti_mem_p (src))
    {
      riscv_emit_ti_word_moves (dest, src);
      return;
    }

  /* Keep constants in the constant pool for now, so that they can be
     loaded into a q register.  Moves that end up not involving a q register
     are split into word moves of the constant before register allocation.  */
  if (CONSTANT_P (src) && src != CONST0_RTX (TImode))
    {
      if (MEM_P (dest))
	src = force_reg (TImode, src);
      else if (rtx mem = force_const_mem (TImode, src))
	src = validize_mem (mem);
      else
	{
	  riscv_emit_ti_word_moves (dest, src);
	  return;
	}
    }

  if (MEM_P (dest) && MEM_P (src))
    src = force_reg (TImode, src);

  emit_insn (gen_rtx_SET (dest, src));
}

/* Return the set of TImode pseudos in the current function that may be
   allocated to XESPV q registers: operands of q-register instructions
   (intrinsics, memcpy, memcmp) and pseudos copied to or from a q register,
   together with the pseudos they are copied to or from.  The result is
   cached for the current pass.  */

static bitmap
riscv_xespv_q_pseudos ()
{
  static bitmap_obstack q_pseudos_obstack;
  static bitmap q_pseudos;
  static opt_pass *cached_pass;
  static int cached_funcdef_no = -1;

  if (!q_pseudos)
    {
      bitmap_obstack_initialize (&q_pseudos_obstack);
      q_pseudos = BITMAP_ALLOC (&q_pseudos_obstack);
    }
  else if (cached_pass == current_pass
	   && cached_funcdef_no == cfun->funcdef_no)
    return q_pseudos;

  cached_pass = current_pass;
  cached_funcdef_no = cfun->funcdef_no;
  bitmap_clear (q_pseudos);

  auto_vec<std::pair<unsigned int, unsigned int> > copies;
  for (rtx_insn *insn = get_insns (); insn; insn = NEXT_INSN (insn))
    {
      if (!NONDEBUG_INSN_P (insn))
	continue;

      rtx pat = PATTERN (insn);
      if (riscv_xespv_memcpy_pattern_p (pat))
	{
	  subrtx_iterator::array_type array;
	  FOR_EACH_SUBRTX (iter, array, pat, NONCONST)
	    if (REG_P (*iter) && GET_MODE (*iter) == TImode
		&& !HARD_REGISTER_P (*iter))
	      bitmap_set_bit (q_pseudos, REGNO (*iter));
	  continue;
	}

      rtx set = single_set (insn);
      if (!set || GET_MODE (SET_DEST (set)) != TImode)
	continue;

      rtx dest = SET_DEST (set), src = SET_SRC (set);
      if (!REG_P (dest) || !REG_P (src))
	continue;

      if (!HARD_REGISTER_P (dest) && !HARD_REGISTER_P (src))
	copies.safe_push (std::make_pair (REGNO (dest), REGNO (src)));
      else if (riscv_q_reg_rtx_p (src) && !HARD_REGISTER_P (dest))
	bitmap_set_bit (q_pseudos, REGNO (dest));
      else if (riscv_q_reg_rtx_p (dest) && !HARD_REGISTER_P (src))
	bitmap_set_bit (q_pseudos, REGNO (src));
    }

  bool changed = true;
  while (changed)
    {
      changed = false;
      for (auto &copy : copies)
	if (bitmap_bit_p (q_pseudos, copy.first)
	    != bitmap_bit_p (q_pseudos, copy.second))
	  {
	    bitmap_set_bit (q_pseudos, copy.first);
	    bitmap_set_bit (q_pseudos, copy.second);
	    changed = true;
	  }
    }

  return q_pseudos;
}

/* Return true if X is a pseudo that may be allocated to a q register.  */

static bool
riscv_xespv_q_pseudo_p (rtx x)
{
  return (REG_P (x) && !HARD_REGISTER_P (x)
	  && bitmap_bit_p (riscv_xespv_q_pseudos (), REGNO (x)));
}

/* Return true if a TImode move from SRC to DEST should be split into
   word-sized moves.  After reload, these are the moves that do not involve
   a q register.  Before register allocation, split the moves that cannot
   end up using a q register, so that their pseudos can be decomposed
   just like on targets without movti.  */

bool
riscv_split_movti_xespv_p (rtx dest, rtx src)
{
  if (riscv_q_reg_rtx_p (dest) || riscv_q_reg_rtx_p (src))
    return false;

  if (reload_completed)
    return true;

  if (!can_create_pseudo_p ())
    return false;

  return !riscv_xespv_q_pseudo_p (dest) && !riscv_xespv_q_pseudo_p (src);
}

/* Split a TImode move from SRC to DEST that does not involve a q register
   into word-sized moves.  */

void
riscv_split_movti_xespv (rtx dest, rtx src)
{
  unsigned int nwords = GET_MODE_SIZE (TImode) / UNITS_PER_WORD;
  rtx d[4], s[4];

  src = avoid_constant_pool_reference (src);
  if (REG_P (dest) && !HARD_REGISTER_P (dest))
    emit_clobber (dest);

  for (unsigned int i = 0; i < nwords; i++)
    {
      d[i] = operand_subword (dest, i, 0, TImode);
      s[i] = operand_subword (src, i, 0, TImode);
      gcc_assert (d[i] && s[i]);
    }

  /* Pick an order in which no source word is clobbered before it is
     read: copy overlapping registers downwards if needed, and load the
     word that holds the address of SRC last.  */
  int last = -1;
  bool reverse = false;
  if (REG_P (dest) && REG_P (src))
    reverse = REGNO (dest) > REGNO (src);
  else if (REG_P (dest) && MEM_P (src))
    for (unsigned int i = 0; i < nwords; i++)
      if (reg_overlap_mentioned_p (d[i], src))
	last = i;

  for (unsigned int n = 0; n < nwords; n++)
    {
      unsigned int i = reverse ? nwords - 1 - n : n;
      if ((int) i != last)
	riscv_emit_move (d[i], s[i]);
    }
  if (last >= 0)
    riscv_emit_move (d[last], s[last]);
}

/* Return the assembly code for movti_xespv, or "#" if the move does not
   involve an XESPV q register and needs to be split.  */

const char *
riscv_output_movti_xespv (rtx *operands)
{
  rtx dest = operands[0], src = operands[1];

  if (riscv_split_movti_xespv_p (dest, src))
    return "#";

  if (riscv_q_reg_rtx_p (dest))
    {
      if (riscv_q_reg_rtx_p (src))
	return "esp.orq\t%0,%1,%1";
      if (src == CONST0_RTX (TImode))
	return "esp.zero.q\t%0";
      gcc_assert (MEM_P (src));
      operands[1] = XEXP (src, 0);
      return "esp.vld.128.ip\t%0,%1,0";
    }

  gcc_assert (riscv_q_reg_rtx_p (src) && MEM_P (dest));
  operands[0] = XEXP (dest, 0);
  return "esp.vst.128.ip\t%1,%0,0";
}

/* Block moves with XESPV q registers (-mxespv-memcpy).

   The copy is done in 16-byte chunks that go through q registers.  Only
   aligned q-register loads and stores are used, so the code behaves the
   same whether the core forces the alignment of misaligned PIE accesses
   or handles them.  How a chunk is moved depends on the alignment known
   at compile time:

   - a 16-byte aligned source or destination uses esp.vld.128.ip or
     esp.vst.128.ip;
   - an 8-byte aligned one uses esp.vld.l.64.ip and esp.vld.h.64.ip, or
     esp.vst.l.64.ip and esp.vst.h.64.ip;
   - for any other source, aligned 16-byte blocks are loaded and each
     chunk is extracted from two consecutive blocks by esp.src.q, with
     SAR_BYTE holding the misalignment.  Blocks that hold no source byte
     are never read;
   - any other destination is first brought to 16-byte alignment with
     scalar moves.

   The bytes that do not fill a chunk are moved with scalar instructions,
   or esp.vld.l.64.ip and esp.vst.l.64.ip for 8 of them when possible.  */

struct riscv_xespv_copy
{
  /* The source and destination pointers, in pseudos that the q-register
     loads and stores advance.  For a misaligned source, SRC points to the
     next aligned block to load and PREV holds the block before it.  */
  rtx src, dst, prev;

  /* For a misaligned source, the misalignment of the original pointer,
     which is also in SAR_BYTE.  */
  rtx shift;

  /* The alignment of SRC and DST in bytes, at most 16.  A source alignment
     below 8 means that the source is copied as misaligned.  */
  unsigned int salign, dalign;

  /* The original memory references, for the scalar moves.  */
  rtx src_mem, dst_mem;
};

/* Return true if block moves should use XESPV q registers.  SPEED_P is
   true if the move is optimized for speed.  */

bool
riscv_xespv_memcpy_p (bool speed_p)
{
  return (riscv_xespv_memcpy
	  && TARGET_XESPV_QREGS
	  && (stringop_strategy & STRATEGY_VECTOR)
	  && optimize >= 2
	  && speed_p);
}

/* Emit REG = REG OP VAL.  */

static void
riscv_xespv_update (rtx reg, rtx_code op, rtx val)
{
  rtx res = expand_simple_binop (Pmode, op, reg, val, reg, 0, OPTAB_WIDEN);
  if (res != reg)
    emit_move_insn (reg, res);
}

/* Jump to LABEL if REG & MASK is zero.  */

static void
riscv_xespv_jump_if_clear (rtx reg, HOST_WIDE_INT mask, rtx_code_label *label)
{
  rtx bits = expand_simple_binop (Pmode, AND, reg, GEN_INT (mask), NULL_RTX,
				  1, OPTAB_WIDEN);
  emit_cmp_and_jump_insns (bits, const0_rtx, EQ, NULL_RTX, Pmode, 1, label);
}

/* Load the aligned 16-byte block at PTR into Q and advance PTR by INC.  */

static void
riscv_xespv_load_block (rtx q, rtx ptr, int inc)
{
  emit_insn (gen_xespv_vld128_ip (q, ptr, GEN_INT (inc)));
}

/* Load the next chunk from an aligned source.  */

static rtx
riscv_xespv_load_chunk (riscv_xespv_copy &c)
{
  rtx q = gen_reg_rtx (TImode);
  if (c.salign >= 16)
    riscv_xespv_load_block (q, c.src, 16);
  else
    {
      emit_insn (gen_xespv_vld_l64_ip (q, c.src, GEN_INT (8)));
      emit_insn (gen_xespv_vld_h64_ip (q, c.src, GEN_INT (8), q));
    }
  return q;
}

/* Store chunk Q to the destination.  */

static void
riscv_xespv_store_chunk (riscv_xespv_copy &c, rtx q)
{
  if (c.dalign >= 16)
    emit_insn (gen_xespv_vst128_ip (q, c.dst, GEN_INT (16)));
  else
    {
      emit_insn (gen_xespv_vst_l64_ip (q, c.dst, GEN_INT (8)));
      emit_insn (gen_xespv_vst_h64_ip (q, c.dst, GEN_INT (8)));
    }
}

/* Copy N chunks, N being at most 8 for an aligned source and at most 4
   for a misaligned one.  The loads come before the stores to hide their
   latency.  For a misaligned source, KEEP_PREV says whether C.PREV must
   still be the same pseudo afterwards, as in a loop.  */

static void
riscv_xespv_copy_group (riscv_xespv_copy &c, unsigned int n, bool keep_prev)
{
  rtx chunks[8];

  if (c.salign >= 8)
    for (unsigned int i = 0; i < n; i++)
      chunks[i] = riscv_xespv_load_chunk (c);
  else
    {
      /* Chunk I comes from blocks I and I + 1, block 0 being C.PREV.
	 Block N replaces it once chunk 0 has been extracted.  */
      rtx blocks[5];
      blocks[0] = c.prev;
      for (unsigned int i = 1; i <= n; i++)
	blocks[i] = (i < n || n == 1) ? gen_reg_rtx (TImode) : c.prev;
      for (unsigned int i = 1; i < n; i++)
	riscv_xespv_load_block (blocks[i], c.src, 16);
      if (n == 1)
	riscv_xespv_load_block (blocks[1], c.src, 16);
      for (unsigned int i = 0; i < n; i++)
	{
	  chunks[i] = gen_reg_rtx (TImode);
	  emit_insn (gen_xespv_src_q (chunks[i], blocks[i], blocks[i + 1]));
	  if (i == 0 && n > 1)
	    riscv_xespv_load_block (c.prev, c.src, 16);
	}
      if (n == 1)
	{
	  if (keep_prev)
	    emit_move_insn (c.prev, blocks[1]);
	  else
	    c.prev = blocks[1];
	}
    }

  for (unsigned int i = 0; i < n; i++)
    riscv_xespv_store_chunk (c, chunks[i]);
}

/* Prepare to copy from a misaligned source: set SAR_BYTE and load the
   first block.  */

static void
riscv_xespv_start_shifted (riscv_xespv_copy &c)
{
  c.shift = force_reg (Pmode, expand_simple_binop (Pmode, AND, c.src,
						   GEN_INT (15), NULL_RTX, 1,
						   OPTAB_WIDEN));
  emit_insn (gen_xespv_movx_w_sar_bytes (c.shift));
  riscv_xespv_update (c.src, AND, GEN_INT (-16));
  c.prev = gen_reg_rtx (TImode);
  riscv_xespv_load_block (c.prev, c.src, 16);
}

/* Copy the last chunk from a misaligned source and point C.SRC at the
   source byte after it.  The second block of the chunk is the one that
   holds its last byte, which is C.PREV again if the source turns out to
   be aligned: the block after C.PREV may hold no source byte.  */

static void
riscv_xespv_finish_shifted (riscv_xespv_copy &c)
{
  riscv_xespv_update (c.src, PLUS, c.shift);
  rtx addr = expand_simple_binop (Pmode, PLUS, c.src, constm1_rtx, NULL_RTX,
				  0, OPTAB_WIDEN);
  addr = expand_simple_binop (Pmode, AND, addr, GEN_INT (-16), NULL_RTX, 0,
			      OPTAB_WIDEN);
  addr = copy_to_mode_reg (Pmode, addr);
  rtx next = gen_reg_rtx (TImode);
  riscv_xespv_load_block (next, addr, 0);
  rtx chunk = gen_reg_rtx (TImode);
  emit_insn (gen_xespv_src_q (chunk, c.prev, next));
  riscv_xespv_store_chunk (c, chunk);
}

/* Emit a loop that runs riscv_xespv_copy_group (C, GROUP, true) as many
   times as it takes for C.DST to reach END.  END must differ from C.DST
   on entry.  */

static void
riscv_xespv_copy_loop (riscv_xespv_copy &c, unsigned int group, rtx end)
{
  rtx_code_label *label = gen_label_rtx ();
  emit_label (label);
  riscv_xespv_copy_group (c, group, true);
  emit_cmp_and_jump_insns (c.dst, end, NE, NULL_RTX, Pmode, 1, label);
}

/* Copy N chunks, N being a positive constant.  */

static void
riscv_xespv_copy_chunks_const (riscv_xespv_copy &c, unsigned HOST_WIDE_INT n)
{
  bool shifted = c.salign < 8;
  unsigned int group = shifted ? 4 : 8;

  if (shifted)
    {
      riscv_xespv_start_shifted (c);
      n--;
    }

  if (n > 8)
    {
      unsigned HOST_WIDE_INT bytes = n / group * group * 16;
      rtx end = expand_simple_binop (Pmode, PLUS, c.dst, GEN_INT (bytes),
				     NULL_RTX, 0, OPTAB_WIDEN);
      riscv_xespv_copy_loop (c, group, force_reg (Pmode, end));
      n %= group;
    }
  while (n)
    {
      unsigned int k = MIN (n, group);
      riscv_xespv_copy_group (c, k, false);
      n -= k;
    }

  if (shifted)
    riscv_xespv_finish_shifted (c);
}

/* Copy N chunks, N being a positive value in a register.  */

static void
riscv_xespv_copy_chunks_var (riscv_xespv_copy &c, rtx n)
{
  bool shifted = c.salign < 8;
  unsigned int group = shifted ? 4 : 8;
  int log2_group = shifted ? 2 : 3;

  if (shifted)
    {
      riscv_xespv_start_shifted (c);
      n = expand_simple_binop (Pmode, PLUS, n, constm1_rtx, NULL_RTX, 0,
			       OPTAB_WIDEN);
    }

  rtx_code_label *skip = gen_label_rtx ();
  rtx bytes = expand_simple_binop (Pmode, AND, n, GEN_INT (-group), NULL_RTX,
				   1, OPTAB_WIDEN);
  bytes = expand_simple_binop (Pmode, ASHIFT, bytes, GEN_INT (4), NULL_RTX,
			       1, OPTAB_WIDEN);
  emit_cmp_and_jump_insns (bytes, const0_rtx, EQ, NULL_RTX, Pmode, 1, skip);
  rtx end = expand_simple_binop (Pmode, PLUS, c.dst, bytes, NULL_RTX, 0,
				 OPTAB_WIDEN);
  riscv_xespv_copy_loop (c, group, force_reg (Pmode, end));
  emit_label (skip);

  for (int bit = log2_group - 1; bit >= 0; bit--)
    {
      skip = gen_label_rtx ();
      riscv_xespv_jump_if_clear (n, HOST_WIDE_INT_1 << bit, skip);
      riscv_xespv_copy_group (c, 1 << bit, true);
      emit_label (skip);
    }

  if (shifted)
    riscv_xespv_finish_shifted (c);
}

/* Return a BLKmode reference to the memory at ADDR, based on MEM and with
   alignment ALIGN in bytes.  */

static rtx
riscv_xespv_mem (rtx mem, rtx addr, unsigned int align)
{
  mem = change_address (mem, BLKmode, addr);
  set_mem_align (mem, align * BITS_PER_UNIT);
  return mem;
}

/* Copy SIZE bytes from C.SRC to C.DST with scalar moves, assuming ALIGN
   bytes of alignment, and advance both pointers if ADVANCE.  */

static void
riscv_xespv_copy_scalar (riscv_xespv_copy &c, unsigned HOST_WIDE_INT size,
			 unsigned int align, bool advance)
{
  move_by_pieces (riscv_xespv_mem (c.dst_mem, c.dst, align),
		  riscv_xespv_mem (c.src_mem, c.src, align), size,
		  align * BITS_PER_UNIT, RETURN_BEGIN);
  if (advance)
    {
      riscv_xespv_update (c.src, PLUS, GEN_INT (size));
      riscv_xespv_update (c.dst, PLUS, GEN_INT (size));
    }
}

/* Copy 8 bytes from C.SRC to C.DST, which are 8-byte aligned, through a
   q register.  */

static void
riscv_xespv_copy_8 (riscv_xespv_copy &c)
{
  rtx q = gen_reg_rtx (TImode);
  emit_insn (gen_xespv_vld_l64_ip (q, c.src, GEN_INT (8)));
  emit_insn (gen_xespv_vst_l64_ip (q, c.dst, GEN_INT (8)));
}

/* Copy the last LEN bytes, LEN being a constant below 16.  */

static void
riscv_xespv_copy_tail_const (riscv_xespv_copy &c, unsigned HOST_WIDE_INT len)
{
  unsigned int align = MIN (c.salign, c.dalign);
  if (len >= 8 && align >= 8)
    {
      riscv_xespv_copy_8 (c);
      len -= 8;
      align = 8;
    }
  if (len)
    riscv_xespv_copy_scalar (c, len, align, false);
}

/* Copy the last LEN bytes, LEN being a value below 2 * MAX in a register.  */

static void
riscv_xespv_copy_tail_var (riscv_xespv_copy &c, rtx len, unsigned int max)
{
  for (unsigned int size = max; size; size /= 2)
    {
      rtx_code_label *skip = gen_label_rtx ();
      riscv_xespv_jump_if_clear (len, size, skip);
      unsigned int align = MIN (MIN (c.salign, c.dalign), size);
      if (size == 8 && align == 8)
	riscv_xespv_copy_8 (c);
      else
	riscv_xespv_copy_scalar (c, size, align, size > 1);
      emit_label (skip);
    }
}

/* Copy the bytes before the first 16-byte boundary of C.DST, which may be
   all of them if the copy is short.  Return the number of bytes copied.  */

static rtx
riscv_xespv_align_dst (riscv_xespv_copy &c)
{
  rtx head = expand_unop (Pmode, neg_optab, c.dst, NULL_RTX, 0);
  head = force_reg (Pmode, expand_simple_binop (Pmode, AND, head,
						GEN_INT (15), NULL_RTX, 1,
						OPTAB_WIDEN));
  for (unsigned int size = c.dalign; size < 16; size *= 2)
    {
      rtx_code_label *skip = gen_label_rtx ();
      riscv_xespv_jump_if_clear (head, size, skip);
      riscv_xespv_copy_scalar (c, size, 1, true);
      emit_label (skip);
    }
  return head;
}

/* Expand a block move of LENGTH bytes from SRC to DEST with XESPV q
   registers.  Return false if that is not possible or worthwhile.  */

bool
riscv_expand_block_move_xespv (rtx dest, rtx src, rtx length)
{
  if (!riscv_xespv_memcpy_p (optimize_insn_for_speed_p ())
      || MEM_VOLATILE_P (dest)
      || MEM_VOLATILE_P (src))
    return false;

  riscv_xespv_copy c;
  c.salign = MIN (MEM_ALIGN (src), 128) / BITS_PER_UNIT;
  c.dalign = MIN (MEM_ALIGN (dest), 128) / BITS_PER_UNIT;

  /* Aligning the destination at run time and shifting a misaligned source
     only pay off for longer copies, even more so when scalar misaligned
     accesses are fast.  */
  unsigned HOST_WIDE_INT min_length = 16;
  if (c.dalign < 8)
    min_length = riscv_slow_unaligned_access_p ? 32 : 64;
  else if (c.salign < 8 && !riscv_slow_unaligned_access_p)
    min_length = 32;
  if (CONST_INT_P (length) && UINTVAL (length) < min_length)
    return false;

  c.src = copy_addr_to_reg (XEXP (src, 0));
  c.dst = copy_addr_to_reg (XEXP (dest, 0));
  c.src_mem = src;
  c.dst_mem = dest;

  if (CONST_INT_P (length) && c.dalign >= 8)
    {
      unsigned HOST_WIDE_INT len = UINTVAL (length);
      riscv_xespv_copy_chunks_const (c, len / 16);
      riscv_xespv_copy_tail_const (c, len % 16);
      return true;
    }

  /* The tail is shared with short copies, which have not aligned the
     destination.  */
  riscv_xespv_copy tail = c;
  if (c.dalign < 8)
    tail.salign = 1;

  rtx len = copy_to_mode_reg (Pmode, length);
  rtx_code_label *tail_label = gen_label_rtx ();
  if (!CONST_INT_P (length))
    emit_cmp_and_jump_insns (len, GEN_INT (min_length), LTU, NULL_RTX, Pmode,
			     1, tail_label);
  if (c.dalign < 8)
    {
      riscv_xespv_update (len, MINUS, riscv_xespv_align_dst (c));
      c.dalign = 16;
      c.salign = 1;
    }
  rtx n = expand_simple_binop (Pmode, LSHIFTRT, len, GEN_INT (4), NULL_RTX,
			       1, OPTAB_WIDEN);
  riscv_xespv_copy_chunks_var (c, n);
  riscv_xespv_update (len, AND, GEN_INT (15));
  emit_label (tail_label);
  riscv_xespv_copy_tail_var (tail, len, min_length / 2);
  return true;
}

/* User-facing q-register operations.  Names and argument order follow
   LLVM's __builtin_riscv_esp_* intrinsics.  */

enum riscv_xespv_builtin
{
  XESPV_BUILTIN_ANDQ,
  XESPV_BUILTIN_NOTQ,
  XESPV_BUILTIN_ORQ,
  XESPV_BUILTIN_XORQ,
  XESPV_BUILTIN_ZERO_Q,
  XESPV_BUILTIN_SRC_Q,
  XESPV_BUILTIN_MOVI_8_A,
  XESPV_BUILTIN_MOVI_32_A,
  XESPV_BUILTIN_VLD_128_IP,
  XESPV_BUILTIN_VST_128_IP,
  XESPV_BUILTIN_MAX
};

#define RISCV_XESPV_BUILTIN_BASE 0x1000

static tree xespv_v16qi_type;
static tree xespv_v4si_type;
static tree xespv_builtin_decls[XESPV_BUILTIN_MAX];

bool
riscv_xespv_builtin_code_p (unsigned int subcode)
{
  return (subcode >= RISCV_XESPV_BUILTIN_BASE
	  && subcode < RISCV_XESPV_BUILTIN_BASE + XESPV_BUILTIN_MAX);
}

tree
riscv_xespv_builtin_decl (unsigned int subcode)
{
  gcc_assert (riscv_xespv_builtin_code_p (subcode));
  tree decl = xespv_builtin_decls[subcode - RISCV_XESPV_BUILTIN_BASE];
  return decl ? decl : error_mark_node;
}

static tree
riscv_xespv_add_builtin (unsigned int id, const char *name, tree type)
{
  unsigned int code
    = ((RISCV_XESPV_BUILTIN_BASE + id) << RISCV_BUILTIN_SHIFT)
      | RISCV_BUILTIN_GENERAL;
  tree decl = add_builtin_function (name, type, code, BUILT_IN_MD, NULL, NULL);
  xespv_builtin_decls[id] = decl;
  return decl;
}

void
riscv_xespv_init_builtins (void)
{
  if (!TARGET_XESPV_QREGS)
    return;

  xespv_v16qi_type = build_vector_type (char_type_node, 16);
  xespv_v4si_type = build_vector_type (integer_type_node, 4);
  if (TYPE_MODE (xespv_v16qi_type) != TImode
      || TYPE_MODE (xespv_v4si_type) != TImode)
    return;

  tree v16 = xespv_v16qi_type;
  tree v4 = xespv_v4si_type;
  tree ptr = ptr_type_node;
  tree si = integer_type_node;
  tree usi = unsigned_type_node;

  riscv_xespv_add_builtin (XESPV_BUILTIN_ANDQ, "__builtin_riscv_esp_andq",
			   build_function_type_list (v16, v16, v16, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_NOTQ, "__builtin_riscv_esp_notq",
			   build_function_type_list (v16, v16, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_ORQ, "__builtin_riscv_esp_orq",
			   build_function_type_list (v16, v16, v16, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_XORQ, "__builtin_riscv_esp_xorq",
			   build_function_type_list (v16, v16, v16, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_ZERO_Q, "__builtin_riscv_esp_zero_q",
			   build_function_type_list (v16, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_SRC_Q, "__builtin_riscv_esp_src_q",
			   build_function_type_list (v16, v16, v16, usi,
						     NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_MOVI_8_A,
			   "__builtin_riscv_esp_movi_8_a",
			   build_function_type_list (si, v16, si, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_MOVI_32_A,
			   "__builtin_riscv_esp_movi_32_a",
			   build_function_type_list (si, v4, si, NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_VLD_128_IP,
			   "__builtin_riscv_esp_vld_128_ip",
			   build_function_type_list (ptr, ptr, si, ptr,
						     NULL_TREE));
  riscv_xespv_add_builtin (XESPV_BUILTIN_VST_128_IP,
			   "__builtin_riscv_esp_vst_128_ip",
			   build_function_type_list (ptr, v16, ptr, si,
						     NULL_TREE));
}

static rtx
riscv_xespv_builtin_target (rtx target, machine_mode mode)
{
  if (target && REG_P (target) && GET_MODE (target) == mode)
    return target;
  return gen_reg_rtx (mode);
}

static rtx
riscv_xespv_force_ti (rtx x)
{
  if (!REG_P (x) || GET_MODE (x) != TImode)
    x = force_reg (TImode, x);
  return x;
}

static rtx
riscv_xespv_copy_ptr (tree arg)
{
  rtx ptr = expand_normal (arg);
  if (!REG_P (ptr) || GET_MODE (ptr) != SImode)
    ptr = force_reg (SImode, ptr);
  rtx copy = gen_reg_rtx (SImode);
  emit_move_insn (copy, ptr);
  return copy;
}

static bool
riscv_xespv_ip_offset_tree (tree arg, HOST_WIDE_INT *val, int size)
{
  if (!tree_fits_shwi_p (arg))
    {
      error ("xespv post-increment must be an integer constant");
      return false;
    }

  HOST_WIDE_INT offset = tree_to_shwi (arg);
  if (!riscv_xespv_ip_offset_p (GEN_INT (offset), size))
    {
      error ("xespv post-increment is out of range");
      return false;
    }

  *val = offset;
  return true;
}

static bool
riscv_xespv_element_index (tree arg, HOST_WIDE_INT *val, HOST_WIDE_INT max)
{
  if (!tree_fits_shwi_p (arg))
    {
      error ("xespv element index must be an integer constant");
      return false;
    }

  HOST_WIDE_INT index = tree_to_shwi (arg);
  if (!IN_RANGE (index, 0, max))
    {
      error ("xespv element index is out of range");
      return false;
    }

  *val = index;
  return true;
}

static void
riscv_xespv_store_through (tree out, rtx val)
{
  rtx addr = expand_normal (out);
  if (!REG_P (addr) || GET_MODE (addr) != Pmode)
    addr = force_reg (Pmode, addr);

  rtx mem = gen_rtx_MEM (TImode, addr);
  if (TREE_CODE (out) == ADDR_EXPR)
    set_mem_attributes (mem, TREE_OPERAND (out, 0), 1);
  else
    set_mem_align (mem, 128);
  emit_move_insn (mem, val);
}

static rtx
riscv_xespv_expand_qop (rtx target, tree exp, int nargs, enum insn_code icode)
{
  rtx ops[3];
  ops[0] = riscv_xespv_builtin_target (target, TImode);
  gcc_assert (nargs <= 2);
  for (int i = 0; i < nargs; i++)
    ops[i + 1] = riscv_xespv_force_ti (expand_normal (CALL_EXPR_ARG (exp, i)));

  if (nargs == 0)
    emit_insn (GEN_FCN (icode) (ops[0]));
  else if (nargs == 1)
    emit_insn (GEN_FCN (icode) (ops[0], ops[1]));
  else
    emit_insn (GEN_FCN (icode) (ops[0], ops[1], ops[2]));
  return ops[0];
}

static rtx
riscv_xespv_expand_movi (rtx target, tree exp, HOST_WIDE_INT max,
			 enum insn_code icode)
{
  HOST_WIDE_INT index;
  if (!riscv_xespv_element_index (CALL_EXPR_ARG (exp, 1), &index, max))
    return const0_rtx;

  rtx dest = riscv_xespv_builtin_target (target, SImode);
  rtx vec = riscv_xespv_force_ti (expand_normal (CALL_EXPR_ARG (exp, 0)));
  emit_insn (GEN_FCN (icode) (dest, vec, GEN_INT (index)));
  return dest;
}

rtx
riscv_xespv_expand_builtin (unsigned int subcode, tree exp, rtx target)
{
  gcc_assert (riscv_xespv_builtin_code_p (subcode));
  unsigned int id = subcode - RISCV_XESPV_BUILTIN_BASE;

  switch (id)
    {
    case XESPV_BUILTIN_ANDQ:
      return riscv_xespv_expand_qop (target, exp, 2, CODE_FOR_xespv_andq);
    case XESPV_BUILTIN_NOTQ:
      return riscv_xespv_expand_qop (target, exp, 1, CODE_FOR_xespv_notq);
    case XESPV_BUILTIN_ORQ:
      return riscv_xespv_expand_qop (target, exp, 2, CODE_FOR_xespv_orq);
    case XESPV_BUILTIN_XORQ:
      return riscv_xespv_expand_qop (target, exp, 2, CODE_FOR_xespv_xorq);
    case XESPV_BUILTIN_ZERO_Q:
      return riscv_xespv_expand_qop (target, exp, 0, CODE_FOR_xespv_zero_q);

    case XESPV_BUILTIN_SRC_Q:
      {
	rtx dest = riscv_xespv_builtin_target (target, TImode);
	rtx a = riscv_xespv_force_ti (expand_normal (CALL_EXPR_ARG (exp, 0)));
	rtx b = riscv_xespv_force_ti (expand_normal (CALL_EXPR_ARG (exp, 1)));
	rtx sar = expand_normal (CALL_EXPR_ARG (exp, 2));
	if (!REG_P (sar) || GET_MODE (sar) != SImode)
	  sar = force_reg (SImode, sar);
	emit_insn (gen_xespv_movx_w_sar_bytes (sar));
	emit_insn (gen_xespv_src_q (dest, a, b));
	return dest;
      }

    case XESPV_BUILTIN_MOVI_8_A:
      return riscv_xespv_expand_movi (target, exp, 15, CODE_FOR_xespv_movi_8_a);
    case XESPV_BUILTIN_MOVI_32_A:
      return riscv_xespv_expand_movi (target, exp, 3, CODE_FOR_xespv_movi_32_a);

    case XESPV_BUILTIN_VLD_128_IP:
      {
	HOST_WIDE_INT offset;
	if (!riscv_xespv_ip_offset_tree (CALL_EXPR_ARG (exp, 1), &offset, 16))
	  return const0_rtx;
	rtx vec = gen_reg_rtx (TImode);
	rtx ptr = riscv_xespv_copy_ptr (CALL_EXPR_ARG (exp, 0));
	emit_insn (gen_xespv_vld128_ip (vec, ptr, GEN_INT (offset)));
	riscv_xespv_store_through (CALL_EXPR_ARG (exp, 2), vec);
	return ptr;
      }

    case XESPV_BUILTIN_VST_128_IP:
      {
	HOST_WIDE_INT offset;
	if (!riscv_xespv_ip_offset_tree (CALL_EXPR_ARG (exp, 2), &offset, 16))
	  return const0_rtx;
	rtx vec = riscv_xespv_force_ti (expand_normal (CALL_EXPR_ARG (exp, 0)));
	rtx ptr = riscv_xespv_copy_ptr (CALL_EXPR_ARG (exp, 1));
	emit_insn (gen_xespv_vst128_ip (vec, ptr, GEN_INT (offset)));
	return ptr;
      }

    default:
      gcc_unreachable ();
    }
}
