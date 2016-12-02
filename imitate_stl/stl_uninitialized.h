#ifndef  STL_UNINITIALIZED_H
#define  STL_UNINITIALIZED_H
#include "stl_construct.h"

namespace yxSTL
{
	/*
	 *
	 *注意没有将copy函数最优化(调用memcopy)
	 *注意以下工具函数没有用traits技法，调用最优化的copy函数
	 *
	 * */

	//将区间[first, last)的元素拷贝到result开始的区间
	template<typename Iterator>
	Iterator uninitialized_copy(Iterator first, Iterator last, Iterator result)
	{
		Iterator cur = result;
		for (; first != last; ++first, ++cur)
			construct(&*cur, *first);//想想*cur的返回值
		return cur;
	}

	//在区间[first, last)填充T对象
	template <typename Iterator, typename T>
	Iterator uninitialized_fill(Iterator first, Iterator last, const T& value)
	{
		Iterator cur = first;
		for (; cur != last; ++cur)
			construct(&*cur, value);
		return first;
	}

	//从first开始，填充n个T对象
	template <typename Iterator, typename Size, typename T>
	Iterator uninitialized_fill_n(Iterator first, Size n, const T& value)
	{
		Iterator cur = first;
		for (; n > 0; --n, ++cur)
			construct(&*cur, value);
		return cur;
	}

	//将［first, last）的元素逆向拷贝
	template <typename Iterator>
	Iterator copy_backward(Iterator first, Iterator last, Iterator result)
	{
		Iterator cur = last;
		while(cur != first)
			*(--result) = *(--cur);
		return result;
	}

}

#endif
