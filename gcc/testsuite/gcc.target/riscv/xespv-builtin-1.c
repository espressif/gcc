/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */

typedef __attribute__ ((vector_size (16))) char esp_vec128_t;
typedef __attribute__ ((vector_size (16))) int esp_vec128_32_t;

void
logic (void *pa, void *pb, void *pc)
{
  esp_vec128_t a, b, c;
  pa = __builtin_riscv_esp_vld_128_ip (pa, 16, &a);
  pb = __builtin_riscv_esp_vld_128_ip (pb, 16, &b);
  c = __builtin_riscv_esp_andq (a, b);
  c = __builtin_riscv_esp_orq (c, a);
  c = __builtin_riscv_esp_xorq (c, b);
  c = __builtin_riscv_esp_notq (c);
  (void) __builtin_riscv_esp_vst_128_ip (c, pc, 16);
}

void
shift_bytes (void *pc)
{
  esp_vec128_t a = __builtin_riscv_esp_zero_q ();
  esp_vec128_t b = __builtin_riscv_esp_notq (a);
  esp_vec128_t c = __builtin_riscv_esp_src_q (a, b, 4);
  (void) __builtin_riscv_esp_vst_128_ip (c, pc, 0);
}

int
extract (esp_vec128_t *p, esp_vec128_32_t *q)
{
  esp_vec128_t a;
  esp_vec128_32_t b;
  __builtin_riscv_esp_vld_128_ip (p, 0, &a);
  __builtin_riscv_esp_vld_128_ip (q, 0, &b);
  return (__builtin_riscv_esp_movi_8_a (a, 3)
	  + __builtin_riscv_esp_movi_32_a (b, 1));
}

/* { dg-final { scan-assembler "esp\\.vld\\.128\\.ip" } } */
/* { dg-final { scan-assembler "esp\\.vst\\.128\\.ip" } } */
/* { dg-final { scan-assembler "esp\\.andq" } } */
/* { dg-final { scan-assembler "esp\\.orq" } } */
/* { dg-final { scan-assembler "esp\\.xorq" } } */
/* { dg-final { scan-assembler "esp\\.notq" } } */
/* { dg-final { scan-assembler "esp\\.zero\\.q" } } */
/* { dg-final { scan-assembler "esp\\.movx\\.w\\.sar\\.bytes" } } */
/* { dg-final { scan-assembler "esp\\.src\\.q" } } */
/* { dg-final { scan-assembler "esp\\.movi\\.8\\.a" } } */
/* { dg-final { scan-assembler "esp\\.movi\\.32\\.a" } } */
