#define XUANTIE_IMPLIED_TABLE	\
  {"xtheadc", "xtheadcmo"},	\
  {"xtheadc", "xtheadsync"},	\
  {"xtheadc", "xtheadba"},	\
  {"xtheadc", "xtheadbb"},	\
  {"xtheadc", "xtheadbs"},	\
  {"xtheadc", "xtheadcondmov"},	\
  {"xtheadc", "xtheadmemidx"},	\
  {"xtheadc", "xtheadmempair"},	\
  {"xtheadc", "xtheadfmemidx"},	\
  {"xtheadc", "xtheadfmv"},	\
  {"xtheadc", "xtheadmac"},	\
				\
  {"xtheade", "xtheadcmo"},	\
  {"xtheade", "xtheadsync"},	\
  {"xtheade", "xtheadba"},	\
  {"xtheade", "xtheadbb"},	\
  {"xtheade", "xtheadbs"},	\
  {"xtheade", "xtheadcondmov"},	\
  {"xtheade", "xtheadmemidx"},	\
  {"xtheade", "xtheadfmv"},	\
  {"xtheade", "xtheadmac"},	\
  {"xtheade", "xtheadint"},     \
				\
  {"xtheadse", "xtheadcmo"},	\
				\
  {"xtheadvdot", "v"},		\
  {"xtheadvsfa", "v"},		\
				\
  {"xxtccef", "d"},		\
  {"xxtccev", "v"},		\
  {"xtheadvector", "xtheadzvamo"},		\
  {"p", "zpn"},						\
  {"p", "zpsfoperand",					\
   [] (const riscv_subset_list *subset_list) -> bool	\
   { return subset_list->xlen () == 64;}},		\
  {"p", "zprvsfextra",					\
   [] (const riscv_subset_list *subset_list) -> bool	\
   { return subset_list->xlen () == 64;}},		\
  {"zpsfoperand", "zpn",				\
   [] (const riscv_subset_list *subset_list) -> bool	\
   { return subset_list->xlen () == 32;}},		\


#define XUANTIE_EXT_VERSION_TABLE			\
  {"p",		  ISA_SPEC_CLASS_NONE,	0, 9},		\
  {"zpn",	  ISA_SPEC_CLASS_NONE,	0, 9},		\
  {"zprvsfextra", ISA_SPEC_CLASS_NONE,	0, 9},		\
  {"zpsfoperand", ISA_SPEC_CLASS_NONE,	0, 9},		\
  {"xtheadc", ISA_SPEC_CLASS_NONE, 1, 0},		\
  {"xtheade", ISA_SPEC_CLASS_NONE, 1, 0},		\
  {"xtheadfpp", ISA_SPEC_CLASS_NONE, 0, 1},		\
  {"xtheadmatrix", ISA_SPEC_CLASS_NONE, 1, 0},		\
  {"xtheadse", ISA_SPEC_CLASS_NONE, 1, 0},		\
  {"xtheadvdot", ISA_SPEC_CLASS_NONE, 1, 0},		\
  {"xtheadvsfa", ISA_SPEC_CLASS_NONE, 0, 2},		\
  {"xxtccei", ISA_SPEC_CLASS_NONE, 0, 2},		\
  {"xxtccef", ISA_SPEC_CLASS_NONE, 0, 2},		\
  {"xxtccev", ISA_SPEC_CLASS_NONE, 0, 2},		\
  {"zvamo", ISA_SPEC_CLASS_NONE, 0, 7},   \
  {"zvlsseg", ISA_SPEC_CLASS_NONE, 0, 7},   \
  {"v",       ISA_SPEC_CLASS_20190608, 0, 7},   \
  {"xtheadzvamo", ISA_SPEC_CLASS_NONE, 1, 0},   \

