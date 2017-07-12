#pragma once
#include<cstdio>
#include<cassert>
#include<fstream>
#include<iostream>
#ifdef USING_UUZ
#include"string.h"
#include"vector.h"
#else
#include<string>
#include<vector>
using std::string;
using std::vector;
#endif
namespace uuz
{
	class file 
	{
		const static unsigned int maxline = 4096;
	public:
		file() : begin((char*)malloc(maxline+1)), limit(begin + maxline), avail(limit) { assert(p && begin && limit && avail); };
		file(const char* c, const char* w = "rb+") : begin((char*)malloc(maxline+1)), limit(begin + maxline), avail(limit) { fopen_s(&p, c, w); assert(p && begin && limit && avail); }
		~file()
		{
			if (p)
				fclose(p);
			if (begin)
				free(begin);
		}
		const char get()
		{
			if (avail == limit)
				getnew();
			return avail == limit ? *avail : *avail++;
		}
		string getline()
		{
			string::size_t dd = 0;
			string p;
			if (avail == limit)
				getnew();
			while (*(avail + dd) != EOF && *(avail + dd) != '\n')
			{
				++dd;
				//std::cout << dd << std::endl;
				if ((avail + dd) == limit)
				{
					getnew();
					//std::cout << *begin << " " << *avail << std::endl;
				}	
				if ((avail + dd) == limit)
				{
					p += string{ avail };
					avail = limit;
					getnew();
					dd = 0;
				}
			}
			p += string{ avail};
			avail = avail + dd + 1 >limit ?limit: avail + dd + 1;
			return p;
		}
		vector<string> getlines()
		{
			vector<string> p;
			auto&& t{ getline() };
			while (!t.empty())
			{
				p.push_back(move(t));
				t = move(getline());
			}
			return p;
		}
	private:
		void getnew()
		{
			if (!flag)
				return;
			auto temp = limit - avail;
			memcpy(begin, avail, temp);
			avail = begin;
			limit = begin + maxline;
			auto sized = limit - (begin + temp);
			auto trued = fread(begin + temp,1, sized, p);
			limit = begin + temp + trued;
			if (trued < sized)
			{
				flag = false;
				*limit = EOF;
			}
		}
		unsigned int size = 0;
		bool flag = true;
		FILE* p = stdin;
		char* begin = nullptr;
		char* limit = nullptr;
		char* avail = nullptr;
	};
}