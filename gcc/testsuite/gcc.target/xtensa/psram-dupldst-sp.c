/* Storing a1 cannot be reloaded.  dupldst emits memw instead.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=dupldst" } */

void
store_a1 (int *p)
{
  register void *sp __asm__ ("a1");
  *p = (int) sp;
}

/* { dg-final { scan-assembler "s32i\[^\\n\]*sp\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times "s32i" 1 } } */
/* { dg-final { scan-assembler-not "l32i" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
