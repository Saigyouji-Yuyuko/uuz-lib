#pragma once
#include<cstring>
#include<cstdlib>
#include"prepare.h"
#include<initializer_list>
#include<cassert>
#include<cmath>
#include<iostream>
namespace uuz
{
	constexpr float vector_speed = 1.5;
	/*template<typename T>
	class vector_iterator 
	{
		using self = vector_iterator<T>;
		friend vector<T>;
	public:
		self() = default;
		self(const self& d) :p{ d.p } {}

		self& operator=(const self& d) { p = d.p; return *this; }

		virtual use_type& operator*()override { return *p; }
		virtual const use_type& operator*()const override { return *p; }

		virtual self& operator++()override { ++p; return *this; }
		self operator++(int) { auto l = *this; ++p; return l; }
		virtual self& operator--()override { --p; return *this; }
		self operator--(int) { auto l = *this; --p; return l; }

		virtual self& operator-=(const int i)override { p -= i; return *this; }
		virtual self& operator+=(const int i)override { p += i; return *this; }
		friend int operator-(const self& a, const self& b)
		{
			return a.p - b.p;
		}
		friend int operator-(const self& a, const size_t b)
		{
			return self{ a.p - b };
		}
		friend self operator+ (const self& a, const size_t b)
		{
			return self{ a.p + b };
		}

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.p == b.p;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return !(a == b);
		}
		friend bool operator<(const self& a, const self& b)noexcept
		{
			return a.p < b.p;
		}
		friend bool operator>(const self& a, const self& b)noexcept
		{
			return b < a;
		}
		friend bool operator<=(const self& a, const self& b)noexcept
		{
			return a < b || a == b;
		}
		friend bool operator>=(const self& a, const self& b)noexcept
		{
			return b < a || a == b;
		}

		~vector_iterator() = default;
	private:
		self(const T* d) :p{ d } {  }
		self(void* d) :p{ (T*)d } {  }
		T* p = nullptr;
	};*/
	template<typename T>
	class vector_iterator
	{

	};

	template<typename T>
	struct vector_impl
	{
		using size_t = uint32_t;
		using self = vector_impl;
		self() = default;
		self(const size_t t) :begin((T*)malloc(t * sizeof(T))),
							  max_size(t)
		{
			assert(begin);
		}
		self(const self& t):begin((T*)malloc(t.ssize*sizeof(T))),
							max_size(t.ssize)
		{
			std::copy(t.begin, t.begin + ssize, begin);
		}
		self(self&& t)
		{
			using std::swap;
			swap(t.begin, begin);
			swap(ssize, t.ssize);
			swap(max_size, t.max_size);
		}

		self& operator=(const self& t)
		{
			auto temp{ t };
			swap(t.begin, begin);
			swap(ssize, t.ssize);
			swap(max_size, t.max_size);
		}
		self& operator=(self&& t)
		{
			auto temp{ std::move(t) };
			swap(t.begin, begin);
			swap(ssize, t.ssize);
			swap(max_size, t.max_size);
		}

		void clear()noexcept
		{
			clean();
			free(begin);
			begin = nullptr;
			max_size = 0;
		}
		void clean()noexcept
		{
			for (auto i = begin; i != begin + ssize; ++i)
				i->~T();
			ssize = 0;
		}
		size_t size()const noexcept
		{
			return ssize;
		}

		void insert(const size_t pos, const T* p, const size_t s)
		{
			if (s + size() <= max_size)
			{
				auto temp = (T*)malloc((size() - pos + 1) * sizeof(T));
				std::copy(begin + pos, begin + ssize, temp);
				std::copy(p, p + s, begin + pos);
				std::copy(temp, temp + ssize - pos, begin + pos + s);
				free(temp);
				ssize += s;
			}
			else
			{

			}
		}


		~self()
		{
			clear();
		}
		T* begin = nullptr;
		size_t ssize = 0;
		size_t max_size = 0;



		T* getacopy(const size_t p)
		{
			auto t = (T*)malloc(p * sizeof(T));
			assert(t);
			std::copy(begin,begin+ssize,t);
			//max_size = p;
			return t;
		}

	};


	template<typename T>
	class vector
	{
		using iterator = vector_iterator<T>;
		using self = vector<T>;
		using size_t = uint32_t;
		friend vector_iterator<T>;
		

		
		
	public:


	
	private:
	
	};

	

}