#define XUANTIE_EXT_FLAG_TABLE							\
  {"p",		    &gcc_options::x_riscv_xthead_subext, MASK_DSP},		\
  {"zpn",	    &gcc_options::x_riscv_xthead_subext, MASK_ZPN},		\
  {"zpsfoperand",   &gcc_options::x_riscv_xthead_subext, MASK_ZPSFOPERAND},	\
  {"zprvsfextra",   &gcc_options::x_riscv_xthead_subext, MASK_ZPRVSFEXTRA},	\
  {"xtheadc",       &gcc_options::x_riscv_xthead_subext, MASK_XTHEADC},		\
  {"xtheade",       &gcc_options::x_riscv_xthead_subext, MASK_XTHEADE},		\
  {"xtheadfpp",     &gcc_options::x_riscv_xthead_subext, MASK_XTHEADFPP},		\
  {"xtheadmatrix",  &gcc_options::x_riscv_xthead_subext, MASK_XTHEADMATRIX},	\
  {"xtheadse",      &gcc_options::x_riscv_xthead_subext, MASK_XTHEADSE},	\
  {"xtheadvdot",    &gcc_options::x_riscv_xthead_subext, MASK_XTHEADVDOT},	\
  {"xtheadvsfa",    &gcc_options::x_riscv_xthead_subext, MASK_XTHEADVSFA},		\
  {"xxtccei",       &gcc_options::x_riscv_xthead_subext, MASK_XXTCCEI},		\
  {"xxtccef",       &gcc_options::x_riscv_xthead_subext, MASK_XXTCCEF},		\
  {"xxtccev",       &gcc_options::x_riscv_xthead_subext, MASK_XXTCCEV},		\

#define TARGET_SUBSET_VERSION_P(SUBSET, MAJOR, MINOR) \
  (riscv_current_subset_list () -> lookup (SUBSET, MAJOR, MINOR) != NULL)

const char *xt_mcpu = "unknown";

/* Compute default -mabi option from arch string.  */

static const char *
xt_compute_default_abi(const char* arch_str)
{
  location_t loc = UNKNOWN_LOCATION;

  riscv_parse_arch_string (arch_str, NULL, loc);

  if (riscv_current_subset_list ()->xlen() == 64) {
    if (TARGET_SUBSET_VERSION_P("d", RISCV_DONT_CARE_VERSION,
				RISCV_DONT_CARE_VERSION))
      return "-mabi=lp64d";
    if (TARGET_SUBSET_VERSION_P("f", RISCV_DONT_CARE_VERSION,
				RISCV_DONT_CARE_VERSION))
      return "-mabi=lp64f";
    return "-mabi=lp64";
  } else {
    if (TARGET_SUBSET_VERSION_P("e", RISCV_DONT_CARE_VERSION,
				RISCV_DONT_CARE_VERSION))
      return "-mabi=ilp32e";
    if (TARGET_SUBSET_VERSION_P("d", RISCV_DONT_CARE_VERSION,
				RISCV_DONT_CARE_VERSION))
      return "-mabi=ilp32d";
    if (TARGET_SUBSET_VERSION_P("f", RISCV_DONT_CARE_VERSION,
				RISCV_DONT_CARE_VERSION))
      return "-mabi=ilp32f";
    return "-mabi=ilp32";
  }
}

/* Expand default -mabi option from -march option.  */

const char *
xt_expand_abi_from_arch (int argc ATTRIBUTE_UNUSED, const char **argv)
{
  return xt_compute_default_abi(argv[0]);
}

/* Expand default -mabi option from -mcpu option.  */

const char *
xt_expand_abi_from_cpu (int argc, const char **argv)
{
  gcc_assert (argc > 0 && argc <= 2);
  const char *default_abi_str = NULL;
  if (argc >= 2)
    default_abi_str = argv[1];

  const riscv_cpu_info *cpu = riscv_find_cpu (argv[0]);

  if (cpu == NULL)
    {
      if (default_abi_str == NULL)
	return "";
      else
	return xasprintf ("-mabi=%s", default_abi_str);
    }
  else
    return xt_compute_default_abi(cpu->arch);
}

/* Get multilib path for STARTFILE_PREFIX_SPEC.  */

