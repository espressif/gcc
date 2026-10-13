/* { dg-do compile } */
/* { dg-options "-march=rv32ec_zicsr_xespv -mabi=ilp32e -O2 -mxespv-memcmp" } */

/* q registers are not available with ilp32e.  */

#include "xespv-memcmp-1.c"

/* { dg-final { scan-assembler-not {\mesp\.} } } */
