// @Created time :2016年12月11日 星期日 17时10分57秒
//#include <bits/stdc++.h>
#include "../rb_tree.h"
#include "../algorithm.h"
#include <iostream>

using namespace yxSTL;
//using namespace std;

int main()
{
	rb_tree<int, std::less<int> > tree;
	std::cout<<tree.empty()<<std::endl;
	return 0;
}
