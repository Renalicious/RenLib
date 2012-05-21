//---------------------------------------------------------------
// Name: Ren's math library
//
// This contains various simple math functions and definitions
// so as to not rely on STL. Not that STL's math implementation is slow,
// but this will be a great learning experience!
//
// Thankfully we live in a time of the internet and open source,
// so no shortage of examples on how to do things.
//
// This will use MMX, SSE1 - 4 where possible
//---------------------------------------------------------------

#include "Math.h"

//Accurate square root
float rMath::sqrt(const float num)
{
	__asm
	{
		sqrtss		xmm0, num
		movss		num, xmm0
	}

	return num;
}

//Fast square root
float rMath::rsqrt(const float num)
{
	__asm
	{
		rsqrtss		xmm0, num
		movss		num, xmm0
	}

	return num;
}

//Fast inverse squareroot
//Citation: http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root/
float rMath::invSqrt(const float x)
{
	dword i = 0x5F1F1412 - (*(dword*)&x >> 1);
	float tmp = *(float*)&i;
	return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
}

int rMath::pow(const int n, const int exp)
{
	int result, exponent;

	for(result = n, exponent = exp; exp > 1; exponent--)
		result *= n;
	
	return result;
}

qword rMath::pow64(const int n, const int exp)
{
	qword result;
	int exponent;

	for(result = n, exponent = exp; exp > 1; exponent--)
		result *= n;
	
	return result;
}

int rMath::pow_base2(const int n, const int exp)
{
	if(exp > 0)
		return n << exp;

	else if(exp < 0)
		return n >> exp;

	else
		return n;
}

//V1.6 log(x) approximation by TomC, helped: bootsy,infuzion
float rMath::log10(const float x)
{
	float y;
	float log=.4342945;
	float F1=1;
	float F2=2;
	float FP33=.3333333;
	float FP2=.2;

	__asm
	{
		movaps	xmm0,x;
		movaps	xmm1,F1;
		movaps	xmm2,xmm0;
		subps	xmm0,xmm1;
		addps	xmm1,xmm2;
		divps	xmm0,xmm1;
		movaps	xmm1,xmm0;
		mulps	xmm1,xmm0;
		mulps	xmm0,F2;
		movaps	xmm2,xmm1;
		mulps	xmm2,FP33;
		mulps	xmm1,xmm1;
		mulps	xmm1,FP2;
		addps	xmm1,xmm2;
		addps	xmm1,F1;
		mulps	xmm1,xmm0;
		mulps	xmm1,log;
		movaps	y,xmm1;
	}
	return y;
}

int rMath::abs(const int n)
{
	if(n < 0)
		return n * -1;
	else
		return n;
}

float rMath::abs(const float n)
{
	if(n < 0)
		return n * -1;
	else
		return n;
}

float rMath::wrap(const float n, const float min, const float max)
{
	float spread, result;

	spread = abs(min) + abs(max);
	result = n;

	while(result < min || result > max)
	{
		if(result < min)
			result += spread;

		else if(result > max)
			result -= spread;
	}

	return result;
}

int rMath::wrap(const int n, const int min, const int max)
{
	int spread, result;

	spread = abs(min) + abs(max);
	result = n;

	while(result < min || result > max)
	{
		if(result < min)
			result += spread;

		else if(result > max)
			result -= spread;
	}

	return result;
}

float rMath::RoundToInt(const float val)
{
	int result;
	_asm
	{
		fld val;
		fistp result;
	}
	return (float)result;
}

int rMath::invAbs(const int n, const int limit)
{
	return abs(limit - n);
}

//---------------------------------------------------------------
//--- Distance between two points
//---------------------------------------------------------------
float rMath::dist2d(const float2 &p1, const float2 &p2)
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

float rMath::dist3d(const float3 &p1, const float3 &p2)
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

float rMath::dist_sse(const float4 &p1, const float4 &p2)
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

float rMath::distFast_sse(const float4 &p1, const float4 &p2)
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
float rMath::dot2d(const float2 &v1, const float2 &v2)
{
	return ( (v1.x * v2.x) + (v1.y + v2.y) );
}

float rMath::dot3d(const float3 &v1, const float3 &v2)
{
	return ( (v1.x * v2.x) + (v1.y + v2.y) + (v1.z + v2.z) );
}

float rMath::dot_sse( const float4 &v1, const float4 &v2 )
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

//Constants
const double rMath::PI64 = 3.1415926535897932384626433832795028841971693993751058;
const float rMath::PI = 3.1415926535897932384626433832795028841971693993751058f;

const float rMath::half_PI = PI * 0.5f;
const float rMath::two_PI = PI * 2.0f;
const float rMath::DegToRad = 180.0f / PI;
const float rMath::RadToDeg = PI / 180.0f;