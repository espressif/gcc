/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -Os -mtune=esp32p4 -mxespv-memcmp" } */
/* { dg-skip-if "" { *-*-* } { "-O2" "-O3" } } */

/* -mxespv-memcmp only applies when optimizing for speed.  */

#include "xespv-memcmp-1.c"

/* { dg-final { scan-assembler-not {\mesp\.} } } */
