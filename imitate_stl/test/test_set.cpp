// @Created time :2016年12月16日 星期五 14时06分33秒
#include <iostream>
#include <cstdio>
#include "../set.h"
#include "../vector.h"

using namespace yxSTL;

//it's no memorey leaked, checked by valgrind,

void test1()
{
	set<int> s;
	s.insert(3);
	printf("found 3? :%s\n", s.find(3)!=s.end()?"yes":"no");
	printf("found 2? :%s\n", s.find(2)!=s.end()?"yes":"no");
	s.insert(6);
	s.insert(3);
	s.insert(3);
	s.insert(5);
	s.insert(10);
	s.insert(-2);
	s.insert(-9);
	s.insert(155);
	s.insert(62);
	printf("size ? :%d\n", s.size());
	typedef set<int, std::less<int>>::iterator itor;
	for (itor it = s.begin(); it != s.end(); ++it)
	{
		printf("%d ", *it);
	}
	printf("\n");
}

void test2()
{
	vector<int > v(6,6);
	v.push_back(3);
	v.push_back(4);
	v.push_back(8);
	v.push_back(5);
	v.push_back(7);
	set<int > s(v.begin(), v.end());
	typedef set<int>::iterator itor;
	for (itor it = s.begin(); it != s.end(); ++it)
		printf("%d ", *it);
	printf("\n");
	
	itor it = s.find(5);
	if (it != s.end())
		s.erase(it);
	
	s.debug();

	it = s.find(7);
	std::cout<<*it<<std::endl;
	if (it != s.end())
		s.erase(it);

	for (itor it = s.begin(); it != s.end(); ++it)
		printf("%d ", *it);
	printf("\n");
	
	//s.clear();
	printf("s is empty? :%s\n", s.empty()? "yes": "no");
	for (it = s.begin(); it != s.end(); ++it)
		printf("%d ", *it);
	printf("\n");

	for (it = s.lower_bound(4); it != s.end(); ++it)
		printf("%d ", *it);
	printf("\n");

	for (it = s.lower_bound(5); it != s.end(); ++it)
		printf("%d ", *it);
	printf("\n");

	for (it = s.upper_bound(4); it != s.end(); ++it)
		printf("%d ", *it);
	printf("\n");

	s.clear();
	printf("s is empty? :%s\n", s.empty()? "yes": "no");

}

void test3()
{
	class node
	{
		public :
			node(int a = 0, int b = 0)
				:value1(a), value2(b)
			{
			}

			bool operator < (const node& n) const
			{
				if (value1 != n.value1)
					return value1 < n.value1;
				return value2 < n.value2;
			}
			int getv2() const
			{
				return value2;
			}
		private :
			int value1;
			int value2;
	};
	set<node > s;
	s.insert(node(2,4));
	
	struct cmp
	{
		bool operator() (const node &a, const node &b) const
		{
			return a.getv2() < b.getv2();
		}	
	};
	set<node, cmp> s2;
	s2.insert(node(5,6));
	s2.insert(node(7,8));
}

int main()
{
	test1();
	test2();
	test3();
	return 0;
}
