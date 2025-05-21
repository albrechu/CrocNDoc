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
#include <cnd/plot.h>
#include <cnd/xutils.h>
#include <cnd/globals.h>
#include <vectrex.h>

/////////////////////////////////////////////////////////////////////////
//	Defines
//
#define NARRATOR 0
#define CROC     1
#define DOC      2
#define MSG(ch, cnt) ((cnt << 2)| ch)
#define MSG_N(cnt) MSG(NARRATOR, cnt)
#define MSG_C(cnt) MSG(CROC, cnt)
#define MSG_D(cnt) MSG(DOC, cnt)
#define MSG_END 0x00
#define WORD_LU(wd, arr) case wd: PLOT.word = arr; PLOT.wordLen = sizeof arr; break

/////////////////////////////////////////////////////////////////////////
//	Globals
//
// [[data]]
const char snort[5]    = "SNORT";
const char wheeze[6]   = "WHEEZE";
const char nnghh[5]    = "NNGHH";
const char quack[5]    = "QUACK";
const char beware[6]   = "BEWARE";
const char laser[5]    = "LASER";
const char you[3]      = "YOU";
const char feel[4]     = "FEEL";
const char a[1]        = "A";
const char menacing[8] = "MENACING";
const char presence[8] = "PRESENCE";
const char watching[8] = "WATCHING";
const char of[2]       = "OF";
const char traps[5]    = "TRAPS";

enum Word_
{
    Word_Space,
    Word_Dot,
    Word_Snort,
    Word_Wheeze,
    Word_Nnghh,
    Word_Quack,
    Word_Beware,
    Word_Laser,
    Word_You,
    Word_Feel,
    Word_A,
    Word_Menacing,
    Word_Presence,
    Word_Watching,
    Word_Of,
    Word_Traps,
};

const PlotPoint g_tutorial[] = 
{
    MSG_D(5),
    Word_Snort,
    Word_Dot,
    Word_Wheeze,
    Word_Dot,
    Word_Quack,
    MSG_N(8),
    Word_You,
    Word_Feel,
    Word_A,
    Word_Menacing,
    Word_Presence,
    Word_Watching,
    Word_You,
    Word_Dot,
    MSG_N(4),
    Word_Beware,
    Word_Of,
    Word_Traps,
    Word_Dot,
    MSG_END,
};

const PlotPoint* g_plots[] = 
{
    g_tutorial,
};

plot_t g_plot;

/////////////////////////////////////////////////////////////////////////
//	Functions
//
void plot_init(void) 
{
    MEMZERO(PLOT);
    PLOT.typeWriterTicks = (1 << 5);
}

void plot_typewriter_next(void)
{
    if (PLOT.typeWriterTicks & WORLD.ticks)
    {
    next_char:
        if (PLOT.wordIdx < PLOT.wordLen) // Next character
        {
            PLOT.msg[PLOT.msgIdx++] = PLOT.word[PLOT.wordIdx++];
        }
        else if (PLOT.pointIdx < PLOT.points) // Next word
        {
        next_word:
            switch (PLOT.point[PLOT.pointIdx++])
            {
            case Word_Space: 
                PLOT.msg[PLOT.msgIdx++] = ' ';
                PLOT.wordLen = 0;
                goto next_word;
            case Word_Dot: 
                PLOT.msg[PLOT.msgIdx]     = '.';
                PLOT.msg[PLOT.msgIdx + 1] = '.';
                PLOT.msg[PLOT.msgIdx + 2] = '.';
                PLOT.msgIdx += 3;
                PLOT.wordLen = 0;
                break;
            WORD_LU(Word_Snort, snort);
            WORD_LU(Word_Wheeze, wheeze);
            WORD_LU(Word_Nnghh, nnghh);
            WORD_LU(Word_Quack, quack);
            WORD_LU(Word_Beware, beware);
            WORD_LU(Word_Laser, laser);
            WORD_LU(Word_You, you);
            WORD_LU(Word_Feel, feel);
            WORD_LU(Word_A, a);
            WORD_LU(Word_Menacing, menacing);
            WORD_LU(Word_Presence, presence);
            WORD_LU(Word_Watching, watching);
            WORD_LU(Word_Of, of);
            WORD_LU(Word_Traps, traps);
            default:
                break;
            }
            PLOT.wordIdx = 0;
            goto next_char;
        }
        else 
        {            
            PLOT.msgIdx = 0;
            if (PLOT.point[PLOT.pointIdx]) // Next message
            {
                switch (PLOT.point[PLOT.pointIdx] & 0x3)
                {
                case NARRATOR:
                    PLOT.msgPos.y = -80;
                    PLOT.msgPos.x = -80;                    
                    break;
                case CROC:
                    PLOT.msgPos.y = 80;
                    PLOT.msgPos.x = -80;
                    break;
                case DOC:
                    PLOT.msgPos.y = 80;
                    PLOT.msgPos.x = -80;
                    break;
                default:
                    break;
                }
                PLOT.points = PLOT.point[PLOT.pointIdx] >> 2;
                ++PLOT.pointIdx;
            }
            else // MSG_END
            {
                PLOT.point  = null;
                PLOT.points = 0;
            }
        }
        PLOT.msg[PLOT.msgIdx] = '\x80';
    }

    Print_Str_d(PLOT.msgPos.y, PLOT.msgPos.x, PLOT.msg); 
}

bool plot_skip(void)
{
    if (PLOT.point == null) // End of plot
    {
        return true;
    }
    else // Skip to end of message
    {
        do
        {
            WORLD.ticks = PLOT.typeWriterTicks;
            plot_typewriter_next();
        } while (PLOT.pointIdx < PLOT.points);
        return false; 
    }
}

void plot_set_plot(const Plot plot)
{
    PLOT.point = g_plots[plot];
    PLOT.msgIdx  = 0;
    PLOT.wordIdx = 0;
    plot_typewriter_next();   
}