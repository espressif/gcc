/* Store with no following load: memw stays in front of the return.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
word_store_then_return (int *p, int v)
{
  *p = v;
}

/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
