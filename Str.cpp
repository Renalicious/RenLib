//---------------------------------------------------------------
// Name: Ren's String Library
//
// A standalone string class not relying on STL.
// Speed may be sacrificed for code size and readibility, but I've
// also included some assembly optimizations which may speed things up.
//
// Using id's string library as inspiration
//---------------------------------------------------------------

#include "Str.h"

//-------------------------------------------
//--- Memory management
//-------------------------------------------

//--- Base initialize
void rStr::init(void)
{
	len = 0;
	alloced = STR_ALLOC_GRAN;
	data = initBuffer;
	r_memset(data, '\0', alloced);
}

rStr::~rStr(void)
{
	freeData();
}

//--- Delete data
void rStr::freeData(void)
{
	if( data && data != initBuffer)
	{
		delete[] data;
		data = initBuffer;
	}
}

//--- Do we need more memory for the string?
void rStr::ensureAlloced(int amount, bool keepOld)
{
	if(amount > alloced)
		reAllocate(amount, keepOld);
}

//--- Reallocate memory to 'amount'. More of less a copy of id's procedure.
void rStr::reAllocate(int amount, bool keepOld)
{
	int mod = amount % STR_ALLOC_GRAN;
	int newSize = 0;
	char *newData;

	//Are we allocating more than STR_ALLOC_GRAN?
	//Align allocation to STR_ALLOC_GRAN (4, 8, 12, 16, 20, ...)
	if(mod == 0)	newSize = amount;
	else			newSize = amount + STR_ALLOC_GRAN - mod;

	//Keep track of allocation
	alloced = newSize;
	newData = new char[alloced];

	//Replace old string with new one
	if(keepOld && data)
	{
		data[len] = '\0';
		for(int i = 0; i <= len; i++)
			newData[i] = data[i];
	}

	if(data && data != initBuffer)
	{
		delete[] data;
	}

	data = newData;
}

//Get length of c[], limit to length of short
int rStr::r_strlen(const char *text)
{
	int i;

#ifdef ASM
	i = r_strlen_asm(text);
#else
	for(i; text[i]; i++) {}
#endif

	return i;
}

void rStr::r_memset(char *text, char c, int length)
{
#ifdef ASM
	r_memset_asm(text, c, length);
#else
	for(int i = 0; i <= len; i++)
		text[i] = c;
#endif
}

void rStr::r_memcopy(char *dst, const char *src, int length)
{
#ifdef ASM
	r_memcopy_asm(dst, src, length);
#else
	for(int i = 0; i < length; i++)
		dst[i] = src[i];
#endif
}

//Copy data from a to b, up to length a
//Keep this around just in case!
void rStr::datacopy(const char *text)
{
	for(int i = 0; i <= len; i++)
		data[i] = text[i];
}
//-------------------------------------------
//--- Constructors
//-------------------------------------------

//--- Basic constructor
rStr::rStr(void)
{
	init();
}

//--- c[] Text constructor
rStr::rStr(const char* text)
{
	int l = 0;

	init();

	l = r_strlen(text);
	ensureAlloced(l + 1);
	len = l;

	datacopy(text);	
}

//--- rStr text constructor
rStr::rStr(const rStr &text)
{
	int l = 0;

	init();

	l = r_strlen(text.data);
	ensureAlloced(l + 1);
	len = l;

	datacopy(text.data);
}

//-------------------------------------------
//--- Sizes
//-------------------------------------------

//--- Return length of rStr
int rStr::length() const
{
	return len;
}

//--- How much memory allocated
int rStr::allocated() const
{
	return alloced;
}

//--- How many times would the register be filled?
int rStr::regCount() const
{
	return alloced / STR_ALLOC_GRAN;
}

size_t rStr::size(void) const
{
	return sizeof(*this);
}

//-------------------------------------------
//--- Find
//-------------------------------------------
int rStr::find(const char c)
{
	return universalFind(c, true, true);
}

int rStr::rfind(const char c)
{
	return universalFind(c, false, true);
}

int rStr::findNoCase(const char c)
{
	return universalFind(c, true, false);
}

int rStr::rfindNoCase(const char c)
{
	return universalFind(c, false, false);
}

int rStr::findText(const rStr &text)
{
	return universalFindText(text.data, true);
}

int rStr::findText(const char* text)
{
	return universalFindText(text, true);
}

int rStr::findTextNoCase(const rStr &text)
{
	return universalFindText(text.data, false);
}

int rStr::findTextNoCase(const char* text)
{
	return universalFindText(text, false);
}

int rStr::count(const char c)
{
	int i, count;

	count = 0;
	for(i = 0; i <= len; i++)
	{
		if(data[i] == c)
			count++;
	}

	return count;
}

