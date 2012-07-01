/************************************************************************************************
File: Write to DDS
************************************************************************************************/
#include "dds.h"

unsigned char DDS_Ident[4] = {0x44, 0x44, 0x53, 0x20}; //This needs to be here in the CPP file

/*
void WriteDDS(const float4 &_data, const rStr fileName)
{
	//--- Define main variables
	rFile outHeader,	outData;
	DDS_PIXELFORMAT		ddsPx;
	DDS_HEADER			ddsHead;

	rMem::r_memset_asm(&ddsPx, 0,  sizeof(DWORD) * 8);
	rMem::r_memset_asm(&ddsHead, 0,  sizeof(DWORD) * 31);

	//--- Define DDS header
	ddsPx.dwSize = 32;
	ddsPx.dwFlags = 4;
	ddsPx.dwFourCC = 116; //32bits per channel, RGBA
	ddsPx.dwRGBBitCount = 0;
	ddsPx.dwRBitMask = 0;
	ddsPx.dwGBitMask = 0;
	ddsPx.dwBBitMask = 0;
	ddsPx.dwABitMask = 0;

	ddsHead.dwSize = 124;
	ddsHead.dwFlags = 0x80000 + 0x1000 + 0x07;
	ddsHead.dwHeight = _SIZE1K_ * _SZMULT_;
	ddsHead.dwWidth = _SIZE1K_ * _SZMULT_;
	ddsHead.dwPitchOrLinearSize = 16; //Each pixel = 16 bytes
	ddsHead.dwDepth = 0;
	ddsHead.dwMipMapCount = 0;
	ddsHead.dwReserved1; //All zero;
	ddsHead.ddspf = ddsPx;
	ddsHead.dwCaps = 0x1000; //Required 0x1000
	ddsHead.dwCaps2 = 0;
	ddsHead.dwCaps3 = 0;
	ddsHead.dwCaps4 = 0;
	ddsHead.dwReserved2 = 0;
	
	//Write data
	starFile.write( (const char*) &DDS_Ident, 4);
	starFile.write( (const char*) &ddsHead, 124);
	starFile.write( (const char*) &_data[0], sizeof(float4) * (ddsHead.dwHeight * ddsHead.dwWidth) );

	starFile.close();
}

void WriteDDS(const rSurf2D_128f &hdrTexture, const rStr fileName)
{
	//--- Define main variables
	rFile outHeader,	outData;
	DDS_PIXELFORMAT		ddsPx;
	DDS_HEADER			ddsHead;

	rMem::r_memset_asm(&ddsPx, 0,  sizeof(DWORD) * 8);
	rMem::r_memset_asm(&ddsHead, 0,  sizeof(DWORD) * 31);

	//--- Define DDS header
	ddsPx.dwSize = 32;
	ddsPx.dwFlags = 4;
	ddsPx.dwFourCC = 116; //32bits per channel, RGBA
	ddsPx.dwRGBBitCount = 0;
	ddsPx.dwRBitMask = 0;
	ddsPx.dwGBitMask = 0;
	ddsPx.dwBBitMask = 0;
	ddsPx.dwABitMask = 0;

	ddsHead.dwSize = 124;
	ddsHead.dwFlags = 0x80000 + 0x1000 + 0x07;
	ddsHead.dwHeight = hdrTexture.getLength();
	ddsHead.dwWidth = hdrTexture.getLength();
	ddsHead.dwPitchOrLinearSize = 16; //Each pixel = 16 bytes
	ddsHead.dwDepth = 0;
	ddsHead.dwMipMapCount = 0;
	ddsHead.dwReserved1; //All zero;
	ddsHead.ddspf = ddsPx;
	ddsHead.dwCaps = 0x1000; //Required 0x1000
	ddsHead.dwCaps2 = 0;
	ddsHead.dwCaps3 = 0;
	ddsHead.dwCaps4 = 0;
	ddsHead.dwReserved2 = 0;

	//--- Set names
	outHeader.setName(fileName.c_str());
	outHeader.setPath("d:/");

	outData.setName(fileName.c_str());
	outData.setPath("d:/");
	
	//--- Write data
	starFile.write( (const char*) &DDS_Ident, 4);
	starFile.write( (const char*) &ddsHead, 124);
	starFile.write( (const char*) &_data[0], sizeof(float4) * (ddsHead.dwHeight * ddsHead.dwWidth) );

	starFile.close();
}
*/