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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "gimple-expr.h"
#include "memmodel.h"
#include "expmed.h"
#include "profile-count.h"
#include "optabs.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "stor-layout.h"
#include "expr.h"
#include "langhooks.h"
#include "stringpool.h"
#include "attribs.h"

#include "riscv-protos.h"
#include "function.h"
#include "xregex.h"

#define RVM_TYPE_ATTR_NAME "RVM type"

/* If TYPE is an ABI-defined RVM type, return its attribute descriptor,
   otherwise return null.  */

tree xt_rvm_lookup_matrix_type_attribute (const_tree type)
{
  if (type == error_mark_node)
    return NULL_TREE;
  return lookup_attribute (RVM_TYPE_ATTR_NAME, TYPE_ATTRIBUTES (type));
}

/* Add type attributes to builtin type tree, currently only the mangled name. */

static void
xt_rvm_add_matrix_type_attribute (tree type, const char *mangled_name)
{
  tree mangled_name_tree = get_identifier (mangled_name);
  tree value = tree_cons (NULL_TREE, mangled_name_tree, NULL_TREE);
  TYPE_ATTRIBUTES (type)
    = tree_cons (get_identifier (RVM_TYPE_ATTR_NAME),
		 value, TYPE_ATTRIBUTES (type));
}

/* Force TYPE to be a sizeless type.  */
static void
xt_rvm_make_matrix_type_sizeless (tree type)
{
  TYPE_ATTRIBUTES (type) = tree_cons (get_identifier ("RVM sizeless type"),
				      NULL_TREE, TYPE_ATTRIBUTES (type));
}

/* Return true if TYPE is a sizeless type.  */
bool
xt_rvm_sizeless_matrix_type_p (const_tree type)
{
  if (type == error_mark_node)
    return NULL_TREE;
  return lookup_attribute ("RVM sizeless type", TYPE_ATTRIBUTES (type));
}

/* If TYPE is a built-in type defined by the RVM ABI, return the mangled name,
   otherwise return NULL.  */
const char *
xt_rvm_mangle_builtin_type (const_tree type)
{
  if (TYPE_NAME (type) && TREE_CODE (TYPE_NAME (type)) == TYPE_DECL)
    type = TREE_TYPE (TYPE_NAME (type));
  if (tree attr = xt_rvm_lookup_matrix_type_attribute (type))
    if (tree id = TREE_VALUE (chain_index (0, TREE_VALUE (attr))))
      return IDENTIFIER_POINTER (id);
  return NULL;
}

/* Static information about each matrix type.  */
struct matrix_type_info
{
  /* The name of the type as declared by riscv_matrix.h
     which is recommend to use. For example: 'vint32m1_t'.  */
  const char *name;

  /* ABI name of matrix type. The type is always available
     under this name, even when riscv_matrix.h isn't included.
     For example:  '__rvm_int32m1_t'.  */
  const char *abi_name;

  /* The C++ mangling of ABI_NAME.  */
  const char *mangled_name;
};

/* Static information about each RVM type.  */
static CONSTEXPR const matrix_type_info matrix_types[] = {
#define DEF_RVM_TYPE(NAME, NCHARS, ABI_NAME, ARGS...) \
  {#NAME, #ABI_NAME, "u" #NCHARS #ABI_NAME},
#include "xuantie-ext-builtins-matrix.def"
};

class registered_function;
class builtin_function;
class function_builder;

static GTY (()) vec<registered_function *, va_gc> *registered_functions;

enum rvm_base_type
{
#define DEF_RVM_BASE_TYPE(NAME, ARGS...) RVM_BASE_##NAME,
#include "xuantie-ext-builtins-matrix.def"
  NUM_BASE_TYPES
};

enum rvm_type_index
{
#define DEF_RVM_TYPE(NAME, NCHARS, ABI_NAME, SCALAR_TYPE, MATRIX_MODE,	    \
		     SUFFIX, SCALAR_SUFFIX)				    \
  RVM_TYPE_##NAME,
#include "xuantie-ext-builtins-matrix.def"
  NUM_RVM_TYPE,
  NUM_RVM_INVALID = NUM_RVM_TYPE
};

/* Builtin types that are used to register RVM intrinsics.  */
struct GTY (()) rvm_builtin_types_t
{
  tree matrix;
  tree scalar;
  tree matrix_ptr;
  tree scalar_ptr;
  tree scalar_const_ptr;
};

rvm_builtin_types_t builtin_types[NUM_RVM_TYPE + 1];

machine_mode xt_rvm_get_matrix_mode (rvm_type_index type_index)
{
  switch (type_index)
  {
#define DEF_RVM_TYPE(NAME, NCHARS, ABI_NAME, SCALAR_TYPE, MATRIX_MODE,	    \
		     SUFFIX, SCALAR_SUFFIX)				    \
    case RVM_TYPE_##NAME:						   \
      return MATRIX_MODE##mode;
#include "xuantie-ext-builtins-matrix.def"
  default:
    gcc_unreachable ();
  }
  gcc_unreachable ();
}

struct rvm_arg_type_info
{
  CONSTEXPR rvm_arg_type_info (rvm_base_type base_type_in)
      : base_type (base_type_in)
  {
  }
  enum rvm_base_type base_type;

  tree get_tree_type (rvm_type_index) const;
};

const char *xt_rvm_get_index_suffix (rvm_type_index type_index)
{
  switch (type_index)
  {
#define DEF_RVM_TYPE(NAME, NCHARS, ABI_NAME, SCALAR_TYPE, MATRIX_MODE,	    \
		     SUFFIX, SCALAR_SUFFIX)				    \
    case RVM_TYPE_##NAME:						   \
      return #SUFFIX;
#include "xuantie-ext-builtins-matrix.def"
  default:
    gcc_unreachable ();
  }
  gcc_unreachable ();
}

const char *xt_rvm_get_function_name_prefix ()
{
  return "__riscv_th";
}

