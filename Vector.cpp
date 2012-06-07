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
#include "Vector.h"

//---------------------------------------------------------------
//--- 2D vector class
//---------------------------------------------------------------
rVec2::rVec2(void)
{
	x = 0.0f;
	y = 0.0f;
}

rVec2::rVec2(const rVec2 &newVector)
{
	x = newVector.x;
	y = newVector.y;
}

rVec2::rVec2(const float2 &val)
{
	x = val.x;
	y = val.y;
}

rVec2::rVec2(const int2 &val)
{
	x = (float)val.x;
	y = (float)val.y;
}

rVec2::rVec2(const float newX, const float newY)
{
	x = newX;
	y = newY;
}

//Set and Get
void rVec2::set(const float newX, const float newY)
{
	x = newX;
	y = newY;
}

void rVec2::set(const float2 &p1, const float2 &p2)
{
	//Vector from two points
	x = p2.x - p1.x;
	y = p2.y - p1.y;
}

rVec2 rVec2::get(void) const
{
	return *this;
}

//Return data
float rVec2::lengthSqr(void) const
{
	//This is basically a dot product of itself
	return (x*x + y*y);
}

float rVec2::length(void) const
{
	return rMath::sqrt(x*x + y*y);
}

float rVec2::dot(const rVec2 &vec) const
{
	return ( (x * vec.x) + (y * vec.y) );
}

//Basically using id's method
rVec2 rVec2::truncate(const float max)
{
	float adjust, vecLen;

	if(!max)
	{
		zero();
	}
	else
	{
		vecLen = lengthSqr();
		if( vecLen > (max * max) )
		{
			adjust = max * rMath::invSqrt(vecLen);

			x *= adjust;
			y *= adjust;
		}
	}

	return *this;
}

float4 rVec2::asFloat4(void) const
{
	float4 res = {x, y, 0.0f, 0.0f};
	return res;
}

float3 rVec2::asFloat3(void) const
{
	float3 res = {x, y, 0.0f};
	return res;
}

//Misc
void rVec2::zero(void)
{
	x = y = 0.0f;
}
void rVec2::snap(void)
{
	x = rMath::RoundToInt(x);
	y = rMath::RoundToInt(y);
}
void rVec2::reverse(void)
{
	x *= -1;
	y *= -1;
}
void rVec2::swap(void)
{
	float temp;

	temp = x;
	x = y;
	y = temp;
}

//Operators
rVec2 rVec2::operator-(void) const
{
	return rVec2(-x, -y);
}


rVec2 rVec2::operator-(const rVec2 &vec) const
{
	return rVec2(x - vec.x, y - vec.y);
}

rVec2 rVec2::operator+(const rVec2 &vec) const
{
	return rVec2(x + vec.x, y + vec.y);
}

rVec2 operator*(const float val, const rVec2 &vec)
{
	return rVec2(val * vec.x, val * vec.y);
}

float rVec2::operator*(const rVec2 &vec) const
{
	return x * vec.x + y * vec.y;
}

rVec2 rVec2::operator/(const rVec2 &vec) const
{
	return rVec2(x / vec.x, y / vec.y);
}

