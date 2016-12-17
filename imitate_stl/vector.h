#ifndef VECTOR_H
#define VECTOR_H
#include "memory.h"
#include "stl_uninitialized.h"
#include "algorithm.h"

namespace yxSTL
{
	template<typename T, typename Alloc = allocator<T> >
	class vector
	{
		public :
			typedef T			value_type;
			typedef T*			pointer;
			typedef T*			iterator;
			typedef T&			reference;
			typedef const T*	const_pointer;
			typedef const T&	const_reference;
			typedef size_t		size_type;
			typedef ptrdiff_t	difference_type;
		
		private :
			typedef allocator<T> data_allocator;
	
		private :
			iterator start;
			iterator finish;
			iterator end_of_storage;


			//内存管理的所有操作由以下几个私有函数完成
		private :
			/*
			 *目前没有使用内存池，释放内存由下面的函数完成
			 *使用内存池的话，需要传首地址和元素个数，回收到内存池
			 * 
			 * */
			void deallocate()
			{
				destroy(start, finish);
				
				::operator delete (&*start); 
				//&*start获取迭代器内部指针，当然vector的迭代器就是指针
				//此处没有调用析构函数
				
				start = finish = end_of_storage = NULL;
			}

			iterator allocate_and_fill(size_type n, const_reference value)
			{
				iterator result = data_allocator::allocate(n);
				uninitialized_fill_n(result, n, value);
				return result;
			}

			void fill_initialze(size_type n, const_reference value)
			{
				start	  = allocate_and_fill(n, value);
				finish	  = start + n;
				end_of_storage = finish;
			}

			//元素的构造和析构调用全局函数
			//全局构造和析构函数在标准STL里面有优化，
			//具体是先判断构造和析构是否有意义
		public :
			iterator begin()
			{
				return start;
			}

			const iterator begin() const
			{
				return start;
			}

			iterator end()
			{
				return finish;
			}

			const iterator end() const
			{
				return finish;
			}			

			size_type size() const
			{
				return end() - begin();
			}
			
			size_type capacity() const
			{
				return end_of_storage - begin();
			}
			
			bool empty() const
			{
				return begin() == end();
			}
			
			reference operator[](size_type index)
			{
				return *(begin() + index);
			}

			reference operator[](size_type index) const
			{
				return *(begin() + index);
			}
			
			vector():start(NULL), finish(NULL), end_of_storage(NULL)
			{
			}
			
			vector(size_type n, const_reference value)
			{
				fill_initialze(n ,value);
			}
			
			vector(iterator first, iterator last)
			{
				start = allocate_and_fill(last - first, T());
				copy(first, last, start);
				finish = start + (last - first);
				end_of_storage = start + (last - first);
			}

			explicit vector(size_type n)
			{
				fill_initialze(n , value_type());
			}
				
			vector(const vector<T>& tmp) //先申请内存，再调用placement new更好
			{
				start = allocate_and_fill(tmp.size(), T());
				copy(tmp.begin(), tmp.end(), start);
				finish = start + tmp.size();
				end_of_storage = start + tmp.size();
			}	
		
			vector<T>& operator =(const vector<T>& tmp)
			{
				if (capacity() < tmp.size())
				{
					deallocate();
					start = allocate_and_fill(tmp.size(), T());
					copy(tmp.begin(), tmp.end(), start);
					finish = start + tmp.size();
					end_of_storage = start + tmp.size();
				}
				else
				{
					destroy(begin(), end());
					copy(tmp.begin(), tmp.end(), begin());
					finish = begin() + tmp.size();
				}
			}	

			~vector()
			{	
				deallocate();	
			}
			
			void push_back(const T& value)
			{
				if (finish != end_of_storage)
				{
					construct(end(), value);
					++finish;
				}
				else
					insert(end(), value);
			}
			
			void pop_back()
			{
				--finish;
				destroy(finish);
			}
			
			reference front()
			{
				return *start;
			}
			
			reference back()
			{
				return *(finish - 1);
			}
			
			iterator erase(iterator position)
			{
				if (position + 1 != end())
					copy(position + 1, finish, position);
				--finish;
				destroy(finish);
				return position;
			}
			
			iterator erase(iterator first, iterator last)
			{
				iterator newfinish = copy(last, finish, first);
				destroy(newfinish, finish);
				finish = finish - (last - first);
				return first;
			}
			
			void insert(iterator position, const T& value)
			{
				insert(position, (size_type)1, value);
			}

			void insert(iterator position, size_type n, const T& value)
			{
				if (0 == n) return ;
				if (size_type(end_of_storage - finish) < n ) //剩余空间不足
				{
					const size_type old_size = size();
					const size_type len = old_size + max(old_size, n);
					iterator new_start = data_allocator::allocate(len);
					iterator new_finish = new_start;

					new_finish = uninitialized_copy(start, position, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, value);
					new_finish = uninitialized_copy(position, finish, new_finish);

					deallocate();
					start = new_start;
					finish = new_finish;
					end_of_storage = start + len;
					return ; 
				}

				iterator old_finish = finish;
				const size_type elems_after = finish - position;
				if  (elems_after > n)
				{
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, value);
				}
				else
				{
					uninitialized_fill_n(finish, n - elems_after, value);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, value);
				}
			}

			void resize(size_type new_size, const T& value)
			{
				if (new_size < size())
					erase(begin() + new_size, end());
				else
					insert(end(), new_size - size(), value);
			}
			
			void resize(size_type new_size)
			{
				resize(new_size, value_type());
			}
			
			void clear()
			{
				erase(begin(), end());
			}

			void swap(vector<T>& tmp)
			{
				yxSTL::swap(this->start, tmp.start);
				yxSTL::swap(this->finish, tmp.finish);
				yxSTL::swap(this->end_of_storage, tmp.end_of_storage);
			}

			bool operator==(vector<T>& tmp)
			{
				if (tmp.size() != size())
					return false;
				for (size_type i = 0; i < size(); i++)
					if (tmp[i] != start[i])
						return false;
				return true;
			}
	};
}

#endif
