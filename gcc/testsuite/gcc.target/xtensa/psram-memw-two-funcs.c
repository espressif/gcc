/* A retired store must not leak a memw into the next function.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

void
psram_store (int *p, int v)
{
  *p = v;
}

int
psram_load (int *q)
{
  return *q;
}

/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
/* { dg-final { scan-assembler-not {psram_load:(?:(?!\.size).)*\tmemw} } } */
