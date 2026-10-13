/* Restrict inline asm operands of Espressif instructions.

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

/* Most Espressif RV-PIE instructions, and a few DSP ones, encode register
   operands without bit 3, so they only accept x8-x15/x24-x31 or
   f8-f15/f24-f31.  Inline asm passes such operands with plain "r" or "f".
   This pass reads the asm template, finds the operands that land in those
   fields and narrows their constraints to the encodable registers, so the
   register allocator never picks one the assembler would reject.  */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "memmodel.h"
#include "tm_p.h"
#include "insn-config.h"
#include "emit-rtl.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "tree-pass.h"

namespace {

struct esp_asm_insn
{
  const char *name;
  /* Operand kinds, as described in riscv-esp-asm.def.  */
  const char *operands;
};

const esp_asm_insn esp_asm_insns[] = {
#define ESP_ASM_INSN(NAME, OPERANDS) { NAME, OPERANDS },
#include "riscv-esp-asm.def"
#undef ESP_ASM_INSN
};

/* Return the entry for the LEN-character mnemonic NAME, or null.  */

const esp_asm_insn *
esp_asm_lookup (const char *name, size_t len)
{
  char buf[64];
  if (len >= sizeof buf)
    return NULL;
  for (size_t i = 0; i < len; i++)
    buf[i] = TOLOWER (name[i]);
  buf[len] = '\0';

  size_t lo = 0, hi = ARRAY_SIZE (esp_asm_insns);
  while (lo < hi)
    {
      size_t mid = (lo + hi) / 2;
      int cmp = strcmp (buf, esp_asm_insns[mid].name);
      if (cmp == 0)
	return &esp_asm_insns[mid];
      if (cmp < 0)
	hi = mid;
      else
	lo = mid + 1;
    }
  return NULL;
}

/* If [P, END) is a lone operand reference such as "%1" or "%z1", return
   its operand number, otherwise return -1.  */

int
esp_asm_operand_ref (const char *p, const char *end)
{
  while (p < end && ISSPACE (*p))
    p++;
  while (end > p && ISSPACE (end[-1]))
    end--;
  if (p == end || *p++ != '%')
    return -1;
  if (p < end && ISALPHA (*p))
    p++;
  if (p == end || !ISDIGIT (*p))
    return -1;
  int n = 0;
  while (p < end && ISDIGIT (*p))
    {
      n = n * 10 + (*p++ - '0');
      if (n >= MAX_RECOG_OPERANDS)
	return -1;
    }
  return p == end ? n : -1;
}

/* Return CONSTRAINT with every "r" or "g" narrowed to the GPRs Espressif
   instructions encode and, for an FPR field (KIND 'F'), every "f" narrowed
   to the encodable FPRs.  */

const char *
esp_asm_narrow_constraint (const char *constraint, char kind)
{
  auto_vec<char, 32> buf;
  bool changed = false;
  for (const char *p = constraint; *p; )
    {
      int len = CONSTRAINT_LEN (*p, p);
      const char *repl = NULL;
      if (len == 1 && (*p == 'r' || *p == 'g'))
	repl = "er";
      else if (len == 1 && *p == 'f' && kind == 'F')
	repl = "ef";
      if (repl)
	{
	  buf.safe_push (repl[0]);
	  buf.safe_push (repl[1]);
	  changed = true;
	}
      else
	for (int i = 0; i < len && p[i]; i++)
	  buf.safe_push (p[i]);
      while (len-- > 0 && *p)
	p++;
    }
  if (!changed)
    return constraint;
  return ggc_alloc_string (buf.address (), buf.length ());
}

/* An asm insn whose operands are being narrowed.  */

class esp_asm
{
public:
  esp_asm (rtx body, rtx asmop);
  void scan (const char *templ);

private:
  void scan_statement (const char *p, const char *end);
  void narrow (int opno, char kind, const char *mnemonic);

