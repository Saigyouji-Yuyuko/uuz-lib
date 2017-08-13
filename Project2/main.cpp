#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"ptr.h"
#include"test\test.h"
#include"test\debug.h"
#include"Container\allocator.h"
#include"Container\array.h"
#include"Container\vector.h"
#include<deque>
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"

//#include<deque>
//#include<map>
//#include<set>
//#include<string>
//#include<future>
//#include<iostream>
//#include<vector>
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
#include<deque>
using namespace std;
int foo(vector<int>& a)noexcept
{
	int s = 0,p=a.size();
	for (auto i = 0; i != p; ++i)
		s += a[i];
	a.clear();
	return s;
}
int bar(vector<int>& a)noexcept
{
	int s = 0;
	for (auto i : a)
		s += i;
	a.clear();
	return s;
}
int main(void) 
{
	uuz::vector<int> p;
	p.push_back(1);
	p.push_back(p[0]);
	p.push_back(p[0]);
	p.push_back(p[0]);
	p.push_back(p[0]);
	p.push_back(p[0]);
	for (auto i : p)
		uuz::println(i);
	system("pause");
	return 0;
}