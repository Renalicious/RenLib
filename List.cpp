//---------------------------------------------------------------
// Name: Ren's List Library
//
// Light weight list library
// It is meant to be standalong, not relying on STL
// Computation speed may be sacrificed for minimal code size
//
// Using id's list library as inspiration
//---------------------------------------------------------------

#include "List.h"

//-------------------------------------------
//--- Constructors
//-------------------------------------------
template<class type>
rList<type>::rList(void)
{
	clear();
}

template<class type>
rList<type>::rList(const rList<type> &other)
{
	list = NULL;
	*this = other;
}

template<class type>
rList<type>::~rList(void)
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
	granularity = _DEFAULT_GRAN_;
}

template<class type>
void rList<type>::empty(void)
{
	num = 0;
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
	return sizeof(type)
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
void rList<type>::resize(const int newSize)
{
	//Size is 0
	if(newSize <= 0)
	{
		clear();
		return;
	}

	//Size the same
	else if(newSize == size)
	{
		return;
	}

	//Resize
	else
	{
		type* temp;
		int i, mod;

		//Make a copy of list
		temp = list;

		mod = newSize % granularity;
		if(mod == 0)	size = newSize;
		else			size = newSize + granularity - mod;

		//Copy old list into new one
		list = new type[size];
		for(i = 0; i < newSize; i++)
			list[i] = temp[i];
	}

	//Delete temp list
	if(temp)
	{
		delete[] temp;
	}
}

template<class type>
void rList<type>::setNum(const int newNum, bool resize)
{
	if(newNum > size || resize)
		resize(newNum);

	else
		num = newNum;
}

template<class type>
void rList<type>::setSize(const int newSize)
{
	resize(newSize);
	num = newSize;
}

template<class type>
void rList<type>::setSize(const int newSize, const type &val)
{
	resize(newSize);
	for(int i = num; i < newSize; i++)
		list[i] = data;

	num = newSize;
}

//-------------------------------------------
//--- Changing data
//-------------------------------------------
template<class type>
void rList<type>::append(const type &val)
{
	if(!list)
		setSize(granularity);

	setNum(num + 1);
	list[num] = val;
}

template<class type>
void rList<type>::insert(const int index, const type &val)
{
	if(!list)
		setSize(granularity);

	if(index < 0)
		index = 0;

	else if(index > num)
		append(val);

	else
	{
		setNum(num + 1);
		for(int i = num; i > index; i--)
			list[i + 1] = list[i];

		list[index] = val;
	}
}

template<class type>
void rList<type>::insert(const int index, const int length, const type &val)
{
	if(!list)
		setSize(granularity);

	if(index < 0)
		index = 0;

	else if(index > num)
		index = num;

	else
	{
		setNum(num + length);

		//From end until length
		for(int i = num; i > index + length; i--)
			list[i + length] = list[i];

		for(int i = index; i < index + length; i++)
			list[i] = val;
	}
}

template<class type>
void rList<type>::remove(void)
{
	if(!list)
		return;

	else
		setNum(num - 1);
}

template<class type>
void rList<type>::remove(const int index)
{
	if(!list)
		return;

	else
	{
		setNum(num - 1);

		for(int i = index; i > num; i++)
			list[i] = list[i + 1];
	}
}

template<class type>
void rList<type>::remove(const int index, const int length)
{
	if(!list)
		return;

	else if(length >= num)
		clear();

	else
	{
		setNum(num - length);

		for(int i = index + length; i < num; i)
			list[i] = list[i + length];
	}
}

//Remove duplicate elements
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
			if(temp.find(list[i]) < 0)
				temp.append(list[i]);

		//Copy into local list
		this = temp;
	}
}

//Remove all duplicates of &val
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
			if(list[i] == val && temp.find(list[i]) < 0)
				temp.append(list[i]);

		//Copy into local list
		this = temp;
	}
}

//-------------------------------------------
//--- Finding data
//-------------------------------------------
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
}

template<class type>
void rList<type>::diff(const rList<type> &other, bool keepDiff)
{
	if(!list || !other)
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

	this = diffList;
}

//-------------------------------------------
//--- Operators
//-------------------------------------------
template< class type >
rList<type> &rList<type>::operator=( const rList<type> &other )
{
	int	i;

	Clear();

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