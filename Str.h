//---------------------------------------------------------------
// Name: Ren's String Library
//
// A standalone string class not relying on STL.
// Speed may be sacrificed for code size and readibility, but I've
// also included some assembly optimizations which may speed things up.
//
// Using id's string library as inspiration
//
// Preprocessor definitions based on available instruction sets:
// Scalar, MMX, SSE, AVX
//---------------------------------------------------------------

/*
	Changes and versions:
	Version 1.0.1 - May 15 2012: +Added an int pointer as a return value for the split method,
									This breaks the rules of functional programing, but it's much more convenient
									getting the number of splits this way.

								 +Added some 'fact code' optimizations to "r_memcopy_asm", by way of explicit loop lengths,
									this will reduce the number of loop compares for long strings.

								 +Fixed a crash bug in the split method where the chunkLength would end up 0.
									Eg, splitting text with token '\n', on a line that only contained '\n'.

	Version 1.0.0 - May 2012: initial release
*/

#ifndef _RENSTR_
#define _RENSTR_
#define _RENSTR_VER_	"1.0.1"



	#ifdef			_MMX_
		const int	STR_ALLOC_GRAN = 8;			//8 chars, 64bits MMX

	#elif defined	_SSE_
		#undef _MMX_
		const int	STR_ALLOC_GRAN = 16;		//16 chars, 128bits SSE

	#elif defined	_AVX_
		#undef _MMX_
		#undef _SSE_
		const int	STR_ALLOC_GRAN = 32;		//32 chars, 256bits AVX

	#else 
		#define ASM
		const int	STR_ALLOC_GRAN = 4;			//4 chars, 32bits scalar
	#endif

class rStr
{
public:
				rStr(void);
				rStr(const rStr &text);
				rStr(const char* text);
				~rStr(void);

	//--- Sizes
	int			length(void) const;					//Until last character, not NULL ('\0')
	int			allocated(void) const;				//Capacity of string in bytes
	int			regCount(void) const;				//Alloced / register size
	size_t		size(void) const;					//Size type

	//--- Find
	int			find(const char c);					//Case sensitive, from left
	int			rfind(const char c);				//Case sensitive, from right
	int			findNoCase(const char c);			//Case insensitive, from left
	int			rfindNoCase(const char c);			//Case insensitive, from right
	int			findText(const char* text);			//Find word
	int			findText(const rStr &text);			//Find word
	int			findTextNoCase(const char* text);	//Find word, case insensitive
	int			findTextNoCase(const rStr &text);	//Find word, case insensitive
	int			count(const char c);				//How many instances of c
	int			count(const char* text);			//How many instances of text
	int			count(const rStr &text);

	//--- Compare, 4 methods available:
	//1) Compare against local strings
	//2) Compare against local strings, n length
	//3) Compare against local strings, at index, n length
	//4) Compare two external strings, at index, n length
	bool		compare(const char* lookupText);
	bool		compare(const rStr &lookupText);
	bool		compare(const char* lookupText, int length);
	bool		compare(const rStr &lookupText, int length);
	bool		compare(const char* lookupText, int index, int length);	
	bool		compare(const rStr &lookupText, int index, int length);
	bool		compare(const char* srcText, const char* lookupText, int index, int length);
	bool		compare(const rStr &srcText, const rStr &lookupText, int index, int length);

	bool		compareNoCase(const char* lookupText);
	bool		compareNoCase(const rStr &lookupText);
	bool		compareNoCase(const char* lookupText, int length);
	bool		compareNoCase(const rStr &lookupText, int length);
	bool		compareNoCase(const char* lookupText, int index, int length);
	bool		compareNoCase(const rStr &lookupText, int index, int length);
	bool		compareNoCase(const char* srcText, const char* lookupText, int index, int length);
	bool		compareNoCase(const rStr &srcText, const rStr &lookupText, int index, int length);

