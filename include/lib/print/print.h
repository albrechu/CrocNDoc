// ***************************************************************************
// print
// ***************************************************************************

#pragma once
#include <vectrex.h>

// ---------------------------------------------------------------------------
// all functions for debugging purposes only,
// ---------------------------------------------------------------------------

void print_string(int y, int x, char* text);
void print_unsigned_int(int y, int x, unsigned int z);
void print_signed_int(int y, int x, int z);
void print_long_unsigned_int(int y, int x, long unsigned int z);
void print_long_signed_int(int y, int x, long signed int z);
void print_binary(int y, int x, unsigned int z);
void print_long_binary(int y, int x, long unsigned int z);
void print_hex(int y, int x, unsigned int z);
void print_long_hex(int y, int x, long unsigned int z);
void print_char(int y, int x, char c);

// ---------------------------------------------------------------------------

#define PRINT_SCALE 0x7F

// ***************************************************************************
// end of file
// ***************************************************************************
