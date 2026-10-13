;; Machine description for the Espressif XESPV vendor extension.
;; Copyright (C) 2026 Free Software Foundation, Inc.

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

;; The q registers q0-q7 are 128 bits wide and hold TImode values.  They can
;; only be loaded from and stored to 16-byte aligned memory whose address is
;; in one of x8-x15/x24-x31.  The instructions used here have the same
;; syntax in all supported versions of the extension.

;; Espressif constraints start with "e".  There must be no single-letter
;; "e" constraint, or these could not be parsed.

(define_register_constraint "er" "ESP_GR_REGS"
  "@internal
   General purpose register usable by Espressif RV-PIE instructions
   (x8-x15, x24-x31).  riscv-esp-asm.cc substitutes it for @code{r} in
   asm operands those instructions use.")

(define_register_constraint "ef" "TARGET_HARD_FLOAT ? ESP_FP_REGS : (TARGET_ZFINX ? ESP_GR_REGS : NO_REGS)"
  "@internal
   Floating-point register usable by Espressif RV-PIE instructions (f8-f15,
   f24-f31), if available, reuse GPR as FPR when use zfinx.
   riscv-esp-asm.cc substitutes it for @code{f} in asm operands those
   instructions use.")

;; Used by compiler patterns and by the __builtin_riscv_esp_* intrinsics.
(define_register_constraint "eq" "TARGET_XESPV_QREGS ? Q_REGS : NO_REGS"
  "@internal
   An XESPV q register (q0-q7).")

(define_memory_constraint "em"
  "@internal
   A 16-byte aligned memory operand whose address is a general purpose
   register usable by Espressif RV-PIE instructions."
  (and (match_code "mem")
       (match_test "riscv_xespv_q_mem_p (op)")))

;; Upstream has no movti pattern.  If it gets one, this expander must be
;; merged into it: keep the TARGET_XESPV_QREGS case below and remove the
;; duplicate.
(define_expand "movti"
  [(set (match_operand:TI 0 "nonimmediate_operand")
	(match_operand:TI 1 "general_operand"))]
  "TARGET_XESPV_QREGS"
{
  riscv_expand_movti_xespv (operands[0], operands[1]);
  DONE;
})

;; Moves that cannot involve a q register are split into word moves before
;; register allocation, the rest after reload if they ended up not using
;; a q register.  The q-register alternatives are disparaged so that the
;; register allocator only uses q registers for values that are explicitly
;; placed there (register variables and __builtin_riscv_esp_* results):
;; using q registers behind the user's back would break code that must not
;; touch the coprocessor state, such as interrupt handlers.
(define_insn_and_split "movti_xespv"
  [(set (match_operand:TI 0 "nonimmediate_operand" "=r,r,m,r,m,?eq,?eq,?em,?eq")
	(match_operand:TI 1 "general_operand"      " r,m,r,J,J,eq,em,eq,J"))]
  "TARGET_XESPV_QREGS
   && (register_operand (operands[0], TImode)
       || reg_or_0_operand (operands[1], TImode))"
  { return riscv_output_movti_xespv (operands); }
  "&& riscv_split_movti_xespv_p (operands[0], operands[1])"
  [(const_int 0)]
{
  riscv_split_movti_xespv (operands[0], operands[1]);
  DONE;
}
  [(set_attr "type" "multi,multi,multi,multi,multi,move,load,store,move")
   (set_attr "mode" "TI")
   (set_attr "length" "16,16,16,16,16,4,4,4,4")])

;; Instructions used by the -mxespv-memcpy block move expansion.  The loads
;; and stores access memory at operand 1 and increment it by operand 2.
;; The accesses are unspecs so that dropping an unused increment does not
;; leave a movti_xespv whose memory has lost its alignment.

(define_c_enum "unspec" [
  UNSPEC_XESPV_VLD128
  UNSPEC_XESPV_VST128
  UNSPEC_XESPV_VLD_L64
  UNSPEC_XESPV_VLD_H64
  UNSPEC_XESPV_VST_L64
  UNSPEC_XESPV_VST_H64
  UNSPEC_XESPV_ANDQ
  UNSPEC_XESPV_NOTQ
  UNSPEC_XESPV_ORQ
  UNSPEC_XESPV_XORQ
  UNSPEC_XESPV_ZERO_Q
  UNSPEC_XESPV_MOVI32
  UNSPEC_XESPV_MOVI8
])

(define_c_enum "unspecv" [
  UNSPECV_XESPV_MOVX_W_SAR_BYTES
  UNSPECV_XESPV_SRC_Q
])

(define_insn "xespv_vld128_ip"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(mem:TI (match_operand:SI 1 "register_operand" "+er"))]
		   UNSPEC_XESPV_VLD128))
   (set (match_dup 1)
	(plus:SI (match_dup 1)
		 (match_operand:SI 2 "const_int_operand")))]
  "TARGET_XESPV_QREGS && riscv_xespv_ip_offset_p (operands[2], 16)"
  "esp.vld.128.ip\t%0,%1,%2"
  [(set_attr "type" "load")
   (set_attr "mode" "TI")])