	bool		isAlpha(int c) const;				//Is character a letter?
	bool		isNumeric(int c) const;				//Is character a number?
	bool		isTab(char c) const;				//Is is a tab character?
	bool		isNewLine(char c) const;			//Is it a newline character?
	
	//--- Whole operations
	void		empty(void);						//Erase all data
	void		clear(void);						//Erase all data and reinit
	bool		isEmpty(void);						//No data, size > 1?
	bool		isClear(void);						//No data, size == 1?

	//--- Case
	bool		isUpper(const char c);				//Is the character uppercase at index?
	bool		isUpper(const char c) const;		//Is the character uppercase at index? (Local data)
	bool		hasUpper(void);						//Does the data contain uppercase characters?
	bool		hasUpper(const char* text);			//Does the string contain uppercase characters?
	void		toUpper(void);						//Make data uppercase
	char		toUpper(const char c);				//Make index uppercase
	char		toUpper(const char c) const;		//Make index uppercase (Local data)
	char*		toUpper(const char* text);			//Make everything uppercase

	bool		isLower(const char c);				//Is the character lowercase at index?
	bool		isLower(const char c) const;		//Is the character lowercase at index? (Local data)
	bool		hasLower(void);						//Does the data contain lowercase characters?
	bool		hasLower(const char* text);			//Does the string contain lowercase characters?
	void		toLower(void);						//Make index lowercase
	char		toLower(const char c);				//Make index lowercase
	char		toLower(const char c) const;		//Make index lowercase (Local data)
	char*		toLower(const char* text);			//Make everything lowercase

	//--- Adding data
	void		append(const char c);				//Add char at end
	void		append(const char* text);			//Add text at end
	void		append(const rStr &text);			//Add string at end
	void		insert(const char c, int index);	//Insert char at index
	void		insert(const char* text, int index);//Insert text at index
	void		insert(const rStr &text, int index);//Insert string at index

	//--- Changing data
	void		replace(const char old, const char nw);
	void		replace(const char old, const char* newText);
	void		replace(const char old, const rStr &newText);
	void		replace(const char* oldText, const char* newText);
	void		replace(const rStr &oldText, const rStr &newText);
	void		fill(const char c);					//Replace everything with c, no changing length
	void		fill(const char c, int newLength);	//Replace everything with c, change length
	void		del(void);							//Delete end index, and shink string
	void		del(int index);						//Delete char and shrink string
	void		del(int start, int end);			//Delete area and shrink string
	void		stripLeading(const char c);			//Delete all chars c from data
	void		stripTrailing(const char c);		//Delete all chars c from end of dataz
	void		stripPath(void);					//rfind "/" or "\\" and delete from 0 to pos

	//--- Returning data, and conversions
	const char*	c_str(void) const;									//Return as char array
	const char*	intToChar(const signed int n);						//Return int as char array
	const char*	floatToChar(const float n, const int precision = 8);//Return float or double as rStr
	rStr*		split(const char token, int * _retVal);							//Split local text at token, return array
	rStr*		split(const char* text, const char token, int * _retVal);			//Split text at token, return array
	rStr*		split(const rStr &text, const char token, int * _retVal);			//Split text at token, return array

	//--- Memory opperations
	void				reAllocate(int amount, bool keepOld);		//Increase or decrease allocated memory

	//--- Opperators
	void				operator=(const char* text);
	void				operator=(const rStr &text);

	char				operator[](int index) const;
	char &				operator[](int index);

	friend bool			operator==(const char* a, const rStr &b);
	friend bool			operator==(const rStr &a, const char* b);
	friend bool			operator==(const rStr &a, const rStr &b);

	friend bool			operator!=(const char* a, const rStr &b);
	friend bool			operator!=(const rStr &a, const char* b);
	friend bool			operator!=(const rStr &a, const rStr &b);

	friend rStr			operator+(const rStr &a, const rStr &b);
	friend rStr			operator+(const char* a, const rStr &b);
	friend rStr			operator+(const rStr &a, const char* b);

