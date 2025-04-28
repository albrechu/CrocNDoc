// ***************************************************************************
// vectrex memory configuration
// ***************************************************************************

#include <vectrex.h>

// ---------------------------------------------------------------------------
// standard 32K ROM + 1K RAM configuration
// ---------------------------------------------------------------------------

__asm(
	".bank rom(BASE=0x0000,SIZE=0x8000,FSFX=_rom)\n\t"
	".area .cartridge		(BANK=rom)\n\t"
	".area .boot			(BANK=rom)\n\t"
	".area .text  			(BANK=rom)\n\t"
	".area .text.hot		(BANK=rom)\n\t"
	".area .text.unlikely	(BANK=rom)\n\t"
	".area .text.last		(BANK=rom)\n\t"
);

// ---------------------------------------------------------------------------
// Vectrex internal memory configuration

__asm(
	".bank ram(BASE=0xc880,SIZE=0x036b,FSFX=_ram)\n\t"
	".area .data            (BANK=ram)\n\t"
	".area .bss             (BANK=ram)\n\t"
);

// ***************************************************************************
// end of file
// ***************************************************************************
