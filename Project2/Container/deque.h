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
		using self = deque_iterator<T>;
		using block = T[deque_node_size];
		using map_type = block*;

		self& operator+=(const int t)noexcept
		{
			const auto offert = t + (now - first());
			if (offert >= 0 && offert < deque_node_size<T>)
			{
				now += t;
			}
			else
			{
				auto offect_node = offert > 0 ? offert / deque_node_size<T> : -((-offert - 1) / deque_node_size<T>) - 1;
				this_block += offect_node;
				now = first() + (offert - offect_node*deque_node_size<T>);
			}
			return *this;
		}
		self& operator-=(const int t)noexcept
		{
			*this += -t;
			return *this;

		}
		self& operator++() noexcept
		{
			++now;
			if(now == last())
			{
				++this_block;
				now = first();
			}
			return *this;
		}
		self operator++(int)noexcept
		{
			auto p{ *this };
			++(*this);
			return p;
		}
		self& operator--()noexcept
		{
			if (now == first())
			{
				--this_block;
				now = last() - 1;
			}
			else
				--now;
			return *this;
		}
		self operator--(int) noexcept
		{
			auto p{ *this };
			--(*this);;
			return p;
		}

		T& operator*()noexcept
		{
			return *now;
		}
		const T& operator*()const noexcept
		{
			return *now;
		}
		T* operator->()noexcept
		{
			return now;
		}
		const T* operator->()const noexcept
		{
			return now;
		}

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.now == b.now;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return !(a == b);
		}
		friend bool operator<(const self& a, const self& b)noexcept
		{
			return a.now < b.now;
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
			return (a.this_block - b.this_block)*deque_node_size<T> +(a.now - a.last()) + (b.first() - b.now);
		}
		friend self operator-(const self& a,int b)noexcept
		{
			self c{ a };
			c -= b;	
			return c;
		}

	private:
		deque_iterator(const map_type* b,const T* a)
			:this_block(const_cast<map_type*>(b)),now(const_cast<T*>(a)){}
		
		T* first()const noexcept
		{
			return *this_block;
		}
		T* last()const noexcept
		{
			return first() + deque_node_size<T>;
		}

		map_type* this_block = nullptr;
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
		map_type* map = nullptr; //map指针
		size_t block_max_size = 0;    //map大小
		size_t block_size = 0;   // 有效block数
		size_t begin_block = 0;  //begin的block
		size_t end_block = 0;    //end 的block
		size_t ssize = 0;       
		T* b_begin = nullptr;  
		T* b_end = nullptr;
		Allocator alloc;
	public:

		T& at(size_t pos)
		{
			if (pos >= size())
				throw(out_of_range(""));
			return this->operator[](pos);
		}
		const T& at(size_t pos) const
		{
			if (pos >= size())
				throw(out_of_range(""));
			return this->operator[](pos);
		}

		T& operator[](size_t pos)
		{
			return *(begin() + pos);
		}
		const T& operator[](size_t pos) const
		{
			return *(begin() + pos);
		}

		T& front()noexcept
		{
			return *b_begin;
		}
		const T& front() const noexcept
		{
			return *b_begin;
		}

		T& back()
		{
			if (b_end == map[end_block])
				return *(static_cast<T*>(map[end_block - 1]) + deque_node_size<T>-1);
			return *(b_end - 1);
		}
		const T& back() const
		{
			if (b_end == map[end_block])
				return *(static_cast<T*>(map[end_block - 1]) + deque_node_size<T>-1);
			return *(b_end - 1);
		}

		iterator begin() noexcept
		{
			return iterator(map + begin_block, b_begin);
		}
		const iterator begin() const noexcept
		{
			return iterator(map + begin_block, b_begin);
		}
		const iterator cbegin() const noexcept
		{
			return iterator(map + begin_block, b_begin);
		}

		iterator end() noexcept
		{
			return iterator(map + end_block, b_end);
		}
		const iterator end() const noexcept
		{
			return iterator(map + end_block, b_end);
		}
		const iterator cend() const noexcept
		{
			return iterator(map + end_block, b_end);
		}

		void pop_back()
		{
			back().~T();
			if (b_end == map[end_block])
			{
				--end_block;
				b_end = static_cast<T*>(map[end_block]) + deque_node_size<T> -1;
			}
			else
				--b_end;
		}

		void pop_front()
		{
			front().~T();
			++b_begin;
			if(b_begin == static_cast<T*>(map[begin_block])+deque_node_size<T>)
			{
				++begin_block;
				b_begin = static_cast<T*>(map[begin_block]);
			}
		}

		bool empty() const noexcept
		{
			return ssize == 0;
		}

		size_t size() const noexcept
		{
			return ssize;
		}

		size_t max_size() const noexcept
		{
			return size();
		}

		void clear()
		{
			if (begin_block != end_block)
			{
				muiltdestory(b_begin, ((T*)map[begin_block]) + deque_node_size<T>);
				for (auto i = begin_block + 1; i < end_block; ++i)
					muiltdestory(map[i], ((T*)map[i]) + deque_node_size<T>);
				muiltdestory(map[end_block], b_end);
			}
			else
				muiltdestory(b_begin, b_end);
			for (auto j = 0; j != block_size; ++j)
				destoryblock(map[j]);
			alloc.deallocate(map,block_max_size);
			map = nullptr;
			block_max_size = 0;    
			block_size = 0;   
			begin_block = 0; 
			end_block = 0;   
			ssize = 0;
			b_begin = nullptr;
			b_end = nullptr;
		}




		~deque()
		{
			clear();
		}
	private:
		map_type makeblock()
		{
			block_Allocator allo(alloc);
			map_type k = nullptr;
			try
			{
				k = allo.allocate();
			}
			catch(bad_alloc&)
			{
				throw;
			}
			catch(...)
			{
				allo.deallocate(k);
				throw;
			}
			return k;
		}

		void destoryblock(map_type t)noexcept
		{
			block_Allocator allo(alloc);
			allo.deallocate(t);
		}

		static void muiltdestory(T* a,T*b)noexcept
		{
			for (auto i = a; i != b; ++i)
				(*i).~T();
		}

		void kuorong()
		{
			
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