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

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/types.h>

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define PLOT g_plot

/////////////////////////////////////////////////////////////////////////
//	Types
//
enum Plot_
{
    Plot_Tutorial,
    Plot_JumpWorld,
    Plot_Water,
    Plot_Gravity,
};
typedef u8 Plot;
typedef u8 PlotPoint;

typedef struct plot_t
{
    // [[current]]
    PlotPoint const* point;
    const char*      word;
    char             msg[0x40];
    u8               msgIdx;
    v2i              msgPos;
    PlotPoint        pointIdx;
    u8               points;
    u8               wordIdx;
    u8               wordLen;
    // [[config]]
    u8 typeWriterTicks;
} plot_t;

/////////////////////////////////////////////////////////////////////////
//	Globals
//
extern plot_t g_plot;

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void plot_set_plot(const Plot plot);
void plot_typewriter_next(void);
bool plot_skip(void);
void plot_init(void);