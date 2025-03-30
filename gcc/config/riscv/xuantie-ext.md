;; Machine description for XUANTIE vendor extensions
;; Copyright (C) 2024 Free Software Foundation, Inc.
;; Contributed by Alibaba Damo Academy Xuantie Team.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(include "xuantie-pipeline-c906v.md")
(include "xuantie-pipeline-c910.md")
(include "xuantie-ext-predicates.md")
(include "xuantie-ext-constraints.md")
(include "xuantie-ext-p.md")
(include "xuantie-ext-matrix.md")
(include "xuantie-ext-cce.md")

(define_c_enum "unspec" [
  UNSPEC_FCLASS
  UNSPEC_FCLASS_FOR_QUIET_CMP
])

(define_c_enum "unspecv" [
  ;; CSR read
  UNSPECV_CSR_READ
  ;; CSR write
  UNSPECV_CSR_WRITE
  ;; CSR set bits
  UNSPECV_CSR_SET_BITS
  ;; CSR clear bits
  UNSPECV_CSR_CLEAR_BITS
])

(define_constants [
   (VXSAT_REGNUM		0x009)

   (MSTATUS_REGNUM		0x300)
   (MEPC_REGNUM			0x341)
   (MCAUSE_REGNUM		0x342)

   (MSTATUS_MIE_BIT		8)
])

;; C908/C907 has a special acceleration for div and mod. When a div/mod instruction
;; is given after another mod/div and they use a common divisor and divisor,
;; only 2 extra cycles are needed for this current div/mod instruction.
;; To implement this features, we treat the div and mod pair as a single
;; instruction and use the SPN "divmodm4" to cover it.

(define_code_iterator any_divmod_div [div udiv])
(define_code_attr any_divmod_mod [(div "mod") (udiv "umod")])
(define_code_attr any_divmod_u [(div "") (udiv "u")])
(define_code_attr any_divmod_extend [(div "sign_extend") (udiv "zero_extend")])

