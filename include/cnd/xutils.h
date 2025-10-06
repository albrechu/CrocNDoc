/*
 * MIT License
 * 
 * Copyright (c) 2025 Julian Albrecht
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define MEMZERO(ref) memset(&ref, 0, sizeof(ref));

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/types.h>
#include <vectrex.h>

/////////////////////////////////////////////////////////////////////////
//	Utility Functions
//
void* memset(void* dest, int val, long unsigned int len);

force_inline void reset_pen(void)
{
	VIA_port_a = 0;    // clear D/A register
	VIA_port_b = 0x03; // mux=1, disable mux
	VIA_port_b = 0x02; // mux=1, enable mux
	VIA_port_b = 0x02; // do it again
	VIA_port_b = 0x01; // disable mux
}
force_inline void reset_0_ref(void)
{
	VIA_cntl      = 0xCC;
	VIA_shift_reg = 0x00;

	reset_pen();
}

force_inline i8 abs_a(const i8 x)
{
	const i8 mask = x >> ((sizeof(i8) * 8) - 1);
	return (x + mask) ^ mask;
}

force_inline void moveto_d(const i8 y, const i8 x)
{
	VIA_port_a    = y;
	VIA_port_b    = 0;
	VIA_cntl      = 0xCE;
	VIA_shift_reg = 0x00;
	VIA_port_b    = 0x01;
	VIA_port_a    = x;
	VIA_t1_cnt_hi = 0x00;
	const i8 yAbs = abs_a(y);
	i8 xAbs = abs_a(x);
	xAbs = xAbs > yAbs ? xAbs : yAbs;
	i8 nop = ((xAbs > 0x40) << 2) | ((xAbs > 0x64) << 1) << 2;

	while ((VIA_int_flags & 0x40) == 0);
	while (nop--);
}

force_inline void beam_set_position(const i8 y, const i8 x)
{
	reset_0_ref();
	Moveto_d(y, x);
}

u8  manhattan(i8 const a, i8 const b);
u16 random();
void random_mix_seed(u8 value);


