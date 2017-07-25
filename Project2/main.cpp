#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"time.h"
#include"ptr.h"
#include"string.h"
#include"debug.h"
#include"vector.h"

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
#include <iomanip>
#pragma warning(disable:4996)

int main()
{
	for (int ii = 1; ii <= 100000000; ii *= 10)
	{
		{
			uuz::time a{ "all-std::" };
			std::vector<int> p;
			{
				uuz::time a1{ "std::vector::push_back  " + uuz::tostring(ii) };
				for (int i = 0; i != ii; ++i)
					p.push_back(i);
			}

			{
				uuz::time a1{ "std::vector::erase  " + uuz::tostring(ii) };
				while (!p.empty())
					p.erase(p.begin());
			}

		}
		{
			uuz::time a{ "all-uuz::" };
			std::vector<int> p;
			{
				uuz::time a1{ "uuz::vector::push_back  " + uuz::tostring(ii) };
				for (int i = 0; i != ii; ++i)
					p.push_back(i);
			}

			{
				uuz::time a1{ "uuz::vector::erase  " + uuz::tostring(ii) };
				while (!p.empty())
					p.erase(p.begin());
			}

		}
	}
	system("pause");

	return 0;
}