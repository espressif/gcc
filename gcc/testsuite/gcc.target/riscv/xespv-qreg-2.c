/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */

/* Pseudos produced by xespv intrinsics are allocated to q registers and
   loaded or stored directly, without going through general registers.  */

typedef __attribute__ ((vector_size (16))) char esp_vec128_t;

void
add (void *pa, void *pb, void *pc)
{
  esp_vec128_t a, b, c;
  pa = __builtin_riscv_esp_vld_128_ip (pa, 0, &a);
  pb = __builtin_riscv_esp_vld_128_ip (pb, 0, &b);
  c = __builtin_riscv_esp_xorq (a, b);
  (void) __builtin_riscv_esp_vst_128_ip (c, pc, 0);
}

void
loop (esp_vec128_t *p, int n)
{
  esp_vec128_t acc = __builtin_riscv_esp_zero_q ();
  for (int i = 0; i < n; i++)
    {
      esp_vec128_t x;
      __builtin_riscv_esp_vld_128_ip (&p[i], 0, &x);
      acc = __builtin_riscv_esp_xorq (acc, x);
    }
  (void) __builtin_riscv_esp_vst_128_ip (acc, p, 0);
}

void
constant (void *pc)
{
  esp_vec128_t k = { 1, 2, 3, 4 };
  esp_vec128_t z = __builtin_riscv_esp_zero_q ();
  esp_vec128_t c = __builtin_riscv_esp_xorq (k, z);
  (void) __builtin_riscv_esp_vst_128_ip (c, pc, 0);
}

/* { dg-final { scan-assembler "esp\\.xorq\tq\[0-7\],q\[0-7\],q\[0-7\]" } } */
/* { dg-final { scan-assembler "esp\\.zero\\.q\tq\[0-7\]" } } */
/* { dg-final { scan-assembler-not "\t(lw|sw)\t" } } */
/* { dg-final { scan-assembler-not "esp\\.v(ld|st)\\.128\\.ip\tq\[0-7\],(zero|ra|sp|gp|tp|t\[0-2\]|a\[67\]|s\[2-7\]),0" } } */
