//---------------------------------------------------------------
// Name: Ren's file IO library
//
// This light weight file class uses 'stdio.h' because it is pretty light weight.
// The main purpose of this class is to act as a wrapper, more or less.
// I'm not sure I could write my own cross platform file IO system.
//
// It will store file paths and file names separatly as static arrays
//
// The rFile object will accept, or return data, which will be
// written into a file as (char *)
//---------------------------------------------------------------

/*
	Changes and versions:
	Version 1.0.0 - May 2012: initial release
*/

#include <stdio.h>

#ifndef _RENFILE_
#define _RENFILE_

#ifndef NULL
#define NULL 0
#endif

//Limit file paths and names to this size.
const int SizeLimit = 256;

struct rFileName
{
	char	name[SizeLimit];
	char	path[SizeLimit];
	int		nLen;
	int		pLen;
};

class rFile
{
public:
						rFile(void);
						rFile(const char* path, const char* name);
						rFile(const char* path, const char* name, size_t size, const unsigned long length);

	//File probes
	bool				fileExists(void);			//Open the file to see if it exists
	int					fileAttribs(void);			//Not available for now, without external includes

	//Name Access
	void				setPath(const char* path);	//Set new path
	void				setName(const char* name);	//Set new name

	const char*			getPath(void) const;		//Get current path
	const char*			getName(void) const;		//Get current file name
	const char*			getFull(void) const;		//Get current full path and name

	void				clearPath(void);			//Set path to NULL
	void				clearName(void);			//Set file name to NULL
	void				clearFull(void);			//Set path and file name to NULL

	//Sizes
	void				setSize(const size_t size);	//Set size of data to be written
	size_t				getSize(void) const;		//Get size of data, either for writing, or size of read data

	void				setLength(const unsigned long length);	//Set length of data to write
	const unsigned long	getLength(void) const;		//Get legnth of data, either fo writing, or length of read data

	//Read and Write
	char*				readAll(void);				//Read currently defined file
	bool				writeAll(const char* data);	//Write to currently defined file, return success or fail
	bool				writeAppend(const char* data); //Append data to existing file

protected:
	rFileName			fileName;
	size_t				typeSize;
	unsigned long		dataLen;					//Max 4GB

	void				init(void);
};
#endif