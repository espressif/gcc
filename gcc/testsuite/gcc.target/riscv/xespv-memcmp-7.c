/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei -mabi=ilp32f -O2 -mtune=esp32p4 -mxespv-memcmp" } */

/* The option does nothing without xespv.  */

#include "xespv-memcmp-1.c"

/* { dg-final { scan-assembler-not {\mesp\.} } } */
