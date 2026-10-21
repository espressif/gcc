/* { dg-require-effective-target global_constructor } */
/* { dg-options "-O2 -fprofile-arcs -ftest-coverage -fprofile-counter-size=32 -fprofile-update=function-call -fdump-tree-optimized" } */

int
foo (int x)
{
  if (x)
    return 1;
  return 0;
}

int
main (void)
{
  return foo (1);
}

/* { dg-final { scan-tree-dump "__gcov_increment_fn_32" "optimized" } } */
/* Merge function is a slot in gcov_info, emitted as data, not GIMPLE.  */
/* { dg-final { scan-assembler "__gcov_merge_add_32" } } */
/* { dg-final { scan-tree-dump-not "__gcov_increment_fn\[^_\]" "optimized" } } */