int rStr::count(const rStr &text)
{
	return count(text.data);
}

int rStr::count(const char* text)
{
	if(!text)
		return 0;

	int matchCount = 0;
	int textLength = 0;

	textLength = r_strlen(text);

	//Search for matches, and determine new length
	for(int i = 0; i <= len; i++)
	{
		//Compare old word, against new word, at position i, limit search to the length of the new word
		if( compare(data, text, i, textLength) )
		{
			matchCount++;
			i += textLength - 1; //Move ahead past oldWord
		}
	}

	return matchCount;
}

int rStr::universalFindText(const char* text, bool matchCase)
{
	int textLength;

	textLength = r_strlen(text);
	int j = 0;

	//Start looking through our data for a match
	for(int i = 0; i <= len; i++)
	{
		if(matchCase)
		{
			//Found something? Make sure it's contiguous
			if(data[i] == text[0])
			{
				for(j = 0; j < textLength || j+i <= len; j++)
				{
					if(data[i+j] != text[j])
					{
						break;
					}
				}
			}
		}

		else
		{
			if(toLower(data[i]) == toLower(text[0]))
			{
				for(j = 0; j < textLength || j+i <= len; j++)
				{
					if( toUpper(data[i+j]) != toUpper(text[j]) )
					{
						break;
					}
				}
			}
		}

		if(!text[j])
		{
			return i;
		}
	}

	return -1;
}

//--- Multipurpose find function to reduce code duplication
int rStr::universalFind(const char c, bool isForward, bool matchCase)
{
	//Initial check
	if(!c)
		return -1;

//Keeping this pre-processor block because it's big enough
#ifdef ASM
	else
	{
		if(matchCase)
		{
			if(isForward)
				return find_asm(data, c, len);
			else
				return rfind_asm(data, c, len);
		}
		else
		{
			char cAlt;
			if(isLower(c))
			{
				cAlt = toUpper(c);

				if(isForward)
					return findNoCase_asm(data, c, cAlt, len);
				else
					return rfindNoCase_asm(data, c, cAlt, len);
			}
			else
			{
				cAlt = toLower(c);
				if(isForward)
					return findNoCase_asm(data, cAlt, c, len);
				else
					return rfindNoCase_asm(data, cAlt, c, len);
			}
		}
	}
#else
	//Loops
	else
	{
		//Forward
		if(isForward)
		{
			for(int i = 0; i <= len; i++)
			{
				if(matchCase)
				{
					if(data[i] == c)
					{
						return i;
					}
				}
				else
				{
					if( (data[i] == c) || (toLower(data[i]) == c) || (toUpper(data[i]) == c) )
					{
						return i;
					}
				}
			}
		}

		//Backward
		else
		{
			for(int i = len; i >= 0; i--)
			{
				if(matchCase)
				{
					if(data[i] == c)
					{
						return i;
					}
				}
				else
				{
					if( (data[i] == c) || (toLower(data[i]) == c) || (toUpper(data[i]) == c) )
					{
						return i;
					}
				}
			}
		}
	}

#endif

	//Didn't find anything
	return -1;
}

//-------------------------------------------
//--- Compare
//-------------------------------------------
bool rStr::isAlpha(int c) const
{
	return ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') );
}

bool rStr::isNumeric(int c) const
{
	return ( c <= '9' && c >= '0' );
}

bool rStr::isTab(char c) const
{
	return (c == '\t');
}

bool rStr::isNewLine(char c) const
{
	return ( c == '\n' || c == '\r' || c == '\v' );
}

//--- Compare, case sensitive
bool rStr::compare(const char* lookupText)
{
	return universalCompare(data, lookupText, -1, true);
}

bool rStr::compare(const rStr &lookupText)
{
	return universalCompare(data, lookupText.data, -1, true);
}

bool rStr::compare(const char* lookupText, int length)
{
	return universalCompare(data, lookupText, length, true);
}

bool rStr::compare(const rStr &lookupText, int length)
{
	return universalCompare(data, lookupText.data, length, true);
}

bool rStr::compare(const char* lookupText, int index, int length)
{
	return universalCompare(&data[index], lookupText, length, true);
}

bool rStr::compare(const rStr &lookupText, int index, int length)
{
	return universalCompare(&data[index], lookupText.data, length, true);
}

bool rStr::compare(const char* srcText, const char* lookupText, int index, int length)
{
	return universalCompare(&srcText[index], lookupText, length, true);
}

bool rStr::compare(const rStr &srcText, const rStr &lookupText, int index, int length)
{
	return universalCompare(&srcText.data[index], lookupText.data, length, true);
}

