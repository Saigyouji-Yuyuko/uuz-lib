#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include"time.h"
#include"ptr.h"
#include"string.h"
#include"debug.h"
#include"pair.h"
#include"list.h"
#include"vector.h"
#include"forward_list.h"
#include"file.h"
#include"array.h"
#include"priority_queue.h"


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
	std::array<char, 6> letters{ 'o', 'm', 'g', 'w', 't', 'f' };
	if (!letters.empty()) {
		std::cout << "The last character is: " << letters.back() << '\n';
	}
	system("pause");
	return 0;
}