//---------------------------------------------------------------
// Name: Ren's custom type library
//
// This file contains a few useful types, memory aligned
//
// DOT product is using SSE 'sqrtss' so we don't need to access math.h
// pow() is done through multiplication.
//---------------------------------------------------------------
#include "CustomTypes.h"

float floatRoundToInt(const float val)
{
	int result;
	_asm
	{
		fld val;
		fistp result;
	}
	return (float)result;
}