//Compare, NOT case sensitive
bool rStr::compareNoCase(const char* lookupText)
{
	return universalCompare(data, lookupText, -1, false);
}

bool rStr::compareNoCase(const rStr &lookupText)
{
	return universalCompare(data, lookupText.data, -1, false);
}

bool rStr::compareNoCase(const char* lookupText, int length)
{
	return universalCompare(data, lookupText, length, false);
}

bool rStr::compareNoCase(const rStr &lookupText, int length)
{
	return universalCompare(data, lookupText.data, length, false);
}

bool rStr::compareNoCase(const char* lookupText, int index, int length)
{
	return universalCompare(&data[index], lookupText, length, false);
}

bool rStr::compareNoCase(const rStr &lookupText, int index, int length)
{
	return universalCompare(&data[index], lookupText.data, length, false);
}

bool rStr::compareNoCase(const char* srcText, const char* lookupText, int index, int length)
{
	return universalCompare(&srcText[index], lookupText, length, false);
}

bool rStr::compareNoCase(const rStr &srcText, const rStr &lookupText, int index, int length)
{
	return universalCompare(&srcText.data[index], lookupText.data, length, false);
}

//--- Universal compare, using id's method again, cause it works.
bool rStr::universalCompare(const char* srcText, const char* lookupText, int length, bool matchCase)
{
	int src = 0;
	int dst = 0;
	int dif = 0;

	//Compare characters of each string
	do{
		src = *srcText++;
		dst = *lookupText++;
		dif = src - dst;

		//Decrement length to 0
		//if we haven't quit up to now, the strings are equal
		if(!length--)
			return true;

		if(matchCase)
		{
			//Dif should be 0, otherwise the characters are different.
			if(dif)
				return false;
		}

		else
		{
			while(dif)
			{
				if(src <= 'Z' && src >= 'A')
				{
					dif += ('a' - 'A');
					if(!dif)
						break;
				}

				if(dst <= 'Z' && dst >= 'A')
				{
					dif -= ('a' - 'A');
					if(!dif)
						break;
				}

				if(dif)
					return false;
			}					
		}

	}while(src);

	return true;
}

//-------------------------------------------
//--- Whole operations
//-------------------------------------------

//--- Erase all data to null
void rStr::empty(void)
{
	data[0] = '\0';
	len = 0;
}

//--- Reinitialize to 0
void rStr::clear(void)
{
	freeData();
	init();
}

bool rStr::isEmpty()
{
	return(!data[0]);
}

bool rStr::isClear()
{
	if(data == initBuffer && len == 0 && alloced == STR_ALLOC_GRAN)
		return true;

	else
		return false;
}

//-------------------------------------------
//--- Case
//-------------------------------------------
bool rStr::isLower(const char c)
{
	return (c >= 'a' && c <= 'z');
}

bool rStr::isLower(const char c) const
{
	return (c >= 'a' && c <= 'z');
}

bool rStr::isUpper(const char c)
{
	return (c >= 'A' && c <= 'Z');
}

bool rStr::isUpper(const char c) const
{
	return (c >= 'A' && c <= 'Z');
}

bool rStr::hasLower(void)
{
	for(int i = 0; i < len; i++)
		if(isLower(data[i]))
			return true;
	
	return false;
}

bool rStr::hasUpper(void)
{
	for(int i = 0; i < len; i++)
		if(isUpper(data[i]))
			return true;
	
	return false;
}

bool rStr::hasLower(const char* text)
{
	int textLength;

	if(!text)
		return false;

	textLength = r_strlen(text);

	for(int i = 0; i < textLength; i++)
		if(isLower(text[i]))
			return true;
	
	return false;
}

bool rStr::hasUpper(const char* text)
{
	int textLength;

	if(!text)
		return false;

	textLength = r_strlen(text);

	for(int i = 0; i < textLength; i++)
		if(isUpper(text[i]))
			return true;
	
	return false;
}

void rStr::toLower(void)
{
	for(int i = 0; i <= len; i++)
		data[i] = toLower(data[i]);
}

void rStr::toUpper(void)
{
	for(int i = 0; i <= len; i++)
		data[i] = toUpper(data[i]);
}

char rStr::toLower(const char c)
{
	if(!isLower(c) && isAlpha(c))
		return (c + ('a' - 'A'));

	return c;
}

char rStr::toLower(const char c) const
{
	if(!isLower(c) && isAlpha(c))
		return (c + ('a' - 'A'));

	return c;
}

char rStr::toUpper(const char c)
{
	if(!isUpper(c) && isAlpha(c))
		return (c - ('a' - 'A'));

	return c;
}

