// @Created time :2016年12月06日 星期二 16时34分01秒
//#include <bits/stdc++.h>
//using namespace std;

#include <iostream>
#include <cstdio>
#include "../priority_queue.h"
#include "../vector.h"
using namespace yxSTL;

int main()
{
	vector<int> v;
	v.push_back(4);
	v.push_back(45);
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(-8);
	priority_queue <int> pq(v.begin(), v.end());
	pq.debug();
	getchar();
	printf("pq.size :%d\n", pq.size());
	while (!pq.empty())
	{
		printf("top :%d\n", pq.top());
		pq.pop();
		pq.debug();
		getchar();
	}	

	priority_queue<int > q;
	for (int i = 0; i < 10; i++)
	{
		q.push(rand() % 17);
		q.debug();
		getchar();
	}
	for (int i = 0; i < 9; i++)
	{
		printf("top :%d\n", q.top());
		q.pop();
		q.debug();
		getchar();
	}
	return 0;
}
