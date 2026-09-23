/* A literal pool load cannot alias a store.  It takes no memw.  */
/* { dg-do compile } */
/* { dg-options "-Og -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
const_after_store (int *p, int *q, int v)
{
  *p = v;
  *q = 0x12345678;
}

/* { dg-final { scan-assembler "l32r" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*l32r" } } */
/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
