//---------------------------------------------------------------
// Name: Ren's String Library
//
// A standalone string class not relying on STL.
// Speed may be sacrificed for code size and readibility, but I've
// also included some assembly optimizations which may speed things up.
//
// Using id's string library as inspiration
//
// This file contains MMX assembly methods
//---------------------------------------------------------------
#include "Str.h"

#ifdef _MMX_
void rStr::toLower_mmx(void)
{
	__asm emms
}
void rStr::toUpper_mmx(void)
{
	__asm emms
}
bool rStr::hasLower_mmx(void)
{
	__asm emms
}
bool rStr::hasUpper_mmx(void)
{
	__asm emms
}
void rStr::fill_mmx(const char c) 
{
	__asm emms
}

//This doesn't seem to work when dst has data in it, garg!
void rStr::r_memset_gran_mmx(const char* dst, const char c, const int count)
{
	int l = count;
	if(l <= 0)
		return;
		
	union {
		unsigned char			bytes[8];
		unsigned short			words[4];
		unsigned int			dwords[2];
	} dat;

	dat.bytes[0] = c;
	dat.bytes[1] = c;
	dat.words[1] = dat.words[0];
	dat.dwords[1] = dat.dwords[0];

	__asm
	{
		mov		edi, dst
		mov		ecx, l

		movq	mm1, dat
loop8:
		movntq	8[edi], mm1
		add		edi, 8
		dec		ecx
		jnz		loop8
	}
	__asm emms
}
#endif