rVec2& rVec2::operator-=(const rVec2 &vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

rVec2& rVec2::operator+=(const rVec2 &vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

rVec2& rVec2::operator*=(const rVec2 &vec)
{
	x *= vec.x;
	y *= vec.y;

	return *this;
}

rVec2& rVec2::operator/=(const rVec2 &vec)
{
	x /= vec.x;
	y /= vec.y;

	return *this;
}

rVec2 rVec2::operator*(const float val) const
{
	return rVec2(x * val, y * val);
}

rVec2 rVec2::operator/(const float val) const
{
	return rVec2(x / val, y / val);
}

rVec2& rVec2::operator*=(const float val)
{
	x *= val;
	y *= val;

	return *this;
}

rVec2& rVec2::operator/=(const float val)
{
	x /= val;
	y /= val;

	return *this;
}

//---------------------------------------------------------------
//--- 3D vector class
//---------------------------------------------------------------
rVec3::rVec3(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

rVec3::rVec3(const rVec3 &newVector)
{
	x = newVector.x;
	y = newVector.y;
	z = newVector.z;
}

rVec3::rVec3(const float3 &val)
{
	x = val.x;
	y = val.y;
	z = val.z;
}

rVec3::rVec3(const int3 &val)
{
	x = (float)val.x;
	y = (float)val.y;
	z = (float)val.z;
}

rVec3::rVec3(const float newX, const float newY, const float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

//Set and Get
void rVec3::set(const float newX, const float newY, const float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

void rVec3::set(const float3 &p1, const float3 &p2)
{
	//Vector from two points
	x = p2.x - p1.x;
	y = p2.y - p1.y;
	z = p2.z - p1.z;
}

rVec3 rVec3::get(void) const
{
	return *this;
}

//Return data
float rVec3::lengthSqr(void) const
{
	//This is basically a dot product of itself
	return (x*x + y*y + z*z);
}

float rVec3::length(void) const
{
	return rMath::sqrt(x*x + y*y + z*z);
}

float rVec3::dot(const rVec3 &vec) const
{
	return ( (x * vec.x) + (y * vec.y) + (z * vec.z) );
}

rVec3 rVec3::cross(const rVec3 &a) const
{
	return rVec3( y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x );
}

rVec3& rVec3::cross(const rVec3 &a, const rVec3 &b)
{
	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;

	return *this;
}

rVec3 rVec3::truncate(const float max)
{
	float adjust, vecLen;

	if(!max)
	{
		zero();
	}
	else
	{
		vecLen = lengthSqr();
		if( vecLen > (max * max) )
		{
			adjust = max * rMath::invSqrt(vecLen);

			x *= adjust;
			y *= adjust;
			z *= adjust;
		}
	}

	return *this;
}

float4 rVec3::asFloat4(void) const
{
	float4 res = {x, y, z, 0.0f};
	return res;
}

float3 rVec3::asFloat3(void) const
{
	float3 res = {x, y, z};
	return res;
}

//Misc
void rVec3::zero(void)
{
	x = y = z = 0.0f;
}
void rVec3::snap(void)
{
	x = rMath::RoundToInt(x);
	y = rMath::RoundToInt(y);
	z = rMath::RoundToInt(z);
}
void rVec3::reverse(void)
{
	x *= -1;
	y *= -1;
	z *= -1;
}

//Operators
rVec3 rVec3::operator-(void) const
{
	return rVec3(-x, -y, -z);
}

rVec3 rVec3::operator-(const rVec3 &vec) const
{
	return rVec3(x - vec.x, y - vec.y, z - vec.z);
}

rVec3 rVec3::operator+(const rVec3 &vec) const
{
	return rVec3(x + vec.x, y + vec.y, z + vec.z);
}

//This is apparently also needed
rVec3 operator*(const float val, const rVec3 &vec)
{
	return rVec3(val * vec.x, val * vec.y, val * vec.z);
}

float rVec3::operator*(const rVec3 &vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

rVec3 rVec3::operator/(const rVec3 &vec) const
{
	return rVec3(x / vec.x, y / vec.y, z / vec.z);
}

rVec3& rVec3::operator=(const rVec3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;

	return *this;
}


rVec3& rVec3::operator-=(const rVec3 &vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;

	return *this;
}

rVec3& rVec3::operator+=(const rVec3 &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;

	return *this;
}

rVec3& rVec3::operator*=(const rVec3 &vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;

	return *this;
}

rVec3& rVec3::operator/=(const rVec3 &vec)
{
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;

	return *this;
}

rVec3 rVec3::operator*(const float val) const
{
	return rVec3(x * val, y * val, z * val);
}

rVec3 rVec3::operator/(const float val) const
{
	return rVec3(x / val, y / val, z / val);
}

rVec3& rVec3::operator*=(const float val)
{
	x *= val;
	y *= val;
	z *= val;

	return *this;
}

rVec3& rVec3::operator/=(const float val)
{
	x /= val;
	y /= val;
	z /= val;

	return *this;
}