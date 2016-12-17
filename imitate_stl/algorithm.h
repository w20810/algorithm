//@2016/12/2
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cstdio> //for printf to debug

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

	template <typename Iterator, typename Compare>
	void insert_sort(Iterator first, Iterator last, Compare cmp)
	{
		_insert_sort(first, last, *first, cmp);
	}

	template<typename Iterator, typename value_type, typename Compare> //randomAccessIterator
	void _insert_sort(Iterator first, Iterator last, value_type, Compare cmp)
	{	
		if (last - first < 1)
			return ;	
		for (Iterator i = first + 1; i != last; ++i)
		{
			Iterator j = i - 1;
			value_type tmp = *i;
			while (j + 1 != first && cmp(tmp, *j))
			{
				*(j + 1) = *j;
				--j;
			}
			*(j + 1) = tmp;
		}
	}

	static const int __stl_threshold = 16;
	
	template <typename Size>
	Size __lg(Size n)
	{
		Size result = 0;
		while (n)
		{
			++result;
			n >>= 1;
		}
		return result;
	}

	template <typename Iterator, typename Compare>
	bool isMedian(Iterator x, Iterator p1, Iterator p2, Compare cmp)
	{
		if ((!cmp(*x, *p1) && !cmp(*p2, *x))
			|| (!cmp(*x, *p2) && !cmp(*p1, *x))) // x >= p1 && p2 >= x || x>= p2 && p1 >=x
				return true;
		return false;
	}

	template <typename Iterator, typename Compare, typename value_type, typename Size>
	void __sort(Iterator first, Iterator last, Compare cmp, value_type, Size depth, const Size maxDepth)
	{
		if (last - first <= __stl_threshold) //个数小于16调用插入排序
		{
			insert_sort(first, last, cmp);
			return ;
		}
		if (depth > maxDepth)//深度太大，调用堆排序
		{
			make_heap(first, last, cmp); //建堆
			sort_heap(first, last, cmp); //堆排
			return ;
		}

		Iterator median = first;
		Iterator mid = first + (last - first) / 2;
		if (isMedian(mid, first, last -1, cmp))
		{
			yxSTL::swap(*mid, *median);	
		}
		else if (isMedian(last - 1, first, mid, cmp))
		{
			yxSTL::swap(*(last - 1), *median);
		}

		Iterator i = first, j = last - 1;
		value_type pivot = *first;
		//printf("%d %d [%d]\n", depth, maxDepth, last - first);
		while (i != j)
		{
			while (i != j && !cmp(*j, pivot)) // j >= pivot
				--j;
			yxSTL::swap(*i, *j);
			while (i != j && !cmp(pivot, *i)) // pivot >= i
				++i;
			yxSTL::swap(*i, *j);
		}
		*i = pivot;
		__sort(first, i, cmp, *first, depth + 1, maxDepth);
		__sort(i + 1, last, cmp, *first, depth + 1, maxDepth);
	}

	template <typename Iterator, typename value_type>
	void _sort(Iterator first, Iterator last, value_type)
	{
		__sort(first, last, less<value_type>(), *first, first - first, __lg(last - first));
	}

	template <typename Iterator>
	void sort(Iterator first, Iterator last)
	{
		_sort(first, last, *first);
	}

	template <typename Iterator, typename Compare>
	void sort(Iterator first, Iterator last, Compare cmp)
	{
		__sort(first, last, cmp, *first, first - first, __lg(last - first));
	}

}//end yxSTL

#endif      
