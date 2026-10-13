/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=esp32p4 -mxespv-memcpy" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-Og" "-Oz" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Constant-size copies for each combination of 16-byte, 8-byte and byte
   alignment of the destination and the source.  */

#define COPY(DA, SA, N)							\
void									\
c_d##DA##_s##SA##_##N (char *d, const char *s)				\
{									\
  __builtin_memcpy (__builtin_assume_aligned (d, DA),			\
		    __builtin_assume_aligned (s, SA), N);		\
}

#define SIZES(DA, SA)							\
  COPY (DA, SA, 0) COPY (DA, SA, 1) COPY (DA, SA, 2) COPY (DA, SA, 3)	\
  COPY (DA, SA, 4) COPY (DA, SA, 5) COPY (DA, SA, 6) COPY (DA, SA, 7)	\
  COPY (DA, SA, 8) COPY (DA, SA, 9) COPY (DA, SA, 10)			\
  COPY (DA, SA, 11) COPY (DA, SA, 12) COPY (DA, SA, 13)		\
  COPY (DA, SA, 14) COPY (DA, SA, 15) COPY (DA, SA, 16)		\
  COPY (DA, SA, 17) COPY (DA, SA, 31) COPY (DA, SA, 32)		\
  COPY (DA, SA, 33) COPY (DA, SA, 47) COPY (DA, SA, 48)		\
  COPY (DA, SA, 49) COPY (DA, SA, 63) COPY (DA, SA, 64)		\
  COPY (DA, SA, 65) COPY (DA, SA, 80) COPY (DA, SA, 81)		\
  COPY (DA, SA, 88) COPY (DA, SA, 96) COPY (DA, SA, 112)		\
  COPY (DA, SA, 124) COPY (DA, SA, 127) COPY (DA, SA, 128)		\
  COPY (DA, SA, 129) COPY (DA, SA, 255) COPY (DA, SA, 256)		\
  COPY (DA, SA, 1023) COPY (DA, SA, 1024) COPY (DA, SA, 20000)

/*
** c_d16_s16_16:
**	esp.vld.128.ip	(q[0-7]),a1,16
**	esp.vst.128.ip	\1,a0,16
**	ret
*/

/*
** c_d16_s8_16:
**	esp.vld.l.64.ip	(q[0-7]),a1,8
**	esp.vld.h.64.ip	\1,a1,8
**	esp.vst.128.ip	\1,a0,16
**	ret
*/

/*
** c_d8_s16_16:
**	esp.vld.128.ip	(q[0-7]),a1,16
**	esp.vst.l.64.ip	\1,a0,8
**	esp.vst.h.64.ip	\1,a0,8
**	ret
*/

/*
** c_d8_s8_16:
**	esp.vld.l.64.ip	(q[0-7]),a1,8
**	esp.vld.h.64.ip	\1,a1,8
**	esp.vst.l.64.ip	\1,a0,8
**	esp.vst.h.64.ip	\1,a0,8
**	ret
*/

/* The last block is loaded from the address of the last source byte, so
   that no block without source bytes is read.  */
/*
** c_d16_s1_32:
**	andi	(a[0-7]),a1,15
**	esp.movx.w.sar.bytes	\1
**	andi	a1,a1,-16
**	...
**	add	a1,a1,\1
**	addi	a1,a1,-1
**	...
**	andi	a1,a1,-16
**	esp.vld.128.ip	(q[0-7]),a1,0
**	esp.src.q	(q[0-7]),q[0-7],\2
**	esp.vst.128.ip	\3,a[0-7],16
**	ret
*/

/*
** c_d16_s16_256:
**	addi	(a[0-7]),a0,256
**	esp.vld.128.ip	(q[0-7]),a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vld.128.ip	q[0-7],a1,16
**	esp.vst.128.ip	\2,a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	esp.vst.128.ip	q[0-7],a0,16
**	bne	a0,\1,.L[0-9]+
**	ret
*/

SIZES (16, 16)
SIZES (16, 8)
SIZES (16, 1)
SIZES (8, 16)
SIZES (8, 8)
SIZES (8, 1)
SIZES (1, 16)
SIZES (1, 8)
SIZES (1, 1)

/* With fast scalar misaligned accesses, misaligned sources are copied with
   q registers from 32 bytes on and misaligned destinations from 64 bytes
   on, each with one SAR_BYTE setup.  */
/* { dg-final { scan-assembler-times {\mesp\.movx\.w\.sar\.bytes\M} 92 } } */
/* { dg-final { scan-assembler-not {\m(call|tail)\tmemcpy\M} } } */
