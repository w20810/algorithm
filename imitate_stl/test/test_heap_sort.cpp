// @Created time :2016年12月06日 星期二 14时54分05秒
//#include <bits/stdc++.h>
#include <iostream>
#include "../algorithm.h"
#include "../vector.h"
using namespace yxSTL;
using namespace std;

void printVector(vector<int>& v)
{
	for (int i = 0; i < v.size(); i++)
		printf("%d ", v[i]);
	printf("\n");
}

struct cmpLess
{
	bool operator ()(const int& a, const int& b)const
	{
		return a < b;
	}
};

int main()
{
	vector<int > v;
	for (int i = 0; i < 8; i++)
		v.push_back(rand()%13);
	printVector(v);
	make_heap(v.begin(), v.end(), greater<int>());
	printVector(v);
	sort_heap(v.begin(), v.end(), greater<int>());
	printVector(v);
	v.push_back(666);
	push_heap(v.begin(), v.end(), greater<int>()); //此时是一个堆，但可能不是有序的
//	make_heap(v.begin(), v.end(), greater<int>());
	sort_heap(v.begin(), v.end(), greater<int>());
	printVector(v);
	return 0;
}
