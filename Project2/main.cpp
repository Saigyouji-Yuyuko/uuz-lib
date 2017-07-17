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
int main()
{
	// Create a vector containing integers
	uuz::vector<int> v = { 7, 5, 16, 8 };

	// Add two more integers to vector
	v.push_back(25);
	v.push_back(13);

	// Iterate and print values of vector
	for (int n : v) {
		std::cout << n << '\n';
	}
	system("pause");
	return 0;

}