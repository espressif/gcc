/* Prototypes for the Espressif XESPV vendor extension.
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

#ifndef GCC_RISCV_XESPV_PROTOS_H
#define GCC_RISCV_XESPV_PROTOS_H

/* Routines implemented in riscv-xespv.cc.  */
extern bool riscv_xespv_q_class_p (reg_class_t);
extern enum reg_class riscv_insn_base_reg_class (rtx_insn *);
extern bool riscv_xespv_ip_offset_p (rtx, int);
extern bool riscv_xespv_q_mem_p (rtx);
extern void riscv_expand_movti_xespv (rtx, rtx);
extern bool riscv_split_movti_xespv_p (rtx, rtx);
extern void riscv_split_movti_xespv (rtx, rtx);
extern const char *riscv_output_movti_xespv (rtx *);
extern bool riscv_xespv_memcpy_p (bool);
extern bool riscv_expand_block_move_xespv (rtx, rtx, rtx);
extern bool riscv_xespv_builtin_code_p (unsigned int);
extern tree riscv_xespv_builtin_decl (unsigned int);
extern void riscv_xespv_init_builtins (void);
extern rtx riscv_xespv_expand_builtin (unsigned int, tree, rtx);

/* Routines implemented in riscv-esp-asm.cc.  */
rtl_opt_pass * make_pass_riscv_esp_asm (gcc::context *ctxt);

#endif /* ! GCC_RISCV_XESPV_PROTOS_H */
