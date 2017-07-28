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
#pragma warning(disable:4996)

int main()
{
	uuz::forward_list<int> x = { 1, 2, 2, 3, 3, 2, 1, 1, 2 };

	std::cout << "contents before:";
	for (auto val : x)
		std::cout << ' ' << val;
	std::cout << '\n';

	x.unique();
	std::cout << "contents after unique():";
	for (auto val : x)
		std::cout << ' ' << val;
	std::cout << '\n';
	system("pause");
	return 0;
}