(define_insn "thead_<any_divmod_u>divmodsi4"
  [(set (match_operand:SI                    0 "register_operand" "=&r")
	(any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			   (match_operand:SI 2 "register_operand" " r")))
   (set (match_operand:SI                    3 "register_operand" "=r")
	(<any_divmod_mod>:SI (match_dup 1)
			   (match_dup 2)))]
  "TARGET_DIV && XUANTIE_TUNE_C907_OR_C908_P"
  { return TARGET_64BIT ?
    "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"
    : "div<any_divmod_u>\t%0,%1,%2\n\t" "rem<any_divmod_u>\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "SI")])

(define_insn "thead_<any_divmod_u>divmoddi4"
  [(set (match_operand:DI                    0 "register_operand" "=&r")
	(any_divmod_div:DI (match_operand:DI 1 "register_operand" " r")
			   (match_operand:DI 2 "register_operand" " r")))
   (set (match_operand:DI                    3 "register_operand" "=r")
	(<any_divmod_mod>:DI (match_dup 1)
			   (match_dup 2)))]
  "TARGET_DIV && TARGET_64BIT && XUANTIE_TUNE_C907_OR_C908_P"
  { return "div<any_divmod_u>\t%0,%1,%2\n\t" "rem<any_divmod_u>\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

(define_insn "*divmodsi4_extend1"
  [(set (match_operand:DI                        0 "register_operand" "=&r")
	(<any_divmod_extend>:DI
	    (any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			       (match_operand:SI 2 "register_operand" " r"))))
   (set (match_operand:DI                        3 "register_operand" "=r")
	(<any_divmod_extend>:DI
	    (<any_divmod_mod>:SI (match_dup 1)
				 (match_dup 2))))]
  "TARGET_DIV && TARGET_64BIT && XUANTIE_TUNE_C907_OR_C908_P"
  { return "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

(define_insn "*divmodsi4_extend2"
  [(set (match_operand:DI                        0 "register_operand" "=&r")
	(<any_divmod_extend>:DI
	    (any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			       (match_operand:SI 2 "register_operand" " r"))))
   (set (match_operand:SI                        3 "register_operand" "=r")
	(<any_divmod_mod>:SI (match_dup 1)
			     (match_dup 2)))]
  "TARGET_DIV && TARGET_64BIT && XUANTIE_TUNE_C907_OR_C908_P"
  { return "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

(define_insn "*divmodsi4_extend3"
  [(set (match_operand:SI                    0 "register_operand" "=&r")
	(any_divmod_div:SI (match_operand:SI 1 "register_operand" " r")
			   (match_operand:SI 2 "register_operand" " r")))
   (set (match_operand:DI                    3 "register_operand" "=r")
	(<any_divmod_extend>:DI
	    (<any_divmod_mod>:SI (match_dup 1)
				 (match_dup 2))))]
  "TARGET_DIV && TARGET_64BIT && XUANTIE_TUNE_C907_OR_C908_P"
  { return "div<any_divmod_u>w\t%0,%1,%2\n\t" "rem<any_divmod_u>w\t%3,%1,%2"; }
  [(set_attr "length" "8")
   (set_attr "type" "idivmod")
   (set_attr "mode" "DI")])

;; XUANTIE bugfix

(define_insn "*xt_fmemidx_movdf_hardfloat"
  [(set (match_operand:DF 0 "fpr_or_xt_mem_operand"      "=f,th_m_mir,f,th_m_miu")
       (match_operand:DF 1 "fpr_or_xt_mem_or_0_operand" " th_m_mir,fG,th_m_miu,fG"))]
  "TARGET_DOUBLE_FLOAT && TARGET_XTHEADFMEMIDX
   && TARGET_XTHEADMEMIDX && xt_fmemidx_movdf_hardfloat_p (operands[0], operands[1]) == 1"
  { return riscv_output_move (operands[0], operands[1]); }
  [(set_attr "move_type" "fpload,fpstore,fpload,fpstore")
   (set_attr "mode" "DF")
   (set (attr "length") (const_int 16))])

;; THead Optimization

;; TARGET_XUANTIE_XOR_COMBINE

(define_insn "*xt_xor<mode>3"
  [(set (match_operand:X        0 "register_operand" "=r,r")
	(xor:X (match_operand:X 1 "register_operand" "%r,r")
	       (match_operand:X 2 "xt_uns16_arith_operand" "r,I")))]
  "TARGET_XUANTIE_COMBINE_XOR
   && !(TARGET_ZBS && uimm_extra_bit_or_twobits (operands[2], VOIDmode))"
  "xor%i2\t%0,%1,%2"
  [(set_attr "type" "logical")
   (set_attr "mode" "<MODE>")])


;; Bit field extract form mem patterns.

(define_expand "extvmisalign<mode>"
  [(set (match_operand:GPR 0 "register_operand")
	(sign_extract:GPR (match_operand:BLK 1 "memory_operand")
			  (match_operand 2 "const_int_operand")
			  (match_operand 3 "const_int_operand")))]
  "TARGET_XUANTIE_UNALIGNED_ACCESS"
{
  if (INTVAL (operands[3]) == 0
      && INTVAL (operands[2]) == GET_MODE_BITSIZE (<MODE>mode))
    {
      xt_emit_libcall_for_unaligned_access (RISCV_EP_EXTV, operands, <MODE>mode);
      DONE;
    }

  FAIL;
})

(define_expand "extzvmisalign<mode>"
  [(set (match_operand:GPR 0 "register_operand")
	(zero_extract:GPR (match_operand:BLK 1 "memory_operand")
			  (match_operand 2 "const_int_operand")
			  (match_operand 3 "const_int_operand")))]
  "TARGET_XUANTIE_UNALIGNED_ACCESS"
{
  if (INTVAL (operands[3]) == 0
      && INTVAL (operands[2]) == GET_MODE_BITSIZE (<MODE>mode))
    {
      xt_emit_libcall_for_unaligned_access (RISCV_EP_EXTZV, operands, <MODE>mode);
      DONE;
    }

  FAIL;
})

(define_expand "insvmisalign<mode>"
  [(set (zero_extract:GPR (match_operand:BLK 0 "memory_operand")
			 (match_operand 1 "const_int_operand")
			 (match_operand 2 "const_int_operand"))
	(match_operand:GPR 3 "register_operand"))]
  "TARGET_XUANTIE_UNALIGNED_ACCESS"
{
  if (INTVAL (operands[2]) == 0
      && INTVAL (operands[1]) == GET_MODE_BITSIZE (<MODE>mode))
    {
      xt_emit_libcall_for_unaligned_access (RISCV_EP_INSV, operands, <MODE>mode);
      DONE;
    }

  FAIL;
})

(define_insn "@fclass<ANYF:mode><X:mode>2"
  [(set (match_operand:X	0 "register_operand" "=r")
	(unspec:X
	  [(match_operand:ANYF  1 "register_operand" " f")]
	UNSPEC_FCLASS))]
  "TARGET_HARD_FLOAT || TARGET_ZFINX"
  "fclass.<fmt>\t%0,%1"
  [(set_attr "type" "fcmp")
   (set_attr "mode" "<UNITMODE>")])

(define_insn_and_split "fquiet_cmp<ANYF:mode><X:mode>_helper"
  [(set (match_operand:X	0 "register_operand" "=r")
	(unspec:X
	  [(match_operand:ANYF  1 "reg_or_no_nan_double_operand" " f")
	   (match_operand:ANYF  2 "reg_or_no_nan_double_operand" " f")]
	UNSPEC_FCLASS_FOR_QUIET_CMP))
    (clobber (match_operand:X   3))]
  "TARGET_XUANTIE_FCLASS && (TARGET_HARD_FLOAT || TARGET_ZFINX)"
  "#"
  "&& 1"
  [(const_int 0)]
  {
    rtx op0 = operands[0];
    rtx op1 = operands[1];
    rtx op2 = operands[2];
    rtx op3 = operands[3];

    if (REG_P (op1) && REG_P (op2))
      {
	emit_insn (gen_fclass2 (<ANYF:MODE>mode, <X:MODE>mode, op0, op1));
	emit_insn (gen_fclass2 (<ANYF:MODE>mode, <X:MODE>mode, op3, op2));
	emit_insn (gen_ior<X:mode>3 (op0, op0, op3));
      }
    else if (!REG_P (op1) && REG_P (op2))
      emit_insn (gen_fclass2 (<ANYF:MODE>mode, <X:MODE>mode, op0, op2));
    else if (REG_P (op1) && !REG_P (op2))
      emit_insn (gen_fclass2 (<ANYF:MODE>mode, <X:MODE>mode, op0, op1));
    else
      emit_move_insn (op0, GEN_INT (1));

    DONE;
  }
  [(set_attr "type" "fcmp")
   (set_attr "mode" "<UNITMODE>")])

(define_insn "fquiet_cmp<ANYF:mode>_helper"
  [(unspec_volatile
	[(match_operand:ANYF 0 "reg_or_no_nan_double_operand" " f")
	 (match_operand:ANYF 1 "reg_or_no_nan_double_operand" " f")]
	UNSPEC_FCLASS_FOR_QUIET_CMP)]
  "TARGET_XUANTIE_FCLASS && (TARGET_HARD_FLOAT || TARGET_ZFINX)"
  "feq.<fmt>\tzero,%0,%1"
  [(set_attr "type" "fcmp")
   (set_attr "mode" "<UNITMODE>")])

(define_expand "f<quiet_pattern>_quiet<ANYF:mode><X:mode>3_opt"
   [(set (match_operand:X	 	0 "register_operand")
	 (unspec:X [(match_operand:ANYF 1 "register_operand")
		    (match_operand:ANYF 2 "register_operand")]
		   QUIET_COMPARISON))]
  "TARGET_XUANTIE_FCLASS && (TARGET_HARD_FLOAT || TARGET_ZFINX)"
  {
    rtx op0 = operands[0];
    rtx op1 = operands[1];
    rtx op2 = operands[2];
    rtx tmp = gen_reg_rtx (<X:MODE>mode);
    rtx label = gen_label_rtx ();

    emit_insn (gen_fquiet_cmp<ANYF:mode><X:mode>_helper (op0, op1, op2, tmp));

    if (!HONOR_SNANS (<ANYF:MODE>mode))
      riscv_emit_binary (LT, op0, op0, GEN_INT (0x100));
    else
      {
	riscv_emit_binary (LT, op0, op0, GEN_INT (0x200));
	emit_insn (gen_fquiet_cmp<ANYF:mode>_helper (op1, op2));
      }
    riscv_expand_conditional_branch (label, EQ, op0, GEN_INT (0));
    riscv_emit_binary (<QUIET_PATTERN>, op0, op1, op2);
    emit_label (label);
    DONE;
  }
)

(define_insn "xt_csr_read"
  [(unspec_volatile [
	(match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "csr_address" "")]
    UNSPECV_CSR_READ)
    (clobber (match_dup 0))]
  ""
  {
    unsigned HOST_WIDE_INT reg_num = UINTVAL (operands[1]);
    switch (reg_num)
    {
      case MSTATUS_REGNUM:
	return "csrr\t%0,mstatus";
      case MEPC_REGNUM:
	return "csrr\t%0,mepc";
      case MCAUSE_REGNUM:
	return "csrr\t%0,mcause";
      case VXSAT_REGNUM:
	return "csrr\t%0,vxsat";
      default:
	gcc_unreachable ();
    }
    gcc_unreachable ();
  }
[(set_attr "type" "fmove")])

(define_insn "xt_csr_write"
  [(unspec_volatile [(match_operand:SI 0 "csr_address" "")
		     (match_operand:SI 1 "register_operand" "r")]
		    UNSPECV_CSR_WRITE)]
  ""
  {
    unsigned HOST_WIDE_INT reg_num = UINTVAL (operands[0]);
    switch (reg_num)
    {
      case MSTATUS_REGNUM:
	return "csrw\tmstatus,%1";
      case MEPC_REGNUM:
	return "csrw\tmepc,%1";
      case MCAUSE_REGNUM:
	return "csrw\tmcause,%1";
      case VXSAT_REGNUM:
	return "csrw\tvxsat,%1";
      default:
	gcc_unreachable ();
    }
    gcc_unreachable ();
  }
[(set_attr "type" "fmove")])

(define_insn "xt_csr_set_bits"
  [(unspec_volatile [(match_operand:SI 0 "csr_address" "")
		     (match_operand:SI 1 "csr_operand" "rK")]
		    UNSPECV_CSR_SET_BITS)]
  ""
  {
    gcc_assert (UINTVAL (operands[0]) == MSTATUS_REGNUM);
    return "csrs%i1\tmstatus,%1";
  }
[(set_attr "type" "fmove")])

(define_insn "xt_csr_clear_bits"
  [(unspec_volatile [(match_operand:SI 0 "csr_address" "")
		     (match_operand:SI 1 "csr_operand" "rK")]
		    UNSPECV_CSR_CLEAR_BITS)]
  ""
  {
    gcc_assert (UINTVAL (operands[0]) == MSTATUS_REGNUM);
    return "csrc%i1\tmstatus,%1";
  }
[(set_attr "type" "fmove")])
