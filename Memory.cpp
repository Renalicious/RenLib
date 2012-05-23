

#include "Memory.h"

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- Regular loops, 1 value itteration, arbitraty len
//--------------------------------------------------------------------
void rMem::r_memset_arb(char* dst, char c, int len)
{
	while(len && c)
	{
		dst[len] = c;
		len--;
	}
}

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- Unrolled loop, 16 values per itteration
//--------------------------------------------------------------------
void rMem::r_memset_16b(char* dst, char c, int len)
{
	if(len % 16 == 0)
	{
		while(len && c)
		{
			dst[len - 0] = c; dst[len - 1] = c; dst[len - 2] = c; dst[len - 3] = c;
			dst[len - 4] = c; dst[len - 5] = c; dst[len - 6] = c; dst[len - 7] = c;
			dst[len - 8] = c; dst[len - 9] = c; dst[len -10] = c; dst[len -11] = c;
			dst[len -12] = c; dst[len -13] = c; dst[len -14] = c; dst[len -15] = c;
			len -= 16;
		}
	}
}

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- Unrolled loop, 64 values per itteration
//--------------------------------------------------------------------
void rMem::r_memset_64b(char* dst, char c, int len)
{
	if(len % 64 == 0)
	{
		while(len && c)
		{
			dst[len - 0] = c; dst[len - 1] = c; dst[len - 2] = c; dst[len - 3] = c;
			dst[len - 4] = c; dst[len - 5] = c; dst[len - 6] = c; dst[len - 7] = c;
			dst[len - 8] = c; dst[len - 9] = c; dst[len -10] = c; dst[len -11] = c;
			dst[len -12] = c; dst[len -13] = c; dst[len -14] = c; dst[len -15] = c;
			
			dst[len -16] = c; dst[len -17] = c; dst[len -18] = c; dst[len -19] = c;
			dst[len -20] = c; dst[len -21] = c; dst[len -22] = c; dst[len -23] = c;
			dst[len -24] = c; dst[len -25] = c; dst[len -26] = c; dst[len -27] = c;
			dst[len -28] = c; dst[len -29] = c; dst[len -30] = c; dst[len -31] = c;
			
			dst[len -32] = c; dst[len -33] = c; dst[len -34] = c; dst[len -35] = c;
			dst[len -36] = c; dst[len -37] = c; dst[len -38] = c; dst[len -39] = c;
			dst[len -40] = c; dst[len -41] = c; dst[len -42] = c; dst[len -43] = c;
			dst[len -44] = c; dst[len -45] = c; dst[len -46] = c; dst[len -47] = c;
			
			dst[len -48] = c; dst[len -49] = c; dst[len -50] = c; dst[len -51] = c;
			dst[len -52] = c; dst[len -53] = c; dst[len -54] = c; dst[len -55] = c;
			dst[len -56] = c; dst[len -57] = c; dst[len -58] = c; dst[len -59] = c;
			dst[len -60] = c; dst[len -61] = c; dst[len -62] = c; dst[len -63] = c;
			len -= 64;
		}
	}
}

//--------------------------------------------------------------------
//--- Copy memory from 'src' to 'dst'
//--- Regular loops, 1 value itteration, arbitraty len
//--------------------------------------------------------------------
void rMem::r_memcpy_arb(char* dst, char* src, int len)
{
	while(len)
	{
		dst[len] = src[len];
		len--;
	}
}

//--------------------------------------------------------------------
//--- Copy memory from 'src' to 'dst'
//--- Unrolled loop, 16 values per itteration
//--------------------------------------------------------------------
void rMem::r_memcpy_16b(char* dst, char* src, int len)
{
	if(len % 16 == 0)
	{
		while(len)
		{
			dst[len - 0] = src[len - 0]; dst[len - 1] = src[len - 1]; dst[len - 2] = src[len - 2]; dst[len - 3] = src[len - 3]; 
			dst[len - 4] = src[len - 4]; dst[len - 5] = src[len - 5]; dst[len - 6] = src[len - 6]; dst[len - 7] = src[len - 7]; 
			dst[len - 8] = src[len - 8]; dst[len - 9] = src[len - 9]; dst[len - 10] = src[len - 10]; dst[len - 11] = src[len - 11]; 
			dst[len - 12] = src[len - 12]; dst[len - 13] = src[len - 13]; dst[len - 14] = src[len - 14]; dst[len - 15] = src[len - 15];

			len -= 16;
		}
	}
}

