/* Without -mserialize-volatile the pass must still fence a volatile load.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mno-serialize-volatile -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
vol_load (int *p, volatile int *q, int v)
{
  *p = v;
  return *q;
}

/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
