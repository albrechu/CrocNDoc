/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <cnd/track.h>
#include <vectrex.h>

/////////////////////////////////////////////////////////////////////////
//	Defines and Global Data
//
#define DUR(note, duration) note, duration
#define SPV 17
#define SPL 12
#define SPN 10
#define V(a) DUR(a, SPV)
#define L(a) DUR(a, SPL)
#define N(a) DUR(a, SPN)
#define V2(a, b) CH0 | a, DUR(b, SPV)
#define L2(a, b) CH0 | a, DUR(b, SPL)
#define N2(a, b) CH0 | a, DUR(b, SPN)

// #define PAUSE 16

// Speeds
// #define SPQ 8
// #define SPE 7
#define SPH 11
#define SPQ 9
#define SPE 9
#define SPS 8

#define SPHF 10
#define SPQF 8
#define SPEF 8
#define SPSF 7
// One-Channel
#define H(a) DUR(a, SPHF)
#define Q(a) DUR(a, SPQF)
#define E(a) DUR(a, SPEF)
#define S(a) DUR(a, SPSF)
// Two-Channels
#define H2(a, b) CH0 | a, DUR(b, SPHF)
#define Q2(a, b) CH0 | a, DUR(b, SPQF)
#define E2(a, b) CH0 | a, DUR(b, SPEF)
#define S2(a, b) CH0 | a, DUR(b, SPSF)
// Three-Channels
#define H3(a, b, c) CH0 | a, CH1 | b, DUR(c, SPHF)
#define Q3(a, b, c) CH0 | a, CH1 | b, DUR(c, SPQF)
#define S3(a, b, c) CH0 | a, CH1 | b, DUR(c, SPSF)

// Two-Channels Fast
#define Q2F(a, b) CH0 | a, DUR(b, SPQF)
#define E2F(a, b) CH0 | a, DUR(b, SPEF)
#define E3F(a, b, c) CH0 | a, CH1 | b, DUR(c, SPEF)

#define CORNERIA_PAUSE2 16
#define CORNERIA_PAUSE3 63

