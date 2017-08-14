#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"ptr.h"
#include"test\test.h"
#include"test\debug.h"
#include"Container\allocator.h"
#include"Container\array.h"
#include"Container\vector.h"
#include"Container\list.h"
#include"Container\forward_list.h"
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
struct a
{
	~a() { std::cout << "ddd" << std::endl; }
};
struct b : public a
{
	a g;
};
int main(void) 
{
	
	{
		b c;
	}
	system("pause");
	return 0;
}