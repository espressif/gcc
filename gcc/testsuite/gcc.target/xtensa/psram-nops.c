/* Nop strategy separates a word store from a word load by four nops.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=nops" } */

int
word_gap (int *p, int *q, int v)
{
  *p = v;
  return *q;
}

/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*nop\[^\\n\]*\\n\[\[:space:\]\]*nop\[^\\n\]*\\n\[\[:space:\]\]*nop\[^\\n\]*\\n\[\[:space:\]\]*nop\[^\\n\]*\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler-times {\tnop(\.n)?\n} 4 } } */
/* { dg-final { scan-assembler-not "memw" } } */
