#pragma once
#include"container.h"
namespace uuz
{
	template<typename T,typename A>
	class deque;

	template<typename T>
	constexpr static int deque_node_size = 4096 / sizeof(T) > 8 ? 4096 / sizeof(T) : 8;


	template<typename T>
	struct deque_node
	{
		T(*data) [deque_node_size<T>];
	};

	template<typename T>
	class deque_iterator
	{
	public:
		template<typename T1, typename A>
		friend class deque;
	private:
		deque_iterator(const T* now,const deque_node<T> b,const deque_node<T>* block)
			:block(const_cast<deque_node<T>*>(block)),b(const_cast<deque_node<T>>(b)),
				now(const_cast<T*>(now)){}

		deque_node<T>* block = nullptr;
		deque_node<T> b = nullptr;
		T* now = nullptr;
	};

	template<typename T, typename A = uuz::allocator<T>>
	class deque
	{
	public:
        using Allocator = typename uuz::exchange<A, deque_node<T>>::type;
		using iterator = deque_iterator<T>;
		using node = deque_node<T>;
	private:
		node* p = nullptr;
		size_t ssize = 0;
		size_t block_size = 0;
		T* b = nullptr;
		T* e = nullptr;
		Allocator alloc;
	public:
		deque()noexcept(std::is_nothrow_copy_constructible_v<Allocator> 
					&& std::is_nothrow_default_constructible_v<Allocator>) 
			: deque(Allocator()) {}
		explicit deque(const A& alloc)noexcept(std::is_nothrow_constructible_v<Allocator,A&>)
			:alloc(alloc){}
		explicit deque(size_t count, const T& value = T{}, const Allocator& alloc = Allocator())
			:deque(alloc)
		{
			try
			{
				
			}
			catch(...)
			{
				clear();
				throw;
			}
		}
		template< typename InputIt,typename = is_input<T,InputIt>>
		deque(InputIt first, InputIt last,const Allocator& alloc = Allocator());
		deque(const deque& other):deque(other.begin(),other.end(),A()){}
		deque(const deque& other, const Allocator& alloc) :deque(other.begin(), other.end(), alloc) {}
		deque(deque&& other)noexcept:deque(other.alloc)
		{
			this->swap(other);
		}
		deque(deque&& other, const Allocator& alloc):deque(alloc)
		{
			this->swap(other);
		}
		deque(std::initializer_list<T> init,const Allocator& alloc = Allocator())
			:deque(init.begin(),init.end(),alloc){}

		deque& operator=(const deque& other)
		{
			if(this!=&other)
			{
				deque temp(other, alloc);
				this->swap(temp);
			}
			return *this;
		}
		deque& operator=(deque&& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if (this != &other)
			{
				deque temp(std::move(other), alloc);
				this->swap(temp);
			}
			return *this;
		}
		deque& operator=(std::initializer_list<T> ilist)
		{
			deque temp(ilist, alloc);
			this->swap(temp);
			return *this;
		}


		//´ýÓÅ»¯
		void assign(size_t count, const T& value)
		{
			deque temp(count, value, alloc);
			this->swap(temp);
		}
		template< typename InputIt, typename = is_input<T, InputIt>>
		void assign(InputIt first, InputIt last)
		{
			deque temp(first, last, alloc);
			this->swap(temp);
		}
		void assign(std::initializer_list<T> ilist)
		{
			deque temp(ilist, alloc);
			this->swap(temp);
		}

		Allocator get_allocator() const
		{
			return alloc;
		}

		T& at(size_t pos);
		const T& at(size_t pos) const;

		T& operator[](size_t pos);
		const T& operator[](size_t pos) const;

		T& front()
		{
			if (b)
				throw(out_of_range(""));
			return *b;
		}
		const T& front() const
		{
			if (b)
				throw(out_of_range(""));
			return *b;	
		}

		T& back()
		{
			if (e)
				throw(out_of_range(""));
			return *e;
		}
		const T& back() const
		{
			if (e)
				throw(out_of_range(""));
			return *e;
		}

		iterator begin() noexcept
		{
			return iterator(b, p[0], p);
		}
		const iterator begin() const noexcept
		{
			return iterator(b, p[0], p);
		}
		const iterator cbegin() const noexcept
		{
			return iterator(b, p[0], p);
		}

		iterator end() noexcept
		{
			return iterator(e, p[], p);
		}
		const iterator end() const noexcept;
		const iterator cend() const noexcept;

		bool empty() const noexcept
		{
			return ssize == 0;
		}

		size_t size() const noexcept
		{
			return ssize;
		}

		size_t max_size() const noexcept;

		void shrink_to_fit();

		void clear() noexcept;

		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);
		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last);
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		void push_back(const T& value);
		void push_back(T&& value);

		template< class... Args >
		T& emplace_back(Args&&... args);

		void pop_back();

		void push_front(const T& value);
		void push_front(T&& value);

		template< class... Args >
		T& emplace_front(Args&&... args);

		void pop_front();

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(deque& other) noexcept//();
		{
			if(other.alloc == alloc)
			{
				using std::swap;
				swap(p, other.p);
				swap(ssize, other.ssize);
				swap(block_size, other.block_size);
				swap(b, other.b);
				swap(e, other.e);
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's not defined that allocator is not eaqul");
#else
				deque temp1(std::move(other), alloc);
				deque temp2(std::move(*this), other.alloc);
				this->swap(temp1);
				other.swap(temp2);
#endif
			}
		}

		~deque()noexcept
		{
			clear();
		}
	private:
		node makeblock()
		{
			//static_cast<decltype(n)>(1) << (sizeof(n) * 8 - __builtin_clz(n | 1))
			_BitScanReverse
			return alloc.allocator();
		}
	};
	template< class T, class Alloc >
	bool operator==(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator!=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator<(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator<=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator>(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator>=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);

	template< class T, class Alloc >
	void swap(deque<T, Alloc>& lhs,deque<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

}