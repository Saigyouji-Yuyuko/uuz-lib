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
							max_size(t.ssize),
							ssize(t.ssize)
		{
			for (auto i = t.begin; i != t.begin + t.ssize; ++i)
				auto p = new(begin + (i - t.begin)) T(*i);
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
				assert(temp);
				/*std::copy(begin + pos, begin + ssize, temp);
				std::copy(p, p + s, begin + pos);
				std::copy(temp, temp + ssize - pos, begin + pos + s);*/
				std::copy(begin + pos, begin + ssize, temp);
				std::copy(p, p + s, begin + pos);
				mulitmove(temp, temp + ssize - pos, begin + pos + s);
				free(temp);
				ssize += s;
			}
			else
			{
				auto ss = (ssize *vector_speed >= ssize + s) ? ssize *vector_speed : ssize + s;
				auto temp = (T*)malloc(ss * sizeof(T));
				assert(temp);
				/*std::copy(begin, begin + pos, temp);
				std::copy(p, p + s, temp + pos);
				std::copy(begin + pos, begin + ssize, temp + pos + s);*/
				std::copy(begin, begin + pos, temp);
				std::copy(p, p + s, temp + pos);
				mulitmove(begin + pos, begin + ssize, temp + pos + s);
				free(begin);
				begin = temp;
				ssize += s;
				max_size = ss;
			}
		}
		void append(const T* p, const size_t s)
		{
			if (s + size() <= max_size)
			{
				std::copy(p, p + s, begin + size);
				ssize += s;
			}
			else
			{
				auto ss = (ssize *vector_speed >= ssize + s) ? ssize *vector_speed : ssize + s;
				auto temp = (T*)malloc(ss * sizeof(T));
				assert(temp);
				std::copy(begin, begin + ssize, temp);
				std::copy(p, p + s, temp + ssize);
				free(begin);
				ssize += s;
				max_size = ss;
			}
		}
		void erase(const size_t pos1, const size_t pos2)
		{
			for (auto i = begin + pos1; i != begin + pos2; ++i)
				i->~T();
			mulitmove(begin + pos2, begin + ssize, begin + pos1);
			ssize -= pos2 - pos1;
		}

		~self()
		{
			clear();
		}
		T* begin = nullptr;
		size_t ssize = 0;
		size_t max_size = 0;

	};


	template<typename T>
	class vector
	{
		using iterator = vector_iterator<T>;
		using self = vector<T>;
		using size_t = uint32_t;
		friend vector_iterator<T>;
		
		vector_impl core;
		
		
	public:
		self() = default;
		self(const size_t n):core{n}{}
		self(const self& t):core{t.core}{}
		self(self&& t):core{std::move(t.core)}{}
		template<int N>
		self(const T(&a)[N]) : core{ N } { core.insert(a, N); }
		self(const T* a, const size_t p) :core{ p } { core.insert(a,p); }
		self(const iterator& a, const iterator& b)
		{
			core.assign(a.data, b - a);
		}
		template<int U>
		self(const U& a, const U&b)
		{
			core.assign(&(*a), b - a);
		}



		~vector() = default;
	private:
	
	};

	

}