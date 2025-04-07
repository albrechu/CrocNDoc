#include <track.h>

#define DUR(note, duration) note, duration
#define SPV 17
#define SPL 12
#define SPN 10
#define SPQ 7
#define DURV(note) DUR(note, SPV)
#define DURL(note) DUR(note, SPL)
#define DURN(note) DUR(note, SPN)
#define DURQ(note) DUR(note, SPQ)
// #define PAUSE 16

const nibble_ampoff_table_t melodymasterSoft =
{
	.amplitudes = { 0xFD, 0xDD, 0xCC, 0xCC, 0xBB, 0xBB, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

const twang_table_t twangtable =
{
	.frequencies = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

const track_t g_level0 = 
{
    .amplitudes  = &melodymasterSoft,
    .frequencies = &twangtable,
    .notes       = 
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
        // DURQ(Note_C4),
        // DURQ(Note_D5),
        // DURQ(Note_E5),
        // DURQ(Note_G5),
        // DURQ(Note_A5),
        // DURQ(Note_C5),
        // DURQ(Note_D6),
        // DURQ(Note_C5),
        // DURQ(Note_A5),
        // DURQ(Note_G5),
        // DURQ(Note_E5),
        // DURQ(Note_D5),
        // DURQ(Note_C4),
        // DURQ(Note_G4),


        // DURQ(Note_D4),
        // DURQ(Note_E4),
        // DURQ(Note_F4),
        // DURQ(Note_A4),
        // DURQ(Note_D5),
        // DURQ(Note_E5),
        // DURQ(Note_F5),
        // DURQ(Note_A5),
        // DURQ(Note_A5),
        // DURQ(Note_B5),
        // DURQ(Note_C5),
        // DURQ(Note_E6),
        // DURQ(Note_D6),
        // DURQ(Note_E6),
        // DURQ(Note_F6),
        // DURQ(Note_A6),

        // DURQ(Note_F4),
        // DURQ(Note_G4),
        // DURQ(Note_A4),
        // DURQ(Note_C4),
        // DURQ(Note_F5),
        // DURQ(Note_G5),
        // DURQ(Note_A5),
        // DURQ(Note_D6),
        // DURQ(Note_C5),
        // DURQ(Note_D6),
        // DURQ(Note_G6),
        // DURQ(Note_A6),
        // DURQ(Note_F6),
        // DURQ(Note_A6),
        // DURQ(Note_C6),
        // DURQ(Note_D7),

        // DURQ(Note_A4),
        // DURQ(Note_B4),
        // DURQ(Note_C4),
        // DURQ(Note_E5),
        // DURQ(Note_A5),
        // DURQ(Note_B5),
        // DURQ(Note_C5),
        // DURQ(Note_E6),
        // DURQ(Note_A5),
        // DURQ(Note_B5),
        // DURQ(Note_E6),
        // DURQ(Note_A6),
        // DURQ(Note_B6),
        // DURQ(Note_A6),
        // DURQ(Note_C6),
        // DURQ(Note_D7),

        // DURQ(Note_C4),
        // DURQ(Note_D5),
        // DURQ(Note_E5),
        // DURQ(Note_G5),
        // DURQ(Note_A5),
        // DURQ(Note_C5),
        // DURQ(Note_D6),
        // DURQ(Note_C5),
        // DURQ(Note_A5),
        // DURQ(Note_G5),
        // DURQ(Note_E5),
        // DURQ(Note_D5),
        // DURQ(Note_C4),
        // DURQ(Note_G4),
        // DURL(Note_A5),
        // DURN(Note_G5),
        // DURV(Note_F5),
        // DURQ(Note_F5),
        // DURN(Note_G5),
        
        // DURL(Note_AS5),
        // DURN(Note_GS5),
        // DURV(Note_FS5),
        // DURQ(Note_FS5),
        // DURQ(Note_GS5)


        // | Note_End,
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

        DURQ(Note_C4),
        DURQ(Note_D4),
        DURQ(Note_E4),
        DURQ(Note_F4),

        DURL(Note_G4),
        DURN(Note_F4),
        DURL(Note_E4),

        DURQ(Note_C4),
        DURQ(Note_D4),
        DURQ(Note_E4),
        DURQ(Note_F4),

        DURL(Note_G4),
        DURN(Note_F4),
        DURL(Note_E4),
        DURN(Note_F4),
        DURN(Note_G4),
        DURL(Note_F4),

        DURQ(Note_C4),
        DURQ(Note_D4),
        DURV(Note_B4),
        
        DURQ(Note_C4),
        DURQ(Note_D4),
        DURQ(Note_E4),
        DURQ(Note_F4),
        
        DURL(Note_G4),
        DURN(Note_F4),
        DURV(Note_E4),
        DURQ(Note_E4),
        DURQ(Note_F4),

        DURL(Note_G4),
        DURN(Note_F4),
        DURL(Note_E4),

        DURQ(Note_B5),
        DURQ(Note_C5),
        DURL(Note_B5),

        DURQ(Note_F4),
        DURQ(Note_G4),
        DURL(Note_F4),

        DURQ(Note_C4),
        DURQ(Note_D4),
        DURL(Note_B4),
        DURL(Note_A7)
        | Note_End,
        
    },
};

const track_t* g_tracks[] = 
{
    &g_level0,
};

const track_t* track_get(Track const track)
{
    return g_tracks[track];
}
