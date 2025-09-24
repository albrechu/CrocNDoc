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
//	Includes
//
#include <cnd/types.h>

/////////////////////////////////////////////////////////////////////////
//	Types
//
typedef struct element_t
{
    void* data;
    i8 y;
    i8 x;
} element_t, *element;

typedef union queue_pointer_t
{
    element_t* element;
    struct
    {
        u8 page;
        u8 idx;
    };
} queue_pointer_t;

/////////////////////////////////////////////////////////////////////////
//	Globals
//
extern queue_pointer_t drawQueue;

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void draw_stack_clear(void);
void draw_stack_draw(void);

#define draw_queue_push(mesh, y, x)\
{\
    *drawQueue.element = (element_t){ (void*)(mesh), (y), (x) };\
    drawQueue.idx     += (u8)sizeof(element_t);\
}
