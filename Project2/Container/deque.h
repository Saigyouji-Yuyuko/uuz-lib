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
		using block = T[deque_node_size<T>];
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
		deque_iterator() = default;

		T* first()const noexcept
		{
			return reinterpret_cast<T*>(*this_block);
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
		using block = T[deque_node_size<T>];
		using block_Allocator = typename uuz::exchange<A, block>::type;
		using map_type = block*;
        using Allocator = typename uuz::exchange<A, map_type>::type;
		using iterator = deque_iterator<T>;
	private:
		map_type* map = nullptr; //map指针
		size_t block_max_size = 0;    //map大小
		size_t block_size = 0;   // 有效block数
		iterator _begin;
		iterator _end;
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
				auto k = iterator(&map[0], block_first(map[0]));
				_begin = k;
				for(;i!=count;++i,(void)++k)
					new(&*k) T(value);
				_end = _begin + count;
			}
			catch(...)
			{
				auto kk = begin();
				for (auto j = 0; j != i; ++j, (void)++kk)
					(*kk).~T();
				_end = _begin;
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
				auto k = iterator(&map[0], block_first(map[0]));
				_begin = k;
				for (; i != last; ++i, (void)++k)
					new(&*k) T(*i);
				_end = _begin + count;
				ssize = count;
			}
			catch (...)
			{
				auto kk = begin();
				for (auto j = first; j != i; ++j, (void)++kk)
					kk->~T(); 
				_end = _begin;
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
			return *_begin;
		}
		const T& front() const noexcept
		{
			return *_begin;
		}

		T& back()
		{
			return *_end;
		}
		const T& back() const
		{
			return *_end;
		}

		iterator begin() noexcept
		{
			return _begin;
		}
		const iterator begin() const noexcept
		{
			return _begin;
		}
		const iterator cbegin() const noexcept
		{
			return _begin;
		}

		iterator end() noexcept
		{
			return _end;
		}
		const iterator end() const noexcept
		{
			return _end;
		}
		const iterator cend() const noexcept
		{
			return _end;
		}

		void shrink_to_fit()
		{
			iterator k{ map[0],block_first(map[0]) };
			auto count = begin() - k;
			if(count >= size())
			{
				move_or_copy_con(begin(), size(), k);
				for (auto i = begin(); i != end(); ++i)
					(*i).~T();
			}
			else
			{
				move_or_copy_con(begin(), count, k);
				move_or_copy_ass_for(begin() + count, size() - count, begin());
				for (auto i = begin()+count; i != end(); ++i)
					(*i).~T();
			}
			auto temp = size();
			_begin = k;
			_end = _begin + temp;
			for (auto i = _end.this_block - map + 1; i != block_size; ++i)
				destoryblock(map[i]);
			block_size = _end.this_block - map;
		}

		iterator erase(const iterator& pos)
		{
			return erase(pos, pos + 1);
		}
		iterator erase(const iterator& first, const iterator& last)
		{
			auto k = last - first;
			move_or_copy_ass_for(last, end() - last, first);
			auto kk = end();
			_end -= k;
			for (auto i = end(); i != kk; ++i)
				(*i).~T();
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
				_begin = k;
				auto kk = 0;
				for (auto i = begin() + p; kk != p; ++i, (void)++kk)
					new(&*(i)) T(value);
			}
			else
			{
				kuorong(count, false);
				auto k = end();
				k += count;
				if (count <= size() - p)
				{
					move_or_copy_con(end() - count, count, end());
					move_or_copy_ass_back(begin() + p - count, p - count, end() - count);
				}
				else
					move_or_copy_con(begin() + p, size() - p, k - size() + p);
				_end = k;
				auto kk = 0;
				for (auto i = begin() + p; kk!= p; ++i,(void)++kk)
					new(&*(i)) T(value);
			}
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
				_begin = k;
				auto j = first;
				for (auto i = begin() + p; j!=last; ++i, (void)++j)
					new(&*(i)) T(*j);
			}
			else
			{
				kuorong(count, false);
				auto k = end();
				k += count;
				if (count <= size() - p)
				{
					move_or_copy_con(end() - count, count, end());
					move_or_copy_ass_back(begin() + p - count, p - count, end() - count);
				}
				else
					move_or_copy_con(begin() + p, size() - p, k - size() + p);
				_end = k;
				auto j = first;
				for (auto i = begin() + p; j != last; ++i, (void)++j)
					new(&*(i)) T(*j);
			}
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
				move_or_copy_con(begin(), 1, k);
				move_or_copy_ass_for(begin() + 1, p, begin());
				_begin = k;
				new(&*(begin() + p + 1)) T(std::forward<Args>(args)...);
			}
			else
			{
				kuorong(1, false);
				auto k = end();
				++k;
				move_or_copy_con(end(), 1, k);
				move_or_copy_ass_back(begin() + p, size() - p, begin() + p + 1);
				_end = k;
				new(&*(begin() + p + 1)) T(std::forward<Args>(args)...);
			}
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
			kuorong(1, false);
			++_end;
			new(_end.now) T(std::forward<Args>(args)...);
			return *_end;
		}

		void pop_back()
		{
			back().~T();
			--_end;
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
			kuorong(1, true);
			--_begin;
			new(_bgein.now) T(std::forward<Args>(args)...);
			return *_begin;
		}

		void pop_front()
		{
			front().~T();
			++_begin;
		}

		bool empty() const noexcept
		{
			return _begin == _end;
		}

		size_t size() const noexcept
		{
			return  _end - _begin;
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
			if(size() >= count)
				erase(begin() + count, end());
			else
			{
				kuorong(count - size(), false);
				auto k = end();
				for (auto i = 0; i != count - size(); ++i, (void)++k)
					new(&*k) T(value);
				_end = _end + count - size();
			}
		}

		void clean()noexcept
		{
			for (auto i = _begin; i != _end; ++i)
				(*i).~T();
			if(map && map[0])
				_begin = _end = iterator( &map[0],block_first(map[0]) );
			else
				_begin = _end = iterator{};
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
			_begin = _end = iterator{};
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
				allo.deallocate(k,1);
				throw;
			}
			return k;
		}

		void destoryblock(map_type t)noexcept
		{
			block_Allocator allo(alloc);
			allo.deallocate(t, 1);
		}
			
		static inline T* block_first(map_type a)noexcept
		{
			return reinterpret_cast<T*>(a);
		}

		static inline T* block_end(map_type a)noexcept
		{
			return reinterpret_cast<T*>(a) + deque_node_size<T>;
		}

		void construct(size_t t)
		{
			auto ll = trans(t);
			try
			{
				map = alloc.allocate(ll);
			}
			catch (bad_alloc&) { throw; }
			catch(...) { alloc.deallocate(map, ll); throw; }
			auto i = 0;
			try
			{
				for (; i != t; ++i)
					map[i] = makeblock();
			}
			catch(...)
			{
				for (auto j = 0; j != i; ++j)
					destoryblock(map[i]);
				alloc.deallocate(map, ll);
				map = nullptr;
				throw;
			}
			block_max_size = ll;
			block_size = t;
			_begin = _end = iterator(&map[0], block_first(map[0]));
		}

		void kuorong(size_t s,bool front)
		{
			if (!map)
				return construct(s);
			if(front)
			{
				iterator head( &map[0],block_first(map[0]) );
				if(head - _begin < s)
				{
					auto k = ((s - (head - _begin)) >> deque_node_size_log<T>) + 1;
					if(block_size - (_end.this_block - map)-1 >=k)
					{
						std::rotate(_begin.this_block, _end.this_block + 1, _end.this_block + k + 1);
						_begin.this_block += k;
						_end.this_block += k;
					}
					else if(block_max_size <= block_size + k)
					{
						auto ll = trans(block_size + k);
						map_type* kk = nullptr;
						try
						{
							kk = alloc.allocate(ll);
						}
						catch (bad_alloc&) { throw; }
						catch (...) { alloc.deallocate(kk, ll); throw; }
						memset(kk, 0, ll);
						memmove(map, kk + k, block_size);
						auto i = 0;
						try
						{
							for (; i != k; ++i)
								kk[i] = makeblock();
						}
						catch(...)
						{
							for (auto j = 0; j != i; ++j)
								destoryblock(kk[i]);
							alloc.deallocate(kk, ll);
							throw;
						}
						_begin.this_block = kk + k + (_begin.this_block - map);
						_end .this_block = kk + k + (_end.this_block - map);
						block_max_size = ll;
						block_size += k;
					}
					else
					{
						memmove(map, map + k, block_size);
						auto i = 0;
						try
						{
							for (; i != k; ++i)
								map[i] = makeblock();
						}
						catch (...)
						{
							for (auto j = 0; j != i; ++j)
								destoryblock(map[i]);
							memcpy(map+k, map , block_size);
							//赋值后面的为nullptr

							throw;
						}
						_begin.this_block += k;
						_end.this_block += k;
						block_size += k;
					}
				}
			}
			else
			{
				iterator nil(&map[block_size - 1], block_end(map[block_size - 1]));
				if(_end - nil <s)
				{
					auto k = ((s - (_end - nil)) >> deque_node_size_log<T>) + 1;
					if ((_begin.this_block - map) >= k)
					{
						std::rotate(_begin.this_block - k, _begin.this_block, _end.this_block + 1);
						_begin.this_block -= k;
						_end.this_block -= k;
					}
					else if (block_max_size <= block_size + k)
					{
						auto ll = trans(block_size + k);
						map_type* kk = nullptr;
						try
						{
							kk = alloc.allocate(ll);
						}
						catch (bad_alloc&) { throw; }
						catch (...) { alloc.deallocate(kk, ll); throw; }
						memset(kk, 0, ll);
						memmove(map, kk, block_size);
						auto i = block_size;
						try
						{
							for (; i != block_size + k; ++i)
								kk[i] = makeblock();
						}
						catch (...)
						{
							for (auto j = block_size; j != i; ++j)
								destoryblock(kk[i]);
							alloc.deallocate(kk, ll);
							throw;
						}
						_begin.this_block = kk  + (_begin.this_block - map);
						_end.this_block = kk  + (_end.this_block - map);
						block_max_size = ll;
						block_size += k;
					}
					else
					{
						auto i = block_size;
						try
						{
							for (; i != block_size + k; ++i)
								map[i] = makeblock();
						}
						catch (...)
						{
							for (auto j = 0; j != i; ++j)
							{
								destoryblock(map[i]);
								map[i] = nullptr;
							}
							throw;
						}
						block_size += k;
					}
				}
			}
		}

		static size_t trans(size_t t)noexcept
		{
			if (!t)
				return 0;
#ifdef _MSC_VER
			DWORD index;
			auto k = _BitScanReverse(&index, t - 1);
			if(k)
				return 1 << (index + 1);
			//throw("");
#elif __GNUC__
			auto k = __builtin_clz(t - 1);
			return 1 << (k + 1);
#else
			size_t k = 1;
			while (k < t)
				k << 1;
			return k;
#endif
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