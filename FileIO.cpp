//---------------------------------------------------------------
// Name: Ren's file IO library
//
// This light weight file class uses 'stdio.h' because it is pretty light weight.
// The main purpose of this class is to act as a wrapper, more or less.
//
// It will store file paths and file names separatly as static arrays
//
// The rFile object will accept, or return data, which will be
// written into a file as (char *)
//---------------------------------------------------------------
#include "FileIO.h"

//-------------------------------------------
//--- Constructors
//-------------------------------------------
rFile::rFile(void)
{
	init();
	typeSize = 0;
	dataLen = 0;
}

rFile::rFile(const char* path, const char* name)
{
	int i;
	init();
	typeSize = 0;
	dataLen = 0;

	for(i = 0; i < SizeLimit && path[i]; i++)
		fileName.path[i] = path[i];
	
	fileName.pLen = i;

	for(i = 0; i < SizeLimit && name[i]; i++)
		fileName.name[i] = name[i];
	
	fileName.nLen = i;
}

rFile::rFile(const char* path, const char* name, size_t size, const unsigned long length)
{
	int i;
	init();
	typeSize = size;
	dataLen = length;

	for(i = 0; i < SizeLimit && path[i]; i++)
		fileName.path[i] = path[i];
	
	fileName.pLen = i;

	for(i = 0; i < SizeLimit && name[i]; i++)
		fileName.name[i] = name[i];
	
	fileName.nLen = i;
}

void rFile::init(void)
{
	for(int i = 0; i < SizeLimit; i++)
	{
		fileName.name[i] = '\0';
		fileName.path[i] = '\0';
	}
}

//-------------------------------------------
//--- File probes
//-------------------------------------------
bool rFile::fileExists(void)
{
	//Make sure we have a path and file name
	if(!fileName.nLen || !fileName.pLen)
		return false;

	else
	{
		//Examine file
		FILE *fPtr;

		//File doesn't exist
		if( fopen_s(&fPtr, getFull(), "r") != 0)
		{
			return false;
		}

		//File exists, close it if it's open
		else
		{
			if(fPtr)
				fclose(fPtr);

			return true;
		}		
	}
}

//There might not be a simple way to determine what the attribs are
//using stdio by itself. For now I'll just pass through
//to fileExists() and return 0 or -1.
//I might consider "unistd.h" in the future
int rFile::fileAttribs(void)
{
	if(fileExists())
		return 0;

	else
		return -1;
}

//-------------------------------------------
//--- Name Access
//-------------------------------------------
void rFile::setPath(const char* newPath)
{
	int i;

	for(i = 0; i < SizeLimit && newPath[i]; i++)
		fileName.path[i] = newPath[i];

	fileName.pLen = i;
}

void rFile::setName(const char* newName)
{
	int i;

	for(i = 0; i < SizeLimit && newName[i]; i++)
		fileName.name[i] = newName[i];

	fileName.nLen = i;
}

const char* rFile::getPath(void) const
{
	int i = 0;
	int p = 0;

	char *f = new char[fileName.pLen];

	while(p < fileName.pLen)
	{
		f[i] = fileName.path[p];
		i++;
		p++;
	}
	return f;
}

const char* rFile::getName(void) const
{
	int i = 0;
	int n = 0;

	char *f = new char[fileName.nLen];

	while(n < fileName.nLen)
	{
		f[i] = fileName.name[n];
		i++;
		n++;
	}
	return f;
}

const char* rFile::getFull(void) const
{
	int i = 0;
	int n = 0;
	int p = 0;

	char *f = new char[fileName.nLen + fileName.pLen + 2];

	while(p < fileName.pLen)
	{
		f[i] = fileName.path[p];
		i++;
		p++;
	}

	f[i] = '/';
	i++;

	while(n < fileName.nLen)
	{
		f[i] = fileName.name[n];
		i++;
		n++;
	}

	f[i] = '\0';
	return f;
}

void rFile::clearPath(void)
{
	for(int i = 0; i < SizeLimit; i++)
		fileName.path[i] = '\0';

	fileName.pLen = 0;
}

void rFile::clearName(void)
{
	for(int i = 0; i < SizeLimit; i++)
		fileName.name[i] = '\0';

	fileName.nLen = 0;
}

void rFile::clearFull(void)
{
	init();
}

//-------------------------------------------
//--- Sizes
//-------------------------------------------
void rFile::setSize(const size_t newSize)
{
	typeSize = newSize;
}

void rFile::setLength(const unsigned long newLength)
{
	dataLen = newLength;
}

size_t rFile::getSize(void) const
{
	return typeSize;
}

const unsigned long rFile::getLength(void) const
{
	return dataLen;
}

//-------------------------------------------
//--- Read and Write
//-------------------------------------------
char* rFile::readAll(void)
{
	//Make sure we have a path and file name
	if(!fileName.nLen || !fileName.pLen)
		return NULL;

	if(!fileExists())
		return NULL;

	else
	{
		FILE *fp;
		long len;
		char *buf;

		fopen_s(&fp, getFull(), "rb");

		fseek(fp, 0, SEEK_END);	//go to end
		len=ftell(fp);			//get position at end (length)
		fseek(fp, 0, SEEK_SET);	//go to beg.

		buf = new char[len];	//malloc buffer
		fread(buf, len, 1, fp);	//read into buffer

		fclose(fp);

		dataLen = len;
		typeSize = sizeof(buf);

		return buf;
	}
}

//Write to file, returns false on error, true on success
bool rFile::writeAll(const char* data)
{
	if(!fileName.nLen || !fileName.pLen || !dataLen || !typeSize)
		return false;

	else
	{
		FILE *fp;

		fopen_s(&fp, getFull(), "wb");
		fwrite(data, typeSize, dataLen / typeSize, fp);
		fclose(fp);

		return true;
	}
}

//Append to current file
bool rFile::writeAppend(const char* data)
{
	if(!fileName.nLen || !fileName.pLen || !dataLen || !typeSize)
		return false;

	else
	{
		FILE *fp;

		fopen_s(&fp, getFull(), "ab+");
		fwrite(data, typeSize, dataLen / typeSize, fp);
		fclose(fp);

		return true;
	}
}