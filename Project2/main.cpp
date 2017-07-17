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
void print_vec(const uuz::vector<int>& vec)
{
	for (auto x : vec) {
		std::cout << ' ' << x;
	}
	std::cout << '\n';
}

int main()
{
	uuz::vector<int> vec(3, 100);
	print_vec(vec);

	auto it = vec.begin();
	it = vec.insert(it, 200);
	print_vec(vec);

	vec.insert(it, 2, 300);
	print_vec(vec);

	// "it" no longer valid, get a new one:
	it = vec.begin();

	uuz::vector<int> vec2(2, 400);
	//vec.insert(it + 2, vec2.begin(), vec2.end());
	//print_vec(vec);

	int arr[] = { 501,502,503 };
	vec.insert(vec.begin(), arr, arr + 3);
	print_vec(vec);
	system("pause");
	return 0;

}