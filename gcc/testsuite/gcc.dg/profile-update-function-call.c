/* { dg-require-effective-target global_constructor } */
/* { dg-options "-O2 -fprofile-arcs -ftest-coverage -fprofile-update=function-call -fdump-tree-optimized" } */

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

/* { dg-final { scan-tree-dump "__gcov_increment_fn" "optimized" } } */
/* { dg-final { scan-tree-dump-not "PROF_edge_counter" "optimized" } } */
