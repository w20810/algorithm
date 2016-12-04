#ifndef STACK_H
#define STACK_H

#include "vector.h"

namespace yxSTL
{
	//stack是容器适配器，标准STL的默认实现容器是deque，目前没有实现deque
	//也可以指定list为实现容器
	//stack没有迭代器
	//内存由实现容器来分配
	template <typename T, typename Sequence = vector<T> >
	class stack
	{
		public :
			typedef typename Sequence::value_type		value_type;
			typedef typename Sequence::reference		reference;
			typedef typename Sequence::const_reference	const_reference;
			typedef typename Sequence::size_type		size_type;	

		private :
			Sequence c;

		public :
			stack(){}
			
			~stack(){}

			bool empty()
			{
				return c.empty();
			}

			size_type size()
			{
				return c.size();
			}

			reference top()
			{
				return c.back();
			}

			const_reference top() const
			{
				return c.back();
			}

			void push(value_type value)
			{
				c.push_back(value);
			}

			void pop()
			{
				return c.pop_back();
			}
	};

}//end of yxSTL

#endif
