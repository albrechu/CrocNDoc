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

#include <cnd/types.h>

enum Note_
{
    G2 = 0x00U, GS2 = 0x01U, AB2 = 0x01U,
    A2 = 0x02U, AS2 = 0x03U, BB2 = 0x03U,
    BH = 0x04U,
    C3 = 0x05U, CS3 = 0x06U, DB3 = 0x06U,
    D3 = 0x07U, DS3 = 0x08U, EB3 = 0x08U,
    E3 = 0x09U,
    F3 = 0x0AU, FS3 = 0x0BU, GB3 = 0x0BU,
    G3 = 0x0CU, GS3 = 0x0DU, AB3 = 0x0DU,
    A3 = 0x0EU, AS3 = 0x0FU, BB3 = 0x0FU,
    B3 = 0x10U,
    C4 = 0x11U, CS4 = 0x12U, DB4 = 0x12U,
    D4 = 0x13U, DS4 = 0x14U, EB4 = 0x14U,
    E4 = 0x15U,
    F4 = 0x16U, FS4 = 0x17U, GB4 = 0x17U,
    G4 = 0x18U, GS4 = 0x19U, AB4 = 0x19U,
    A4 = 0x1AU, AS4 = 0x1BU, BB4 = 0x1BU,
    B4 = 0x1CU,
    C5 = 0x1DU, CS5 = 0x1EU, DB5 = 0x1EU,
    D5 = 0x1FU, DS5 = 0x20U, EB5 = 0x20U,
    E5 = 0x21U,
    F5 = 0x22U, FS5 = 0x23U, GB5 = 0x23U,
    G5 = 0x24U, GS5 = 0x25U, AB5 = 0x25U,
    A5 = 0x26U, AS5 = 0x27U, BB5 = 0x27U,
    B5 = 0x28U,
    C6 = 0x29U, CS6 = 0x2AU, DB6 = 0x2AU,
    D6 = 0x2BU, DS6 = 0x2CU, EB6 = 0x2CU,
    E6 = 0x2DU,
    F6 = 0x2EU, FS6 = 0x2FU, GB6 = 0x2FU,
    G6 = 0x30U, GS6 = 0x31U, AB6 = 0x31U,
    A6 = 0x32U, AS6 = 0x33U, BB6 = 0x33U,
    B6 = 0x34U,
    C7 = 0x35U, CS7 = 0x36U, DB7 = 0x36U,
    D7 = 0x37U, DS7 = 0x38U, EB7 = 0x38U,
    E7 = 0x39U,
    F7 = 0x3AU, FS7 = 0x3BU, GB7 = 0x3BU,
    G7 = 0x3CU, GS7 = 0x3DU, AB7 = 0x3DU,
    A7 = 0x3EU, AS7 = 0x3FU, BB7 = 0x3FU,
    Note_End = 0x80U, CH0 = 0x80U, CH1 = 0x80U, CHN = 0x80U
};
typedef u8 Note;

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

extern const track_t g_yellow;
extern const track_t g_level0;
extern const track_t g_corneria;
extern const track_t g_jibjig;
extern const track_t* g_tracks[];

inline const track_t* track_get(Track const track)
{
    return g_tracks[track];
}

#endif /* TRACK_H */