/* Volatile s8i keeps the compiler memw in front and is not fenced twice.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
vol_byte (volatile char *p, int c)
{
  *p = c;
}

/* { dg-final { scan-assembler "memw\\n\[\[:space:\]\]*s8i" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*memw" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 2 } } */
