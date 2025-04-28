// ***************************************************************************
// debug
// ***************************************************************************

#include <vectrex.h>

// ---------------------------------------------------------------------------

const int yy0 = 90;
const int xx0 = -120;
const char mm0[] = "ASSERT FAILED:\x80";
__asm(". = . - 1\n\t"); // this is soooo dirty...

const int yy1 = 40;
const int xx1 = -120;
const char mm1[] = "FILE:\x80";
__asm(". = . - 1\n\t"); // this is soooo dirty...

const int yy2 = -10;
const int xx2 = -120;
const char mm2[] = "FUNCTION:\x80";
__asm(". = . - 1\n\t"); // this is soooo dirty...

const int yy3 = -60;
const int xx3 = -120;
const char mm3[] = "LINE:\x80";

void _f_assert_failed(char* file, unsigned int length, unsigned long int line, char* func, char* expr, unsigned int l_expr)
{
	long unsigned int text_hw = Vec_Text_HW;
	Vec_Text_HW = 0xF850;

	Clear_Sound();
	
	char* p = file + length;
	length = 0;
	while ((*(--p) != '\\') && (*p != '/') && (p != file))
	{
		++length;
	}

	volatile char vfile[length--];
	unsigned int i;
	for (i = 0; i < length; ++i)
	{
		char c = *(++p);
		if ((c >= 'a') && (c <= 'z')) c -= ('a' - 'A');
		vfile[i] = c;
	}
	vfile[length] = '\x80';

	volatile char vline[7];
	Clear_Score((void*) &vline[0]);
	while (line > 255)
	{
		Add_Score_a(255, (void*) &vline[0]);
		line -= 255;
	}
	Add_Score_a((unsigned int) line, (void*) &vline[0]);


	i = 0;
	while(func[i++] != '\0')
	{
	}
	volatile char vfunc[i];
	--i;
	vfunc[i] = '\x80';
	while(--i != 255)
	{
		char c = func[i];
		if ((c >= 'a') && (c <= 'z')) c -= ('a' - 'A');
		vfunc[i] = c;
	}

	volatile char vexpr[l_expr];
	i = 0;
	while(i < l_expr)
	{
		char c = expr[i];
		if ((c >= 'a') && (c <= 'z')) c -= ('a' - 'A');
		if ((c < 0x20) || (c > 0x5f)) c = 0x66;
		vexpr[i] = c;
		++i;
	}
	vexpr[l_expr-1] = '\x80';

	do
	{
		Wait_Recal();
		Intensity_5F();
		Print_List((void*) &yy0);
		Print_Str_d(  70, -120, (char*) &vexpr[0]);
		Print_Str_d(  20, -120, (char*) &vfile[0]);
		Print_Str_d( -30, -120, (char*) &vfunc[0]);
		Print_Str_d( -80, -120, (char*) &vline[0]);
		Read_Btns();	
	}
	while (!(Vec_Buttons & 0b00000001));

	Vec_Text_HW = text_hw;
}

// ***************************************************************************
// end of file
// ***************************************************************************
