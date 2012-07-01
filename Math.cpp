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

float rMath::exp(const float n)
{
	return pow(E, n);
}

float rMath::pow(const float base, const float exp)
{
	float result, exponent, b;
	
	//--- Assembly can't do negative exponents or bases, so make it positive
	exponent = rMath::abs(exp);
	b = rMath::abs(base);
	
	//--- Return 1 if exponent is 0
	if(exponent == 0)
		return 1.0f;
	
	_asm
	{
		fld [exponent]
		fld [b]
		fyl2x
		fld1
		fld st(1)
		fprem
		f2xm1
		faddp st(1),st(0)
		fscale;
		fxch;
		fstp st(0)
		fstp [result]
	}
	
	//--- If exponent is negative, return the inverse
	if(exp < 0)
	{
		if(base < 0)
			return -1 / result;
		else
			return 1 / result;
	}
	
	else
	{
		if(base < 0)
			return -result;
		else
			return result;
	}
}

int rMath::pow(const int n, const int exp)
{
	int result, exponent;

	for(result = n, exponent = rMath::abs(exp); exponent > 1; exponent--)
		result *= n;
	
	if(exp > 0)
		return result;
		
	else if (exp < 0)
		return 0;
		
	else
		return 1;
}

qword rMath::pow64(const int n, const int exp)
{
	qword result;
	int exponent;

	for(result = n, exponent = rMath::abs(exp); exponent > 1; exponent--)
		result *= n;
	
	if(exp > 0)
		return result;
		
	else if (exp < 0)
		return 0;
		
	else
		return 1;
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

//Fast log10, using log2(n) / log2(10)
float rMath::log10Fast(const float n)
{
	return rMath::log2Fast(n) / 3.321928094887362f;
}

//Fast log2 of a float
float rMath::log2Fast(const float val)
{
	float temp = val;

	int * const  exp_ptr = reinterpret_cast <int *> (&temp);
	int          x = *exp_ptr;
	const int    log_2 = ((x >> 23) & 255) - 128;
	x &= ~(255 << 23);
	x += 127 << 23;
	*exp_ptr = x;

	return temp + log_2;
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
	return limit - n;
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

float rMath::pointsTheta3d(const float3 &start, const float3 &p1, const float3 &p2)
{
	//--- Variables
	float4 s, e1, e2;
	float theta;

	s.x = s.y = s.z = s.w = 0.0f;
	e1 = e2 = s;

	s.x = start.x; s.y = start.y; s.z = start.z;
	e1.x = p1.x; e1.y = p1.y; e1.z = p1.z;
	e2.x = p2.x; e2.y = p2.y; e2.z = p2.z;

	//--- Get vectors 1 and 2
	__asm
	{
		//Move variables into registers
		movaps	xmm1, s
		movaps	xmm2, xmm1
		//movaps	xmm1, e1
		//movaps	xmm2, e2

		//Create v1 and v2
		subps	xmm1, e1
		subps	xmm2, e2

		//DOT from v1 and v2 (store in xmm6)
		movaps	xmm5, xmm1 //make a copy to not destroy vector
		mulps	xmm5, xmm2

		movaps	xmm6, xmm5
		haddps	xmm6, xmm6
		haddps	xmm6, xmm6

		//LENGTH from v1 and v2 (store in xmm7)
		//sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z)) * sqrt((b.x * b.x) + (b.y * b.y) + (b.z * b.z))
		mulps	xmm1, xmm1
		mulps	xmm2, xmm2

		haddps	xmm1, xmm1
		haddps	xmm1, xmm1

		haddps	xmm2, xmm2
		haddps	xmm2, xmm2

		sqrtss	xmm1, xmm1
		sqrtss	xmm2, xmm2

		mulss	xmm1, xmm2
		movss	xmm7, xmm1

		//Compute theta
		divss	xmm6, xmm7
		movss	theta, xmm6
	}

	//--- Return
	return theta;
}

float rMath::vectorTheta3d(const float3 &a, const float3 &b)
{
	float dot, length, theta;

	dot = a.x * b.x + a.y * b.y + a.z * b.z;
	length = sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z)) * sqrt((b.x * b.x) + (b.y * b.y) + (b.z * b.z));
	theta = dot / length;

	return theta;
}

//Constants
const double rMath::E64 = 2.718281828459045235360287;
const float rMath::E = 2.718281828459045235360287f;
const double rMath::PI64 = 3.1415926535897932384626433832795028841971693993751058;
const float rMath::PI = 3.1415926535897932384626433832795028841971693993751058f;

const float rMath::half_PI = PI * 0.5f;
const float rMath::two_PI = PI * 2.0f;
const float rMath::DegToRad = 180.0f / PI;
const float rMath::RadToDeg = PI / 180.0f;