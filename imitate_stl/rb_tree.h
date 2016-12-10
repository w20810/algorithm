#ifndef RB_TREE_H
#define RB_TREE_H

#include "algorithm.h"
#include "memory.h"

namespace yxSTL
{
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = false;
	const __rb_tree_color_type __rb_tree_black = true;

	template <typename T>
	struct	__rb_tree_node
	{
		typedef __rb_tree_color_type		color_type;

		color_type			color;
		__rb_tree_node		*father;
		__rb_tree_node		*lson;
		__rb_tree_node		*rson;
		T					value;
		__rb_tree_node():color(__rb_tree_red), father(NULL),
			lson(NULL), rson(NULL),T(){}
		
	};
	
	//封装一个指向__rb_tree_node的指针
	//提供自增自减，取址等操作
	template<typename T>
	class __rb_tree_iterator
	{
		public :
			typedef T			value_type;
			typedef T*			pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
		
			typedef __rb_tree_iterator<T>		self;
			typedef __rb_tree_node<value_type>* link_type;
		
			__rb_tree_iterator(): pnode(){}
			__rb_tree_iterator(link_type x): pnode(x){}
			__rb_tree_iterator(const self& x){pnode = x.pnode;}

			reference operator*() const
			{
				return pnode->value;
			}

			//作指针操作
		//	pointer operator->() const
		//	{
		//		return &(operator*());
		//	}
			
			void increment()
			{
				if (NULL != pnode->rson)
				{
					link_type son = pnode->rson;
					while (NULL != son->lson)
						son = son->lson;
					pnode = son;
				}
				else
				{
					link_type fa = pnode->father;
					while (pnode == fa->rson)
					{
						pnode = fa;
						fa = fa->father;
					}
					if (pnode->lson != fa)
						pnode = fa;
					   	//处理pnode一开始就是root,而且无rson的情况
				}
			}

			void decrement()
			{
				//此时pnode在end()处，直接递减到最大值处
				if (__rb_tree_red == pnode->color && pnode == pnode->father->father)
					pnode = pnode->rson;
				else if (NULL != pnode->lson)
				{
					link_type son = pnode->lson;
					while (NULL != son->rson)
						son = son->rson;
					pnode = son;
				}
				else
				{
					link_type fa = pnode->father;
					while (pnode == fa->lson)
					{
						pnode = fa;
						fa = fa->father;
					}
					pnode = fa;
				}
			}

			self& operator++()
			{
				increment();
				return *this;
			}

			const self operator++(int)
			{
				self res = *this;
				increment();
				return res;
			}

			self& operator--()
			{
				decrement();
				return *this;
			}

			const self operator--(int)
			{
				self res = *this;
				decrement();
				return res;
			}

		private :
			link_type	pnode;
	};

}//end of yxSTL

#endif
