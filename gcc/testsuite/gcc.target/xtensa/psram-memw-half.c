/* s16i then a sign-extending load.  One memw, so the extend wrapper is seen.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
half_then_load (short *p, short *q, int c)
{
  *p = c;
  return *q;
}

/* { dg-final { scan-assembler "s16i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l16" } } */
/* { dg-final { scan-assembler-not "l16\[^\\n\]*\\n\[\[:space:\]\]*memw" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
