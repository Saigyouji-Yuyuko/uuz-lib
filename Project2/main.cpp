#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"ptr.h"
#include"test\test.h"
#include"test\debug.h"
#include"Container/set.h"
#include"Container\allocator.h"
#include"Container\array.h"
#include"Container\vector.h"
#include"Container\list.h"
#include"Container\forward_list.h"
#include"Container\set.h"
#include"test\Container\vector_test.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"

#include<map>
#include<deque>
//#include<map>
//#include<set>
//#include<string>
//#include<future>
//#include<iostream>
#include<vector>
//#include<memory>
//#include<ctime>
//#include<queue>
//#include<list>
//#include<functional>
//#include<forward_list>
//#include<type_traits>
//#include<thread>
//#include<mutex>
//#include<filesystem>
//#include<array>
//#include<algorithm>
//#include<numeric>
//#include<iomanip>
//#include<fstream>
#pragma warning(disable:4996)
using namespace std;
void display_sizes(const uuz::multiset<int> &nums1,
	const uuz::multiset<int> &nums2,
	const uuz::multiset<int> &nums3)
{
	std::cout << "nums1: " << nums1.size()
		<< " nums2: " << nums2.size()
		<< " nums3: " << nums3.size() << '\n';
}

int main()
{
	for(int i =1;i<=10000000;i*=10)
	{
		uuz::println(i);
		{
			std::deque<int>b;
			b.resize(i);
			{
				uuz::block_time a{ "deque" };
				for (int j = 0; j != i; ++j)
					b.push_back(rand());
			}
			uuz::println("");
		}
		{
			std::vector<int>b;
			b.reserve(i);
			{
				uuz::block_time a{ "vector" };
				for (int j = 0; j != i; ++j)
					b.push_back(rand());
			}
			uuz::println("");
		}
		{
			uuz::vector<int>b;
			b.reserve(i);
			{
				uuz::block_time a{ "uuz::vector" };
				for (int j = 0; j != i; ++j)
					b.push_back(rand());
			}
			uuz::println("");
		}
		uuz::println("---------------------------");
	}
	system("pause");
	return 0;
}