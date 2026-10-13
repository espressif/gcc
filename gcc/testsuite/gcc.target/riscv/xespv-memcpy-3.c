/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=esp32p4 -mxespv-memcpy" } */

/* Copies that -mxespv-memcpy leaves to the scalar expansion.  */

struct s64 { char b[64]; } __attribute__ ((aligned (16)));

void
copy_volatile (volatile struct s64 *d, struct s64 *s)
{
  *d = *s;
}

__attribute__ ((cold)) void
copy_cold (char *d, const char *s)
{
  __builtin_memcpy (__builtin_assume_aligned (d, 16),
		    __builtin_assume_aligned (s, 16), 64);
}

void
copy_15 (char *d, const char *s)
{
  __builtin_memcpy (__builtin_assume_aligned (d, 16),
		    __builtin_assume_aligned (s, 16), 15);
}

void
copy_unaligned_dst_31 (char *d, const char *s)
{
  __builtin_memcpy (d, __builtin_assume_aligned (s, 16), 31);
}

void
copy_unaligned_src_31 (char *d, const char *s)
{
  __builtin_memcpy (__builtin_assume_aligned (d, 16), s, 31);
}

/* { dg-final { scan-assembler-not {\mesp\.} } } */
