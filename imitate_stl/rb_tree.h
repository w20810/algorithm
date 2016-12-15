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
					if (pnode->rson != fa)
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
				//FIXME : 前提保证了x最多只有一个孩子
				if (x == get_root())
				{
					if (NULL == x->lson && NULL == x->rson) //size == 1
					{
						put_node(x);
						header->father = header;
						header->lson = header;
						header->rson = header;
						return ;
					}
					else if (NULL == x->lson)
					{
						header->father = x->rson;
						header->lson = x->rson;
						x->rson->father = header;
						x->rson->color = __rb_tree_black;
						put_node(x);
						return ;
					}
					else if (NULL == x->rson)
					{
						header->father = x->lson;
						header->rson = x->lson;
						x->lson->father = header;
						x->lson->color = __rb_tree_black;
						put_node(x);
						return ;
					}
					printf("mustn't come here\n");//不可能右这种情况
				}

				if (x == header->rson)
					header->rson = get_pre(x);
				else if (x == header->lson)
					header->lson = get_next(x);

				if (__rb_tree_red == x->color) //x为红色，删掉之后不会影响红黑树的性质
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

				//此时x的颜色为黑色，且最多只有一个孩子,孩子的颜色为红色
				if (__rb_tree_red == color(x->lson) || __rb_tree_red == color(x->rson))
				{
					//若孩子节点的颜色为红色，删除x,然后直接把孩子节点变为黑色
					//这样，红黑树的性质依然保持不变
					link_type pson = (x->lson ? x->lson : x->rson);
					pson->color = __rb_tree_black;
					pson->father = x->father;
					if (x->father->lson == x)
						x->father->lson = pson;
					else
						x->father->rson = pson;
					put_node(x);
					return ;
				}

				//默认NIL节点为黑色
				//下面讨论x为黑色,而且其孩子（有可能为NIL）为黑色
				
				link_type cur = NULL;
				if (x->lson != NULL)
					cur = x->lson;
				else if (x->rson != NULL)
					cur = x->rson;
			
				link_type father = x->father;
				if (x->father->lson == x)
					x->father->lson = cur;
				else 
					x->father->rson = cur;
				if (cur != NULL)
					cur->father = x->father;
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

				if (__rb_tree_red == bro->color) //FIXME:bro's position is no use
				{
					//FIXME
					//father's color must be black
					//bro' 2 son's color must be black
					
					//swap father's and bro's color
					father->color = __rb_tree_red;
					bro->color = __rb_tree_black;
					if (cur == father->lson)
						rotate_left(father);
					else
						rotate_right(father);
					_erase_rebalance(cur, father);
					//cur's bro's color is black now.
				}
				else //bro's color is black and bro must not NIL
				{
					//FIXME
					//statu 1 , bro' 2 son's color is black(bro's position is no use)
					if (color(bro->lson) == __rb_tree_black && 
							color(bro->rson) == __rb_tree_black)
					{
						//printf("bro is black, and his 2 son also is black @@@@@\n");
						//printf("cur is : %p  @@@@@\n", cur);
						getchar();
						bro->color = __rb_tree_red;
						if (__rb_tree_black == father->color)
							_erase_rebalance(father, father->father);
						else
						{
							father->color = __rb_tree_black;
							return ;
						}
						return ;
					}
				//	if (cur == father->lson)
				//	{

				//	}
				//	else
				//	{

				//	}

					else if (color(bro->lson) == __rb_tree_red &&
							color(bro->rson) == __rb_tree_black) //statu 2
					{
						if (cur == father->lson)
						{
							bro->color = __rb_tree_red;
							bro->lson->color = __rb_tree_black;
							rotate_right(bro);
							_erase_rebalance(cur, father); //to statu 3
						}
						else
						{
							bro->color = father->color;
							rotate_right(father);
							if (bro->lson != NULL)
								bro->lson->color = __rb_tree_black;
							if (bro->rson != NULL)
								bro->rson->color = __rb_tree_black;
						}
					}
					else if (color(bro->rson) == __rb_tree_red) //statu3
					{
						if (cur == father->lson)
						{
							bro->color = father->color;
							rotate_left(father);
							if (bro->lson != NULL)
								bro->lson->color = __rb_tree_black;
							if (bro->rson != NULL)
								bro->rson->color = __rb_tree_black;
						}
						else
						{
							bro->color = __rb_tree_red;
							bro->rson->color = __rb_tree_black;
							rotate_left(bro);
							_erase_rebalance(cur, father);
						}
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
					if (!cmp(x, cur->value) && !cmp(cur->value, x))//x == cur->value
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
				
				//printf("find addr:%p\n", i.get());
				getchar();
				link_type cur = i.get();
				if (NULL == cur->rson) //无右孩子
				{
					//ok,直接删掉这个节点
				//	printf("dirctly\n");
				//	getchar();
					erase_rebalance(cur);
				}
				else //有右孩子
				{
					link_type nxt = get_next(cur);
					//printf("indirctly next:%p\n", nxt);
					getchar();
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
				printf("header:(%p) \n", header);
				_debug(get_root());
				printf("]\n");
			}

			void _debug(link_type cur)
			{
				if (cur)
				{
					printf("addr:%p (%p %p) father:%p value:%d color:%s \n", 
							cur, cur->lson, cur->rson, cur->father,
						   	cur->value, (cur->color?"black":"red"));
			//		getchar();
					_debug(cur->lson);
					_debug(cur->rson);
				}
			}

			bool check_isVaild()
			{
				bool ok = true;
				check_isVaild(get_root(), ok);
				return ok;
			}

			int check_isVaild(link_type cur, bool &ok)
			{
				if (NULL == cur)
					return 1;

				int l = check_isVaild(cur->lson, ok);
				int r = check_isVaild(cur->rson, ok);

				if (l != r)
					ok = false;
				if (__rb_tree_black == cur->color)
					return l + 1;
				return l;
			}
	};
}//end of yxSTL
#endif
