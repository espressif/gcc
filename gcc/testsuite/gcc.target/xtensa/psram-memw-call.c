/* A call is not an unconditional jump.  The following load takes the memw.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

extern void ext (void);

int
call_then_load (int *p, int *q, int v)
{
  *p = v;
  ext ();
  return *q;
}

/* { dg-final { scan-assembler "call\[^\\n\]*\\n\[\[:space:\]\]*memw\\n\[\[:space:\]\]*l32i" } } */
/* { dg-final { scan-assembler-not "memw\\n\[\[:space:\]\]*call" } } */
/* { dg-final { scan-assembler-times {\tmemw\n} 1 } } */
