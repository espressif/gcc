/* Store, then an unconditional j to a call.  The call keeps GCC from
   deleting that jump and duplicating the return.  memw belongs to the
   j, so the return does not get another one.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

extern int ext_sink (int);

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
  return ext_sink (2);
}

/* { dg-final { scan-assembler "memw\\n\[\[:space:\]\]*j\[^\\n\]*\\n" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*ret" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
