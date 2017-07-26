#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"time.h"
#include"ptr.h"
#include"string.h"
#include"debug.h"
#include"pair.h"
#include"list.h"
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
#include<filesystem>
#include<array>
#include<algorithm>
#include<numeric>
#include<iomanip>
#pragma warning(disable:4996)
std::ostream& operator<<(std::ostream& ostr, const uuz::list<int>& list)
{
	for (auto &i : list) {
		ostr << " " << i;
	}
	return ostr;
}

int main()
{
	uuz::list<int> list = { 8,7,5,9,0,1,3,2,6,4 };

	std::cout << "before:     " << list << "\n";
	list.sort();
	std::cout << "ascending:  " << list << "\n";
	list.sort(std::greater<int>());
	std::cout << "descending: " << list << "\n";
	system("pause");
	return 0;
}