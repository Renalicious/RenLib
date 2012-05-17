//---------------------------------------------------------------
// Name: Ren's string dictionary library
//
// Light weight string dictionary library, uses strings for both keys and values
// It is meant to be standalong, not relying on STL
// Computation speed may be sacrificed for minimal code size
//
// This dictionary is a container of two lists: keys-values
// Keys are always unique, right now they can be anything... so that may change. 
//---------------------------------------------------------------

const int _DEF_DICT_GRAN_ = 4;

//---------------------------------------------------------------
//---
//--- Dictionary class
//---
//---------------------------------------------------------------
template<class key_t, class val_t>
class rDict
{
public:
					rDict(int newGran = _DEF_DICT_GRAN_);
					rDict(const rDict &other);
					~rDict<key_t, val_t>(void);

	//Sizes and memory
	const int		getNum(void) const;										//Get number of elements in list
	const int		getSize(void) const;									//Get how many elements are allocated for
	int				getNum(void); 
	int				getSize(void);

	size_t			getDictSize(void);										//Get size of type
	size_t			getMemUsed(void);										//Get size of allocated list, based on size * num
	size_t			getMemAlloced(void);									//Get size of allocated memory, sizeof * size

	//Resizing
	void			condense(void);											//Reallocate memory down to number of elements
	void			ensureAlloced(const int newSize, bool keepOld = true);	//Make sure we have enough memory allocated

	//Changing data
	void			append(const key_t &key, const val_t &val);				//Add new (unique) key / val at end
	void			insert(const key_t &key, const val_t &val, const int index); //Insert new (unique) key / val at index
	void			remove();												//Remove last key / val pair
	void			removeKey(const key_t &key);							//Remove specific key
	void			removePos(const int index);								//Remove specific index

	//Locating data
	const int		findKey(const key_t &key);								//Find index of key
	const int		findVal(const val_t &val);								//Find first index of val
	const int		find(const key_t &key, const val_t & val) const;		//Find specific key / val pair
	const int*		indexesOf(const val_t &val) const;						//Return all index of matched values

	//Returning data
	const key_t&	getKey(const int index);								//Return key at index
	const val_t&	getVal(const int index);								//Return value at index
	const key_t&	getKey(const int index) const;
	const val_t&	getVal(const int index) const;

	//Whole operations
	void			clear(void);											//Reset list to zero
	void			empty(void);											//Empty list without resizing or reallocating
	bool			isClear(void);											//Is the dictionary NULL?
	bool			isEmpty(void);											//Are the number of elements 0?
	bool			isClear(void) const;
	bool			isEmpty(void) const;
	
	bool			appendDict(const rDict<key_t, val_t> &other);			//Append another dictionary to local
	bool			replaceDict(const rDict<key_t, val_t> &other);			//Replace local dictionary with anther one

	//Operators
	bool			operator==(const rDict &dict2);
	friend bool		operator==(const rDict &dict1, const rDict &dict2);

protected:
	key_t*			keys;
	val_t*			vals;

	int				num;
	int				size;
	int				granularity;

	void			reAllocate(const int newSize, bool keepOld);
};

//-------------------------------------------
//--- Constructors
//-------------------------------------------
template<class key_t, class val_t>
rDict<key_t, val_t>::rDict(int newGran)
{
	keys = NULL;
	vals = NULL;
	granularity = newGran;
	clear();
}

template<class key_t, class val_t>
rDict<key_t, val_t>::rDict(const rDict<key_t, val_t> &other)
{
	keys = NULL;
	vals = NULL;
	clear();

	*this = other;
}

template<class key_t, class val_t>
rDict<key_t, val_t>::~rDict(void)
{
	clear();
}

//-------------------------------------------
//--- Sizes and memory
//-------------------------------------------
template<class key_t, class val_t>
const int rDict<key_t, val_t>::getNum(void) const
{
	return num;
}

template<class key_t, class val_t>
const int rDict<key_t, val_t>::getSize(void) const
{
	return size;
}

template<class key_t, class val_t>
int rDict<key_t, val_t>::getNum(void)
{
	return num;
}

template<class key_t, class val_t>
int rDict<key_t, val_t>::getSize(void)
{
	return size;
}

template<class key_t, class val_t>
size_t rDict<key_t, val_t>::getDictSize(void)
{
	return sizeof(key_t) + sizeof(val_t);
}

template<class key_t, class val_t>
size_t rDict<key_t, val_t>::getMemUsed(void)
{
	return (sizeof(key_t) + sizeof(val_t)) * num;
}

template<class key_t, class val_t>
size_t rDict<key_t, val_t>::getMemAlloced(void)
{
	return (sizeof(key_t) + sizeof(val_t)) * size;
}

//-------------------------------------------
//--- Resizing
//-------------------------------------------
template<class key_t, class val_t>
void rDict<key_t, val_t>::condense(void)
{
	//Make sure lists are not NULL, and we have elements
	if(keys && vals && num)
		reAllocate(num, true);

	else
		clear();
}

