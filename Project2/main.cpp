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
#include<array>
#include<algorithm>
#include<numeric>
#include <iomanip>
#pragma warning(disable:4996)
template<int N>
struct addd
{
	static const int value = N + addd<N-1>::value;
};
template<>
struct addd<1>
{
	static const int value = 1;
};
int main()
{
	std::cout << addd<60>::value << std::endl;

	return 0;
}