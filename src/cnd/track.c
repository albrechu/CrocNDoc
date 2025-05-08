#include <cnd/track.h>

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
#define SPQ 10
#define SPE 9
#define SPS 8
#define SPHF 9
#define SPQF 8
#define SPEF 7
#define SPSF 6
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
#define EI3(a, b, c) CH0 | a, CH1 | b, DUR(c, SPEF)

// Two-Channels Fast
#define Q2F(a, b) CH0 | a, DUR(b, SPQF)
#define E2F(a, b) CH0 | a, DUR(b, SPEF)

const nibble_ampoff_table_t melodymasterSoft =
    {
        .amplitudes = {0xFF, 0xFF, 0xEE, 0xEE, 0xDD, 0xDD, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

const twang_table_t twangtable =
    {
        .frequencies = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

const track_t g_yellow =
    {
        .amplitudes = &melodymasterSoft,
        .frequencies = &twangtable,
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
        .amplitudes = &melodymasterSoft,
        .frequencies = &twangtable,
        .notes =
            {
                // Measure 1
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 2
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 3
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 4
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                Q2(G4, C3),
                Q2(G4, E3),
                Q2(G4, G3),
                Q2(G4, C4),

                // Measure 5
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 6
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 7
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 8
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                Q2(G4, C3),
                Q2(G4, E3),
                Q2(G4, G3),
                Q2(G4, C4),

                // Measure 9
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 10
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 11
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 12
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                Q2(G4, C3),
                Q2(G4, E3),
                Q2(G4, G3),
                Q2(G4, C4),

                // Measure 13
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 14
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 15
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),

                // Measure 16
                E2(F4, C3),
                E2(F4, G3),
                E2(F4, C4),
                E2(F4, G4),
                Q2(G4, C3),
                Q2(G4, E3),
                Q2(G4, G3),
                Q2(G4, C4) | Note_End,
            },
};

const track_t g_level0 =
    {
        .amplitudes = &melodymasterSoft,
        .frequencies = &twangtable,
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

#define CROCODILE_CACOPHONY_BASE 2

#define CROCODILE_CACOPHONY_VERY_SHORT (CROCODILE_CACOPHONY_BASE * 2)
#define CROCODILE_CACOPHONY_SHORT (CROCODILE_CACOPHONY_BASE * 3)
#define CROCODILE_CACOPHONY_HALF (CROCODILE_CACOPHONY_BASE * 4)
#define CROCODILE_CACOPHONY_SPEED (CROCODILE_CACOPHONY_BASE * 6)
#define CROCODILE_CACOPHONY_DOT (CROCODILE_CACOPHONY_BASE * 9)
#define CROCODILE_CACOPHONY_SPEED2 (CROCODILE_CACOPHONY_BASE * 12)
#define CROCODILE_CACOPHONY_PAUSE2 (CROCODILE_CACOPHONY_BASE * 18)
#define CROCODILE_CACOPHONY_PAUSE3 (CROCODILE_CACOPHONY_BASE * 24)
#define CROCODILE_CACOPHONY_LONG (CROCODILE_CACOPHONY_BASE * 30)

#define CC1(x, speed) x, speed
#define CC1S(x) CC1(x, CROCODILE_CACOPHONY_SHORT)
#define CC2S(x, y) x | CHN, CC1S(y)
#define CC3S(x, y, z) x | CHN, CC2S(y, z)
#define CC1H(x) CC1(x, CROCODILE_CACOPHONY_HALF)
#define CC2H(x, y) x | CHN, CC1H(y)
#define CC3H(x, y, z) x | CHN, CC2H(y, z)
#define CC1N(x) CC1(x, CROCODILE_CACOPHONY_SPEED)
#define CC2N(x, y) x | CHN, CC1H(y)
#define CC3N(x, y, z) x | CHN, CC2H(y, z)

const track_t g_crocodileCacophony =
    {
        .amplitudes = &melodymasterSoft,
        .frequencies = &twangtable,
        .notes =
            {
                CC1S(D4),
                CC1S(D4),
                CC1N(D4),
                CC1N(D4),
                CC1S(D4),
                CC1S(D4),
                CHN + D4, CHN + A5, D6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + AS5, DS6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + B6, DS6, CROCODILE_CACOPHONY_SPEED,

                CC1S(D4),
                CC1S(D4),
                CC1N(D4),
                CC1N(D4),
                CC1S(D4),
                CC1S(D4),
                CHN + D4, CHN + C6, E6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + CS6, F6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + D6, F6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, FS6, CROCODILE_CACOPHONY_SHORT,

                CC1S(D4),
                CC1S(D4),
                CC1N(D4),
                CC1N(D4),
                CC1S(D4),
                CC1S(D4),
                CHN + D4, CHN + B5, E6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + C6, G6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + E6, GS6, CROCODILE_CACOPHONY_SPEED,

                CC1S(D4),
                CC1S(D4),
                CC1N(D4),
                CC1N(D4),
                CC1S(D4),
                CC1S(D4),
                CHN + D4, CHN + D6, FS6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + DS6, G6, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, CHN + E6, G6, CROCODILE_CACOPHONY_SHORT,
                CHN + E6, GS6, CROCODILE_CACOPHONY_SHORT,

                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G4, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + G6, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + G6, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + G6, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G6, AS6, CROCODILE_CACOPHONY_SHORT,

                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G4, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + FS6, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + FS6, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + FS6, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + FS6, A6, CROCODILE_CACOPHONY_SHORT,

                CHN + G3, CHN + DS6, AS6, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, AS6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + AS5, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G4, CHN + DS6, AS6, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, AS5, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + DS6, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + D6, AS6, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + DS6, AS6, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, AS6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + AS5, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + DS6, AS6, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, AS5, CROCODILE_CACOPHONY_SHORT,
                AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + F6, AS6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, AS6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, AS6, CROCODILE_CACOPHONY_SHORT,

                CHN + G3, CHN + DS6, A6, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, A6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + A5, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G4, CHN + DS6, A6, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, A5, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + DS6, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + D6, A6, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, CHN + DS6, A6, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, A6, CROCODILE_CACOPHONY_SHORT,
                G3, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + A5, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + DS6, A6, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + D6, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, A5, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,
                CHN + G4, CHN + DS6, A6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + D6, A6, CROCODILE_CACOPHONY_SHORT,
                A6, CROCODILE_CACOPHONY_SHORT,

                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                B3, CROCODILE_CACOPHONY_SPEED,
                G3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                B3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                B3, CROCODILE_CACOPHONY_SPEED,
                G3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                FS3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                B3, CROCODILE_CACOPHONY_SPEED,
                G3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                B3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                CC1S(G3),
                B3, CROCODILE_CACOPHONY_SPEED,
                G3, CROCODILE_CACOPHONY_SPEED2,
                CC1S(G3),
                CC1S(G3),
                A3, CROCODILE_CACOPHONY_SPEED2,

                CHN + D3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + D5, A5, CROCODILE_CACOPHONY_SHORT,
                D3, CROCODILE_CACOPHONY_HALF,
                F3, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + A3, CHN + D5, A5, CROCODILE_CACOPHONY_PAUSE2,
                CHN + F3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + CS5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + CS5, A5, CROCODILE_CACOPHONY_VERY_SHORT,
                G3, CROCODILE_CACOPHONY_HALF,
                F3, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + CS5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + A3, CHN + D5, A5, CROCODILE_CACOPHONY_SHORT,
                CHN + D5, A5, CROCODILE_CACOPHONY_SHORT,
                CHN + F3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + D3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED2,

                CHN + D3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + D5, A5, CROCODILE_CACOPHONY_SHORT,
                D3, CROCODILE_CACOPHONY_HALF,
                F3, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + A3, CHN + D5, A5, CROCODILE_CACOPHONY_PAUSE2,
                CHN + F3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, CHN + CS5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + CS5, A5, CROCODILE_CACOPHONY_VERY_SHORT,
                G3, CROCODILE_CACOPHONY_HALF,
                F3, CROCODILE_CACOPHONY_SPEED,
                CHN + E3, CHN + CS5, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + D3, CHN + D5, A5, CROCODILE_CACOPHONY_SPEED,

                G3, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, D6, CROCODILE_CACOPHONY_SPEED,
                FS4, CROCODILE_CACOPHONY_SPEED,
                CHN + D4, D6, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, D6, CROCODILE_CACOPHONY_PAUSE2,
                DS4, CROCODILE_CACOPHONY_SHORT,
                FS4, CROCODILE_CACOPHONY_SPEED,
                FS4, CROCODILE_CACOPHONY_SPEED,

                CHN + DS4, C6, CROCODILE_CACOPHONY_SHORT,
                AS5, CROCODILE_CACOPHONY_SHORT,
                CHN + G3, A5, CROCODILE_CACOPHONY_SPEED,
                CS4, CROCODILE_CACOPHONY_SPEED,
                CHN + FS4, AS5, CROCODILE_CACOPHONY_SPEED,
                CHN + CS4, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + G3, AS5, CROCODILE_CACOPHONY_SHORT,
                CC1S(D4),
                CHN + FS4, A5, CROCODILE_CACOPHONY_SPEED,
                CHN + FS4, G5, CROCODILE_CACOPHONY_SPEED2,

                D6, CROCODILE_CACOPHONY_SPEED2,
                D6, CROCODILE_CACOPHONY_SPEED,
                D6, CROCODILE_CACOPHONY_PAUSE2,
                C6, CROCODILE_CACOPHONY_SHORT,
                AS5, CROCODILE_CACOPHONY_SHORT,
                A5, CROCODILE_CACOPHONY_SPEED,
                A5, CROCODILE_CACOPHONY_SHORT,
                AS5, CROCODILE_CACOPHONY_SPEED,
                A5, CROCODILE_CACOPHONY_SPEED2,
                C6, CROCODILE_CACOPHONY_SPEED,
                AS5, CROCODILE_CACOPHONY_SPEED,
                A5, CROCODILE_CACOPHONY_SPEED,
                A5, CROCODILE_CACOPHONY_SHORT,
                AS5, CROCODILE_CACOPHONY_SPEED,
                C6, CROCODILE_CACOPHONY_DOT,
                A5, CROCODILE_CACOPHONY_SHORT,
                AS5, CROCODILE_CACOPHONY_SHORT,
                D6, CROCODILE_CACOPHONY_SPEED2 | Note_End,
                /*
                        0x80 + D3,	0x80 + D5,  A5, CROCODILE_CACOPHONY_SPEED,
                                                        0x80 + D5,  A5, CROCODILE_CACOPHONY_SHORT,
                               D3,					 								CROCODILE_CACOPHONY_HALF,
                               F3,					 								CROCODILE_CACOPHONY_SPEED,
                        0x80 + G3,	0x80 + D5,  A5, CROCODILE_CACOPHONY_SPEED,
                      0x80 + A3,	0x80 + D5,  A5, CROCODILE_CACOPHONY_SHORT,
                                                        0x80 + D5,  A5, CROCODILE_CACOPHONY_SHORT,
                                                        0x80 + D5,  A5, CROCODILE_CACOPHONY_DOT,
                        0x80 + F3,	0x80 + D5,  A5, CROCODILE_CACOPHONY_SPEED,
                        0x80 + G3,	0x80 + CS5, A5, CROCODILE_CACOPHONY_SPEED,
                                                        0x80 + CS5, A5,	CROCODILE_CACOPHONY_VERY_SHORT,
                                     G3,									        CROCODILE_CACOPHONY_HALF,
                                     F3,									        CROCODILE_CACOPHONY_SPEED,
                      0x80 + G3,	0x80 + CS5, A5, CROCODILE_CACOPHONY_SPEED,
                        0x80 + A3,  0x80 + D5,  A5, CROCODILE_CACOPHONY_SHORT,
                                                        0x80 + D5,  A5, CROCODILE_CACOPHONY_SHORT,
                        0x80 + F3,	0x80 + D5,  A5, CROCODILE_CACOPHONY_SPEED,
                        0x80 + D3,	0x80 + D5,  A5,	CROCODILE_CACOPHONY_SPEED,


                        D3, CROCODILE_CACOPHONY_DOT,
                        D3, CROCODILE_CACOPHONY_HALF,
                        F3, CROCODILE_CACOPHONY_SPEED,
                        G3, CROCODILE_CACOPHONY_DOT,
                        A3, CROCODILE_CACOPHONY_LONG,
                        F3, CROCODILE_CACOPHONY_SPEED,
                        G3, CROCODILE_CACOPHONY_DOT,
                        G3, CROCODILE_CACOPHONY_HALF,
                        F3, CROCODILE_CACOPHONY_SPEED,
                        G3, CROCODILE_CACOPHONY_SPEED,
                        A3, CROCODILE_CACOPHONY_SPEED,
                        F3, CROCODILE_CACOPHONY_SPEED,
                        D3,
                        */
                // 1 | Note_End
            },
};

const track_t g_jibjig =
    {
        .amplitudes = &melodymasterSoft,
        .frequencies = &twangtable,
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

const track_t *g_tracks[] =
    {
        &g_crocodileCacophony,
        &g_yellow,
        &g_level0,
        &g_corneria,
};
