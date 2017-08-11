#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"ptr.h"
#include"test\test.h"
#include"Container\allocator.h"
#include"Container\array.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"

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
MYTINYSTL_TEST_(AddTestDemo)
{
	EXPECT_EQ(3, std::max(3, 2));
	EXPECT_EQ(2, std::max(1, 1));
}
int main()
{
	std::cout.sync_with_stdio(false);
	
	RUN_ALL_TESTS();

	system("pause");
	return 0;
}