/* A volatile word store is not duplicated.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=dupldst" } */

void
dup_vol (volatile int *p, int v)
{
  *p = v;
}

/* { dg-final { scan-assembler "memw\\n\[\[:space:\]\]*s32i" } } */
/* { dg-final { scan-assembler-times "s32i" 1 } } */
/* { dg-final { scan-assembler-not "l32i" } } */
