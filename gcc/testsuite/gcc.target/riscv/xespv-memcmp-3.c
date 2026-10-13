/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=esp32p4 -mxespv-memcmp" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-Og" "-Oz" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

/* One pointer is 16-byte aligned and loaded directly.  The other is
   shifted, and its last block is the one that holds the last byte.  */
/*
** mix16:
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],0
**	esp.src.q	q[0-7],q[0-7],q[0-7]
**	...
*/
int
mix16 (const void *a, const void *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 16), b, 64);
}

/* One pointer is 8-byte aligned, so it uses the 64-bit pair.  */
/*
** mix8:
**	...
**	esp.vld.l.64.ip	q[0-7],a[0-7],8
**	...
**	esp.vld.h.64.ip	q[0-7],a[0-7],8
**	...
**	esp.src.q	q[0-7],q[0-7],q[0-7]
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],0
**	...
*/
int
mix8 (const void *a, const void *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 8), b, 64);
}

/*
** tail8:
**	...
**	esp.vld.l.64.ip	q[0-7],a[0-7],8
**	...
**	esp.vld.h.64.ip	q[0-7],a[0-7],8
**	...
**	lbu	a[0-7],0\(a[0-7]\)
**	...
*/
int
tail8 (const void *a, const void *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 8),
			   __builtin_assume_aligned (b, 8), 17);
}

/* Five chunks are one more than the unrolled limit, so this is a loop.  */
/*
** c80:
**	...
**	beq	a[0-7],zero,\.L[0-9]+
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	...
**	j	\.L[0-9]+
**	...
*/
int
c80 (const void *a, const void *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 16),
			   __builtin_assume_aligned (b, 16), 80);
}

/* The same length, misaligned: the loop copies each block back, and the
   last block is loaded with a zero increment.  */
/*
** cmis80:
**	...
**	esp.vld.128.ip	(q[0-7]),[a-z][a-z0-9]*,16
**	esp.src.q	q[0-7],q[0-7],\1
**	esp.orq	q[0-7],\1,\1
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],0
**	esp.src.q	q[0-7],q[0-7],q[0-7]
**	...
*/
int
cmis80 (const void *a, const void *b)
{
  return __builtin_memcmp (a, b, 80);
}

/* Not a multiple of 16: a scalar byte follows the shifted chunks.  */
/*
** cmis65:
**	...
**	esp.src.q	q[0-7],q[0-7],q[0-7]
**	...
**	lbu	a[0-7],0\(a[0-7]\)
**	...
*/
int
cmis65 (const void *a, const void *b)
{
  return __builtin_memcmp (a, b, 65);
}