tree rvm_arg_type_info::get_tree_type (rvm_type_index type_index) const
{
  switch (base_type)
  {
#define DEF_RVM_BASE_TYPE(NAME, TYPE)	\
  case RVM_BASE_##NAME: 		\
  return TYPE;
#include "xuantie-ext-builtins-matrix.def"
  default:
    gcc_unreachable ();
  }
  gcc_unreachable ();
}

/* Enumerates the RVM operand types.  */
enum operand_type_index
{
  OP_TYPE_none,
#define DEF_RVM_OP_TYPE(NAME) OP_TYPE_##NAME,
#include "xuantie-ext-builtins-matrix.def"
  NUM_OP_TYPES
};

/* Static information about operand suffix for each RVM type.  */
const char *const operand_suffixes[NUM_OP_TYPES] = {
    "", /* OP_TYPE_none.  */
#define DEF_RVM_OP_TYPE(NAME) "_" #NAME,
#include "xuantie-ext-builtins-matrix.def"
};

struct rvm_type_info
{
  enum rvm_type_index index;
  uint64_t required_extensions;
};

struct rvm_op_info
{
  const rvm_type_info *types;
  const operand_type_index op;
  rvm_arg_type_info ret;
  const rvm_arg_type_info *args;
};

static const rvm_type_info all_ops[] = {
#define DEF_RVM_I_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_U_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_F_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_IM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_UM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_FM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info all_m1_ops[] = {
#define DEF_RVM_I_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_U_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_F_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info all_m2_ops[] = {
#define DEF_RVM_IM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_UM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_FM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info iu_ops[] = {
#define DEF_RVM_I_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#define DEF_RVM_U_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info si_di_ops[] = {
#define DEF_RVM_SI_DI_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info si_ops[] = {
#define DEF_RVM_SI_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info hfm2_ops[] = {
#define DEF_RVM_HFM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info sfm2_ops[] = {
#define DEF_RVM_SFM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info dfm2_ops[] = {
#define DEF_RVM_DFM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info dim2_ops[] = {
#define DEF_RVM_DIM2_OPS(TYPE, REQUIRE){RVM_TYPE_##TYPE, REQUIRE},
#include "xuantie-ext-builtins-matrix.def"
    {NUM_RVM_TYPE, 0}};

static const rvm_type_info none_ops[] = {{NUM_RVM_TYPE, 0}};

static CONSTEXPR const rvm_arg_type_info rvm_arg_type_info_end =
    rvm_arg_type_info (NUM_BASE_TYPES);

/* A list of args for size_t func () function.  */
static CONSTEXPR const rvm_arg_type_info void_args[] =
    {rvm_arg_type_info_end};

/* A list of args for size_t func () function.  */
static CONSTEXPR const rvm_arg_type_info end_args[] =
    {rvm_arg_type_info_end};

static CONSTEXPR const rvm_arg_type_info size_args[] =
    {rvm_arg_type_info (RVM_BASE_size), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info size_size_ops =
    {none_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_size),	/* Return type  */
     size_args /* Args */};

static CONSTEXPR const rvm_op_info void_void_ops =
    {none_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_void),	/* Return type  */
     void_args /* Args */};

static CONSTEXPR const rvm_op_info size_void_ops =
    {none_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_size),	/* Return type  */
     void_args /* Args */};

static CONSTEXPR const rvm_arg_type_info cptr_long_args[] =
    {rvm_arg_type_info (RVM_BASE_scalar_const_ptr),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_cptr_long_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     cptr_long_args /* Args */};

static CONSTEXPR const rvm_arg_type_info cptr_long_m_args[] =
    {rvm_arg_type_info (RVM_BASE_scalar_const_ptr),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info void_cptr_long_m_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_void),	/* Return type  */
     cptr_long_m_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_size_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_size_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_mv,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_size_args /* Args */};

static CONSTEXPR const rvm_arg_type_info scalar_args[] =
    {rvm_arg_type_info (RVM_BASE_scalar),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_scalar_ops =
    {iu_ops,					/* Types  */
     OP_TYPE_m_x,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     scalar_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_scalar_size_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_scalar),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_scalar_size_ops =
    {iu_ops,					/* Types  */
     OP_TYPE_m_x,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_scalar_size_args /* Args */};

static CONSTEXPR const rvm_op_info scalar_m_size_ops =
    {iu_ops,					/* Types  */
     OP_TYPE_x_m,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_scalar),	/* Return type  */
     m_size_args /* Args */};

static CONSTEXPR const rvm_op_info m_void_ops =
    {all_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     void_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_size_m_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m2_m2_size_m_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_size_m_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_size_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m2_size_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),	/* Return type  */
     m2_size_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_m_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_m_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mm,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_m_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_m_size_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_m_size_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mv,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_m_size_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_scalar_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_scalar),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_scalar_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_scalar_args /* Args */};

static CONSTEXPR const rvm_op_info si_m_m_m_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mm,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_m_args /* Args */};

static CONSTEXPR const rvm_op_info si_m_m_m_size_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mv,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_m_size_args /* Args */};

static CONSTEXPR const rvm_op_info si_m_m_scalar_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_scalar_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_um_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_um_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mm,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_um_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_um_size_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_um_size_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mv,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_um_size_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_unsigned_scalar_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_unsigned_scalar),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_unsigned_scalar_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_unsigned_scalar_args /* Args */};

static CONSTEXPR const rvm_op_info mi8_m_um_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mm,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix_mi8),	/* Return type  */
     m_um_args /* Args */};

static CONSTEXPR const rvm_op_info mi8_m_um_size_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mv,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix_mi8),	/* Return type  */
     m_um_size_args /* Args */};

static CONSTEXPR const rvm_op_info mi8_m_unsigned_scalar_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix_mi8),	/* Return type  */
     m_unsigned_scalar_args /* Args */};