//--------------------------------------------------------------------
//--- Copy memory from 'src' to 'dst'
//--- Unrolled loop, 64 values per itteration
//--------------------------------------------------------------------
void rMem::r_memcpy_64b(char* dst, char* src, int len)
{
	if(len % 64 == 0)
	{
		while(len)
		{
			dst[len - 0] = src[len - 0]; dst[len - 1] = src[len - 1]; dst[len - 2] = src[len - 2]; dst[len - 3] = src[len - 3]; 
			dst[len - 4] = src[len - 4]; dst[len - 5] = src[len - 5]; dst[len - 6] = src[len - 6]; dst[len - 7] = src[len - 7]; 
			dst[len - 8] = src[len - 8]; dst[len - 9] = src[len - 9]; dst[len - 10] = src[len - 10]; dst[len - 11] = src[len - 11]; 
			dst[len - 12] = src[len - 12]; dst[len - 13] = src[len - 13]; dst[len - 14] = src[len - 14]; dst[len - 15] = src[len - 15]; 
			
			dst[len - 16] = src[len - 16]; dst[len - 17] = src[len - 17]; dst[len - 18] = src[len - 18]; dst[len - 19] = src[len - 19]; 
			dst[len - 20] = src[len - 20]; dst[len - 21] = src[len - 21]; dst[len - 22] = src[len - 22]; dst[len - 23] = src[len - 23]; 
			dst[len - 24] = src[len - 24]; dst[len - 25] = src[len - 25]; dst[len - 26] = src[len - 26]; dst[len - 27] = src[len - 27]; 
			dst[len - 28] = src[len - 28]; dst[len - 29] = src[len - 29]; dst[len - 30] = src[len - 30]; dst[len - 31] = src[len - 31]; 
			
			dst[len - 32] = src[len - 32]; dst[len - 33] = src[len - 33]; dst[len - 34] = src[len - 34]; dst[len - 35] = src[len - 35]; 
			dst[len - 36] = src[len - 36]; dst[len - 37] = src[len - 37]; dst[len - 38] = src[len - 38]; dst[len - 39] = src[len - 39]; 
			dst[len - 40] = src[len - 40]; dst[len - 41] = src[len - 41]; dst[len - 42] = src[len - 42]; dst[len - 43] = src[len - 43]; 
			dst[len - 44] = src[len - 44]; dst[len - 45] = src[len - 45]; dst[len - 46] = src[len - 46]; dst[len - 47] = src[len - 47];
			
			dst[len - 48] = src[len - 48]; dst[len - 49] = src[len - 49]; dst[len - 50] = src[len - 50]; dst[len - 51] = src[len - 51]; 
			dst[len - 52] = src[len - 52]; dst[len - 53] = src[len - 53]; dst[len - 54] = src[len - 54]; dst[len - 55] = src[len - 55]; 
			dst[len - 56] = src[len - 56]; dst[len - 57] = src[len - 57]; dst[len - 58] = src[len - 58]; dst[len - 59] = src[len - 59]; 
			dst[len - 60] = src[len - 60]; dst[len - 61] = src[len - 61]; dst[len - 62] = src[len - 62]; dst[len - 63] = src[len - 63]; 
			len -= 64;
		}
	}
}