char rStr::toUpper(const char c) const
{
	if(!isUpper(c) && isAlpha(c))
		return (c - ('a' - 'A'));

	return c;
}

char* rStr::toLower(const char* text)
{
	int l;
	
	l = r_strlen(text);
	char* newData = new char[l];
	r_memset(newData, '\0', l);

	for(int i = 0; i < l; i++)
		newData[i] = toLower(text[i]);

	newData[l] = '\0';
	return newData;
}

char* rStr::toUpper(const char* text)
{
	int l;
	
	l = r_strlen(text);
	char* newData = new char[l];
	r_memset(newData, '\0', l);

	for(int i = 0; i < l; i++)
		newData[i] = toUpper(text[i]);

	newData[l] = '\0';
	return newData;
}

//-------------------------------------------
//--- Adding data
//-------------------------------------------
void rStr::append(const char c)
{
	if(c)
	{
		ensureAlloced(len + 2, true);
		data[len] = c;
		len++;	
		data[len] = '\0';
	}
}

void rStr::append(const char* text)
{
	if(text[0] != '\0')
	{
		int textLength;
		int newLength;

		textLength = r_strlen(text);
		newLength = len + textLength;

		ensureAlloced(newLength + 2, true);

		//Append data here
		for(int i = 0; i < textLength; i++)
			data[len + i] = text[i];

		len = newLength;

		data[len] = '\0';
	}
}

void rStr::append(const rStr &text)
{
	append(text.data);
}

void rStr::insert(const char c, int index)
{
	int i;

	if(index < 0)			index = 0;
	else if(index > len)	index = len;

	ensureAlloced(len + 2, true);

	//Id's briliant technique :D!!!
	for(i = len; i >= index; i--)
		data[i + 1] = data[i];

	data[index] = c;
	len++;
}

void rStr::insert(const char* text, int index)
{
	int i;
	int l;

	if(index < 0)			index = 0;
	else if(index > len)	index = len;

	l = r_strlen(text);
	ensureAlloced(len + l + 1, true);

	//Id's brilliant technique :)
	for(i = len; i >= index; i--)
		data[i + l] = data[i];

	for(i = 0; i < l; i++)
		data[i + index] = text[i];

	len += l;
}

void rStr::insert(const rStr &text, int index)
{
	//Check if we're using our own data,
	//If we are make a copy, otherwise it'll start overwriting itself
	if(*this == text)
	{
		char *dataCopy = new char[alloced]; //Used alloced so it'll work with ASM

		r_memset(dataCopy, '\0', len);
		r_memcopy(dataCopy, data, len);
		insert(dataCopy, index);
	}

	else
		insert(text.c_str(), index);
}

//-------------------------------------------
//--- Changing data
//-------------------------------------------
void rStr::replace(const char old, const char nw)
{
	for(int i = 0; i <= len; i++)
		if(data[i] == old)
			data[i] = nw;
}

void rStr::replace(const char old, const char* newText)
{
	universalReplace(&old, newText);
}

void rStr::replace(const char old, const rStr &newText)
{
	universalReplace(&old, newText.data);
}

void rStr::replace(const char* oldText, const char* newText)
{
	universalReplace(oldText, newText);
}

void rStr::replace(const rStr &oldText, const rStr &newText)
{
	universalReplace(oldText.data, newText.data);
}

//Fill current length
void rStr::fill(const char c)
{
	if(c)
	{
		r_memset(data, c, len);
		data[len] = '\0';
	}
}

//Fill new length
void rStr::fill(const char c, int newLength)
{
	if(c)
	{
		ensureAlloced(newLength + 2, true);
		len = newLength;

		r_memset(data, c , len);
		data[len] = '\0';
	}
}

//Delete last char, resize data
void rStr::del(void)
{
	int newLength = len - 1;

	ensureAlloced(newLength + 1, true);
	len = newLength;
	data[len] = '\0';
}

//Delete char at index, resize data
void rStr::del(int index)
{
	if(index > len)
		del();

	else
	{
		if(index < 0)
			index = 0;

		int newLength = len - 1;
		rStr oldString = data;

		ensureAlloced(newLength + 1, false);
		len = newLength;

		//Copy 0 - index, and then index + 1 to len
		r_memcopy(data, oldString.data, 0 + index);
		r_memcopy(data + index, oldString.data + index + 1, len - index);

		data[len] = '\0';
	}
}

