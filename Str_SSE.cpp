//---------------------------------------------------------------
// Name: Ren's String Library
//
// A standalone string class not relying on STL.
// Speed may be sacrificed for code size and readibility, but I've
// also included some assembly optimizations which may speed things up.
//
// Using id's string library as inspiration
//
// This file contains SSE assembly methods
//---------------------------------------------------------------
#include "Str.h"

#ifdef _SSE_
void rStr::toLower_sse(void)
{
}
void rStr::toUpper_sse(void)
{
}
bool rStr::hasLower_sse(void)
{
}
bool rStr::hasUpper_sse(void)
{
}
void rStr::fill_sse(const char c) 
{
}

//Unaligned memory
//TODO: this crashes at "movups [edi], xmm0"
void memset_ups_sse(const char* dst, const char c, const int l)
{
	int repeat;
	union
	{
		__int8		i8[16];
		__int16		i16[8];
		__int32		i32[4];
		__int64		i64[2];
	}dat;

	dat.i8[0] = c;
	dat.i8[1] = c;
	dat.i16[1] = dat.i16[0];
	dat.i32[1] = dat.i32[0];
	dat.i64[1] = dat.i64[0];

	if(l % 8 == 0)			repeat = 8;
	else if(l % 4 == 0)		repeat = 4;
	else if(l % 2 == 0)		repeat = 2;
	else					repeat = 1;

	__asm
	{
		mov			edi, dst		//Move dest pointer to edi
		movdqu		xmm0, dat		//Move 128bit variable into XMM0
		mov			ecx, l			//Move regCount into ecx
		cmp			ecx, 0			//See if length is zero
		jz			exit			//If zero, exit

		mov			eax, repeat
		cmp			eax, 8
		je			loop16_8		//8 iterations per loop

		cmp			eax, 4
		je			loop16_4		//4 iterations per loop

		cmp			eax, 2
		je			loop16_2		//2 iterations per loop

		cmp			eax, 1
		je			loop16_1		//1 iteration per loop

loop16_8:
		movdqu		16[edi     ], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 16], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 32], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 48], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 64], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 80], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 96], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 112], xmm0		//Move XMM0 into edi
		add			edi, 128
		sub			ecx, 8					//We're going by regCount, not length
		jne			loop16_8
		je			exit

loop16_4:
		movdqu		16[edi     ], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 16], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 32], xmm0		//Move XMM0 into edi
		movdqu		16[edi + 48], xmm0		//Move XMM0 into edi

		add			edi, 64
		sub			ecx, 4					//We're going by regCount, not length
		jne			loop16_4
		je			exit

loop16_2:
		movdqu		[edi     ], xmm0		//Move XMM0 into edi
		movdqu		[edi + 16], xmm0		//Move XMM0 into edi

		add			edi, 32
		sub			ecx, 2					//We're going by regCount, not length
		jne			loop16_2
		je			exit

loop16_1:
		movdqu		[edi     ], xmm0		//Move XMM0 into edi

		add			edi, 16
		sub			ecx, 1					//We're going by regCount, not length
		jne			loop16_1

exit:
		nop
	}
}

//SSE 4.2 required
//TODO: Fix this, doesn't return proper length
int rStr::r_strlen_sse42(const char* src)
{
	int i;
	__asm
	{
		mov		esi, src
		mov		eax, -16
		pxor	xmm0, xmm0

STRLEN_LOOP:
		add		eax, 16
		PcmpIstrI xmm0, 16[esi + eax], 1000b
		jnz		STRLEN_LOOP

		add		eax, 16
		mov		i, eax
	}

	return i;
}

//String length SSE, untested!
int rStr::r_strlen_sse(const char *src)
{
	//Reference http://www.masm32.com/board/index.php?PHPSESSID=a963cea0300afb51f45bdc51f9a5cdc6&topic=12529.0
	__asm
	{
		pop			eax					//trash the return address (still on stack, though)
		pop			eax					//get the src pointer
		pxor		xmm0, xmm0			//zero for comparison
		mov			[esp-12], edx		//edx preserved because Masm32 szLen preserves it
		movups		xmm1, [eax]			//move 16 bytes into xmm1, unaligned (adapted from Lingo/NightWare)
		pcmpeqb		xmm1, xmm0			//set bytes in xmm1 to FF if nullbytes found in xmm1
		mov			edx,eax				//save pointer to string
		pmovmskb	eax, xmm1			//set byte mask in eax
		bsf			eax, eax			//bit scan forward
		jne			Lt16				//less than 16 bytes, we are done
		and			edx, -16			//align initial pointer to 16-byte boundary
		lea			eax, [edx+16]		//aligned pointer + 16 (first 0..15 dealt with by movups above)

@@:	
		pcmpeqb		xmm0, [eax]			//---- inner loop inspired by Lingo, with adaptions -----
		pcmpeqb		xmm1, [eax+16]		//compare packed bytes in [m128] and xmm1 for equality
		lea			eax, [eax+32]		//len counter (moving up lea or add costs 3 cycles for the 191 byte string)
		por			xmm1, xmm0			//or them: one of the mem locations may contain a nullbyte
		pmovmskb	edx, xmm1			//set byte mask in edx
		test		edx, edx
		jz			@B

		sub			eax, [esp-4]		//subtract original src pointer
		shl			edx, 16				//create space for the ecx bytes
		push		ecx					//this push trashes the src pointer (all registers preserved, except eax = return value)
		pmovmskb	ecx, xmm0			//set byte mask in ecx (has to be repeated, sorry)
		or			edx, ecx			//combine xmm0 and xmm1 results
		bsf			edx, edx			//bit scan for the index
		pop			ecx
		lea			eax, [eax+edx-32]	//add scan index

Lt16:
		mov			edx, [esp-12]		//for saving edx, we use a free stack slot; -8 is the ret address
		jmp			dword ptr [esp-2*4]	//ret address, one arg
	}
}
#endif