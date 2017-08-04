#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"time.h"
#include"ptr.h"
#include"string.h"
#include"debug.h"
#include"pair.h"
#include"list.h"
#include"vector.h"
#include"forward_list.h"
#include"file.h"
#include"array.h"
#include"priority_queue.h"
#include"rbtree.h"

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
#include<type_traits>
#include<thread>
#include<mutex>
#include<filesystem>
#include<array>
#include<algorithm>
#include<numeric>
#include<iomanip>
#include<fstream>
#pragma warning(disable:4996)
int main()
{
	for (int i = 1; i <= 10000000; i *= 10)
	{
		uuz::print(i);
		{
			uuz::time a{ "std" };
			std::set<int> p;
			for (int j = 1; j <= i; ++j)
				p.insert(j);
			/*for (int j = 1; j <= i; ++j)
				p.erase(j);*/
			
			//uuz::print(p.size());
		}
		{
			uuz::time a{ "uuz" };
			uuz::rb_tree<int> p;
			for (int j = i; j >= 1; --j)
				p.emplace(j);
			/*for (int j = 1; j <= i; ++j)
				p.dele(p.find(j));*/
			//uuz::print(*p.begin());
			//uuz::print(p.size());
		}
		uuz::print("--------------");
	}
	system("pause");
	return 0;
}