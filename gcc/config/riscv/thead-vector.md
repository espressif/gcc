(define_c_enum "unspec" [
  UNSPEC_TH_VLB
  UNSPEC_TH_VLBU
  UNSPEC_TH_VLH
  UNSPEC_TH_VLHU
  UNSPEC_TH_VLW
  UNSPEC_TH_VLWU

  UNSPEC_TH_VLSB
  UNSPEC_TH_VLSBU
  UNSPEC_TH_VLSH
  UNSPEC_TH_VLSHU
  UNSPEC_TH_VLSW
  UNSPEC_TH_VLSWU

  UNSPEC_TH_VLXB
  UNSPEC_TH_VLXBU
  UNSPEC_TH_VLXH
  UNSPEC_TH_VLXHU
  UNSPEC_TH_VLXW
  UNSPEC_TH_VLXWU

  UNSPEC_TH_VSUXB
  UNSPEC_TH_VSUXH
  UNSPEC_TH_VSUXW

  UNSPEC_TH_VWLDST

  UNSPEC_TH_VLSEGB
  UNSPEC_TH_VLSEGBU
  UNSPEC_TH_VLSEGH
  UNSPEC_TH_VLSEGHU
  UNSPEC_TH_VLSEGW
  UNSPEC_TH_VLSEGWU

  UNSPEC_TH_VLSSEGB
  UNSPEC_TH_VLSSEGBU
  UNSPEC_TH_VLSSEGH
  UNSPEC_TH_VLSSEGHU
  UNSPEC_TH_VLSSEGW
  UNSPEC_TH_VLSSEGWU

  UNSPEC_TH_VLXSEGB
  UNSPEC_TH_VLXSEGBU
  UNSPEC_TH_VLXSEGH
  UNSPEC_TH_VLXSEGHU
  UNSPEC_TH_VLXSEGW
  UNSPEC_TH_VLXSEGWU

  UNSPEC_TH_VFEXP2
  UNSPEC_TH_VFTANH
  UNSPEC_TH_VFSIG
  UNSPEC_TH_VFREC
])

(define_int_iterator UNSPEC_TH_VLMEM_OP [
  UNSPEC_TH_VLB UNSPEC_TH_VLBU
  UNSPEC_TH_VLH UNSPEC_TH_VLHU
  UNSPEC_TH_VLW UNSPEC_TH_VLWU
])

(define_int_iterator UNSPEC_TH_VLSMEM_OP [
  UNSPEC_TH_VLSB UNSPEC_TH_VLSBU
  UNSPEC_TH_VLSH UNSPEC_TH_VLSHU
  UNSPEC_TH_VLSW UNSPEC_TH_VLSWU
])

(define_int_iterator UNSPEC_TH_VLXMEM_OP [
  UNSPEC_TH_VLXB UNSPEC_TH_VLXBU
  UNSPEC_TH_VLXH UNSPEC_TH_VLXHU
  UNSPEC_TH_VLXW UNSPEC_TH_VLXWU
])

(define_int_iterator UNSPEC_TH_VLSEGMEM_OP [
  UNSPEC_TH_VLSEGB UNSPEC_TH_VLSEGBU
  UNSPEC_TH_VLSEGH UNSPEC_TH_VLSEGHU
  UNSPEC_TH_VLSEGW UNSPEC_TH_VLSEGWU
])

(define_int_iterator UNSPEC_TH_VLSSEGMEM_OP [
  UNSPEC_TH_VLSSEGB UNSPEC_TH_VLSSEGBU
  UNSPEC_TH_VLSSEGH UNSPEC_TH_VLSSEGHU
  UNSPEC_TH_VLSSEGW UNSPEC_TH_VLSSEGWU
])

(define_int_iterator UNSPEC_TH_VLXSEGMEM_OP [
  UNSPEC_TH_VLXSEGB UNSPEC_TH_VLXSEGBU
  UNSPEC_TH_VLXSEGH UNSPEC_TH_VLXSEGHU
  UNSPEC_TH_VLXSEGW UNSPEC_TH_VLXSEGWU
])

(define_int_attr vlmem_op_attr [
  (UNSPEC_TH_VLB "b") (UNSPEC_TH_VLBU "bu")
  (UNSPEC_TH_VLH "h") (UNSPEC_TH_VLHU "hu")
  (UNSPEC_TH_VLW "w") (UNSPEC_TH_VLWU "wu")
  (UNSPEC_TH_VLSB "b") (UNSPEC_TH_VLSBU "bu")
  (UNSPEC_TH_VLSH "h") (UNSPEC_TH_VLSHU "hu")
  (UNSPEC_TH_VLSW "w") (UNSPEC_TH_VLSWU "wu")
  (UNSPEC_TH_VLXB "b") (UNSPEC_TH_VLXBU "bu")
  (UNSPEC_TH_VLXH "h") (UNSPEC_TH_VLXHU "hu")
  (UNSPEC_TH_VLXW "w") (UNSPEC_TH_VLXWU "wu")
  (UNSPEC_TH_VSUXB "b")
  (UNSPEC_TH_VSUXH "h")
  (UNSPEC_TH_VSUXW "w")
  (UNSPEC_TH_VLSEGB "b") (UNSPEC_TH_VLSEGBU "bu")
  (UNSPEC_TH_VLSEGH "h") (UNSPEC_TH_VLSEGHU "hu")
  (UNSPEC_TH_VLSEGW "w") (UNSPEC_TH_VLSEGWU "wu")
  (UNSPEC_TH_VLSSEGB "b") (UNSPEC_TH_VLSSEGBU "bu")
  (UNSPEC_TH_VLSSEGH "h") (UNSPEC_TH_VLSSEGHU "hu")
  (UNSPEC_TH_VLSSEGW "w") (UNSPEC_TH_VLSSEGWU "wu")
  (UNSPEC_TH_VLXSEGB "b") (UNSPEC_TH_VLXSEGBU "bu")
  (UNSPEC_TH_VLXSEGH "h") (UNSPEC_TH_VLXSEGHU "hu")
  (UNSPEC_TH_VLXSEGW "w") (UNSPEC_TH_VLXSEGWU "wu")
])

