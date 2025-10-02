/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <vectrex.h>
#include <cnd/sound.h>

/////////////////////////////////////////////////////////////////////////
//	Types
//
/**
* @brief cartridge_t contains startup title information.
*/
struct cartridge_t
{
	char        copyright[11]; // copyright string, must start with "g GCE" and must end with "\x80"
	void const* music;		   // 16 bit memory address of title music data
	i8          title_height;  // signed 8 bit value, height of game title letters
	u8          title_width;   // unsigned 8 bit value, width of game title letters
	i8          title_y;	   // signed 8 bit value, y coordinate of game title
	i8          title_x;	   // signed 8 bit value, x coordinate of game title
	char        title[]; 	   // game title string, must end with "\x80\x00"
};

/////////////////////////////////////////////////////////////////////////
//	Global Data
//
const struct cartridge_t game_header __attribute__((section(".cartridge"), used)) = 
{
	.copyright 	    = "g GCE 2025\x80",	
	.music 		    = (void const*)&g_musicOff,
	.title_height 	= -8,
	.title_width 	= 80,
	.title_y 	    = 16,
	.title_x 	    = -57,
	.title 	     	= "CROC&DOC\x80"	
};

// ---------------------------------------------------------------------------

__asm(". = . - 1\n\t"); // this is soooo dirty...
const i8 end_of_cartridge_header __attribute__((section(".cartridge"), used)) = 0;
