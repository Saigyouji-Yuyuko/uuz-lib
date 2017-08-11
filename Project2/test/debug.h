#pragma once
#include<iostream>
#include<deque>
#include<cassert>
#include<string>
#include<chrono>
namespace uuz
{
	//���һ��T
	template<typename T>
	void println(const T& t)
	{
		std::cout << t << std::endl;
	}

	//����һ���ʱ�䣬�����һ���ַ�������˵��
	class block_time
	{
	public:

		block_time(const char* p) :start{ std::chrono::system_clock::now() }, l{ p } {}
		block_time() :start{ std::chrono::system_clock::now() }, l{ "" } {};
		block_time(const std::string& p) :start{ std::chrono::system_clock::now() }, l{ p } {}
		block_time(std::string&& p) :start{ std::chrono::system_clock::now() }, l{ std::move(p) } {}
		block_time(const block_time&) = delete;
		block_time(block_time&&) = delete;
		block_time& operator=(const block_time&) = delete;
		block_time& operator=(block_time&&) = delete;
		~block_time()
		{
			auto end = std::chrono::system_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			std::cout << l << " " << double(duration.count()) * std::chrono::microseconds::period::num / 1000 << "ms" << std::endl;
		}
	private:
		std::chrono::time_point<std::chrono::system_clock> start;
		std::string l;
	};




}