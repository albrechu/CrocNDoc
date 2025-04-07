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

#ifndef TYPES_H
#define TYPES_H

/////////////////////////////////////////////////////////////////////////
//	Integral Types
//
typedef unsigned int  u8;
typedef signed   int  i8;
typedef unsigned long u16;
typedef signed long   i16;
typedef u8            event_t;
typedef u8            result_t;
typedef u8            tick_t;
typedef i8            bool;

enum /* Generic */
{
	true  = 1,
	false = 0,
	null  = 0,
};

/////////////////////////////////////////////////////////////////////////
//	Function Pointers
//
typedef void(*procedure_t)(void);

/////////////////////////////////////////////////////////////////////////
//	Math Types
//
typedef struct v2i
{
	i8 x;
	i8 y;
} v2i;

typedef union vec2
{
	struct 
	{
		i8 x_msb;
		i8 x_lsb;
		i8 y_msb;
		i8 y_lsb;
	};
	struct
	{
		i16 x;
		i16 y;
	};
} vec2;

#define VEC_IS_LOCAL(vec) (vec.x_msb | vec.y_msb) == 0 || (vec.x_msb | vec.y_msb) == -1 // Checks whether a 16 bit vector is within 8-bit coords.
#define VEC_IS_GLOBAL(vec) (vec.x_msb | vec.y_msb) != 0 && (vec.x_msb | vec.y_msb) != -1

/** 
* @brief rect_t is a rectangle defined by its position and size.
* 
* Right-Handed Coordinate System (RHCS):
*              127
*               + top
*               |
* left 128 - ---0--- + 127 right
*               |
*               - bottom
*              128
*/
typedef union rect_t
{
	struct // attributes
	{
		v2i pos; // bottom-left
		v2i size;
	};
	struct // components
	{
		i8 x; // left
		i8 y; // bottom
		i8 width; 
		i8 height;
	};
} rect_t, *rect;

#endif /* TYPES_H */