//@2016/12/2
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

	//swap
	template <typename T>
	void swap(T& a, T& b)
	{
		T tmp(a);
		a = b;
		b = tmp;
	}
	
	//find
	template <typename Iterator, typename T>
	Iterator find(Iterator first, Iterator last, const T& value)
	{
		Iterator cur = first;
		for (; cur != last; ++cur)
			if (value == *cur)
				break;
		return cur;	
	}

	
	//heap, 实现priority_queue的底层算法
	//
	template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
	void __push_heap(RandomAccessIterator first, Distance holeIndex, T value, Compare cmp)
	{
		Distance parentIndex = (holeIndex - 1) / 2;
		while (holeIndex != 0 && cmp(*(first + parentIndex), value))
		{
			*(first + holeIndex) = *(first + parentIndex);
			holeIndex = parentIndex;
			parentIndex = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template <typename RandomAccessIterator, typename Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare)
	{
		__push_heap(first, last - first - 1, *(last - 1), Compare());
	}

	template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
	void __adjust_heap(RandomAccessIterator first, Distance len, Distance holeIndex, T value, Compare cmp)//从holeIndex处向下调整
	{
		Distance rSonIndex = holeIndex * 2 + 2;
		while (rSonIndex < len)
		{
			if (cmp(*(first + rSonIndex), *(first + (rSonIndex -1))))
				--rSonIndex;
			if (cmp(value, *(first + rSonIndex)))
			{
				*(first + holeIndex) = *(first + rSonIndex);
				holeIndex = rSonIndex;
				rSonIndex = holeIndex * 2 + 2;
			}
			else
				break;
		}
		if (rSonIndex == len)
		{
			if (cmp(value, *(first + (rSonIndex - 1))))
			{
				*(first + holeIndex) = *(first + (rSonIndex - 1));
				holeIndex = rSonIndex - 1;
			}
		}
		*(first + holeIndex) = value;
	}

	template <typename RandomAccessIterator, typename Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare)
	{
		swap(*first, *(last - 1));
		__adjust_heap(first, last - first - 1, first - first, *first, Compare());
	}

	//FIXME:排序前确保是一个堆	
	template <typename RandomAccessIterator, typename Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare)
	{
		while (last - first > 1)
			pop_heap(first, last--, Compare());
	}

	template <typename RandomAccessIterator, typename Distance, typename Compare>
	void __make_heap(RandomAccessIterator first, Distance len, Compare)
	{	
		Distance curIndex = (len - 2) / 2;
		while (curIndex >= 0)
		{
			__adjust_heap(first, len, curIndex, *(first + curIndex), Compare());
			--curIndex;
		}
	}

	template <typename RandomAccessIterator, typename Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare)
	{
		__make_heap(first, last - first, Compare());
	}

	template <typename T>
	struct less
	{
		bool operator()(const T& a, const T& b)
		{
			return a < b;
		}
	};
}//end yxSTL

#endif      