static CONSTEXPR const rvm_arg_type_info um_um_args[] =
    {rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mu8_um_um_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mm,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix_mu8),	/* Return type  */
     um_um_args /* Args */};

static CONSTEXPR const rvm_arg_type_info um_um_size_args[] =
    {rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info (RVM_BASE_size),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mu8_um_um_size_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mv,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix_mu8),	/* Return type  */
     um_um_size_args /* Args */};

static CONSTEXPR const rvm_arg_type_info um_unsigned_scalar_args[] =
    {rvm_arg_type_info (RVM_BASE_unsigned_matrix),
     rvm_arg_type_info (RVM_BASE_unsigned_scalar),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mu8_um_unsigned_scalar_ops =
    {si_di_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix_mu8),	/* Return type  */
     um_unsigned_scalar_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_mi8_mi8_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mi8),
     rvm_arg_type_info (RVM_BASE_matrix_mi8),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_mi8_mi8_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_mi8_mi8_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_mu8_mu8_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mu8),
     rvm_arg_type_info (RVM_BASE_matrix_mu8),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_mu8_mu8_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_mu8_mu8_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_mu8_mi8_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mu8),
     rvm_arg_type_info (RVM_BASE_matrix_mi8),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_mu8_mi8_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_mu8_mi8_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_mi8_mu8_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mi8),
     rvm_arg_type_info (RVM_BASE_matrix_mu8),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_mi8_mu8_ops =
    {si_ops,					/* Types  */
     OP_TYPE_mx,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m_mi8_mu8_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_m_m2_args[] =
    {rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_m_m2_ops =
    {hfm2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),	/* Return type  */
     m_m_m2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_m_m_args[] =
    {rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_m_m_ops =
    {sfm2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),	/* Return type  */
     m_m_m_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_m_m_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m2_m2_m_m_ops =
    {dfm2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_m_m_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m_mf16_mf16_args[] =
    {rvm_arg_type_info (RVM_BASE_double_trunc_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mf16),
     rvm_arg_type_info (RVM_BASE_matrix_mf16),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m_mf16_mf16_ops =
    {sfm2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_double_trunc_matrix),	/* Return type  */
     m_mf16_mf16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_mf32_mf32_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mf32),
     rvm_arg_type_info (RVM_BASE_matrix_mf32),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m2_m2_mf32_mf32_ops =
    {dfm2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_mf32_mf32_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_mi16_mi16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mi16),
     rvm_arg_type_info (RVM_BASE_matrix_mi16),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m2_mi16_mi16_ops =
    {dim2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_mi16_mi16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_mu16_mu16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mu16),
     rvm_arg_type_info (RVM_BASE_matrix_mu16),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m2_mu16_mu16_ops =
    {dim2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_mu16_mu16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_mu16_mi16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mu16),
     rvm_arg_type_info (RVM_BASE_matrix_mi16),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m2_mu16_mi16_ops =
    {dim2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_mu16_mi16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info m2_mi16_mu16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix),
     rvm_arg_type_info (RVM_BASE_matrix_mi16),
     rvm_arg_type_info (RVM_BASE_matrix_mu16),
     rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info m_m2_mi16_mu16_ops =
    {dim2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     m2_mi16_mu16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi8_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi8), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi8_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi8_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi16), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi16_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi32_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi32), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi32_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi32_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi64_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi64), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi64_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi64_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu8_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu8), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu8_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu8_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu16), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu16_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu32_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu32), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu32_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu32_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu64_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu64), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu64_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu64_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mf16_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mf16), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mf16_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mf16_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mf32_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mf32), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mf32_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mf32_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mf64_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mf64), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mf64_ops =
    {all_m1_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mf64_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi8x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi8x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi8x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi8x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi16x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi16x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi16x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi16x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi32x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi32x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi32x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi32x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mi64x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mi64x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mi64x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mi64x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu8x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu8x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu8x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu8x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu16x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu16x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu16x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu16x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu32x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu32x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu32x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu32x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mu64x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mu64x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mu64x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mu64x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mf16x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mf16x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mf16x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mf16x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mf32x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mf32x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mf32x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mf32x2_args /* Args */};

static CONSTEXPR const rvm_arg_type_info mf64x2_args[] =
    {rvm_arg_type_info (RVM_BASE_matrix_mf64x2), rvm_arg_type_info_end};

static CONSTEXPR const rvm_op_info mreinterpret_m_mf64x2_ops =
    {all_m2_ops,					/* Types  */
     OP_TYPE_none,				/* Suffix */
     rvm_arg_type_info (RVM_BASE_matrix),	/* Return type  */
     mf64x2_args /* Args */};

/* Static information about a set of functions.  */
struct function_group_info
{
  /* The base name, as a string.  */
  const char *base_name;

  /* Describes the behavior associated with the function base name.  */
  const builtin_function *const *functions;
  const rvm_op_info ops_infos;
};

class builtin_function
{
public:
  /* Shape the function name according to function_instance.  */
  virtual char *get_name (const function_group_info &group,
			  const rvm_type_info &type_info,
			  bool overloaded_p) const = 0;

  /* Define all functions associated with the given group.  */
  virtual void build (function_builder &, const function_group_info &) const = 0;

  /* Define all functions associated with the given group.  */
  virtual insn_code get_insn_code (const rvm_op_info &ops_infos,
				   const rvm_type_info &type_info) const = 0;

  virtual bool has_target_p (tree return_type) const = 0;

  virtual unsigned int get_mcfg_num () const = 0;

  virtual bool can_be_overloaded_p () const = 0;

  virtual bool check (location_t location, tree fndecl,
		      unsigned int nargs, tree *args) const = 0;
};

/* Describes a function decl.  */
class GTY (()) registered_function
{
public:
  const builtin_function *function;

  /* The decl itself.  */
  tree GTY ((skip)) decl;

  insn_code icode;
  bool has_target_p;
  /* The overload hash of non-overloaded intrinsic is determined by
     the overload name and argument list.  */
  const char *overload_name;

