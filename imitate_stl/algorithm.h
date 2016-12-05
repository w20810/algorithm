//@2016/12/2
//
#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace yxSTL
{
	//max
	template <typename T1>
	inline T1 max(const T1& x, const T1& y)
	{
		return x < y ? y : x;
	}

	//fill
	template <typename Iterator, typename T>
	Iterator fill(Iterator first, Iterator last, const T& value)
	{
		Iterator result = first;
		while (first != last)
		{
			*first = value;
			++first;
		}
		return result;
	}

	//copy
	template <typename Iterator>
	Iterator copy(Iterator first, Iterator last, Iterator result)
	{
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}

	template <typename T>
	void swap(T& a, T& b)
	{
		T tmp(a);
		a = b;
		b = tmp;
	}
	
	template <typename Iterator, typename T>
	Iterator find(Iterator first, Iterator last, const T& value)
	{
		Iterator cur = first;
		for (; cur != last; ++cur)
			if (value == *cur)
				break;
		return cur;	
	}

	template <typename RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__push_heap(first, last - first - 1, *(last - 1));
	}
	
	template <typename RandomAccessIterator, typename Distance, typename T>
	void __push_heap(RandomAccessIterator first, Distance holeIndex, T value)
	{
		Distance parentIndex = (holeIndex - 1) / 2;
		while (holeIndex != 0 && *(first + parentIndex) < value)
		{
			*(first + holeIndex) = *(first + parentIndex);
			holeIndex = parentIndex;
			parentIndex = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template <typename RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		*first = *(last - 1);
		__adjust_heap(first, last - first - 1, first - first, *first);
	}

	template <typename RandomAccessIterator, typename Distance, typename T>
	void __adjust_heap(RandomAccessIterator first, Distance len, Distance holeIndex, T value)//从holeIndex处向下调整
	{
		Distance rSonIndex = holeIndex * 2 + 2;
		while (rSonIndex < len)
		{
			if (*(first + rSonIndex) < *(first + (rSonIndex - 1)))
				--rSonIndex;
			if (value < *(first + rSonIndex))
			{
				*(first + holeIndex) = *(first + rSonIndex);
				holeIndex = rSonIndex;
				rSonIndex = holeIndex * 2 + 2;
			}
			else
				break;
		}
		if (rSonIndex - 1 == len)
		{
			if (value < *(first + (rSonIndex - 1)))
			{
				*(first + holeIndex) = *(first + (rSonIndex - 1));
				holeIndex = rSonIndex - 1;
			}
		}
		*(first + holeIndex) = value;
	}

	template <typename RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		while (first - last > 1)
			pop_heap(first, last--);
	}

	template <typename RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		__make_heap(first, last - first);
	}

	template <typename RandomAccessIterator, typename Distance>
	void __make_heap(RandomAccessIterator first, Distance len)
	{	
		Distance curIndex = (len - 2) / 2;
		while (curIndex >= 0)
		{
			__adjust_heap(first, len, curIndex, *(first + curIndex));
			--curIndex;
		}
	}

}//end yxSTL

#endif      