//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- Does either 1 byte per itteration, or 16 depending on modulus
//--- Not SIMD
//--------------------------------------------------------------------
void rMem::r_memset_asm(void* dst, char c, int len)
{
	if(len % 16 == 0)
	{
		__asm
		{
			mov		edi, dst		//Move dest pointer to edi
			mov		al, c			//Move character into lower EAX
			mov		ecx, len		//Move length into ecx

			cmp		ecx, 0			//See if length is zero
			jz		exit16			//If zero, exit

	loop16:
			mov		[edi   ], al		//Move character into edi
			mov		[edi+ 1], al		//Move character into edi
			mov		[edi+ 2], al		//Move character into edi
			mov		[edi+ 3], al		//Move character into edi
			mov		[edi+ 4], al		//Move character into edi
			mov		[edi+ 5], al		//Move character into edi
			mov		[edi+ 6], al		//Move character into edi
			mov		[edi+ 7], al		//Move character into edi
			mov		[edi+ 8], al		//Move character into edi
			mov		[edi+ 9], al		//Move character into edi
			mov		[edi+10], al		//Move character into edi
			mov		[edi+11], al		//Move character into edi
			mov		[edi+12], al		//Move character into edi
			mov		[edi+13], al		//Move character into edi
			mov		[edi+14], al		//Move character into edi
			mov		[edi+15], al		//Move character into edi
			add		edi, 16			//Increment pointer by 1 byte
			sub		ecx, 16			//Decrement pointer by 1
			jne		loop16			//Check is counter is 0

	exit16:
			nop
		}
	}
	else
	{
		__asm
		{
			mov		edi, dst		//Move dest pointer to edi
			mov		al, c			//Move character into lower EAX
			mov		ecx, len		//Move length into ecx

			cmp		ecx, 0			//See if length is zero
			jz		exit1			//If zero, exit

	loop1:
			mov		[edi], al		//Move character into edi
			inc		edi				//Increment pointer by 1 byte
			dec		ecx				//Decrement pointer by 1
			jne		loop1			//Check is counter is 0

	exit1:
			nop
		}
	}
}

