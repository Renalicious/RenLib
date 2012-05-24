//---------------------------------------------------------------
// Name: Ren's memory library
//
// This file contains simple assembly optimized memory funcitons
// similar to what rStr has, but global.
//---------------------------------------------------------------

#include "CustomTypes.h"

#ifndef _RENMEM_
#define _RENMEM_

class rMem
{
public:
	//--- Mundane loops
	static void		r_memset_arb(char* dst, char c, int size);
	static void		r_memset_16b(char* dst, char c, int size);
	static void		r_memset_64b(char* dst, char c, int size);
	
	static void		r_memcpy_arb(char* dst, char* src, int size);
	static void		r_memcpy_16b(char* dst, char* src, int size);
	static void		r_memcpy_64b(char* dst, char* src, int size);
	
	//--- Scalar assembly
	static void		r_memset_asm(void* dst, char c, int size);
	static void		r_memcpy_asm(void* dst, void* src, int size);
	
	//--- SSE assembly
	static void		r_memzero_sse4K(void* dst, int size);
	static void		r_memset_sse4K(void* dst, char c, int size);
	static void		r_memcpy_sse4K(void* dst, void* src, int size);
	
	//--- AVX assembly
	static void		r_memzero_avx4K(void* dst, int size);
	static void		r_memset_avx4K(void* dst, char c, int size);
	static void		r_memcpy_avx4K(void* dst, void* src, int size);
};

#endif /* _RENMEM */