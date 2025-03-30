/* Copyright (C) 2024 Free Software Foundation, Inc.
   Contributed by Alibaba Damo Academy Xuantie Team.

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

#define IN_TARGET_CODE 1

#include "rtl.h"
#include "sched-int.h"
#include "xuantie-tune.def"
#include "opts.h"

poly_uint16 xt_rvm_chunks;

/* Types for load/store pairs.  */
enum sched_fusion_type
{
  SCHED_FUSION_NONE = 0,
  SCHED_FUSION_LD_SIGN_EXTEND,
  SCHED_FUSION_LD_ZERO_EXTEND,
  SCHED_FUSION_LD,
  SCHED_FUSION_ST,
  SCHED_FUSION_NUM
};

void
xt_option_override (const struct riscv_tune_param *tune_param,
		    struct gcc_options *opts,
		    struct gcc_options *opts_set)
{
 
  xt_rvm_chunks = TARGET_XTHEADMATRIX ? poly_uint16 (2, 0, 2)
				      : poly_uint16 (0, 0, 0);

  /* If not XUANTIE, do nothing.  */
  if (!TARGET_XUANTIE)
    return;

  if (!optimize_size)
    {
      SET_OPTION_IF_UNSET (opts, opts_set,
			   param_max_inline_insns_single,
			   400);
      SET_OPTION_IF_UNSET (opts, opts_set,
			   param_max_inline_insns_auto,
			   30);
    }

  SET_OPTION_IF_UNSET (opts, opts_set,
		       g_switch_value,
		       0);

  /* This optimization will cause the when the size of memmove/memset to
     generate is unknown in compile time, which may increase the dynamic
     instruction count when the loop is sample(like the loop is just 
     assign values to array). */
  SET_OPTION_IF_UNSET (opts, opts_set,
		       flag_tree_loop_distribute_patterns,
		       0);

  if (optimize == 3)
    SET_OPTION_IF_UNSET (opts, opts_set,
                         riscv_ipa_unhot_inline,
                         1);

  /* If not optimizing for size, set the default
     alignment to what the target wants.  */
  if (!opts->x_optimize_size)
    {
      if (opts->x_flag_align_loops && !opts->x_str_align_loops)
	opts->x_str_align_loops = tune_param->loop_align;
      if (opts->x_flag_align_jumps && !opts->x_str_align_jumps)
	opts->x_str_align_jumps = tune_param->jump_align;
      if (opts->x_flag_align_functions && !opts->x_str_align_functions)
	opts->x_str_align_functions = tune_param->function_align;
    }

  if (!opts_set->x_riscv_4k_optimize && riscv_microarchitecture == c910)
    opts->x_riscv_4k_optimize = 1;

  if (riscv_libcc_runtime && TARGET_64BIT)
    warning (0, "mccrt is only supported in rv32* arch");

  /* For XUANTIE, when Ospace is turned on, X is turned on by default.  */
  if (!opts->x_riscv_unaligned_libcall && optimize_size)
    SET_OPTION_IF_UNSET (opts, opts_set, riscv_unaligned_libcall, 1);

  if (!riscv_rvv_auto_vectorize && !OPTION_SET_P (flag_tree_vectorize))
    {
      SET_OPTION_IF_UNSET (opts, opts_set, flag_tree_loop_vectorize, 0);
      SET_OPTION_IF_UNSET (opts, opts_set, flag_tree_slp_vectorize, 0);
    }

  if (!riscv_expand_split_imm)
    SET_OPTION_IF_UNSET (opts, opts_set, riscv_expand_addsi_extend, 0);
}

/* This function implements the target macro TARGET_SCHED_ADJUST_COST.
   It corrects the value of COST based on the relationship between
   INSN and DEP through the dependence DEP_TYPE.  It returns the new
   value.  */

int
xt_sched_adjust_cost (rtx_insn *insn, int dep_type, rtx_insn *dep, int cost)
{
  /* Avoid adjust cost between INSN and JUMP_INSN or CALL_INSN,
     because every INSN has output dependency with JUMP_INSN,
     and many INSNs has output dependency with CALL_INSN,
     which will increase vector INSN's priority much higher than others.  */
  if (recog_memoized (insn) < 0 || recog_memoized (dep) < 0
      || !NONJUMP_INSN_P (insn) || !NONJUMP_INSN_P (dep))
    return cost;

  if (sched_verbose >= 9 && sched_dump)
    fprintf (sched_dump, ";;\t\tAdjust cost: insn: %4d -> %4d, dep_type: %s, "
	     "cost: %2d, latency: %2d\n", INSN_UID (dep), INSN_UID (insn),
	     reg_note_name[dep_type], cost, insn_default_latency (dep));

  return cost;
}

/* Try to use libcall to extract bit field form mem. Return nonzero if using libcall.  */