template<class key_t, class val_t>
void rDict<key_t, val_t>::ensureAlloced(const int newSize, bool keepOld)
{
	if(newSize > size)
		reAllocate(newSize, keepOld);
}

template<class key_t, class val_t>
void rDict<key_t, val_t>::reAllocate(const int newSize, bool keepOld)
{
	int i, mod;
	key_t* newKeys;
	val_t* newVals;

	mod = newSize % granularity;
	if(mod == 0)	size = newSize;
	else			size = newSize + granularity - mod;

	newKeys = new key_t[size];
	newVals = new val_t[size];

	//Copy old list into new one, as long as old list is not NULL
	if(keepOld)
	{
		if(keys)
			for(i = 0; i < num; i++)
				newKeys[i] = keys[i];

		if(vals)
			for(i = 0; i < num; i++)
				newVals[i] = vals[i];
	}

	if(keys && keys != NULL)
	{
		delete[] keys;
	}

	if(vals && vals != NULL)
	{
		delete[] vals;
	}

	keys = newKeys;
	vals = newVals;
}

//-------------------------------------------
//--- Changing data
//-------------------------------------------
template<class key_t, class val_t>
void rDict<key_t, val_t>::append(const key_t &key, const val_t &val)
{
	if(!keys)
	{
		ensureAlloced(granularity, false);
	}

	//Append on if it's not a duplicate
	if(findKey(key) == -1)
	{
		//Allocate memory, keep elements, add new value to end, increment number of elements by 1
		ensureAlloced(num + 1, true);
		keys[num] = key;
		vals[num] = val;
		num++;
	}
}

template<class key_t, class val_t>
void rDict<key_t, val_t>::insert(const key_t &key, const val_t &val, const int index)
{
	int pos;

	if(!keys)
	{
		ensureAlloced(granularity, false);
	}

	//Insert only if it's not a duplicate
	if(findKey(key) == -1)
	{
		if(index < 0)
			pos = 0;

		else if(index > num)
			append(key, val);

		else
		{
			pos = index;

			//Increment number of elements by 1
			//Allocate memory, keep elements, add new value
			num++;
			ensureAlloced(num + 1, true);
			for(int i = num; i > pos; i--)
			{
				keys[i] = keys[i - 1];
				vals[i] = vals[i - 1];
			}

			keys[pos] = key;
			vals[pos] = val;
		}
	}	
}

//Remove last key / val;
template<class key_t, class val_t>
void rDict<key_t, val_t>::remove(void)
{
	if(num)
		num--;

	else
		clear();
}

//Remove a specific key
template<class key_t, class val_t>
void rDict<key_t, val_t>::removeKey(const key_t &key)
{
	int pos;

	//Make sure we have data
	if(num)
	{
		//Check if we have a match
		pos = findKey(key);
		if(pos >= 0)
		{
			//Remove key and value
			for(int i = num; i > pos; i--)
			{
				keys[i - 1] = keys[i];
				vals[i - 1] = vals[i];
			}

			num--;
		}
	}

	else
		clear();
}

template<class key_t, class val_t>
void rDict<key_t, val_t>::removePos(const int index)
{
	int pos;

	//Make sure we have data
	if(num)
	{
		if(index < 0)
			pos = 0;

		else if(index > num)
			remove();

		else
		{
			pos = index;

			//Remove key and value
			for(int i = num; i > pos; i--)
			{
				keys[i - 1] = keys[i];
				vals[i - 1] = vals[i];
			}

			num--;
		}
	}

	else
		clear();
}

//-------------------------------------------
//--- Locating data
//-------------------------------------------
template<class key_t, class val_t>
const int rDict<key_t, val_t>::findKey(const key_t &key)
{
	int i;

	if(!num)
		return -1;

	else
	{
		for(i = 0; i < num; i++)
			if(keys[i] == key)
				return i;
	}

	return -1;
}

template<class key_t, class val_t>
const int rDict<key_t, val_t>::findVal(const val_t &val)
{
	int i;

	if(!num)
		return -1;

	else
	{
		for(i = 0; i < num; i++)
			if(vals[i] == val)
				return i;
	}

	return -1;
}

template<class key_t, class val_t>
const int rDict<key_t, val_t>::find(const key_t &key, const val_t & val) const
{
	int i;

	if(!num)
		return -1;

	else
	{
		for(i = 0; i < num; i++)
			if(keys[i] == key && vals[i] == val)
				return i;
	}

	return -1;
}

//Find indexes of all values in dictionary
//Returns -1 if not found,
//Returns int array with: element 0 == # of matches, elements 1 to n == indexes
template<class key_t, class val_t>
const int* rDict<key_t, val_t>::indexesOf( const val_t & val) const
{
	int i, count;
	int *result;

	if(!num)
		return -1;

	//Search for matches
	else
	{
		for(i = 0; i < num; i++)
			if(vals[i] == val)
				count++;

		//If we have matches, create our array, and populate with index
		if(count)
		{
			//First element contains number of matches
			result = new int[count + 1];
			result[0] = count; 
			count = 0;

			//Find indexes in this loop
			for(i = 0; i < num; i++)
			{
				if(vals[i] == val)
				{
					result[count + 1] = i;
					count++;
				}
			}

			return result;
		}
	}

	return -1;
}

