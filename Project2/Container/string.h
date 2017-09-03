#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"container.h"
#include<cstring>
#include<cassert>
#ifdef DEBUG
#include"debug.h"
#endif
#pragma warning(disable:4996)
namespace uuz
{
	template<typename CharT> 
	struct char_traits;

	template<>
	struct char_traits<char>
	{
		using char_type = char;
		using int_type = int;
		using off_type = std::streamoff;
		using pos_type = std::streampos;
		using state_type = std::mbstate_t;


		static constexpr void assign(char_type& r, const char_type& a)noexcept
		{
			r = a;
		}
		static char_type* assign(char_type* p, std::size_t count, char_type a)noexcept
		{
			::memset(p, a, count);
			return p;
		}

		static constexpr bool eq(char_type a, char_type b)noexcept
		{
			return a == b;
		}
		static constexpr bool lt(char_type a, char_type b)noexcept
		{
			return a < b;
		}

		static char_type* move(char_type* dest, const char_type* src, std::size_t count)noexcept
		{
			return static_cast<char_type*>(::memmove(dest, src, count));
		}

		static char_type* copy(char_type* dest, const char_type* src, std::size_t count)noexcept
		{
			return move(dest, src, count);
		}

		static constexpr int compare(const char_type* s1, const char_type* s2, std::size_t count)noexcept
		{
			return ::strncmp(s1, s2, count);
		}

		static constexpr size_t length(const char_type* s)noexcept
		{
			return ::strlen(s);
		}

		static constexpr const char_type* find(const char_type* p, std::size_t count, const char_type& ch)
		{
			for (auto i = 0; i != count; ++i)
				if (eq(*(p + i), ch))
					return p + i;
			return nullptr;
		}

		static constexpr char_type to_char_type(int_type c) noexcept
		{
			return char_type(c);
		}

		static constexpr int_type to_int_type(char_type c)
		{
			return to_int_type(c);
		}

		static constexpr bool eq_int_type(int_type c1, int_type c2)
		{
			return c1 == c2;
		}

		static constexpr int_type eof()
		{
			return 0;
		}

		static constexpr int_type not_eof(int_type e) noexcept
		{
			return eq_int_type(e, eof()) ? 1 : e;
		}
	};

	template<typename CharT, typename Traits>
	class basic_string_view;

	template<typename CharT, typename Traits, typename Allocator>
	class basic_string;

	template<typename CharT, typename Traits = char_traits<CharT>>
	class basic_string_iterator
	{
	public:
		using difference_type =typename  Traits::difference_type;
		using value_type = CharT;
		using pointer = CharT*;
		using reference = CharT&;
		using iterator_category = uuz::random_access_iterator_tag;

		template<typename CarT, typename Trits, typename Allocator>
		friend class basic_string;

		template<typename CarT, typename Taits>
		friend class basic_string_view;
	private:
		using self = basic_string_iterator;

		explicit basic_string_iterator(CharT* t) :dat{ t } {}
		explicit basic_string_iterator(const CharT* t) :dat{ const_cast<CharT*>(t) } {}
		pointer dat;
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

		reference operator*()noexcept
		{
			return *dat;
		}
		const reference operator*()const noexcept
		{
			return *dat;
		}
		pointer operator->()noexcept
		{
			return dat;
		}
		const pointer operator->()const noexcept
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
		friend self operator-(const self& a, const difference_type b)noexcept
		{
			self c{ a };
			c -= b;
			return c;
		}

	};

	template<typename CharT,typename Traits = std::char_traits<CharT>>
	class basic_string_view
	{
	public:
		using traits_type = Traits;
		using value_type = CharT;
		using pointer = CharT*;
		using const_pointer = const CharT*;
		using reference = CharT&;
		using const_reference = const CharT&;
		using iterator = const basic_string_iterator<CharT>;
		using const_iterator = iterator;
		//using reverse_iterator = std::reverse_iterator<const_iterator>;
		using size_type = size_t;
		using difference_type =typename Traits::difference_type;

		static constexpr size_type npos{ static_cast<size_type>(-1) };
	private:
		const CharT* dat = nullptr;
		size_type ssize = 0;
	public:
		constexpr basic_string_view() noexcept = default;
		constexpr basic_string_view(const basic_string_view& other) noexcept = default;
		constexpr basic_string_view(const CharT* s, size_type count) noexcept:dat(s),ssize{count}{}
		constexpr basic_string_view(const CharT* s):dat(s),ssize(Traits::length(dat)){}

		constexpr basic_string_view& operator=(const basic_string_view& view) noexcept = default;

		constexpr const_iterator begin() const noexcept
		{
			return iterator(dat);
		}
		constexpr const_iterator cbegin() const noexcept
		{
			return iterator(dat);
		}

		constexpr const_iterator end() const noexcept
		{
			return iterator(dat + ssize);
		}
		constexpr const_iterator cend() const noexcept
		{
			return iterator(dat + ssize);
		}

