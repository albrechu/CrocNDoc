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

#ifndef XUTILS_H
#define XUTILS_H

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

force_inline void beam_set_position(const i8 y, const i8 x)
{
	// Reset0Ref
	VIA_cntl         = 0xCC;
	//VIA_shift_reg    = 0x01;

	/*dp_VIA_t1_cnt_lo = 0x7F;*/
	/*Moveto_d(y, x);*/
	VIA_port_a    = y;		   // y pos to dac
	VIA_cntl      = 0xCE; // disable zero, disable all blank
	VIA_port_b    = 0;		   // mux enable, dac to -> integrator y (and x)
	VIA_shift_reg = 0;		   // all output is BLANK
	VIA_port_b++;			   // mux disable, dac only to x
	VIA_port_a = x;			   // dac -> x
	VIA_t1_cnt_hi = 0;		   // start timer
	while ((VIA_int_flags & 0x40) == 0); // wait till timer finishes
}

//force_inline bool is_local(v2l const x)
//{
//    return x.x < 128 && x.y < 128 && x.x > -129 && x.y > -129;
//}

//force_inline bool is_global(v2l const x)
//{
//    return !is_local(x);
//}
u8 manhattan(i8 const a, i8 const b)
{
    const i8 aMask = a >> 7;
    const i8 bMask = b >> 7;
    return U8((a ^ aMask) - aMask) + U8((b ^ bMask) - bMask);
}

//force_inline i8 max(i8 const x, i8 const y)
//{
//    return x - ((x - y) & ((x - y) >> 7));
//}
//
//force_inline i8 min(i8 const x, i8 const y)
//{
//    return y + ((x - y) & ((x - y) >> 7));
//}

#endif /* XUTILS */