  rtx m_body;
  rtx m_asmop;
  int m_noutputs;
  int m_ninputs;
  /* Operands already narrowed or diagnosed.  */
  unsigned HOST_WIDE_INT m_done;
};

esp_asm::esp_asm (rtx body, rtx asmop)
  : m_body (body), m_asmop (asmop), m_noutputs (0),
    m_ninputs (ASM_OPERANDS_INPUT_LENGTH (asmop)), m_done (0)
{
  if (GET_CODE (body) == SET)
    m_noutputs = 1;
  else if (GET_CODE (body) == PARALLEL)
    while (m_noutputs < XVECLEN (body, 0)
	   && GET_CODE (XVECEXP (body, 0, m_noutputs)) == SET
	   && (GET_CODE (SET_SRC (XVECEXP (body, 0, m_noutputs)))
	       == ASM_OPERANDS))
      m_noutputs++;
}

/* Narrow asm operand OPNO, used as a KIND operand of MNEMONIC.  */

void
esp_asm::narrow (int opno, char kind, const char *mnemonic)
{
  if (opno >= m_noutputs + m_ninputs)
    return;

  /* An input tied to an output gets its register from the output.  */
  if (opno >= m_noutputs)
    {
      const char *c = ASM_OPERANDS_INPUT_CONSTRAINT (m_asmop,
						     opno - m_noutputs);
      char *end;
      unsigned long match = strtoul (c, &end, 10);
      if (ISDIGIT (*c) && *end == '\0' && match < (unsigned) m_noutputs)
	opno = match;
    }

  if (m_done & (HOST_WIDE_INT_1U << opno))
    return;
  m_done |= HOST_WIDE_INT_1U << opno;

  rtx op;
  if (opno < m_noutputs)
    {
      rtx set = GET_CODE (m_body) == SET ? m_body : XVECEXP (m_body, 0, opno);
      op = SET_DEST (set);
      ASM_OPERANDS_OUTPUT_CONSTRAINT (SET_SRC (set))
	= esp_asm_narrow_constraint
	    (ASM_OPERANDS_OUTPUT_CONSTRAINT (SET_SRC (set)), kind);
    }
  else
    {
      int i = opno - m_noutputs;
      op = ASM_OPERANDS_INPUT (m_asmop, i);
      ASM_OPERANDS_INPUT_CONSTRAINT (m_asmop, i)
	= esp_asm_narrow_constraint (ASM_OPERANDS_INPUT_CONSTRAINT (m_asmop, i),
				     kind);
    }

  /* A register variable keeps its register, so report one that the
     instruction cannot encode.  */
  if (SUBREG_P (op))
    op = SUBREG_REG (op);
  if (!REG_P (op) || !HARD_REGISTER_P (op))
    return;
  unsigned int regno = REGNO (op);
  location_t loc = ASM_OPERANDS_SOURCE_LOCATION (m_asmop);
  if (kind == 'F' && FP_REG_P (regno))
    {
      if (!((regno - FP_REG_FIRST) & 8))
	error_at (loc, "operand %d of %qs must be one of f8-f15 or f24-f31, "
		  "not %qs", opno, mnemonic, reg_names[regno]);
    }
  else if (!ESP_GR_REG_P (regno))
    error_at (loc, "operand %d of %qs must be one of x8-x15 or x24-x31, "
	      "not %qs", opno, mnemonic, reg_names[regno]);
}

/* Narrow the operands used by the instruction in [P, END).  */

void
esp_asm::scan_statement (const char *p, const char *end)
{
  /* Skip labels.  */
  const char *word;
  for (;;)
    {
      while (p < end && ISSPACE (*p))
	p++;
      word = p;
      while (p < end && !ISSPACE (*p) && *p != ':')
	p++;
      if (p == end || *p != ':')
	break;
      p++;
    }

  const esp_asm_insn *insn = esp_asm_lookup (word, p - word);
  if (!insn)
    return;

  for (const char *kind = insn->operands; *kind && p < end; kind++)
    {
      const char *start = p;
      while (p < end && *p != ',')
	p++;
      if (*kind == 'R' || *kind == 'F')
	{
	  int opno = esp_asm_operand_ref (start, p);
	  if (opno >= 0)
	    narrow (opno, *kind, insn->name);
	}
      if (p < end)
	p++;
    }
}

/* Narrow the operands used by the instructions in TEMPL.  */

void
esp_asm::scan (const char *templ)
{
  const char *p = templ;
  while (*p)
    {
      const char *end = p;
      while (*end && *end != '\n' && *end != ';' && *end != '#')
	end++;
      scan_statement (p, end);
      if (*end == '#')
	while (*end && *end != '\n')
	  end++;
      p = *end ? end + 1 : end;
    }
}

const pass_data pass_data_riscv_esp_asm =
{
  RTL_PASS, /* type */
  "riscv_esp_asm", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_riscv_esp_asm : public rtl_opt_pass
{
public:
  pass_riscv_esp_asm (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_riscv_esp_asm, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) final override
  {
    return TARGET_XESPV || TARGET_XESPDSP;
  }

  unsigned int execute (function *) final override;
};

unsigned int
pass_riscv_esp_asm::execute (function *)
{
  for (rtx_insn *insn = get_insns (); insn; insn = NEXT_INSN (insn))
    if (NONDEBUG_INSN_P (insn) && asm_noperands (PATTERN (insn)) > 0)
      {
	rtx body = PATTERN (insn);
	rtx asmop = extract_asm_operands (body);
	esp_asm (body, asmop).scan (ASM_OPERANDS_TEMPLATE (asmop));
      }
  return 0;
}

} // anon namespace

rtl_opt_pass *
make_pass_riscv_esp_asm (gcc::context *ctxt)
{
  return new pass_riscv_esp_asm (ctxt);
}