(define_insn "xespv_vst128_ip"
  [(set (mem:TI (match_operand:SI 1 "register_operand" "+er"))
	(unspec:TI [(match_operand:TI 0 "register_operand" "eq")]
		   UNSPEC_XESPV_VST128))
   (set (match_dup 1)
	(plus:SI (match_dup 1)
		 (match_operand:SI 2 "const_int_operand")))]
  "TARGET_XESPV_QREGS && riscv_xespv_ip_offset_p (operands[2], 16)"
  "esp.vst.128.ip\t%0,%1,%2"
  [(set_attr "type" "store")
   (set_attr "mode" "TI")])

;; The high half of operand 0 is left unspecified.
(define_insn "xespv_vld_l64_ip"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(mem:DI (match_operand:SI 1 "register_operand" "+er"))]
		   UNSPEC_XESPV_VLD_L64))
   (set (match_dup 1)
	(plus:SI (match_dup 1)
		 (match_operand:SI 2 "const_int_operand")))]
  "TARGET_XESPV_QREGS && riscv_xespv_ip_offset_p (operands[2], 8)"
  "esp.vld.l.64.ip\t%0,%1,%2"
  [(set_attr "type" "load")
   (set_attr "mode" "DI")])

(define_insn "xespv_vld_h64_ip"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(match_operand:TI 3 "register_operand" "0")
		    (mem:DI (match_operand:SI 1 "register_operand" "+er"))]
		   UNSPEC_XESPV_VLD_H64))
   (set (match_dup 1)
	(plus:SI (match_dup 1)
		 (match_operand:SI 2 "const_int_operand")))]
  "TARGET_XESPV_QREGS && riscv_xespv_ip_offset_p (operands[2], 8)"
  "esp.vld.h.64.ip\t%0,%1,%2"
  [(set_attr "type" "load")
   (set_attr "mode" "DI")])

(define_insn "xespv_vst_l64_ip"
  [(set (mem:DI (match_operand:SI 1 "register_operand" "+er"))
	(unspec:DI [(match_operand:TI 0 "register_operand" "eq")]
		   UNSPEC_XESPV_VST_L64))
   (set (match_dup 1)
	(plus:SI (match_dup 1)
		 (match_operand:SI 2 "const_int_operand")))]
  "TARGET_XESPV_QREGS && riscv_xespv_ip_offset_p (operands[2], 8)"
  "esp.vst.l.64.ip\t%0,%1,%2"
  [(set_attr "type" "store")
   (set_attr "mode" "DI")])

(define_insn "xespv_vst_h64_ip"
  [(set (mem:DI (match_operand:SI 1 "register_operand" "+er"))
	(unspec:DI [(match_operand:TI 0 "register_operand" "eq")]
		   UNSPEC_XESPV_VST_H64))
   (set (match_dup 1)
	(plus:SI (match_dup 1)
		 (match_operand:SI 2 "const_int_operand")))]
  "TARGET_XESPV_QREGS && riscv_xespv_ip_offset_p (operands[2], 8)"
  "esp.vst.h.64.ip\t%0,%1,%2"
  [(set_attr "type" "store")
   (set_attr "mode" "DI")])

