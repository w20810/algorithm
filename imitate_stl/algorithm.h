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

}//end yxSTL

#endif      
