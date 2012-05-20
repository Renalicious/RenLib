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

#include "CustomTypes.h"

class rMath
{
public:
	static float				sqrt(const float n);						//Square root using SSE2
	static float				rsqrt(const float n);						//Square root using SSE2, fast
	static float				invSqrt(const float n);						//Fast inverse square
	static int					pow(const int n, const int exp);			//Power
	static qword				pow64(const int n, const int exp);			//Power 64bit integer
	static int					pow_base2(const int n, const int exp);		//Binary power
	static int					abs(const int n);
	static float				abs(const float n);

	//Misc stuff
	static float				RoundToInt(const float val);				//Round float to nearest integer
	static int					wrap(const int n, const int min, const int max);		//Wrap out of range number to beween min and max
	static float				wrap(const float n, const float min, const float max);
	static int					invAbs(const int n, const int limit);		//From 0 to limit... 0 + n = limit - n

	//Distance between points (not DOT product)
	static float				dist2d(const float2 &p1, const float2 &p2);
	static float				dist3d(const float3 &p1, const float3 &p2);
	static float				dist_sse(const float4 &p1, const float4 &p2);
	static float				distFast_sse(const float4 &p1, const float4 &p2);

	//DOT product between vectors
	static float				dot2d(const float2 &v1, const float2 &v2);
	static float				dot3d(const float3 &v1, const float3 &v2);
	static float				dot_sse(const float4 &v1, const float4 &v2);

	//Constants
	static const double			PI64;
	static const float			PI;
	static const float			half_PI;
	static const float			two_PI;
	static const float			DegToRad;
	static const float			RadToDeg;
private:
};