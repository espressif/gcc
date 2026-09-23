/* Backedge is a conditional branch.  It fences the s8i and retires it.  */
/* { dg-do compile } */
/* { dg-options "-O2 -fno-tree-loop-distribute-patterns -fno-builtin -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
fill_bytes (char *p, int n)
{
  int i;

  for (i = 0; i < n; i++)
    p[i] = (char) i;
}

/* { dg-final { scan-assembler "s8i\[^\\n\]*\\n\[\[:space:\]\]*memw" } } */
/* { dg-final { scan-assembler "bnez" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