  /* The argument list part of the hash value. Add the unsigned/signed type
     and machine mode of each argument to the hash value. */
  vec<tree> GTY ((skip)) argument_types;

  /* True if the decl represents an overloaded function that needs to be
     resolved. */
  bool overloaded_p;

  /* The hash value to indicate the non-overloaded function. Generate hash value
     based on overload_name and argument_types. */
  hashval_t overloaded_hash () const;

  /* Generate hash value based on the overload_name and the argument list passed
     by the user when calling. */
  hashval_t overloaded_hash (const vec<tree, va_gc> &);
};

hashval_t
registered_function::overloaded_hash () const
{
  inchash::hash h;
  tree type;
  unsigned int unsigned_p, mode_p;
  h.add (overload_name, strlen (overload_name));

  /* For __riscv_th_mdup_m_x, only the first argument is used to generate the hash.  */
  if (argument_types.length () == 1
      && !(POINTER_TYPE_P (argument_types[0])
	  || AGGREGATE_TYPE_P (argument_types[0])))
  {
    h.add_int (TYPE_UNSIGNED (argument_types[0]));
    h.add_int (TYPE_MODE (argument_types[0]));
    return h.end ();
  }

  for (unsigned int i = 0; i < argument_types.length (); i++)
  {
    type = argument_types[i];
    bool addr_p = (POINTER_TYPE_P (type) || AGGREGATE_TYPE_P (type));
    unsigned_p = addr_p ? TYPE_UNSIGNED (TREE_TYPE (type))
			: TYPE_UNSIGNED (type);
    mode_p = addr_p ? TYPE_MODE (TREE_TYPE (type)) : TYPE_MODE (type);

    if (addr_p || xt_rvm_lookup_matrix_type_attribute (type))
    {
      h.add_int (unsigned_p);
      h.add_int (mode_p);
    }
  }

  return h.end ();
}

hashval_t
registered_function::overloaded_hash (const vec<tree, va_gc> &arglist)
{
  argument_types = vNULL;
  unsigned int len = arglist.length ();

  for (unsigned int i = 0; i < len; i++)
    argument_types.safe_push (TREE_TYPE (arglist[i]));

  return overloaded_hash ();
}

/* Hash traits for overload registered_function. */
struct non_overloaded_registered_function_hasher
    : nofree_ptr_hash<registered_function>
{
  static hashval_t hash (value_type);
  static bool equal (value_type, const compare_type &);
};

/* All registered function decls, hashed on overload_name and argument list
   of the registered_function.  This is used for looking up implementations
   of non-overloaded functions. */
static hash_table<non_overloaded_registered_function_hasher>
    *non_overloaded_function_table;

inline hashval_t
non_overloaded_registered_function_hasher::hash (value_type value)
{
  return value->overloaded_hash ();
}

inline bool
non_overloaded_registered_function_hasher::equal (value_type value,
						 const compare_type &key)
{
  return ((strcmp (value->overload_name, key->overload_name) == 0)
	  && value->overloaded_hash () == key->overloaded_hash ());
}

class function_builder
{
public:
  void allocate_argument_types (const function_group_info &group,
				const rvm_type_info &, vec<tree> &) const;
  void add_overloaded_function (const builtin_function &,
				const function_group_info &group,
				const rvm_type_info &type_info);
  void add_unique_function (const builtin_function &,
			    const function_group_info &,
			    tree, vec<tree> &, const rvm_type_info &);
  void register_function_group (const function_group_info &);

private:
  registered_function &add_function (const builtin_function &,
				    const char *,
				    tree, tree, const char *,
				    const vec<tree> &, bool, insn_code, bool);
};

/* Add a function called NAME with type FNTYPE and attributes ATTRS.
   INSTANCE describes what the function does.  */
registered_function &
function_builder::add_function (const builtin_function &function,
				const char *name, tree fntype, tree attrs,
				const char *overload_name,
				const vec<tree> &argument_types,
				bool overloaded_p,
				insn_code icode,
				bool has_target_p)
{
  unsigned int code = vec_safe_length (registered_functions);
  code = ((code) << RISCV_BUILTIN_SHIFT) + RISCV_BUILTIN_MATRIX;

  /* We need to be able to generate placeholders to enusre that we have a
     consistent numbering scheme for function codes between the C and C++
     frontends, so that everything ties up in LTO.

     Currently, tree-streamer-in.c:unpack_ts_function_decl_value_fields
     validates that tree nodes returned by TARGET_BUILTIN_DECL are non-NULL and
     some node other than error_mark_node. This is a holdover from when builtin
     decls were streamed by code rather than by value.

     Ultimately, we should be able to remove this validation of BUILT_IN_MD
     nodes and remove the target hook. For now, however, we need to appease the
     validation and return a non-NULL, non-error_mark_node node, so we
     arbitrarily choose integer_zero_node.  */
  tree decl = simulate_builtin_function_decl (input_location, name, fntype, code, NULL, attrs);

  registered_function &rfn = *ggc_alloc<registered_function>();
  rfn.function = &function;
  rfn.decl = decl;
  rfn.overload_name = overload_name ? xstrdup (overload_name) : NULL;
  rfn.argument_types = argument_types;
  rfn.overloaded_p = overloaded_p;

  rfn.icode = icode;
  rfn.has_target_p = has_target_p;
  vec_safe_push (registered_functions, &rfn);

  return rfn;
}

/* Add a built-in function for INSTANCE, with the argument types given
   by ARGUMENT_TYPES and the return type given by RETURN_TYPE. NAME is
   the "full" name for C function. OVERLOAD_NAME is the "short" name for
   C++ overloaded function. OVERLOAD_NAME can be nullptr because some
   instance doesn't have C++ overloaded function.  */
