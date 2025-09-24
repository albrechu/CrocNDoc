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
//	Macro Values
//
#define EOS					"\x80" // End of String
#define TILE_SCALE_BITS     5
#define TILES_VIEW_WIDTH    I8(256u >> (TILE_SCALE_BITS + 1))
#define TILES_VIEW_HEIGHT   I8(256u >> (TILE_SCALE_BITS + 1))
#define WORLD_WIDTH         8
#define WORLD_HEIGHT        8
#define WORLD_STRIDE        I8(TILES_VIEW_WIDTH * WORLD_WIDTH)
#define STRIDE_BITS         5
#define WORLD_EXTENT        I8(TILES_VIEW_HEIGHT * WORLD_HEIGHT)
#define TILE_WIDTH          I8(1 << 5)
#define TILE_HEIGHT         I8(1 << 5)
#define TILE_WIDTH_2        (TILE_WIDTH  >> 1)
#define TILE_WIDTH_4        (TILE_WIDTH  >> 2)
#define TILE_WIDTH_3_4      (TILE_WIDTH - TILE_WIDTH_4) 
#define TILE_HEIGHT_2       (TILE_HEIGHT >> 1)
#define TILE_HEIGHT_4       (TILE_HEIGHT >> 2)
#define TILE_HEIGHT_3_4     (TILE_HEIGHT - TILE_HEIGHT_4) 
#define PLATFORM_TOLERANCE   8
#define PLATFORM_TOLERANCE_2 (PLATFORM_TOLERANCE >> 1)
#define ENTITIES_ACTIVE_MAX 6  // Maximum of entities that can be active.
#define ENTITIES_MAX        16 // Maximum of entities that can be in a level.
#define TEXT_BIG_HEIGHT     -8
#define TEXT_BIG_WIDTH      80
#define TEXT_SMALL_HEIGHT   -4
#define TEXT_SMALL_WIDTH    60


#define ID_INVALID -1
#define ID_CAMERA   0

/////////////////////////////////////////////////////////////////////////
//	Macro References
//
#define WORLD  g_world
#define GAME   g_game
#define CAMERA WORLD.entities[ID_CAMERA]
#define PLAYER GAME.player
#define BTNS   Vec_Buttons

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

#define LENGTH(n) n-1 
#define MIN8(x, y) y + ((x - y) & ((x - y) >> 7))
#define MAX8(x, y) x - ((x - y) & ((x - y) >> 7))
#define MOD2(x, y) ((x) & (y - 1))

#define U8(x)  ((u8)(x))
#define I8(x)  ((i8)(x))
#define U16(x) ((u16)(x))
#define I16(x) ((i16)(x))

#define ARRAY_SIZE(arr)   (sizeof(arr) / sizeof((arr)[0]))
#define ELEMENT_SIZE(arr) (sizeof((arr)[0]))
