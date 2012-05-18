//---------------------------------------------------------------
// Name: Ren's String Library
//
// A standalone string class not relying on STL.
// Speed may be sacrificed for code size and readibility, but I've
// also included some assembly optimizations which may speed things up.
//
// Using id's string library as inspiration
//
// This file contains scalar assembly methods
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

	//Don't bother counting if we're null
	if(!src)
		return 0;

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
	//Fast code loop chooser
	int loopMod = 1;
	if(l % 10 == 0)		loopMod = 10;
	else if(l % 9 == 9) loopMod = 9;
	else if(l % 8 == 8) loopMod = 8;
	else if(l % 7 == 7) loopMod = 7;
	else if(l % 6 == 6) loopMod = 6;
	else if(l % 5 == 5) loopMod = 5;
	else if(l % 4 == 4) loopMod = 4;
	else if(l % 3 == 3) loopMod = 3;
	else if(l % 2 == 2) loopMod = 2;
	else				loopMod = 1;
	__asm
	{
		mov		edi, dst		//Move dest pointer to edi
		mov		esi, src		//Move src pointer to esi
		mov		ecx, l			//Move length to counter register

		cmp		ecx, 0			//See if length is zero
		jz		exit			//If zero, exit

		mov		edx, loopMod
		cmp		edx, 1
		je		loop1
		cmp		edx, 2
		je		loop2
		cmp		edx, 3
		je		loop3
		cmp		edx, 4
		je		loop4
		cmp		edx, 5
		je		loop5
		cmp		edx, 6
		je		loop6
		cmp		edx, 7
		je		loop7
		cmp		edx, 8
		je		loop8
		cmp		edx, 9
		je		loop9
		cmp		edx, 10
		je		loop10

loop10:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		mov		eax, [esi+5]
		mov		[edi+5], eax
		mov		eax, [esi+6]
		mov		[edi+6], eax
		mov		eax, [esi+7]
		mov		[edi+7], eax
		mov		eax, [esi+8]
		mov		[edi+8], eax
		mov		eax, [esi+9]
		mov		[edi+9], eax
		add		esi, 10	
		add		edi, 10
		sub		ecx, 10	
		jne		loop10
		je		exit

loop9:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		mov		eax, [esi+5]
		mov		[edi+5], eax
		mov		eax, [esi+6]
		mov		[edi+6], eax
		mov		eax, [esi+7]
		mov		[edi+7], eax
		mov		eax, [esi+8]
		mov		[edi+8], eax
		add		esi, 9	
		add		edi, 9
		sub		ecx, 9	
		jne		loop9
		je		exit

loop8:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		mov		eax, [esi+5]
		mov		[edi+5], eax
		mov		eax, [esi+6]
		mov		[edi+6], eax
		mov		eax, [esi+7]
		mov		[edi+7], eax
		add		esi, 8	
		add		edi, 8
		sub		ecx, 8	
		jne		loop8
		je		exit

loop7:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		mov		eax, [esi+5]
		mov		[edi+5], eax
		mov		eax, [esi+6]
		mov		[edi+6], eax
		add		esi, 7	
		add		edi, 7
		sub		ecx, 7	
		jne		loop7
		je		exit

loop6:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		mov		eax, [esi+5]
		mov		[edi+5], eax
		add		esi, 6	
		add		edi, 6
		sub		ecx, 6	
		jne		loop6
		je		exit

loop5:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		add		esi, 5	
		add		edi, 5
		sub		ecx, 5	
		jne		loop5
		je		exit

loop4:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		mov		eax, [esi+3]
		mov		[edi+3], eax
		add		esi, 4	
		add		edi, 4
		sub		ecx, 4	
		jne		loop4
		je		exit

loop3:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		mov		eax, [esi+2]
		mov		[edi+2], eax
		add		esi, 3	
		add		edi, 3
		sub		ecx, 3	
		jne		loop3
		je		exit

loop2:
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+1]
		mov		[edi+1], eax
		add		esi, 2	
		add		edi, 2
		sub		ecx, 2	
		jne		loop2
		je		exit

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
#endif