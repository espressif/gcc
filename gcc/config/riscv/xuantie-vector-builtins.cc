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

#ifdef XUANTIE_VECTOR_BUILTINS_SHAPES_H
extern const function_shape *const th_maqa;

extern const function_shape *const th_seg_loadb;
extern const function_shape *const th_seg_loadbu;
extern const function_shape *const th_seg_loadh;
extern const function_shape *const th_seg_loadhu;
extern const function_shape *const th_seg_loadw;
extern const function_shape *const th_seg_loadwu;
extern const function_shape *const th_seg_storeb;
extern const function_shape *const th_seg_storeh;
extern const function_shape *const th_seg_storew;
extern const function_shape *const th_seg_strideloadb;
extern const function_shape *const th_seg_strideloadbu;
extern const function_shape *const th_seg_strideloadh;
extern const function_shape *const th_seg_strideloadhu;
extern const function_shape *const th_seg_strideloadw;
extern const function_shape *const th_seg_strideloadwu;
extern const function_shape *const th_seg_stridestoreb;
extern const function_shape *const th_seg_stridestoreh;
extern const function_shape *const th_seg_stridestorew;
extern const function_shape *const th_seg_indexed_loadb;
extern const function_shape *const th_seg_indexed_loadbu;
extern const function_shape *const th_seg_indexed_loadh;
extern const function_shape *const th_seg_indexed_loadhu;
extern const function_shape *const th_seg_indexed_loadw;
extern const function_shape *const th_seg_indexed_loadwu;
extern const function_shape *const th_seg_indexed_storeb;
extern const function_shape *const th_seg_indexed_storeh;
extern const function_shape *const th_seg_indexed_storew;
extern const function_shape *const xxtccev;
#endif

#ifdef XUANTIE_VECTOR_BUILTINS_SHAPES_CC
/* maqa_def class.  */
struct th_maqa_def : public alu_def
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    build_all (b, group);
  }
};

