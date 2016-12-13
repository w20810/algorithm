// @Created time :2016年12月11日 星期日 17时10分57秒
//#include <bits/stdc++.h>
#include "../rb_tree.h"
#include "../algorithm.h"
#include <iostream>

using namespace yxSTL;
//using namespace std;

void test1()
{
	rb_tree<int, std::less<int> > tree;
	//std::cout<<tree.empty()<<std::endl;
	tree.insert(3);
	//tree.debug();
	tree.insert(6);
//	tree.debug();
	tree.insert(4);
//	tree.debug();
	tree.insert(1);
//	tree.debug();
	tree.insert(8);
//	tree.debug();
	tree.insert(7);
	//tree.debug();
	tree.insert(5);
	tree.debug();
	//std::cout<<std::endl;
//	typedef rb_tree<int, std::less<int> >::iterator it_type;
//	for (it_type it = tree.begin(); it != tree.end(); ++it)
//	{
//		printf("%d \n", *it);
		
//	}
}

void test2()
{
	rb_tree<int, std::less<int> > tree;
	for (int i = 0; i < 1000000; i++)
	{
		tree.insert(rand()%10000);
	}
	typedef rb_tree<int, std::less<int> >::iterator itor;
	int count = 0;
	for (itor it = tree.begin(); it != tree.end(); ++it)
	{
		count++;
		printf("%d ", *it);
	}
	printf("\n");
	printf("total :%d\n", count);
	printf("the depth is :%d\n", tree.depth());
	printf("maxValue:%d minValue:%d\n", tree.max(), tree.min());
}


int main()
{
	test1();
	test2();
	return 0;
}
