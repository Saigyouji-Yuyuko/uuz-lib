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
#pragma warning(disable:4996)
int main()
{
	uuz::vector<char> characters;

	characters.assign(5, 'a');

	for (char c : characters) {
		std::cout << c << '\n';
	}
	system("pause");
	return 0;

}