void function_builder::add_unique_function (const builtin_function &function,
					   const function_group_info &group,
					   tree return_type,
					   vec<tree> &argument_types,
					   const rvm_type_info &type_info)
{
  /* Also add the function under its overloaded alias, if we want
     a separate decl for each instance of an overloaded function.  */
  char *overload_name = (*(group.functions))->get_name (group, type_info, true);

  /* Add the function under its full (unique) name.  */
  char *name = (*(group.functions))->get_name (group, type_info, false);
  insn_code icode = (*(group.functions))->get_insn_code (group.ops_infos, type_info);

  bool has_target_p = (*(group.functions))->has_target_p (return_type);
  bool can_be_overloaded_p = (*(group.functions))->can_be_overloaded_p ();
  tree fntype = build_function_type_array (return_type, argument_types.length (),
					   argument_types.address ());
  registered_function &rfn = add_function (function, name, fntype, NULL_TREE, overload_name,
					   argument_types.copy (), can_be_overloaded_p,
					   icode, has_target_p);

  if (can_be_overloaded_p)
  {
    /* Enter the function into the non-overloaded hash table.  */
    hashval_t hash = rfn.overloaded_hash ();
    registered_function **rfn_slot
      = non_overloaded_function_table->find_slot_with_hash (&rfn, hash, INSERT);
    *rfn_slot = &rfn;
  }
}

/* Add overloaded function for gcc. */
void function_builder::add_overloaded_function (const builtin_function &function,
						const function_group_info &group,
						const rvm_type_info &type_info)
{
  unsigned i;
  registered_function *fn;

  bool can_be_overloaded_p = (*(group.functions))->can_be_overloaded_p ();
  if (can_be_overloaded_p)
  {
    char *name = (*(group.functions))->get_name (group, type_info, true);

    FOR_EACH_VEC_ELT (*registered_functions, i, fn)
      if (strcmp (fn->overload_name, name) == 0)
	return;

    tree fntype = build_function_type (void_type_node, void_list_node);
    add_function (function, name, fntype, NULL_TREE, name, vNULL, 1, (insn_code)0, 0);
  }
}

static void
build_one (const builtin_function &function, function_builder &b,
	   const function_group_info &group,
	   const rvm_type_info &type_info)
{
  /* Byte forms of non-tuple vlxusegei take 21 arguments.  */
  auto_vec<tree, 21> argument_types;
  tree return_type = group.ops_infos.ret.get_tree_type (type_info.index);
  b.allocate_argument_types (group, type_info, argument_types);

  b.add_overloaded_function (function, group, type_info);

  b.add_unique_function (function, group, return_type, argument_types, type_info);
}

/* Add a function instance for every operand && predicate && args
   combination in GROUP.  Take the function base name from GROUP && operand
   suffix from operand_suffixes && mode suffix from type_suffixes && predication
   suffix from predication_suffixes. Use apply_predication to add in
   the predicate.  */

class def_base : public builtin_function
{
  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();
    const char *base_name = group.base_name;

    if (overloaded_p || !can_be_overloaded_p ())
      return concat (prefix, "_", base_name, NULL);

    return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  void build (function_builder &b,
	     const function_group_info &group) const override
  {
    if (group.ops_infos.types[0].index == NUM_RVM_TYPE)
    {
      build_one (*this, b, group, group.ops_infos.types[0]);
    }
    else
    {
      for (unsigned int vec_type_idx = 0;
	   group.ops_infos.types[vec_type_idx].index != NUM_RVM_TYPE;
	   ++vec_type_idx)
	build_one (*this, b, group, group.ops_infos.types[vec_type_idx]);
    }
  }

  bool has_target_p (tree return_type) const override
  {
    return return_type != void_type_node;
  }

  unsigned int get_mcfg_num () const override
  {
    return 0;
  }

  bool can_be_overloaded_p () const override
  {
    return true;
  }

  bool check (location_t location ATTRIBUTE_UNUSED,
	     tree fndecl ATTRIBUTE_UNUSED,
	     unsigned int nargs ATTRIBUTE_UNUSED,
	     tree *args ATTRIBUTE_UNUSED) const override
  {
    return true;
  }
};

template <bool STORE_P, bool STREAM_P>
class mload_mstore_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    if (STORE_P)
    {
      if (STREAM_P)
	return code_for_rvm_msst (xt_rvm_get_matrix_mode (type_info.index), Pmode);
      else
	return code_for_rvm_mst (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    }
    else
    {
      if (STREAM_P)
	return code_for_rvm_msld (xt_rvm_get_matrix_mode (type_info.index), Pmode);
      else
	return code_for_rvm_mld (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    }
  }

  unsigned int get_mcfg_num () const override
  {
    return 2;
  }
};

class mzero_def : public def_base
{
  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();
    const char *base_name = group.base_name;

    return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    return code_for_rvm_mzero (xt_rvm_get_matrix_mode (type_info.index));
  }

  bool can_be_overloaded_p () const override
  {
    return false;
  }
};

class mrelease_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    return CODE_FOR_rvm_mrelease;
  }

  bool can_be_overloaded_p () const override
  {
    return false;
  }
};

template <bool MSETMROW_M_P, bool MSETMROW_N_P, int MSETMCOL_P>
class mset_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    if (MSETMROW_M_P)
      return code_for_rvm_mcfgm (Pmode);
    if (MSETMROW_N_P)
      return code_for_rvm_mcfgn (Pmode);
    if (MSETMCOL_P)
      return code_for_rvm_mcfgk (Pmode);
    gcc_unreachable ();
  }

  bool can_be_overloaded_p () const override
  {
    return false;
  }
};

template <bool XMLENB_P, bool XRLENB_P, int XMSIZE_P>
class mcsr_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    if (XMLENB_P)
      return CODE_FOR_rvm_read_xmlenb;
    if (XRLENB_P)
      return code_for_rvm_read_xrlenb (Pmode);
    if (XMSIZE_P)
      return code_for_rvm_read_xmsize (Pmode);
    gcc_unreachable ();
  }

  bool can_be_overloaded_p () const override
  {
    return false;
  }
};