(define_int_attr vlmem_order_attr [
  (UNSPEC_TH_VLXB "")
  (UNSPEC_TH_VLXH "")
  (UNSPEC_TH_VLXW "")
  (UNSPEC_TH_VSUXB "u")
  (UNSPEC_TH_VSUXH "u")
  (UNSPEC_TH_VSUXW "u")
])

(define_int_iterator UNSPEC_TH_VSMEM_OP [
  UNSPEC_TH_VLB
  UNSPEC_TH_VLH
  UNSPEC_TH_VLW
])

(define_int_iterator UNSPEC_TH_VSSMEM_OP [
  UNSPEC_TH_VLSB
  UNSPEC_TH_VLSH
  UNSPEC_TH_VLSW
])

(define_int_iterator UNSPEC_TH_VSXMEM_OP [
  UNSPEC_TH_VLXB
  UNSPEC_TH_VLXH
  UNSPEC_TH_VLXW
  UNSPEC_TH_VSUXB
  UNSPEC_TH_VSUXH
  UNSPEC_TH_VSUXW
])

(define_int_iterator UNSPEC_TH_VSSEGMEM_OP [
  UNSPEC_TH_VLSEGB
  UNSPEC_TH_VLSEGH
  UNSPEC_TH_VLSEGW
])

(define_int_iterator UNSPEC_TH_VSSSEGMEM_OP [
  UNSPEC_TH_VLSSEGB
  UNSPEC_TH_VLSSEGH
  UNSPEC_TH_VLSSEGW
])

(define_int_iterator UNSPEC_TH_VSXSEGMEM_OP [
  UNSPEC_TH_VLXSEGB
  UNSPEC_TH_VLXSEGH
  UNSPEC_TH_VLXSEGW
])

(define_mode_iterator V_VLS_VT [V VLS VT])
(define_mode_iterator V_VB_VLS_VT [V VB VLS VT])

(define_mode_iterator TH_VSF [
  (RVVM8SF "TARGET_VECTOR_ELEN_FP_32") (RVVM4SF "TARGET_VECTOR_ELEN_FP_32") (RVVM2SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM1SF "TARGET_VECTOR_ELEN_FP_32") (RVVMF2SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
])

(define_insn "@th_vsetvl<mode>"
  [(set (match_operand:P 0 "register_operand" "=&r")
	(unspec:P [(match_operand:P 1 "vector_length_operand" "rK")
		   (match_operand 2 "const_int_operand" "i")
		   (match_operand 3 "const_int_operand" "i")
		   (match_operand 4 "const_int_operand" "i")
		   (match_operand 5 "const_int_operand" "i")] UNSPEC_VSETVL))
   (set (reg:SI VL_REGNUM)
	(unspec:SI [(match_dup 1)
		    (match_dup 2)
		    (match_dup 3)] UNSPEC_VSETVL))
   (set (reg:SI VTYPE_REGNUM)
	(unspec:SI [(match_dup 2)
		    (match_dup 3)
		    (match_dup 4)
		    (match_dup 5)] UNSPEC_VSETVL))]
  "TARGET_XTHEADVECTOR && REG_P (operands[1]) && REGNO(operands[1]) != 0"
  "vset%i1vli\t%0,%1,e%2,%m3,t%p4,m%p5"
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "<MODE>")
   (set (attr "sew") (symbol_ref "INTVAL (operands[2])"))
   (set (attr "vlmul") (symbol_ref "INTVAL (operands[3])"))
   (set (attr "ta") (symbol_ref "INTVAL (operands[4])"))
   (set (attr "ma") (symbol_ref "INTVAL (operands[5])"))])

(define_insn_and_split "@th_vsetvl<mode>_no_side_effects"
  [(set (match_operand:P 0 "register_operand" "=&r")
	(unspec:P [(match_operand:P 1 "vector_length_operand" "rK")
		   (match_operand 2 "const_int_operand" "i")
		   (match_operand 3 "const_int_operand" "i")
		   (match_operand 4 "const_int_operand" "i")
		   (match_operand 5 "const_int_operand" "i")] UNSPEC_VSETVL))]
  "TARGET_XTHEADVECTOR"
  "#"
  "&& epilogue_completed"
  [(parallel
    [(set (match_dup 0)
	  (unspec:P [(match_dup 1) (match_dup 2) (match_dup 3)
		     (match_dup 4) (match_dup 5)] UNSPEC_VSETVL))
     (set (reg:SI VL_REGNUM)
	  (unspec:SI [(match_dup 1) (match_dup 2) (match_dup 3)] UNSPEC_VSETVL))
     (set (reg:SI VTYPE_REGNUM)
	  (unspec:SI [(match_dup 2) (match_dup 3) (match_dup 4)
		      (match_dup 5)] UNSPEC_VSETVL))])]
  ""
  [(set_attr "type" "vsetvl")
   (set_attr "mode" "SI")])


