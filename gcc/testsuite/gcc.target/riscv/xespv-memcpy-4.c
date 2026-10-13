/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2 -mtune=esp32p4" } */

/* -mxespv-memcpy is off by default.  */

#include "xespv-memcpy-1.c"

/* { dg-final { scan-assembler-not {\mesp\.} } } */