class mundefined_def : public def_base
{
  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();
    const char *base_name = group.base_name;

    return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    return code_for_rvm_mundefined (xt_rvm_get_matrix_mode (type_info.index));
  }

  bool can_be_overloaded_p () const override
  {
    return false;
  }
};

template <bool MDUP_P>
class mmov_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    if (MDUP_P)
      return code_for_rvm_mdup_m_x (xt_rvm_get_matrix_mode (type_info.index));

    switch (ops_infos.op)
    {
    case OP_TYPE_m_x:
      return code_for_rvm_mmov_m_x (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    case OP_TYPE_mv:
      return code_for_rvm_mmov_mv (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    case OP_TYPE_x_m:
      return code_for_rvm_mmov_x_m (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    default:
      gcc_unreachable ();
    }
    gcc_unreachable ();
  }
};

template <bool MGET_P>
class tuple_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    if (MGET_P)
      return code_for_rvm_mget (xt_rvm_get_matrix_mode (type_info.index));
    else
      return code_for_rvm_mset (xt_rvm_get_matrix_mode (type_info.index));
  }

  bool check (location_t location, tree fndecl, unsigned int nargs, tree *args) const override
  {
    HOST_WIDE_INT actual;

    if (nargs < 1)
      return false;

    if (!tree_fits_uhwi_p (args[1]))
    {
      error_at (location,
	       "argument %d of %qE must be an integer constant"
	       " expression",
	       2, fndecl);
      return false;
    }

    actual = tree_to_uhwi (args[1]);
    if (!IN_RANGE (actual, 0, 1))
    {
      error_at (location,
	       "passing %wd to argument %d of %qE, which expects"
	       " a value in the range [%wd, %wd]",
	       actual, 2, fndecl, 0, 1);
      return false;
    }

    return true;
    ;
  }
};

template <int UNSPEC>
class alu_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    switch (ops_infos.op)
    {
    case OP_TYPE_mm:
      return code_for_rvm_mm (UNSPEC, xt_rvm_get_matrix_mode (type_info.index), Pmode);
    case OP_TYPE_mv:
      return code_for_rvm_mv (UNSPEC, xt_rvm_get_matrix_mode (type_info.index), Pmode);
    case OP_TYPE_mx:
      return code_for_rvm_mx (UNSPEC, xt_rvm_get_matrix_mode (type_info.index), Pmode);
    default:
      gcc_unreachable ();
    }
    gcc_unreachable ();
  }

  unsigned int get_mcfg_num () const override
  {
    return 2;
  }
};

class msra_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    switch (ops_infos.op)
    {
    case OP_TYPE_mm:
      return code_for_rvm_msra_mm (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    case OP_TYPE_mv:
      return code_for_rvm_msra_mv (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    case OP_TYPE_mx:
      return code_for_rvm_msra_mx (xt_rvm_get_matrix_mode (type_info.index), Pmode);
    default:
      gcc_unreachable ();
    }
    gcc_unreachable ();
  }

  unsigned int get_mcfg_num () const override
  {
    return 2;
  }
};

class fmmacc_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    switch (xt_rvm_get_matrix_mode (type_info.index))
    {
    case E_RVMM2HFmode:
      return code_for_rvm_fmmacc_h (Pmode);
    case E_RVMM2SFmode:
      return code_for_rvm_fmmacc_s (Pmode);
    case E_RVMM2DFmode:
      return code_for_rvm_fmmacc_d (Pmode);
    default:
      gcc_unreachable ();
    }
    gcc_unreachable ();
  }

  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();
    const char *base_name = "fmmacc";

    if (overloaded_p)
      return concat (prefix, "_", base_name, NULL);

    return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  unsigned int get_mcfg_num () const override
  {
    return 3;
  }
};

class fwmmacc_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    switch (xt_rvm_get_matrix_mode (type_info.index))
    {
    case E_RVMM2SFmode:
      return code_for_rvm_fwmmacc_h (Pmode);
    case E_RVMM2DFmode:
      return code_for_rvm_fwmmacc_s (Pmode);
    default:
      gcc_unreachable ();
    }
    gcc_unreachable ();
  }

  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();
    const char *base_name = "fwmmacc";

    if (overloaded_p)
      return concat (prefix, "_", base_name, NULL);

    return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  unsigned int get_mcfg_num () const override
  {
    return 3;
  }
};

template <int UNSPEC>
class mmaqa_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {

    switch (xt_rvm_get_matrix_mode (type_info.index))
    {
    case E_RVMM1SImode:
      return code_for_rvm_mmaqa_b (UNSPEC, Pmode);
    case E_RVMM2DImode:
      return code_for_rvm_mmaqa_h (UNSPEC, Pmode);
    default:
      gcc_unreachable ();
    }
    gcc_unreachable ();
  }

  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();

    const char *base_name = NULL;
    switch (UNSPEC)
    {
    case UNSPEC_MMAQASS:
      base_name = "mmaqa";
      break;
    case UNSPEC_MMAQAUU:
      base_name = "mmaqau";
      break;
    case UNSPEC_MMAQAUS:
      base_name = "mmaqaus";
      break;
    case UNSPEC_MMAQASU:
      base_name = "mmaqasu";
      break;
    default:
      gcc_unreachable ();
    }

    if (overloaded_p)
      return concat (prefix, "_", base_name, NULL);

    return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  unsigned int get_mcfg_num () const override
  {
    return 3;
  }
};

template <int UNSPEC>
class pmmaqa_def : public def_base
{
  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			   const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    return code_for_rvm_pmmaqa_b (UNSPEC, Pmode);
  }

  unsigned int get_mcfg_num () const override
  {
    return 3;
  }
};

template <machine_mode MODE>
class mreinterpret_def : public def_base
{

