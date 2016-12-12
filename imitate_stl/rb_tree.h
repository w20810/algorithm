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
	
	//FIXME:为set量身定制，并不支持map(= . =)
	//先写完，日后改进
	template <typename T, typename Compare, typename Alloc = allocator<__rb_tree_node<T> > >
	class rb_tree
	{
		private :
			typedef T			value_type;
			typedef T*			pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef const T*	const_pointer;
			typedef size_t		size_type;	
			
			typedef __rb_tree_node<value_type>* link_type;
			typedef __rb_tree_node<value_type>	node_type;

			typedef Alloc		node_allocator;

		public :
			typedef __rb_tree_iterator<value_type >		iterator;
		
		private :
			size_type		count_node;
			link_type		header;
			Compare			cmp;
		
		private :
			link_type get_node()
			{
				return node_allocator::allocate();
			}

			link_type get_initialized_node(const_reference value)
			{
				link_type result = node_allocator::allocate();
				construct(result, value);
				return result;
			}

			void put_node(link_type ptr)
			{
				destroy(ptr);
				node_allocator::deallocate(ptr);
			}

			void init_tree()
			{
				header = get_node();
				header->father	= header;
				header->lson	= header;
				header->rson	= header;
			}

			link_type& get_root()
			{
				return header->father;
			}
			
			void rotate_left(link_type x)
			{
				link_type y = x->rson;
				x->rson = y->lson;
				if (NULL != y->lson)
					y->lson->father = x;

				//FIXME:旋转的时候header的左右孩子式不可能变的
				//若x为root,那么
				if (x == get_root())
					get_root() = y;
				else if (x == x->father->lson)
					x->father->lson = y;
				else if (x == x->father->rson)
					x->father->rson = y;
				
				x->father = y;
				y->lson = x;
			}

			void rotate_right(link_type x)
			{
				link_type y = x->lson;
				x->lson = y->rson;
				if (NULL != y->rson)
					y->rson->father = x;

				//FIXME:旋转的时候header的左右孩子式不可能变的
				if (x == get_root())
					get_root() = y;
				else if (x == x->father->lson)
					x->father->lson = y;
				else if (x == x->father->rson)
					x->father->rson = y;

				x->father = y;
				y->rson = x;
			}

			void insert_rebalance(link_type x)
			{
				if (x->father->color ) //FIXME:statu(1)
				{

				}
			}

			void erase_rebalance(link_type x)
			{

			}

		public :
			rb_tree():count_node(0), cmp(Compare())
			{
				init_tree();
			}

			template <typename Comp>
			rb_tree(Comp c) :count_node(0), cmp(c)
			{
				init_tree();
			}

			size_type size()
			{
				return count_node;
			}

			iterator begin() //隐式类型转换
			{
				return header->lson;
			}

			iterator end()
			{
				return header;
			}

			value_type max()
			{
				return header->rson->value;
			}

			value_type min()
			{
				return header->lson->value;
			}

			bool empty()
			{
				return count_node == 0;
			}

			void insert_unique(value_type x)
			{
				
			}

			void insert(value_type x)
			{

			}

			void erase(value_type x)
			{
				
			}

			void erase(iterator x)
			{

			}

	};

}//end of yxSTL

#endif
