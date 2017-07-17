#pragma once
#include<chrono>
#include<iostream>
#ifdef USING_UUZ
#include"string.h"
#else
#include<string>
using std::string;
#endif
namespace uuz
{
	class time
	{
	public:
		time(const char* p):start{ std::chrono::system_clock::now()},l{p}{}
		time() :start{ std::chrono::system_clock::now() }, l{ "" } {};
		time(const time&) = delete;
		time(time&&) = delete;
		time& operator=(const time&) = delete;
		time& operator=(time&&) = delete;
		~time()
		{
			auto end = std::chrono::system_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			std::cout << l << " " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den<<'s'<<std::endl;
		}
	private:
		std::chrono::time_point<std::chrono::system_clock> start;
		string l;
	};
}