#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
//#include"ptr.h"
//#include"test\test.h"
//#include"test\debug.h"
//#include"Container/set.h"
//#include"Container\allocator.h"
//#include"Container\array.h"
//#include"Container\vector.h"
//#include"Container\list.h"
//#include"Container\forward_list.h"
//#include"Container\set.h"
//#include"test\Container\vector_test.h"
//#include"Container\deque.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"
import std;
#include<future>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<fstream>
#include<deque>
#include<map>
#include<set>
#include<string>
#include<future>
#include<iostream>
#include<vector>
#include<memory>
#include<ctime>
#include<queue>
#include<list>
#include<functional>
#include<forward_list>
#include<array>
#include<numeric>
#include<iomanip>
#include<set>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<sstream>
#undef max
#undef min
//#pragma warning(disable:4996)
std::array<int, 5000>num;
std::multimap<int, std::pair<int,int>>cha;
using namespace std;
bool istixing(int a,int b,int c,int d)
{
	std::is_permutation()
	if (a - b > c - d && a - b < c + d && c ==d)
		return true;
	else if (a - c > b - d && a - c < b + d&& b ==d)
		return true;
	else if (a - d > b - c && a - d < b + c&& b==c)
		return true;
	else if (b - c > a - d && b - c < a + d&& a ==d)
		return true;
	else if (b - d > a - c && b - d < a + c&&a ==c)
		return true;
	else if (c - d > a - b && c - d < a + b&& a ==b)
		return true;
	return false;
}
int main()
{
	int n;
	std::invoke(istixing,1,2,3,4);
//	string d{ 300 };
	freopen("C:\\Users\\99650\\Desktop\\123.in", "r+", stdin);
	freopen("C:\\Users\\99650\\Desktop\\1111.txt", "w+", stdout);
	scanf("%d", &n);
	for(auto i=1;i<=n;++i)
	{
		int p;
		scanf("%d", &p);
		for (auto j = 0; j != p; ++j)
			scanf("%d", &num[j]);
		std::sort(num.begin(), num.begin()+p);
		int dd = 0;
		if(p<=3)
		{
			printf("Case #%d: 0\n", i);
			continue;
		}
		for (auto j = 0; j != p-3; ++j)
			for (auto k = j+1; k != p-2; ++k)
				for (auto l = k + 1; l != p - 1; ++l)
					for (auto m = l + 1; m != p; ++m)
					{
						if (istixing(num[m],num[l],num[k],num[j]))
							++dd;
							
					}
		printf("Case #%d: %d\n", i, dd);
		cha.clear();
	}
	//system("pause");
	return 0;
}