		constexpr const_reference operator[](size_type pos) const
		{
			return iterator(dat + pos);
		}

		constexpr const_reference at(size_type pos) const
		{
			if (pos >= ssize)
				throw(out_of_range(""));
			return this->operator[](pos);
		}
		
		constexpr const_reference front() const noexcept
		{
#ifdef DEBUG
			assert(dat == nullptr);
#endif // DEBUG
			return *dat;
		}

		constexpr const_reference back() const noexcept
		{
#ifdef DEBUG
			assert(dat == nullptr);
#endif // DEBUG
			return *dat;
		}

		constexpr const_pointer data() const noexcept
		{
			return dat;
		}

		constexpr size_type size() const noexcept
		{
			return ssize;
		}
		constexpr size_type length() const noexcept
		{
			return ssize;
		}

		constexpr size_type max_size() const noexcept
		{
			return std::numeric_limits<size_type>::max();
		}

		constexpr bool empty() const noexcept
		{
			return ssize == 0;
		}

		constexpr void remove_prefix(size_type n)
		{
			dat += n;
			ssize -= n;
		}

		constexpr void remove_suffix(size_type n)
		{
			ssize -= n;
		}

		constexpr void swap(basic_string_view& v) noexcept
		{
			using std::swap;
			swap(dat, v.dat);
			swap(ssize, v.ssize);
		}

		size_type copy(CharT* dest,size_type count,size_type pos = 0) const
		{
			if (pos > ssize)
				throw(out_of_range(""));
			count = count < ssize - pos ? count : ssize - pos;
			Traits::copy(dest, dat + pos, count);
			return count;
		}

		constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const
		{
			if (pos > ssize)
				throw(out_of_range(""));
			count = count < ssize - pos ? count : ssize - pos;
			return basic_string_view{ dat + pos,count };
		}

		constexpr int compare(basic_string_view v) const noexcept
		{

		}
		constexpr int compare(size_type pos1, size_type count1,basic_string_view v) const;
		constexpr int compare(size_type pos1, size_type count1, basic_string_view v,size_type pos2, size_type count2) const;
		constexpr int compare(const CharT* s) const
		{
			
		}
		constexpr int compare(size_type pos1, size_type count1,const CharT* s) const
		{
			
		}
		constexpr int compare(size_type pos1, size_type count1,
			const CharT* s, size_type count2) const;

		constexpr size_type find(basic_string_view v, size_type pos = 0) const noexcept;
		constexpr size_type find(CharT c, size_type pos = 0) const noexcept;
		constexpr size_type find(const CharT* s, size_type pos, size_type count) const;
		constexpr size_type find(const CharT* s, size_type pos = 0) const;

		constexpr size_type rfind(basic_string_view v, size_type pos = npos) const noexcept;
		constexpr size_type rfind(CharT c, size_type pos = npos) const noexcept;
		constexpr size_type rfind(const CharT* s, size_type pos, size_type count) const;
		constexpr size_type rfind(const CharT* s, size_type pos = npos) const;

		constexpr size_type find_first_of(basic_string_view v, size_type pos = 0) const noexcept;
		constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept;
		constexpr size_type find_first_of(const CharT* s, size_type pos, size_type count) const;
		constexpr size_type find_first_of(const CharT* s, size_type pos = 0) const;

		constexpr size_type find_last_of(basic_string_view v, size_type pos = npos) const noexcept;
		constexpr size_type find_last_of(CharT c, size_type pos = npos) const noexcept;
		constexpr size_type find_last_of(const CharT* s, size_type pos, size_type count) const;
		constexpr size_type find_last_of(const CharT* s, size_type pos = npos) const;

		constexpr size_type find_first_not_of(basic_string_view v, size_type pos = 0) const noexcept;
		constexpr size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept;
		constexpr size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const;
		constexpr size_type find_first_not_of(const CharT* s, size_type pos = 0) const;

		constexpr size_type find_last_not_of(basic_string_view v, size_type pos = npos) const noexcept;
		constexpr size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept;
		constexpr size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const;
		constexpr size_type find_last_not_of(const CharT* s, size_type pos = npos) const;
	};


	template<typename CharT, typename Traits = char_traits<CharT>, typename Allocator = uuz::allocator<CharT>>
	class basic_string
	{
	public:

		using traits_type = Traits;
		using value_type = CharT;
		using allocator_type = Allocator;
		using size_type = typename allocator_traits<Allocator>::size_type;
		using difference_type = typename allocator_traits<Allocator>::difference_type;
		using reference = CharT&;
		using const_reference = const CharT&;
		using pointer = typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;	
		using iterator = basic_string_iterator<CharT, Traits>;
		using const_iterator =const iterator;
		//using reverse_iterator = std::reverse_iterator<const_iterator>;
		//using const_reverse_iterator = const std::reverse_iterator<const_iterator>;
		

