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
	for (int i = 1; i <= 100000000; i *= 10)
	{
		std::cout << i << std::endl;
		{
			uuz::time t{ "std::" };
			std::vector<std::string> p;
			for (int j = 1; j <= i; ++j)
				p.push_back("123");
			while (!p.empty())
				p.pop_back();
		}
		{
			uuz::time t{ "uuz::" };
			uuz::vector<std::string> p;
			for (int j = 1; j <= i; ++j)
				p.push_back("123");
			while (!p.empty())
				p.pop_back();
		}
		std::cout << ".........................." << std::endl;
	}
	system("pause");
	return 0;
}