(define_insn_and_split "@th_pred_vl_mov<mode>"
  [(set (match_operand:P 0 "register_operand"    "=r")
	(unspec:P
	  [(match_operand:P 1 "const_int_operand" " i")]
	UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "li\t%0,%1"
  "&& epilogue_completed"
  [(set (match_dup 0) (match_dup 1))]
  {}
  [(set_attr "type" "arith")])

(define_split
  [(set (match_operand:V_VB_VLS_VT 0 "reg_or_mem_operand")
	(match_operand:V_VB_VLS_VT 1 "reg_or_mem_operand"))]
  "TARGET_XTHEADVECTOR"
  [(const_int 0)]
  {
    emit_insn (gen_pred_th_whole_mov (<MODE>mode, operands[0], operands[1],
				      RVV_VLMAX, GEN_INT(riscv_vector::VLMAX)));
    DONE;
  })

(define_insn_and_split "@pred_th_whole_mov<mode>"
  [(set (match_operand:V_VLS_VT 0 "reg_or_mem_operand"  "=vr,vr, m")
	(unspec:V_VLS_VT
	  [(match_operand:V_VLS_VT 1 "reg_or_mem_operand" " vr, m,vr")
	   (match_operand 2 "vector_length_operand"   " rK, rK, rK")
	   (match_operand 3 "const_1_operand"         "  i, i, i")
	   (reg:SI VL_REGNUM)
	   (reg:SI VTYPE_REGNUM)]
	UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "@
   vmv.v.v\t%0,%1
   vle.v\t%0,%1
   vse.v\t%1,%0"
  "&& REG_P (operands[0]) && REG_P (operands[1])
   && REGNO (operands[0]) == REGNO (operands[1])"
  [(const_int 0)]
  ""
  [(set_attr "type" "vimov,vlds,vlds")
   (set_attr "mode" "<MODE>")
   (set (attr "ta") (symbol_ref "riscv_vector::TAIL_UNDISTURBED"))
   (set (attr "ma") (symbol_ref "riscv_vector::MASK_UNDISTURBED"))
   (set (attr "avl_type_idx") (const_int 3))
   (set_attr "vl_op_idx" "2")])

(define_insn_and_split "@pred_th_whole_mov<mode>"
  [(set (match_operand:VB 0 "reg_or_mem_operand"  "=vr,vr, m")
	(unspec:VB
	  [(match_operand:VB 1 "reg_or_mem_operand" " vr, m,vr")
	   (match_operand 2 "vector_length_operand"   " rK, rK, rK")
	   (match_operand 3 "const_1_operand"         "  i, i, i")
	   (reg:SI VL_REGNUM)
	   (reg:SI VTYPE_REGNUM)]
	UNSPEC_TH_VWLDST))]
  "TARGET_XTHEADVECTOR"
  "@
   vmv.v.v\t%0,%1
   vle.v\t%0,%1
   vse.v\t%1,%0"
  "&& REG_P (operands[0]) && REG_P (operands[1])
   && REGNO (operands[0]) == REGNO (operands[1])"
  [(const_int 0)]
  ""
  [(set_attr "type" "vimov,vlds,vlds")
   (set_attr "mode" "<MODE>")
   (set (attr "ta") (symbol_ref "riscv_vector::TAIL_UNDISTURBED"))
   (set (attr "ma") (symbol_ref "riscv_vector::MASK_UNDISTURBED"))
   (set (attr "avl_type_idx") (const_int 3))
   (set_attr "vl_op_idx" "2")
   (set (attr "sew") (const_int 8))
   (set (attr "vlmul") (symbol_ref "riscv_vector::LMUL_1"))])

(define_insn_and_split "*pred_th_mov<mode>"
  [(set (match_operand:VB_VLS 0 "nonimmediate_operand"               "=vr,   m,  vr,  vr,  vr")
	(if_then_else:VB_VLS
	  (unspec:VB_VLS
	    [(match_operand:VB_VLS 1 "vector_all_trues_mask_operand" "Wc1, Wc1, Wc1, Wc1, Wc1")
	     (match_operand 4 "vector_length_operand"            " rK,  rK,  rK,  rK,  rK")
	     (match_operand 5 "const_int_operand"                "  i,   i,   i,   i,   i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (match_operand:VB_VLS 3 "vector_move_operand"              "  m,  vr,  vr, Wc0, Wc1")
	  (match_operand:VB_VLS 2 "vector_undef_operand"             " vu,  vu,  vu,  vu,  vu")))]
  "TARGET_XTHEADVECTOR"
  "@
   #
   #
   vmcpy.m\t%0,%3
   vmclr.m\t%0
   vmset.m\t%0"
  "&& !reload_completed"
  [(const_int 0)]
  {
    if ((MEM_P (operands[0]) || MEM_P (operands[3]))
        || (REG_P (operands[0]) && REG_P (operands[3])
	    && INTVAL (operands[5]) == riscv_vector::VLMAX))
      {
	emit_move_insn (operands[0], operands[3]);
	DONE;
      }
    FAIL;
  }
  [(set_attr "type" "vldm,vstm,vmalu,vmalu,vmalu")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_mov_width<vlmem_op_attr><mode>"
  [(set (match_operand:V_VLS 0 "nonimmediate_operand")
    (if_then_else:V_VLS
      (unspec:<VM>
	[(match_operand:<VM> 1 "vector_mask_operand")
	 (match_operand 4 "vector_length_operand")
	 (match_operand 5 "const_int_operand")
	 (match_operand 6 "const_int_operand")
	 (match_operand 7 "const_int_operand")
	 (reg:SI VL_REGNUM)
	 (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLMEM_OP)
      (match_operand:V_VLS 3 "vector_move_operand")
      (match_operand:V_VLS 2 "vector_merge_operand")))]
  "TARGET_XTHEADVECTOR"
  {})

(define_insn_and_split "*pred_mov_width<vlmem_op_attr><mode>"
  [(set (match_operand:V_VLS 0 "nonimmediate_operand"	    "=vr,    vr,    vd,     m,    vr,    vr")
    (if_then_else:V_VLS
      (unspec:<VM>
	[(match_operand:<VM> 1 "vector_mask_operand"	   "vmWc1,   Wc1,    vm, vmWc1,   Wc1,   Wc1")
	 (match_operand 4 "vector_length_operand"	      "   rK,    rK,    rK,    rK,    rK,    rK")
	 (match_operand 5 "const_int_operand"		  "    i,     i,     i,     i,     i,     i")
	 (match_operand 6 "const_int_operand"		  "    i,     i,     i,     i,     i,     i")
	 (match_operand 7 "const_int_operand"		  "    i,     i,     i,     i,     i,     i")
	 (reg:SI VL_REGNUM)
	 (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLMEM_OP)
      (match_operand:V_VLS 3 "reg_or_mem_operand"	      "    m,     m,     m,    vr,    vr,    vr")
      (match_operand:V_VLS 2 "vector_merge_operand"	    "    0,    vu,    vu,    vu,    vu,     0")))]
  "(TARGET_XTHEADVECTOR
    && (register_operand (operands[0], <MODE>mode)
	|| register_operand (operands[3], <MODE>mode)))"
  "@
   vl<vlmem_op_attr>.v\t%0,%3%p1
   vl<vlmem_op_attr>.v\t%0,%3
   vl<vlmem_op_attr>.v\t%0,%3,%1.t
   vs<vlmem_op_attr>.v\t%3,%0%p1
   vmv.v.v\t%0,%3
   vmv.v.v\t%0,%3"
  "&& riscv_vector::whole_reg_to_reg_move_p (operands, <MODE>mode, 7)"
  [(set (match_dup 0) (match_dup 3))]
  ""
  [(set_attr "type" "vlde,vlde,vlde,vste,vimov,vimov")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_store_width<vlmem_op_attr><mode>"
  [(set (match_operand:VI 0 "memory_operand"		 "+m")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
	     (match_operand 3 "vector_length_operand"    "   rK")
	     (match_operand 4 "const_int_operand"	"    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VSMEM_OP)
	  (match_operand:VI 2 "register_operand"	 "    vr")
	  (match_dup 0)))]
  "TARGET_XTHEADVECTOR"
  "vs<vlmem_op_attr>.v\t%2,%0%p1"
  [(set_attr "type" "vste")
   (set_attr "mode" "<MODE>")
   (set (attr "avl_type_idx") (const_int 4))
   (set_attr "vl_op_idx" "3")])

(define_insn "@pred_strided_load_width<vlmem_op_attr><mode>"
  [(set (match_operand:VI 0 "register_operand"	      "=vr,    vr,    vd")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 5 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 6 "const_int_operand"	"    i,     i,     i")
	     (match_operand 7 "const_int_operand"	"    i,     i,     i")
	     (match_operand 8 "const_int_operand"	"    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLSMEM_OP)
	  (unspec:VI
	    [(match_operand:VI 3 "memory_operand"	 "    m,     m,     m")
	     (match_operand 4 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")] UNSPEC_TH_VLSMEM_OP)
	  (match_operand:VI 2 "vector_merge_operand"      "    0,    vu,    vu")))]
  "TARGET_XTHEADVECTOR"
  "vls<vlmem_op_attr>.v\t%0,%3,%z4%p1"
  [(set_attr "type" "vlds")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_strided_store_width<vlmem_op_attr><mode>"
  [(set (match_operand:VI 0 "memory_operand"		 "+m")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"	"    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VSSMEM_OP)
	  (unspec:VI
	    [(match_operand 2 "pmode_reg_or_0_operand"   "   rJ")
	     (match_operand:VI 3 "register_operand"       "   vr")] UNSPEC_TH_VSSMEM_OP)
	  (match_dup 0)))]
  "TARGET_XTHEADVECTOR"
  "vss<vlmem_op_attr>.v\t%3,%0,%z2%p1"
  [(set_attr "type" "vsts")
   (set_attr "mode" "<MODE>")
   (set (attr "avl_type_idx") (const_int 5))])

(define_insn "@pred_indexed_load_width<vlmem_op_attr><mode>"
  [(set (match_operand:VI 0 "register_operand"	     "=vd, vr,vd, vr")
	(if_then_else:VI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"  " vm,Wc1,vm,Wc1")
	     (match_operand 5 "vector_length_operand"     " rK, rK,rK, rK")
	     (match_operand 6 "const_int_operand"	 "  i,  i, i,  i")
	     (match_operand 7 "const_int_operand"	 "  i,  i, i,  i")
	     (match_operand 8 "const_int_operand"	 "  i,  i, i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLXMEM_OP)
	  (unspec:VI
	    [(match_operand 3 "pmode_reg_or_0_operand"    " rJ, rJ,rJ, rJ")
	     (mem:BLK (scratch))
	     (match_operand:VI 4 "register_operand" " vr, vr,vr, vr")] UNSPEC_TH_VLXMEM_OP)
	  (match_operand:VI 2 "vector_merge_operand"       " vu, vu, 0,  0")))]
  "TARGET_XTHEADVECTOR"
  "vlx<vlmem_op_attr>.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vldux")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_indexed_<vlmem_order_attr>store_width<vlmem_op_attr><mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	    [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"	"    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VSXMEM_OP)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:VI 2 "register_operand" "  vr")
	   (match_operand:VI 3 "register_operand"  "  vr")] UNSPEC_TH_VSXMEM_OP))]
  "TARGET_XTHEADVECTOR"
  "vs<vlmem_order_attr>x<vlmem_op_attr>.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vstux")
   (set_attr "mode" "<MODE>")])

(define_expand "@pred_th_extract<V_VLSI:mode><P:mode>"
  [(set (match_operand:<VEL> 0 "register_operand")
	(unspec:<VEL>
	  [(vec_select:<VEL>
	     (match_operand:V_VLSI 1 "register_operand")
	     (parallel [(match_operand:P 2 "register_operand" "r")]))
	   (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE))]
  "TARGET_XTHEADVECTOR"
{})

(define_insn "*pred_th_extract<V_VLSI:mode><P:mode>"
  [(set (match_operand:<VEL> 0 "register_operand"   "=r")
  (unspec:<VEL>
    [(vec_select:<VEL>
       (match_operand:V_VLSI 1 "register_operand" "vr")
       (parallel [(match_operand:P 2 "register_operand" "r")]))
     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE))]
  "TARGET_XTHEADVECTOR"
  "vext.x.v\t%0,%1,%2"
  [(set_attr "type" "vimovvx")
   (set_attr "mode" "<V_VLSI:MODE>")])

(define_insn "@pred_th_unit_seg_load<vlmem_op_attr><mode>"
  [(set (match_operand:VT 0 "register_operand"             "=vr,    vr,    vd")
	(if_then_else:VT
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 4 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 5 "const_int_operand"        "    i,     i,     i")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLSEGMEM_OP)
	  (unspec:VT
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (mem:BLK (scratch))] UNSPEC_TH_VLSEGMEM_OP)
	  (match_operand:VT 2 "vector_merge_operand"     "    0,    vu,    vu")))]
  "TARGET_XTHEADVECTOR"
  "vlseg<nf><vlmem_op_attr>.v\t%0,(%z3)%p1"
  [(set_attr "type" "vlsegde")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_unit_seg_store<vlmem_op_attr><mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	     [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	      (match_operand 3 "vector_length_operand"    "   rK")
	      (match_operand 4 "const_int_operand"        "    i")
	      (reg:SI VL_REGNUM)
	      (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VSSEGMEM_OP)
	   (match_operand 1 "pmode_reg_or_0_operand"      "   rJ")
	   (match_operand:VT 2 "register_operand"         "   vr")
	   (mem:BLK (scratch))] UNSPEC_TH_VSSEGMEM_OP))]
  "TARGET_XTHEADVECTOR"
  "vsseg<nf><vlmem_op_attr>.v\t%2,(%z1)%p0"
  [(set_attr "type" "vssegte")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_strided_load<vlmem_op_attr><mode>"
  [(set (match_operand:VT 0 "register_operand"             "=vr,    vr,    vd")
	(if_then_else:VT
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1,   Wc1,    vm")
	     (match_operand 5 "vector_length_operand"    "   rK,    rK,    rK")
	     (match_operand 6 "const_int_operand"        "    i,     i,     i")
	     (match_operand 7 "const_int_operand"        "    i,     i,     i")
	     (match_operand 8 "const_int_operand"        "    i,     i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLSSEGMEM_OP)
	  (unspec:VT
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (match_operand 4 "pmode_reg_or_0_operand"   "   rJ,    rJ,    rJ")
	     (mem:BLK (scratch))] UNSPEC_TH_VLSSEGMEM_OP)
	  (match_operand:VT 2 "vector_merge_operand"     "    0,    vu,    vu")))]
  "TARGET_XTHEADVECTOR"
  "vlsseg<nf><vlmem_op_attr>.v\t%0,(%z3),%z4%p1"
  [(set_attr "type" "vlsegds")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_strided_store<vlmem_op_attr><mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VM>
	     [(match_operand:<VM> 0 "vector_mask_operand" "vmWc1")
	      (match_operand 4 "vector_length_operand"    "   rK")
	      (match_operand 5 "const_int_operand"        "    i")
	      (reg:SI VL_REGNUM)
	      (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VSSSEGMEM_OP)
	   (match_operand 1 "pmode_reg_or_0_operand"      "   rJ")
	   (match_operand 2 "pmode_reg_or_0_operand"      "   rJ")
	   (match_operand:VT 3 "register_operand"         "   vr")
	   (mem:BLK (scratch))] UNSPEC_TH_VSSSEGMEM_OP))]
  "TARGET_XTHEADVECTOR"
  "vssseg<nf><vlmem_op_attr>.v\t%3,(%z1),%z2%p0"
  [(set_attr "type" "vssegts")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_th_indexed_load<vlmem_op_attr><VT:mode><VI:mode>"
  [(set (match_operand:VT 0 "register_operand"           "=&vr,  &vr")
	(if_then_else:VT
	  (unspec:<VT:VM>
	    [(match_operand:<VT:VM> 1 "vector_mask_operand" "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    "   rK,   rK")
	     (match_operand 6 "const_int_operand"        "    i,    i")
	     (match_operand 7 "const_int_operand"        "    i,    i")
	     (match_operand 8 "const_int_operand"        "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VLXSEGMEM_OP)
	  (unspec:VT
	    [(match_operand 3 "pmode_reg_or_0_operand"   "   rJ,   rJ")
	     (mem:BLK (scratch))
	     (match_operand:VI 4 "register_operand"     "   vr,   vr")] UNSPEC_TH_VLXSEGMEM_OP)
	  (match_operand:VT 2 "vector_merge_operand"    "   vu,    0")))]
  "TARGET_XTHEADVECTOR"
  "vlxseg<nf><vlmem_op_attr>.v\t%0,(%z3),%4%p1"
  [(set_attr "type" "vlsegdux")
   (set_attr "mode" "<VT:MODE>")])

(define_insn "@pred_th_indexed_store<vlmem_op_attr><VT:mode><VI:mode>"
  [(set (mem:BLK (scratch))
	(unspec:BLK
	  [(unspec:<VT:VM>
	    [(match_operand:<VT:VM> 0 "vector_mask_operand" "vmWc1")
	     (match_operand 4 "vector_length_operand"    "   rK")
	     (match_operand 5 "const_int_operand"	"    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_TH_VSXSEGMEM_OP)
	   (match_operand 1 "pmode_reg_or_0_operand"      "  rJ")
	   (match_operand:VI 2 "register_operand" "  vr")
	   (match_operand:VT 3 "register_operand"  "  vr")] UNSPEC_TH_VSXSEGMEM_OP))]
  "TARGET_XTHEADVECTOR"
  "vsxseg<nf><vlmem_op_attr>.v\t%3,(%z1),%2%p0"
  [(set_attr "type" "vssegtux")
   (set_attr "mode" "<VT:MODE>")])

;; XTheadVdot

(define_c_enum "unspec" [
  UNSPEC_TH_VMAQA
  UNSPEC_TH_VMAQAU
  UNSPEC_TH_VMAQASU
  UNSPEC_TH_VMAQAUS
  UNSPEC_TH_VPMAQA
  UNSPEC_TH_VPMAQAU
  UNSPEC_TH_VPMAQASU
  UNSPEC_TH_VPMAQAUS
  UNSPEC_TH_VPNCLIP
  UNSPEC_TH_VPNCLIPU
  UNSPEC_TH_VPWADD
  UNSPEC_TH_VPWADDU
])

;; Iterator and attributes for pack narrowing clip instructions.
(define_int_iterator UNSPEC_TH_VPCLIP_ITERATOR [UNSPEC_TH_VPNCLIP UNSPEC_TH_VPNCLIPU])

;; Iterator for pack widening integer add instructions.
(define_int_iterator UNSPEC_TH_WPWADD_ITERATOR [UNSPEC_TH_VPWADD UNSPEC_TH_VPWADDU])

(define_int_iterator UNSPEC_TH_VMAQA_AND_VPMAQA_ITERATOR [
        UNSPEC_TH_VMAQA    UNSPEC_TH_VMAQAU
	UNSPEC_TH_VMAQASU  UNSPEC_TH_VMAQAUS
	UNSPEC_TH_VPMAQA   UNSPEC_TH_VPMAQAU
	UNSPEC_TH_VPMAQASU  UNSPEC_TH_VPMAQAUS])

(define_int_attr instr [
        (UNSPEC_TH_VPNCLIP "vpnclip") (UNSPEC_TH_VPNCLIPU "vpnclip")
        (UNSPEC_TH_VPWADD "vpwadd") (UNSPEC_TH_VPWADDU "vpwadd")
        (UNSPEC_TH_VMAQA "vmaqa") (UNSPEC_TH_VMAQAU "vmaqa")
	(UNSPEC_TH_VMAQASU "vmaqa") (UNSPEC_TH_VMAQAUS "vmaqa")
	(UNSPEC_TH_VPMAQA "vpmaqa") (UNSPEC_TH_VPMAQAU "vpmaqa")
	(UNSPEC_TH_VPMAQASU "vpmaqa") (UNSPEC_TH_VPMAQAUS "vpmaqa")])

(define_int_attr tv_su [(UNSPEC_TH_VPNCLIP "") (UNSPEC_TH_VPNCLIPU "u")
			(UNSPEC_TH_VPWADD "") (UNSPEC_TH_VPWADDU "u")
                        (UNSPEC_TH_VMAQA "") (UNSPEC_TH_VMAQAU "u")
                        (UNSPEC_TH_VMAQASU "su") (UNSPEC_TH_VMAQAUS "us")
                        (UNSPEC_TH_VPMAQA "") (UNSPEC_TH_VPMAQAU "u")
                        (UNSPEC_TH_VPMAQASU "su") (UNSPEC_TH_VPMAQAUS "us")])

(define_mode_iterator TH_VDOT_WEXTI [
  RVVM8HI RVVM4HI RVVM2HI RVVM1HI RVVMF2HI (RVVMF4HI "TARGET_MIN_VLEN > 32")
])

(define_mode_attr TH_VDOT_SI_VM [
  (RVVM8SI "RVVM1BI") (RVVM4SI "RVVMF2BI") (RVVM2SI "RVVMF4BI") (RVVM1SI "RVVMF8BI") (RVVMF2SI "RVVMF16BI")
])

(define_mode_attr TH_VDOT_SI_MAQA [
  (RVVM8SI "RVVM8QI") (RVVM4SI "RVVM4QI") (RVVM2SI "RVVM2QI") (RVVM1SI "RVVM1QI") (RVVMF2SI "RVVMF2QI")
])

(define_insn "@pred_narrow_vpnclip<tv_su><mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd,vd, vr, vr,vd, vr,  vr,  vr, vd, vr,  vr,  vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm,vm,Wc1,Wc1,vm,Wc1,vmWc1,vmWc1, vm,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK,rK, rK, rK,rK, rK,   rK,   rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (match_operand 9 "const_int_operand"                      "  i, i,  i,  i, i,  i,    i,    i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI VXRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC>
	    [(match_operand:TH_VDOT_WEXTI 3 "register_operand"                " vr,vr, vr, vr, 0,  0,   vr,   vr,  0,  0,   vr,   vr")
	     (match_operand:<V_DOUBLE_TRUNC> 4 "vector_shift_operand"  "  0, 0,  0,  0,vr, vr,   vr,   vr, vk, vk,   vk,   vk")] UNSPEC_TH_VPCLIP_ITERATOR)
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     "  0,vu,  0, vu,vu, vu,   vu,    0, vu, vu,   vu,    0")))]
  "TARGET_VECTOR && TARGET_XTHEADVDOT"
  "th.<instr><tv_su>.w%o4\t%0,%3,%v4%p1"
  [(set_attr "type" "vnclip")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_narrow_vpnclip<tv_su><mode>_scalar"
  [(set (match_operand:<V_DOUBLE_TRUNC> 0 "register_operand"           "=&vd, vd, vr, vr,  vr,  vr")
	(if_then_else:<V_DOUBLE_TRUNC>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"                  " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 6 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (match_operand 9 "const_int_operand"                      "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI VXRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC>
	    [(match_operand:TH_VDOT_WEXTI 3 "register_operand"                "  0,  0,  0,  0,   vr,   vr")
	     (match_operand 4 "pmode_reg_or_uimm5_operand"             " rK, rK, rK, rK,   rK,   rK")] UNSPEC_TH_VPCLIP_ITERATOR)
	  (match_operand:<V_DOUBLE_TRUNC> 2 "vector_merge_operand"     " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_VECTOR && TARGET_XTHEADVDOT"
  "th.<instr><tv_su>.w%o4\t%0,%3,%4%p1"
  [(set_attr "type" "vnclip")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")])

(define_insn "@pred_vpwadd<tv_su><mode>"
  [(set (match_operand:TH_VDOT_WEXTI 0 "register_operand"           "=&vd,vd")
	(if_then_else:TH_VDOT_WEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    " rK,rK")
	     (match_operand 6 "const_int_operand"        "  i, i")
	     (match_operand 7 "const_int_operand"        "  i, i")
	     (match_operand 8 "const_int_operand"        "  i, i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
          (unspec:TH_VDOT_WEXTI
	    [(match_operand:<V_DOUBLE_TRUNC> 3 "register_operand"	" vr,vr")
             (match_operand:<V_DOUBLE_TRUNC> 4 "register_operand"	" vr,vr")] UNSPEC_TH_WPWADD_ITERATOR)
	  (match_operand:TH_VDOT_WEXTI 2 "vector_merge_operand"     "")))]
  "TARGET_VECTOR && TARGET_XTHEADVDOT"
  "th.<instr><tv_su>.v%o4\t%0,%3,%4%p1"
  [(set_attr "type" "viwalu")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type_idx") (const_int 8))])

(define_insn "@pred_vpwadd<tv_su><mode>_scalar"
  [(set (match_operand:TH_VDOT_WEXTI 0 "register_operand"           "=&vd,vd")
	(if_then_else:TH_VDOT_WEXTI
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    " rK,rK")
	     (match_operand 6 "const_int_operand"        "  i, i")
	     (match_operand 7 "const_int_operand"        "  i, i")
	     (match_operand 8 "const_int_operand"        "  i, i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
          (unspec:TH_VDOT_WEXTI
	    [(match_operand:<V_DOUBLE_TRUNC> 3 "register_operand"	" vr,vr")
             (match_operand:QI 4 "register_operand"	"  r, r")] UNSPEC_TH_WPWADD_ITERATOR)
	  (match_operand:TH_VDOT_WEXTI 2 "vector_merge_operand"     "")))]
  "TARGET_VECTOR && TARGET_XTHEADVDOT"
  "th.<instr><tv_su>.v%o4\t%0,%3,%4%p1"
  [(set_attr "type" "viwalu")
   (set_attr "mode" "<V_DOUBLE_TRUNC>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type_idx") (const_int 8))])

(define_insn "@pred_maqa<instr><tv_su><mode>"
  [(set (match_operand:VSI 0 "register_operand"           "=&vd,vd")
	(if_then_else:VSI
	  (unspec:<TH_VDOT_SI_VM>
	    [(match_operand:<TH_VDOT_SI_VM> 1 "vector_mask_operand" " vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    " rK,rK")
	     (match_operand 6 "const_int_operand"        "  i, i")
	     (match_operand 7 "const_int_operand"        "  i, i")
	     (match_operand 8 "const_int_operand"        "  i, i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
          (unspec:VSI
	    [(match_operand:VSI 2 "register_operand"	"  0, 0")
	     (match_operand:<TH_VDOT_SI_MAQA> 3 "register_operand"	" vr,vr")
             (match_operand:<TH_VDOT_SI_MAQA> 4 "register_operand"	" vr,vr")] UNSPEC_TH_VMAQA_AND_VPMAQA_ITERATOR)
	  (match_dup 2)))]
  "TARGET_VECTOR && TARGET_XTHEADVDOT"
  "th.<instr><tv_su>.v%o3\t%0,%3,%4%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type_idx") (const_int 8))])

(define_insn "@pred_maqa<instr><tv_su><mode>_scalar"
  [(set (match_operand:VSI 0 "register_operand"           "=&vd,vd")
	(if_then_else:VSI
	  (unspec:<TH_VDOT_SI_VM>
	    [(match_operand:<TH_VDOT_SI_VM> 1 "vector_mask_operand" " vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"    " rK,rK")
	     (match_operand 6 "const_int_operand"        "  i, i")
	     (match_operand 7 "const_int_operand"        "  i, i")
	     (match_operand 8 "const_int_operand"        "  i, i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
          (unspec:VSI
	    [(match_operand:VSI 2 "register_operand"	"  0, 0")
	     (match_operand:QI 3 "register_operand"	" r, r")
             (match_operand:<TH_VDOT_SI_MAQA> 4 "register_operand"	" vr,vr")] UNSPEC_TH_VMAQA_AND_VPMAQA_ITERATOR)
	  (match_dup 2)))]
  "TARGET_VECTOR && TARGET_XTHEADVDOT"
  "th.<instr><tv_su>.v%o3\t%0,%3,%4%p1"
  [(set_attr "type" "vimuladd")
   (set_attr "mode" "<MODE>")
   (set_attr "merge_op_idx" "2")
   (set_attr "vl_op_idx" "5")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[6])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[7])"))
   (set (attr "avl_type_idx") (const_int 8))])

;; Subset of VI with LMUL=1
(define_mode_iterator VI_LMUL1 [
  RVVM1QI RVVM1HI RVVM1SI (RVVM1DI "TARGET_VECTOR_ELEN_64")
])

;; Subset of VI with LMUL=1
(define_mode_iterator VI_LMUL1_S [
  RVVM1QI RVVM1HI RVVM1SI (RVVM1DI "TARGET_VECTOR_ELEN_64 && TARGET_64BIT")
])

;; Subset of VF_ZVFHMIN with LMUL=1
(define_mode_iterator VF_NOBF_LMUL1 [
  (RVVM1HF "TARGET_VECTOR_ELEN_FP_16")
  (RVVM1SF "TARGET_VECTOR_ELEN_FP_32")
  (RVVM1DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator VF_LMUL1 [VF_NOBF_LMUL1 (RVVM1BF "TARGET_VECTOR_ELEN_BF_16")])

(define_insn "@pred_vcpx0_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 3 "const_int_operand" "i")
       (match_operand 4 "const_int_operand" "i")
       (match_operand 5 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VI_LMUL1 2 "register_operand" "vr")]
    UNSPECV_VCPX0)]
  "TARGET_XXTCCEV"
  "vcpx0\t%0,%2%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx0_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 3 "const_int_operand" "i")
       (match_operand 4 "const_int_operand" "i")
       (match_operand 5 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VF_LMUL1 2 "register_operand" "vr")]
    UNSPECV_VCPX0)]
  "TARGET_XXTCCEV"
  "vcpx0\t%0,%2%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[5])"))])

(define_insn "@pred_vcpx1_<mode>"
  [(set (match_operand:VI_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VI_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 5 "const_int_operand" "i, i, i, i")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VI_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VI_LMUL1 4 "register_operand" "vr, vr, vr, vr")] UNSPECV_VCPX1)
      (match_operand:VI_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx1\t%1,%0,%4%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx1_<mode>"
  [(set (match_operand:VF_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VF_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 5 "const_int_operand" "i, i, i, i")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VF_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VF_LMUL1 4 "register_operand" "vr, vr, vr, vr")] UNSPECV_VCPX1)
      (match_operand:VF_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx1\t%1,%0,%4%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[7])"))])

(define_insn "@pred_vcpx2_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 4 "const_int_operand" "i")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VI_LMUL1 2 "register_operand" "vr")
     (match_operand 3 "const_0_to_31_operand" "i")]
    UNSPECV_VCPX2)]
  "TARGET_XXTCCEV"
  "vcpx2\t%0,%2,%3%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx2_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 4 "const_int_operand" "i")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VF_LMUL1 2 "register_operand" "vr")
     (match_operand 3 "const_0_to_31_operand" "i")]
    UNSPECV_VCPX2)]
  "TARGET_XXTCCEV"
  "vcpx2\t%0,%2,%3%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[6])"))])

(define_insn "@pred_vcpx3_<mode>"
  [(set (match_operand:VI_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VI_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (match_operand 8 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VI_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VI_LMUL1 4 "register_operand" "vr, vr, vr, vr")
                  (match_operand 5 "const_0_to_31_operand" "i, i, i, i")] UNSPECV_VCPX3)
      (match_operand:VI_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx3\t%1,%0,%4,%5%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx3_<mode>"
  [(set (match_operand:VF_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VF_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (match_operand 8 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VF_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VF_LMUL1 4 "register_operand" "vr, vr, vr, vr")
                  (match_operand 5 "const_0_to_31_operand" "i, i, i, i")] UNSPECV_VCPX3)
      (match_operand:VF_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx3\t%1,%0,%4,%5%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_vcpx4_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (match_operand 7 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VI_LMUL1 2 "register_operand" "vr")
     (match_operand:VI_LMUL1 3 "register_operand" "vr")
     (match_operand:VI_LMUL1 4 "register_operand" "vr")]
    UNSPECV_VCPX4)]
  "TARGET_XXTCCEV"
  "vcpx4\t%0,%2,%3,%4%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx4_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (match_operand 7 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VF_LMUL1 2 "register_operand" "vr")
     (match_operand:VF_LMUL1 3 "register_operand" "vr")
     (match_operand:VF_LMUL1 4 "register_operand" "vr")]
    UNSPECV_VCPX4)]
  "TARGET_XXTCCEV"
  "vcpx4\t%0,%2,%3,%4%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[7])"))])