		constexpr static size_t npos{ static_cast<size_t>(-1) };
		constexpr static size_t sso_size = 32;
	private:
		union
		{
			CharT sso_data[sso_size];
			struct
			{
				CharT* begin = nullptr;
				size_t max_size = 0;
			}data;
		}dat;
		bool sso_flag = true;
		size_t ssize = 0;
		Allocator alloc;
	public:
		
		basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator())
		{
			//C++17
			//Default constructor. Constructs empty string (zero size and unspecified capacity).
		}
		explicit basic_string(const Allocator& alloc) noexcept:alloc(alloc)
		{
			//C++17
			//Constructs the string with count copies of character ch. The behavior is undefined if count >= npos.

		}
		basic_string(size_t count,CharT ch,const Allocator& alloc = Allocator()):basic_string(alloc)
		{
			try
			{
				assign(count, ch);
			}
			catch(...)
			{
				clear();
				throw;
			}
		}
		basic_string(const basic_string& other,size_t pos,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(other, pos);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(const basic_string& other,size_t pos,size_t count,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(other, pos,count);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(const CharT* s,size_t count,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(s, count);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(const CharT* s,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(s);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		template< typename InputIt, typename = is_input<value_type, InputIt>>
		basic_string(InputIt first, InputIt last,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(first,last);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(const basic_string& other) :basic_string()
		{
			try
			{
				assign(other);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(const basic_string& other, const Allocator& alloc) :basic_string(alloc)
		{
			try
			{
				assign(other);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(basic_string&& other) noexcept
		{
			::memcpy(reinterpret_cast<char*>(this), reinterpret_cast<char*>(&other), sizeof(basic_string));
			alloc = std::move(other.alloc);
			other.sso_flag = true;
			other.clear();
		}
		basic_string(basic_string&& other, const Allocator& alloc)noexcept(allocator_traits<Allocator>::is_always_equal::value) :basic_string(alloc)
		{
			try
			{
				assign(std::move(other));
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		basic_string(std::initializer_list<CharT> init,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(init);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		explicit basic_string(std::basic_string_view<CharT, Traits> sv,
								const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(sv);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string(const T& t, size_t pos, size_t n,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			try
			{
				assign(t);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		
		//C++17
		//If an exception is thrown for any reason, this function has no effect (strong exception guarantee).
		//强异常保证
		basic_string& operator=(const basic_string& str)
		{
			//C++17
			//Replaces the contents with a copy of str.
			//If *this and str are the same object, this function has no effect.
			if (this == &str) //相等滚蛋
				return *this;
			assign_str(str.data(), str.size());
			return *this;
		}
		basic_string& operator=(basic_string&& str)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			//我不知道这是什么傻逼标准，反正C++17这么说的
			//Replaces the contents with those of str using move semantics.str is in a valid but unspecified state afterwards.
			//If std::allocator_traits<Allocator>::propagate_on_container_move_assignment() is true, 
			//the target allocator is replaced by a copy of the source allocator.
			//If it is false and the source and the target allocators do not compare equal,
			//the target cannot take ownership of the source memory and must assign each character individually,
			//allocating additional memory using its own allocator as needed.Unlike other container move assignments, 
			//references, pointers, and iterators to str may be invalidated.
			if (this == &str)
				return *this;
			if(str.sso_flag) //如果sso数据的话直接拷贝
			{
				Traits::copy(str.data(), data(), str.size() + 1);
				ssize = str.size();
				str.clear();
				if constexpr(allocator_traits<Allocator>::propagate_on_container_move_assignment::value)
					alloc = std::move(str.alloc);
				return *this;
			}
			if constexpr(allocator_traits<Allocator>::propagate_on_container_move_assignment::value)
			{ //直接拷贝allocator
				clear();
				alloc = std::move(str.alloc);
				this->dat.data.begin = str.dat.data.begin;
				this->dat.data.max_size = str.dat.data.max_size;
				sso_flag = false;
				str.sso_flag = true;
			}
			else
			{
				if (alloc == str.alloc)
				{
					clear();
					this->dat.data.begin = str.dat.data.begin;
					this->dat.data.max_size = str.dat.data.max_size;
					sso_flag = false;
					str.sso_flag = true;
				}
				else
					//allocator不同转复制
					return this->operator=(str);
			}
			ssize = str.size();
			str.clear();
			return *this;		
		}
		basic_string& operator=(const CharT* s)
		{
			//C++17
			//Replaces the contents with those of null - terminated character string pointed to by s as if by *this = basic_string(s), 
			//which involves a call to Traits::length(s).
			if(s == nullptr)
				clear();
			else
				assign_str(s,Traits::length(s));
			return *this;
		}
		basic_string& operator=(CharT ch)
		{
			//C++ 17
			//Replaces the contents with character ch as if by *this = basic_string(1,c)
			Traits::assign(*data(), ch);
			Traits::assign(*(data() + 1), Traits::eof());
			ssize = 1;
			return *this;
		}
		basic_string& operator=(std::initializer_list<CharT> str)
		{
			//C++17
			//Replaces the contents with those of the initializer list ilist as if by *this = basic_string(ilist)
			assign_iterator(str.begin(), str.end(), str.size());
			return *this;
		}
		basic_string& operator=(basic_string_view<CharT, Traits> sv)
		{
			//C++17
			//Replaces the contents with those of the string view sv as if by assign(sv)
			assign_str(sv.data(), sv.size());
			return *this;
		}

		//C++17
		//If an exception is thrown for any reason, this function has no effect (strong exception guarantee).
		//强异常保证
		basic_string& assign(size_t count, CharT ch)
		{
			//C++17
			// Replaces the contents with count copies of character ch.
			if (capacity() > count)
			{
				ssize = count;
				Traits::assign(data(), ch, count);
				Traits::assign(*(data() + count), Traits::eof());
			}
			else
			{
				if (count >= max_size())
					throw(length_error(""));
				auto p = allocator_traits<Allocator>::allocator(alloc, count + 1);
				
				ssize = count;
				Traits::assign(data(), ch, count);
				Traits::assign(*(data() + count), Traits::eof());
				

				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;
				dat.data.begin = p;
				dat.data.max_size = ssize + 1;
			}
			return *this;
		}
		basic_string& assign(const basic_string& str)
		{
			//C++17
			//Replaces the contents with a copy of str
			return operator=(str);
		}
		basic_string& assign(const basic_string& str,size_t pos,size_t count = npos)
		{
			//C++17
			//Replaces the contents with a substring [pos, pos+count) of str. 
			//If the requested substring lasts past the end of the string, or if count == npos, 
			//the resulting substring is [pos, str.size()). If pos > str.size(), std::out_of_range is thrown.
			if (pos > str.size())
				throw(out_of_range(""));
			assign_str(str.data() + pos, str.size() - pos > count ? count : str.size() - pos);
			return *this;
		}
		basic_string& assign(basic_string&& str)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			//C++17
			//Replaces the contents with those of str using move semantics. 
			//Equivalent to *this = std::move(str).
			return this->operator=(std::move(str));
		}
		basic_string& assign(const CharT* s,size_t count)
		{
			//C++17
			//Replaces the contents with the first count characters of character string pointed to by s.
			//s can contain null characters.
			if (s == nullptr)
				clear();
			else
				assign_str(s, count);
			return *this;
		}
		basic_string& assign(const CharT* s)
		{
			//C++17
			//Replaces the contents with those of null-terminated character string pointed to by s. 
			//The length of the string is determined by the first null character.
			return this->operator=(s);
		}
		template< typename InputIt,typename  = is_input<value_type,InputIt>>
		basic_string& assign(InputIt first, InputIt last)
		{
			//C++17
			//Replaces the contents with copies of the characters in the range [first, last). 
			//This overload does not participate in overload resolution if InputIt does not satisfy InputIterator. 
			assign_iterator(first, last, distance(first, last));
			return *this;
		}
		basic_string& assign(std::initializer_list<CharT> ilist)
		{
			//C++17
			//Replaces the contents with those of the initializer list ilist.
			return this->operator=(ilist);
		}
		basic_string& assign(basic_string_view<CharT, Traits> sv)
		{
			//C++17
			//Replaces the contents with those of the string view sv, 
			//as if by assign(sv.data(), sv.size())
			return this->operator=(sv);
		}
		template < typename T ,
		typename = std::enable_if_t<std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> 
								&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string& assign(const T& t,size_t pos,size_t count = npos)
		{
			//又是个傻逼C++17要求
			//Converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t; ,
			//then replaces the contents with the characters from the subview[pos, pos + count) of sv.
			//If the requested subview lasts past the end of sv, or if count == npos, 
			//the resulting subview is[pos, sv.size()).If pos > sv.size(), std::out_of_range is thrown.
			//This overload only participates in overload resolution
			//if std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> is true 
			//and std::is_convertible_v<const T&, const CharT*> is false.
			auto str = basic_string_view<CharT, Traits>(t);
			if (pos > str.size())
				throw(out_of_range(""));
			assign_str(str.data() + pos, str.size() - pos > count ? count : str.size() - pos);
			return *this;

		}

		allocator_type get_allocator() const noexcept(noexcept(std::is_nothrow_copy_constructible_v<Allocator>))
		{
			//C++17
			//Returns the allocator associated with the string.
			return alloc;
		}

		reference       at(size_type pos)
		{
			if (pos >= size())
				throw(out_of_range(""));
			return this->operator[](pos);
		}
		const_reference at(size_type pos) const
		{
			{
				if (pos >= size())
					throw(out_of_range(""));
				return this->operator[](pos);
			}
		}

		reference       operator[](size_type pos)noexcept
		{
			return *(data() + pos);
		}
		const_reference operator[](size_type pos) const noexcept
		{
			return *(data() + pos);
		}

		CharT& front()
		{
			if (size() == 0)
				throw(out_of_range(""));
			return *data();
		}
		const CharT& front() const
		{
			if (size() == 0)
				throw(out_of_range(""));
			return *data();
		}

		CharT& back()
		{
			if (size() == 0)
				throw(out_of_range(""));
			return *(data()+(size()-1));
		}
		const CharT& back() const
		{
			if (size() == 0)
				throw(out_of_range(""));
			return *(data() + (size() - 1));
		}

		const CharT* data() const noexcept
		{
			return c_str();
		}
		CharT* data()noexcept
		{
			if (sso_flag)
				return data.sso_data;
			else
				return data.data.begin;
		}

		const CharT* c_str() const noexcept
		{
			if (sso_flag)
				return data.sso_data;
			else
				return data.data.begin;
		}

		operator basic_string_view<CharT, Traits>() const noexcept
		{
			return basic_string_view<CharT, Traits>(data(), size());
		}

		iterator begin()noexcept
		{
			return iterator(data());
		}
		const_iterator begin() const noexcept
		{
			return iterator(data());
		}
		const_iterator cbegin() const noexcept
		{
			return iterator(data());
		}

		iterator end() noexcept
		{
			return iterator(data()+size());
		}
		const_iterator end() const noexcept
		{
			return iterator(data() + size());
		}
		const_iterator cend() const noexcept
		{
			return iterator(data() + size());
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}

		size_type size() const noexcept
		{
			return ssize;
		}
		size_type length() const noexcept
		{
			return ssize;
		}

		size_type max_size() const
		{
			return allocator_traits<Allocator>::max_size();
		}

		void reserve(size_type new_cap = 0)
		{
			if (new_cap < capacity())
				return;
			if (new_cap >= max_size())
				throw(length_error(""));
			auto t = allocator_traits<Allocator>::allocate(alloc, new_cap + 1);
			Traits::move(t, data(), size() + 1);
			if(!sso_flag)
				allocator_traits<Allocator>::deallocate(alloc, dat.data.begin,dat.data.max_size);
			dat.data.begin = t;
			dat.data.max_size = new_cap + 1;
			sso_flag = true;
		}

		size_type capacity() const noexcept
		{
			if (sso_flag)
				return sso_size;
			else
				return dat.data.max_size;
		}

		void shrink_to_fit()
		{
			if(!sso_flag)
			{
				if (size() < sso_size)
				{
					auto k = dat.data.begin;
					auto old_size = dat.data.max_size;
					Traits::copy(data.sso_data, k, size() + 1);
					allocator_traits<Allocator>::deallocate(alloc, k, old_size);
					sso_flag = true;
				}
				else if (size() + 1< dat.data.max_size)
				{
					auto k = dat.data.begin;
					auto old_size = dat.data.max_size;
					auto new_c = allocator_traits<Allocator>::allocate(alloc, size() + 1);
					Traits::copy(new_c, k, size() + 1);
					allocator_traits<Allocator>::deallocate(alloc, k, old_size);
					dat.data.begin = new_c;
					dat.data.max_size = size() + 1;
				}
			}
		}

		void clear()noexcept
		{
			if(!sso_flag)
				allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
			ssize = 0;
			sso_flag = true;
		}

		basic_string& insert(size_type index, size_type count, CharT ch)
		{
			if(size()+count < capacity())
			{
				Traits::move(data() + index + count, data() + index, size() - index);
				Traits::assign(data()+index, ch, count);
				Traits::assign(*(data() + size()), Traits::eof);
				ssize += count;
			}
			else
			{
				auto temp = allocator_traits<Allocator>::allocate(alloc, size() + count + 1);
				Traits::move(temp, data(), index);
				Traits::assign(data() + index, ch, count);
				Traits::move(temp + index + count, data()+index, size() - index);
				ssize += count;
				Traits::assign(*(temp + size()), Traits::eof);
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;
				dat.data.begin = temp;
				dat.data.max_size = ssize + 1;
			}
			return *this;
		}
		basic_string& insert(size_type index, const CharT* s)
		{
			if(s)
				insert_str(index, s, Traits::length(s));
			return *this;
		}
		basic_string& insert(size_type index, const CharT* s, size_type count)
		{
			if(s)
				insert_str(index, s, count);
			return *this;
		}
		basic_string& insert(size_type index, const basic_string& str)
		{
			return insert(index, str.data(), str.size());
		}
		basic_string& insert(size_type index, const basic_string& str,size_type index_str, size_type count = npos)
		{
			if (index_str >= str.size())
				throw(out_of_range(""));
			return insert(index, str.data() + index_str, count < str.size() - index ? count : str.size() - index);
		}
		iterator insert(const_iterator pos, CharT ch)
		{
			insert(pos - begin(), 1, ch);
			return pos;
		}
		iterator insert(const_iterator pos, size_type count, CharT ch)
		{
			insert(pos - begin(), count, ch);
			return pos;
		}
		template< typename InputIt, typename = is_input<value_type, InputIt>>
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			insert_iterator(pos - begin(), first, last, distance(first, last));
			return pos;
		}
		iterator insert(const_iterator pos, std::initializer_list<CharT> ilist)
		{
			insert_iterator(pos - begin(), ilist.begin(), ilist.end(), ilist.size());
			return pos;
		}
		basic_string& insert(size_type pos, basic_string_view<CharT, Traits> sv)
		{
			return insert(pos, sv.data());
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string& insert(size_type index, const T& t,size_type index_str, size_type count = npos)
		{
			auto str = basic_string_view<CharT, Traits>(t);
			if (index_str >= str.size())
				throw(out_of_range(""));
			insert_str(str.data() + index_str, str.size() - index_str > count ? count : str.size() - index_str);
			return *this;
		}

		basic_string& erase(size_type index = 0, size_type count = npos)
		{
			if (index > size())
				throw(out_of_range(""));
			erase(iterator(begin() + index), iterator(begin() + (count < size() - index ? count : size() - index)));
			return *this;
		}
		iterator erase(const_iterator position)noexcept
		{
			Traits::move(position.dat, position.dat + 1, end() - position);
			--ssize;
			return position;
		}
		iterator erase(const_iterator first, const_iterator last)noexcept
		{
			Traits::move(first.dat, last.dat, end() - last);
			ssize -= distance(first, last);
			return first;
		}

		void push_back(CharT ch)
		{
			reserve(size() + 1);
			Traits::assign(at(size()), ch);
			Traits::assign(at(size() + 1), Traits::eof());
			++ssize;
		}

		void pop_back()
		{
			Traits::assign(at(size()), Traits::eof());
			--ssize;
		}

		basic_string& append(size_type count, CharT ch)
		{
			if(size()+count < capacity())
			{
				Traits::assign(data() + size(), ch, count);
				Traits::assign(data() + size() + count, Traits::eof());
			}
			else
			{
				if (size() + count + 1 >= max_size())
					throw(length_error(""));
				
				auto p = allocator_traits<Allocator>::allocate(alloc, size() + count + 1);
				Traits::copy(p, data(), size());
				Traits::assign(p + size(), ch, count);
				Traits::assign(p + size() + count, Traits::eof());

				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;

				dat.data.begin = p;
				dat.data.max_size = size() + count + 1;
			}
			ssize += count;
			return *this;
		}
		basic_string& append(const basic_string& str)
		{
			append_str(str.data(), str.size());
			return *this;
		}
		basic_string& append(const basic_string& str,size_type pos,size_type count = npos)
		{
			if (pos >= str.size())
				throw(out_of_range(""));
			return append( str.data() + pos, count < str.size() - pos ? count : str.size() - pos);
		}
		basic_string& append(const CharT* s, size_type count)
		{
			if(s)
				append_str(s, count);
			return *this;
		}
		basic_string& append(const CharT* s)
		{
			if(s)
				append_str(s, Traits::length(s));
			return *this;
		}
		template< typename InputIt, typename = is_input<value_type, InputIt>>
		basic_string& append(InputIt first, InputIt last)
		{
			append_iterator(first, last, distance(first, last));
			return *this;
		}
		basic_string& append(std::initializer_list<CharT> ilist)
		{
			append_iterator(ilist.begin(), ilist.end(), ilist.size());
			return *this;
		}
		basic_string& append(basic_string_view<CharT, Traits> sv)
		{
			append_str(sv.data(), sv.size());
			return *this;
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string& append(const T& t, size_type pos,size_type count = npos)
		{
			auto str = basic_string_view<CharT, Traits>(t);
			if (pos > str.size())
				throw(out_of_range(""));
			append_str(str.data() + pos, str.size() - pos > count ? count : str.size() - pos);
			return *this;
		}

		basic_string& operator+=(const basic_string& str)
		{
			return append(str);
		}
		basic_string& operator+=(CharT ch)
		{
			push_back(ch);
			return *this;
		}
		basic_string& operator+=(const CharT* s)
		{
			return append(s);
		}
		basic_string& operator+=(std::initializer_list<CharT> ilist)
		{
			return append(ilist);
		}
		basic_string& operator+=(basic_string_view<CharT, Traits> sv)
		{
			return append(sv);
		}

		int compare(const basic_string& str) const
		{
			auto p = size() - str.size();
			auto k = Traits::compare(data(), str.data(), p > 0 ? size() : str.size());
			if (k == 0)
				return p;
			return k;
		}
		int compare(size_type pos1, size_type count1,const basic_string& str) const
		{
			
		}
		int compare(size_type pos1, size_type count1,
			const basic_string& str,
			size_type pos2, size_type count2 = npos) const;
		int compare(const CharT* s) const;
		int compare(size_type pos1, size_type count1,
				const CharT* s) const;
		int compare(size_type pos1, size_type count1,
			const CharT* s, size_type count2) const;
		int compare(std::basic_string_view<CharT, Traits> sv) const;
			int compare(size_type pos1, size_type count1,
				std::basic_string_view<CharT, Traits> sv) const;
			template < class T >
		int compare(size_type pos1, size_type count1,
			const T& t,
			size_type pos2, size_type count2 = npos) const;

		basic_string& replace(size_type pos, size_type count,
			const basic_string& str);
		basic_string& replace(const_iterator first, const_iterator last,
				const basic_string& str);
		basic_string& replace(size_type pos, size_type count,
			const basic_string& str,
			size_type pos2, size_type count2 = npos);
		template< class InputIt >
		basic_string& replace(const_iterator first, const_iterator last,
			InputIt first2, InputIt last2);
		
		basic_string& replace(size_type pos, size_type count,
				const CharT* cstr, size_type count2);
		
		basic_string& replace(const_iterator first, const_iterator last,
				const CharT* cstr, size_type count2);
		
		basic_string& replace(size_type pos, size_type count,
				const CharT* cstr);
	
		basic_string& replace(const_iterator first, const_iterator last,
				const CharT* cstr);
		
		basic_string& replace(size_type pos, size_type count,
				size_type count2, CharT ch);
		
		basic_string& replace(const_iterator first, const_iterator last,
				size_type count2, CharT ch);
	
		basic_string& replace(const_iterator first, const_iterator last,
				std::initializer_list<CharT> ilist);

		basic_string& replace(size_type pos, size_type count,
			std::basic_string_view<CharT, Traits> sv);
		basic_string& replace(const_iterator first, const_iterator last,
				std::basic_string_view<CharT, Traits> sv);
		template < class T >
		basic_string& replace(size_type pos, size_type count, const T& t,
				size_type pos2, size_type count2 = npos);

		basic_string substr(size_type pos = 0,size_type count = npos) const
		{
			return basic_string(data(), pos, count);
		}

		size_type copy(CharT* dest,size_type count,size_type pos = 0) const
		{
			count = count < size() - pos ? count : size() - pos;
			Traits::copy(dest, data(), count);
			return count;
		}

		void resize(size_type count)
		{
			return resize(count, Traits::eof());
		}
		void resize(size_type count, CharT ch)
		{
			if(count < size())
				Traits::assign(at(ssize()), Traits::eof());
			else if(count > size())
				append(count - size(), ch);
			ssize = count;
		}

		void swap(basic_string& other) noexcept(is_nothrow_swap_alloc < Allocator >::value )
		{
			if (this == &other)
				return;
			if(alloc == other.alloc)
			{
				char temp[sizeof(basic_string)];
				::memcpy(temp, reinterpret_cast<char*>(this), sizeof(basic_string));
				::memcpy(reinterpret_cast<char*>(this), reinterpret_cast<char*>(&other), sizeof(basic_string));
				::memcpy(reinterpret_cast<char*>(&other), temp, sizeof(basic_string));
			}
			else
			{
#ifdef DEBUG
				assert(false, "It't undefined!");
#endif // DEBUG
				auto k{ other ,this->alloc };
				other = *this;
				*this = std::move(k);
			}
		}

		size_type find(const basic_string& str, size_type pos = 0) const;
		size_type find(const CharT* s, size_type pos, size_type count) const;
		size_type find(const CharT* s, size_type pos = 0) const;
		size_type find(CharT ch, size_type pos = 0) const;
		size_type find(std::basic_string_view<CharT, Traits> sv,
			size_type pos = 0) const;

		size_type rfind(const basic_string& str, size_type pos = npos) const;
		size_type rfind(const CharT* s, size_type pos, size_type count) const;
		size_type rfind(const CharT* s, size_type pos = npos) const;	
		size_type rfind(CharT ch, size_type pos = npos) const;	
		size_type rfind(std::basic_string_view<CharT, Traits> sv,
					size_type pos = npos) const;

		size_type find_first_of(const basic_string& str, size_type pos = 0) const;
		size_type find_first_of(const CharT* s, size_type pos, size_type count) const;
		size_type find_first_of(const CharT* s, size_type pos = 0) const;
		size_type find_first_of(CharT ch, size_type pos = 0) const;
		size_type find_first_of(std::basic_string_view<CharT, Traits> sv,
			size_type pos = 0) const;

		size_type find_first_not_of(const basic_string& str, size_type pos = 0) const;
		size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const;
		size_type find_first_not_of(const CharT* s, size_type pos = 0) const;
		size_type find_first_not_of(CharT ch, size_type pos = 0) const;
		size_type find_first_not_of(std::basic_string_view<CharT, Traits> sv,
			size_type pos = 0) const;

		size_type find_last_of(const basic_string& str, size_type pos = npos) const;
		size_type find_last_of(const CharT* s, size_type pos, size_type count) const;
		size_type find_last_of(const CharT* s, size_type pos = npos) const;
		size_type find_last_of(CharT ch, size_type pos = npos) const;
		size_type find_last_of(std::basic_string_view<CharT, Traits> sv,
			size_type pos = npos) const;

		size_type find_last_not_of(const basic_string& str, size_type pos = npos) const;
		size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const;
		size_type find_last_not_of(const CharT* s, size_type pos = npos) const;
		size_type find_last_not_of(CharT ch, size_type pos = npos) const;
		size_type find_last_not_of(std::basic_string_view<CharT, Traits> sv,
			size_type pos = npos) const;
		
		~basic_string()
		{
			clear();
		}
	private:

		void assign_str(const_pointer p,size_t t)
		{
			if (capacity() > t)//空间够了
			{
				Traits::copy(p, data(), t);
				Traits::assign(*(data() + t), Traits::eof());
			}
			else//空间不够
			{
				if (t >= max_size())
					throw(length_error(""));
				//先申请并复制，出错也安全
				auto pp = allocator_traits<Allocator>::allocator(alloc, t + 1);
				Traits::copy(pp, p, t);
				Traits::assign(*(pp + t), Traits::eof());

				//如果原来有内存资源就释放，否则置sso_flag为false
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;

				dat.data.begin = pp;
				dat.data.max_size = t + 1;	
			}
			ssize = t;
		}
		template<typename It,typename = is_input<value_type,It>>
		void assign_iterator(It first,It end,size_t t)
		{
			if (capacity() > t)//空间够了
			{
				uuz::copy(first, end, data());
				Traits::assign(*(data() + t), Traits::eof());	
			}
			else//空间不够
			{
				if (t >= max_size())
					throw(length_error(""));

				//先申请并复制，出错也安全
				auto p = allocator_traits<Allocator>::allocator(alloc, t + 1);
				uuz::copy(first, end, p);
				Traits::assign(*(p + t), Traits::eof());

				//如果原来有内存资源就释放，否则置sso_flag为false
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;

				dat.data.begin = p;
				dat.data.max_size = t + 1;
			}
			ssize = t;
		}

		void insert_str(size_t index,const_pointer s ,size_t count)
		{
			if (size() + count < capacity())
			{
				Traits::move(data() + index + count, data() + index, size() - index);
				Traits::copy(data() + index, s, count);
				Traits::assign(*(data() + size()), Traits::eof);
			}
			else
			{
				if (size() + count + 1 >= max_size())
					throw(length_error(""));
				auto temp = allocator_traits<Allocator>::allocate(alloc, size() + count + 1);
				Traits::move(temp, data(), index);
				Traits::copy(data() + index, s, count);
				Traits::move(temp + index + count, data() + index, size() - index);
				
				Traits::assign(*(temp + size()), Traits::eof);
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;
				dat.data.begin = temp;
				dat.data.max_size = size() + count + 1;
			}
			ssize += count;
		}
		template<typename It, typename = is_input<value_type, It>>
		void insert_iterator(size_t index,It first, It end, size_t count)
		{
			if (size() + count < capacity())
			{
				Traits::move(data() + index + count, data() + index, size() - index);
				uuz::copy(first,end,data() + index);
				Traits::assign(*(data() + size()), Traits::eof);
			}
			else
			{
				if (size() + count + 1 >= max_size())
					throw(length_error(""));

				auto temp = allocator_traits<Allocator>::allocate(alloc, size() + count + 1);
				Traits::move(temp, data(), index);
				uuz::copy(first, end, data() + index);
				Traits::move(temp + index + count, data() + index, size() - index);	
				Traits::assign(*(temp + size()), Traits::eof);
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;
				dat.data.begin = temp;
				dat.data.max_size = size() + count + 1;
			}
			ssize += count;
		}

		void append_str(const_pointer s, size_t count)
		{
			if (size() + count < capacity())
			{
				Traits::copy(data() + size(), s, count);
				Traits::assign(data() + size() + count, Traits::eof());
			}
			else
			{
				if (size() + count + 1 >= max_size())
					throw(length_error(""));

				auto p = allocator_traits<Allocator>::allocate(alloc, size() + count + 1);
				Traits::copy(p, data(), size());
				Traits::copy(p + size(), s, count);
				Traits::assign(p + size() + count, Traits::eof());
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;

				dat.data.begin = p;
				dat.data.max_size = size() + count + 1;
			}
			ssize += count;
		}
		template<typename It, typename = is_input<value_type, It>>
		void append_iterator(size_t index, It first, It end, size_t count)
		{
			if (size() + count < capacity())
			{
				uuz::copy(first, end, data()+size());
				Traits::assign(data() + size() + count, Traits::eof());
			}
			else
			{
				if (size() + count + 1 >= max_size())
					throw(length_error(""));

				auto temp = allocator_traits<Allocator>::allocate(alloc, size() + count + 1);
				Traits::copy(temp, data(), size());
				uuz::copy(first, end, temp + size());
				Traits::assign(temp + size() + count, Traits::eof());
				if (!sso_flag)
					allocator_traits<Allocator>::deallocate(alloc, dat.data.begin, dat.data.max_size);
				else
					sso_flag = false;
				dat.data.begin = temp;
				dat.data.max_size = size() + count + 1;
			}
			ssize += count;
		}
	};
	using string = basic_string<char>;
	using string_view = basic_string_view<char>;
}