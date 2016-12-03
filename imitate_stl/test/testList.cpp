// @Created time :2016年12月03日 星期六 16时37分23秒
#include <bits/stdc++.h>
#include "../list.h"
#include "../algorithm.h"
//using namespace std;

using namespace yxSTL;

void printList(list<int>& l)
{
	for (list<int>::iterator cur = l.begin(); cur != l.end(); ++cur)
	{
		printf("%d ", *cur);
	}
	printf("\n");
}

int main()
{
	list<int > l(10,1);	
	list<int>::iterator ite = l.begin();
	printList(l);

	printf("\n size:%d \n", l.size());

	l.front() = 3;
	l.back()  = 9;
	printList(l);
    
	if (l.empty())
	{
		printf("is empty\n");
	}

	l.push_back(666);
	l.push_back(777);
	l.push_back(888);

	printList(l);	

	l.pop_back();

	printList(l);
	
	l.remove(1);
	l.remove(666);
	printList(l);


	list<int>::iterator ite1 = find(l.begin(), l.end(), 777);
	if (ite1 != l.end())
	{
		printf("found it:%d\n", *ite1);
	}
	
	list<int>::iterator ite2 = l.insert(l.begin(), 123);
	ite2 = l.insert(ite2, 456);
	ite2 = l.insert(ite2, 789);
	l.insert(ite2, 2, 222);
	printList(l);	


	return 0;
}
