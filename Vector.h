//---------------------------------------------------------------
// Name: Ren's vector library
//
// This file contains 2d and 3d vector classes. It is memory aligned for
// any future assembly optimzation.
//
// DOT product is using SSE 'sqrtss' so we don't need to access math.h
// pow() is done through multiplication.
//
// Inspired by id's library
//---------------------------------------------------------------

#ifndef _RENVECTOR_
#define _RENVECTOR_

#include "Vector.h"
#include "Math.h"
#include "CustomTypes.h"

//---------------------------------------------------------------
//--- Vector 2D
//---------------------------------------------------------------
__declspec(align(16)) class rVec2
{
public:
	float		x;
	float		y;

				rVec2(void);
				rVec2(const rVec2 &newVector);
				rVec2(const float2 &val);
				rVec2(const int2 &val);
				explicit rVec2(const float x, const float y);

	//Get and Set
	void		set(const float x, const float y);
	void		set(const float2 &p1, const float2 &p2); //Create vector from two points
	rVec2		get(void) const;

	//Return data
	float		lengthSqr(void) const;				//Return squared length of vector
	float		length(void) const;					//Return length local vector
	float		dot(const rVec2 &vec) const;		//Return doc product of local and other
	rVec2		truncate(const float max);			//Truncate vector
	float4		asFloat4(void) const;				//Return float4, useful for dot_sse()
	float3		asFloat3(void) const;				//Return float3

	//Opperators
	rVec2		operator-(void) const;				//Same as reverse() + get()

	rVec2		operator-(const rVec2 &vec) const;
	rVec2		operator+(const rVec2 &vec) const;
	float		operator*(const rVec2 &vec) const;
	rVec2		operator/(const rVec2 &vec) const;

	rVec2&		operator-=(const rVec2 &vec);
	rVec2&		operator+=(const rVec2 &vec);
	rVec2&		operator*=(const rVec2 &vec);
	rVec2&		operator/=(const rVec2 &vec);

	rVec2		operator*(const float val) const;
	rVec2		operator/(const float val) const;
	rVec2&		operator*=(const float val);
	rVec2&		operator/=(const float val);

	//Misc
	void		zero(void);							//Zero vector
	void		snap(void);							//Round to closest int	
	void		reverse(void);						//pos -> neg, neg -> pos
	void		swap(void);							//x -> y, y -> x
};

//---------------------------------------------------------------
//--- Vector 3D
//---------------------------------------------------------------
__declspec(align(16)) class rVec3
{
public:
	float		x;
	float		y;
	float		z;

				rVec3(void);
				rVec3(const rVec3 &newVector);
				rVec3(const float3 &val);
				rVec3(const int3 &val);
				explicit rVec3(const float x, const float y, const float z);

	//Get and Set
	void		set(const float x, const float y, const float z);
	void		set(const float3 &p1, const float3 &p2); //Create vector from two points
	rVec3		get(void) const;

	//Return data
	float		lengthSqr(void) const;				//Return squared length of vector
	float		length(void) const;					//Return length local vector
	float		dot(const rVec3 &vec) const;		//Return doc product of local and other
	rVec3		cross(const rVec3 &a) const;		//Return cross product beween local and external
	rVec3&		cross(const rVec3 &a, const rVec3 &b); //Return cross product between two externals
	rVec3		truncate(const float max);			//Truncate vector
	float4		asFloat4(void) const;				//Return float4, useful for dot_sse()
	float3		asFloat3(void) const;				//Return float4, useful for dot_sse()

	//Opperators
	rVec3		operator-(void) const;				//Same as reverse() + get()

	rVec3		operator-(const rVec3 &vec) const;
	rVec3		operator+(const rVec3 &vec) const;
	//rVec3		operator*(const rVec3 &vec) const;
	float		operator*(const rVec3 &vec) const;
	rVec3		operator/(const rVec3 &vec) const;

	rVec3&		operator=(const rVec3 &vec);		//Required?
	rVec3&		operator-=(const rVec3 &vec);
	rVec3&		operator+=(const rVec3 &vec);
	rVec3&		operator*=(const rVec3 &vec);
	rVec3&		operator/=(const rVec3 &vec);

	rVec3		operator*(const float val) const;
	rVec3		operator/(const float val) const;
	rVec3&		operator*=(const float val);
	rVec3&		operator/=(const float val);

	friend rVec3		operator*(const float val, const rVec3 &vec);

	//Misc
	void		zero(void);							//Zero vector
	void		snap(void);							//Round to closest int	
	void		reverse(void);						//pos -> neg, neg -> pos
};

#endif /* _RENVECTOR_ */