(define_insn "@pred_vcpx5_<mode>"
  [(set (match_operand:VI_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VI_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (match_operand 8 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VI_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VI_LMUL1 4 "register_operand" "vr, vr, vr, vr")
                  (match_operand:VI_LMUL1 5 "register_operand" "vr, vr, vr, vr")] UNSPECV_VCPX5)
      (match_operand:VI_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx5\t%1,%0,%4,%5%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx5_<mode>"
  [(set (match_operand:VF_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VF_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (match_operand 8 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VF_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VF_LMUL1 4 "register_operand" "vr, vr, vr, vr")
                  (match_operand:VF_LMUL1 5 "register_operand" "vr, vr, vr, vr")] UNSPECV_VCPX5)
      (match_operand:VF_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx5\t%1,%0,%4,%5%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_vcpx6_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VI_LMUL1_S 2 "register_operand" "vr")
     (match_operand:VI_LMUL1_S 3 "register_operand" "vr")
     (match_operand:<VEL> 4 "register_operand" "r")]
    UNSPECV_VCPX6)]
  "TARGET_XXTCCEV"
  "vcpx6\t%0,%2,%3,%4%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx7_<mode>"
  [(set (match_operand:VI_LMUL1_S 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VI_LMUL1_S
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VI_LMUL1_S [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VI_LMUL1_S 4 "register_operand" "vr, vr, vr, vr")
                  (match_operand:<VEL> 5 "register_operand" "r, r, r, r")] UNSPECV_VCPX7)
      (match_operand:VI_LMUL1_S 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx7\t%1,%0,%4,%5%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx8_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (match_operand 7 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VI_LMUL1 2 "register_operand" "vr")
     (match_operand:VI_LMUL1 3 "register_operand" "vr")
     (match_operand 4 "const_0_to_31_operand" "i")]
    UNSPECV_VCPX8)]
  "TARGET_XXTCCEV"
  "vcpx8\t%0,%2,%3,%4%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")])

