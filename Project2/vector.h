#pragma once
#include<cstring>
#include<cstdlib>
#include"prepare.h"
#include<initializer_list>
#include<cassert>
#include<cmath>
#include<algorithm>
#include<iostream>
namespace uuz
{
	constexpr float vector_speed = 1.5;
	template<typename T,typename A>
	class vector;

	template<typename T>
	class vector_iterator
	{
		using self = vector_iterator;
	public:
		vector_iterator() = delete;
		self(const self& t):dat{t.dat}{}
		self(self&& t) :dat{ t.dat } { t.dat = nullptr; }
	
		self& operator=(const self& t)noexcept
		{
			dat = t.dat;
			return *this;
		}
		self& operator=(self&& t)noexcept
		{
			if (this == &t)
				return *this;
			dat = t.dat;
			t.dat = nullptr;
		}
		self& operator+=(const int t)noexcept
		{
			dat += t;
			return *this;
		}
		self& operator-=(const int t)
		{
			dat -= t;
			return *this;

		}
		self& operator++() noexcept 
		{ 
			*this += 1;
			return *this;
		}
		self operator++(int)noexcept 
		{ 
			auto p{ *this };
			*this += 1;
			return p;
		}
		self& operator--()noexcept
		{ 
			*this -= 1;
			return *this; 
		}
		self operator--(int) noexcept 
		{
			auto p{ *this }; 
			*this -= 1;
			return p;
		}

		T& operator*()noexcept
		{
			return *dat;
		}
		const T& operator*()const noexcept
		{
			return *dat;
		}
		T* operator->()noexcept
		{
			return dat;
		}
		const T* operator->()const noexcept
		{
			return dat;
		}

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.dat == b.dat;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return !(a == b);
		}
		friend bool operator<(const self& a, const self& b)noexcept
		{
			return a.dat < b.dat;
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
			return a > b || a == b;
		}

		friend self operator+(const self& a, const size_t b)
		{
			self c{ a };
			c += b;
			return c;
		}
		friend int operator-(const self& a, const self& b)noexcept
		{
			return a.dat - b.dat;
		}
		friend self operator-(const self& a, const int b)
		{
			self c{ a };
			c -= b;
			return c;
		}

