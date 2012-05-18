//---------------------------------------------------------------
// Name: Ren's List Library
//
// A light weight and standalone list class, not relying on STL.
// Computation speed may be sacrificed for readibility and code size.
//
// Using id's list library as inspiration
//---------------------------------------------------------------

/*
	Changes and versions:
	Version 1.0.0 - May 2012: initial release
*/

#ifndef _RENLIST_
#define _RENLIST_

const int _DEF_LIST_GRAN_ = 4;

#ifndef NULL
#define NULL 0
#endif

template< class type >
class rList
{
public:
					rList(int newGranularity = _DEF_LIST_GRAN_);
					rList(const rList<type> &externalList);
					~rList<type>(void);

	void			clear(void);												//Reset list to zero
	void			empty(void);												//Empty list without resizing or reallocating
	bool			isClear(void);
	bool			isEmpty(void);
	const bool		isClear(void) const;
	const bool		isEmpty(void) const;

	//--- Memory and sizes
	int				getNum(void) const;											//Get number of elements in list
	int				getSize(void) const;										//Get how many elements are allocated for
	size_t			getTypeSize(void) const;									//Get size of type
	size_t			getMemUsed(void) const;										//Get size of allocated list, based on size * num
	size_t			getMemAlloced(void) const;									//Get size of allocated memory, sizeof * size

	//--- Resizing
	void			condense(void);												//Reallocate down to number of elements
	void			ensureAlloced(const int newSize, bool keepOld = true);		//Set number of allocated units to this

	//--- Changing data
	void			append(const type &val);									//Add new element to the end
	void			appendUnique(const type &val);								//Add new element to the end
	void			fill(const int length, const type &val);					//Add N amount of new elements at end
	void			insert(int index, const type &val);							//Insert new element at index
	void			insert(int index, const int length, const type &val);		//Insert N number of new elements at index
	void			remove(void);												//Remove last element
	void			remove(int index);											//Remove element at index, slide the rest down
	void			remove(int index, const int num);							//Remove N elements at indexs, slide the rest down
	void			removeDups(void);											//Remove all duplicates
	void			removeDups(const type &val);								//Remove all duplicates of this element

	//--- Finding data
	int				find(const type &val);										//Find element
	const int		find(const type &val) const;								//Find element
	void			diff(const rList<type> &otherList, bool keepDiff = true);	//Get diff between local and other
	rList<type>&	diff(const rList<type> &list1, const rList<type> &list2, bool keepDiff = true);	//Get diff between list1 and list2
	

	//--- Operators
	rList<type>&	operator=( const rList<type> &other );
	const type&		operator[]( int index ) const;
	type&			operator[]( int index );

private:
	int				num;					//Number of items in the list
	int				size;					//Number of items allocated, but not initialized
	int				granularity;			//Quantize size allocation to this value (Not necessarly bits)
	type*			list;					//Our data

	void			reAllocate(const int newSize, bool keepOld);						//Allocate new units
};

//-------------------------------------------
//--- Constructors
//-------------------------------------------
template<class type>
rList<type>::rList(const int newGranularity)
{
	list		= NULL;
	granularity	= newGranularity;
	clear();
}

template<class type>
rList<type>::rList(const rList<type> &other)
{
	list = NULL;
	*this = other;
}

template< class type >
rList<type>::~rList( void )
{
	clear();
}

template<class type>
void rList<type>::clear(void)
{
	if(list)
		delete[] list;

	list = NULL;
	num = 0;
	size = 0;
	granularity = _DEF_LIST_GRAN_;
}

template<class type>
void rList<type>::empty(void)
{
	num = 0;
}

template<class type>
bool rList<type>::isClear(void)
{
	return(!list);
}

template<class type>
bool rList<type>::isEmpty(void)
{
	return(!num);
}

template<class type>
const bool rList<type>::isClear(void) const
{
	return(!list);
}

template<class type>
const bool rList<type>::isEmpty(void) const
{
	return(!num);
}
//-------------------------------------------
//--- Memory and size
//-------------------------------------------
template<class type>
int rList<type>::getNum(void) const
{
	return num;
}

template<class type>
int rList<type>::getSize(void) const
{
	return size;
}

template<class type>
size_t rList<type>::getTypeSize(void) const
{
	return sizeof(type);
}

template<class type>
size_t rList<type>::getMemUsed(void) const
{
	return sizeof(type) * num;
}

template<class type>
size_t rList<type>::getMemAlloced(void) const
{
	return sizeof(type) * size;
}

//-------------------------------------------
//--- Resizing
//-------------------------------------------
template<class type>
void rList<type>::condense(void)
{
	if(list)
	{
		if(num)
			reAllocate(num, true);

		else
			clear();
	}	
}

template<class type>
void rList<type>::reAllocate(const int newSize, bool keepOld)
{
	int i, mod;
	type* newList;

	mod = newSize % granularity;
	if(mod == 0)	size = newSize;
	else			size = newSize + granularity - mod;

	newList = new type[size];

	//Copy old list into new one, as long as old list is not NULL
	if(list && keepOld)
	{
		for(i = 0; i < num; i++)
			newList[i] = list[i];
	}

	if(list && list != NULL)
	{
		delete[] list;
	}

	list = newList;
}

template<class type>
void rList<type>::ensureAlloced(const int newSize, bool keepOld)
{
	if(newSize > size)
		reAllocate(newSize, keepOld);
}

//-------------------------------------------
//--- Changing data
//-------------------------------------------
template<class type>
void rList<type>::append(const type &val)
{
	//If list is empty, allocate some memory
	if(!list)
		ensureAlloced(granularity, false);

	ensureAlloced(num + 1, true);
	list[num] = val;
	num++;
}

