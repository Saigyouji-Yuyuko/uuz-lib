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
			return a.this_block == b.this_block ? a.this_block < b.this_block : a.now < b.now;
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
		deque() : deque(Allocator()) {}
		explicit deque(const Allocator& alloc):alloc(alloc){}
		deque(size_t count,const T& value,const Allocator& alloc = Allocator()):deque(alloc)
		{
			auto i = 0;
			try
			{
				kuorong(count, true);
				auto k = begin();
				for(;i!=count;++i,(void)++k)
					new(&*k) T(value);
				auto t = begin() + count;
				b_end = t.now;
				block_end = t.this_block - map;
				ssize = count;
			}
			catch(...)
			{
				auto kk = begin();
				for (auto j = 0; j != i; ++j, (void)++kk)
					kk->~T();
				b_end = b_begin;
				end_block = begin_block;
				clear();
				throw;
			}
		}
		explicit deque(size_t count, const Allocator& alloc = Allocator()):deque(count,T{},alloc){}
		template< class InputIt ,typename = is_input<T,InputIt>>
		deque(InputIt first, InputIt last,const Allocator& alloc = Allocator()):deque(alloc)
		{
			auto i = first;
			auto count = last - first;
			try
			{
				kuorong(count, true);
				auto k = begin();
				for (; i != last; ++i, (void)++k)
					new(&*k) T(*i);
				auto t = begin() + count;
				b_end = t.now;
				block_end = t.this_block - map;
				ssize = count;
			}
			catch (...)
			{
				auto kk = begin();
				for (auto j = first; j != i; ++j, (void)++kk)
					kk->~T();
				b_end = b_begin;
				end_block = begin_block;
				clear();
				throw;
			}
		}
		deque(const deque& other):deque(other.begin(),other.end()){}
		deque(const deque& other, const Allocator& alloc):deque(other.begin(),other.end(),alloc){}
		deque(deque&& other):deque()
		{
			this->swap(other);
		}
		deque(deque&& other, const Allocator& alloc):deque()
		{
			this->swap(other);
		}
		deque(std::initializer_list<T> init,const Allocator& alloc = Allocator()):deque(init.begin(),inti.end(),alloc){}
		
		deque& operator=(const deque& other)
		{
			if(this != &other)
			{
				deque temp(other,alloc);
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
		
		void assign(size_t count, const T& value)
		{
			deque temp(count, value, alloc);
			this->swap(temp);
			return *this;
		}
		template< class InputIt ,typename = is_input<T,InputIt>>
		void assign(InputIt first, InputIt last)
		{
			deque temp(first, last, alloc);
			this->swap(temp);
			return *this;
		}
		void assign(std::initializer_list<T> ilist)
		{
			deque temp(ilist, alloc);
			this->swap(temp);
			return *this;
		}

		
		
		Allocator get_allocator()
		{
			return alloc;
		}

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

		void shrink_to_fit()
		{
			iterator k{ map[0],block_first(map[0]) };
			auto count = begin() - k;
			if(count >= ssize)
			{
				move_or_copy_con(begin(), ssize, k);
				for (auto i = begin(); i != end(); ++i)
					(*i).~T();
			}
			else
			{
				move_or_copy_con(begin(), count, k);
				move_or_copy_ass_for(begin() + count, ssize - count, begin());
				for (auto i = begin()+count; i != end(); ++i)
					(*i).~T();
			}
			b_begin = k.now;
			begin_block = k.this_block;
			k += ssize;
			b_end = k.now;
			end_block = k.this_block;
			for (auto i = end_block; i != block_size; ++i)
				destoryblock(map[i]);
			block_size = end_block;
		}

		iterator erase(const iterator& pos)
		{
			return erase(pos, pos + 1);
		}
		iterator erase(const iterator& first, const iterator& last)
		{
			auto k = last - first;
			move_or_copy_ass_for(last, end() - last, first);
			auto d = end() - k;
			auto kk = end();
			end_block = d.this_block - map;
			b_end = d.now;
			for (auto i = end(); i != kk; ++i)
				(*i).~T();
			ssize -= k;
			return first;
		}

		iterator insert(const iterator& pos, const T& value)
		{
			return emplace(pos, value);
		}
		iterator insert(const iterator& pos, T&& value)
		{
			return emplace(pos, std::move(value));
		}
		iterator insert(const iterator& pos, size_t count, const T& value)
		{
			auto p = pos - begin();
			if (p < size() >> 1)
			{
				kuorong(count, true);
				auto k = begin();
				k -= count;
				if (count <= p)
				{
					move_or_copy_con(begin(), count , k);
					move_or_copy_ass_for(begin() + p - count, p - count, begin());
				}
				else
					move_or_copy_con(k, p, begin());
				b_begin = k.now;
				begin_block = k.this_block - map;
				auto kk = 0;
				for (auto i = begin() + p; kk != p; ++i, (void)++kk)
					new(&*(i)) T(value);
			}
			else
			{
				kuorong(count, false);
				auto k = end();
				k += count;
				if (count <= ssize - p)
				{
					move_or_copy_con(end() - count, count, end());
					move_or_copy_ass_back(begin() + p - count, p - count, end() - count);
				}
				else
					move_or_copy_con(begin() + p, ssize - p, k - ssize + p);
				b_end = k.now;
				end_block = k.this_block - map;
				auto kk = 0;
				for (auto i = begin() + p; kk!= p; ++i,(void)++kk)
					new(&*(i)) T(value);
			}
			ssize += count;
			return begin() + p;
		}
		template< class InputIt ,typename = is_input<T,InputIt>>
		iterator insert(const iterator& pos, InputIt first, InputIt last)
		{
			auto count = last - first;
			auto p = pos - begin();
			if (p < size() >> 1)
			{
				kuorong(count, true);
				auto k = begin();
				k -= count;
				if (count <= p)
				{
					move_or_copy_con(begin(), count, k);
					move_or_copy_ass_for(begin() + p - count, p - count, begin());
				}
				else
					move_or_copy_con(k, p, begin());
				b_begin = k.now;
				begin_block = k.this_block - map;
				auto j = first;
				for (auto i = begin() + p; j!=last; ++i, (void)++j)
					new(&*(i)) T(*j);
			}
			else
			{
				kuorong(count, false);
				auto k = end();
				k += count;
				if (count <= ssize - p)
				{
					move_or_copy_con(end() - count, count, end());
					move_or_copy_ass_back(begin() + p - count, p - count, end() - count);
				}
				else
					move_or_copy_con(begin() + p, ssize - p, k - ssize + p);
				b_end = k.now;
				end_block = k.this_block - map;
				auto j = first;
				for (auto i = begin() + p; j != last; ++i, (void)++j)
					new(&*(i)) T(*j);
			}
			ssize += count;
			return begin() + p;
		}
		iterator insert(const iterator& pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		template< class... Args >
		iterator emplace(const iterator& pos, Args&&... args)
		{
			if(pos ==begin())
			{
				emplace_front(std::forward<Args>(args)...);
				return begin();
			}
			else if(pos ==end())
			{
				emplace_back(std::forward<Args>(args)...);
				return end();
			}
			auto p = pos - begin();
			if(p < size()>>1)
			{
				kuorong(1, true);
				auto k = begin();
				--k;
				move_or_copy_con(b_begin, 1, k.now);
				move_or_copy_ass_for(begin() + 1, p, begin());
				b_begin = k.now;
				begin_block = k.this_block - map;
				new(&*(begin() + p + 1)) T(std::forward<Args>(args)...);
			}
			else
			{
				kuorong(1, false);
				auto k = end();
				++k;
				move_or_copy_con(b_end, 1, k.now);
				move_or_copy_ass_back(begin() + p, ssize - p, begin() + p + 1);
				b_end = k.now;
				end_block = k.this_block - map;
				new(&*(begin() + p + 1)) T(std::forward<Args>(args)...);
			}
			++ssize;
			return begin() + p;
		}

		void push_back(const T& value)
		{
			emplace_back(value);
		}
		void push_back(T&& value)
		{
			emplace_back(std::move(value));
		}

		template< typename... Args >
		T& emplace_back(Args&&... args)
		{
			kuorong(1, true);
			if (b_begin == static_cast<T*>(map[begin_block]))
			{
				--begin_block;
				b_begin = static_cast<T*>(map[begin_block]) + deque_node_size<T> - 1;
			}
			new(b_begin) T(std::forward<Args>(args)...);
			++ssize;
			return *b_begin;
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
			--ssize;
		}

		void push_front(const T& value)
		{
			 emplace_front(value);
		}
		void push_front(T&& value)
		{
			 emplace_front(std::move(value));
		}
		
		template< typename... Args >
		T& emplace_front(Args&&... args)
		{
			kuorong(1, false);
			new(b_end) T(std::forward<Args>(args)...);
			++b_end;
			if (b_end == static_cast<T*>(map[end_block])+deque_node_size<T>)
			{
				++end_block;
				b_end = static_cast<T*>(map[end_block]) ;
			}	
			++ssize;
			return *b_end;
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
			--ssize;
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

		size_t capacity() const noexcept
		{
			return block_size * deque_node_size<T>;
		}


		void resize(size_t count)
		{
			return resize(count, T{});
		}
		void resize(size_t count, const T& value)
		{
			if(ssize >= count)
				erase(begin() + count, end());
			else
			{
				kuorong(count - ssize, false);
				auto k = end();
				for (auto i = 0; i != count - ssize; ++i, (void)++k)
					new(&*k) T(value);
				k = end() + count - ssize;
				b_end = k.now;
				end_block = k.this_block - map;
			}
			ssize = count;
		}

		void clean()noexcept
		{
			if (begin_block != end_block)
			{
				muiltdestory(b_begin, block_end(map[begin_block]));
				for (auto i = begin_block + 1; i < end_block; ++i)
					muiltdestory(block_first(map[i]), block_end(map[i]));
				muiltdestory(block_first(map[end_block]), b_end);
			}
			else
				muiltdestory(b_begin, b_end);
			ssize = 0;
			begin_block = 0;
			end_block = 0;
			b_begin = map[0];
			b_end = map[0];
		}

		void clear()
		{
			clean();
			for (auto j = 0; j != block_size; ++j)
				destoryblock(map[j]);
			alloc.deallocate(map,block_max_size);
			map = nullptr;
			block_max_size = 0;    
			block_size = 0;   
			begin_block = 0; 
			end_block = 0;
			b_begin = nullptr;
			b_end = nullptr;
		}

		void swap(deque& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if(other.alloc == alloc)
			{
				char temp[sizeof(deque<T>)];
				memcpy(temp, this, sizeof(deque<T>));
				memcpy(this, &other, sizeof(deque<T>));
				memccpy(&other, temp, sizeof(deque<T>));
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's not defined that allocator is not equal!");
#else // DEBUG
				deque t1(other, alloc);
				deque t2(*this, other.alloc);
				this->swap(t1);
				this->swap(t2);
#endif
			}
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
			allo.deallocate(t, 1);
		}

		static void muiltdestory(T* a,T*b)noexcept
		{
			for (auto i = a; i != b; ++i)
				(*i).~T();
		}

		static T* block_first(map_type a)noexcept
		{
			return static_cast<T*>(a);
		}

		static T* block_end(map_type a)noexcept
		{
			return static_cast<T*>(a) + deque_node_size<T>;
		}

		void kuorong(size_t s,bool front)
		{
			if(front && b_begin - block_first(map[begin_block]) < s)
			{
				auto  need_buffer = ((s - (b_begin - block_first(map[begin_block])))>>deque_node_size_log<T>) + 1;
				if(begin_block < need_buffer)
				{
					auto k = need_buffer - begin_block;
					if(block_size > end_block + k )//后面有空余块时
					{
						std::rotate(map + begin_block, map + end_block + 1,map + end_block + k);
						begin_block += k;
						end_block += k;
					}
					else if(block_size + k < block_max_size)//后面空余块不够，但是map还够的时候
					{
						memmove(map + begin_block + k, map + begin_block, block_size - begin_block);
						auto i = begin_block;
						try
						{
							for (; i != begin_block + k; ++i)
								map[i] = makeblock();
						}
						catch(...)
						{
							for (auto j = begin_block; j != i; ++j)
								destoryblock(map[j]);
							memcpy(map + begin_block, map + begin_block + k, block_size - begin_block);
							throw;
						}
						begin_block += k;
						end_block += k;
						block_size += k;
					}
					else//都不够
					{
						map_type* kk = nullptr;
						auto i = 0;
						try
						{
							kk = alloc.allocate(trans(block_max_size + k));
							for (; i != k; ++i)
								kk[i] = makeblock();
						}
						catch(...)
						{
							if(kk)
							{
								for (auto j = 0; j != i; ++j)
									destoryblock(kk[i]);
								alloc.deallocate(kk);
							}
							throw;
						}
						if(map)
							memcpy(kk + k, map, block_size);
						::swap(kk, map);
						alloc.deallocate(kk);
						block_max_size = trans(block_max_size + k);
						block_size += k;
						begin_block += k;
						end_block += k;
					}
				}
			}
			else if(!front && block_end(map[end_block]) - b_end < s)
			{
				auto  need_buffer = ((s - (block_end(map[end_block]) - b_end)) >> deque_node_size_log<T>) +1;
				if(block_size - end_block - 1 < need_buffer)
				{
					auto k = need_buffer - block_size + end_block + 1;
					if(begin_block >= k)
					{
						std::rotate(map, map + k, map + block_size);
						begin_block -= k;
						end_block -= k;
					}
					else if(block_size + k < block_max_size)
					{
						auto i = 0;
						try
						{
							for (; i != k; ++i)
								map[block_size + i] = makeblock();
						}
						catch(...)
						{
							for (auto j = 0; j != i; ++j)
								destoryblock(map[block_size + j]);
							throw;
						}
						block_size += k;
					}
					else
					{
						map_type* kk = nullptr;
						auto i = 0;
						try
						{
							kk = alloc.allocate(trans(block_max_size + k));
							for (; i != k; ++i)
								kk[block_size + i] = makeblock();
						}
						catch (...)
						{
							if (kk)
							{
								for (auto j = 0; j != i; ++j)
									destoryblock(kk[block_size + i]);
								alloc.deallocate(kk);
							}
							throw;
						}
						if (map)
							memcpy(kk, map, block_size);
						::swap(kk, map);
						alloc.deallocate(kk);
						block_max_size = trans(block_max_size + k);
						block_size += k;
					}
				}
			}
		}
	};
	namespace
	{
		template< class T, class Alloc >
		int compare(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)noexcept
		{
			auto i = lhs.begin();
			auto j = rhs.begin();
			for (; i != lhs.end() && j != rhs.end();++i,(void)++j)
			{
				if (*i < *j)
					return -1;
				else if (*j > *i)
					return 1;
			}
			if (lhs.size() > rhs.size())
				return 1;
			else if (rhs.size() > lhs.size())
				return -1;
			return 0;
		}
	}
	template< class T, class Alloc >
	bool operator==(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs)noexcept
	{
		return lhs.size() == rhs.size() && compare(lhs, rhs) == 0;
	}
	template< class T, class Alloc >
	bool operator!=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs)noexcept
	{
		return !(lhs == rhs);
	}
	template< class T, class Alloc >
	bool operator<(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) < 0;
	}
	template< class T, class Alloc >
	bool operator<=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) <= 0;
	}
	template< class T, class Alloc >
	bool operator>(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) > 0;
	}
	template< class T, class Alloc >
	bool operator>=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) >= 0;
	}

	template< class T, class Alloc >
	void swap(deque<T, Alloc>& lhs,deque<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

}