  char *get_name (const function_group_info &group ATTRIBUTE_UNUSED,
		  const rvm_type_info &type_info ATTRIBUTE_UNUSED,
		  bool overloaded_p ATTRIBUTE_UNUSED) const override
  {
    const char *prefix = xt_rvm_get_function_name_prefix ();

    const char *base_name = "mreinterpret";

    if (overloaded_p)
      return concat (prefix, "_", base_name, xt_rvm_get_index_suffix (type_info.index), NULL);

    return concat (prefix, "_", base_name, group.base_name, xt_rvm_get_index_suffix (type_info.index), NULL);
  }

  insn_code get_insn_code (const rvm_op_info &ops_infos ATTRIBUTE_UNUSED,
			  const rvm_type_info &type_info ATTRIBUTE_UNUSED) const override
  {
    return code_for_rvm_mreinterpret_m (xt_rvm_get_matrix_mode (type_info.index), MODE);
  }

  bool can_be_overloaded_p () const override
  {
    return true;
  }
};

static CONSTEXPR const mload_mstore_def<false, false> mld_obj;
static CONSTEXPR const mload_mstore_def<false, true> msld_obj;
static CONSTEXPR const mload_mstore_def<true, false> mst_obj;
static CONSTEXPR const mload_mstore_def<true, true> msst_obj;
static CONSTEXPR const mmov_def<false> mmov_obj;
static CONSTEXPR const mmov_def<true> mdup_obj;
static CONSTEXPR const mzero_def mzero_obj;
static CONSTEXPR const mundefined_def mundefined_obj;
static CONSTEXPR const alu_def<UNSPEC_MADD> madd_obj;
static CONSTEXPR const alu_def<UNSPEC_MSUB> msub_obj;
static CONSTEXPR const alu_def<UNSPEC_MMUL> mmul_obj;
static CONSTEXPR const alu_def<UNSPEC_MMULH> mmulh_obj;
static CONSTEXPR const msra_def msra_obj;
static CONSTEXPR const alu_def<UNSPEC_MN4CLIP> mn4clip_obj;
static CONSTEXPR const alu_def<UNSPEC_MN4CLIPU> mn4clipu_obj;
static CONSTEXPR const tuple_def<false> mset_obj;
static CONSTEXPR const tuple_def<true> mget_obj;
static CONSTEXPR const pmmaqa_def<UNSPEC_PMMAQASS> pmmaqa_obj;
static CONSTEXPR const pmmaqa_def<UNSPEC_PMMAQAUU> pmmaqau_obj;
static CONSTEXPR const pmmaqa_def<UNSPEC_PMMAQAUS> pmmaqaus_obj;
static CONSTEXPR const pmmaqa_def<UNSPEC_PMMAQASU> pmmaqasu_obj;
static CONSTEXPR const fmmacc_def fmmacc_obj;
static CONSTEXPR const fwmmacc_def fwmmacc_obj;

static CONSTEXPR const mmaqa_def<UNSPEC_MMAQASS> mmaqa_obj;
static CONSTEXPR const mmaqa_def<UNSPEC_MMAQAUU> mmaqau_obj;
static CONSTEXPR const mmaqa_def<UNSPEC_MMAQAUS> mmaqaus_obj;
static CONSTEXPR const mmaqa_def<UNSPEC_MMAQASU> mmaqasu_obj;
static CONSTEXPR const mrelease_def mrelease_obj;
static CONSTEXPR const mset_def<true, false, false> msetmrow_m_obj;
static CONSTEXPR const mset_def<false, true, false> msetmrow_n_obj;
static CONSTEXPR const mset_def<false, false, true> msetmcol_obj;
static CONSTEXPR const mcsr_def<true, false, false> xmlenb_obj;
static CONSTEXPR const mcsr_def<false, true, false> xrlenb_obj;
static CONSTEXPR const mcsr_def<false, false, true> xmsize_obj;

static CONSTEXPR const mreinterpret_def<RVMM1QImode> mreinterpret_i8_obj;
static CONSTEXPR const mreinterpret_def<RVMM1HImode> mreinterpret_i16_obj;
static CONSTEXPR const mreinterpret_def<RVMM1SImode> mreinterpret_i32_obj;
static CONSTEXPR const mreinterpret_def<RVMM1DImode> mreinterpret_i64_obj;
static CONSTEXPR const mreinterpret_def<RVMM1QImode> mreinterpret_u8_obj;
static CONSTEXPR const mreinterpret_def<RVMM1HImode> mreinterpret_u16_obj;
static CONSTEXPR const mreinterpret_def<RVMM1SImode> mreinterpret_u32_obj;
static CONSTEXPR const mreinterpret_def<RVMM1DImode> mreinterpret_u64_obj;
static CONSTEXPR const mreinterpret_def<RVMM1HFmode> mreinterpret_f16_obj;
static CONSTEXPR const mreinterpret_def<RVMM1SFmode> mreinterpret_f32_obj;
static CONSTEXPR const mreinterpret_def<RVMM1DFmode> mreinterpret_f64_obj;

static CONSTEXPR const mreinterpret_def<RVMM2QImode> mreinterpret_i8x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2HImode> mreinterpret_i16x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2SImode> mreinterpret_i32x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2DImode> mreinterpret_i64x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2QImode> mreinterpret_u8x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2HImode> mreinterpret_u16x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2SImode> mreinterpret_u32x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2DImode> mreinterpret_u64x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2HFmode> mreinterpret_f16x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2SFmode> mreinterpret_f32x2_obj;
static CONSTEXPR const mreinterpret_def<RVMM2DFmode> mreinterpret_f64x2_obj;

#define DEF_RVM_FUNCTION(NAME, DEF, OPS_INFO)		\
  const builtin_function *const NAME = &DEF##_obj;
#include "xuantie-ext-builtins-matrix.def"

/* A list of all RVM intrinsic functions.  */
static function_group_info function_groups[] = {
#define DEF_RVM_FUNCTION(NAME, DEF, OPS_INFO) {#NAME, &NAME, OPS_INFO},
#include "xuantie-ext-builtins-matrix.def"
};