//Delete range and resize
void rStr::del(int start, int end)
{
	//Initial checks, make use of existing functionality
	if(start <= 0 && end >= len)	clear();
	else if(start > len)					del();
	else if(start > end && start < len)	del(start);
	else if(start > end && start <= 0)	del(0);
	else if(start <= 0 && end <= 0)		del(0);
	else if(start == end)				del(start);

	else
	{
		
		int dif = end - start;

		int newLength = len - dif;

		rStr oldString = data;
		ensureAlloced(newLength + 1, false);

		//Copy 0 to start
		for(int i = 0; i < start; i++)
			data[i] = oldString[i];

		//Copy end to len
		for(int i = len; i > end; i--)
			data[i-dif - 1] = oldString[i];

		len = newLength;
		data[len] = '\0';
	}
}

void rStr::stripLeading(const char c)
{
	int pos = 0;
	while(data[pos] == c && pos <= len)
		pos++;

	r_memcopy(data, data + pos, len - pos);

	len = len - pos;
	data[len] = '\0';
}

void rStr::stripTrailing(const char c)
{
	int pos = length();
	while(data[pos - 1] == c && pos > 0)
		pos--;

	len = pos;
	data[len] = '\0';
}

//Return false on no path found
void rStr::stripPath(void)
{
	int pos;

	for(pos = length(); pos > 0; pos--)
	{
		if(data[pos - 1] == '\\' || data[pos - 1] == '/')
			break;
	}

	r_memcopy(data, data + pos, len - pos);

	len = len - pos;
	data[len] = '\0';
}
void rStr::universalReplace(const char* old, const char* nw)
{
	int matchCount = 0;
	int oldLength = 0;
	int newLength = 0;

	int oldPos = 0;
	int newPos = 0;

	oldLength = r_strlen(old);
	newLength = r_strlen(nw);
	rStr oldString = data;

	//Search for matches, and determine new length
	for(int i = 0; i <= len; i++)
	{
		//Compare old word, against new word, at position i, limit search to the length of the new word
		if( compare(data, old, i, oldLength) )
		{
			matchCount++;
			i += oldLength - 1; //Move ahead past oldWord
		}
	}

	if(matchCount)
	{
		ensureAlloced(len + ((newLength - oldLength) * matchCount) + 2, false);

		//Replace old with new
		for(oldPos = 0, newPos = 0; oldPos <= oldString.length(); oldPos++)
		{
			if( compare(oldString.data, old, oldPos, oldLength) )
			{
				r_memcopy( data + newPos, nw, newLength );
				oldPos += oldLength - 1; //Move ahead past oldWord
				newPos += newLength;
			}
			else
			{
				data[newPos] = oldString[oldPos];
				newPos++;
			}
		}

		len = r_strlen(data);
		data[newPos] = '\0';
	}
}

//-------------------------------------------
//--- Return data and conversions
//-------------------------------------------

float rStr::toFloat(const rStr &inputVal)
{
	bool isFraction;
	int wholeCount, fracCount, inLen;
	float result;
	int valueStruct[4] = {0,0,0,0}; //sign, number, exp
	rStr input = inputVal;

	//--- Check for infinity and not a number
	if(input.findTextNoCase("nan") >= 0)
	{
		static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
        return (*(const float *) __nan);
	}

	if(input.findTextNoCase("inf") >= 0)
	{
		static const int value = 0x7f800000; 
		return *(float*)&value;
	}

	//-------------------
	//--- Validate number
	//-------------------

	//Check sign and if it's a number
	if(input[0] == '-' && this->isNumeric(input[1]))
		valueStruct[0] = 1;

	else if(isNumeric(input[0]))
		valueStruct[0] = 0;

	else
	{
		static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
        return (*(const float *) __nan);
	}

	//Check for exponent
	inLen = input.length();
	if(input.rfind('e') > 0)
	{
		//Get single digit exponent
		valueStruct[3] = charToInt( input[inLen- 1] );

		//Get double digit exponent, or sign
		if( input.isNumeric( input[inLen - 2] ) )
			valueStruct[3] += charToInt( input[inLen - 2] ) * 10;

		else if(input[inLen - 2] == '-')
			valueStruct[3] *= -1;
		
		//Get tripple digit exponent, or sign
		if( input.isNumeric( input[inLen - 3] ) )
			valueStruct[3] += charToInt( input[inLen - 3] ) * 100;

		else if(input[inLen - 3] == '-')
			valueStruct[3] *= -1;
	}

	//Check number
	wholeCount = 0;
	fracCount = 0;
	isFraction = false;
	for(int i = 0; i < inLen; i++)
	{
		if(input.isNumeric(input[i]) && !isFraction)
		{
			valueStruct[1] = (valueStruct[1] * 10) + charToInt(input[i]);
			wholeCount++;
		}

		else if(input.isNumeric(input[i]) && isFraction)
		{
			valueStruct[2] = (valueStruct[2] * 10) + charToInt(input[i]);
			fracCount++;
		}

		//If we find the radix, switch to fraction
		if(input[i] == '.')
			isFraction = true;
	}

	//Final reconstruction
	result = valueStruct[1]; //Whole nums
	
	if(fracCount)
	{
		float tmp = valueStruct[2];
		for(int i = 0; i < fracCount; i++)
			tmp /= 10;

		result += tmp; //Fraction
	}

	if(valueStruct[0] == 1)
		result *= -1; //Sign

	//Exponent
	if(valueStruct[3])
	{
		float tmp = 1.0f;
		if(valueStruct[3] > 0)
		{
			for(int i = 0; i <= valueStruct[3]; i++)
				tmp *= 10;

			result *= tmp;
		}
		else
		{
			for(int i = 0; i <= valueStruct[3]; i++)
				tmp /= 10;

			result *= tmp;
		}
	}

	//Return result
	return result;
}

