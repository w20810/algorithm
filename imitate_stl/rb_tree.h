#ifndef RB_TREE_H
#define RB_TREE_H

#include "algorithm.h"
#include "memory.h"

#include <cstdio>

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
			lson(NULL), rson(NULL)
		{
		}
		
		__rb_tree_node(const T& x) :color(__rb_tree_red), father(NULL),
			lson(NULL), rson(NULL), value(x)
		{
		}
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

			bool operator==(const self& x) const
			{
				return pnode == x.pnode;
			}

			bool operator!=(const self& x) const
			{
				return pnode != x.pnode;
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
			//	header->color	= __rb_tree_red; //默认为红色
			}

			link_type get_root()
			{
				if (header->father == header)
					return (link_type)NULL;
				return header->father;
			}
			
			bool hasSon(link_type x)
			{
				return (x->lson || x->rson);
			}

			link_type& father(link_type x)
			{
				return x->father;
			}

			link_type& ancle(link_type x)
			{
			 	if (x->father->father->lson == x->father)
					return x->father->father->rson;
				else
					return x->father->father->lson;
			}

			link_type& grandpa(link_type x)
			{
				return x->father->father;
			}

			link_type& brother(link_type x)
			{
				if (x->father->lson == x)
					return x->father->rson;
				else
					return x->father->lson;
			}

			void rotate_left(link_type x)
			{
				link_type y = x->rson;
				x->rson = y->lson;
				if (NULL != y->lson)
					y->lson->father = x;
				y->father = x->father;

				//FIXME:旋转的时候header的左右孩子式不可能变的
				if (x == get_root())
					header->father = y;
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
				y->father = x->father;

				//FIXME:旋转的时候header的左右孩子式不可能变的
				if (x == get_root())
					header->father = y;
				else if (x == x->father->lson)
					x->father->lson = y;
				else if (x == x->father->rson)
					x->father->rson = y;

				x->father = y;
				y->rson = x;
			}

			//FIXME:NIL节点默认为黑色
			void insert_rebalance(link_type x)//FIXME:X为新节点
			{
				//FIXME:根节点一定为黑色，否则直接改为黑色
				//这点对下面十分有用
				//因为父亲节点为根节点，那么就可以不用迭代了，直接返回
				//因为父节点一定为黑色了
				if (get_root() == x && __rb_tree_red == x->color)
				{
					x->color = __rb_tree_black;
					return ;
				}

				if (__rb_tree_black == father(x)->color) //FIXME:father is black
				{
					return ;
				}
				else //FIXME:father is red
				{
					//NIL node's color is black
					if (ancle(x) && __rb_tree_red == ancle(x)->color)//FIXME:ancle is red
					{
						//grandpa must be black
						ancle(x)->color = __rb_tree_black;
						father(x)->color = __rb_tree_black;

						//modify the grandpa's color to rad
						grandpa(x)->color = __rb_tree_red;
						insert_rebalance(grandpa(x));//iterate to grandpa
						return ;
					}
					else //FIXME:ancle(mabey is NIL) is black
					{
						if (x == father(x)->lson && father(x) == grandpa(x)->lson)
						{
							father(x)->color = __rb_tree_black;
							grandpa(x)->color = __rb_tree_red;
							rotate_right(grandpa(x));
						}
						else if (x == father(x)->rson && father(x) == grandpa(x)->lson)
						{
							x->color = __rb_tree_black;
							grandpa(x)->color = __rb_tree_red;
							rotate_left(father(x));
							rotate_right(father(x));
						}
						else if (x == father(x)->lson && father(x) == grandpa(x)->rson)
						{
							x->color = __rb_tree_black;
							grandpa(x)->color = __rb_tree_red;
							rotate_right(father(x));
							//printf("after rotate_right\n");
							//debug();
							//printf("after rotate_left\n");
							rotate_left(father(x));
							//debug();
						}
						else if (x == father(x)->rson && father(x) == grandpa(x)->rson)
						{
							father(x)->color = __rb_tree_black;
							grandpa(x)->color = __rb_tree_red;
							rotate_left(grandpa(x));
						}
					}
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

			void destroy_tree(link_type x)
			{
				if (x->lson)
					destroy_tree(x->lson);
				if (x->rson)
					destroy_tree(x->rson);
				if (x)
					put_node(x);
			}

			~rb_tree()
			{
				destroy_tree(get_root());	
				put_node(header);
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

			size_type depth()
			{
				return depth(get_root());	
			}

			size_type depth(link_type x)
			{
				if (NULL == x)
					return 0;
				size_type ldepth = depth(x->lson);
				size_type rdepth = depth(x->rson);
				return yxSTL::max(ldepth, rdepth) + (size_type)1;
			}

			iterator find(const_reference x)
			{
				link_type cur = get_root();
				while (cur != NULL)
				{
					if (!cmp(x, cur->value) && !cmp(x, cur->value))
						return cur; //隐式转换
					else if (cmp(x, cur->value))
						cur = cur->lson;
					else
						cur = cur->rson;
				}
				return end();
			}

			iterator insert_unique(const_reference x)
			{
				if (find(x) != end())
					return link_type(NULL);
				return insert(x);
			}

			iterator insert(const_reference x)
			{
				if (NULL == get_root())
				{
					link_type node = get_initialized_node(x);
					node->father = header;
					node->color  = __rb_tree_black;
					header->lson = node;
					header->rson = node;
					header->father = node;
					return node;
				}
				link_type cur = get_root();
				link_type node = get_initialized_node(x);
				while (true)
				{
					if (cmp(x, cur->value))
					{
						if (NULL == cur->lson)
						{
							node->father = cur;
							cur->lson = node;
							break;
						}
						cur = cur->lson;
					}
					else
					{
						if (NULL == cur->rson)
						{
							node->father = cur;
							cur->rson =	node;
							break;
						}
						cur = cur->rson;
					}
				}
				if (node == header->lson->lson)
					header->lson = node;
				if (node == header->rson->rson)
					header->rson = node;
				//printf("insert  %d ok\n", node->value);
				//debug();
				insert_rebalance(node);
				++count_node;
				return node;
			}

			void erase(value_type x)
			{
				
			}

			void erase(iterator x)
			{

			}

			void debug()
			{
				printf("[ \n");
				_debug(get_root());
				printf("]\n");
			}

			void _debug(link_type cur)
			{
				if (cur)
				{
					printf("addr%p (%p %p) value:%d color:%s \n", 
							cur, cur->lson, cur->rson,
						   	cur->value, (cur->color?"black":"red"));
					_debug(cur->lson);
					_debug(cur->rson);
				}
			}
	};
}//end of yxSTL

#endif
