#ifndef LIST_H
#define LIST_H
#include "algorithm.h"
#include "memory.h"

namespace yxSTL
{
	template <typename T>
	struct __list_node
	{
		T data;
		__list_node	*next;
		__list_node *prev;	
		__list_node(): next(NULL), prev(NULL){}
		__list_node(T value) : data(value), next(NULL), prev(NULL){}
		__list_node(const __list_node& tmp) : data(tmp.data), next(tmp.next), prev(tmp.prev){}
	};

	//迭代器其实就是把指针封装了一下
	//迭代器不能分配内存，内存有上层结构申请
	//同样不能释放内存
	template <typename T>
	class __list_iterator
	{
		public :
			typedef T			value_type;
			typedef T*			pointer;
			typedef T&			reference;
			typedef const T*	const_pointer;
			typedef const T&	const_reference;
			typedef size_t		size_type;
			typedef ptrdiff_t	different_type;
	
			typedef __list_node<T>*		link_type;
		
			typedef __list_iterator		self;
	
		private :
			link_type pnode;

		public :
			__list_iterator()
			{
				pnode = NULL;
			}
	
			__list_iterator(link_type ptr) : pnode(ptr)
			{
			}
	
			__list_iterator(const self& itor) : pnode(itor.pnode)
			{	
			}

			__list_iterator& operator= (const self& itor)
			{
				pnode = itor.pnode;	
			}

			link_type get()
			{
				return pnode;
			}

			bool operator == (const self& itor) const
			{
				return pnode == itor.pnode;
			}
		
			bool operator != (const self& itor) const
			{
				return pnode != itor.pnode;
			}

			reference operator* () const
			{
				return (*pnode).data;
			}

//			pointer operator-> ()const
//			{
//				return &(operator*());
//			}

			self& operator++()
			{
				pnode = pnode->next;
				return *this;
			}		

			const self operator++(int)
			{
				self tmp = *this;
				++(*this);
				return tmp;
			}

			self& operator--()
			{
				pnode = pnode->prev;
				return *this;
			}
		
			const self operator--(int)
			{
				self tmp = *this;
				--(*this);
				return tmp;
			}
		};

	template <typename T, typename Alloc = allocator<__list_node<T>> >
	class list
	{
		public :
			typedef T			value_type;
			typedef T*			pointer;
			typedef T&			reference;
			typedef const T*	const_pointer;
			typedef const T&	const_reference;
			typedef size_t		size_type;
			typedef ptrdiff_t	different_type;
			
			typedef __list_node<T>*		link_type;
			typedef __list_node<T>		node_type;

			typedef __list_iterator<T>	iterator;

			typedef list				self;
			
		private :
			typedef Alloc				node_allocator;


			//list真正存的是节点的指针，而迭代器只是辅助工具
			//但是各个接口返回的是迭代器
		private :
			link_type					pnode;

		private :
			link_type get_node()
			{
				return link_type(node_allocator::allocate());
			}

			void put_node(link_type ptr)
			{
				destroy(ptr);
				node_allocator::deallocate(ptr);
			}

			link_type get_initialized_node(const_reference value)
			{
				link_type result = node_allocator::allocate();
				construct(result, value);
				return result;
			}

			link_type get_initialized_node(const node_type& value)
			{
				link_type result = node_allocator::allocate();	
				construct(result, value);
				return result;
			}
		
		public :
			list()
			{
				pnode = get_node();
				pnode->next = pnode;
				pnode->prev = pnode;
			}
			
			list(size_type n, const_reference value)
			{
				pnode = get_node();
				pnode->next = pnode;
				pnode->prev = pnode;
				while (n--)
				{
					link_type tmp		= get_initialized_node(value);
					tmp->next			= pnode;
					tmp->prev			= pnode->prev;
					pnode->prev->next	= tmp;
					pnode->prev			= tmp;	
				}
			}

			list(const self& tmp)
			{
				
			}

			~list()
			{
				
			}

			iterator begin()
			{
				return pnode->next;
			}

			iterator end()
			{
				return pnode;
			}

			size_type size()
			{
				size_type result = 0;
				for (iterator cur = begin(); cur != end(); ++cur, ++result);
				return result;
			}

			bool empty()
			{
				return begin() == end();
			}

			reference front()
			{
				return *begin();
			}

			reference back()
			{
				return *(--end());
			}

			void push_back(const_reference value)
			{
				link_type tmp		= get_initialized_node(value);
				tmp->next			= pnode;
				tmp->prev			= pnode->prev;
				pnode->prev->next	= tmp;
				pnode->prev			= tmp;	
			}

			void pop_back()
			{
				link_type tmp = pnode->prev;
				pnode->prev->prev->next = pnode;
				pnode->prev = pnode->prev->prev;
				put_node(tmp);
			}

			iterator remove(const_reference value)
			{
				link_type tmp = pnode->next; //begin
				while (tmp != pnode)
				{
					link_type nxt = tmp->next;
					if (tmp->data == value)
					{
						link_type delnode = tmp;
						tmp->prev->next = tmp->next;
						tmp->next->prev = tmp->prev;
						put_node(tmp);
					}
					tmp = nxt;
				}
				return pnode->next;
			}

			iterator insert(iterator position, const_reference value)
			{
				link_type pos = position.get();
				link_type tmp = get_initialized_node(value);
				tmp->prev = pos->prev;
				tmp->next = pos;
				pos->prev->next = tmp;
				pos->prev = tmp;
				return tmp;
			}

			iterator insert(iterator position, size_type n, const_reference value)
			{
				iterator tmp = position;
				while (n--)
					tmp = insert(position, value);
				return position;
			}

			iterator erase(iterator position)
			{
				link_type pos = position.get();
				link_type result = pos->prev;
				pos->prev->next = pos->next;
				pos->next->prev = pos->prev;
				put_node(pos);
				return result;	
			}
			
			void clear()
			{
				
			}

			void transfer(iterator position, iterator first, iterator last)
			{

			}

			void splice(iterator position, list& x)
			{

			}

			void splice(iterator position, list&, iterator i)
			{
				
			}

			void reverse()
			{
				
			}

			void sort()
			{
				
			}

			void merge()
			{

			}

			void unique()
			{

			}

			

	};//end of list			

} //end of namespace yxSTL



#endif