	friend rStr			operator+(const rStr &a, const char b);
	friend rStr			operator+(const rStr &a, const signed int b);
	friend rStr			operator+(const rStr &a, const float b);
	friend rStr			operator+(const rStr &a, const double b);
	friend rStr			operator+(const rStr &a, const bool b);

	rStr &				operator+=( const rStr &a );
	rStr &				operator+=( const char *a );
	rStr &				operator+=( const char a );
	rStr &				operator+=( const float a );
	rStr &				operator+=( const double a );
	rStr &				operator+=( const signed int a );
	rStr &				operator+=( const bool a );

	//--- Assembly optimized
#ifdef _MMX_
	void		toLower_mmx(void);
	void		toUpper_mmx(void);
	bool		hasLower_mmx(void);
	bool		hasUpper_mmx(void);
	void		fill_mmx(const char c);
	void		r_memcopy_mmx(const char* dst, const char* src, const int length);
	void		r_memset_mmx(char* text, char c, int length);
	void		r_memset_gran_mmx(const char* dst, const char c, const int regCount);

#elif defined _SSE_
	void		toLower_sse(void);
	void		toUpper_sse(void);
	bool		hasLower_sse(void);
	bool		hasUpper_sse(void);
	void		fill_sse(const char c);
	int			r_strlen_sse42(const char* src); //SSE4.2 only!!
	int			r_strlen_sse(const char* src);
	void		r_memset_sse(const char* dst, const char c, const int length);

#elif defined _AVX_
	void		toLower_avx(void);
	void		toUpper_avx(void);
	bool		hasLower_avx(void);
	bool		hasUpper_avx(void);
	void		fill_avx(const char c);
	int			r_strlen_avx(const char* src); //Derived from r_strlen_sse42
	void		r_memset_avx(char* text, char c, int length);

#elif defined ASM
	int			find_asm(const char* text, const char c, const int length);				//Case Sensitive, front to back
	int			rfind_asm(const char* text, const char c, const int length);			//Case Sensitive, back to front
	int			findNoCase_asm(const char* text, const char cL, const char cU, const int length);	//Case Sensitive, front to back
	int			rfindNoCase_asm(const char* text, const char cL, const char cU, const int length);	//Case Sensitive, back to front
	int			r_strlen_asm(const char* text);
	void		r_memcopy_asm(const char* dst, const char* src, const int length);		//Assembly optimized memcopy
	void		r_memset_asm(const char* dst, const char c, const int length);			//Assembly optimized memset
	void		r_memset_gran_asm(const char* dst, const char c, const int regCount);	//Assembly optimized memset, 4 bytes per loop, should cut down on instructions

#endif

	//Current version, may be temporary
	char*		rStrVer(void);						//Return Ren string library version

private:
	//Multipurpose methods so reduce code size, only accessible from public methods
	int			universalFind(const char c, bool isForward = true, bool matchCase = false);
	int			universalFindText(const char* text, bool matchCase);
	void		universalReplace(const char* old, const char* nw);
	bool		universalCompare(const char* srcText, const char* lookupText, int length = -1, bool matchCase = false);

protected:
	int			len;								//How many chars before '\0'?
	char*		data;								//Our c[] string data
	int			alloced;							//How many bytes allocated?
	char		initBuffer[STR_ALLOC_GRAN];			//Every new rStr will be allocated to this

	void		init(void);							//Init strings using initBuffer
	void		ensureAlloced(int amount, bool keepOld = true);	//Make sure we've allocated enough memory	
	void		freeData(void);						//Reset to initBuffer, unless == initBuffer
	int			r_strlen(const char* text);			//Determine string length up to 65535
	int			rPow10(int exp);					//Return a base 10 exponential number

	void		r_memset(char* text, char c, int length);
	void		r_memcopy(char * dst, const char *src, int length);
	void		datacopy(const char* text);
	void		datacopy2(const char* text, int index, int length, int dataOffset = 0);
	void		dataappend(const char* text, int length);
};

#endif /* _RENSTR_ */