int rStr::charToInt(const char c)
{
	if(c == '1') return 1;
	else if(c == '2') return 2;
	else if(c == '3') return 3;
	else if(c == '4') return 4;
	else if(c == '5') return 5;
	else if(c == '6') return 6;
	else if(c == '7') return 7;
	else if(c == '8') return 8;
	else if(c == '9') return 9;
	else if(c == '0') return 0;
	else return -1;
}

//--- Return data as c[] string
const char* rStr::c_str() const
{
	return data;
}

//--- Find all instances of the token, split local data into induvidual chunks. Returns array of rStrs
rStr * rStr::split(const char token, int *_num)
{
	return split(data, token, _num);
}

//--- Find all instances of the token, split rStr data into induvidual chunks. Returns array of rStrs
rStr * rStr::split(const rStr &text, const char token, int *_num)
{
	return split(text.data, token, _num);
}

//--- Find all instances of the token, split c_str data into induvidual chunks.
//--- Returns array of rStrs, or original text if no tokens are found
rStr* rStr::split(const char* text, const char token, int *_num)
{
	int count, curCount, textLength, chunkLength, pos, lastPos;
	rStr *result;
	char *tempText;

	textLength = r_strlen(text);
	tempText = new char[textLength]; //Incase we have a LONG chunk

	//Find token count
	count = 0;
	for(pos = 0; pos < textLength; pos++)	
	{
		if(text[pos] == token)
			count++;
	}

	
	count++;
	*_num = count;

	//Begin populating rStr arrays with chunks
	if(count)
	{
		curCount = 0;
		lastPos = 0;
		result = new rStr[count];

		for(pos = 0; pos <= textLength; pos++)
		{
			if(text[pos] == token || pos == textLength)
			{
				chunkLength = pos - lastPos;

				//If chunk length is 0, don't copy memory or it'll copy junk
				if(chunkLength == 0)
				{
					result[curCount].clear();
				}

				//Special case if we have only 1 character
				else if(chunkLength == 1)
				{
					r_memcopy(tempText, text + lastPos, chunkLength);
					tempText[chunkLength] = '\0';
					result[curCount].append(tempText);
				}

				//Otherwise copy memory
				else
				{
					r_memcopy(tempText, text + lastPos, chunkLength - 1);

					//When reading from a text file and chunking based on newlines, the last chunk might cause issues
					//Putting this here to see if I can catch null characters
					for(int curChar = 0; curChar < chunkLength; curChar++)
					{
						if(tempText[curChar] <= 0)
						{
							chunkLength = curChar;
							break;
						}
					}

					tempText[chunkLength] = '\0';
					result[curCount].append(tempText);
				}

				lastPos = pos + 1; //Skip token
				curCount++;
			}
		}

		delete[] tempText;
		return result;
	}

	else
	{
		delete[] tempText;
		result = new rStr[1];
		result[0] = text;
		return result;
	}
}