		//~vector_iterator(){}
	//private:
		explicit self(T* t) :dat{ t }{}
		explicit self(const T* t) :dat{ const_cast<T*>(t) } {}
		T* dat = nullptr;
	};

	template<typename T,typename Allocator = uuz::allocator>
	class vector
	{
		using self = vector<T, Allocator>;
		using size_t = uint32_t;
		

		using value_type = T;
		using allocator_type = Allocator;
		using size_type = uint32_t;
		using difference_type = int;
		using reference =value_type&;
		using const_reference = const value_type&;
//		using pointer = std::allocator_traits<Allocator>::pointer;
//		using const_pointer = std::allocator_traits<Allocator>::const_pointer;
		using iterator = vector_iterator<T>;
		using const_iterator=const  vector_iterator<T>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		friend vector_iterator<T>;

	public:
		self() = default;
		self(size_t t, const T& value)
		{
			reserve(t);
			for (auto i = 0; i < t; ++i)
				auto k = new(shuju + i) T{ value };
			ssize = t;
		}
		self(const size_t t)
		{
			reserve(t);
		}	
		self(const self& t)
		{
			initfrom(t.begin(), t.end());
		}
		self(self&& t):self()
		{
			this->swap(t);
		}
		self(const std::initializer_list<T>& init)
		{
			initfrom(init.begin(), init.end());
		}
		//�ش� bug 
		template<typename InputIt, std::enable_if_t<
			std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value,
			int> = 0>
		self(InputIt first, InputIt last)
		{
			initfrom(first, last);
		}

		self& operator=(const self& other)
		{
			if (this == &other)
				return *this;
			auto temp{ other };
			this->swap(temp);
			return *this;
		}
		self& operator=(self&& other)
		{
			if (this == &other)
				return *this;
			auto temp{ std::move(other) }; 
			this->swap(temp);
			return *this;
		}
		self& operator=(const std::initializer_list<T>& ilist)
		{
			auto temp( ilist );
			this->swap(temp);
			return *this;
		}

		void assign(const size_t count, const T& value)
		{
			auto temp= self( count,value );
			this->swap(temp);
		}
		//bug
		template< class InputIt, std::enable_if_t<
			std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value,
			int> = 0>
		void assign(const InputIt& first,const InputIt& last)
		{
			auto temp{ first,last };
			this->swap(temp);
		}
		void assign(const std::initializer_list<T>& ilist)
		{
			auto temp{ ilist };
			this->swap(temp);
		}

		T& at(const size_t pos)
		{
			assert(pos < size());
			return  *(data() + pos);
		}
		const T& at(const size_t pos) const
		{
			assert(pos < size());
			return  *(data() + pos);
		}

		T& operator[](size_t pos)noexcept
		{
			return *(data() + pos);
		}
		const T& operator[](size_t pos) const noexcept
		{
			return *(data() + pos);
		}

		T& front()noexcept
		{
			return *(data());
		}
		const T& front() const noexcept
		{
			return *(data());
		}

		T& back()noexcept
		{
			return *(data()+size() - 1);
		}
		const T& back() const noexcept
		{
			return *(data() + size() - 1);
		}

		T* data()noexcept
		{
			return shuju;
		}
		const T* data() const noexcept
		{
			return shuju;
		}

		iterator begin() noexcept
		{
			return iterator{ data() };
		}
		const iterator begin() const noexcept
		{
			return iterator{ data() };
		}
		const iterator cbegin() const noexcept
		{
			return iterator{ data() };
		}

		iterator end() noexcept
		{
			return iterator{ data() + size() };
		}
		const iterator end() const noexcept
		{
			return iterator{ data() + size() };
		}
		const iterator cend() const noexcept
		{
			return iterator{ data() + size() };
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}

		size_t size() const noexcept
		{
			return ssize;
		}

		size_t max_size() const noexcept
		{
			return maxsize;
		}

		void reserve(size_t new_cap)
		{
			if (new_cap <= max_size())
				return;
			auto temp = (T*)malloc(new_cap * sizeof(T));
			assert(temp);
			if (shuju)
			{
				memcpy(temp, shuju, size() * sizeof(T));
				free(shuju);
			}
			shuju = temp;
			maxsize = new_cap;
		}

		size_t capacity() const noexcept
		{
			return max_size();
		}

		void shrink_to_fit()noexcept
		{
			reserve(size());
		}
		
		void clean()noexcept
		{
			for (auto i = shuju; i != shuju + size(); ++i)
				i->~T();
			ssize = 0;
		}

		void clear()noexcept
		{
			if (shuju)
				clean();
			free(shuju);
			shuju = nullptr;
			maxsize = 0;
		}

		iterator insert(const iterator pos, const T& value)
		{
			auto temp{ value };
			return insert(pos, std::move(value));
		}
		iterator insert(const iterator pos, T&& value)
		{
			auto p = pos-begin();
			reserve(size() + 1);
			std::copy((char*)(data() + p), (char*)(data() + size()), (char*)(data() + p + 1));
			auto k = new(data()+p) T(std::move(value));
			++ssize;
			return begin() + p ;
		}
		iterator insert(const iterator pos, size_t count, const T& value)
		{
			auto temp{ value };
			auto p = pos - begin();
			reserve(size() + count);
			std::copy((char*)(data() + p), (char*)(data() + size()), (char*)(data() + p + count));
			for (int i = 0; i < count; ++i)
				auto k = new(data() + p + i) T(temp);
			ssize += count;
			return begin() + p ;
		}
		//bug
		template< typename InputIt, std::enable_if_t<
			std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value,
			int> = 0>
		iterator insert(const iterator pos,  InputIt first, InputIt last)
		{
			auto p = pos - begin();
			auto dis = last - first;
			auto temp = (T*)malloc(dis * sizeof(T));
			assert(temp);
			std::copy(first, last, temp);
			reserve(size() + dis);
			std::copy((char*)(data() + p), (char*)(data() + size()), (char*)(data() + p + dis));
			for (int i = 0; i < dis; ++i)
				auto k = new(data() + p + i) T(std::move(*(temp + i)));
			ssize += dis;
			return begin() + p;
		}
		iterator insert(const iterator pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		template< typename... Args >
		iterator emplace(const iterator pos, Args&&... args)
		{
			auto k = T( std::forward<Args>(args)... );
			insert(pos, std::move(k));
		}

		iterator erase(const iterator pos)
		{
			return erase(pos, pos + 1);
		}
		iterator erase(const iterator first, const iterator last)
		{
			for (auto i = first; i != last; ++i)
				(*i).~T();
			
			auto dis1 = first - begin();
			auto dis2 = last - first;
			std::copy((char*)(data() + dis1 + dis2), (char*)(data() + size()), (char*)(data() + dis1));
			ssize -= dis2;
			return first;
		}

		void push_back(const T& value)
		{
			auto temp{ value };
			emplace_back(std::move(temp));
		}
		void push_back(T&& value)
		{
			 emplace_back(std::move(value));
		}

		template< class... Args >
		T& emplace_back(Args&&... args)
		{
			if (size() == max_size())
				reserve(size() == 0 ? 1 : ceil(size()*vector_speed));
			auto k = new(data() + size()) T(std::forward<Args>(args)...);
			++ssize;
			return *k;
		}

		void pop_back()
		{
			erase(end());
		}

		void resize(const size_t count)
		{
			resize(count, T{});
		}
		void resize(const size_t count, const T& value)
		{
			auto temp = T{ value };
			if (count < size())
				erase(begin() + count, end());
			else if (count > size())
			{
				reserve(count);
				for (int i = size(); i != count; ++i)
					auto k = new(data() + i) T(temp);
				ssize = count;
			}

		}

		void swap(self& other)noexcept
		{
			using std::swap;
			swap(other.shuju, shuju);
			swap(ssize, other.ssize);
			swap(maxsize, other.maxsize);
		}
		
		friend void swap(self& a, self&b)noexcept
		{
			a.swap(b);
		}

		template<typename T1, typename T2>
		friend bool operator==(const vector<T1>& lhs, const vector<T2>& rhs)noexcept
		{
			if (lhs.size() != rhs.size())
				return false;
			return lhs.comp(rhs) == 0;
		}

		template<typename T1, typename T2>
		friend bool operator!=(const vector<T1>& lhs, const vector<T2>& rhs)noexcept
		{
			return !(lhs == rhs);
		}

		template<typename T1, typename T2>
		friend bool operator<(const vector<T1>& lhs, const vector<T2>& rhs)noexcept
		{
			return lhs.comp(rhs) == -1;
		}

		template<typename T1, typename T2>
		friend bool operator<=(const vector<T1>& lhs, const vector<T2>& rhs)noexcept
		{
			return lhs.comp(rhs) <= 0;
		}

		template<typename T1, typename T2>
		friend bool operator>(const vector<T1>& lhs, const vector<T2>& rhs)noexcept
		{
			return lhs.comp(rhs) == 1;
		}

		template<typename T1, typename T2>
		friend bool operator>=(const vector<T1>& lhs, const vector<T2>& rhs)noexcept
		{
			return lhs.comp(rhs) >= 0;
		}

		~vector()noexcept
		{
			clear();
		}
	private:
		template<typename U>
		void initfrom(const U& a, const U& b)
		{
			auto dis = b-a;
			assert(dis >= 0);
			reserve(dis);
			for (auto i = 0; i != dis; ++i)
				auto k = new(shuju + i) T(*(a + i));
			ssize = dis;
		}

		int comp(const self& t)const noexcept
		{
			auto k = std::min(size(), t.size());
			for (int i = 0; i != k; ++i)
			{
				if (this->operator[](i) < t[i])
					return -1;
				else if (this->operator[](i) > t[i])
					return 1;
			}
			if (size() > k)
				return 1;
			else if (t.size() > k)
				return -1;
			return 0;
		}

		T* shuju = nullptr;
		size_t ssize = 0;
		size_t maxsize = 0;
	};

}