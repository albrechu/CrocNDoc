// ***************************************************************************
// vectrex boot loader
// ***************************************************************************

#include <vectrex.h>

// ---------------------------------------------------------------------------

extern int main(void);

// ---------------------------------------------------------------------------

#define __INIT_DATA		1	// set to 1 to initialize global ram
#define __ZERO_BSS 		1	// set to 1 to clear bss segment on reset

// ---------------------------------------------------------------------------

__attribute__((section(".boot")))
void boot(void)
{
	__asm volatile (
	"_crt0:							\n\t"
#if __INIT_DATA
	"_crt0_data:					\n\t"
	"	ldx		#l_.data			\n\t"
	"	beq		_crt0_data_done		\n\t"
	"	ldu		#a_.text			\n\t"
	"	leau	l_.text,u			\n\t"
	"	leau	l_.text.hot,u		\n\t"
	"	leau	l_.text.unlikely,u	\n\t"
	"	ldy		#a_.data			\n\t"
	"_crt0_data_copy:				\n\t"
	"	lda		,u+					\n\t"
	"	sta		,y+					\n\t"
	"	leax	-1,x				\n\t"
	"	bne		_crt0_data_copy		\n\t"
	"_crt0_data_done:				\n\t"
#endif
#if __ZERO_BSS
	"_crt0_bss:						\n\t"
	"	ldx		#l_.bss				\n\t"
	"	beq		_crt0_bss_done		\n\t"
	"	ldy		#a_.bss				\n\t"
	"_crt0_bss_zero:				\n\t"
	"	clr		,y+					\n\t"
	"	leax	-1,x				\n\t"
	"	bne		_crt0_bss_zero		\n\t"
	"_crt0_bss_done:				\n\t"
#endif
	);
	
	if (main())
	{
		Vec_Cold_Flag = 0L;
	}
	Reset();
} 

// ***************************************************************************
// end of file
// ***************************************************************************
