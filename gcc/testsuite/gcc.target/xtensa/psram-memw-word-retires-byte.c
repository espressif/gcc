/* s32i finishes a pending s8i, so the only memw is before the return.  */
/* { dg-do compile } */
/* { dg-options "-O2 -fno-schedule-insns -fno-schedule-insns2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
byte_then_word (char *p, int v)
{
  *p = 1;
  *(int *) (p + 1) = v;
}

/* { dg-final { scan-assembler "s8i\[^\\n\]*\\n\[\[:space:\]\]*add" } } */
/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-not "(?s)s8i.*?memw.*?s32i" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