const char *
xt_get_arch_spec_path (int argc ATTRIBUTE_UNUSED,
	      const char **argv ATTRIBUTE_UNUSED)
{
  const char *arch_spec = "";
  const char *abi_spec = NULL;
  const char *xlen_spec = NULL;

  const riscv_subset_list *subset_list = riscv_current_subset_list ();

  if (subset_list)
    {
      if (subset_list->xlen() == 64)
	xlen_spec = "lib64";
      else if (subset_list->xlen() == 32)
	xlen_spec = "lib32";
      else
	gcc_unreachable ();

      if (subset_list->lookup ("xtheadc"))
	arch_spec = "xthead";

    }

  for (int i = 0; i < argc; i++)
    {
      if (strcmp (argv[i], "-") == 0 || argv[i] == NULL)
	continue;

      if (strstr(argv[i], "mabi=") != NULL)
	abi_spec = argv[i] + strlen("mabi=");

      if (!xlen_spec)
	{
	  if (strstr(argv[i], "rv64") != NULL)
	    xlen_spec = "lib64";
	  if (strstr(argv[i], "rv32") != NULL)
	    xlen_spec = "lib32";
	}
    }

  if (!xlen_spec)
    warning_at (input_location, 0, "Missing -march=");

  if (!abi_spec)
    warning_at (input_location, 0, "Missing -mabi=");

  return concat (xlen_spec, arch_spec, "/", abi_spec, "/", NULL);
}

/* Returns the pipeline based on the value of arch.  */
const char *
xt_expand_tune_form_arch (int argc ATTRIBUTE_UNUSED,
	      const char **argv ATTRIBUTE_UNUSED)
{
  riscv_subset_list *subset_list;
  subset_list = riscv_subset_list::parse (argv[0], UNKNOWN_LOCATION);
  if (subset_list)
    {
      if (subset_list->lookup ("xtheadc"))
	return "c910";
      else if (subset_list->lookup ("xtheade"))
	return "e906";
      else if (subset_list->lookup ("xtheadse"))
	return "e902";
      else if (subset_list->lookup ("xtheadcmo"))
	return "c910";
    }

  return "rocket";;
}

/* Do nothing, just record mcpu, and then use it to do some optimization.  */

void
xt_record_mcpu (const char *mcpu)
{
  xt_mcpu = xstrdup (mcpu);
}

void
riscv_subset_list::xt_finalize ()
{
  if (m_xlen == 32 && lookup ("zprvsfextra"))
    error_at (m_loc, "%<-march=%s%>: The ISA 'zprvsfextra' need RV64.", m_arch);

  if (m_xlen == 64
      && (lookup ("zpn") || lookup ("zpsfoperand") || lookup ("zprvsfextra"))
      && !(lookup ("zpn") && lookup ("zpsfoperand") && lookup ("zprvsfextra")))
    error_at (m_loc, "%<-march=%s%>: At RV64, the ISA 'zpn' 'zpsfoperand'"
		     " 'zprvsfextra' must be present or absent at the same"
		     " time.", m_arch);
  
  if ((lookup ("v", 0, 7) != NULL) && !(lookup ("xtheadvector"))){
    add ("xtheadvector", false);
  }

  if ((lookup ("zvamo", 0, 7) != NULL) && !(lookup ("xtheadzvamo")))
    add ("xtheadzvamo", false);
}

#ifndef XT_USE_UPPER_LEVEL_DIR_FOR_MULTI_LIB_OS
#define XT_USE_UPPER_LEVEL_DIR_FOR_MULTI_LIB_OS 0
#endif

/* Implement TARGET_COMPUTE_MULTILIB_OS.  */
static const char *
xt_compute_multilib_os (
  const char *multilib_os_dir,
  const char *multilib_dir)
{
  if (!XT_USE_UPPER_LEVEL_DIR_FOR_MULTI_LIB_OS)
    return multilib_os_dir;

  /* We don't need to do anything if we don't get multilib_dir.  */
  if (multilib_dir == NULL)
    return multilib_os_dir;

  /* If multilib_dir is the default, multilib_os_dir should also be the default.  */
  if (strcmp (multilib_dir, ".") == 0 || multilib_os_dir != NULL)
    return multilib_os_dir;

  /* According to the definition in gcc/config/riscv/t-linux, we cannot simply copy
     multilib_dir to multilib_os_dir:
	MULTILIB_OSDIRNAMES := $(patsubst lib%,../lib%,$(MULTILIB_DIRNAMES)). */
  return concat ("../", multilib_dir , NULL);
}

#undef TARGET_COMPUTE_MULTILIB_OS
#define TARGET_COMPUTE_MULTILIB_OS xt_compute_multilib_os
