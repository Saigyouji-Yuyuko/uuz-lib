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
	uuz::rb_tree<int> k;
	for (int i = 1; i <= 100; ++i)
		k.emplace(i);
	//k.print();
	int p=0;
	for (auto i : k)
	{
		uuz::print(i);
		++p;
	}
	uuz::print(p);
	//uuz::print(k.size());
	for (int i = 1; i <= 100; ++i)
	{
		k.dele(k.find(i));
		//uuz::print(k.nul.father->get());
	}
	system("pause");
	return 0;
}