template<class type>
void rList<type>::appendUnique(const type &val)
{
	//If list is empty, allocate some memory
	if(!list)
		ensureAlloced(granularity, false);

	if(find(val) == -1)
	{
		ensureAlloced(num + 1, true);
		list[num] = val;
		num++;
	}
}

//Allocate and fill elements at the end of list.
// When working with large lists it's MUCH faster to prefill and then assign elements,
// than to keep appending new elements
template<class type>
void rList<type>::fill(const int length, const type &val)
{
	//If list is empty, allocate enough memory
	if(!list)
		ensureAlloced(length, false);

	ensureAlloced(num + length, true);
	for(int i = num; i < num + length; i++)
		list[i] = val;

	num += length;
}

template<class type>
void rList<type>::insert(int index, const type &val)
{
	//If list is empty, allocate some memory
	if(!list)
		ensureAlloced(granularity, false);

	if(index < 0)
		index = 0;

	else if(index > num)
		append(val);

	num++;
	ensureAlloced(num, true);

	for(int i = num; i > index; i--)
		list[i] = list[i - 1];

	list[index] = val;
}

template<class type>
void rList<type>::insert(int index, const int length, const type &val)
{
	//If list is empty, allocate some memory
	if(!list)
		ensureAlloced(granularity, false);

	if(index < 0)			index = 0;
	else if(index > num)	index = num;

	num += length;
	ensureAlloced(num, true);

	//From end until length
	for(int i = num; i > index + length; i--)
		list[i] = list[i - length];

	for(int i = index; i < index + length; i++)
		list[i] = val;
}

template<class type>
void rList<type>::remove(void)
{
	if(!list)
		return;

	else
		num--;
}

template<class type>
void rList<type>::remove(int index)
{
	if(!list)
		return;

	if(index < 0)
		index = 0;

	else if(index > num)
		index = num;

	else
	{
		num--;
		for(int i = index; i < num; i++)
			list[i] = list[i + 1];
	}
}

template<class type>
void rList<type>::remove(int index, const int length)
{
	if(!list)
		return;

	else if(length >= num)
		clear();

	if(index < 0)
		index = 0;

	else if(index > num)
		index = num;

	else
	{
		num -= length;
		for(int i = index + length; i < num; i++)
			list[i] = list[i + length];
	}
}

//Remove duplicate elements in local list
template<class type>
void rList<type>::removeDups(void)
{
	if(!list)
		return;

	else
	{
		int i;
		rList temp;

		//Look for entries in temp list, or append to it
		for(i = 0; i < num; i++)
		{
			if(temp.find(list[i]) < 0)
				temp.append(list[i]);
		}

		//Copy into local list
		*this = temp;
	}
}

//Remove all duplicates of &val in local list
template<class type>
void rList<type>::removeDups(const type &val)
{
	if(!list)
		return;

	else
	{
		int i;
		rList temp;

		//Find &val in temp list, or append
		for(i = 0; i < num; i++)
		{
			if(list[i] == val && temp.find(list[i]) < 0)
				temp.append(list[i]);

			else if(list[i] == val && temp.find(list[i]) >= 0)
				continue;

			else
				temp.append(list[i]);
		}

		//Copy into local list
		*this = temp;
	}
}

//-------------------------------------------
//--- Finding data
//-------------------------------------------

//Find, return element of -1
template<class type>
int rList<type>::find(const type &val)
{
	if(!list)
		return -1;

	else
	{
		for(int i = 0; i < num; i++)
			if(list[i] == val)
				return i;
	}

	return -1;
}

template<class type>
const int rList<type>::find(const type &val) const
{
	if(!list)
		return -1;

	else
	{
		for(int i = 0; i < num; i++)
			if(list[i] == val)
				return i;
	}

	return -1;
}

//Get the difference between local and external list, keep only difference / keep only similar
template<class type>
void rList<type>::diff(const rList<type> &other, bool keepDiff)
{
	if(!list || other.isEmpty())
		return;

	int i;
	rList diffList;

	//Keep only matches
	if(!keepDiff)
	{
		for(i = 0; i < num; i++)
			if( other.find(list[i]) >= 0 )
				diffList.append(list[i]);
	}

	//Keep only differences
	else
	{
		for(i = 0; i < num; i++)
			if( other.find(list[i]) == -1 )
				diffList.append(list[i]);
	}

	*this = diffList;
}

//Get the difference between list1 and list2, keep only difference / keep only similar
template<class type>
rList<type>& rList<type>::diff(const rList<type> &list1, const rList<type> &list2, bool keepDiff)
{
	if(!list1 || !list2)
		return;

	int i;
	rList diffList;

	//Keep only matches
	if(!keepDiff)
	{
		for(i = 0; i < num; i++)
			if( list2.find(list1[i]) >= 0 )
				diffList.append(list1[i]);
	}

	//Keep only differences
	else
	{
		for(i = 0; i < num; i++)
			if( list2.find(list1[i]) == -1 )
				diffList.append(list1[i]);
	}

	return diffList;
}

//-------------------------------------------
//--- Operators
//-------------------------------------------
template< class type >
rList<type> &rList<type>::operator=( const rList<type> &other )
{
	int	i;

	clear();

	num			= other.num;
	size		= other.size;
	granularity	= other.granularity;

	if ( size )
	{
		list = new type[ size ];
		for( i = 0; i < num; i++ )
		{
			list[ i ] = other.list[ i ];
		}
	}

	return *this;
}

template< class type >
const type &rList<type>::operator[]( int index ) const
{
	if(index < 0)			index = 0;
	else if (index > num)	index = num;

	return list[ index ];
}

template< class type >
type &rList<type>::operator[]( int index )
{
	if(index < 0)			index = 0;
	else if (index > num)	index = num;

	return list[ index ];
}

#endif