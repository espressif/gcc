/* Definitions of option handling for Tensilica's Xtensa target machine.
   Copyright (C) 2019 Free Software Foundation, Inc.
   Contributed by Espressif <jeroen@espressif.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */



#ifndef XTENSA_OPTS_H
#define XTENSA_OPTS_H


enum esp32_psram_fix_type
{
	ESP32_PSRAM_FIX_DUPLDST,
	ESP32_PSRAM_FIX_MEMW,
	ESP32_PSRAM_FIX_NOPS
};


#endif