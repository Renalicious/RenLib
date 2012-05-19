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

rVec2 rVec2::get(void) const
{
	return *this;
}

//Return data
float rVec2::length(void) const
{
	return x + y;
}

float rVec2::dot(void) const
{
	float result;
	result = x*x + y*y;

	__asm
	{
		sqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

float rVec2::dot(const rVec2 &vec) const
{
	float result;
	result = x * vec.x + y * vec.y;

	__asm
	{
		sqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

float rVec2::cross(const rVec2 &vec)
{
	//U x V = Ux*Vy-Uy*Vx
	 return this->x * vec.y - this->y * vec.x;
}

rVec2 rVec2::truncate(const float max) const
{
	if(x >= max && y >= max)
		return rVec2(max, max);

	else if(x < max && y >= max)
		return rVec2(this->x, max);

	else if(x >= max && y < max)
		return rVec2(max, this->y);

	else
		return *this;
}

//Misc
void rVec2::zero(void)
{
	x = y = 0.0f;
}
void rVec2::snap(void)
{
	x = (int)(x + 0.5f);
	y = (int)(y + 0.5f);
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

rVec2 rVec2::operator*(const rVec2 &vec) const
{
	return rVec2(x * vec.x, y * vec.y);
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

rVec3 rVec3::get(void) const
{
	return *this;
}

//Return data
float rVec3::length(void) const
{
	return x + y + z;
}

float rVec3::dot(void) const
{
	float result;
	result = x*x + y*y + z*z;

	__asm
	{
		sqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

float rVec3::dot(const rVec3 &vec) const
{
	float result;
	result = x * vec.x + y * vec.y + z * vec.z;

	__asm
	{
		sqrtss		xmm1, result
		movss		result, xmm1
	}

	return result;
}

rVec3 rVec3::truncate(const float max) const
{
	if(z >= max)
	{
		if(x >= max && y >= max)
			return rVec3(max, max, max);

		else if(x < max && y >= max, max)
			return rVec3(this->x, max, max);

		else if(x >= max && y < max)
			return rVec3(max, this->y, max);

		else
			return rVec3(x, y, max);
	}

	else if(z < max)
	{
		if(x >= max && y >= max)
			return rVec3(max, max, z);

		else if(x < max && y >= max)
			return rVec3(this->x, max, z);

		else if(x >= max && y < max)
			return rVec3(max, this->y, z);
		
		else
			return *this;
	}
}

//Misc
void rVec3::zero(void)
{
	x = y = z = 0.0f;
}
void rVec3::snap(void)
{
	x = (int)(x + 0.5f);
	y = (int)(y + 0.5f);
	z = (int)(z + 0.5f);
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

rVec3 rVec3::operator*(const rVec3 &vec) const
{
	return rVec3(x * vec.x, y * vec.y, z * vec.z);
}

rVec3 rVec3::operator/(const rVec3 &vec) const
{
	return rVec3(x / vec.x, y / vec.y, z / vec.z);
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