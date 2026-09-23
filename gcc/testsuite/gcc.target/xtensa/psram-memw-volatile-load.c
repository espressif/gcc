/* A volatile load retires the pending word store.  One memw, before it.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
vol_load (int *p, volatile int *q, int v)
{
  *p = v;
  return *q;
}

/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
