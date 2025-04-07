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

#ifndef DEFINES_H
#define DEFINES_H

/////////////////////////////////////////////////////////////////////////
//	Attributes
//
#define force_inline __attribute__((always_inline)) // GCC-specific
#define __out			// Indicates that its state does not matter - it is merely written to.
#define __in           // Indicates that it is merely used.
#define __inout        // Indicates that both is used and written to.
#define HAS_SIZE(size) // Indicates the size in bytes.
#define CONST          // Indicates that it is expected that this value is never changed.

/////////////////////////////////////////////////////////////////////////
//	Configuration
//
#define ENGLISH
//#define GERMAN
#define DEVELOPMENT

/////////////////////////////////////////////////////////////////////////
//	Macro Helper Values
//
#define EOS "\x80" // End of String

/////////////////////////////////////////////////////////////////////////
//	Macro Helper Functions
//
#define VSTR(literal)       literal EOS  // Vectrex string
// Define function pointer with return value
#define DEFINE_RET_PFN(func, addr, ret, ...) \
	typedef ret (*func##_t)(__VA_ARGS__);\
	static const func##_t func = (func##_t)addr
// Define function pointer without return value
#define DEFINE_PFN(func, addr, ...) \
	DEFINE_RET_PFN(func, addr, void, __VA_ARGS__)

#define SWAP(x, y) x ^= y;\
				   y ^= x;\
				   x ^= y

#define LENGTH(n) n-1 
#define MIN8(x, y) y + ((x - y) & ((x - y) >> 7))
#define MAX8(x, y) x - ((x - y) & ((x - y) >> 7))

#endif /* DEFINES_H */