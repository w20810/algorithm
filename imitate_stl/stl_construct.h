//@2016/12/1
#ifndef STL_CONSTRUCT_H
#define STL_CONSTRUCT_H

#ifndef __CSTDDEF
#define __CSTDDEF
#include <cstddef>
#endif

namespace yxSTL
{	
	template <typename T1, typename T2>
	inline void construct(T1* p, const T2& value)
	{
		new (p) T1(value);
	}

	//此处没有优化，标准STL里面用traits技法对类型进行了判断，
	//如若此类型的析构是可有可无的，那么直接忽略
	template <typename T>
	inline void destroy(T* ptr)
	{
		ptr->~T();
	}
	
	template <typename T>
	inline void destroy(T* first, T* last)
	{
		while (first != last)
		{
			first->~T();
			++first;
		}
	}	
};

#endif
