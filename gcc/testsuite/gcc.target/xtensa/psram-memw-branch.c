/* A conditional branch does not retire a word store.  The load does.  */
/* { dg-do compile } */
/* { dg-options "-O2 -fno-if-conversion -fno-if-conversion2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
branch_then_load (int *p, int *q, int v, int c)
{
  *p = v;
  if (c)
    return 1;
  return *q;
}

/* { dg-final { scan-assembler "bnez" } } */
/* { dg-final { scan-assembler "memw\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler-not "l32i\[^\\n\]*\\n\[\[:space:\]\]*memw" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
