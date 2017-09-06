#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
//#include"ptr.h"
#include"test\test.h"
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
using namespace std;
int main()
{
	string s,t;
	while (cin >> t)
		s += t;
	int begin = -1;
	int maxx = 0;
	int k = 0;
	for (int i = 0; i != s.size(); ++i)
	{
		if (s[i] >= '0' && s[i] <= '9')
			++k;
		else
		{
			if (k >= maxx)
			{
				maxx = k;
				begin = i - k;
			}
			k = 0;
		}
	}
	if (k >= maxx)
	{
		maxx = k;
		begin = s.size() - k;
	}

	if (maxx)
		cout << s.substr(begin, maxx) << endl << maxx;
	return 0;

}