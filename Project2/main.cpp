#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"ptr.h"
#include"test\test.h"
#include"test\debug.h"
#include"Container/set.h"
#include"Container\allocator.h"
#include"Container\array.h"
#include"Container\vector.h"
#include"Container\list.h"
#include"Container\forward_list.h"
#include"Container\set.h"
#include"test\Container\vector_test.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"

#include<set>
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
using namespace std;
void display_sizes(const uuz::multiset<int> &nums1,
	const uuz::multiset<int> &nums2,
	const uuz::multiset<int> &nums3)
{
	std::cout << "nums1: " << nums1.size()
		<< " nums2: " << nums2.size()
		<< " nums3: " << nums3.size() << '\n';
}

int main()
{
	uuz::multiset<int> nums1{ 3, 1,3,1,4,9, 4, 6, 5, 9 };
	uuz::multiset<int> nums2;
	uuz::multiset<int> nums3;

	std::cout << "Initially:\n";
	display_sizes(nums1, nums2, nums3);

	// copy assignment copies data from nums1 to nums2
	nums2 = nums1;

	std::cout << "After assigment:\n";
	display_sizes(nums1, nums2, nums3);

	// move assignment moves data from nums1 to nums3,
	// modifying both nums1 and nums3
	nums3 = std::move(nums1);

	std::cout << "After move assigment:\n";
	display_sizes(nums1, nums2, nums3);
	for (auto& i : nums3)
		std::cout << i << " ";
	system("pause");
	return 0;
}