//Float and double to string
//Uses a static rStr variable because data gets deinialized before it's retured
//I could use a basic dynamic array, but rStrs are much more convenient :(
const char* rStr::floatToChar(const float inputNumber, const int precisionWidth)
{
#ifndef _RENSTR_
	return "** rStr library missing, or undefined **"; //We're using rStr, make sure we have them
#endif
	//Limit precision to float
	if(precisionWidth != 8)
		return "** Precision not supported yet! **";

	//--- Variables
	int rN, fN, rL, fL, eL;			//Rational num, fraction num, rational length, fraction length, exponent length
	float tmpNum, tmpNum2, frac;	//A local copy of inputNumber, only positive
	bool isNeg, isLong, isShort;	//Keep track of negative, and if an exponent is needed

	static rStr result;				//Keep variable outside of local scope, or it returns garbage
	rStr rational;					//Store rational numbers
	rStr fraction;					//Store fraction numbers (as rational)

	//--- Check sign
	if(inputNumber < 0)
	{
		tmpNum = inputNumber * -1;
		tmpNum2 = tmpNum;
		isNeg = true;
	}
	else
	{
		tmpNum = inputNumber;
		tmpNum2 = tmpNum;
		isNeg = false;
	}

	//--- Seperate real and fraction numbers, then normalize into an integer
	//Extract rational and fraction
	for(rL = 0; tmpNum > 0 ; rL++)
	{
		if(tmpNum > 2147483647)
			tmpNum = tmpNum / 10;
		else
			tmpNum = (int)tmpNum / 10; //Treat our float like an integer when less than __int32
	}

	if( rL > 0 && rL < precisionWidth )
	{
		//Reset
		isLong = false;
		isShort = false;
		fL = precisionWidth - rL;
		rN = (int)tmpNum2;
		frac = tmpNum2 - rN;		

		//Extract fraction based on remaining precision
		for(int i = 0; i < fL; i++)
			frac *= 10;

		fN = (int)frac;

		rational.append(rational.intToChar(rN));
		fraction.append(fraction.intToChar(fN));
	}

	//Assume number won't fit in an int, or long, or long long, etc
	else
	{
		//Fractional number only
		isLong = false;
		isShort = false;
		frac = tmpNum2;
		tmpNum = tmpNum2;
		if(rL == 0)
		{
			for(fL = 0; frac < 1; fL++)
				frac *= 10;

			if(fL < precisionWidth)
				for(int i = 0; i < (precisionWidth - fL); i++)
					frac *= 10; //This is premultiplied to 1.0 from above

			else
				for(int i = 0; i < precisionWidth; i++)
					frac *= 10; //This is premultiplied to 1.0 from above

			if(fL > 3)
			{
				isShort = true;
				eL = fL; //Exponent outside precision
			}

			fN = (int)frac;
			fL = precisionWidth;

			rational.append('0');
			fraction.append(fraction.intToChar(fN));
		}

		//No fractions		
		else
		{
			for(int i = 0; i < (rL - precisionWidth); i++)
				tmpNum /= 10;

			if(rL > precisionWidth)
			{
				isLong = true;
				eL = --rL; //Exponent outside precision
			}

			rN = (int)tmpNum;
			rL = precisionWidth;

			rational.append(rational.intToChar(rN));
			fraction.append('0');
		}
	}

	result.clear(); //Because it is static
	result.append(rational);
	result.append(fraction);

	if(isNeg)
	{
		if(isLong && !isShort)
		{
			result.insert('-', 0);
			result.insert('.', 2);
			result.append("e+");
			result.append(result.intToChar(eL));
		}

		else if(!isLong && isShort)
		{
			result.insert('-', 0);
			result.insert('.', 2);
			result.append("e-");
			result.append(result.intToChar(eL));
		}

		else
		{
			result.insert('-', 0);
			result.insert('.', rational.length());
		}
	}

	else
	{
		if(isLong && !isShort)
		{
			result.insert('.', 1);
			result.append("e+");
			result.append(result.intToChar(eL));
		}

		else if(!isLong && isShort)
		{
			result.insert('.', 1);
			result.append("e-");
			result.append(result.intToChar(eL));
		}

		else
		{
			result.insert('.', rational.length());
		}
	}

	//Return result
	return result.c_str();
}

//--- Convert an integer to a char array without external libraries
const char* rStr::intToChar(const signed int inputNumber)
{
	int i, j, dividend, power, n;
	int nLength = 0;
	char c;
	bool isPositive = true;
	char *result;

	//Max size
	if(inputNumber >= 2147483647)
		return "2147483647\0";

	else if(inputNumber <= -2147483647)
		return "-2147483647\0";

	//Determine sign, and copy number into a work variable
	//We can only work on positive numbers
	if(inputNumber < 0)
	{
		n = inputNumber * -1;
		isPositive = false;
	}

	else
		n = inputNumber;

	//Determine length of number
	while(n > 0)
	{
		n = n / 10;
		nLength++;
	}

	if(!isPositive)
		n = inputNumber * -1;

	else
		n = inputNumber;


	result = new char[nLength + 1];

	//Expose each place number by dividing by a power
	for(i = 0, j = nLength; j >= 0; j--, i++)
	{
		power = rPow10(j - 1);
		dividend = n / power;
		c = 48 + dividend;

		//Remove highest order number
		n = n - (dividend * power);

		//Assign to array
		if(!isPositive && !i)
			result[i] = '-';

		if(!isPositive)
			result[i + 1] = c;

		else
			result[i] = c;
	}

	result[nLength] = '\0';
	return result;
}

