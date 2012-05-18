//---------------------------------------------------------------
// Name: Ren's String Library
//
// A standalone string class not relying on STL.
// Speed may be sacrificed for code size and readibility, but I've
// also included some assembly optimizations which may speed things up.
//
// Using id's string library as inspiration
//
// This file contains AVX assembly methods.
// Note: I don't have a sandy bridge CPU, so these are untested
//---------------------------------------------------------------
#include "Str.h"

#ifdef _AVX_
void rStr::toLower_avx(void)
{
}
void rStr::toUpper_avx(void)
{
}
bool rStr::hasLower_avx(void)
{
}
bool rStr::hasUpper_avx(void)
{
}
void rStr::fill_avx(const char c) 
{
}
void rStr::r_memset_avx(char* text, char c, int length) 
{
}

//Derived from r_strlen_sse42
int rStr::r_strlen_avx(const char* src)
{
	int i;
	__asm
	{
		mov		esi, src
		mov		eax, -32
		pxor	ymm0, ymm0

STRLEN_LOOP:
		add		eax, 32
		PcmpIstrI ymm0, 32[esi + eax], 1000b
		jnz		STRLEN_LOOP

		add		eax, 32
		mov		i, eax
	}

	return i;
}
#endif