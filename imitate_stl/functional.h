#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

namespace yxSTL
{
	//函数对象
	template <typename T>
	struct less
	{
		bool operator()(const T& a, const T& b) const
		{
			return a < b;
		}
	};

}//end of yxSTL


#endif
