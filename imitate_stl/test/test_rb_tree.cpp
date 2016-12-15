// @Created time :2016年12月11日 星期日 17时10分57秒
//#include <bits/stdc++.h>
#include "../rb_tree.h"
#include "../algorithm.h"
#include <iostream>

using namespace yxSTL;
//using namespace std;

void test1()
{
	printf("press any key to start test1...\n");
	getchar();
	rb_tree<int, std::less<int> > tree;
	if (tree.empty())
		printf("the tree is empty\n");
	tree.insert(3);
	tree.insert(6);
	tree.insert(4);
	tree.insert(1);
	tree.insert(8);
	tree.insert(7);
	tree.insert(5);
	tree.debug();
	typedef rb_tree<int, std::less<int> >::iterator it_type;
	for (it_type it = tree.begin(); it != tree.end(); ++it)
	{
		printf("%d \n", *it);	
	}
	printf("size:%d\n", tree.size());
	printf("depth:%d\n", tree.depth());
}

void test2()
{
	printf("press any key to start test2...\n");
	getchar();
	rb_tree<int, std::less<int> > tree;
	for (int i = 0; i < 1000000; i++)
	{
		tree.insert(rand());
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

void test3()
{
	rb_tree <int, std::less<int> > tree;
	for (int i = 0; i < 10; i++)
		tree.insert((rand() % 10 ^ 37) % 13);
	tree.debug();
	typedef rb_tree<int, std::less<int> >::iterator itor;
	for (itor it = tree.begin(); it != tree.end(); ++it)
	{
		printf("%d ", *it);
	}
	printf("\n");
	while (1)
	{
		printf("[ ");
		for (itor it = tree.begin(); it != tree.end(); ++it)
		{
			printf("%d ", *it);
		}
		printf("]\n");
		printf("the tree is vaild : %s\n", (tree.check_isVaild()?"yes":"no"));
		printf("witch one are you want to erase??\n");
		int x;
		scanf("%d", &x);
		tree.erase(x);
		tree.debug();
	}
}

int main()
{
//	test1();
//	test2();
	test3();
	return 0;
}
