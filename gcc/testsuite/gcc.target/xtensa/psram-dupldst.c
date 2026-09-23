/* dupldst inserts a reload between the original store and the copy.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=dupldst" } */

void
dup_word (int *p, int v)
{
  *p = v;
}

/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*l32i\[^\\n\]*\\n\[\[:space:\]\]*s32i" } } */
/* { dg-final { scan-assembler-times "s32i" 2 } } */
/* { dg-final { scan-assembler-times "l32i" 1 } } */
/* { dg-final { scan-assembler-not "memw" } } */
