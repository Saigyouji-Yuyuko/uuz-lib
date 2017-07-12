#pragma once
#include<iostream>
#include<cassert>
namespace uuz
{
	template<typename T>
	void print(const T& t)
	{
		std::cout << t << std::endl;
	}
}