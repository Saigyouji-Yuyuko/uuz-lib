#pragma once
#include"container.h"
namespace uuz
{
	template<typename T,typename A>
	class deque;

	template<typename T>
	constexpr static int deque_node_size = sizeof(T) <= 1 ? 4096 : sizeof(T) <= 2 ? 2048 : sizeof(T) <= 4 ? 1024 :
		sizeof(T) <= 8 ? 512 : sizeof(T) <= 16 ? 256 : sizeof(T) <= 32 ? 128 : sizeof(T) <= 64 ? 64 :
		sizeof(T) <= 128 ? 32 : sizeof(T) <= 256 ? 16 : sizeof(T) <= 512 ? 8 : 4;

	template<typename T>
	constexpr static int deque_node_size_log = sizeof(T) <= 1 ? 12 : sizeof(T) <= 2 ? 11 : sizeof(T) <= 4 ? 10 :
		sizeof(T) <= 8 ? 9 : sizeof(T) <= 16 ? 8 : sizeof(T) <= 32 ? 7 : sizeof(T) <= 64 ? 6 :
		sizeof(T) <= 128 ? 5 : sizeof(T) <= 256 ? 4 : sizeof(T) <= 512 ? 3 : 2;


	template<typename T>
	class deque_iterator
	{
	public:
		template<typename T1, typename A>
		friend class deque;
	private:
		deque_iterator(const T* now,const T* b,const T* block)
			:block(const_cast<T*>(block)),b(const_cast<T*>(b)),
				now(const_cast<T*>(now)){}

		T** block = nullptr;
		T* b = nullptr;
		T* now = nullptr;
	};

	template<typename T, typename A = uuz::allocator<T>>
	class deque
	{
	public:
		using block = T[deque_node_size];
		using block_Allocator = typename uuz::exchange<A, block>::type;
		using map_type = block*;
        using Allocator = typename uuz::exchange<A, map_type>::type;
		using iterator = deque_iterator<T>;
	private:
		map_type* table = nullptr; //map指针
		size_t block_size = 0;    //map大小
		size_t back_table = 0;   // 有效block数
		size_t end_block = 0;    //有数据的block
		size_t ssize = 0; 
		T* b_begin = nullptr;  
		T* b_end = nullptr;
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
			if(count != 0)
			{
				makemap(count);
				try
				{
					auto i = 0;
					while(count > deque_node_size<T>)
					{
						for (auto j = 0; j != deque_node_size<T>; ++j)
							table[i][j] = value;
						++i;
						count -= deque_node_size<T>;
					}
					if(count > 0)
						for (auto j = 0; j != count; ++j)
							table[i][j] = value;
					b_end = &table[i][count - 1];
					end_block = i;
					b_begin = &table[0][0];
					ssize = count;
				}
				catch (...)
				{
					clear();
					throw;
				}
			}
		}
		template< typename InputIt,typename = is_input<T,InputIt>>
		deque(InputIt first, InputIt last,const Allocator& alloc = Allocator()): deque(alloc)
		{
			if(last != first)
			{
				auto sssize = last - first;
				makemap(sssize);
				try
				{
					auto k = first;
					for (auto i = 0; i != block_size; ++i)
						for (auto j = 0; j != deque_node_size<T>; ++j)
						{
							table[i][j] = *k;
							++k;
							if (k == last)
							{
								end_block = i;
								b_end = &table[i][j];
								goto ll;
							}
						}
				ll: b_begin= &table[0][0];
					ssize = sssize;
				}
				catch (...)
				{
					clear();
					throw;
				}
			}
			
		}
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


		//待优化
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

		T& at(size_t pos)
		{
			if (pos >= size() )
				throw(out_of_range(""));
			return begin() + pos;
		}
		const T& at(size_t pos) const
		{
			if (pos >= size())
				throw(out_of_range(""));
			return begin() + pos;
		}

		T& operator[](size_t pos)noexcept
		{
			return begin() + pos;
		}
		const T& operator[](size_t pos) const noexcept
		{
			return begin() + pos;
		}

		T& front()
		{
			if (empty())
				throw(out_of_range(""));
			return *b_begin;
		}
		const T& front() const
		{
			if (empty())
				throw(out_of_range(""));
			return *b_begin;
		}

		T& back()
		{
			if (empty())
				throw(out_of_range(""));
			return *b_end;
		}
		const T& back() const
		{
			if (empty())
				throw(out_of_range(""));
			return *b_end;
		}

		iterator begin() noexcept
		{
			
		}
		const iterator begin() const noexcept
		{
			
		}
		const iterator cbegin() const noexcept
		{
			
		}

		iterator end() noexcept
		{
			
		}
		const iterator end() const noexcept;
		const iterator cend() const noexcept;

		bool empty() const noexcept
		{
			return b_begin == b_end;
		}

		size_t size() const noexcept
		{
			return ssize;
		}

		size_t capacity() const noexcept
		{
			return back_table * deque_node_size<T>;
		}

		size_t max_size() const noexcept;

		void shrink_to_fit();

		void clear() noexcept 
		{

		}

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

		void push_back(const T& value)
		{
			emplace_back(value);
		}
		void push_back(T&& value)
		{
			emplace_back(std::move(value));
		}

		template< class... Args >
		T& emplace_back(Args&&... args)
		{
			
		}

		void pop_back()
		{
			(*b_end).~T();
			if (b_end == b_begin)
			{
				end_block = 0;
				b_end = b_begin = &table[0][0];
			}
			else if (b_end == &table[end_block][0])
			{
				--end_block;
				b_end = &table[end_block][deque_node_size<T>-1];
			}
			else
				--b_end;
		}

		void push_front(const T& value)
		{
			emplace_front(value);
		}
		void push_front(T&& value) 
		{
			emplace_front(value);
		}

		template< class... Args >
		T& emplace_front(Args&&... args);

		void pop_front();

		void resize(size_t count);
		void resize(size_t count, const T& value);

		void swap(deque& other) noexcept//();
		{
			if(other.alloc == alloc)
			{
				
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
		map_type makeblock()
		{
			block_Allocator all(alloc);
			return all.allocate();
		}

		void destroyblock(map_type t)
		{
			block_Allocator all(alloc);
			all.deallocate(t);
		}

		void makemap(size_t cap)
		{
			auto k = cap >> deque_node_size_log<T> +(cap&-cap == cap ? 0 : 1);
			auto kk = trans(k);
			map_type* temp;
			try
			{
				temp = alloc.allocate(kk);
				memset(temp, 0, kk);
				if (table)
					memcpy(temp, table, back_table );
			}
			catch (bad_alloc&) { throw; }
			catch (...) { alloc.deallocate(temp);throw; }
			auto i = back_table;
			try
			{
				for (; i != k; ++i)
					temp[i] = makeblock();
			}
			catch(...)
			{
				for (auto j = back_table; j != i; ++j)
					destroyblock(temp[i]);
				alloc.deallocate(temp);
				throw;
			}
			back_table = k;
			block_size = kk;
			swap(temp, table);
			alloc.deallocate(temp);
		}
		//todo
		static size_t trans(size_t cap)noexcept
		{
			return cap;
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