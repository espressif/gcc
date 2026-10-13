/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=rocket -mxespv-memcmp" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-Og" "-Oz" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

/* rocket reports slow misaligned accesses, so the shifted expansion
   starts at 32 bytes.  16 bytes stay a library call.  */

/*
** s32:
**	...
**	esp.src.q	q[0-7],q[0-7],q[0-7]
**	...
**	esp.vld.128.ip	q[0-7],a[0-7],0
**	...
*/
int
s32 (const void *a, const void *b)
{
  return __builtin_memcmp (a, b, 32);
}

/*
** s16:
**	...
**	tail	memcmp
*/
int
s16 (const void *a, const void *b)
{
  return __builtin_memcmp (a, b, 16);
}
