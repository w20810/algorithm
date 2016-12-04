// @Created time :2016年12月04日 星期日 14时27分15秒
#include <bits/stdc++.h>
#include "../stack.h"
#include "../list.h"
using namespace yxSTL;
//using namespace std;

int main()
{
/*
	stack<int > st;
	st.push(3);
	st.push(4);
	st.push(5);
	printf("size:%d\n", st.size());
	st.pop();
	printf("top:%d\n", st.top());
	printf("size:%d\n",st.size());
	st.pop();
	printf("top:%d\n", st.top());
	printf("size:%d\n", st.size());
*/
	stack<int, list<int> > stlist;
	stlist.push(111);
	stlist.push(222);
	stlist.push(333);
	stlist.push(444);
	printf("is empty:%s\n", stlist.empty()? "yes": "no");
	printf("size:%d\n", stlist.size());
	stlist.pop();
	printf("size:%d\n", stlist.size());
	printf("top:%d\n", stlist.top());
	stlist.pop();
	printf("size:%d\n", stlist.size());
	printf("top:%d\n", stlist.top());
    stlist.pop();
	printf("size:%d\n", stlist.size());
	printf("top:%d\n", stlist.top());
	stlist.pop();
	printf("is empty: %s\n", stlist.empty()?"yes":"no");

	list<int > l1(10, 3);
	list<int > l2(l1);
	for (list<int>::iterator it = l2.begin(); it != l2.end(); ++it)
	{
		printf("%d ", *it);	
	}	
	printf("\n");

	l2.push_back(6);
	l2.push_front(1);
	l2.push_back(6);
	for (list<int >::iterator it = l2.begin(); it != l2.end(); ++it)
	{
		printf("%d ", *it);
	}
	printf("\n");
	
	l1.pop_back();
	l1.pop_front();
	l1.pop_back();
	l1.pop_front();

	printf("l1 size:%d\n", l1.size());	
	l2.splice(l2.begin(), l1);

	l2.unique();
	for (list<int >::iterator it = l2.begin(); it != l2.end(); ++it)
	{
		printf("%d ", *it);
	}
	printf("\n");
	return 0;
}