;; SAR_BYTE is not modelled as a register: writing it and using it are
;; volatile so that they stay in order.
(define_insn "xespv_movx_w_sar_bytes"
  [(unspec_volatile [(match_operand:SI 0 "register_operand" "er")]
		    UNSPECV_XESPV_MOVX_W_SAR_BYTES)]
  "TARGET_XESPV_QREGS"
  "esp.movx.w.sar.bytes\t%0"
  [(set_attr "type" "move")
   (set_attr "mode" "SI")])

(define_insn "xespv_andq"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(match_operand:TI 1 "register_operand" "eq")
		    (match_operand:TI 2 "register_operand" "eq")]
		   UNSPEC_XESPV_ANDQ))]
  "TARGET_XESPV_QREGS"
  "esp.andq\t%0,%1,%2"
  [(set_attr "type" "move")
   (set_attr "mode" "TI")])

(define_insn "xespv_notq"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(match_operand:TI 1 "register_operand" "eq")]
		   UNSPEC_XESPV_NOTQ))]
  "TARGET_XESPV_QREGS"
  "esp.notq\t%0,%1"
  [(set_attr "type" "move")
   (set_attr "mode" "TI")])

(define_insn "xespv_orq"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(match_operand:TI 1 "register_operand" "eq")
		    (match_operand:TI 2 "register_operand" "eq")]
		   UNSPEC_XESPV_ORQ))]
  "TARGET_XESPV_QREGS"
  "esp.orq\t%0,%1,%2"
  [(set_attr "type" "move")
   (set_attr "mode" "TI")])

(define_insn "xespv_zero_q"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(const_int 0)] UNSPEC_XESPV_ZERO_Q))]
  "TARGET_XESPV_QREGS"
  "esp.zero.q\t%0"
  [(set_attr "type" "move")
   (set_attr "mode" "TI")])

;; Operand 0 is the bitwise exclusive or of operands 1 and 2.
(define_insn "xespv_xorq"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec:TI [(match_operand:TI 1 "register_operand" "eq")
		    (match_operand:TI 2 "register_operand" "eq")]
		   UNSPEC_XESPV_XORQ))]
  "TARGET_XESPV_QREGS"
  "esp.xorq\t%0,%1,%2"
  [(set_attr "type" "move")
   (set_attr "mode" "TI")])

;; Operand 0 is word OPERAND 2 of the q register, little-endian.
(define_insn "xespv_movi_32_a"
  [(set (match_operand:SI 0 "register_operand" "=er")
	(unspec:SI [(match_operand:TI 1 "register_operand" "eq")
		    (match_operand:SI 2 "const_int_operand")]
		   UNSPEC_XESPV_MOVI32))]
  "TARGET_XESPV_QREGS && IN_RANGE (INTVAL (operands[2]), 0, 3)"
  "esp.movi.32.a\t%1,%0,%2"
  [(set_attr "type" "move")
   (set_attr "mode" "SI")])

;; Operand 0 is byte OPERAND 2 of the q register, little-endian.  The
;; assembler accepts the index, and the value is zero-extended by the
;; caller when it is used as an unsigned byte.
(define_insn "xespv_movi_8_a"
  [(set (match_operand:SI 0 "register_operand" "=er")
	(unspec:SI [(match_operand:TI 1 "register_operand" "eq")
		    (match_operand:SI 2 "const_int_operand")]
		   UNSPEC_XESPV_MOVI8))]
  "TARGET_XESPV_QREGS && IN_RANGE (INTVAL (operands[2]), 0, 15)"
  "esp.movi.8.a\t%1,%0,%2"
  [(set_attr "type" "move")
   (set_attr "mode" "SI")])

;; Operand 0 is the 16 bytes at offset SAR_BYTE of the 32 bytes formed by
;; operand 1 followed by operand 2.
(define_insn "xespv_src_q"
  [(set (match_operand:TI 0 "register_operand" "=eq")
	(unspec_volatile:TI [(match_operand:TI 1 "register_operand" "eq")
			     (match_operand:TI 2 "register_operand" "eq")]
			    UNSPECV_XESPV_SRC_Q))]
  "TARGET_XESPV_QREGS"
  "esp.src.q\t%0,%1,%2"
  [(set_attr "type" "move")
   (set_attr "mode" "TI")])
