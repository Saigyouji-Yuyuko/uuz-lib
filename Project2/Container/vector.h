#pragma once
#include<cstring>
#include<cstdlib>
#include"container.h"
#include<initializer_list>
#include<cassert>
#include<cmath>
#include<algorithm>
#include<iostream>
namespace uuz
{
	constexpr float vector_speed = 1.7;//??
	template<typename T,typename A>
	class vector;

	template<typename T,typename A>
	class vector_iterator
	{
		using self = vector_iterator;
		friend class vector<T, A>;
	public:
		self& operator+=(const int t)noexcept
		{
			dat += t;
			return *this;
		}
		self& operator-=(const int t)noexcept
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

		friend self operator+(const self& a, const size_t b)noexcept
		{
			self c{ a };
			c += b;
			return c;
		}
		friend int operator-(const self& a, const self& b)noexcept
		{
			return a.dat - b.dat;
		}
		friend self operator-(const self& a, const int b)noexcept
		{
			self c{ a };
			c -= b;
			return c;
		}

		//~vector_iterator(){}
	private:
		explicit self(T* t) :dat{ t }{}
		explicit self(const T* t) :dat{ const_cast<T*>(t) } {}
		T* dat = nullptr;
	};

	template<typename T,typename Allocator = uuz::allocator<T>>
	class vector
	{
		using self = vector<T, Allocator>;
		
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = uint32_t;
		using difference_type = int;
		using reference =value_type&;
		using const_reference = const value_type&;
//		using pointer = std::allocator_traits<Allocator>::pointer;
//		using const_pointer = std::allocator_traits<Allocator>::const_pointer;
		using iterator = vector_iterator<T,Allocator>;
		using const_iterator=const  vector_iterator<T, Allocator>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	private:
		Allocator alloc{};
		T* shuju = nullptr;
		size_t ssize = 0;
		size_t maxsize = 0;

	public:
		vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}
		explicit vector(const Allocator& alloc) noexcept : alloc{alloc}{}
		self(size_t t, const T& value, const Allocator& alloc = Allocator()):vector(alloc)
		{
			reserve(t);
			auto i = 0;
			try
			{
				for (; i < t; ++i)
					auto k = new(shuju + i) T{ value };
			}
			catch (...)
			{
				for (auto j = 0; j != i; ++j)
					(*(shuju + j)).~T();
				alloc.deallocate(shuju, maxsize);
				shuju = nullptr;
				maxsize = 0;
				throw;
			}
			ssize = t;
		}
		self(const size_t t, const Allocator& alloc = Allocator()) :vector(t, T{}, alloc) {}
		self(const self& t):vector(Allocator())
		{
			initfrom(t.begin(), t.end());
		}
		self(const vector& other, const Allocator& alloc) :vector(alloc)
		{
			initfrom(t.begin(), t.end());
		}
		self(self&& t)noexcept:vector(Allocator())
		{
			this->swap(t);
		}
		self(const std::initializer_list<T>& init, const Allocator& alloc = Allocator()) :vector(alloc)
		{
			initfrom(init.begin(), init.end());
		}
		template<typename InputIt, typename = is_input<T, InputIt>>
		self(self&& other, const Allocator& alloc):vector(alloc)
		{
			this->swap(other);
		}
		template<typename InputIt, typename = is_input<T, InputIt>>
		self(InputIt first, InputIt last,const Allocator& alloc = Allocator()) :vector(alloc)
		{
			initfrom(first, last);
		}

