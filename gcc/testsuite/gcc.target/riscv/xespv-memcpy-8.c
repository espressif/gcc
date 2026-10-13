/* { dg-do assemble } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv2p2 -mabi=ilp32f -O2 -mxespv-memcpy -save-temps" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-Og" "-Oz" "-flto" } } */

/* The same copies as xespv-memcpy-2.c with xespv 2.2, whose encodings
   differ from those of 3.0.  */

#define COPY(DA, SA)							\
void									\
v_d##DA##_s##SA (char *d, const char *s, unsigned int n)		\
{									\
  __builtin_memcpy (__builtin_assume_aligned (d, DA),			\
		    __builtin_assume_aligned (s, SA), n);		\
}

COPY (16, 16)
COPY (16, 8)
COPY (16, 1)
COPY (8, 16)
COPY (8, 8)
COPY (8, 1)
COPY (1, 16)
COPY (1, 8)
COPY (1, 1)

/* { dg-final { scan-assembler-times {\mesp\.movx\.w\.sar\.bytes\M} 5 } } */
/* { dg-final { scan-assembler {\mesp\.src\.q\M} } } */
/* { dg-final { scan-assembler {\mesp\.vld\.128\.ip\M} } } */
/* { dg-final { scan-assembler {\mesp\.vst\.128\.ip\M} } } */
/* { dg-final { scan-assembler {\mesp\.vld\.l\.64\.ip\M} } } */
/* { dg-final { scan-assembler {\mesp\.vld\.h\.64\.ip\M} } } */
/* { dg-final { scan-assembler {\mesp\.vst\.l\.64\.ip\M} } } */
/* { dg-final { scan-assembler {\mesp\.vst\.h\.64\.ip\M} } } */
/* { dg-final { scan-assembler-not {\m(call|tail)\tmemcpy\M} } } */
