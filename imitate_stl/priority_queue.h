#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "algorithm.h"
#include "vector.h"
#include "functional.h"

namespace yxSTL
{
	//默认用vector作为底层实现容器，用xxx_heap算法实现
	//内存分配由实现容器分配
	//没有迭代器
	//默认函数对象为less<T>,用来比较大小
	template <typename T, typename Sequence = vector<T>, 
		 typename Compare = less<typename Sequence::value_type> >
	class priority_queue
	{
		public :
			typedef typename Sequence::value_type			value_type;
			typedef typename Sequence::reference			reference;
			typedef typename Sequence::size_type			size_type;
			typedef typename Sequence::const_reference		const_reference;
	
		private :
			Sequence c;
			Compare cmp;
	
		public :
			priority_queue():c(){}
			
			template <typename Iterator>
			priority_queue(Iterator first, Iterator last)
				:c(first, last)
			{
				make_heap(c.begin(), c.end(), cmp);
			}

			bool empty()
			{
				return c.empty();
			}
		
			size_type size()
			{
				return c.size();
			}

			void push(const_reference value)
			{
				c.push_back(value);
				push_heap(c.begin(), c.end(), cmp);
			}

			void pop()
			{
				pop_heap(c.begin(), c.end(), cmp);
				c.pop_back();
			}	

			const_reference top()
			{
				return c.front();
			}

			//debug
			void debug()
			{
				printf("----------------------------------\n");
				for (typename Sequence::iterator it = c.begin(); it != c.end(); ++it)
				{
					printf("%d ", *it);
				}
				printf("\n");
				printf("-----------------------------------\n");
			}
	};

}//end of yxSTL
#endif

