//*****************************************************
// DDS header structure:
//
// Big Endien encoding?
// unsigned char DDS_Header_Raw[124] = {
//  0x44, 0x44, 0x53, 0x20, /*Magic Number 'D''D''S'' '*/
// 	0x7C, 0x00, 0x00, 0x00, /*dwSize*/
//  0x07, 0x10, 0x08, 0x00, /*dwFlags*/
//  0x00, 0x08, 0x00, 0x00, /*dwHeight*/
// 	0x00, 0x20, 0x00, 0x00, /*dwWidth*/
//  0x00, 0x00, 0x00, 0x10, /*dwPitchOrLinearSize*/
//  0x00, 0x00, 0x00, 0x00, /*dwDepth*/
// 	0x00, 0x00, 0x00, 0x00, /*dwMipMapCount*/
//  0x00, 0x00, 0x00, 0x00, /*dwReserved1[11]*/
//  0x00, 0x00, 0x00, 0x00, /*[1]*/
// 	0x00, 0x00, 0x00, 0x00, /*[2]*/
//  0x00, 0x00, 0x00, 0x00, /*[3]*/
//  0x00, 0x00, 0x00, 0x00, /*[4]*/
// 	0x00, 0x00, 0x00, 0x00, /*[5]*/
//  0x00, 0x00, 0x00, 0x00, /*[6]*/
//  0x00, 0x00, 0x00, 0x00, /*[7]*/
// 	0x00, 0x00, 0x00, 0x00, /*[8]*/
//  0x00, 0x00, 0x00, 0x00, /*[9]*/
//  0x00, 0x00, 0x00, 0x00, /*[10]*/
// 	0x20, 0x00, 0x00, 0x00, /*ddspf::dwSize*/
//  0x04, 0x00, 0x00, 0x00, /*ddspf::dwFlags*/
//  0x74, 0x00, 0x00, 0x00, /*ddspf::dwFourCC*/
// 	0x00, 0x00, 0x00, 0x00, /*ddspf::dwRGBBitCount*/
//  0x00, 0x00, 0x00, 0x00, /*ddspf::dwRBitMask*/
//  0x00, 0x00, 0x00, 0x00, /*ddspf::dwGBitMask*/
// 	0x00, 0x00, 0x00, 0x00, /*ddspf::dwBBitMask*/
//  0x00, 0x00, 0x00, 0x00, /*ddspf::dwABitMask*/
//  0x00, 0x10, 0x00, 0x00, /*dwCaps*/
// 	0x00, 0x00, 0x00, 0x00, /*dwCaps2*/
//  0x00, 0x00, 0x00, 0x00, /*dwCaps3*/
//  0x00, 0x00, 0x00, 0x00, /*dwCaps4*/
// 	0x00, 0x00, 0x00, 0x00 /*dwReserved2*/
// };
//*******************************************************
#ifndef DDS_H
#define DDS_H

#include "CustomTypes.h"

typedef unsigned long DWORD;

struct DDS_PIXELFORMAT {
  DWORD dwSize;							//Structure size; set to 32 (bytes)
  DWORD dwFlags;						//Values which indicate what type of data is in the surface.
  DWORD dwFourCC;						//116 = D3DFMT_A32B32G32R32F
  DWORD dwRGBBitCount;					//Number of bits in an RGB (possibly including alpha) format
  DWORD dwRBitMask;						//0x 00 ff 00 00
  DWORD dwGBitMask;						//0x 00 00 ff 00
  DWORD dwBBitMask;						//0x 00 00 00 ff
  DWORD dwABitMask;						//0x ff 00 00 00
};

typedef struct {
  DWORD           dwSize;               //Set to 124, size of header (exclude magic #)
  DWORD           dwFlags;              //Flags to indicate which member (of this header) contain valid data.
  DWORD           dwHeight;             //Image height
  DWORD           dwWidth;              //Image width
  DWORD           dwPitchOrLinearSize;  //The pitch, number of bytes, per scan line (pixel) in an uncompressed texture
  DWORD           dwDepth;              //Depth of a volume texture (in pixels), otherwise unused.
  DWORD           dwMipMapCount;        //Number of mipmap levels, otherwise unused.
  DWORD           dwReserved1[11];      //Unused
  DDS_PIXELFORMAT ddspf;                //See "struct DDS_PIXELFORMAT" above
  DWORD           dwCaps;               //Specifies the complexity of the surfaces stored; Required 0x1000
  DWORD           dwCaps2;              //Unused
  DWORD           dwCaps3;              //Unused
  DWORD           dwCaps4;              //Unused
  DWORD           dwReserved2;          //Unused
} DDS_HEADER;

#endif /* DDS_H */