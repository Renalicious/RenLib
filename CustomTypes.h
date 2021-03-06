//---------------------------------------------------------------
// Name: Ren's custom type library
//
// This file contains a few useful types, memory aligned
//
// DOT product is using SSE 'sqrtss' so we don't need to access math.h
// pow() is done through multiplication.
//---------------------------------------------------------------

#ifndef _RENTYPES_
#define _RENTYPES_

//#define _SSE3_

//---------------------------------------------------------------
//--- Typedefs
//---------------------------------------------------------------
typedef bool				bit;
typedef unsigned __int8		byte;
typedef	unsigned __int16	word;
typedef unsigned __int32	dword;
typedef unsigned __int64	qword;

//---------------------------------------------------------------
//--- Union
//---------------------------------------------------------------
__declspec(align(16)) union _r64i
{
	byte	bytes[8];
	word	words[4];
	dword	dwords[2];
	qword	qwords[1];
};

__declspec(align(16)) union _r128i
{
	byte	bytes[16];
	word	words[8];
	dword	dwords[4];
	qword	qwords[2];
};

__declspec(align(16)) union _r128f
{
	float	f[4];
};

//---------------------------------------------------------------
//--- Struct, single precision float
//---------------------------------------------------------------
__declspec(align(16)) struct float2
{
	float x;
	float y;
};
__declspec(align(16)) struct float3
{
	float x;
	float y;
	float z;
};

__declspec(align(16)) struct float4
{
	float w;
	float x;
	float y;
	float z;
};

//---------------------------------------------------------------
//--- Struct, integer
//---------------------------------------------------------------
__declspec(align(16)) struct int2
{
	int x;
	int y;
};

__declspec(align(16)) struct int3
{
	int x;
	int y;
	int z;
};

__declspec(align(16)) struct int4
{
	int w;
	int x;
	int y;
	int z;
};

//---------------------------------------------------------------
//--- Struct, unsigned char
//---------------------------------------------------------------
__declspec(align(4)) struct byte2
{
	unsigned char x;
	unsigned char y;
};

__declspec(align(4)) struct byte3
{
	unsigned char  x;
	unsigned char  y;
	unsigned char  z;
};

__declspec(align(4)) struct byte4
{
	unsigned char  w;
	unsigned char  x;
	unsigned char  y;
	unsigned char  z;
};

__declspec(align(4)) struct RGBA
{
	unsigned char  r;
	unsigned char  g;
	unsigned char  b;
	unsigned char  a;
};

__declspec(align(4)) struct ARGB
{
	unsigned char  a;
	unsigned char  r;
	unsigned char  g;
	unsigned char  b;
};

#endif /* _RENTYPES_ */