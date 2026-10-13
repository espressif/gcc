/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } } */

/* Register variables keep their register, so one an Espressif instruction
   cannot encode is an error.  */

void
f (int *p, float y)
{
  register int *r __asm__ ("a7") = p;
  __asm__ volatile ("esp.vld.128.ip q0, %0, 16" : "+r" (r)); /* { dg-error "operand 0 of 'esp.vld.128.ip' must be one of x8-x15 or x24-x31, not 'a7'" } */

  register float x __asm__ ("fa6") = y;
  __asm__ volatile ("esp.vfadd.f32.vf q0, q1, %0" : : "f" (x)); /* { dg-error "operand 0 of 'esp.vfadd.f32.vf' must be one of f8-f15 or f24-f31, not 'fa6'" } */

  register int *ok __asm__ ("s1") = p;
  __asm__ volatile ("esp.vld.128.ip q0, %0, 16" : "+r" (ok));

  register int any __asm__ ("a6") = 1;
  __asm__ volatile ("esp.addx2 %0, %0, %0" : "+r" (any));
}
