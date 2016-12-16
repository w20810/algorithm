#ifndef SET_H
#define SET_H

#include "rb_tree.h"
#include "algorithm.h"

namespace yxSTL
{
	template <typename Key, typename Compare = less<Key> >
	class set
	{
		private :
			typedef Key					value_type;
			typedef Key*				pointer;
			typedef Key&				reference;
			typedef const Key&			const_reference;
			typedef const Key*			const_pointer;
			typedef size_t				size_type;

			typedef rb_tree<Key, Compare>	 rb_tree_t;//底层数据结构
		public :
			typedef typename rb_tree_t::iterator  iterator;
		
		private :
			rb_tree_t c;
		
		public :
			set(){}
			~set(){}
			
			template<typename Iterator>
			set(Iterator first, Iterator last)
			:c()
			{
				while (first != last)
					c.insert_unique(*first++);
			}	
			
			bool empty()
			{
				return c.empty();
			}
			
			size_type size()
			{
				return c.size();
			}

			iterator begin()
			{
				return c.begin();
			}
		
			iterator end()
			{
				return c.end();
			}

			iterator find(const_reference x)
			{
				return c.find(x);
			}

			void insert(const_reference x)
			{
				c.insert_unique(x);
			}

			void erase(iterator x)
			{
				c.erase(x);
			}

			void debug()
			{
				c.debug();
			}

			void clear()
			{
				c.clear();
			}

			iterator lower_bound(const_reference x)
			{
				return c.lower_bound(x);
			}

			iterator upper_bound(const_reference x)
			{
				return c.upper_bound(x);
			}

			size_type count(const_reference x)
			{
				return c.count(x);
			}
	};

	template <typename Key, typename Compare>
	class multiset
	{
		private:

		public :

	};

}//end of yxSTL

#endif 
