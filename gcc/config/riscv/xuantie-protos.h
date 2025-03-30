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

/* Routines implemented in xuantie-pass-mcfg.cc.  */
rtl_opt_pass * make_pass_mcfg (gcc::context *ctxt);

/* Routines implemented in xuantie-ext-builtins.cc.  */
extern bool xt_rvm_check_builtin_call (location_t, vec<location_t>, unsigned int, tree, unsigned int, tree *);
extern tree xt_rvm_resolve_overloaded_builtin (location_t, unsigned int, tree, vec<tree, va_gc> *);

struct pragma_intrinsic_flags
{
  int intrinsic_target_flags;

  int intrinsic_riscv_vector_elen_flags;
  int intrinsic_riscv_zvl_flags;
  int intrinsic_riscv_zvb_subext;
  int intrinsic_riscv_zvk_subext;
};

/* Routines implemented in riscv-c.cc.  */
extern void
riscv_pragma_intrinsic_flags_restore (struct pragma_intrinsic_flags *);

extern void
riscv_pragma_intrinsic_flags_pollute (struct pragma_intrinsic_flags *);