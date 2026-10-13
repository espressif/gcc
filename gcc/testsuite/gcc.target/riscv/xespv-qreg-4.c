/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */

/* q registers are only used when requested explicitly, never for
   ordinary 128-bit copies.  Misaligned memory is never accessed with
   q-register loads and stores, which require 16-byte alignment.  */

typedef unsigned int u32x4 __attribute__ ((vector_size (16)));
typedef unsigned int u32x4u __attribute__ ((vector_size (16), aligned (4)));

void
vcopy (u32x4 *p, u32x4 *q)
{
  *q = *p;
}

void
ldcopy (long double *p, long double *q)
{
  *q = *p;
}

long double
ldadd (long double a, long double b)
{
  return a + b;
}

void
vconst (u32x4 *p)
{
  *p = (u32x4) { 1, 2, 3, 4 };
}

void
misaligned (u32x4u *p, u32x4u *q)
{
  register u32x4 v __asm__ ("q1") = *p;
  __asm__ volatile ("" : "+eq" (v));
  *q = v;
}

/* { dg-final { scan-assembler-times "esp\\." 2 } } */
/* { dg-final { scan-assembler-times "esp\\.vld\\.128\\.ip\tq1,\[a-z0-9\]+,0" 1 } } */
/* { dg-final { scan-assembler-times "esp\\.vst\\.128\\.ip\tq1,\[a-z0-9\]+,0" 1 } } */
/* { dg-final { scan-assembler-not "esp\\.v(ld|st)\\.128\\.ip\tq1,a\[01\],0" } } */
