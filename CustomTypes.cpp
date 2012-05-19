//---------------------------------------------------------------
// Name: Ren's custom type library
//
// This file contains a few useful types, memory aligned
//
// DOT product is using SSE 'sqrtss' so we don't need to access math.h
// pow() is done through multiplication.
//---------------------------------------------------------------
#include "CustomTypes.h"

//---------------------------------------------------------------
//--- Distance between two points
//---------------------------------------------------------------
float dist2d(const float2 &p1, const float2 &p2)
{
	float Dx, Dy, result;

	Dx = p2.x - p1.x;
	Dy = p2.y - p1.y;

	result = (Dx * Dx) + (Dy * Dy);

	__asm
	{
		sqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

float dist3d(const float3 &p1, const float3 &p2)
{
	float Dx, Dy, Dz, result;

	Dx = p2.x - p1.x;
	Dy = p2.y - p1.y;
	Dz = p2.z - p1.z;

	result = (Dx * Dx) + (Dy * Dy) + (Dz * Dz);

	__asm
	{
		sqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

float dist_sse(const float4 &p1, const float4 &p2)
{
	float result;

#ifdef _SSE3_
	__asm 
	{
		mov			esi, p1
		mov			edi, p2
		movaps		xmm0, [esi]
		movaps		xmm1, [edi]
		subps		xmm0, xmm1
		mulps		xmm0, xmm0

		haddps		xmm0, xmm0
		haddps		xmm0, xmm0

		sqrtss		xmm0, xmm0
		movss		result, xmm0
	}
#else
	__asm
	{
		mov			esi, p1
		mov			edi, p2
		movaps		xmm0, [esi]
		movaps		xmm1, [edi]
		subps		xmm0, xmm1
		mulps		xmm0, xmm0

		pshufd		xmm1, xmm0,	49
		addps		xmm0, xmm1
		pshufd		xmm1, xmm0,	2
		addps		xmm0, xmm1

		sqrtss		xmm0, xmm0
		movss		result, xmm0
	}
#endif
	return result;
}

float distFast_sse(const float4 &p1, const float4 &p2)
{
	float result;

#ifdef _SSE3_
	__asm 
	{
		mov			esi, p1
		mov			edi, p2
		movaps		xmm0, [esi]
		movaps		xmm1, [edi]
		subps		xmm0, xmm1
		mulps		xmm0, xmm0

		haddps		xmm0, xmm0
		haddps		xmm0, xmm0

		rsqrtss		xmm0, xmm0
		movss		result, xmm0
	}
#else
	__asm
	{
		mov			esi, p1
		mov			edi, p2
		movaps		xmm0, [esi]
		movaps		xmm1, [edi]
		subps		xmm0, xmm1
		mulps		xmm0, xmm0

		pshufd		xmm1, xmm0,	49
		addps		xmm0, xmm1
		pshufd		xmm1, xmm0,	2
		addps		xmm0, xmm1

		rsqrtss		xmm0, xmm0
		movss		result, xmm0
	}
#endif
	return result;
}

//---------------------------------------------------------------
//--- DOT product between two vectors
//---------------------------------------------------------------
float dot2d(const float2 &v1, const float2 &v2)
{
	return ( (v1.x * v2.x) + (v1.y + v2.y) );
}

float dot3d(const float3 &v1, const float3 &v2)
{
	return ( (v1.x * v2.x) + (v1.y + v2.y) + (v1.z + v2.z) );
}

float dot_sse( const float4 &v1, const float4 &v2 )
{
	float result;

#ifdef _SSE3_
	__asm
	{
		mov			esi, v1
		mov			edi, v2
		movaps		xmm0, [esi]
		mulps		xmm0, [edi]
		haddps		xmm0, xmm0
		haddps		xmm0, xmm0
		movss		result, xmm0
	}
#else
	__asm
	{
		mov			esi, v1
		mov			edi, v2
		movaps		xmm0, [esi]
		mulps		xmm0, [edi]

		pshufd		xmm1, xmm0,	49
		addps		xmm0, xmm1
		pshufd		xmm1, xmm0,	2
		addps		xmm0, xmm1

		movss		result, xmm0
	}
#endif
	return result;
}