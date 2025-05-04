// ***************************************************************************
// print
// ***************************************************************************

#include <vectrex.h>
#include <lib/print.h>

static inline __attribute__((always_inline)) 
void reset_print_position(void)
{
	dp_VIA_cntl = 0xcc;				// zero the integrators, reset beam
	dp_VIA_t1_cnt_lo = PRINT_SCALE;	// set scale
}

// ---------------------------------------------------------------------------
// print binary unsigned int value at absolute coordinates (y, x)

void print_binary(int y, int x, unsigned int z)
{
	volatile char message[9];
	message[8] = '\x80';
	unsigned int i = 7;
	do
	{
		message[i--] = (char) ('0' + (z & 1));
		z >>= 1; 
	}
	while (i != 255);	
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print a c string (with \0 at the end) at absolute coordinates (y, x)

void print_char(int y, int x, char c)
{
	char s[4];
	s[0] = c;
	s[1] = ' ';
	s[2] = ' ';
	s[3] = '\x80';
	
	Reset0Ref(); // Reset beam
	dp_VIA_t1_cnt_lo = 0x7F; // Scale beam
	Moveto_d(y, x);
	Print_Str((void*) s);
}

// ---------------------------------------------------------------------------
// print long hex value at absolute coordinates (y, x)

void print_hex(int y, int x, unsigned int z)
{
	volatile char message[3];
	message[2] = '\x80';
	if ((message[1] = (z & 0b00001111) + '0') > '9')
	{
		message[1] += 7;
	}
	z >>= 4;
	if ((message[0] = z + '0') > '9')
	{
		message[0] += 7;
	}
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print long hex value at absolute coordinates (y, x)

void print_long_hex(int y, int x, long unsigned int z)
{
	char message[5];
	message[4] = '\x80';
	unsigned int lo = (unsigned int) z;
	unsigned int hi = (unsigned int) (z >> 8);
	
	if ((message[3] = (lo & 0b00001111) + '0') > '9')
	{
		message[3] += 7;
	}
	lo >>= 4;
	if ((message[2] = (lo & 0b00001111) + '0') > '9')
	{
		message[2] += 7;
	}
	if ((message[1] = (hi & 0b00001111) + '0') > '9')
	{
		message[1] += 7;
	}
	hi >>= 4;
	if ((message[0] = (hi & 0b00001111) + '0') > '9')
	{
		message[0] += 7;
	}
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print long binary unsigned int value at absolute coordinates (y, x)

void print_long_binary(int y, int x, long unsigned int z)
{
	volatile char message[17];
	message[16] = '\x80';
	unsigned int i = 15;
	do
	{
		message[i--] = (char) ('0' + (z & 1));
		z >>= 1; 
	}
	while (i != 255);	
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print decimal unsigned long int value at absolute coordinates (y, x)

void print_long_signed_int(int y, int x, long signed int z)
{
	volatile char message[7];
	message[6] = '\x80';
	message[4] = '0';
	message[3] = '0';
	message[2] = '0';
	message[1] = '0';
	if (z >= 0)
	{
		message[0] = (char) 0x2b;
	}
	else
	{
		message[0] = (char) 0x2d;
		z = -z;
	}
	while ((long unsigned int) z >= 10000)
	{
		++message[1];
		z -= 10000;
	}
	while (z >= 1000)
	{
		++message[2];
		z -= 1000;
	}
	while (z >= 100)
	{
		++message[3];
		z -= 100;
	}

	while (z >= 10)
	{
		++message[4];
		z -= 10;
	}
	message[5] = '0' + (unsigned char) z;
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print decimal unsigned long int value at absolute coordinates (y, x)

void print_long_unsigned_int(int y, int x, unsigned long int z)
{
	volatile char message[6];
	message[5] = '\x80';
	message[3] = '0';
	message[2] = '0';
	message[1] = '0';
	message[0] = '0';
	while (z >= 10000)
	{
		++message[0];
		z -= 10000;
	}
	while (z >= 1000)
	{
		++message[1];
		z -= 1000;
	}
	while (z >= 100)
	{
		++message[2];
		z -= 100;
	}
	while (z >= 10)
	{
		++message[3];
		z -= 10;
	}
	message[4] = '0' + (unsigned char) z;
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print decimal int value at absolute coordinates (y, x)

void print_signed_int(int y, int x, int z)
{
	volatile char message[5];
	message[4] = '\x80';
	message[2] = '0';
	
	if (z >= 0)
	{
		message[0] = (char) 0x2b;
	}
	else
	{
		message[0] = (char) 0x2d;
		z = -z;
	}
	if ((unsigned int) z < 100)
	{
		message[1] = '0';
	}
	else
	{
		message[1] = '1';
		z -= 100;
	}
	while (z >= 10)
	{
		++message[2];
		z -= 10;
	}
	message[3] = '0' + (unsigned char) z;
	
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ---------------------------------------------------------------------------
// print a c string (with \0 at the end) at absolute coordinates (y, x)

void print_string(int y, int x, char* text)
{
	reset_print_position();
	Print_Str_d(y, x, (void*) text);
}

// ---------------------------------------------------------------------------
// print decimal unsigned int value at absolute coordinates (y, x)

void print_unsigned_int(int y, int x, unsigned int z)
{
	volatile char message[4];
	message[3] = '\x80';
	message[1] = '0';
	message[0] = '0';
	while (z >= 100)
	{
		++message[0];
		z -= 100;
	}
	while (z >= 10)
	{
		++message[1];
		z -= 10;
	}
	message[2] = '0' + z;
	reset_print_position();
	Print_Str_d(y, x, (void*) &message[0]);
}

// ***************************************************************************
// end of file
// ***************************************************************************
