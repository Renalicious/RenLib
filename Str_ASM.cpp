//---------------------------------------------------------------
// Name: Ren's String Library
//
// Light weight string library
// It is meant to be standalong, not relying on STL
// Computation speed may be sacrificed for minimal code size
//
// Using id's string library as inspiration
//
// This file contains assembly optimized functions
//---------------------------------------------------------------
#include "Str.h"

#ifdef ASM

//rfind, back to front
int rStr::rfind_asm(const char *src, const char c, const int l)
{
	if(!l)
		return -1;

	int i;
	__asm
	{
		mov		edi, src	//Move text pointer into edi
		add		edi, l		//Move to last point in data
		mov		eax, 0
		mov		al, c		//Move test character into EAX
		mov		ecx, l		//Move length into ECX

loop1:
		mov		ebx, [edi]	//Move src to EAX
		cmp		bl, al		//Compare lower EBX against test character
		je		match		//exit if there is a match
		dec		edi			//If not, increment pointer
		dec		ecx			//Decrement counter by 1
		jz		noMatch
		jnz		loop1		//If not zero, goto loop1

noMatch:
		mov		i, -1		//Move -1 into i, no match
		jmp		exit

match:
		mov		i, ecx		//Move counter to i

exit:
		nop
		
	}

	return i; //Diff in position
}

//Find, front to back
int rStr::find_asm(const char *src, const char c, const int l)
{
	if(!l)
		return -1;

	int i;
	__asm
	{
		mov		edi, src	//Move text pointer into edi
		mov		eax, 0
		mov		al, c		//Move test character into EAX
		mov		ecx, l		//Move length into ECX

loop1:
		mov		ebx, [edi]	//Move src to EAX
		cmp		bl, al		//Compare lower EBX against test character
		je		match		//exit if there is a match
		inc		edi			//If not, increment pointer
		dec		ecx			//Decrement counter by 1
		jz		noMatch
		jnz		loop1		//If not zero, goto loop1

noMatch:
		mov		i, -1		//Move -1 into i, no match
		jmp		exit

match:
		mov		i, ecx		//Move counter to i
		mov		ecx, l		//Move l into ECX
		sub		ecx, i		//Position difference
		mov		i, ecx		//Move new difference back into i

exit:
		nop
		
	}

	return i; //Diff in position
}

//rFind case insensitive, back to front
int rStr::rfindNoCase_asm(const char *src, const char charLow, const char charUpr, const int l)
{
	if(!l)
		return -1;

	int i;
	__asm
	{
		mov		edi, src	//Move text pointer into edi
		add		edi, l		//Move to last point in data
		mov		eax, 0
		mov		al, charLow	//Move test character into EAX
		mov		ah, charUpr	//Move test character into EAX
		mov		ecx, l		//Move length into ECX

loop1:
		mov		ebx, [edi]	//Move src to EAX
		cmp		bl, al		//Compare lower EBX against test character
		je		match		//exit if there is a match
		cmp		bl, ah		//Compare lower EBX against test character
		je		match		//exit if there is a match
		dec		edi			//If not, increment pointer
		dec		ecx			//Decrement counter by 1
		jz		noMatch
		jnz		loop1		//If not zero, goto loop1

noMatch:
		mov		i, -1		//Move -1 into i, no match
		jmp		exit

match:
		mov		i, ecx		//Move counter to i

exit:
		nop
		
	}

	return i; //Diff in position
}

//Find case insensitive, front to back
int rStr::findNoCase_asm(const char *src, const char charLow, const char charUpr, const int l)
{
	if(!l)
		return -1;

	int i;
	__asm
	{
		mov		edi, src	//Move text pointer into edi
		mov		eax, 0
		mov		al, charLow	//Move test character into EAX
		mov		ah, charUpr	//Move test character into EAX
		mov		ecx, l		//Move length into ECX

loop1:
		mov		ebx, [edi]	//Move src to EAX
		cmp		bl, al		//Compare lower EBX against test character
		je		match		//exit if there is a match
		cmp		bl, ah		//Compare lower EBX against test character
		je		match		//exit if there is a match
		inc		edi			//If not, increment pointer
		dec		ecx			//Decrement counter by 1
		jz		noMatch
		jnz		loop1		//If not zero, goto loop1

noMatch:
		mov		i, -1		//Move -1 into i, no match
		jmp		exit

match:
		mov		i, ecx		//Move counter to i
		mov		ecx, l		//Move l into ECX
		sub		ecx, i		//Position difference
		mov		i, ecx		//Move new difference back into i

exit:
		nop
		
	}

	return i; //Diff in position
}

//String length, based on '\0' terminator
int rStr::r_strlen_asm(const char *src)
{
	/* Reference: http://www.int80h.org/strlen/ */
	int i = 0;
	__asm
	{
		mov		edi, src	//Move text pointer into edi
		sub		ecx, ecx	//Make ecx 0
		not		ecx			//Make ecx maximum unsigned __int32
		sub		al, al		//lower register A 0
		cld
		repne	scasb		//Scan bytes

		not		ecx			//Get unsigned inverse to figure out length (1/x)
		dec		ecx			//Minus 1
		mov		i, ecx
	}
	
	return i;
}

//Copy memory
void rStr::r_memcopy_asm(const char* dst, const char* src, const int l)
{
	__asm
	{
		mov		edi, dst		//Move dest pointer to edi
		mov		esi, src		//Move src pointer to esi
		mov		ecx, l			//Move length to counter register

		cmp		ecx, 0			//See if length is zero
		jz		exit			//If zero, exit

loop1:
		mov		eax, [esi]		//Move src to EAX
		mov		[edi], eax		//Move EAX to dst
		inc		esi				//Increment pointers by 1byte
		inc		edi
		dec		ecx				//Decrement counter by 1
		jne		loop1			//If not zero, goto loop1

exit:
		nop						//No opperation
	}
}

//Fill memory based on arbitrary length, stuff that isn't aligned to granularity.
void rStr::r_memset_asm(const char* dst, const char c, const int l)
{
	__asm
	{
		mov		edi, dst		//Move dest pointer to edi
		mov		al, c			//Move character into lower EAX
		mov		ecx, l			//Move length into ecx

		cmp		ecx, 0			//See if length is zero
		jz		exit			//If zero, exit

loop1:
		mov		[edi], al		//Move character into edi
		inc		edi				//Increment pointer by 1 byte
		dec		ecx				//Decrement pointer by 1
		jne		loop1			//Check is counter is 0

exit:
		nop
	}
}

//Fill memory using granularity, should make things a bit faster
//because we're doing 4 bytes per loop instead of 4. This isn't SIMD.
void rStr::r_memset_gran_asm(const char* dst, const char c, const int l)
{
	__asm
	{
		mov		edi, dst		//Move dest pointer to edi
		mov		al, c			//Move character into lower EAX
		mov		ecx, l			//Move regCount into ecx

		cmp		ecx, 0			//See if length is zero
		jz		exit			//If zero, exit

loop4:
		mov		[edi    ], al	//Do 4 copies each loop
		mov		[edi + 1], al
		mov		[edi + 2], al
		mov		[edi + 3], al
		add		edi, 4
		dec		ecx				//We're going by regCount, not length
		jne		loop4

exit:
		nop
	}
}
#endif