#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<vector>
#include<memory>
#include <ctime>
#include<queue>
#include<list>
#include"time.h"
#include<functional>
#include<forward_list>
#include"string.h"
#include"debug.h"
#pragma warning(disable:4996)
int main()
{
	uuz::string p{ "12312345163123123" };
	p += p+p;
	uuz::print(p);
	uuz::print(p.size());
	uuz::print(p.count("123"));
	uuz::string d{ std::move(p) };
	uuz::print(d);
	//std::cin >> d;
	uuz::print(d);
	
	system("pause");
	return 0;
}
