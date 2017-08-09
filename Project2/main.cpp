#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"time.h"
#include"ptr.h"
#include"Container/string.h"
#include"debug.h"
#include"Container/pair.h"
#include"Container/list.h"
#include"Container/vector.h"
#include"Container/forward_list.h"
#include"file.h"
#include"Container/array.h"
#include"Container/priority_queue.h"
#include"Container/rbtree.h"

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
	std::vector<int>a;
	auto  o = std::move(a.begin(), a.end(), a.begin());

	system("pause");
	return 0;
}