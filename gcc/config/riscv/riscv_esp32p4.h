/* XESPV q-register types for ESP32-P4.
   Copyright (C) 2026 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _RISCV_ESP32P4_H
#define _RISCV_ESP32P4_H

/* Q-register operations are the __builtin_riscv_esp_* intrinsics, with
   the same names as LLVM.  esp_vec128_t is the 128-bit q value.  */

typedef __attribute__ ((vector_size (16))) char esp_vec128_t;
typedef __attribute__ ((vector_size (16))) short esp_vec128_16_t;
typedef __attribute__ ((vector_size (16))) int esp_vec128_32_t;

#endif
