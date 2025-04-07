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

#ifndef TRACK_H
#define TRACK_H

#include <defines.h>
#include <types.h>

enum Note_
{
    Note_G2 = 0x00U, Note_GS2 = 0x01U,
    Note_A2 = 0x02U, Note_AS2 = 0x03U,
    Note_B2 = 0x04U,
    Note_C3 = 0x05U, Note_CS3 =	0x06U,
    Note_D3 = 0x07U, Note_DS3 =	0x08U,
    Note_E3 = 0x09U,
    Note_F3 = 0x0AU, Note_FS3 =	0x0BU,
    Note_G3 = 0x0CU, Note_GS3 =	0x0DU,
    Note_A3 = 0x0EU, Note_AS3 = 0x0FU,
    Note_B3 = 0x10U,
    Note_C4 = 0x11U, Note_CS4 = 0x12U,
    Note_D4 = 0x13U, Note_DS4 =	0x14U,
    Note_E4 = 0x15U,
    Note_F4 = 0x16U, Note_FS4 = 0x17U,
    Note_G4 = 0x18U, Note_GS4 = 0x19U,
    Note_A4 = 0x1AU, Note_AS4 =	0x1BU,
    Note_B4 = 0x1CU,
    Note_C5 = 0x1DU, Note_CS5 =	0x1EU,
    Note_D5 = 0x1FU, Note_DS5 = 0x20U,
    Note_E5 = 0x21U,
    Note_F5 = 0x22U, Note_FS5 =	0x23U,
    Note_G5 = 0x24U, Note_GS5 = 0x25U,
    Note_A5 = 0x26U, Note_AS5 = 0x27U,
    Note_B5 = 0x28U,
    Note_C6 = 0x29U, Note_CS6 = 0x2AU,
    Note_D6 = 0x2BU, Note_DS6 = 0x2CU,
    Note_E6 = 0x2DU,
    Note_F6 = 0x2EU, Note_FS6 =0x2FU,
    Note_G6 = 0x30U, Note_GS6 =0x31U,
    Note_A6 = 0x32U, Note_AS6 =0x33U,
    Note_B6 = 0x34U,
    Note_C7 = 0x35U, Note_CS7 =0x36U,
    Note_D7 = 0x37U, Note_DS7 =0x38U,
    Note_E7 = 0x39U,
    Note_F7 = 0x3AU, Note_FS7 = 0x3BU,
    Note_G7 = 0x3CU, Note_GS7 = 0x3DU,
    Note_A7 = 0x3EU, Note_AS7 = 0x3FU,
    Note_End = 0x80,
};
typedef u8 Note;

enum Track_
{
    Track_Level0,
};
typedef u8 Track;

typedef struct nibble_ampoff_table_t
{
    u8 amplitudes[16];
} nibble_ampoff_table_t, *nibble_ampoff_table;

typedef struct twang_table_t
{
    u8 frequencies[8];
} twang_table_t, *twang_table;


typedef struct track_t
{
    nibble_ampoff_table_t const* amplitudes;
    twang_table_t const*         frequencies;
    Note notes[];
} track_t, *track;

extern const track_t g_level0;

force_inline const track_t* track_get(Track const track);



#endif /* TRACK_H */