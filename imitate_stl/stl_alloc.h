/*
 *先使用简单版，以后改成内存池
 *＠2016/12/1
 *
 * */

#ifndef STL_ALLOC_H
#define STL_ALLOC_H

#ifndef CSTDDEF__
#define CSTDDEF__
#include <cstddef>
#endif


namespace yxSTL
{
	/*
	 *负责分配内存和销毁内存
	 *
	 * */
	template <typename T>
	class allocator
	{
		public :
			typedef T			value_type;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef size_t		size_type;
			typedef ptrdiff_t	different_type;

		public :
			static pointer allocate()
			{
				return _allocate((different_type)1, (pointer)0);
			}
			
			static pointer allocate(size_type n, const void* hint = 0)
			{
				//调用的时候alloc.allocate(n)
				//产生n个value_type对象
				return _allocate((different_type)n, (pointer)0);
			}
			
			static void deallocate(pointer ptr)
			{
				//销毁一个value_type对象
				_deallocate(ptr);
			}
			
			static void deallocate(pointer ptr, size_type n)
			{
				
			}
		private:
			template <typename TP>
			static inline TP* _allocate(ptrdiff_t size, TP*)
			{
				TP* tmp = (TP*)(::operator new ((size_t)(size * sizeof(T))));
				return tmp;
			}
			
			template <typename TP>
			static inline void _deallocate(TP* buffer)
			{
				::operator delete (buffer);
			}

			template <typename TP>
			static inline void _deallocate(TP* begin, TP* end)
			{
			
			}
			
	};
}

#endif
