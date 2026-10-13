/* { dg-do compile } */
/* { dg-options "-march=rv32ec_zicsr_xespv -mabi=ilp32e -O2" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } } */

/* q registers need 16-byte aligned stack slots, which ilp32e does not
   provide.  */

typedef unsigned int u32x4 __attribute__ ((vector_size (16)));

void
load (u32x4 *p)
{
  register u32x4 v __asm__ ("q0") = *p; /* { dg-error "the register specified for 'v' cannot be accessed by the current target" } */
  __asm__ volatile ("" :: "r" (v));
}