void
xt_emit_libcall_for_unaligned_access (enum riscv_extraction_pattern ep,
					 rtx *operands, machine_mode mode)
{
  gcc_assert (mode == SImode || mode == DImode);

  rtx libfunc = NULL_RTX;
  switch (ep)
    {
    case RISCV_EP_INSV:
      {
	libfunc = init_one_libfunc (concat ("__thead_uwrite", (mode == SImode ? "4" : "8"), NULL));
	emit_library_call (libfunc, LCT_NORMAL, mode,
			   operands[3], mode,
			   XEXP (operands[0], 0), Pmode);
	emit_clobber (operands[0]);
	return;
      }
    case RISCV_EP_EXTV:
    case RISCV_EP_EXTZV:
      {
	libfunc = init_one_libfunc (concat ("__thead_uread", (mode == SImode ? "4" : "8"), NULL));
	rtx target = emit_library_call_value (libfunc, operands[0], LCT_PURE, mode,
					      XEXP (operands[1], 0), Pmode);
	emit_use (operands[1]);
	if (!rtx_equal_p (operands[0], target))
	  emit_move_insn (operands[0], target);

	return;
      }
    default:
      gcc_unreachable ();
    }
}

/* Determine whether the XTheadFmemidx move is supported.
   Function return values:
    - 1: Utilize XTheadFmemidx move
    - 0: Perform a standard move operation
    - -1: Encounter an illegal instruction, indicating an invalid operation.  */
int
xt_fmemidx_movdf_hardfloat_p (rtx dest, rtx src)
{
  /* Without M expansion, we cannot get th_m operands. */
  if (!TARGET_XTHEADFMEMIDX)
    return 0;

  if (xt_mem_operand (dest, DFmode))
    {
      /* At 32 bits, we cannot use zero as the source operand of the fsrd/flrd
	instruction because it is 32 bits.  */
      if (const_0_operand (src, DFmode))
	return TARGET_64BIT ? 1 : -1;

      /* Pseudo-registers and floating-point registers are legal.  */
      if (register_operand (src, DFmode))
	return fpr_operand (src, DFmode) ? 1 : -1;
    }
  else if (xt_mem_operand (src, DFmode))
    {
      if (register_operand (dest, DFmode))
	return fpr_operand (dest, DFmode) ? 1 : -1;
    }

  return 0;
}

/* Implement TARGET_IV_GEN_LOAD_INDEX.
   Don't generate index for induction variables for the loop when
   4k-optimize is enable and loop size is larger than 4k.  */

static bool
xt_iv_gen_load_index (HOST_WIDE_INT loop_niter,
			 machine_mode mem_mode)
{
  if (VECTOR_MODE_P (mem_mode))
    return true;

  HOST_WIDE_INT mem_size = GET_MODE_SIZE (mem_mode).to_constant();

  if (!riscv_4k_optimize)
    return true;

  if (loop_niter * mem_size > 4048)
    return false;
  return true;
}

/* Implement TARGET_PROMOTED_TYPE.  */

static tree
xt_promoted_type (const_tree t)
{
  if (SCALAR_FLOAT_TYPE_P (t)
      && TYPE_PRECISION (t) == 16
      && TYPE_MODE (t) == BFmode)
    return riscv_bfloat16_type_node;

  return NULL_TREE;
}

/* Implement TARGET_CANNOT_SUBSTITUTE_MEM_EQUIV_P.  */

static bool
xt_cannot_substitute_mem_equiv_p (rtx mem)
{
  gcc_assert (MEM_P (mem));

  /* th_mem is special. On rv32, it cannot safely replace pseudo-registers
     with equivalent memory values during register allocation, such as
     equivalent replacement between GPR and FPR, because it does no
     support memory splitting.  */
  if (!TARGET_64BIT && xt_mem_operand (mem, DFmode))
    return true;

  return false;
}

bool
xt_rtx_costs (rtx x, machine_mode mode ATTRIBUTE_UNUSED, int outer_code,
	      int opno ATTRIBUTE_UNUSED, int *total,
	      bool speed ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (x))
    {
    case XOR:
      if (TARGET_XUANTIE_COMBINE_XOR && outer_code == SET
	  && REG_P (XEXP (x, 0)) && CONST_INT_P (XEXP (x, 1))
	  && !SMALL_OPERAND (INTVAL (XEXP (x, 1)))
	  && !(TARGET_ZBS && uimm_extra_bit_or_twobits (XEXP (x, 1), VOIDmode)))
	{
	  *total = TARGET_XUANTIE_EXPAND_SPLIT_IMM ? COSTS_N_INSNS (2) + 2
                                             : COSTS_N_INSNS (1);
	  return true;
	}
      break;
    default:
      return false;
    }
  return false;
}

/* Implement the TARGET_OPTAB_SUPPORTED_P hook.  */

static bool
xt_by_pieces_mode_supported_p (fixed_size_mode mode)
{
  if (!riscv_rvv_auto_vectorize && VECTOR_MODE_P (mode))
    return false;

  return true;
}

#include "xuantie-ext-lib.cc"
#include "xuantie-pass-dsext.cc"
#include "xuantie-pass-load-merging.cc"
#include "xuantie-pass-mcfg.cc"

#undef TARGET_IV_GEN_LOAD_INDEX
#define TARGET_IV_GEN_LOAD_INDEX xt_iv_gen_load_index

#undef TARGET_PROMOTED_TYPE
#define TARGET_PROMOTED_TYPE xt_promoted_type

#undef TARGET_CANNOT_SUBSTITUTE_MEM_EQUIV_P
#define TARGET_CANNOT_SUBSTITUTE_MEM_EQUIV_P xt_cannot_substitute_mem_equiv_p

#undef TARGET_BY_PIECES_MODE_SUPPORTED_P
#define TARGET_BY_PIECES_MODE_SUPPORTED_P xt_by_pieces_mode_supported_p