//-------------------------------------------
//--- Returning data
//-------------------------------------------
template<class key_t, class val_t>
const key_t& rDict<key_t, val_t>::getKey(const int index)
{
	int pos;

	if(num)
	{
		if(index < 0)
			pos = 0;

		else if(index > num)
			pos = num;

		else
			pos = index;

		return keys[pos];
	}

	else
	{
		return NULL; //This might break things, need to find a better solution
	}
}

template<class key_t, class val_t>
const val_t& rDict<key_t, val_t>::getVal(const int index)
{
	int pos;

	if(num)
	{
		if(index < 0)
			pos = 0;

		else if(index > num)
			pos = num;

		else
			pos = index;

		return vals[pos];
	}

	else
	{
		return NULL; //This might break things, need to find a better solution
	}
}


template<class key_t, class val_t>
const key_t& rDict<key_t, val_t>::getKey(const int index) const
{
	int pos;

	if(isEmpty())
	{
		return keys[0];
	}

	else
	{
		if(index < 0)
			pos = 0;

		else if(index > num)
			pos = num;

		else
			pos = index;

		return keys[pos];
	}
}

template<class key_t, class val_t>
const val_t& rDict<key_t, val_t>::getVal(const int index) const
{
	int pos;

	if(isEmpty())
	{
		return vals[0];
	}

	else
	{
		if(index < 0)
			pos = 0;

		else if(index > num)
			pos = num;

		else
			pos = index;

		return vals[pos];
	}
}
//-------------------------------------------
//--- Whole operations
//-------------------------------------------
template<class key_t, class val_t>
void rDict<key_t, val_t>::empty(void)
{
	num = 0;
}

template<class key_t, class val_t>
void rDict<key_t, val_t>::clear(void)
{
	if(keys)
		delete[] keys;

	if(vals)
		delete[] vals;

	num = 0;
	size = 0;
	granularity = _DEF_DICT_GRAN_;
}

template<class key_t, class val_t>
bool rDict<key_t, val_t>::isEmpty(void)
{
	return (!num);
}

template<class key_t, class val_t>
bool rDict<key_t, val_t>::isClear(void)
{
	return (!size && !keys && !vals);
}

template<class key_t, class val_t>
bool rDict<key_t, val_t>::isEmpty(void) const
{
	return (!num);
}

template<class key_t, class val_t>
bool rDict<key_t, val_t>::isClear(void) const
{
	return (!size && !keys && !vals);
}

//Append other directory onto local directory
template<class key_t, class val_t>
bool rDict<key_t, val_t>::appendDict(const rDict<key_t, val_t> &other)
{
	int i, otherPos, otherNum, foundDup;

	//Check for self assignment
	if(*this == other)
	{
		return false;
	}

	else
	{
		i = num;
		foundDup = 0;
		otherPos = 0;
		otherNum = other.getNum();

		ensureAlloced(num + otherNum, true);

		while( i < num + otherNum)
		{
			//If we didn't find a duplicate, copy it over
			if( findVal(other.getVal(otherPos)) == -1)
			{
				keys[i] = other.getKey(otherPos);
				vals[i] = other.getVal(otherPos);
				i++;
			}

			//If we did find a duplicate, make a note of it
			else
			{
				foundDup++;
			}

			otherPos++;
		}

		//Set new number of elements, and condense dictionary
		if(i > num)
		{
			num = num + otherNum - foundDup;
			condense();
			return true;
		}

		//We didn't do anything
		else
		{
			return false;
		}
	}
}

template<class key_t, class val_t>
bool rDict<key_t, val_t>::replaceDict(const rDict<key_t, val_t> &other)
{
	//Check for self assignment
	if(*this == other)
	{
		return false;
	}

	else
	{
		clear();

		num = other.getNum();
		ensureAlloced(num, false);
		
		for(int i = 0; i < num; i++)
		{
			keys[i] = other.getKey(i);
			vals[i] = other.getVal(i);
		}

		return true;
	}
}

//-------------------------------------------
//--- Operators
//-------------------------------------------
template<class key_t, class val_t>
bool rDict<key_t, val_t>::operator==(const rDict<key_t, val_t> &dict2)
{
	if( (keys == dict2.keys) && (vals == dict2.vals) )
		return true;

	else
		return false;
}

template<class key_t, class val_t>
bool operator==(const rDict<key_t, val_t> &dict1, const rDict<key_t, val_t> &dict2)
{
	if( (dict1.keys == dict2.keys) && (dict1.vals == dict2.vals) )
		return true;

	else
		return false;
}