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
//--- Position math
//---------------------------------------------------------------
float dot2(const float2 &p1, const float2 &p2)
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

float dot3(const float3 &p1, const float3 &p2)
{
#ifdef _SSE2_
	
	//I wonder how much recasting float3 into float4 slows things down
	float result = 0.0f;
	float4 p2a = {0.0f, 0.0f, 0.0f, 0.0f};
	float4 p1a = {0.0f, 0.0f, 0.0f, 0.0f};

	p2a.w = 0.0f;
	p2a.x = p2.x;
	p2a.y = p2.y;
	p2a.z = p2.z;

	p1a.w = 0.0f;
	p1a.x = p1.x;
	p1a.y = p1.y;
	p1a.z = p1.z;

	__asm
	{
		movups		xmm1, p2a		//[-][p2.x][p2.y][p2.z] <-- p2
		movups		xmm2, p1a		//[-][p1.x][p1.y][p1.z] <-- p1
		subps		xmm1, xmm2		//[-][ Dx ][ Dy ][ Dz ] = [-][p2.x][p2.y][p2.z] - [-][p1.x][p1.y][p1.z]
		mulps		xmm1, xmm1		//[-][Dx^2][Dy^2][Dz^2] = [-][ Dx ][ Dy ][ Dz ] * [-][ Dx ][ Dy ][ Dz ]

#ifdef _SSE3_
		haddps		xmm1, xmm1		//[-][Dx^2][Dy^2 + Dz^2] <-- [-][Dx^2][Dy^2][Dz^2]
		haddps		xmm1, xmm1		//[-][-][Dx^2 + Dy^2 + Dz^2] <-- [-][Dx^2][Dy^2 + Dz^2]
#else
		pshufd		xmm0,xmm1,	49//0b00110001	//1->0, 3->2
		addps		xmm1,xmm0				//0+1, xx, 2+3, xx
		pshufd		xmm0,xmm1,	2//0b00000010	//2->0
		addps		xmm1,xmm0		
#endif

		sqrtss		xmm1, xmm1		//Sqrt [][][][Dx^2 + Dy^2 + Dz^2]
		movss		result, xmm1	//result <-- [][][][Sqrt(Dx^2 + Dy^2 + Dz^2)]
	}
	return result;
#else

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

#endif
}

float dot2Fast(const float2 &p1, const float2 &p2)
{
	float Dx, Dy, result;

	Dx = p2.x - p1.x;
	Dy = p2.y - p1.y;

	result = (Dx * Dx) + (Dy * Dy);

	__asm
	{
		rsqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

float dot3Fast(const float3 &p1, const float3 &p2)
{
	float Dx, Dy, Dz, result;

	Dx = p2.x - p1.x;
	Dy = p2.y - p1.y;
	Dz = p2.z - p1.z;

	result = (Dx * Dx) + (Dy * Dy) + (Dz * Dz);

	__asm
	{
		rsqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}