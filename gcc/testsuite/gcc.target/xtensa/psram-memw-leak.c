/* A store pending at the end of a function without a return must not
   leak a memw into the next function.  */
/* { dg-do compile } */
/* { dg-options "-O2 -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw" } */

extern void die (void) __attribute__ ((noreturn));

void
word_then_die (int *p, int v)
{
  *p = v;
  die ();
}

int
load_after_die (int *q)
{
  return *q;
}

/* { dg-final { scan-assembler-not {load_after_die:(?:(?!\.size).)*\tmemw} } } */
