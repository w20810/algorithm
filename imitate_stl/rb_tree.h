#ifndef RB_TREE_H
#define RB_TREE_H

#include "algorithm.h"
#include "memory.h"

#include <cstdio> //for debug call printf()

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
			
			link_type get()
			{
				return pnode;
			}

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

			link_type get_next(link_type x) //获取后继节点
			{
				link_type result = x;
				if (NULL != result->rson)
				{
					link_type son = result->rson;
					while (NULL != son->lson)
						son = son->lson;
					result = son;
				}
				else
				{
					link_type fa = result->father;
					while (result == fa->rson)
					{
						result = fa;
						fa = fa->father;
					}
					if (result->lson != fa)
						result = fa;
					   	//处理x一开始就是root,而且无rson的情况
				}
				return result;
			}

			link_type get_pre(link_type x) //获取前驱节点
			{
				//此时x在end()处，直接递减到最大值处
				link_type result = x;	
				if (__rb_tree_red == result->color && result == result->father->father)
					result = result->rson;
				else if (NULL != result->lson)
				{
					link_type son = result->lson;
					while (NULL != son->rson)
						son = son->rson;
					result = son;
				}
				else
				{
					link_type fa = result->father;
					while (result == fa->lson)
					{
						result = fa;
						fa = fa->father;
					}
					result = fa;
				}
				return result;
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

			__rb_tree_color_type color(link_type x)
			{
				if (NULL == x || x->color == __rb_tree_black)
					return __rb_tree_black;
				return __rb_tree_red;
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

			void erase_rebalance(link_type x) //FIXME:x为实际要被删掉的节点
			{
				//FIXME : 前提保证了x一定没有右孩子
				if (x == get_root())
				{
					if (NULL == x->lson) //size == 1
					{
						put_node(x);
						header->father = header;
						header->lson = header;
						header->rson = header;
						return ;
					}
					header->father = x->lson;
					header->rson = x->lson;
					x->lson->father = header;
					x->lson->color = __rb_tree_black;
					put_node(x);
					return ;
				}

				if (x == header->rson)
					header->rson = get_pre(x);
				else if (x == header->lson)
					header->lson = get_next(x);

				if (__rb_tree_red == x->color)
				{
					if (x == x->father->lson)
						x->father->lson = x->lson;
					else
						x->father->rson = x->lson;
					if (NULL != x->lson) // 非叶子节点
						x->lson->father = x->father;
					put_node(x);
					return ;
				}

				if (x->lson != NULL && __rb_tree_red == x->lson->color)
				{
					x->lson->color = __rb_tree_black;
					x->lson->father = x->father;
					if (x->father->lson == x)
						x->father->lson = x->lson;
					else
						x->father->rson = x->lson;
					put_node(x);
					return ;
				}

				//下面讨论x为黑色,而且其左孩子（有可能为NIL）为黑色
				link_type cur = x->lson;
				link_type father = x->father;
				if (x->father->lson == x)
					x->father->lson = x->lson;
				else 
					x->father->rson = x->lson;
				if (x->lson != NULL)
					x->lson->father = x->father;
				put_node(x);
				_erase_rebalance(cur, father);		
			}

			void _erase_rebalance(link_type cur, link_type father) //cur为双重黑色
			{
				//由于cur为双重黑色，所以其兄弟节点一定不为空
				//否则，兄弟路径上的黑节点个数小于当前节点cur的路径上黑节点的个数
				
				if (cur == get_root())
				{
					//cur->color = __rb_tree_black;
					return ;
				}

				link_type bro = NULL;
				if (cur == father->lson) //
					bro = father->rson;
				else
					bro = father->lson;

				if (__rb_tree_red == bro->color)
				{
					//father's color must be black
					
					//swap father's and bro's color
					father->color = __rb_tree_red;
					bro->color = __rb_tree_black;
					if (cur == father->lson)
						rotate_left(father);
					else
						rotate_right(father);
					_erase_rebalance(cur, father);
				}
				else //bro's color is black and bro must not NIL
				{
					//statu 1 , bro' 2 son's color is black
					if (color(bro->lson) == __rb_tree_black && 
							color(bro->rson) == __rb_tree_black)
					{
						bro->color = __rb_tree_red;
						if (__rb_tree_black == father->color)
							_erase_rebalance(father, father->father);
						else
						{
							father->color = __rb_tree_red;
							return ;
						}
					}
					else if (color(bro->lson) == __rb_tree_red &&
							color(bro->rson) == __rb_tree_black) //statu 2
					{
						bro->color = __rb_tree_red;
						bro->lson->color = __rb_tree_black;
						rotate_right(bro);
						_erase_rebalance(cur, father); //to statu 3
					}
					else if (color(bro->rson) == __rb_tree_red) //statu3
					{
						bro->color = father->color;
					
						if (cur == father->lson)
							rotate_left(father);
						else
							rotate_right(father);
					
						if (bro->lson != NULL)
							bro->lson->color = __rb_tree_black;
						if (bro->rson != NULL)
							bro->rson->color = __rb_tree_black;
					}
				}
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
				iterator i = find(x);
				if (i == end())
					return ;
				
				link_type cur = i.get();
				if (++i == end()) // 无后继
				{
					//ok,直接删掉这个节点
					erase_rebalance(cur);
				}
				else //有后继
				{
					link_type nxt = i.get();
					cur->value = nxt->value;
					//删掉后继节点
					erase_rebalance(nxt);
				}
				//实际被删掉的节点无右子树
				--count_node;
				return ;
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
					printf("addr:%p (%p %p) value:%d color:%s \n", 
							cur, cur->lson, cur->rson,
						   	cur->value, (cur->color?"black":"red"));
					_debug(cur->lson);
					_debug(cur->rson);
				}
			}
	};
}//end of yxSTL
#endif
