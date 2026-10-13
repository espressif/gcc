/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */

/* Local register variables in XESPV q registers.  */

typedef unsigned int u32x4 __attribute__ ((vector_size (16)));

void
load (u32x4 *p)
{
  register u32x4 v __asm__ ("q0") = *p;
  __asm__ volatile ("# use %0" :: "eq" (v));
}

void
store (u32x4 *p)
{
  register u32x4 v __asm__ ("q2");
  __asm__ volatile ("# def %0" : "=eq" (v));
  *p = v;
}

void
copy (void)
{
  register u32x4 a __asm__ ("q3");
  register u32x4 b __asm__ ("q4");
  __asm__ volatile ("# def %0" : "=eq" (a));
  b = a;
  __asm__ volatile ("# use %0" :: "eq" (b));
}

void
zero (void)
{
  register u32x4 v __asm__ ("q5") = (u32x4) { 0, 0, 0, 0 };
  __asm__ volatile ("# use %0" :: "eq" (v));
}

void
constant (void)
{
  register u32x4 v __asm__ ("q6") = (u32x4) { 1, 2, 3, 4 };
  __asm__ volatile ("# use %0" :: "eq" (v));
}

void
offset (u32x4 *p)
{
  register u32x4 v __asm__ ("q7") = p[3];
  __asm__ volatile ("" : "+eq" (v));
  p[5] = v;
}

/* { dg-final { scan-assembler "esp\\.vld\\.128\\.ip\tq0,a0,0" } } */
/* { dg-final { scan-assembler "# use q0" } } */
/* { dg-final { scan-assembler "esp\\.vst\\.128\\.ip\tq2,a0,0" } } */
/* { dg-final { scan-assembler "esp\\.orq\tq4,q3,q3" } } */
/* { dg-final { scan-assembler "esp\\.zero\\.q\tq5" } } */
/* { dg-final { scan-assembler "esp\\.vld\\.128\\.ip\tq6,\[a-z0-9\]+,0" } } */
/* { dg-final { scan-assembler "esp\\.vld\\.128\\.ip\tq7,\[a-z0-9\]+,0" } } */
/* { dg-final { scan-assembler "esp\\.vst\\.128\\.ip\tq7,\[a-z0-9\]+,0" } } */
/* { dg-final { scan-assembler-times "esp\\.v(ld|st)\\.128\\.ip\tq\[0-7\],(s0|s1|a\[0-5\]|s8|s9|s10|s11|t\[3-6\]),0" 5 } } */
