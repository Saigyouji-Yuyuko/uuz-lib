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
	for (int i = 1; i <= 100000000; i *= 10)
	{
		uuz::print(i);
		{
			uuz::time ii{ "std" };
			std::vector<int> k;
			for (int j = 0; j != i; ++j)
				k.push_back(j);
			while (!k.empty())
				k.pop_back();
		}
		{
			uuz::time ii{ "uuz" };
			uuz::vector<int> k;
			for (int j = 0; j != i; ++j)
				k.push_back(j);
			while (!k.empty())
				k.pop_back();
		}
		uuz::print("-----------------");
	}
	system("pause");
}