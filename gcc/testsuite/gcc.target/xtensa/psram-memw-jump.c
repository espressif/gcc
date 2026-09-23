/* Store with no load before an unconditional j: memw belongs to that j.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

int
j_after_store (int *p, int v, int a, int b)
{
  if (a)
    {
      *p = v;
      goto done;
    }
  if (b)
    return 1;
done:
  return 2;
}

/* { dg-final { scan-assembler "memw\\n\[\[:space:\]\]*j\[^\\n\]*\\n" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
