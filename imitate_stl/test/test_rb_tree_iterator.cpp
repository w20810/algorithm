// @Created time :2016年12月09日 星期五 15时09分41秒
//#include <bits/stdc++.h>
//using namespace std;

#include "../rb_tree.h"
#include <iostream>
using namespace yxSTL;

int main()
{
	__rb_tree_iterator<int> it;
	__rb_tree_iterator<int> it1(it);
	*it = 3;
	std::cout<<*it1<<std::endl;		
    return 0;
}
