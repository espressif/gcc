/* dupldst leaves s8i alone and fences it once on the way out.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=dupldst" } */

void
dup_byte (char *p, int c)
{
  *p = c;
}

/* { dg-final { scan-assembler "s8i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-not "l32i" } } */
/* { dg-final { scan-assembler-not "s32i" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
