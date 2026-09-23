/* Word store then load: memw goes before the load and retires the store.
   Narrow loads hide the memory operand under sign_extend or zero_extend.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
word_store_then_load (int *p, int *q, int v)
{
  *p = v;
  return *q;
}

int
word_then_byte (int *a, unsigned char *b)
{
  *a = 1;
  return *b;
}

int
word_then_uhalf (int *a, unsigned short *b)
{
  *a = 1;
  return *b;
}

int
word_then_half (int *a, short *b)
{
  *a = 1;
  return *b;
}

/* memw sits between the store and the load.  */
/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l8ui" } } */
/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l16ui" } } */
/* { dg-final { scan-assembler "s32i\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l16si" } } */

/* The load must not still sit directly against the store.  */
/* { dg-final { scan-assembler-not "s32i\[^\\n\]*\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler-not "s32i\[^\\n\]*\\n\[\[:space:\]\]*l8ui" } } */
/* { dg-final { scan-assembler-not "s32i\[^\\n\]*\\n\[\[:space:\]\]*l16ui" } } */
/* { dg-final { scan-assembler-not "s32i\[^\\n\]*\\n\[\[:space:\]\]*l16si" } } */

/* { dg-final { scan-assembler-not "l32i\[^\\n\]*\\n\[\[:space:\]\]*memw" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 4 } } */
