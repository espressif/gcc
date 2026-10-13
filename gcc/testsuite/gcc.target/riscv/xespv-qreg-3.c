/* { dg-do compile } */
/* { dg-options "-march=rv32imafc_zicsr_zifencei_xespv -mabi=ilp32f -O2" } */

/* q registers are call-clobbered and are spilled to the stack around
   calls; more live q values than q registers also need spilling.  Spill
   slots are accessed through a base register in x8-x15/x24-x31.  */

typedef __attribute__ ((vector_size (16))) char esp_vec128_t;

extern void ext (void);
esp_vec128_t sink[10];

void
across_call (void *p)
{
  esp_vec128_t a = __builtin_riscv_esp_zero_q ();
  a = __builtin_riscv_esp_orq (a, a);
  ext ();
  esp_vec128_t c = __builtin_riscv_esp_notq (a);
  (void) __builtin_riscv_esp_vst_128_ip (c, p, 0);
}

void
pressure (void)
{
  esp_vec128_t v0 = __builtin_riscv_esp_zero_q ();
  esp_vec128_t v1 = __builtin_riscv_esp_notq (v0);
  esp_vec128_t v2 = __builtin_riscv_esp_notq (v1);
  esp_vec128_t v3 = __builtin_riscv_esp_notq (v2);
  esp_vec128_t v4 = __builtin_riscv_esp_notq (v3);
  esp_vec128_t v5 = __builtin_riscv_esp_notq (v4);
  esp_vec128_t v6 = __builtin_riscv_esp_notq (v5);
  esp_vec128_t v7 = __builtin_riscv_esp_notq (v6);
  esp_vec128_t v8 = __builtin_riscv_esp_notq (v7);
  esp_vec128_t v9 = __builtin_riscv_esp_notq (v8);
  esp_vec128_t s = __builtin_riscv_esp_orq (v0, v1);
  s = __builtin_riscv_esp_orq (s, v2);
  s = __builtin_riscv_esp_orq (s, v3);
  s = __builtin_riscv_esp_orq (s, v4);
  s = __builtin_riscv_esp_orq (s, v5);
  s = __builtin_riscv_esp_orq (s, v6);
  s = __builtin_riscv_esp_orq (s, v7);
  s = __builtin_riscv_esp_orq (s, v8);
  s = __builtin_riscv_esp_orq (s, v9);
  sink[0] = v0;
  sink[1] = v1;
  sink[2] = v2;
  sink[3] = v3;
  sink[4] = v4;
  sink[5] = v5;
  sink[6] = v6;
  sink[7] = v7;
  sink[8] = v8;
  sink[9] = v9;
  (void) s;
}

/* { dg-final { scan-assembler "esp\\.notq\tq\[0-7\],q\[0-7\]" } } */
/* { dg-final { scan-assembler "esp\\.vst\\.128\\.ip\tq\[0-7\],(s0|s1|a\[0-5\]|s8|s9|s10|s11|t\[3-6\]),0" } } */
/* { dg-final { scan-assembler-not "esp\\.v(ld|st)\\.128\\.ip\tq\[0-7\],(zero|ra|sp|gp|tp|t\[0-2\]|a\[67\]|s\[2-7\]),0" } } */