/* Allocate arguments of the function.  */
void function_builder::allocate_argument_types (const function_group_info &group,
						const rvm_type_info &type_info,
						vec<tree> &argument_types) const
{
  for (unsigned int i = 0;
       group.ops_infos.args[i].base_type != NUM_BASE_TYPES; ++i)
    argument_types.quick_push (
	group.ops_infos.args[i].get_tree_type (type_info.index));

  for (unsigned int i = 0; i < (*(group.functions))->get_mcfg_num (); ++i)
    argument_types.quick_push (size_type_node);
}

/* Register all the functions in GROUP.  */
void function_builder::register_function_group (const function_group_info &group)
{
  (*group.functions)->build (*this, group);
}

static rtx
xt_rvm_expand_builtins (unsigned int code, tree exp, rtx target)
{
  if (!TARGET_XTHEADMATRIX)
    return NULL_RTX;

  registered_function &rfn = *(*registered_functions)[code];
  return riscv_expand_builtin_direct (rfn.icode, target, exp, rfn.has_target_p);
}

bool xt_rvm_check_builtin_call (location_t location, vec<location_t>, unsigned int code,
				tree fndecl, unsigned int nargs, tree *args)
{
  const registered_function &rfn = *(*registered_functions)[code];
  return rfn.function->check (location, fndecl, nargs, args);
}

tree xt_rvm_resolve_overloaded_builtin (location_t loc, unsigned int code, tree fndecl,
					vec<tree, va_gc> *arglist)
{
  if (code >= vec_safe_length (registered_functions))
    return NULL_TREE;

  registered_function *rfun = (*registered_functions)[code];

  if (!rfun || !rfun->overloaded_p)
    return NULL_TREE;

  /* According to the rvm intrinisc doc, we have no such overloaded function
     with empty args.  Unfortunately, we register the empty args function as
     overloaded for avoiding conflict.  Thus, there will actual one register
     function after return NULL_TREE back to the middle-end, and finally result
     in ICE when expanding.

     Here we report error when overloaded function with empty args.  */
  if (rfun->overloaded_p && arglist->length () == 0)
    error_at (loc, "no matching function call to %qE with empty arguments",
	      fndecl);

  hashval_t hash = rfun->overloaded_hash (*arglist);
  registered_function *rfn = non_overloaded_function_table->find_with_hash (rfun, hash);

  return rfn ? rfn->decl : NULL_TREE;
}

/* Helper function for register a single built-in RVM ABI type.  */
static void
xt_rvm_register_builtin_type (rvm_type_index type, tree eltype, machine_mode mode)
{
  builtin_types[type].scalar = eltype;
  builtin_types[type].scalar_ptr = build_pointer_type (eltype);
  builtin_types[type].scalar_const_ptr
    = build_pointer_type (build_qualified_type (eltype, TYPE_QUAL_CONST));

  if (!xt_rvm_mode_p (mode))
    return;

  tree vectype = build_vector_type_for_mode (eltype, mode);

  vectype = build_distinct_type_copy (vectype);
  gcc_assert (vectype == TYPE_MAIN_VARIANT (vectype));
  SET_TYPE_STRUCTURAL_EQUALITY (vectype);
  TYPE_ARTIFICIAL (vectype) = 1;
  TYPE_INDIVISIBLE_P (vectype) = 1;
  xt_rvm_add_matrix_type_attribute (vectype, matrix_types[type].mangled_name);
  xt_rvm_make_matrix_type_sizeless (vectype);
  lang_hooks.types.register_builtin_type (vectype, matrix_types[type].name);

  if (!vectype)
    return;

  builtin_types[type].matrix = vectype;
  builtin_types[type].matrix_ptr = build_pointer_type (vectype);
}

/* Register the built-in RVM ABI types.  */
static void
xt_rvm_register_builtin_matrix_types ()
{
  /* Get type node from get_typenode_from_name to prevent we have different type
     node define in different target libraries, e.g. int32_t defined as
     `long` in RV32/newlib-stdint, but `int` for RV32/glibc-stdint.h.
     NOTE: uint[16|32|64]_type_node already defined in tree.h.  */
  tree int8_type_node = get_typenode_from_name (INT8_TYPE);
  tree uint8_type_node = get_typenode_from_name (UINT8_TYPE);
  tree int16_type_node = get_typenode_from_name (INT16_TYPE);
  tree int32_type_node = get_typenode_from_name (INT32_TYPE);
  tree int64_type_node = get_typenode_from_name (INT64_TYPE);

  machine_mode mode;
#define DEF_RVM_TYPE(NAME, NCHARS, ABI_NAME, SCALAR_TYPE, MATRIX_MODE, ARGS...)	\
  mode = MATRIX_MODE##mode;							\
  xt_rvm_register_builtin_type (RVM_TYPE_##NAME, SCALAR_TYPE##_type_node, mode);
#include "xuantie-ext-builtins-matrix.def"
}

static void
xt_rvm_init_builtins ()
{
  if (!TARGET_XTHEADMATRIX)
    return;

  xt_rvm_register_builtin_matrix_types ();
  non_overloaded_function_table = new hash_table<non_overloaded_registered_function_hasher>(1023);

  function_builder builder;
  for (unsigned int i = 0; i < ARRAY_SIZE (function_groups); ++i)
    builder.register_function_group (function_groups[i]);
}

static void
xt_init_builtins ()
{
  if (!maybe_get_identifier ("__bf16"))
    lang_hooks.types.register_builtin_type (riscv_bfloat16_type_node, "__bf16");

  if (!maybe_get_identifier ("__fp16"))
    lang_hooks.types.register_builtin_type (riscv_float16_type_node,
					    "__fp16");

  xt_rvm_init_builtins ();
}

static tree
xt_builtin_decl (unsigned int code, bool)
{
  if (code >= vec_safe_length (registered_functions))
    return error_mark_node;

  return (*registered_functions)[code]->decl;
}
