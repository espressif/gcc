/* Two s8i instructions: only the last one is fenced, once.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
two_bytes (char *p, char *q, int a, int b)
{
  *p = a;
  *q = b;
}

/* { dg-final { scan-assembler "s8i\[^\\n\]*\\n\[\[:space:\]\]*s8i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