/* th_seg_loadb_def class.  */
struct th_seg_loadb_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("b");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_loadbu_def class.  */
struct th_seg_loadbu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("bu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_loadh_def class.  */
struct th_seg_loadh_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("h");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_loadhu_def class.  */
struct th_seg_loadhu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("hu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_loadw_def class.  */
struct th_seg_loadw_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("w");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_loadwu_def class.  */
struct th_seg_loadwu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("wu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_storeb_def class.  */
struct th_seg_storeb_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("b");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_storeb_def class.  */
struct th_seg_storeh_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("h");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_storew_def class.  */
struct th_seg_storew_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("w");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_strideloadb_def class.  */
struct th_seg_strideloadb_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("b");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_strideloadbu_def class.  */
struct th_seg_strideloadbu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("bu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_strideloadh_def class.  */
struct th_seg_strideloadh_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>h.  */
    b.append_name ("h");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_strideloadhu_def class.  */
struct th_seg_strideloadhu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>h.  */
    b.append_name ("hu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_strideloadw_def class.  */
struct th_seg_strideloadw_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("w");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_strideloadwu_def class.  */
struct th_seg_strideloadwu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlsseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("wu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_stridestoreb_def class.  */
struct th_seg_stridestoreb_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vssseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("b");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_stridestoreh_def class.  */
struct th_seg_stridestoreh_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vssseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>h.  */
    b.append_name ("h");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_stridestorew_def class.  */
struct th_seg_stridestorew_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vssseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("w");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_loadb_def class.  */
struct th_seg_indexed_loadb_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("b");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_loadbu_def class.  */
struct th_seg_indexed_loadbu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("bu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_loadh_def class.  */
struct th_seg_indexed_loadh_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>h.  */
    b.append_name ("h");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_loadhu_def class.  */
struct th_seg_indexed_loadhu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>h.  */
    b.append_name ("hu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_loadw_def class.  */
struct th_seg_indexed_loadw_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("w");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_loadwu_def class.  */
struct th_seg_indexed_loadwu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vlxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("wu");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_storeb_def class.  */
struct th_seg_indexed_storeb_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vsxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>b.  */
    b.append_name ("b");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_storeh_def class.  */
struct th_seg_indexed_storeh_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vsxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>h.  */
    b.append_name ("h");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_seg_indexed_storew_def class.  */
struct th_seg_indexed_storew_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_vsxseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>w.  */
    b.append_name ("w");

    if (!overloaded_p)
      {
	/* vop<nf>b --> vop<nf>b_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>b_v --> vop<nf>b_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

SHAPE(th_maqa, th_maqa)

SHAPE(th_seg_loadb, th_seg_loadb)
SHAPE(th_seg_loadbu, th_seg_loadbu)
SHAPE(th_seg_loadh, th_seg_loadh)
SHAPE(th_seg_loadhu, th_seg_loadhu)
SHAPE(th_seg_loadw, th_seg_loadw)
SHAPE(th_seg_loadwu, th_seg_loadwu)
SHAPE(th_seg_storeb, th_seg_storeb)
SHAPE(th_seg_storeh, th_seg_storeh)
SHAPE(th_seg_storew, th_seg_storew)
SHAPE(th_seg_strideloadb, th_seg_strideloadb)
SHAPE(th_seg_strideloadbu, th_seg_strideloadbu)
SHAPE(th_seg_strideloadh, th_seg_strideloadh)
SHAPE(th_seg_strideloadhu, th_seg_strideloadhu)
SHAPE(th_seg_strideloadw, th_seg_strideloadw)
SHAPE(th_seg_strideloadwu, th_seg_strideloadwu)
SHAPE(th_seg_stridestoreb, th_seg_stridestoreb)
SHAPE(th_seg_stridestoreh, th_seg_stridestoreh)
SHAPE(th_seg_stridestorew, th_seg_stridestorew)
SHAPE(th_seg_indexed_loadb, th_seg_indexed_loadb)
SHAPE(th_seg_indexed_loadbu, th_seg_indexed_loadbu)
SHAPE(th_seg_indexed_loadh, th_seg_indexed_loadh)
SHAPE(th_seg_indexed_loadhu, th_seg_indexed_loadhu)
SHAPE(th_seg_indexed_loadw, th_seg_indexed_loadw)
SHAPE(th_seg_indexed_loadwu, th_seg_indexed_loadwu)
SHAPE(th_seg_indexed_storeb, th_seg_indexed_storeb)
SHAPE(th_seg_indexed_storeh, th_seg_indexed_storeh)
SHAPE(th_seg_indexed_storew, th_seg_indexed_storew)

/* xxtccev_def class.  */
struct xxtccev_def : public build_base
{
  /* Normalize vcpx<N>_f_frm or vcpx<N>_f to vcpx<N>.  */
  static void normalize_base_name (char *to, const char *from, int limit)
  {
    strncpy (to, from, limit - 1);
    char *suffix = strstr (to, "_f");

    if (suffix)
      *suffix = '\0';

    to[limit - 1] = '\0';
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vcpx<N> --> vcpx<N>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (type_suffixes[instance.type.index].scalar);
      }

    if (!overloaded_p && instance.base->has_rounding_mode_operand_p())
      b.append_name ("_rm");

    /* According to Xuantie coprocessor intrinsic document, it does not add
       "_mu" suffix for vcpx<N>_mu overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_mu)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }

  bool check (function_checker &c) const override
  {
    if (c.base->has_rounding_mode_operand_p ())
      {
	unsigned int frm_num = c.arg_num () - 1;

	return c.require_immediate (frm_num, FRM_STATIC_MIN, FRM_STATIC_MAX);
      }

    return true;
  }
};

SHAPE(xxtccev, xxtccev)

#endif

#ifdef XUANTIE_VECTOR_BUILTINS_BASES_H
extern const function_base *const th_vpnclip;
extern const function_base *const th_vpnclipu;
extern const function_base *const th_vpwadd;
extern const function_base *const th_vpwaddu;
extern const function_base *const th_vmaqa;
extern const function_base *const th_vmaqau;
extern const function_base *const th_vmaqasu;
extern const function_base *const th_vmaqaus;
extern const function_base *const th_vpmaqa;
extern const function_base *const th_vpmaqau;
extern const function_base *const th_vpmaqasu;
extern const function_base *const th_vpmaqaus;

extern const function_base *const th_vlsegb;
extern const function_base *const th_vlsegbu;
extern const function_base *const th_vlsegh;
extern const function_base *const th_vlseghu;
extern const function_base *const th_vlsegw;
extern const function_base *const th_vlsegwu;
extern const function_base *const th_vssegb;
extern const function_base *const th_vssegh;
extern const function_base *const th_vssegw;
extern const function_base *const th_vlssegb;
extern const function_base *const th_vlssegbu;
extern const function_base *const th_vlssegh;
extern const function_base *const th_vlsseghu;
extern const function_base *const th_vlssegw;
extern const function_base *const th_vlssegwu;
extern const function_base *const th_vsssegb;
extern const function_base *const th_vsssegh;
extern const function_base *const th_vsssegw;
extern const function_base *const th_vlxsegb;
extern const function_base *const th_vlxsegbu;
extern const function_base *const th_vlxsegh;
extern const function_base *const th_vlxseghu;
extern const function_base *const th_vlxsegw;
extern const function_base *const th_vlxsegwu;
extern const function_base *const th_vsxsegb;
extern const function_base *const th_vsxsegh;
extern const function_base *const th_vsxsegw;

extern const function_base *const xt_vcpx0;
extern const function_base *const xt_vcpx0_frm;
extern const function_base *const xt_vcpx1;
extern const function_base *const xt_vcpx1_frm;
extern const function_base *const xt_vcpx2;
extern const function_base *const xt_vcpx2_frm;
extern const function_base *const xt_vcpx3;
extern const function_base *const xt_vcpx3_frm;
extern const function_base *const xt_vcpx4;
extern const function_base *const xt_vcpx4_frm;
extern const function_base *const xt_vcpx5;
extern const function_base *const xt_vcpx5_frm;
extern const function_base *const xt_vcpx6;
extern const function_base *const xt_vcpx7;
extern const function_base *const xt_vcpx8;
extern const function_base *const xt_vcpx8_frm;
extern const function_base *const xt_vcpx9;
extern const function_base *const xt_vcpx9_frm;
extern const function_base *const xt_vcpx10;
extern const function_base *const xt_vcpx10_frm;

extern const function_base *const th_vfexp2;
extern const function_base *const th_vfexp2_frm;
extern const function_base *const th_vftanh;
extern const function_base *const th_vftanh_frm;
extern const function_base *const th_vfsig;
extern const function_base *const th_vfsig_frm;
extern const function_base *const th_vfrec;
extern const function_base *const th_vfrec_frm;
#endif

#ifdef XUANTIE_VECTOR_BUILTINS_BASES_CC

/* Implements th_vpnclip/th_vpnclipu.  */
template<int UNSPEC>
class th_vpnclip : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override { return false; }

  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XTHEADVDOT)
      error_at (e.location,"%qE requires 'xtheadvdot' for march", e.fndecl);

    switch (e.op_info->op)
      {
      case OP_TYPE_wx:
	return e.use_exact_insn (
	  code_for_pred_narrow_vpnclip_scalar (UNSPEC, e.vector_mode ()));
      case OP_TYPE_wv:
	return e.use_exact_insn (
	  code_for_pred_narrow_vpnclip (UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements th_vpwadd/th_vpwaddu.  */
template<int UNSPEC>
class th_vpwadd : public function_base
{
public:
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XTHEADVDOT)
      error_at (e.location,"%qE requires 'xtheadvdot' for march", e.fndecl);

    switch (e.op_info->op)
      {
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_vpwadd_scalar (UNSPEC, e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_vpwadd (UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements th_vmaqa/th_vmaqau/th_vmaqasu/th_vmaqaus/th_vpmaqa/th_vpmaqau/th_vpmaqasu/th_vpmaqaus.  */
template<int UNSPEC>
class th_maqa : public function_base
{
public:
  bool has_merge_operand_p () const override { return false; }

  rvv_base_type get_mask_type () const override { return RVV_BASE_maqa_mask; }

  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XTHEADVDOT)
      error_at (e.location,"%qE requires 'xtheadvdot' for march", e.fndecl);

    switch (e.op_info->op)
      {
      case OP_TYPE_vx:
	return e.use_exact_insn (
	  code_for_pred_maqa_scalar (UNSPEC, UNSPEC, e.vector_mode ()));
      case OP_TYPE_vv:
	return e.use_exact_insn (
	  code_for_pred_maqa (UNSPEC, UNSPEC, e.vector_mode ()));
      default:
	gcc_unreachable ();
      }
  }
};

/* Implements vlsegb.v.  */
template<int UNSPEC>
class th_vlsegb : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_unit_seg_load (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vlsegh.v.  */
template<int UNSPEC>
class th_vlsegh : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_unit_seg_load (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vlsegw.v.  */
template<int UNSPEC>
class th_vlsegw : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_unit_seg_load (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vssegb.v.  */
template<int UNSPEC>
class th_vssegb : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_unit_seg_store (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vssegh.v.  */
template<int UNSPEC>
class th_vssegh : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_unit_seg_store (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vssegw.v.  */
template<int UNSPEC>
class th_vssegw : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_unit_seg_store (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vlssegb.v.  */
template<int UNSPEC>
class th_vlssegb : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_strided_load (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vlssegh.v.  */
template<int UNSPEC>
class th_vlssegh : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_strided_load (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vlssegw.v.  */
template<int UNSPEC>
class th_vlssegw : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index pred) const override
  {
    return pred != PRED_TYPE_none;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_strided_load (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vsssegb.v.  */
template<int UNSPEC>
class th_vsssegb : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_strided_store (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vsssegh.v.  */
template<int UNSPEC>
class th_vsssegh : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_strided_store (UNSPEC, e.vector_mode ()));
  }
};

/* Implements vsssegw.v.  */
template<int UNSPEC>
class th_vsssegw : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_strided_store (UNSPEC, e.vector_mode ()));
  }
};

template<int UNSPEC>
class th_vlxsegb : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_indexed_load (UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template<int UNSPEC>
class th_vlxsegh : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_indexed_load (UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template<int UNSPEC>
class th_vlxsegw : public function_base
{
public:
  unsigned int call_properties (const function_instance &) const override
  {
    return CP_READ_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_indexed_load (UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template<int UNSPEC>
class th_vsxsegb : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_indexed_store (UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template<int UNSPEC>
class th_vsxsegh : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_indexed_store (UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template<int UNSPEC>
class th_vsxsegw : public function_base
{
public:
  bool apply_tail_policy_p () const override { return false; }
  bool apply_mask_policy_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }

  bool can_be_overloaded_p (enum predication_type_index) const override
  {
    return true;
  }

  rtx expand (function_expander &e) const override
  {
    gcc_assert (TARGET_XTHEADVECTOR);
    return e.use_exact_insn (
      code_for_pred_th_indexed_store (UNSPEC, e.vector_mode (), e.index_mode ()));
  }
};

template <bool MAY_REQUIRE_FRM, enum frm_op_type FRM_OP, bool HAS_TARGET>
class xt_vcpx_base : public function_base
{
public:
  bool has_merge_operand_p () const override { return HAS_TARGET; }

  bool has_rounding_mode_operand_p () const override
  {
    return FRM_OP == HAS_FRM;
  }

  bool may_require_frm_p () const override { return MAY_REQUIRE_FRM; }

  bool xt_has_idx_operand_p () const override { return true; }

  bool apply_vl_p () const override { return false; }

  unsigned int call_properties (const function_instance &) const override
  {
    return CP_WRITE_MEMORY;
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx0 : public xt_vcpx_base<true, FRM_OP, false>
{
public:  
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx0 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx1 : public xt_vcpx_base<true, FRM_OP, true>
{
public:  
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx1 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx2 : public xt_vcpx_base<true, FRM_OP, false>
{
public:
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx2 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx3 : public xt_vcpx_base<true, FRM_OP, true>
{
public:
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);
    
    return e.use_exact_insn (code_for_pred_vcpx3 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx4 : public xt_vcpx_base<true, FRM_OP, false>
{
public:
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx4 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx5 : public xt_vcpx_base<true, FRM_OP, true>
{
public:
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx5 (e.vector_mode()));
  }
};

class xt_vcpx6 : public xt_vcpx_base<false, NO_FRM, false>
{
public:  
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx6 (e.vector_mode()));
  }
};

class xt_vcpx7 : public xt_vcpx_base<false, NO_FRM, true>
{
public:  
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx7 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx8 : public xt_vcpx_base<true, FRM_OP, false>
{
public:  
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx8 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx9 : public xt_vcpx_base<true, FRM_OP, false>
{
public:  
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx9 (e.vector_mode()));
  }
};

template <enum frm_op_type FRM_OP = NO_FRM>
class xt_vcpx10 : public xt_vcpx_base<true, FRM_OP, true>
{
public:
  rtx expand (function_expander &e) const override
  {
    if (!TARGET_XXTCCEV)
      error_at (e.location,"%qE requires 'xxtccev' for march", e.fndecl);

    return e.use_exact_insn (code_for_pred_vcpx10 (e.vector_mode()));
  }
};

/* Implements vfexp2/vftanh/vfsig/vfrec.  */
template<int UNSPEC, enum frm_op_type FRM_OP = NO_FRM>
class th_unop : public function_base
{
public:
  bool has_rounding_mode_operand_p () const override
  {
    return FRM_OP == HAS_FRM;
  }

  bool may_require_frm_p () const override { return true; }

  rtx expand (function_expander &e) const override
  {
    return e.use_exact_insn (code_for_th_pred (UNSPEC, e.vector_mode ()));
  }
};

static CONSTEXPR const th_vpnclip<UNSPEC_TH_VPNCLIP> th_vpnclip_obj;
static CONSTEXPR const th_vpnclip<UNSPEC_TH_VPNCLIPU> th_vpnclipu_obj;

static CONSTEXPR const th_vpwadd<UNSPEC_TH_VPWADD> th_vpwadd_obj;
static CONSTEXPR const th_vpwadd<UNSPEC_TH_VPWADDU> th_vpwaddu_obj;

static CONSTEXPR const th_maqa<UNSPEC_TH_VMAQA> th_vmaqa_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VMAQAU> th_vmaqau_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VMAQASU> th_vmaqasu_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VMAQAUS> th_vmaqaus_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VPMAQA> th_vpmaqa_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VPMAQAU> th_vpmaqau_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VPMAQASU> th_vpmaqasu_obj;
static CONSTEXPR const th_maqa<UNSPEC_TH_VPMAQAUS> th_vpmaqaus_obj;

static CONSTEXPR const th_vlsegb<UNSPEC_TH_VLSEGB> th_vlsegb_obj;
static CONSTEXPR const th_vlsegb<UNSPEC_TH_VLSEGBU> th_vlsegbu_obj;
static CONSTEXPR const th_vlsegh<UNSPEC_TH_VLSEGH> th_vlsegh_obj;
static CONSTEXPR const th_vlsegh<UNSPEC_TH_VLSEGHU> th_vlseghu_obj;
static CONSTEXPR const th_vlsegw<UNSPEC_TH_VLSEGW> th_vlsegw_obj;
static CONSTEXPR const th_vlsegw<UNSPEC_TH_VLSEGWU> th_vlsegwu_obj;
static CONSTEXPR const th_vssegb<UNSPEC_TH_VLSEGB> th_vssegb_obj;
static CONSTEXPR const th_vssegh<UNSPEC_TH_VLSEGH> th_vssegh_obj;
static CONSTEXPR const th_vssegw<UNSPEC_TH_VLSEGW> th_vssegw_obj;
static CONSTEXPR const th_vlssegb<UNSPEC_TH_VLSSEGB> th_vlssegb_obj;
static CONSTEXPR const th_vlssegb<UNSPEC_TH_VLSSEGBU> th_vlssegbu_obj;
static CONSTEXPR const th_vlssegh<UNSPEC_TH_VLSSEGH> th_vlssegh_obj;
static CONSTEXPR const th_vlssegh<UNSPEC_TH_VLSSEGHU> th_vlsseghu_obj;
static CONSTEXPR const th_vlssegw<UNSPEC_TH_VLSSEGW> th_vlssegw_obj;
static CONSTEXPR const th_vlssegw<UNSPEC_TH_VLSSEGWU> th_vlssegwu_obj;
static CONSTEXPR const th_vsssegb<UNSPEC_TH_VLSSEGB> th_vsssegb_obj;
static CONSTEXPR const th_vsssegh<UNSPEC_TH_VLSSEGH> th_vsssegh_obj;
static CONSTEXPR const th_vsssegw<UNSPEC_TH_VLSSEGW> th_vsssegw_obj;
static CONSTEXPR const th_vlxsegb<UNSPEC_TH_VLXSEGB> th_vlxsegb_obj;
static CONSTEXPR const th_vlxsegb<UNSPEC_TH_VLXSEGBU> th_vlxsegbu_obj;
static CONSTEXPR const th_vlxsegh<UNSPEC_TH_VLXSEGH> th_vlxsegh_obj;
static CONSTEXPR const th_vlxsegh<UNSPEC_TH_VLXSEGHU> th_vlxseghu_obj;
static CONSTEXPR const th_vlxsegw<UNSPEC_TH_VLXSEGW> th_vlxsegw_obj;
static CONSTEXPR const th_vlxsegw<UNSPEC_TH_VLXSEGWU> th_vlxsegwu_obj;
static CONSTEXPR const th_vsxsegb<UNSPEC_TH_VLXSEGB> th_vsxsegb_obj;
static CONSTEXPR const th_vsxsegh<UNSPEC_TH_VLXSEGH> th_vsxsegh_obj;
static CONSTEXPR const th_vsxsegw<UNSPEC_TH_VLXSEGW> th_vsxsegw_obj;

static CONSTEXPR const xt_vcpx0<NO_FRM> xt_vcpx0_obj;
static CONSTEXPR const xt_vcpx0<HAS_FRM> xt_vcpx0_frm_obj;
static CONSTEXPR const xt_vcpx1<NO_FRM> xt_vcpx1_obj;
static CONSTEXPR const xt_vcpx1<HAS_FRM> xt_vcpx1_frm_obj;
static CONSTEXPR const xt_vcpx2<NO_FRM> xt_vcpx2_obj;
static CONSTEXPR const xt_vcpx2<HAS_FRM> xt_vcpx2_frm_obj;
static CONSTEXPR const xt_vcpx3<NO_FRM> xt_vcpx3_obj;
static CONSTEXPR const xt_vcpx3<HAS_FRM> xt_vcpx3_frm_obj;
static CONSTEXPR const xt_vcpx4<NO_FRM> xt_vcpx4_obj;
static CONSTEXPR const xt_vcpx4<HAS_FRM> xt_vcpx4_frm_obj;
static CONSTEXPR const xt_vcpx5<NO_FRM> xt_vcpx5_obj;
static CONSTEXPR const xt_vcpx5<HAS_FRM> xt_vcpx5_frm_obj;
static CONSTEXPR const xt_vcpx6 xt_vcpx6_obj;
static CONSTEXPR const xt_vcpx7 xt_vcpx7_obj;
static CONSTEXPR const xt_vcpx8<NO_FRM> xt_vcpx8_obj;
static CONSTEXPR const xt_vcpx8<HAS_FRM> xt_vcpx8_frm_obj;
static CONSTEXPR const xt_vcpx9<NO_FRM> xt_vcpx9_obj;
static CONSTEXPR const xt_vcpx9<HAS_FRM> xt_vcpx9_frm_obj;
static CONSTEXPR const xt_vcpx10<NO_FRM> xt_vcpx10_obj;
static CONSTEXPR const xt_vcpx10<HAS_FRM> xt_vcpx10_frm_obj;

static CONSTEXPR const th_unop<UNSPEC_TH_VFEXP2> th_vfexp2_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFEXP2, HAS_FRM> th_vfexp2_frm_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFTANH> th_vftanh_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFTANH, HAS_FRM> th_vftanh_frm_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFSIG> th_vfsig_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFSIG, HAS_FRM> th_vfsig_frm_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFREC> th_vfrec_obj;
static CONSTEXPR const th_unop<UNSPEC_TH_VFREC, HAS_FRM> th_vfrec_frm_obj;

/* theadvdot.  */
BASE (th_vpnclip)
BASE (th_vpnclipu)
BASE (th_vpwadd)
BASE (th_vpwaddu)
BASE (th_vmaqa)
BASE (th_vmaqau)
BASE (th_vmaqasu)
BASE (th_vmaqaus)
BASE (th_vpmaqa)
BASE (th_vpmaqau)
BASE (th_vpmaqasu)
BASE (th_vpmaqaus)

/* xtheadvector seg load store*/
BASE (th_vlsegb)
BASE (th_vlsegbu)
BASE (th_vlsegh)
BASE (th_vlseghu)
BASE (th_vlsegw)
BASE (th_vlsegwu)
BASE (th_vssegb)
BASE (th_vssegh)
BASE (th_vssegw)
BASE (th_vlssegb)
BASE (th_vlssegbu)
BASE (th_vlssegh)
BASE (th_vlsseghu)
BASE (th_vlssegw)
BASE (th_vlssegwu)
BASE (th_vsssegb)
BASE (th_vsssegh)
BASE (th_vsssegw)
BASE (th_vlxsegb)
BASE (th_vlxsegbu)
BASE (th_vlxsegh)
BASE (th_vlxseghu)
BASE (th_vlxsegw)
BASE (th_vlxsegwu)
BASE (th_vsxsegb)
BASE (th_vsxsegh)
BASE (th_vsxsegw)

/* Xxtccev */
BASE (xt_vcpx0)
BASE (xt_vcpx0_frm)
BASE (xt_vcpx1)
BASE (xt_vcpx1_frm)
BASE (xt_vcpx2)
BASE (xt_vcpx2_frm)
BASE (xt_vcpx3)
BASE (xt_vcpx3_frm)
BASE (xt_vcpx4)
BASE (xt_vcpx4_frm)
BASE (xt_vcpx5)
BASE (xt_vcpx5_frm)
BASE (xt_vcpx6)
BASE (xt_vcpx7)
BASE (xt_vcpx8)
BASE (xt_vcpx8_frm)
BASE (xt_vcpx9)
BASE (xt_vcpx9_frm)
BASE (xt_vcpx10)
BASE (xt_vcpx10_frm)

/* xtheadvsfa  */
BASE (th_vfexp2)
BASE (th_vfexp2_frm)
BASE (th_vftanh)
BASE (th_vftanh_frm)
BASE (th_vfsig)
BASE (th_vfsig_frm)
BASE (th_vfrec)
BASE (th_vfrec_frm)
#endif

#ifdef XUANTIE_VECTOR_BUILTINS_DEF
#endif

#ifdef XUANTIE_VECTOR_BUILTINS_H
#endif

#ifdef XUANTIE_VECTOR_BUILTINS_CC

/* A list of SI integer will be registered for intrinsic functions.  */
static const rvv_type_info si_ops[] = {
#define DEF_RVV_SI_OPS(TYPE, REQUIRE) {VECTOR_TYPE_##TYPE, REQUIRE},
#include "xuantie-vector-builtins.def"
  {NUM_VECTOR_TYPES, 0}};

/* A list of USI integer will be registered for intrinsic functions.  */
static const rvv_type_info usi_ops[] = {
#define DEF_RVV_USI_OPS(TYPE, REQUIRE) {VECTOR_TYPE_##TYPE, REQUIRE},
#include "xuantie-vector-builtins.def"
  {NUM_VECTOR_TYPES, 0}};

/* A list of Int Tuple types will be registered for intrinsic functions.  */
static const rvv_type_info tuple_int_ops[] = {
#define DEF_RVV_INT_TUPLE_OPS(TYPE, REQUIRE) {VECTOR_TYPE_##TYPE, REQUIRE},
#include "xuantie-vector-builtins.def"
  {NUM_VECTOR_TYPES, 0}};

/* A list of integer LMUL1 will be registered for intrinsic functions.  */
static const rvv_type_info lmul1_i_s_ops[] = {
#define DEF_RVV_LMUL1_I_S_OPS(TYPE, REQUIRE) {VECTOR_TYPE_##TYPE, REQUIRE},
#include "xuantie-vector-builtins.def"
  {NUM_VECTOR_TYPES, 0}};

/* A list of floating-point LMUL1 will be registered for intrinsic functions. */
static const rvv_type_info lmul1_f_ops[] = {
#define DEF_RVV_LMUL1_F_OPS(TYPE, REQUIRE) {VECTOR_TYPE_##TYPE, REQUIRE},
#include "xuantie-vector-builtins.def"
  {NUM_VECTOR_TYPES, 0}};

/* A list of SF will be registered for intrinsic functions.  */
static const rvv_type_info sf_ops[] = {
#define DEF_RVV_SF_OPS(TYPE, REQUIRE) {VECTOR_TYPE_##TYPE, REQUIRE},
#include "xuantie-vector-builtins.def"
  {NUM_VECTOR_TYPES, 0}};

/* A list of args for vector_type func (vector_type, vector_type, vector_type)
 * function.  */
static CONSTEXPR const rvv_arg_type_info th_i_maqa_vvv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_signed_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_signed_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_i_maqa_vxv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_signed_char),
     rvv_arg_type_info (RVV_BASE_maqa_quad_signed_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_iu_maqa_vvv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_signed_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_unsigned_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_iu_maqa_vxv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_signed_char),
     rvv_arg_type_info (RVV_BASE_maqa_quad_unsigned_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_ui_maqa_vvv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_unsigned_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_signed_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_ui_maqa_vxv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_unsigned_char),
     rvv_arg_type_info (RVV_BASE_maqa_quad_signed_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_u_maqa_vvv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_unsigned_vector),
     rvv_arg_type_info (RVV_BASE_maqa_quad_unsigned_vector), rvv_arg_type_info_end};

static CONSTEXPR const rvv_arg_type_info th_u_maqa_vxv_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_unsigned_char),
     rvv_arg_type_info (RVV_BASE_maqa_quad_unsigned_vector), rvv_arg_type_info_end};

/* A static operand information for vector_type func (vector_type, vector_type,
 * vector_type) function registration. */
static CONSTEXPR const rvv_op_info th_i_maqa_vvvv_ops
  = {si_ops,				  /* Types */
     OP_TYPE_vv,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_i_maqa_vvv_args /* Args */};

static CONSTEXPR const rvv_op_info th_i_maqa_vvxv_ops
  = {si_ops,				  /* Types */
     OP_TYPE_vx,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_i_maqa_vxv_args /* Args */};

static CONSTEXPR const rvv_op_info th_iu_maqa_vvvv_ops
  = {si_ops,				  /* Types */
     OP_TYPE_vv,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_iu_maqa_vvv_args /* Args */};

static CONSTEXPR const rvv_op_info th_iu_maqa_vvxv_ops
  = {si_ops,				  /* Types */
     OP_TYPE_vx,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_iu_maqa_vxv_args /* Args */};

static CONSTEXPR const rvv_op_info th_ui_maqa_vvvv_ops
  = {si_ops,				  /* Types */
     OP_TYPE_vv,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_ui_maqa_vvv_args /* Args */};

static CONSTEXPR const rvv_op_info th_ui_maqa_vvxv_ops
  = {si_ops,				  /* Types */
     OP_TYPE_vx,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_ui_maqa_vxv_args /* Args */};

static CONSTEXPR const rvv_op_info th_u_maqa_vvvv_ops
  = {usi_ops,				  /* Types */
     OP_TYPE_vv,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_u_maqa_vvv_args /* Args */};

static CONSTEXPR const rvv_op_info th_u_maqa_vvxv_ops
  = {usi_ops,				  /* Types */
     OP_TYPE_vx,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     th_u_maqa_vxv_args /* Args */};

static CONSTEXPR const rvv_op_info th_i_pwadd_vvxv_ops
  = {wexti_ops,				  /* Types */
     OP_TYPE_vx,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     wvx_args /* Args */};

static CONSTEXPR const rvv_op_info th_i_pwadd_vvvv_ops
  = {wexti_ops,				  /* Types */
     OP_TYPE_vv,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     wvv_args /* Args */};

static CONSTEXPR const rvv_op_info th_u_pwadd_vvxv_ops
  = {wextu_ops,				  /* Types */
     OP_TYPE_vx,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     wvx_args /* Args */};

static CONSTEXPR const rvv_op_info th_u_pwadd_vvvv_ops
  = {wextu_ops,				  /* Types */
     OP_TYPE_vv,			  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     wvv_args /* Args */};

/* A static operand information for double demote type func (vector_type,
 * shift_type) function registration. */
static CONSTEXPR const rvv_op_info th_i_pnclip_vwv_ops
  = {i_ops,					       /* Types */
     OP_TYPE_wv,				       /* Suffix */
     rvv_arg_type_info (RVV_BASE_double_trunc_vector), /* Return type */
     shift_wv_args /* Args */};

/* A static operand information for double demote type func (vector_type,
 * shift_type) function registration. */
static CONSTEXPR const rvv_op_info th_u_pnclip_vwv_ops
  = {u_ops,					       /* Types */
     OP_TYPE_wv,				       /* Suffix */
     rvv_arg_type_info (RVV_BASE_double_trunc_vector), /* Return type */
     shift_wv_args /* Args */};

/* A static operand information for double demote type func (vector_type,
 * size_t) function registration. */
static CONSTEXPR const rvv_op_info th_i_pnclip_vwx_ops
  = {i_ops,					       /* Types */
     OP_TYPE_wx,				       /* Suffix */
     rvv_arg_type_info (RVV_BASE_double_trunc_vector), /* Return type */
     v_size_args /* Args */};

/* A static operand information for double demote type func (vector_type,
 * size_t) function registration. */
static CONSTEXPR const rvv_op_info th_u_pnclip_vwx_ops
  = {u_ops,					       /* Types */
     OP_TYPE_wx,				       /* Suffix */
     rvv_arg_type_info (RVV_BASE_double_trunc_vector), /* Return type */
     v_size_args /* Args */};

/* A static operand information for vector_type func (const scalar_type *)
 * function registration. */
static CONSTEXPR const rvv_op_info tuple_v_int_scalar_const_ptr_ops
  = {tuple_int_ops,				  /* Types */
     OP_TYPE_v,				  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     scalar_const_ptr_args /* Args */};

/* A static operand information for void func (scalar_type *, vector_type)
 * function registration. */
static CONSTEXPR const rvv_op_info tuple_v_int_scalar_ptr_ops
  = {tuple_int_ops,				/* Types */
     OP_TYPE_v,				/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     scalar_ptr_args /* Args */};

/* A static operand information for vector_type func (const scalar_type *,
 * ptrdiff_t) function registration. */
static CONSTEXPR const rvv_op_info tuple_v_int_scalar_const_ptr_ptrdiff_ops
  = {tuple_int_ops,				  /* Types */
     OP_TYPE_v,				  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     scalar_const_ptr_ptrdiff_args /* Args */};

/* A static operand information for void func (scalar_type *, ptrdiff_t,
 * vector_type) function registration. */
static CONSTEXPR const rvv_op_info tuple_v_int_scalar_ptr_ptrdiff_ops
  = {tuple_int_ops,				/* Types */
     OP_TYPE_v,				/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     scalar_ptr_ptrdiff_args /* Args */};

/* A static operand information for vector_type func (const scalar_type *,
 * index_type) function registration. */
static CONSTEXPR const rvv_op_info tuple_v_int_scalar_const_ptr_index_ops
  = {tuple_int_ops,				  /* Types */
     OP_TYPE_v,				  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     scalar_const_ptr_index_args /* Args */};

/* A static operand information for void func (scalar_type *, index_type,
 * vector_type) function registration. */
static CONSTEXPR const rvv_op_info tuple_v_int_scalar_ptr_index_ops
  = {tuple_int_ops,				/* Types */
     OP_TYPE_v,				/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     scalar_ptr_index_args /* Args */};

static CONSTEXPR const rvv_op_info sf_v_ops
  = {sf_ops,				  /* Types */
     OP_TYPE_v,				  /* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     v_args /* Args */};

void
function_builder::append_width (int sew)
{
  switch (sew)
    {
    case 8:
      append_name ("b");
      break;
    case 16:
      append_name ("h");
      break;
    case 32:
      append_name ("w");
      break;
    case 64:
      break;
    default:
      gcc_unreachable ();
    }
}

/* A list of args for func (vector_type, unsigned) function.  */
static CONSTEXPR const rvv_arg_type_info vi_args[]
  = {rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_unsigned_int), rvv_arg_type_info_end};

/* A list of args for func (vector_type, vector_type, unsigned)
 * function.  */
static CONSTEXPR const rvv_arg_type_info vvi_args[]
  = {rvv_arg_type_info (RVV_BASE_vector), rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_unsigned_int), rvv_arg_type_info_end};

/* A list of args for func (vector_type, vector_type, scalar_type)
 * function.  */
static CONSTEXPR const rvv_arg_type_info vvx_args[]
  = {rvv_arg_type_info (RVV_BASE_vector), rvv_arg_type_info (RVV_BASE_vector),
     rvv_arg_type_info (RVV_BASE_scalar), rvv_arg_type_info_end};

static CONSTEXPR const rvv_op_info xt_all_vcpx_void_v_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     v_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_void_v_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     v_args /* Args */};

static CONSTEXPR const rvv_op_info xt_all_vcpx_vv_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     v_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_vv_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     v_args /* Args */};

static CONSTEXPR const rvv_op_info xt_all_vcpx_void_vi_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vi_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_void_vi_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vi_args /* Args */};

static CONSTEXPR const rvv_op_info xt_all_vcpx_vvi_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     vi_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_vvi_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     vi_args /* Args */};

static CONSTEXPR const rvv_op_info xt_all_vcpx_void_vvv_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vvv_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_void_vvv_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vvv_args /* Args */};

static CONSTEXPR const rvv_op_info xt_all_vcpx_vvv_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     vv_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_vvv_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     vv_args /* Args */};

static CONSTEXPR const rvv_op_info xt_iu_vcpx_void_vvx_ops
  = {lmul1_i_s_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vvx_args /* Args */};

static CONSTEXPR const rvv_op_info xt_iu_vcpx_vvx_ops
  = {lmul1_i_s_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     vx_args /* Args */};

static CONSTEXPR const rvv_op_info xt_all_vcpx_void_vvi_ops
  = {lmul1_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vvi_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_void_vvi_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vvi_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_void_vvx_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_void), /* Return type */
     vvx_args /* Args */};

static CONSTEXPR const rvv_op_info xt_f_vcpx_vvx_ops
  = {lmul1_f_ops,				/* Types */
     OP_TYPE_v,			/* Suffix */
     rvv_arg_type_info (RVV_BASE_vector), /* Return type */
     vx_args /* Args */};

/* vop/vop_mu will be registered.  */
static CONSTEXPR const predication_type_index none_mu_preds[]
  = {PRED_TYPE_none, PRED_TYPE_mu, NUM_PRED_TYPES};

#endif

#ifdef XUANTIE_VECTOR_BUILTINS_TYPES_DEF
#endif

#ifdef XUANTIE_VECTOR_BUILTINS_FUNCTIONS_DEF

#define REQUIRED_EXTENSIONS VECTOR_EXT
DEF_RVV_FUNCTION (th_vpnclipu, narrow_alu, none_m_preds, th_u_pnclip_vwv_ops)
DEF_RVV_FUNCTION (th_vpnclipu, narrow_alu, none_m_preds, th_u_pnclip_vwx_ops)
DEF_RVV_FUNCTION (th_vpnclip, narrow_alu, none_m_preds, th_i_pnclip_vwv_ops)
DEF_RVV_FUNCTION (th_vpnclip, narrow_alu, none_m_preds, th_i_pnclip_vwx_ops)

DEF_RVV_FUNCTION (th_vpwaddu, widen_alu, none_m_preds, th_u_pwadd_vvvv_ops)
DEF_RVV_FUNCTION (th_vpwaddu, widen_alu, none_m_preds, th_u_pwadd_vvxv_ops)
DEF_RVV_FUNCTION (th_vpwadd, widen_alu, none_m_preds, th_i_pwadd_vvvv_ops)
DEF_RVV_FUNCTION (th_vpwadd, widen_alu, none_m_preds, th_i_pwadd_vvxv_ops)

DEF_RVV_FUNCTION (th_vmaqau, th_maqa, none_m_preds, th_u_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vmaqau, th_maqa, none_m_preds, th_u_maqa_vvxv_ops)
DEF_RVV_FUNCTION (th_vmaqa, th_maqa, none_m_preds, th_i_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vmaqa, th_maqa, none_m_preds, th_i_maqa_vvxv_ops)
DEF_RVV_FUNCTION (th_vmaqasu, th_maqa, none_m_preds, th_iu_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vmaqasu, th_maqa, none_m_preds, th_iu_maqa_vvxv_ops)
DEF_RVV_FUNCTION (th_vmaqaus, th_maqa, none_m_preds, th_ui_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vmaqaus, th_maqa, none_m_preds, th_ui_maqa_vvxv_ops)

DEF_RVV_FUNCTION (th_vpmaqau, th_maqa, none_m_preds, th_u_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vpmaqau, th_maqa, none_m_preds, th_u_maqa_vvxv_ops)
DEF_RVV_FUNCTION (th_vpmaqa, th_maqa, none_m_preds, th_i_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vpmaqa, th_maqa, none_m_preds, th_i_maqa_vvxv_ops)
DEF_RVV_FUNCTION (th_vpmaqasu, th_maqa, none_m_preds, th_iu_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vpmaqasu, th_maqa, none_m_preds, th_iu_maqa_vvxv_ops)
DEF_RVV_FUNCTION (th_vpmaqaus, th_maqa, none_m_preds, th_ui_maqa_vvvv_ops)
DEF_RVV_FUNCTION (th_vpmaqaus, th_maqa, none_m_preds, th_ui_maqa_vvxv_ops)
#undef REQUIRED_EXTENSIONS

#define REQUIRED_EXTENSIONS XTHEADVECTOR_EXT
DEF_RVV_FUNCTION (th_vlsegb, th_seg_loadb, full_preds, tuple_v_int_scalar_const_ptr_ops)
DEF_RVV_FUNCTION (th_vlsegbu, th_seg_loadbu, full_preds, tuple_v_int_scalar_const_ptr_ops)
DEF_RVV_FUNCTION (th_vlsegh, th_seg_loadh, full_preds, tuple_v_int_scalar_const_ptr_ops)
DEF_RVV_FUNCTION (th_vlseghu, th_seg_loadhu, full_preds, tuple_v_int_scalar_const_ptr_ops)
DEF_RVV_FUNCTION (th_vlsegw, th_seg_loadw, full_preds, tuple_v_int_scalar_const_ptr_ops)
DEF_RVV_FUNCTION (th_vlsegwu, th_seg_loadwu, full_preds, tuple_v_int_scalar_const_ptr_ops)
DEF_RVV_FUNCTION (th_vssegb, th_seg_storeb, none_m_preds, tuple_v_int_scalar_ptr_ops)
DEF_RVV_FUNCTION (th_vssegh, th_seg_storeh, none_m_preds, tuple_v_int_scalar_ptr_ops)
DEF_RVV_FUNCTION (th_vssegw, th_seg_storew, none_m_preds, tuple_v_int_scalar_ptr_ops)
DEF_RVV_FUNCTION (th_vlssegb, th_seg_strideloadb, full_preds, tuple_v_int_scalar_const_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vlssegbu, th_seg_strideloadbu, full_preds, tuple_v_int_scalar_const_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vlssegh, th_seg_strideloadh, full_preds, tuple_v_int_scalar_const_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vlsseghu, th_seg_strideloadhu, full_preds, tuple_v_int_scalar_const_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vlssegw, th_seg_strideloadw, full_preds, tuple_v_int_scalar_const_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vlssegwu, th_seg_strideloadwu, full_preds, tuple_v_int_scalar_const_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vsssegb, th_seg_stridestoreb, none_m_preds, tuple_v_int_scalar_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vsssegh, th_seg_stridestoreh, none_m_preds, tuple_v_int_scalar_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vsssegw, th_seg_stridestorew, none_m_preds, tuple_v_int_scalar_ptr_ptrdiff_ops)
DEF_RVV_FUNCTION (th_vlxsegb, th_seg_indexed_loadb, full_preds, tuple_v_int_scalar_const_ptr_index_ops)
DEF_RVV_FUNCTION (th_vlxsegbu, th_seg_indexed_loadbu, full_preds, tuple_v_int_scalar_const_ptr_index_ops)
DEF_RVV_FUNCTION (th_vlxsegh, th_seg_indexed_loadh, full_preds, tuple_v_int_scalar_const_ptr_index_ops)
DEF_RVV_FUNCTION (th_vlxseghu, th_seg_indexed_loadhu, full_preds, tuple_v_int_scalar_const_ptr_index_ops)
DEF_RVV_FUNCTION (th_vlxsegw, th_seg_indexed_loadw, full_preds, tuple_v_int_scalar_const_ptr_index_ops)
DEF_RVV_FUNCTION (th_vlxsegwu, th_seg_indexed_loadwu, full_preds, tuple_v_int_scalar_const_ptr_index_ops)
DEF_RVV_FUNCTION (th_vsxsegb, th_seg_indexed_storeb, none_m_preds, tuple_v_int_scalar_ptr_index_ops)
DEF_RVV_FUNCTION (th_vsxsegh, th_seg_indexed_storeh, none_m_preds, tuple_v_int_scalar_ptr_index_ops)
DEF_RVV_FUNCTION (th_vsxsegw, th_seg_indexed_storew, none_m_preds, tuple_v_int_scalar_ptr_index_ops)
#undef REQUIRED_EXTENSIONS

#define REQUIRED_EXTENSIONS XXTCCEV_EXT
DEF_RVV_FUNCTION (xt_vcpx0, xxtccev, none_mu_preds, xt_all_vcpx_void_v_ops)
DEF_RVV_FUNCTION (xt_vcpx0_frm, xxtccev, none_mu_preds, xt_f_vcpx_void_v_ops)
DEF_RVV_FUNCTION (xt_vcpx1, xxtccev, none_mu_preds, xt_all_vcpx_vv_ops)
DEF_RVV_FUNCTION (xt_vcpx1_frm, xxtccev, none_mu_preds, xt_f_vcpx_vv_ops)
DEF_RVV_FUNCTION (xt_vcpx2, xxtccev, none_mu_preds, xt_all_vcpx_void_vi_ops)
DEF_RVV_FUNCTION (xt_vcpx2_frm, xxtccev, none_mu_preds, xt_f_vcpx_void_vi_ops)
DEF_RVV_FUNCTION (xt_vcpx3, xxtccev, none_mu_preds, xt_all_vcpx_vvi_ops)
DEF_RVV_FUNCTION (xt_vcpx3_frm, xxtccev, none_mu_preds, xt_f_vcpx_vvi_ops)
DEF_RVV_FUNCTION (xt_vcpx4, xxtccev, none_mu_preds, xt_all_vcpx_void_vvv_ops)
DEF_RVV_FUNCTION (xt_vcpx4_frm, xxtccev, none_mu_preds, xt_f_vcpx_void_vvv_ops)
DEF_RVV_FUNCTION (xt_vcpx5, xxtccev, none_mu_preds, xt_all_vcpx_vvv_ops)
DEF_RVV_FUNCTION (xt_vcpx5_frm, xxtccev, none_mu_preds, xt_f_vcpx_vvv_ops)
DEF_RVV_FUNCTION (xt_vcpx6, xxtccev, none_mu_preds, xt_iu_vcpx_void_vvx_ops)
DEF_RVV_FUNCTION (xt_vcpx7, xxtccev, none_mu_preds, xt_iu_vcpx_vvx_ops)
DEF_RVV_FUNCTION (xt_vcpx8, xxtccev, none_mu_preds, xt_all_vcpx_void_vvi_ops)
DEF_RVV_FUNCTION (xt_vcpx8_frm, xxtccev, none_mu_preds, xt_f_vcpx_void_vvi_ops)
DEF_RVV_FUNCTION (xt_vcpx9, xxtccev, none_mu_preds, xt_f_vcpx_void_vvx_ops)
DEF_RVV_FUNCTION (xt_vcpx9_frm, xxtccev, none_mu_preds, xt_f_vcpx_void_vvx_ops)
DEF_RVV_FUNCTION (xt_vcpx10, xxtccev, none_mu_preds, xt_f_vcpx_vvx_ops)
DEF_RVV_FUNCTION (xt_vcpx10_frm, xxtccev, none_mu_preds, xt_f_vcpx_vvx_ops)
#undef REQUIRED_EXTENSIONS

#define REQUIRED_EXTENSIONS XTHEADVSFA_EXT
DEF_RVV_FUNCTION (th_vfexp2, alu, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vfexp2_frm, alu_frm, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vftanh, alu, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vftanh_frm, alu_frm, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vfsig, alu, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vfsig_frm, alu_frm, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vfrec, alu, full_preds, sf_v_ops)
DEF_RVV_FUNCTION (th_vfrec_frm, alu_frm, full_preds, sf_v_ops)
#undef REQUIRED_EXTENSIONS

#endif