(define_insn "@pred_vcpx8_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (match_operand 7 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VF_LMUL1 2 "register_operand" "vr")
     (match_operand:VF_LMUL1 3 "register_operand" "vr")
     (match_operand 4 "const_0_to_31_operand" "i")]
    UNSPECV_VCPX8)]
  "TARGET_XXTCCEV"
  "vcpx8\t%0,%2,%3,%4%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[7])"))])

(define_insn "@pred_vcpx9_<mode>"
  [(unspec_volatile
    [(unspec:<VM>
      [(match_operand:<VM> 1 "vector_mask_operand" "vmWc1")
       (match_operand 5 "const_int_operand" "i")
       (match_operand 6 "const_int_operand" "i")
       (match_operand 7 "const_int_operand" "i")
       (reg:SI VL_REGNUM)
       (reg:SI VTYPE_REGNUM)
       (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
     (match_operand 0 "const_0_3_operand" "i")
     (match_operand:VF_LMUL1 2 "register_operand" "vr")
     (match_operand:VF_LMUL1 3 "register_operand" "vr")
     (match_operand:<VEL> 4 "register_operand" "f")]
    UNSPECV_VCPX9)]
  "TARGET_XXTCCEV"
  "vcpx9\t%0,%2,%3,%4%p1"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[7])"))])

(define_insn "@pred_vcpx10_<mode>"
  [(set (match_operand:VF_LMUL1 0 "register_operand" "=vd, vd, vr, vr")
    (if_then_else:VF_LMUL1
      (unspec:<VM>
        [(match_operand:<VM> 2 "vector_mask_operand" "vm, vm, Wc1, Wc1")
         (match_operand 6 "const_int_operand" "i, i, i, i")
         (match_operand 7 "const_int_operand" "i, i, i, i")
         (match_operand 8 "const_int_operand" "i, i, i, i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)
         (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
      (unspec_volatile:VF_LMUL1 [(match_operand 1 "const_0_3_operand" "i, i, i, i")
                  (match_operand:VF_LMUL1 4 "register_operand" "vr, vr, vr, vr")
                  (match_operand:<VEL> 5 "register_operand" "f, f, f, f")] UNSPECV_VCPX10)
      (match_operand:VF_LMUL1 3 "vector_merge_operand" "vu, 0, vu, 0")))]
  "TARGET_XXTCCEV"
  "vcpx10\t%1,%0,%4,%5%p2"
  [(set_attr "type" "vcpx")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	  (symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

;; XTheadVsfa
(define_int_iterator VSFA_IT [UNSPEC_TH_VFEXP2 UNSPEC_TH_VFTANH UNSPEC_TH_VFSIG UNSPEC_TH_VFREC])
(define_int_attr vsfa_insn [(UNSPEC_TH_VFEXP2 "vfexp2") (UNSPEC_TH_VFTANH "vftanh") (UNSPEC_TH_VFSIG "vfsig") (UNSPEC_TH_VFREC "vfrec")])

(define_insn "@th_pred_<vsfa_insn><mode>"
  [(set (match_operand:TH_VSF 0 "register_operand"           "=vd, vd, vr, vr")
	(if_then_else:TH_VSF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand" " vm, vm,Wc1,Wc1")
	     (match_operand 4 "vector_length_operand"    " rK, rK, rK, rK")
	     (match_operand 5 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 6 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 7 "const_int_operand"        "  i,  i,  i,  i")
	     (match_operand 8 "const_int_operand"        "  i,  i,  i,  i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:TH_VSF
	    [(match_operand:TH_VSF 3 "register_operand"       " vr, vr, vr, vr")] VSFA_IT)
	  (match_operand:TH_VSF 2 "vector_merge_operand"     " vu,  0, vu,  0")))]
  "TARGET_VECTOR"
  "<vsfa_insn>.v\t%0,%3%p1"
  [(set_attr "type" "vfsqrt")
   (set_attr "mode" "<MODE>")
   (set_attr "vl_op_idx" "4")
   (set (attr "ta") (symbol_ref "riscv_vector::get_ta(operands[5])"))
   (set (attr "ma") (symbol_ref "riscv_vector::get_ma(operands[6])"))
   (set (attr "avl_type_idx") (const_int 7))
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])