		self& operator=(const self& other)
		{
			if (this == &other)
				return *this;
			auto temp{ other,alloc };
			this->swap(temp);
			return *this;
		}
		self& operator=(self&& other)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if (this == &other)
				return *this;
			auto temp{ std::move(other),alloc}; 
			this->swap(temp);
			return *this;
		}
		self& operator=(const std::initializer_list<T>& ilist)
		{
			auto temp( ilist ,alloc);
			this->swap(temp);
			return *this;
		}

		void assign(const size_t count, const T& value)
		{
			auto temp= self( count,value,alloc );
			this->swap(temp);
		}
		template< class InputIt, typename = is_input<T, InputIt>>
		void assign(InputIt first,InputIt last)
		{
			auto temp( first,last ,alloc);
			this->swap(temp);
		}
		void assign(const std::initializer_list<T>& ilist)
		{
			auto temp{ ilist ,alloc};
			this->swap(temp);
		}

		allocator_type get_allocator() const noexcept
		{
			return alloc;
		}


		T& at(const size_t pos)
		{
			if (pos >= ssize)
				throw(out_of_range{""});//错误描述
			return  *(data() + pos);
		}
		const T& at(const size_t pos) const
		{
			if (pos >= ssize)
				throw(out_of_range{ "" });//错误描述
			return  *(data() + pos);
		}

		T& operator[](const size_t pos)noexcept
		{
			return *(data() + pos);
		}
		const T& operator[](const size_t pos) const noexcept
		{
			return *(data() + pos);
		}

		T& front()
		{
			if (empty())
				throw(out_of_range{ "" });
			return *(data());
		}
		const T& front() const
		{
			if (empty())
				throw(out_of_range{ "" });
			return *(data());
		}

		T& back()noexcept
		{
			if (empty())
				throw(out_of_range{ "" });
			return *(data()+size() - 1);
		}
		const T& back() const noexcept
		{
			if (empty())
				throw(out_of_range{ "" });
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
			shrinksize(new_cap);
		}

		size_t capacity() const noexcept
		{
			return max_size();
		}

		void shrink_to_fit()
		{
			shrinksize(size());
		}
		
		void clean()noexcept
		{
			for (auto i = shuju; i != shuju + size(); ++i)
				(*i).~T();	
			ssize = 0;
		}

		void clear()noexcept
		{
			if (ssize)
				clean();
			alloc.deallocate(shuju,maxsize);
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
			if (pos == end())
			{
				push_back(std::move(value));
				return end();
			}
			auto p = pos - begin();
			reserve(size() + 1);
			move_or_copy_con(data() + size() - 1, 1, data() + size());
			move_or_copy_ass(data() + p, size() - p - 1, data() + p + 1);
			*(data() + p) = std::move(value);
			++ssize;
			return begin() + p ;
		}
		iterator insert(const iterator pos, size_t count, const T& value)
		{
			auto temp{ value };
			if (pos == end())
			{
				for (int i = 0; i < count-1; ++i)
					push_back(temp);
				push_back(std::move(temp));
				return end();
			}
			auto p = pos - begin();
			reserve(size() + count);
			//std::copy((char*)(data() + p), (char*)(data() + size()), (char*)(data() + p + count));
			//auto k = size() - p - count;
			move_or_copy(data() + p, size() - p, data() + p + count);
			for (int i = 0; i < count-1; ++i)
				 new(data() + p + i) T(temp);
			*(data() + p + count - 1) = std::move(temp);
			ssize += count;
			return begin() + p ;
		}
		template< typename InputIt, typename = is_input<T, InputIt>>
		iterator insert(const iterator pos,  InputIt first, InputIt last)
		{
			auto p = pos - begin();
			//auto count = distance(first,last);//必须要改
			auto count = last - first;
			reserve(size() + count);
			//std::copy((char*)(data() + p), (char*)(data() + size()), (char*)(data() + p + dis));
			move_or_copy(data() + p, size() - p, data() + p + count);
			auto k = first;
			for (int i = 0; i < count; ++i,(void)++k)
				 *(data() + p + i)=(*k);
			ssize += count;
			return begin() + p;
		}
		iterator insert(const iterator pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		template< typename... Args >
		iterator emplace(const iterator pos, Args&&... args)
		{
			auto k = T( uuz::forward<Args>(args)... );
			return insert(pos, std::move(k));
		}

		iterator erase(const iterator pos)
		{
			return erase(pos, pos + 1);
		}
		iterator erase(const iterator first, const iterator last)
		{
			if (first == begin() && last == end())
			{
				clear();
				return begin();
			}		
			auto dis1 = first - begin();
			auto dis2 = last - first;
			//std::copy((T*)(data() + dis1 + dis2), (T*)(data() + size()), (T*)(data() + dis1));
			move_or_copy_ass(data() + dis1 + dis2, size() - dis1 - dis2, data() + dis1);
			for (auto i = 0; i != dis2; ++i)
				(*(data() + ssize - i)).~T();
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
			if (ssize == maxsize)
				reserve(size() == 0 ? 1 : ceil(size()*vector_speed)); //ceil ? 
			auto k = new(shuju + ssize) T(std::forward<Args>(args)...);
			++ssize;
			return *k;
		}

		void pop_back()
		{
			auto k = shuju + ssize - 1;
			(*k).~T();
			--ssize;
		}

		void resize(const size_t count)
		{
			resize(count, T{});
		}
		void resize(const size_t count, const T& value)
		{
			auto temp = T{ value };
			if (count < size())
			{
				for (auto i = shuju + count; i != shuju + ssize; ++i)
					(*i).T();
			}
			else if (count > size())
			{
				reserve(count);
				int i = size();
				try
				{
					for (; i != count; ++i)
						new(data() + i) T(temp);
				}
				catch (...)
				{
					for (auto j = size(); j != i; ++j)
						(*(shuju + j)).~T();
					throw;
				}
			}
			ssize = count;
		}

		void swap(self& other)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			using std::swap;
			if (alloc == other.alloc)
			{
				swap(other.shuju, shuju);
				swap(ssize, other.ssize);
				swap(maxsize, other.maxsize);
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's undefined that allocate is not equal");
#else
				self temp1(other, alloc);
				self temp2(*this, other.alloc);
				this->swap(temp1);
				other.swap(temp2);
#endif 
			}
		}

		~vector()noexcept
		{
			clear();
		}
	private:
		template<typename U>
		void initfrom(const U& a, const U& b)
		{
			auto dis = b-a; //bug 双向和前向迭代器无法使用 应该使用distance
			reserve(dis);
			auto i = 0;
			try
			{
				for (; i != dis; ++i)
					auto k = new(shuju + i) T(*(a + i));
			}
			catch(...)
			{
				for (auto j = 0; j != i; ++j)
					(*(shuju + j)).~T();
				alloc.deallocate(shuju, maxsize);
				shuju = nullptr;
				maxsize = 0;
				throw;
			}
			ssize = dis;
		}

		void shrinksize(size_t new_cap)
		{
			auto temp = alloc.allocate(new_cap);
			if (shuju)
			{
				try
				{
					move_or_copy_con(shuju, ssize, temp);
				}
				catch (...)
				{
					alloc.deallocate(temp, new_cap);
					throw;
				}
				for (auto i = shuju; i != shuju + ssize; ++i)
					(*i).~T();
				using std::swap;
				swap(shuju, temp);
				alloc.deallocate(temp, maxsize);
			}
			else
				shuju = temp;
			maxsize = new_cap;
		}

	};
	namespace
	{
		template<typename T,typename U>
		int comp(const vector<T, U>& a,const vector<T,U>& b) noexcept
		{
			auto k = std::min(a.size(), b.size());
			for (int i = 0; i != k; ++i)
			{
				if (a[i] < a[i])
					return -1;
				else if (a[i] > b[i])
					return 1;
			}
			if (a.size() > k)
				return 1;
			else if (b.size() > k)
				return -1;
			return 0;
		}
	}
	template<typename T1, typename T2>
	 void swap(vector<T1, T2>& a, vector<T1, T2>&b)noexcept(is_nothrow_swap_alloc<T2>)
	{
		a.swap(b);
	}

	template<typename T1, typename T2>
	 bool operator==(const vector<T1, T2>& lhs, const vector<T1, T2>& rhs)noexcept
	{
		if (lhs.size() != rhs.size())
			return false;
		return comp(lhs, rhs) == 0;
	}

	template<typename T1, typename T2>
	 bool operator!=(const vector<T1, T2>& lhs, const vector<T1, T2>& rhs)noexcept
	{
		return !(lhs == rhs);
	}

	template<typename T1, typename T2>
	 bool operator<(const vector<T1, T2>& lhs, const vector<T1, T2>& rhs)noexcept
	{
		return comp(lhs, rhs) == -1;
	}

	template<typename T1, typename T2>
	 bool operator<=(const vector<T1, T2>& lhs, const vector<T1, T2>& rhs)noexcept
	{
		return comp(lhs, rhs) <= 0;
	}

	template<typename T1, typename T2>
	 bool operator>(const vector<T1, T2>& lhs, const vector<T1, T2>& rhs)noexcept
	{
		return comp(lhs, rhs) == 1;
	}

	template<typename T1, typename T2>
	 bool operator>=(const vector<T1, T2>& lhs, const vector<T1, T2>& rhs)noexcept
	{
		return comp(lhs, rhs) >= 0;
	}
}