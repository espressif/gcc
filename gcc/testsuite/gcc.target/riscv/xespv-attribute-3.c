/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32imac_xespv2p2 -mabi=ilp32" } */

int
foo (void)
{
  return 0;
}

/* { dg-final { scan-assembler "\\.attribute arch, \"rv32i2p1_m2p0_a2p1_c2p0_\[a-z0-9_\]*xespv2p2\"" } } */
