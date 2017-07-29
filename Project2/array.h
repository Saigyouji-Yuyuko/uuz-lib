#pragma once
#include"prepare.h"
#include<cstddef>
#include<algorithm>
namespace uuz
{
	template<typename T, std::size_t N>
	class array;

	template<typename T, std::size_t N>
	class array_iterator
	{
		using self = array_iterator;
		friend class array<T,N>;
	public:
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
	private:
		explicit constexpr  self(T* t) :dat{ t } {}
		explicit constexpr self(const T* t) :dat{ const_cast<T*>(t) } {}
		T* dat = nullptr;
	};

	template<typename T,std::size_t N>
	class array
	{
		using iterator = array_iterator<T,N>;
		using size_t = unsigned int;
	public:
		constexpr array() = default;
		array(const T& p)
		{
			for (auto i = 0; i != N; ++i)
				what[i] = p;
		}
		array(size_t a, const T& p = T{})
		{
			assert(a <= N);
			for (auto i = 0; i != a; ++i)
				what[i] = p;
		}
		template<size_t Y,typename = std::enable_if_t<(Y <= N)>>
		array(const T(&a)[Y])
		{
			std::copy(a, a + Y, what);
		}
		template<typename InputIt,typename = is_input<T,InputIt>>
		array(const InputIt& a, const InputIt& b)
		{
			auto k = a;
			for (auto i = 0; i != N && k != last; ++i, ++k)
				what[i] = *k;
		}
		array(const std::initializer_list<T>& init):array(init.begin(),init.end()){}

		array& operator=(const std::initializer_list<T>& init)
		{
			auto t{ init };
			this->swap(std::move(t));
			return *this;
		}
		template<size_t Y, typename = std::enable_if_t<(Y <= N)>>
		array& operator=(const T (&a)[N])
		{
			auto t{ a };
			this->swap(std::move(t));
			return *this;
		}

		constexpr T& at(size_t pos)
		{
			if (pos < N)
				return what[pos];
			throw("");
		}
		constexpr const T& at(size_t pos) const
		{
			if (pos < N)
				return what[pos];
			throw("");
		}

		constexpr T& operator[](size_t pos)noexcept
		{
			return what[pos];
		}
		constexpr const T& operator[](size_t pos) const noexcept 
		{
			return what[pos];
		}

		constexpr T& front() noexcept
		{
			return what[0];
		}
		constexpr const T& front() const noexcept
		{
			return what[0];
		}

		constexpr T& back()noexcept
		{
			return back[N - 1];
		}
		constexpr const T& back() const noexcept
		{
			return back[N - 1];
		}

		constexpr T* data() noexcept
		{
			return &what;
		}
		constexpr const T* data() const noexcept
		{
			return &what;
		}

		constexpr bool empty()const noexcept
		{
			return N == 0;
		}
		constexpr size_t size() const noexcept
		{
			return N
		}
		constexpr size_t max_size() const noexcept
		{
			return N;
		}

		void fill(const T& value)noexcept(what[0] = value)
		{
			for (const auto&i : what)
				i = value;
		}

		constexpr iterator begin() noexcept
		{
			return iterator{ what };
		}
		constexpr const iterator begin() const noexcept
		{
			return iterator{ what };
		}
		constexpr const iterator cbegin() const noexcept
		{
			return iterator{ what };
		}

		constexpr iterator end() noexcept
		{
			return iterator{ (what+N) };
		}
		constexpr const iterator end() const noexcept
		{
			return iterator{ (what + N) };
		}
		constexpr const iterator cend() const noexcept
		{
			return iterator{ (what + N) };
		}

		void swap(array& t)noexcept
		{
			using std::swap;
			std::swap(what, t.what);
		}


	private:
		T what[N];
	};
}