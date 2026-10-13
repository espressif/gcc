/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */

/* Most Espressif RV-PIE instructions only encode x8-x15/x24-x31 and
   f8-f15/f24-f31, so "r", "g" and "f" operands they use are restricted to
   those registers.  The last arguments arrive in a6/a7 (fa6/fa7) and must
   be moved.  */

void sink (int, int, int, int, int, int, int, int);

void
load (int a, int b, int c, int d, int e, int f, int g, int *h)
{
  __asm__ volatile ("esp.vld.128.ip q0, %0, 16" : "+r" (h));
}

void
movi (int a, int b, int c, int d, int e, int f, int g)
{
  int r;
  __asm__ volatile ("esp.movi.32.a q0, %0, 1" : "=r" (r));
  sink (a, b, c, d, e, f, g, r);
}

int *
tied (int a, int b, int c, int d, int e, int f, int g, int *h)
{
  int *r;
  __asm__ volatile ("esp.vld.128.ip q0, %1, 16" : "=r" (r) : "0" (h));
  return r;
}

void
general (int a, int b, int c, int d, int e, int f, int g, int *h)
{
  __asm__ volatile ("esp.vld.128.ip q0, %0, 16" : : "g" (h));
}

void
multi (int a, int b, int c, int d, int e, int f, int *g, int *h)
{
  __asm__ volatile ("1:\tESP.VST.128.IP q0, %[p], 16 # %[q]\n\t"
		    "esp.zero.q q1; esp.vst.128.ip q1, %[q], 16"
		    : [p] "+r" (g), [q] "+r" (h));
}

/* These instructions and unknown ones accept any register.  */

int
full (int a, int b, int c, int d, int e, int f, int g, int h)
{
  int r;
  __asm__ ("esp.addx2 %0, %1, %2" : "=r" (r) : "r" (g), "r" (h));
  return r;
}

void
unknown (int a, int b, int c, int d, int e, int f, int g, int h)
{
  __asm__ volatile ("foo %0" : : "r" (h));
}

float
fpr_out (float a, float b, float c, float d, float e, float f, float g)
{
  float r;
  __asm__ volatile ("esp.movi.32.f q0, %0, 1" : "=f" (r));
  return r + a + b + c + d + e + f + g;
}

void
fpr_in (float a, float b, float c, float d, float e, float f, float g, float h)
{
  __asm__ volatile ("esp.vfadd.f32.vf q0, q1, %0" : : "f" (h));
}

/* { dg-final { scan-assembler-times {esp\.vld\.128\.ip q0, (s0|s1|a[0-5]|s[89]|s1[01]|t[3-6]), 16} 3 } } */
/* { dg-final { scan-assembler-times {esp\.movi\.32\.a q0, (s0|s1|a[0-5]|s[89]|s1[01]|t[3-6]), 1} 1 } } */
/* { dg-final { scan-assembler-times {ESP\.VST\.128\.IP q0, (s0|s1|a[0-5]|s[89]|s1[01]|t[3-6]), 16} 1 } } */
/* { dg-final { scan-assembler-times {esp\.vst\.128\.ip q1, (s0|s1|a[0-5]|s[89]|s1[01]|t[3-6]), 16} 1 } } */
/* { dg-final { scan-assembler-times {esp\.movi\.32\.f q0, (fs[01]|fa[0-5]|fs[89]|fs1[01]|ft([89]|1[01])), 1} 1 } } */
/* { dg-final { scan-assembler-times {esp\.vfadd\.f32\.vf q0, q1, (fs[01]|fa[0-5]|fs[89]|fs1[01]|ft([89]|1[01]))} 1 } } */
/* { dg-final { scan-assembler-times {esp\.addx2 [a-z0-9]+, a6, a7} 1 } } */
/* { dg-final { scan-assembler-times {foo a7} 1 } } */
