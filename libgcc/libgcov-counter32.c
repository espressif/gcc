/* 32-bit in-memory gcov counter entry points for -fprofile-counter-size=32.
   Kept in its own translation unit so libgcov-merge.c and libgcov-profiler.c
   stay identical to upstream.  */
/* Copyright (C) 2026 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "libgcov.h"

#if defined(inhibit_libc)

void __gcov_merge_add_32 (gcov_s32 *counters __attribute__ ((unused)),
			  unsigned n_counters __attribute__ ((unused))) {}
void __gcov_merge_ior_32 (gcov_s32 *counters __attribute__ ((unused)),
			  unsigned n_counters __attribute__ ((unused))) {}
void __gcov_merge_time_profile_32 (gcov_s32 *counters
				   __attribute__ ((unused)),
				   unsigned n_counters __attribute__ ((unused)))
{}

#else

void
__gcov_merge_add_32 (gcov_s32 *counters, unsigned n_counters)
{
  gcov_u32 *c32 = (gcov_u32 *) counters;

  for (; n_counters; c32++, n_counters--)
    *c32 += (gcov_u32) gcov_get_counter ();
}

void
__gcov_merge_ior_32 (gcov_s32 *counters, unsigned n_counters)
{
  gcov_u32 *c32 = (gcov_u32 *) counters;

  for (; n_counters; c32++, n_counters--)
    *c32 |= (gcov_u32) gcov_get_counter_target ();
}

void
__gcov_merge_time_profile_32 (gcov_s32 *counters, unsigned n_counters)
{
  unsigned int i;

  for (i = 0; i < n_counters; i++)
    {
      gcov_type value = gcov_get_counter_target ();

      if (value && (!counters[i] || value < counters[i]))
	counters[i] = (gcov_s32) value;
    }
}

/* Weak default.  A strong user definition overrides this copy.  */

__attribute__ ((weak)) gcov_s32
__gcov_increment_fn_32 (gcov_s32 *counter)
{
  gcov_u32 *c32 = (gcov_u32 *) counter;
  return ++*c32;
}

#endif
