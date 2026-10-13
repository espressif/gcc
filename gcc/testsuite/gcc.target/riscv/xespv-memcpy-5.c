/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -Os -mtune=esp32p4 -mxespv-memcpy" } */
/* { dg-skip-if "" { *-*-* } { "-O2" "-O3" } } */

/* -mxespv-memcpy only applies when optimizing for speed.  */

#include "xespv-memcpy-1.c"

/* { dg-final { scan-assembler-not {\mesp\.} } } */
