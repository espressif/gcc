/* s8i then l8ui: the byte fence is the only memw.  The load adds none.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
byte_then_load (char *p, char *q, int c)
{
  *p = c;
  return *q;
}

/* { dg-final { scan-assembler "s8i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l8ui" } } */
/* { dg-final { scan-assembler-not "l8ui\[^\\n\]*\\n\[\[:space:\]\]*memw" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
