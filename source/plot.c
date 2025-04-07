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
#include <plot.h>
#include <vectrex.h>

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#define NARRATOR 0
#define CROC     1
#define DOC      2
#define MSG(ch, cnt) ((cnt << 2)| ch)
#define MSG_N(cnt) MSG(NARRATORnt)
#define MSG_C(cnt) MSG(CROC, cnt)
#define MSG_D(cnt) MSG(DOC, cnt)
#define MSG_END 0x00
#define WORD_LU(word, arr) case word: g_word = arr; g_wordLen = sizeof arr; break

/////////////////////////////////////////////////////////////////////////
//	Globals
//
// [[data]]
const char snort[5]   = "SNORT";
const char wheeze[6]  = "WHEEZE";
const char nnghh[5]   = "NNGHH";
const char quack[5]   = "QUACK";
const char beware[6]  = "BEWARE";
const char laser[5]   = "LASER";

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
};

PlotPoint g_wakeUp[] = 
{
    MSG_D(5),
    Word_Snort,
    Word_Dot,
    Word_Wheeze,
    Word_Dot,
    Word_Quack,
    MSG_C(2),
    Word_Nnghh,
    Word_Dot,
    MSG_END,
};

PlotPoint* g_plots[] = 
{
    g_wakeUp,
};

// [[current]]
static char        g_msgBuf[0x80]; 
static u8          g_msgIdx = 0;
static v2i         g_msgPos;
static PlotPoint   g_plotPointIdx = 0;
static u8          g_plotPoints = 0;
static PlotPoint*  g_plot      = null;
static const char* g_word      = null;
static u8          g_wordIdx   = 0;
static u8          g_wordLen   = 0;
// [[config]]
static u8        g_typeWriterSpeed = 4;


/////////////////////////////////////////////////////////////////////////
//	Functions
//
void plot_typewriter_next(const u8 ticks)
{
    if (g_typeWriterSpeed ^ (g_typeWriterSpeed & ticks))
    {
    next_char:
        if (g_wordIdx < g_wordLen) // Next character
        {
            g_msgBuf[g_msgIdx++] = g_word[g_wordIdx++];
        }
        else if (g_plotPointIdx < g_plotPoints) // Next word
        {
        next_word:
            switch (g_plot[g_plotPointIdx++])
            {
            case Word_Space: 
                g_msgBuf[g_msgIdx++] = ' ';
                g_wordLen = 0;
                goto next_word;
            case Word_Dot: 
                g_msgBuf[g_msgIdx]     = '.';
                g_msgBuf[g_msgIdx + 1] = '.';
                g_msgBuf[g_msgIdx + 2] = '.';
                g_msgIdx += 3;
                g_wordLen = 0;
                break;
            WORD_LU(Word_Snort, snort);
            WORD_LU(Word_Wheeze, wheeze);
            WORD_LU(Word_Nnghh, nnghh);
            WORD_LU(Word_Quack, quack);
            WORD_LU(Word_Beware, beware);
            WORD_LU(Word_Laser, laser);
            default:
                break;
            }
            g_wordIdx = 0;
            goto next_char;
        }
        else 
        {            
            g_msgIdx = 0;
            if (g_plot[g_plotPointIdx]) // Next message
            {
                switch (g_plot[g_plotPointIdx] & 0x3)
                {
                case NARRATOR:
                    g_msgPos.y = -100;
                    g_msgPos.x = -50;                    
                    break;
                case CROC:
                    g_msgPos.y = 100;
                    g_msgPos.x = -50;
                    break;
                case DOC:
                    g_msgPos.y = 100;
                    g_msgPos.x = 50;
                    break;
                default:
                    break;
                }
                g_plotPoints   = g_plot[g_plotPointIdx] >> 2;
                ++g_plotPointIdx;
            }
            else // MSG_END
            {
                Print_Str_d(10, 10, "END OF STRING\x80");
                g_plot       = null;
                g_plotPoints = 0;
            }
        }
        g_msgBuf[g_msgIdx] = '\x80';
    }
}

bool plot_skip(void)
{
    if (g_plot == null) // End of plot
    {
        return true;
    }
    else // Skip to end of message
    {
        u8 ticks = 0;
        do
        {
            ticks ^= g_typeWriterSpeed;
            plot_typewriter_next(g_typeWriterSpeed);
        } while (g_plotPointIdx < g_plotPoints);
        return false; 
    }
}

void plot_print(void)
{
    Print_Str_d(g_msgPos.y, g_msgPos.x, g_msgBuf);
}

void plot_set_plot(const Plot plot)
{
    g_plot    = g_plots[plot];
    g_msgIdx  = 0;
    g_wordIdx = 0;
    plot_typewriter_next(g_typeWriterSpeed);   
}