const amplitudes_t g_amplitudes =
{
        .amplitudes = {0xCC, 0xCC, 0xBB, 0xBB, 0xAA, 0xAA, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

const frequencies_t g_frequencies =
    {
        .frequencies = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

const track_t g_yellow =
    {
        .amplitudes = &g_amplitudes,
        .frequencies = &g_frequencies,
        .notes =
            {
                // Measure 1 - Intro jumps
                E2F(BB4, F3),
                E2F(BB5, F3),
                E2F(BB4, F3),
                E2F(BB5, F3),
                E2F(BB4, F3),
                E2F(BB5, F3),
                E2F(BB4, F3),
                E2F(BB5, F3),
                // Measure 2 - Melody climbs
                Q2F(D5, G3),
                Q2F(F5, G3),
                Q2F(G5, G3),
                E2F(BB5, G3),
                E2F(BB5, G3),
                Q2F(D6, G3),
                Q2F(C6, G3),
                E2F(BB5, G3),
                E2F(AB5, G3),
                E2F(BB5, G3),
                // Measure 3 - More jumps
                E2F(BB4, F3),
                E2F(BB5, F3),
                E2F(BB4, F3),
                E2F(BB5, F3),
                // Measure 4 - Soaring high
                Q2F(G5, G3),
                E2F(BB5, G3),
                Q2F(C6, G3),
                E2F(EB6, G3),
                E2F(BB5, G3),
                Q2F(C6, G3),
                Q2F(D6, G3),
                E2F(BB5, G3),
                Q2F(G5, G3),
                Q2F(F5, G3),
                // Measure 5 - Downward
                Q2F(F5, F3),
                Q2F(D5, F3),
                Q2F(C5, F3),
                E2F(BB4, F3),
                Q2F(C5, F3),
                E2F(BB4, F3),
                E2F(BB4, F3),
                E2F(BB4, F3),
                E2F(BB4, F3),
                Q2F(D5, G3),
                Q2F(F5, G3),
                Q2F(G5, G3),
                E2F(BB5, G3),
                Q2F(C6, G3),
                E2F(BB5, G3),

                // Measure 6 - Near the end
                Q2F(D6, G3),
                Q2F(D6, G3),
                Q2F(C6, G3),
                E2F(BB5, G3),
                Q2F(G5, G3),
                Q2F(F5, G3),
                Q2F(G5, G3),
                E2F(BB5, G3),
                Q2F(C6, G3),
                E2(BB5, G3),

                // Measure 7 - Final
                Q2F(F5, F3),
                Q2F(G5, F3),
                E2F(BB5, F3),
                Q2F(C6, F3),
                Q2F(D6, F3),
                Q2F(C6, F3),
                E2F(BB5, F3),
                Q2F(G5, F3),
                Q2F(G5, F3),
                E2F(BB5, F3),

                // End
                E2F(BB5, F3) | Note_End,
            },
};

const track_t g_corneria =
    {
        .amplitudes = &g_amplitudes,
        .frequencies = &g_frequencies,
        .notes =
            {
                // 1.1
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 1.2
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(BB6), E(C7), E(A6), E3F(F6, G3, G4), E(A6),
                // 1.3
                E3F(G5, F3, F4), E(D6), E3F(G6, F3, F4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 1.4
                E3F(G5, F3, F4), E(D6), E3F(G6, F3, F4), E(BB5), E(C6), E(A5), E3F(F5, F3, F4), E(A5),
                // 2.1
                E3F(G5, EB3, EB4), E(D6), E3F(G6, EB3, EB4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 2.2
                E3F(G5, EB3, EB4), E(D6), E3F(G6, EB3, EB4), E(BB6), E(A6), E(F6), E3F(D6, F3, F4), E(BB5),
                // 2.3
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 2.4
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(BB5), E(A5), E(F5), E(D5), E(F5),
                // 3.1 First Repeat
                E3F(G3, G5, G6), E(G4), E2F(G5, G3), E(G4), E(G3), E3F(G4, D5, G5), E(G3), E(G4),
                // 3.2
                E3F(G3, D5, G5), E(G4), E(G3), E(G4), E3F(G3, D5, G5), E3F(G4, D5, A5), E(G3), E3F(G3, D5, BB5),
                // 3.3
                E3F(EB3, EB5, EB6), E(EB4), E2F(EB3, EB5), E3F(EB4, BB4, EB5), E(EB3), E3F(EB4, BB4, EB5), E(EB3), E(EB4),
                // 3.4
                E3F(F3, F5, F6), E(F4), E2F(F3, F5), E3F(F4, C5, F5), E(F3), E3F(F4, C5, F5), E3F(F3, F5, F6), E(F4),
                // 4.1
                E3F(G3, G5, G6), E(G4), E2F(G5, G3), E(G4), E(G3), E3F(G4, D5, G5), E(G3), E(G4),
                // 4.2
                E3F(G3, D5, G5), E(G4), E(G3), E(G4), E3F(G3, D5, G5), E3F(G4, D5, A5), E(G3), E3F(G3, D5, BB5),
                // 4.3
                E3F(EB3, EB5, EB6), E(EB4), E2F(EB3, EB5), E3F(EB4, BB4, EB5), E(EB3), E3F(EB4, BB4, EB5), E(EB3), E(EB4),
                // 4.4
                E3F(F3, F5, F6), E(F4), E2F(F3, F5), E3F(F4, C5, F5), E(F3), E3F(F4, C5, F5), E3F(F3, F5, F6), E(F4),
                // 5.1
                E3F(EB3, C6, EB6), E(EB4), E3F(EB3, BB5, D6), E(EB4), E3F(EB3, A5, C6), E(EB4), E3F(EB3, G5, BB5), E(EB4),
                // 5.2
                E3F(F3, F5, A5), S2(F4, BB5), S(A5), E2(F3, G5), E3F(F4, F5, A5), E(F3), E3F(F4, D5, F5), E3F(F3, EB5, G5), E3F(F4, F5, A5),
                // 5.3
                E3F(D3, A5, C6), E(D4), E3F(D3, G5, BB5), E3F(D4, F5, A5), E(D3), E3F(D4, F5, A5), E3F(D3, G5, BB5), E3F(D4, A5, C6),
                // 5.4
                E3F(G3, A5, C6), E(G4), E3F(G3, BB5, D6), E3F(G4, BB5, D6), E(G3), E(G4), E3F(G3, BB5, D6), E(F3),
                // 6.1
                E3F(EB3, BB5, EB6), E(EB4), E3F(EB3, G5, BB5), E3F(EB4, G5, BB5), E(EB3), E(EB4), E3F(EB3, BB5, EB6), E(EB4),
                // 6.2
                E3F(F3, C6, F6), E(F4), E3F(F3, A5, C6), E3F(F4, A5, C6), E(F3), E(F4), E3F(F3, C6, F6), E(F4),
                //6.3
                E3F(GB3, DB6, GB6), E(GB4), E3F(GB3, BB5, DB6), E3F(GB4, BB5, DB6), E(GB3), E(GB4), E3F(GB3, DB6, GB6), E(GB4),
                //6.4 1. Begin
                E3F(G3, G4, G6), E(D6), E(G6), E3F(AB3, AB4, AB6), E(EB6), E(AB6), E3F(A3, A4, A6), E(E6),
                //7.1 1. Last one
                E3F(A3, A4, A6), E3F(BB3, BB4, BB6), E(F6), E(BB6), S3(B3, B4, B6), S(C7), S(DB7), S(D7), Q2(G6, GB7),

                // 3.1 Second Repeat
                E3F(G3, G5, G6), E(G4), E2F(G5, G3), E(G4), E(G3), E3F(G4, D5, G5), E(G3), E(G4),
                // 3.2
                E3F(G3, D5, G5), E(G4), E(G3), E(G4), E3F(G3, D5, G5), E3F(G4, D5, A5), E(G3), E3F(G3, D5, BB5),
                // 3.3
                E3F(EB3, EB5, EB6), E(EB4), E2F(EB3, EB5), E3F(EB4, BB4, EB5), E(EB3), E3F(EB4, BB4, EB5), E(EB3), E(EB4),
                // 3.4
                E3F(F3, F5, F6),
                E(F4),
                E2F(F3, F5),
                E3F(F4, C5, F5),
                E(F3),
                E3F(F4, C5, F5),
                E3F(F3, F5, F6),
                E(F4),
                // 4.1
                E3F(G3, G5, G6),
                E(G4),
                E2F(G5, G3),
                E(G4),
                E(G3),
                E3F(G4, D5, G5),
                E(G3),
                E(G4),
                // 4.2
                E3F(G3, D5, G5),
                E(G4),
                E(G3),
                E(G4),
                E3F(G3, D5, G5),
                E3F(G4, D5, A5),
                E(G3),
                E3F(G3, D5, BB5),
                // 4.3
                E3F(EB3, EB5, EB6),
                E(EB4),
                E2F(EB3, EB5),
                E3F(EB4, BB4, EB5),
                E(EB3),
                E3F(EB4, BB4, EB5),
                E(EB3),
                E(EB4),
                // 4.4
                E3F(F3, F5, F6),
                E(F4),
                E2F(F3, F5),
                E3F(F4, C5, F5),
                E(F3),
                E3F(F4, C5, F5),
                E3F(F3, F5, F6),
                E(F4),
                // 5.1
                E3F(EB3, C6, EB6),
                E(EB4),
                E3F(EB3, BB5, D6),
                E(EB4),
                E3F(EB3, A5, C6),
                E(EB4),
                E3F(EB3, G5, BB5),
                E(EB4),
                // 5.2
                E3F(F3, F5, A5),
                S2(F4, BB5),
                S(A5),
                E2(F3, G5),
                E3F(F4, F5, A5),
                E(F3),
                E3F(F4, D5, F5),
                E3F(F3, EB5, G5),
                E3F(F4, F5, A5),
                // 5.3
                E3F(D3, A5, C6),
                E(D4),
                E3F(D3, G5, BB5),
                E3F(D4, F5, A5),
                E(D3),
                E3F(D4, F5, A5),
                E3F(D3, G5, BB5),
                E3F(D4, A5, C6),
                // 5.4
                E3F(G3, A5, C6),
                E(G4),
                E3F(G3, BB5, D6),
                E3F(G4, BB5, D6),
                E(G3),
                E(G4),
                E3F(G3, BB5, D6),
                E(F3),
                // 6.1
                E3F(EB3, BB5, EB6),
                E(EB4),
                E3F(EB3, G5, BB5),
                E3F(EB4, G5, BB5),
                E(EB3),
                E(EB4),
                E3F(EB3, BB5, EB6),
                E(EB4),
                // 6.2
                E3F(F3, C6, F6),
                E(F4),
                E3F(F3, A5, C6),
                E3F(F4, A5, C6),
                E(F3),
                E(F4),
                E3F(F3, C6, F6),
                E(F4),
                //6.3
                E3F(GB3, DB6, GB6),
                E(GB4),
                E3F(GB3, BB5, DB6),
                E3F(GB4, BB5, DB6),
                E(GB3),
                E(GB4),
                E3F(GB3, DB6, GB6),
                E(GB4),
                // 7.2 Second End
                E3F(G3, D4, G6),
                E(D6),
                E(C6),
                E(G5),
                E(D5),
                E(C5),
                E(G4),
                E(D4),
                
                // 7.3
                E3F(G3, D4, G5),
                E3F(G3, D4, G5),
                Q3(F3, F5, F6),
                Q3(F3, F5, F6),
                E3F(F3, C4, F5),
                E3F(F3, C4, F5),
                //7.4
                Q3(EB3, EB5, EB6),
                Q3(EB3, EB5, EB6),
                E3F(EB3, DB4, EB5),
                E3F(EB3, DB4, EB5),
                Q3(EB3, D5, D6),
                //7.5
                H3(F3, EB4, D5),
                Q2(EB5, EB6),
                Q2(F5, F6),
                //8.1
                E3F(G3, D4, G5),
                E3F(G3, D4, G5),
                Q3(F3, F5, F6),
                Q3(F3, F5, F6),
                E3F(F3, F4, F5),
                E3F(F3, F4, F5),
                //8.2
                Q3(EB3, EB5, EB6),
                Q3(EB3, EB5, EB6),
                E3F(EB3, BB3, EB5),
                E3F(EB3, BB3, EB5),
                Q3(EB3, D5, D6),
                //8.3
                H3(F3, EB4, D5),
                Q2(EB5, EB6),
                Q2(F5, F6),
                //8.4
                Q3(G3, D4, G5),
                E(BB5),
                E(A5),
                E(F5),
                E(D5),
                E(F5),
                //8.5
                E3F(G3, G4, G5),
                E(D6),
                E3F(G3, G4, G6),
                E(G5),
                E(G6),
                E(G5),
                E(D6),
                E(G6),
                //9.1
                E3F(G3, G4, G5),
                E(D6),
                E3F(G3, G4, G6),
                E(BB6),
                E(C7),
                E(A6),
                E3F(G3, G4, F6),
                E(A6),
                //9.2
                E3F(F3, F4, G5),
                E(D6),
                E3F(F3, F4, G6),
                E(G5),
                E(G6),
                E(G5),
                E(D6),
                E(G6),
                //9.3
                E3F(F3, F4, G5),
                E(D6),
                E3F(F3, F4, G6),
                E(BB5),
                E(C6),
                E(A5),
                E3F(F3, F4, F5),
                E(A5),
                //9.4
                E3F(EB3, EB4, G5),
                E(D6),
                E3F(EB3, EB4, G6),
                E(G5),
                E(G6),
                E(G5),
                E(D6),
                E(G6),
                //10.1
                E3F(EB3, EB4, G5),
                E(D6),
                E3F(EB3, EB4, G6),
                E(BB6),
                E(A6),
                E(F6),
                E3F(F3, F4, D6),
                E(BB5),
                //10.2
                E3F(G3, G4, G5),
                E(D6),
                E3F(G3, G4, G6),
                E(G5),
                E(G6),
                E(G5),
                E(D6),
                E(G6),
                //10.3
                E3F(G3, G4, G5),
                E(D6),
                E3F(G3, G4, G6),
                E(BB5),
                E(A5),
                E(F5),
                E(D5),
                E(F5),
                //10.4
                E3F(G3, D4, G5),
                E2F(G3, D4),
                Q2(D4, G4),
                //CORNERIA_PAUSE2,
                Q2(D4, G4),
                //CORNERIA_PAUSE2,
                //10.5
                Q2(D4, G4),
                //CORNERIA_PAUSE2,
                Q2(D4, G4),
                E2F(G3, D4),
                E2F(D4, G4),
                E2F(D4, G4),
                //11.1
                E2F(G3, D4),
                E2F(G3, D4),
                Q2(D4, G4),
                E2F(G3, D4),
                Q2(D4, G4),
                E2F(D4, G4),
                //11.2
                E3F(D4, G4, D6),
                Q(D6),
                Q(DB6),
                Q(C6),
                E(A5),
                //11.3
                E3F(A3, E4, A5),
                E2F(A3, E4),
                Q2(E4, A4),
                Q2(E4, A4),
                //11.4
                Q2(E4, A4),
                Q2(E4, A4),
                E2F(A3, E4),
                E2F(E4, A4),
                E2F(E4, A4),
                //11.5
                E2F(A3, E4),
                E2F(A3, E4),
                Q2(E4, A4),
                E2F(A3, E4),
                Q2(E4, A4),
                E2F(E4, A4),
                //12.1
                S3(A4, A5, A6),
                S2(A5, A6),
                E2F(A5, A6),
                //CORNERIA_PAUSE2,
                Q2(E5, E6),
                Q2(F5, F6),
                E2F(FS5, FS6),
                //Repeat from 10.4
                //10.4 Repeat
                E3F(G3, D4, G5), E2F(G3, D4), Q2(D4, G4), Q2(D4, G4),
                //10.5 Repeat
                Q2(D4, G4), Q2(D4, G4), E2F(G3, D4), E2F(D4, G4), E2F(D4, G4),
                //11.1 Repeat
                E2F(G3, D4), E2F(G3, D4), Q2(D4, G4), E2F(G3, D4), Q2(D4, G4), E2F(D4, G4),
                //11.2 Repeat
                E3F(D4, G4, D6), Q2(D6, D7), Q2(DB6, DB7), Q2(C6, C7), E2F(A5, A6),
                //11.3 Repeat
                E3F(A3, E4, A5), E2F(A3, E4), Q2(E4, A4), Q2(E4, A4),
                //11.4
                Q2(E4, A4), Q2(E4, A4), E2F(A3, E4), E2F(E4, A4), E2F(E4, A4),
                //11.5 Repeat
                E2F(A3, E4), E2F(A3, E4), Q2(E4, A4), E2F(A3, E4), Q2(E4, A4), E2F(E4, A4),
                //12.1 Repeat
                S3(A4, A5, A6), S2(A5, A6), E2F(A5, A6), Q2(E5, E6), Q2(F5, F6), E2F(FS5, FS6),
                //Begin From Start (1.1) But Jump to 7.2 instead of 6.4
                // 1.1
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 1.2 
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(BB6), E(C7), E(A6), E3F(F6, G3, G4), E(A6),
                // 1.3
                E3F(G5, F3, F4), E(D6), E3F(G6, F3, F4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 1.4
                E3F(G5, F3, F4), E(D6), E3F(G6, F3, F4), E(BB5), E(C6), E(A5), E3F(F5, F3, F4), E(A5),
                // 2.1
                E3F(G5, EB3, EB4), E(D6), E3F(G6, EB3, EB4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 2.2
                E3F(G5, EB3, EB4), E(D6), E3F(G6, EB3, EB4), E(BB6), E(A6), E(F6), E3F(D6, F3, F4), E(BB5),
                // 2.3
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(G5), E(G6), E(G5), E(C6), E(G6),
                // 2.4
                E3F(G5, G3, G4), E(D6), E3F(G6, G3, G4), E(BB5), E(A5), E(F5), E(D5), E(F5),
                // 3.1 First Repeat
                E3F(G3, G5, G6), E(G4), E2F(G5, G3), E(G4), E(G3), E3F(G4, D5, G5), E(G3), E(G4),
                // 3.2
                E3F(G3, D5, G5), E(G4), E(G3), E(G4), E3F(G3, D5, G5), E3F(G4, D5, A5), E(G3), E3F(G3, D5, BB5),
                // 3.3
                E3F(EB3, EB5, EB6), E(EB4), E2F(EB3, EB5), E3F(EB4, BB4, EB5), E(EB3), E3F(EB4, BB4, EB5), E(EB3), E(EB4),
                // 3.4
                E3F(F3, F5, F6), E(F4), E2F(F3, F5), E3F(F4, C5, F5), E(F3), E3F(F4, C5, F5), E3F(F3, F5, F6), E(F4),
                // 4.1
                E3F(G3, G5, G6), E(G4), E2F(G5, G3), E(G4), E(G3), E3F(G4, D5, G5), E(G3), E(G4),
                // 4.2
                E3F(G3, D5, G5), E(G4), E(G3), E(G4), E3F(G3, D5, G5), E3F(G4, D5, A5), E(G3), E3F(G3, D5, BB5),
                // 4.3
                E3F(EB3, EB5, EB6), E(EB4), E2F(EB3, EB5), E3F(EB4, BB4, EB5), E(EB3), E3F(EB4, BB4, EB5), E(EB3), E(EB4),
                // 4.4
                E3F(F3, F5, F6), E(F4), E2F(F3, F5), E3F(F4, C5, F5), E(F3), E3F(F4, C5, F5), E3F(F3, F5, F6), E(F4),
                // 5.1
                E3F(EB3, C6, EB6), E(EB4), E3F(EB3, BB5, D6), E(EB4), E3F(EB3, A5, C6), E(EB4), E3F(EB3, G5, BB5), E(EB4),
                // 5.2
                E3F(F3, F5, A5), S2(F4, BB5), S(A5), E2(F3, G5), E3F(F4, F5, A5), E(F3), E3F(F4, D5, F5), E3F(F3, EB5, G5), E3F(F4, F5, A5),
                // 5.3
                E3F(D3, A5, C6), E(D4), E3F(D3, G5, BB5), E3F(D4, F5, A5), E(D3), E3F(D4, F5, A5), E3F(D3, G5, BB5), E3F(D4, A5, C6),
                // 5.4
                E3F(G3, A5, C6), E(G4), E3F(G3, BB5, D6), E3F(G4, BB5, D6), E(G3), E(G4), E3F(G3, BB5, D6), E(F3),
                //6.1
                E3F(EB3, BB5, EB6), E(EB4), E3F(EB3, G5, BB5), E3F(EB4, G5, BB5), E(EB3), E(EB4), E3F(EB3, BB5, EB6), E(EB4),
                //6.2
                E3F(F3, C6, F6), E(F4), E3F(F3, A5, C6), E3F(F4, A5, C6), E(F3), E(F4), E3F(F3, C6, F6), E(F4),
                //6.3
                E3F(GB3, DB6, GB6), E(GB4), E3F(GB3, BB5, DB6), E3F(GB4, BB5, DB6), E(GB3), E(GB4), E3F(GB3, DB6, GB6), E(GB4),
                //7.2
                E3F(G3, D4, G6), E(D6), E(C6), E(G5), E(D5), E(C5), E(G4), E(D4),
                // 7.3
                E3F(G3, D4, G5), E3F(G3, D4, G5), Q3(F3, F5, F6), Q3(F3, F5, F6), E3F(F3, C4, F5), E3F(F3, C4, F5),
                //7.4
                Q3(EB3, EB5, EB6), Q3(EB3, EB5, EB6), E3F(EB3, DB4, EB5), E3F(EB3, DB4, EB5), Q3(EB3, D5, D6),
                //7.5
                H3(F3, EB4, D5), Q2(EB5, EB6), Q2(F5, F6),
                //8.1
                E3F(G3, D4, G5), E3F(G3, D4, G5), Q3(F3, F5, F6), Q3(F3, F5, F6), E3F(F3, F4, F5), E3F(F3, F4, F5),
                //8.2
                Q3(EB3, EB5, EB6), Q3(EB3, EB5, EB6), E3F(EB3, BB3, EB5), E3F(EB3, BB3, EB5), Q3(EB3, D5, D6),
                //8.3
                H3(F3, EB4, D5), Q2(EB5, EB6), Q2(F5, F6),
                //8.4
                Q3(G3, D4, G5), E(BB5), E(A5), E(F5), E(D5), E(F5),
                //8.5
                E3F(G3, G4, G5), E(D6), E3F(G3, G4, G6), E(G5), E(G6), E(G5), E(D6), E(G6),
                //9.1
                E3F(G3, G4, G5), E(D6), E3F(G3, G4, G6), E(BB6), E(C7), E(A6), E3F(G3, G4, F6), E(A6),
                //9.2
                E3F(F3, F4, G5), E(D6), E3F(F3, F4, G6), E(G5), E(G6), E(G5), E(D6), E(G6),
                //9.3
                E3F(F3, F4, G5), E(D6), E3F(F3, F4, G6), E(BB5), E(C6), E(A5), E3F(F3, F4, F5), E(A5),
                //9.4
                E3F(EB3, EB4, G5), E(D6), E3F(EB3, EB4, G6), E(G5), E(G6), E(G5), E(D6), E(G6),
                //10.1
                E3F(EB3, EB4, G5), E(D6), E3F(EB3, EB4, G6), E(BB6), E(A6), E(F6), E3F(F3, F4, D6), E(BB5),
                //10.2
                E3F(G3, G4, G5), E(D6), E3F(G3, G4, G6), E(G5), E(G6), E(G5), E(D6), E(G6),
                //10.3
                E3F(G3, G4, G5), E(D6), E3F(G3, G4, G6), E(BB5), E(A5), E(F5), E(D5), E(F5),
                //10.4
                E3F(G3, D4, G5), E2F(G3, D4), Q2(D4, G4), Q2(D4, G4),
                //10.5
                Q2(D4, G4), Q2(D4, G4), E2F(G3, D4), E2F(D4, G4), E2F(D4, G4),
                //11.1
                E2F(G3, D4), E2F(G3, D4), Q2(D4, G4), E2F(G3, D4), Q2(D4, G4), E2F(D4, G4),
                //11.2
                E3F(D4, G4, D6), Q(D6), Q(DB6), Q(C6), E(A5),
                ////11.3
                E3F(A3, E4, A5), E2F(A3, E4), Q2(E4, A4), Q2(E4, A4),
                //11.4
                Q2(E4, A4), Q2(E4, A4), E2F(A3, E4), E2F(E4, A4), E2F(E4, A4),
                //11.5
                E2F(A3, E4), E2F(A3, E4), Q2(E4, A4), E2F(A3, E4), Q2(E4, A4), E2F(E4, A4),
                //12.1
                E3F(A4, A5, A6), E2F(A5, A6), E2F(A5, A6),
                //CORNERIA_PAUSE2,
                Q2(E5, E6), Q2(F5, F6), E2F(FS5, FS6),
                //12.2
                Q3(G4, G5, G6), E(EB5), E(A5), E(F5), E(D5), E(F5), E(G5),
                //12.3
                H3(G4, G5, D6),
                G4, CORNERIA_PAUSE3
                | Note_End,
            },
};

#undef SPHF
#undef SPQF
#undef SPEF
#undef SPSF

#define SPPQ 13
#define SPPE 11
#define SPGF 12
#define SPHF 13
#define SPQF 11
#define SPEF 7
#define SPSF 5
#define G2F(a, b) CH0 | a, DUR(b, U8(SPGF))
#define QP(a) DUR(a, U8(SPPQ))
#define EP(a) DUR(a, U8(SPPE))
#define Q2P(a, b) CH0 | a, DUR(b, U8(SPPQ))
#define E2P(a, b) CH0 | a, DUR(b, U8(SPPE))

const track_t g_champion =
{
    .amplitudes = &g_amplitudes,
    .frequencies = &g_frequencies,
    .notes =
    {
        // 1
        Q2P(E5,  EB3),
        Q2P(E5,  EB3),
        Q2P(E5,  EB3),
        Q2(E5,  EB3),
        Q2(D5,  ES3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5,  BS3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BS5, BS3),
        // 2
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2P(E5,  EB3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(CS6, BS3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5,  B3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BS5, BS3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5, B3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(D5,  D3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5, B3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BS5, BS3),
        // 3
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5, B3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(E5, AS3),
        Q(F5),// Doppelt?
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5, B3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BS5, BS3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BB5, B3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(BS5, BS3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(C6, BS3),
        E2(E5,  EB3),
        E2(E5,  EB3),
        Q2(CS6, CS4),
        H2(EB6, E4),
        H2(E5, EB3),
        // 4.1
        Q2(ES6, BS4),
        E(G4),
        E(B4),
        E2(EB6, GB4),
        E(BB4),
        S2(AS5, F4),
        S(BS5),
        S2(C6, AS4),
        S(DB6),
        
        // 4.2 First Iteration
        E2(EB6, EB4),
        E(B4),
        E(EB4),
        E2(BB5, B4),
        E(EB4),
        E(B4),
        E2(EB6, EB4),
        E2(D6, B4),
        E2(DB6, EB4),
        E(B4),
        E2(C6, EB4),
        E(B4),
        E2(CB6, EB4),
        E(B4),
        E2(BB5, EB4),
        E(B4),
        
        E2P(BS5, ES4), // @TOOD
        E(BS4),
        E(EB4),
        E(B4),

        E2P(ES6, EB4), // @TOOD
        E(B4),
        E(EB4),
        E(B4),

        // 5.
        E2(AB5, ES4),
        E2(AB5, BS4),
        E3F(AS5, B5, EB4),
        E(B4),

        E2(AB5, AS4),
        E2(AB5, B4),
        E2(DB6, G4),
        E(A4),


        E2(EB6, EB4),
        E(B4),
        E(EB4),
        E2(BB5, B4),
        E(EB4),
        E(B4),
        E2(EB6, EB4),
        E2(D6, B4),
        E2(DB6, EB4),
        E(B4),
        E2(C6, EB4),
        E(B4),
        E2(CB6, EB4),
        E(B4),
        E2(BB5, EB4),
        E2(C6, B4),

        H2(ES6, ES4), // @TOOD
        E(BS4),
        E(EB4),
        E(B4),

        H2(A6, EB4), // @TOOD
        E(B4),
        E(EB4),
        E(B4),

        H2(ES6, ES4), // @TOOD
        E(BS4),
        E(EB4),
        E(B4),

        H2(AS6, EB4), // @TOOD
        E(B4),
        E(EB4),
        E(B4),

        E2(EB6, EB4),
        E2(EB6, GB4),
        H(EB4),
        E2(EB6, GB4),
        H(EB4),
        E2(EB6, D5),
        H(D5),
        E2(EB6, C5),

        E(EB4),
        E2(EB6, GB4),
        H(EB4),
        E2(EB6, GB4),

        E2(ES6, EB4),
        E(D5),
        E2(EB6, DB5),
        E(C5),

        E2(EB6, EB4),
        E2(EB6, GB4),
        H(EB4),
        E2(EB6, GB4),

        E(EB4),
        E2(EB6, D5),
        E(DB5),
        E2(EB6, C5), 

        // 6.
        E(EB4),
        E2(EB6, GB4),
        E(EB4),
        E2(EB6, G4),

        E2(FS6, EB4),
        E(D5),
        E2(F6, DB5),
        E(C5),

        H2(EB6, B4),
        H2(E5, EB4),
        H2(CS6, ES4),
        H2(CS5, EB4),
        
        G2F(E5, EB4),
        E(FS4),
        E(EB4),
        E(F4),

        E(EB4),
        E(F4),
        E(EB4),
        E(F4),

        G2F(E5, EB4),
        E(FS4),
        E(EB4),
        E(F4),

        E(EB4),
        E(F4),
        E(EB4),
        E(F4),

        G2F(F5, EB4),
        E(FS4),
        E(EB4),
        E(F4),

        E(EB4),
        E(F4),
        E(EB4),
        E(F4),

        G2F(GB5, EB4),
        E(FS4),
        E(EB4),
        E(F4),

        E(EB4),
        E(F4),
        E(EB4),
        E(F4)

        
        | Note_End,
    }
};


const track_t g_level0 =
    {
        .amplitudes = &g_amplitudes,
        .frequencies = &g_frequencies,
        .notes =
            {
                /*
                DURQ(Note_D4),
                DURQ(Note_E4),
                DURQ(Note_F4),
                DURQ(Note_A4),
                DURQ(Note_D5),
                DURQ(Note_E5),
                DURQ(Note_F5),
                DURQ(Note_A5),
                DURQ(Note_A5),
                DURQ(Note_B5),
                DURQ(Note_C5),
                DURQ(Note_E6),
                DURQ(Note_D6),
                DURQ(Note_E6),
                DURQ(Note_F6),
                DURQ(Note_A6),

                DURQ(Note_F4),
                DURQ(Note_G4),
                DURQ(Note_A4),
                DURQ(Note_C4),
                DURQ(Note_F5),
                DURQ(Note_G5),
                DURQ(Note_A5),
                DURQ(Note_D6),
                DURQ(Note_C5),
                DURQ(Note_D6),
                DURQ(Note_G6),
                DURQ(Note_A6),
                DURQ(Note_F6),
                DURQ(Note_A6),
                DURQ(Note_C6),
                DURQ(Note_D7),

                DURQ(Note_A4),
                DURQ(Note_B4),
                DURQ(Note_C4),
                DURQ(Note_E5),
                DURQ(Note_A5),
                DURQ(Note_B5),
                DURQ(Note_C5),
                DURQ(Note_E6),
                DURQ(Note_A5),
                DURQ(Note_B5),
                DURQ(Note_E6),
                DURQ(Note_A6),
                DURQ(Note_B6),
                DURQ(Note_A6),
                DURQ(Note_C6),
                DURQ(Note_D7),
        */
                E2(C4, C3),
                E2(D5, D4),
                E2(E5, E4),
                E2(G5, G4),
                E2(A5, A4),
                E2(C5, C4),
                E2(D6, D5),
                E2(C5, C4),
                E2(A5, A4),
                E2(G5, G4),
                E2(E5, E4),
                E2(D5, D4),
                E2(C4, C3),
                E2(G4, G3),

                E2(D4, D4),
                E2(E4, E4),
                E2(F4, F4),
                E2(A4, A4),
                E2(D5, D5),
                E2(E5, E5),
                E2(F5, F5),
                E2(A5, A5),
                E2(A5, A5),
                E2(B5, B5),
                E2(C5, C5),
                E2(E6, E6),
                E2(D6, D6),
                E2(E6, E6),
                E2(F6, F6),
                E2(A6, A6),

                E2(F4, F3),
                E2(G4, G3),
                E2(A4, A3),
                E2(C4, C3),
                E2(F5, F4),
                E2(G5, G4),
                E2(A5, A4),
                E2(D6, D5),
                E2(C5, C4),
                E2(D6, D5),
                E2(G6, G5),
                E2(A6, A5),
                E2(F6, F5),
                E2(A6, A5),
                E2(C6, C5),
                E2(D7, D6),

                E2(A4, A5),
                E2(B4, B5),
                E2(C4, C5),
                E2(E5, E6),
                E2(A5, A6),
                E2(B5, B6),
                E2(C5, C6),
                E2(E6, E7),
                E2(A5, A6),
                E2(B5, B6),
                E2(E6, E7),
                E2(A6, A7),
                E2(B6, AS7),
                E2(A6, A7),
                E2(C6, C7),
                E2(D7, D7),

                E2(C4, C3),
                E2(D5, D4),
                E2(E5, E4),
                E2(G5, G4),
                E2(A5, A4),
                E2(C5, C4),
                E2(D6, D5),
                E2(C5, C4),
                E2(A5, A4),
                E2(G5, G4),
                E2(E5, E4),
                E2(D5, D4),
                E2(C4, C3),
                E2(G4, G3),
                Q2(A5, A4),
                Q2(G5, G4),
                Q2(F5, F4),
                E2(F5, F4),
                Q2(G5, G4),

                Q2(AS5, AS6),
                Q2(GS5, GS6),
                Q2(FS5, FS6),
                E2(FS5, FS6),
                E2(GS5, GS6)

                    | Note_End,

                /*
                DURN(Note_A4),
                DURN(Note_C4),
                DURN(Note_E4),
                DURN(Note_C4),
                DURN(Note_G4),
                DURN(Note_G4),
                DURN(Note_E4),
                DURN(Note_F4),

                DURN(Note_A4),
                DURN(Note_C4),
                DURN(Note_E4),
                DURN(Note_C4),
                DURN(Note_G4),
                DURN(Note_G4),
                DURN(Note_E4),
                DURN(Note_F4),

                DURN(Note_A4),
                DURN(Note_C4),
                DURN(Note_E4),
                DURN(Note_C4),
                DURN(Note_G4),
                DURN(Note_G4),
                DURN(Note_E4),
                DURN(Note_F4),

                DURN(Note_A4),
                DURN(Note_C4),
                DURN(Note_E4),
                DURN(Note_C4),
                DURN(Note_G4),
                DURN(Note_G4),
                DURN(Note_E4),
                DURN(Note_F4),
                DURN(Note_A4),
                DURN(Note_C4),
                DURN(Note_E4),
                DURN(Note_C4),
                DURN(Note_G4),
                DURN(Note_G4),
                DURN(Note_E4),
                DURN(Note_F4),

                DURN(Note_A4),
                DURN(Note_C4),
                DURN(Note_E4),
                DURN(Note_C4),
                DURN(Note_G4),
                DURN(Note_G4),
                DURN(Note_E4),
                DURN(Note_F4),
        */

                // N(A4),
                // N(C4),
                // N(E4),
                // N(C4),
                // N(G4),
                // N(G4),
                // N(E4),
                // N(F4),

                // N(A4),
                // N(C4),
                // N(E4),
                // N(C4),
                // N(G4),
                // N(G4),

                // Q(C4),
                // Q(D4),
                // Q(E4),
                // Q(F4),

                // L(G4),
                // N(F4),
                // L(E4),

                // Q(C4),
                // Q(D4),
                // Q(E4),
                // Q(F4),

                // L(G4),
                // N(F4),
                // L(E4),
                // N(F4),
                // N(G4),
                // L(F4),

                // Q(C4),
                // Q(D4),
                // V(B4),

                // Q(C4),
                // Q(D4),
                // Q(E4),
                // Q(F4),

                // L(G4),
                // N(F4),
                // V(E4),
                // Q(E4),
                // Q(F4),

                // L(G4),
                // N(F4),
                // L(E4),

                // Q(B5),
                // Q(C5),
                // L(B5),

                // Q(F4),
                // Q(G4),
                // L(F4),

                // Q(C4),
                // Q(D4),
                // L(B4),
                // L(A7)
                // | Note_End,

            },
};

#define CROCODILE_CACOPHONY_32ND       4
#define CROCODILE_CACOPHONY_SIXTEENTH  6
#define CROCODILE_CACOPHONY_EIGHTH     8
#define CROCODILE_CACOPHONY_QUARTER    12
#define CROCODILE_CACOPHONY_DOT        18
#define CROCODILE_CACOPHONY_PAUSE      24
#define CROCODILE_CACOPHONY_PAUSE_LONG 36

#define CC1(x, speed) x, speed
#define CCT1(x) CC1(x, CROCODILE_CACOPHONY_32ND)
#define CCT2(x, y) x | CHN, CCT1(y)
#define CCS1(x) CC1(x, CROCODILE_CACOPHONY_SIXTEENTH)
#define CCS2(x, y) x | CHN, CCS1(y)
#define CCS3(x, y, z) x | CHN, CCS2(y, z)
#define CCE1(x) CC1(x, CROCODILE_CACOPHONY_EIGHTH)
#define CCE2(x, y) x | CHN, CCE1(y)
#define CCE3(x, y, z) x | CHN, CCE2(y, z)
#define CCQ1(x) CC1(x, CROCODILE_CACOPHONY_QUARTER)
#define CCQ2(x, y) x | CHN, CCQ1(y)
#define CCQ3(x, y, z) x | CHN, CCQ2(y, z)
#define CCH1(x) CC1(x, CROCODILE_CACOPHONY_PAUSE)
#define CCH2(x, y) x | CHN, CCH1(y)
#define CCH3(x, y, z) x | CHN, CCH2(y, z)
#define CCPD1(x) CC1(x, CROCODILE_CACOPHONY_DOT)
#define CCP1(x) CC1(x, CROCODILE_CACOPHONY_PAUSE_LONG)
#define CCP2(x, y) x | CHN, CCP1(y)
#define CCP3(x, y, z) x | CHN, CCP2(y, z)

const track_t g_crocodileCacophony =
    {
        .amplitudes = &g_amplitudes,
        .frequencies = &g_frequencies,
        .notes =
            {
                CCS1(D4),
                CCS1(D4),
                CCQ1(D4),
                CCQ1(D4),
                CCS1(D4),
                CCS1(D4),
                CCQ3(D4, A5, D6),
                CCQ3(D4, AS5, DS6),
                CCQ3(D4, B6, DS6),
                
                CCS1(D4),
                CCS1(D4),
                CCQ1(D4),
                CCQ1(D4),
                CCS1(D4),
                CCS1(D4),
                CCQ3(D4, C6, E6),
                CCQ3(D4, CS6, F6),
                CCS3(D4, D6, F6),
                CCS2(D6, FS6),

                CCS1(D4),
                CCS1(D4),
                CCQ1(D4),
                CCQ1(D4),
                CCS1(D4),
                CCS1(D4),

                CCQ3(D4, B5, E6),
                CCQ3(D4, C6, G6),
                CCQ3(D4, E6, GS6),

                CCS1(D4),
                CCS1(D4),
                CCQ1(D4),
                CCQ1(D4),
                CCS1(D4),
                CCS1(D4),
                CCQ3(D4, D6, FS6),
                CCQ3(D4, DS6, G6),
                CCS3(D4, E6, G6),
                CCS2(E6, GS6),

                CCS3(G3, G6, AS6),
                CCS2(G6, AS6),
                CCS2(G6, AS6),
                CCS1(G3),
                CCS3(G3, G6, AS6),
                CCS3(G4, G6, AS6),
                
                CCS2(G6, AS6),
                CCS2(G6, AS6),
                CCS3(G3, G6, AS6),
                CCS1(G3),
                CCS2(G6, AS6),
                CCQ3(G4, G6, AS6),
                CCS3(G3, G6, AS6),
                CCS2(G6, AS6),
                CCS3(G3, G6, AS6), 
                CCS2(G6, AS6), 
                CCS2(G6, AS6), 
                CCS1(G3),
                CCQ3(G4, G6, AS6),
                CCS3(G3, G6, AS6),
                CCS2(G6, AS6),
                CCS2(G6, AS6),
                CCS3(G3, G6, AS6),
                CCS1(G3), 
                CCS2(G6, AS6), 
                CCQ3(G4, G6, AS6),
                CCS3(G3, G6, AS6), 
                CCS2(G6, AS6),

                CCS3(G3, FS6, A6), 
                CCS2(FS6, A6), 
                CCS2(FS6, A6), 
                CCS1(G3), 
                CCQ3(G3, FS6, A6), 
                CCS3(G4, FS6, A6), 
                CCS2(FS6, A6), 
                CCS2(FS6, A6), 
                CCS3(G3, FS6, A6), 
                CCS1(G3), 
                CCS2(FS6, A6), 
                CCQ3(G4, FS6, A6), 
                CCS3(G3, FS6, A6), 
                CCS2(FS6, A6), 
                CCS3(G3, FS6, A6), 
                CCS2(FS6, A6), 
                CCS2(FS6, A6), 
                CCS1(G3), 
                CCQ3(G4, FS6, A6), 
                CCS3(G3, FS6, A6), 
                CCS2(FS6, A6), 
                CCS2(FS6, A6), 
                CCS3(G3, FS6, A6), 
                CCS1(G3), 
                CCS2(FS6, A6), 
                CCQ3(G4, FS6, A6), 
                CCS3(G3, FS6, A6), 
                CCS2(FS6, A6), 

                CCS3(G3, DS6, AS6), 
                CCS1(AS6), 
                CCS2(D6, AS6), 
                CCS1(G3), 
                CCQ3(G3, AS5, AS6), 
                CCS3(G4, DS6, AS6), 
                CCS1(AS6), 
                CCS2(D6, AS6), 
                CCS2(G3, AS6), 
                CCS2(G3, AS5), 
                CCS1(AS6), 
                CCQ3(G4, DS6, AS6), 
                CCS3(G3, D6, AS6), 
                CCS1(AS6), 
                CCS3(G3, DS6, AS6), 
                CCS1(AS6), 
                CCS2(D6, AS6), 
                CCS1(G3), 
                CCQ3(G4, AS5, AS6), 
                CCS3(G3, DS6, AS6), 
                CCS1(AS6), 
                CCS2(D6, AS6), 
                CCS2(G3, AS6), 
                CCS2(G3, AS5), 
                CCS1(AS6), 
                CCQ3(G4, F6, AS6), 
                CCS2(G3, AS6), 
                CCS2(D6, AS6), 

                CCS3(G3, DS6, A6), 
                CCS1(A6), 
                CCS2(D6, A6), 
                CCS1(G3), 
                CCQ3(G3, A5, A6), 
                CCS3(G4, DS6, A6), 
                CCS1(A6), 
                CCS2(D6, A6), 
                CCS2(G3, A6), 
                CCS2(G3, A5), 
                CCS1(A6), 
                CCQ3(G4, DS6, A6), 
                CCS3(G3, D6, A6), 
                CCS1(A6), 
                CCS3(G3, DS6, A6), 
                CCS1(A6), 
                CCS2(D6, A6), 
                CCS1(G3), 
                CCQ3(G4, A5, A6), 
                CCS3(G3, DS6, A6), 
                CCS1(A6), 
                CCS2(D6, A6), 
                CCS2(G3, A6), 
                CCS2(G3, A5), 
                CCS1(A6), 
                CCQ3(G4, DS6, A6), 
                CCS3(G3, D6, A6), 
                CCS1(A6), 

                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCQ1(B3), 
                CCH1(G3), 
                CCS1(G3),
                CCS1(G3),
                CCH1(B3),
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCQ1(B3),
                CCH1(G3),
                CCS1(G3),
                CCS1(G3),
                CCH1(FS3),
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCQ1(B3), 
                CCH1(G3),
                CCS1(G3),
                CCS1(G3),
                CCH1(B3), 
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCS1(G3),
                CCQ1(B3), 
                CCH1(G3), 
                CCS1(G3),
                CCS1(G3),
                CCH1(A3), 

                CCQ3(D3, D5, A5),
                CCS2(D5, A5),
                CCE1(D3),
                CCQ1(F3),
                CCQ3(G3, D5, A5), 
                CCP3(A3, D5, A5),
                CCQ3(F3, D5, A5), 
                CCQ3(G3, CS5, A5),
                CCT2(CS5, A5), 
                CCE1(G3), 
                CCQ1(F3), 
                CCQ3(G3, CS5, A5), 
                CCS3(A3, D5, A5),
                CCS2(D5, A5), 
                CCQ3(F3, D5, A5), 
                CCH3(D3, D5, A5), 

                CCQ3(D3, D5, A5), 
                CCS2(D5, A5),
                CCE1(D3),
                CCQ1(F3), 
                CCQ3(G3, D5, A5),
                CCP3(A3, D5, A5),
                CCQ3(F3, D5, A5), 
                CCQ3(G3, CS5, A5), 
                CCT2(CS5, A5),
                CCE1(G3),
                CCQ1(F3), 
                CCQ3(E3, CS5, A5), 
                CCQ3(D3, D5, A5), 

                CCQ1(G3), 
                CCQ2(D4, D6), 
                CCQ1(FS4), 
                CCQ2(D4, D6), 
                CCP2(G3, D6),
                CCS1(DS4), 
                CCQ1(FS4), 
                CCQ1(FS4), 

                CCS2(DS4, C6),
                CCS1(AS5), 
                CCQ2(G3, A5), 
                CCQ1(CS4), 
                CCQ2(FS4, AS5), 
                CCQ2(CS4, A5), 
                CCS2(G3, AS5), 
                CCS1(D4),
                CCQ2(FS4, A5),
                CCH2(FS4, G5),

                CCH1(D6),
                CCQ1(D6),
                CCP1(D6), 
                CCS1(C6), 
                CCS1(AS5), 
                CCQ1(A5), 
                CCS1(A5),
                CCQ1(AS5),
                CCH1(A5),
                CCQ1(C6), 
                CCQ1(AS5),
                CCQ1(A5),
                CCS1(A5), 
                CCQ1(AS5),
                CCPD1(C6),
                CCS1(A5), 
                CCS1(AS5), 
                CCH1(D6) | Note_End,
                
            },
};

const track_t g_jibjig =
    {
        .amplitudes = &g_amplitudes,
        .frequencies = &g_frequencies,
        .notes =
            {
                S(A5), S(B5), Q(C5), E(A5), Q(C5), E(A5), E(D5), E(C5), E(B5), Q(C5), E(A5), E(D5), E(C5), E(B5), Q(C5), E(A5),
                E(A5), E(B5), E(C5), E(B5), S(A5), S(B5), Q(C5), E(A5), Q(C5), E(A5), E(D5), E(C5), E(B5), Q(C5), E(A5), E(F5), E(E5), E(D5), Q(E5), S(C5),

                E(F5), E(G5), E(A6), Q(G5), Q2(C5, D5), Q(F5), Q2(A6, A6), Q(D6), Q(G6), Q(F6), Q2(G6, E6), Q(G6) | Note_End,

                // Measure 1
                // E2(GS4, E3), E2(GS4, E3), E2(DS5, GS3), E2(CS5, GS3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, GS3),
                // E2(E4, E3), E2(E4, E3), E2(B4, GS3), E2(GS4, GS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, GS3),

                // // Measure 2
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(E5, B2),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),
                // E2(E4, E3), E2(E4, E3), E2(B4, E3),

                // // Measure 3
                // E2(GS4, E3), E2(GS4, E3), E2(DS5, GS3), E2(CS5, GS3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, GS3),
                // E2(E4, E3), E2(E4, E3), E2(B4, GS3), E2(GS4, GS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, GS3),

                // // Measure 4
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(B4, E3),
                // E2(GS4, E3), E2(GS4, E3), E2(E4, E3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),

                // // Measure 5
                // E2(GS4, E3), E2(GS4, E3), E2(B4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, DS3),
                // E2(B4, E3), E2(B4, E3), E2(E4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, DS3),

                // // Measure 6
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(E5, B2),
                // E2(B4, GS2), E2(B4, GS2), E2(E4, GS2),
                // E2(GS4, GS2), E2(GS4, GS2), E2(B4, GS2),

                // // Measure 7
                // E2(GS4, E3), E2(GS4, E3), E2(B4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, DS3),
                // E2(B4, E3), E2(B4, E3), E2(E4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, DS3),

                // // Measure 8
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(B4, E3),
                // E2(GS4, E3), E2(GS4, E3), E2(E4, E3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),

                // // Measure 9
                // E2(B4, GS3), E2(B4, GS3), E2(DS5, B3), E2(CS5, B3),
                // E2(FS4, GS3), E2(FS4, GS3), E2(B4, B3),
                // E2(DS4, GS3), E2(DS4, GS3), E2(GS4, B3), E2(B4, B3),
                // E2(CS5, GS3), E2(CS5, GS3), E2(E5, B3),

                // // Measure 10
                // E2(DS5, E3), E2(DS5, E3), E2(B4, E3),
                // E2(CS5, E3), E2(CS5, E3), E2(DS5, E3),
                // E2(B4, GS2), E2(B4, GS2), E2(GS4, GS2),
                // E2(E4, GS2), E2(E4, GS2), E2(B4, GS2),

                // // Measure 11
                // E2(B4, GS3), E2(B4, GS3), E2(DS5, B3), E2(CS5, B3),
                // E2(FS4, GS3), E2(FS4, GS3), E2(B4, B3),
                // E2(DS4, GS3), E2(DS4, GS3), E2(GS4, B3), E2(B4, B3),
                // E2(CS5, GS3), E2(CS5, GS3), E2(E5, B3),

                // // Measure 12
                // E2(DS5, E3), E2(DS5, E3), E2(B4, E3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, E3),
                // E2(B4, GS2), E2(B4, GS2), E2(GS4, GS2),
                // E2(E4, GS2), E2(E4, GS2), E2(B4, GS2),

                // // Measure 13
                // E2(GS4, E3), E2(GS4, E3), E2(B4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, DS3),
                // E2(B4, E3), E2(B4, E3), E2(E4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, DS3),

                // // Measure 14
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(E5, B2),
                // E2(B4, GS2), E2(B4, GS2), E2(E4, GS2),
                // E2(GS4, GS2), E2(GS4, GS2), E2(B4, GS2),

                // // Measure 15
                // E2(GS4, E3), E2(GS4, E3), E2(B4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, DS3),
                // E2(B4, E3), E2(B4, E3), E2(E4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, DS3),

                // // Measure 16
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(B4, E3),
                // E2(GS4, E3), E2(GS4, E3), E2(E4, E3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),

                // // Measure 17
                // E2(GS4, E3), E2(GS4, E3), E2(DS5, GS3), E2(CS5, GS3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, GS3),
                // E2(E4, E3), E2(E4, E3), E2(B4, GS3), E2(GS4, GS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, GS3),

                // // Measure 18
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(E5, B2),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),
                // E2(E4, E3), E2(E4, E3), E2(B4, E3),

                // // Measure 19
                // E2(GS4, E3), E2(GS4, E3), E2(DS5, GS3), E2(CS5, GS3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, GS3),
                // E2(E4, E3), E2(E4, E3), E2(B4, GS3), E2(GS4, GS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, GS3),

                // // Measure 20
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(B4, E3),
                // E2(GS4, E3), E2(GS4, E3), E2(E4, E3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),

                // // Measure 21
                // E2(GS4, E3), E2(GS4, E3), E2(B4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, DS3),
                // E2(B4, E3), E2(B4, E3), E2(E4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, DS3),

                // // Measure 22
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(E5, B2),
                // E2(B4, GS2), E2(B4, GS2), E2(E4, GS2),
                // E2(GS4, GS2), E2(GS4, GS2), E2(B4, GS2),

                // // Measure 23
                // E2(GS4, E3), E2(GS4, E3), E2(B4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(GS4, DS3),
                // E2(B4, E3), E2(B4, E3), E2(E4, DS3), E2(GS4, DS3),
                // E2(CS5, E3), E2(CS5, E3), E2(E5, DS3),

                // // Measure 24
                // E2(DS5, B2), E2(DS5, B2), E2(GS4, B2),
                // E2(CS5, B2), E2(CS5, B2), E2(B4, E3),
                // E2(GS4, E3), E2(GS4, E3), E2(E4, E3),
                // E2(B4, E3), E2(B4, E3), E2(GS4, E3),

                // // Measure 25
                // E2(B4, GS3), E2(B4, GS3), E2(DS5, B3), E2(CS5, B3),
                // E2(FS4, GS3), E2(FS4, GS3), E2(B4, B3),
                // E2(DS4, GS3), E2(DS4, GS3), E2(GS4, B3), E2(B4, B3),
                // E2(CS5, GS3), E2(CS5, GS3), E2(E5, B3),

                // // Measure 26
                // E2(DS5, E3), E2(DS5, E3), E2(B4, E3),
                // E2(CS5, E3), E2(CS5, E3), E2(DS5, E3),
                // E2(B4, GS2), E2(B4, GS2), E2(GS4, GS2),
                // E2(E4, GS2), E2(E4, GS2), E2(B4, GS2),

                // // Measure 27
                // E2(B4, GS3), E2(B4, GS3), E2(DS5, B3), E2(CS5, B3),
                // E2(FS4, GS3), E2(FS4, GS3), E2(B4, B3),
                // E2(DS4, GS3), E2(DS4, GS3), E2(GS4, B3), E2(B4, B3),
                // E2(CS5, GS3), E2(CS5, GS3), E2(E5, B3),

                // Measure 28
                // E2(DS5, E3), E2(DS5, E3) | Note_End,
            },
};

#define SOUND_EXPL_RISE 0
#define SOUND_EXPL_FALL 1
#define SOUND_EXPL_EVEN -1

#define SOUND_VOL_RISE 0
#define SOUND_VOL_FALL 1
#define SOUND_VOL_EVEN -1

const explosion_t g_monsterPeng =
{
    0b00101100, SOUND_EXPL_RISE, SOUND_VOL_RISE, 0x40U
};

const track_t musicOff =
{
    .amplitudes = (amplitudes_t const*)&Vec_ADSR_FADE4,
    .frequencies = (frequencies_t const*)&Vec_TWANG_VIBEHL,
    .notes = {128, 128, 0, 128}
};