//--- Base 10 power function
int rStr::rPow10(const int exp)
{
	if(exp < 1)
		return 1;

	int base = 10;
	for(int i = 1; i < exp; i++)
		base *= 10;

	return base;
}

//-------------------------------------------
//--- Operators
//-------------------------------------------

//--- Opperator =
void rStr::operator=(const rStr &text)
{
	int l = text.length();
	ensureAlloced(l + 1, false);
	for(int i = 0; i < l; i++)
		data[i] = text[i];
	data[l] = '\0';
	len = l;
}

void rStr::operator=(const char* text)
{
	int l;
	if(!text)
	{
		ensureAlloced(1, false);
		data[0] = '\0';
		len = 0;
		return;
	}

	if(text == data)
		return;

	l = r_strlen(text);
	ensureAlloced(l + 1, false);
	rStr::r_memcopy_asm(data, text, l);

	len = l;
}

//--- Opperator []
char rStr::operator[](int index) const
{
	if(index >= 0 && index <= len)
		return data[index];

	else if(index > len)
		return data[len];

	else if(index < 0)
		return data[0];

	else
		return data[0];
}

char& rStr::operator[](int index)
{
	if(index >= 0 && index <= len)
		return data[index];

	else if(index > len)
		return data[len];

	else if(index < 0)
		return data[0];

	else
		return data[0];
}

//--- Equality
bool operator==(const char* a, const rStr &b)
{
	if(a == b.data)
		return true;
	else
		return false;
}

bool operator==(const rStr &a, const char* b)
{
	if(a.data == b)
		return true;
	else
		return false;
}

bool operator==(const rStr &a, const rStr &b)
{
	if(a.data == b.data)
		return true;
	else
		return false;
}

//--- Not equality
bool operator!=(const char* a, const rStr &b)
{
	if(a != b.data)
		return true;
	else
		return false;
}

bool operator!=(const rStr &a, const char* b)
{
	if(a.data != b)
		return true;
	else
		return false;
}

bool operator!=(const rStr &a, const rStr &b)
{
	if(a.data != b.data)
		return true;
	else
		return false;
}

//--- Addition text
rStr operator+(const rStr &a, const rStr &b)
{
	rStr result(a);
	result.append(b);
	return result;
}

rStr operator+(const char* a, const rStr &b)
{
	rStr result(a);
	result.append(b);
	return result;
}

rStr operator+(const rStr &a, const char* b)
{
	rStr result(a);
	result.append(b);
	return result;
}

//--- Addition other
rStr operator+(const rStr &a, const char b)
{
	rStr result(a);
	result.append(b);
	return result;
}

rStr operator+(const rStr &a, const signed int b)
{
	rStr result( a );
	result.append( result.intToChar(b) );
	return result;
}

rStr operator+(const rStr &a, const float b)
{
	rStr result( a );
	result.append( result.floatToChar(b) );
	return result;
}

rStr operator+(const rStr &a, const double b)
{
	rStr result( a );
	result.append( result.floatToChar( (float)b) );
	return result;
}

rStr operator+( const rStr &a, const bool b )
{
	rStr result( a );
	result.append( b ? "true" : "false" );
	return result;
}

//--- Addition equals other
rStr &rStr::operator+=( const rStr &a )
{
	append( a );
	return *this;
}

rStr &rStr::operator+=( const char *a )
{
	append( a );
	return *this;
}

rStr &rStr::operator+=( const char a )
{
	append( a );
	return *this;
}

rStr &rStr::operator+=( const signed int a )
{
	append(intToChar(a));
	return *this;
}

rStr &rStr::operator+=( const float a )
{
	append(floatToChar(a));
	return *this;
}

rStr &rStr::operator+=( const double a )
{
	append(floatToChar( (float)a) );
	return *this;
}

rStr &rStr::operator+=( const bool a )
{
	append( a ? "true" : "false" );
	return *this;
}

//-------------------------------------------
//--- Version
//-------------------------------------------
char* rStr::rStrVer(void)
{
	return _RENSTR_VER_;
}

//-------------------------------------------
//--- Assmebly
//-------------------------------------------

/* Moved to separate files for organization */
/* Str_ASM.cpp = Scalar assembly            */
/* Str_MMX.cpp = MMX optimized assembly     */
/* Str_SSE.cpp = SSE optimized assembly     */
/* Str_AVX.cpp = AVX optimized assembly     */