//--------------------------------------------------------------------
//--- Copy memory of 'src' into 'dst'
//--- Does either 1 byte per itteration, or 16 depending on modulus
//--- Not SIMD
//--------------------------------------------------------------------
void rMem::r_memcpy_asm(void* dst, void* src, int len)
{
	if(len % 16 == 0)
	{
		__asm
		{
			mov		edi, dst		//Move dest pointer to edi
			mov		esi, src		//Move source pointer to edi
			mov		ecx, len		//Move length into ecx

			cmp		ecx, 0			//See if length is zero
			jz		exit16			//If zero, exit

	loop16:
			mov		eax, [esi   ]		//Move src into eax
			mov		[edi   ], eax		//Move eax into dst
			mov		eax, [esi +1]
			mov		[edi +1], eax
			mov		eax, [esi +2]
			mov		[edi +2], eax
			mov		eax, [esi +3]
			mov		[edi +3], eax
			mov		eax, [esi +4]
			mov		[edi +5], eax
			mov		eax, [esi +5]
			mov		[edi +5], eax
			mov		eax, [esi +6]
			mov		[edi +6], eax
			mov		eax, [esi +7]
			mov		[edi +7], eax
			mov		eax, [esi +8]
			mov		[edi +8], eax
			mov		eax, [esi +9]
			mov		[edi +9], eax
			mov		eax, [esi+10]
			mov		[edi+10], eax
			mov		eax, [esi+11]
			mov		[edi+11], eax
			mov		eax, [esi+12]
			mov		[edi+12], eax
			mov		eax, [esi+13]
			mov		[edi+13], eax
			mov		eax, [esi+14]
			mov		[edi+14], eax
			mov		eax, [esi+15]
			mov		[edi+15], eax
			add		edi, 16			//Increment pointer by 16 byte
			add		esi, 16			//Increment pointer by 16 byte
			sub		ecx, 16			//Decrement pointer by 16
			jne		loop16			//Check is counter is 0

	exit16:
			nop
		}
	}
	else
	{
	__asm
		{
			mov		edi, dst		//Move dest pointer to edi
			mov		esi, src		//Move source pointer to edi
			mov		ecx, len		//Move length into ecx

			cmp		ecx, 0			//See if length is zero
			jz		exit1			//If zero, exit

	loop1:
			mov		eax, [esi   ]	//Move src into eax
			mov		[edi   ], eax	//Move eax into dst
			inc		edi				//Increment pointer by 1 byte
			inc		esi				//Increment pointer by 1 byte
			dec		ecx				//Decrement pointer by 1
			jne		loop1			//Check is counter is 0

	exit1:
			nop
		}
	}
}

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- SSE, faster than scalar assembly for orderd data sets larger than 3MB
//--------------------------------------------------------------------
void rMem::r_memset_sse4K(void* dst, char c, int len)
{
	_r128i dat;
	dat.bytes[0] = c;
	dat.bytes[1] = dat.bytes[0];
	dat.words[1] = dat.words[0];
	dat.dwords[1] = dat.dwords[0];
	dat.qwords[1] = dat.qwords[0];
	
	__asm
	{	
		mov		edi, dst
		mov		eax, len
		movaps	xmm1, dat

		pxor	xmm0,xmm0
		shr		eax,12				;divide by 4096, one page len.
		
	align 16
	outer:
		prefetchnta	[edi+4096]
		mov		edx,4096/16			;we handle 4096 bytes per inner loop, each MOVAPS handle 16 of those bytes.

	align 16
	inner:
			movaps	[edi],xmm1
			movaps	[edi+16],xmm1
			movaps	[edi+32],xmm1
			movaps	[edi+48],xmm1
			add		edi,16*4
			sub		edx,1*4
			jnz		inner
			
		sub		eax,1
		jnz		outer

		ret
	}
}

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- SSE, faster than scalar assembly for orderd data sets larger than 3MB
//--------------------------------------------------------------------
void rMem::r_memcpy_sse4K(void* dst, void* src, int len)
{	
	__asm
	{	
		mov		edi, dst
		mov		esi, src
		mov		eax, len

		pxor	xmm0,xmm0
		shr		eax,12				;divide by 4096, one page len.
		
	align 16
	outer:
		prefetchnta	[edi+4096]
		prefetchnta	[esi+4096]
		mov		edx,4096/16			;we handle 4096 bytes per inner loop, each MOVAPS handle 16 of those bytes.

	align 16
	inner:
			movaps	xmm1, [esi]
			movaps	[edi], xmm1
			movaps	xmm1, [esi+16]
			movaps	[edi+16], xmm1
			movaps	xmm1, [esi+32]
			movaps	[edi+32], xmm1
			movaps	xmm1, [esi+48]
			movaps	[edi+48], xmm1

			add		edi, 16*4
			add		esi, 16*4
			sub		edx, 1*4
			jnz		inner
			
		sub		eax,1
		jnz		outer

		ret
	}
}

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- AVX, faster than scalar assembly for orderd data sets larger than 3MB
//--------------------------------------------------------------------
void rMem::r_memset_avx4K(void* dst, char c, int len)
{
	__declspec(align(16)) union _r256i
	{
		byte	bytes[32];
		word	words[16];
		dword	dwords[8];
		qword	qwords[4];
	};
	
	_r256i dat;
	dat.bytes[0] = c;
	dat.bytes[1] = dat.bytes[0];
	dat.words[1] = dat.words[0];
	dat.dwords[1] = dat.dwords[0];
	dat.qwords[1] = dat.qwords[0];
	dat.qwords[2] = dat.qwords[0];
	dat.qwords[3] = dat.qwords[0];
	
	__asm
	{	
		mov		edi, dst
		mov		eax, len
		movaps	ymm1, dat

		pxor	ymm0,ymm0
		shr		eax,12				;divide by 4096, one page len.
		
	align 16
	outer:
		prefetchnta	[edi+4096]
		mov		edx,4096/32			;we handle 4096 bytes per inner loop, each MOVAPS handle 32 of those bytes.

	align 16
	inner:
			movaps	[edi],ymm1
			movaps	[edi+32],ymm1
			movaps	[edi+64],ymm1
			movaps	[edi+96],ymm1
			add		edi,32*4
			sub		edx,1*4
			jnz		inner
			
		sub		eax,1
		jnz		outer

		ret
	}
}

//--------------------------------------------------------------------
//--- Set memory of 'dst' to 'c'
//--- AVX, faster than scalar assembly for orderd data sets larger than 3MB
//--------------------------------------------------------------------
void rMem::r_memcpy_avx4K(void* dst, void* src, int len)
{	
	__asm
	{	
		mov		edi, dst
		mov		esi, src
		mov		eax, len

		pxor	ymm0,ymm0
		shr		eax,12				;divide by 4096, one page len.
		
	align 16
	outer:
		prefetchnta	[edi+4096]
		prefetchnta	[esi+4096]
		mov		edx,4096/32			;we handle 4096 bytes per inner loop, each MOVAPS handle 16 of those bytes.

	align 16
	inner:
			movaps	ymm1, [esi]
			movaps	[edi], ymm1
			movaps	ymm1, [esi+32]
			movaps	[edi+32], ymm1
			movaps	ymm1, [esi+64]
			movaps	[edi+64], ymm1
			movaps	ymm1, [esi+96]
			movaps	[edi+96], ymm1

			add		edi, 32*4
			add		esi, 32*4
			sub		edx, 1*4
			jnz		inner
			
		sub		eax,1
		jnz		outer

		ret
	}
}