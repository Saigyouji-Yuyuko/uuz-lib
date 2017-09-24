#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
//#include"ptr.h"
#include"test\test.h"
//#include"test\debug.h"
//#include"Container/set.h"
//#include"Container\allocator.h"
//#include"Container\array.h"
//#include"Container\vector.h"
//#include"Container\list.h"
//#include"Container\forward_list.h"
//#include"Container\set.h"
//#include"test\Container\vector_test.h"
//#include"Container\deque.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"
#include<future>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<fstream>
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
#include<array>
#include<numeric>
#include<iomanip>
#include<set>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<sstream>
#include<algorithm>
#include"Container\pair.h"
using namespace std;
int main()
{
	cout << std::is_trivially_destructible_v<uuz::pair<std::vector<int>, int>> << endl;
	auto a = uuz::make_pair(1,2) ;
	cout << a.first;
	system("pause");
	return 0;
}