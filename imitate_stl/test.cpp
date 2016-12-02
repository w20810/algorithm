// @Created time :2016年12月02日 星期五 13时12分25秒
#include <iostream>
#include "vector.h"
using namespace yxSTL;
using namespace std;

struct node
{
	int a;
	node(){ a = 2; cout<<"node construct"<<endl;}
	node(int i):a(i){cout<<"i node construct"<<endl;}
	~node(){cout<< a<<" node destruct"<<this<<endl;}
};

int main()
{
	vector<node > v1(5, node(5));
	vector<node > v2(7, node(7));
	
	printf("v1 : ");
	for (int i = 0; i < v1.size(); i++)
		printf("%d ", v1[i].a);
	printf("\n");

	printf("v2 : ");
	for (int i = 0; i < v2.size(); i++)
		printf("%d ", v2[i].a);
	printf("\n");

	v1.swap(v2);

	printf("v1 : ");
	for (int i = 0; i < v1.size(); i++)
		printf("%d ", v1[i].a);
	printf("\n");

	printf("v2 : ");
	for (int i = 0; i < v2.size(); i++)
		printf("%d ", v2[i].a);
	printf("\n");

	return 0;
}
