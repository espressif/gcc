/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=esp32p4" } */

/* -mxespv-memcmp is off by default.  */

int
c16 (const char *a, const char *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 16),
			   __builtin_assume_aligned (b, 16), 16);
}

/* { dg-final { scan-assembler-not {\mesp\.} } } */
