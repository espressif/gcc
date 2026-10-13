/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=esp32p4 -mxespv-memcmp" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-Og" "-Oz" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

/*
** c16:
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	esp.xorq	q[0-7],q[0-7],q[0-7]
**	esp.movi.32.a	q[0-7],a[0-7],0
**	...
*/
int
c16 (const char *a, const char *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 16),
			   __builtin_assume_aligned (b, 16), 16);
}

/*
** c8:
**	...
**	esp.vld.l.64.ip	q[0-7],a[0-7],8
**	esp.vld.l.64.ip	q[0-7],a[0-7],8
**	esp.vld.h.64.ip	q[0-7],a[0-7],8
**	esp.vld.h.64.ip	q[0-7],a[0-7],8
**	esp.xorq	q[0-7],q[0-7],q[0-7]
**	...
*/
int
c8 (const char *a, const char *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 8),
			   __builtin_assume_aligned (b, 8), 16);
}

/* Misaligned sources are assembled with esp.src.q.  The last block is
   loaded with a zero increment, from the address of the last byte.  */
/*
** c1:
**	...
**	esp.movx.w.sar.bytes	a[0-7]
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],0
**	esp.src.q	q[0-7],q[0-7],q[0-7]
**	...
**	ret
*/
int
c1 (const char *a, const char *b)
{
  return __builtin_memcmp (a, b, 64);
}

/*
** tail:
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	...
**	lbu	a[0-7],0\(a[0-7]\)
**	...
**	ret
*/
int
tail (const char *a, const char *b)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 16),
			   __builtin_assume_aligned (b, 16), 17);
}

/*
** var:
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],16
**	...
**	esp.xorq	q[0-7],q[0-7],q[0-7]
**	...
*/
int
var (const char *a, const char *b, unsigned n)
{
  return __builtin_memcmp (__builtin_assume_aligned (a, 16),
			   __builtin_assume_aligned (b, 16), n);
}

/* Below the misaligned threshold the compare stays a library call.  */
/*
** short_mis:
**	...
**	tail	memcmp
*/
int
short_mis (const char *a, const char *b)
{
  return __builtin_memcmp (a, b, 32);
}

/* In the variable-length loop the block just loaded is copied back with
   esp.orq, so the next iteration shifts from that block rather than from
   the first one.  */
/* A short length branches to the byte loop.  The q-register loop copies
   the block it just loaded back with esp.orq.  */
/*
** vmis:
**	...
**	bltu	a[0-7],a[0-7],\.L[0-9]+
**	...
**	lbu	a[0-7],0\(a[0-7]\)
**	...
**	esp.vld.128.ip	(q[0-7]),[a-z][a-z0-9]*,16
**	esp.src.q	q[0-7],q[0-7],\1
**	esp.orq	q[0-7],\1,\1
**	...
*/
int
vmis (const char *a, const char *b, unsigned n)
{
  return __builtin_memcmp (a, b, n);
}
