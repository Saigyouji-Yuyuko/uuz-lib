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

	namespace
	{
		template<typename CharT,typename size_type,typename = typename char_traits<CharT>::char_type>
		constexpr int _compare(CharT* s1,size_type count1,
			CharT* s2, size_type count2)noexcept
		{
			int k = count1 - count2;
			auto p = char_traits<CharT>::compare(s1, s2, k > 0 ? count1 : count2);
			if (p == 0)
				return k;
			return p;
		}
		template<typename CharT, typename size_type, typename = typename char_traits<CharT>::char_type>
		constexpr CharT* _find(CharT* s1,CharT* s2, size_type count2)noexcept
		{
			while (*(s1 + count2))
				if (*s1 == *s2 && char_traits<CharT>::compare(s1, s2, count2) == 0)
					return s1;
				else
					++s1;
			return nullptr;
		}
		template<typename CharT, typename size_type, typename = typename char_traits<CharT>::char_type>
		constexpr CharT* _find(CharT* begin, CharT* end, CharT* s2, size_type count2)noexcept
		{
			while((end - count2) != begin)
				if (*(end - count2) == *s2 && char_traits<CharT>::compare(end - count2, s2, count2) == 0)
					return end - count2;
				else
					--end;
			return char_traits<CharT>::compare(begin, s2, count2) == 0 ? begin : nullptr;
		}
		template<typename CharT, typename size_type, typename = typename char_traits<CharT>::char_type>
		constexpr CharT* _find_first_of(CharT* begin, CharT* s2, size_type count)noexcept
		{

			while(*begin)
			{
				for (auto i = 0; i != count; ++i)
					if (char_traits<CharT>::eq(*(s2 + i), *begin))
						return begin;
				++begin;
			}
			return nullptr;
		}
		template<typename CharT, typename size_type, typename = typename char_traits<CharT>::char_type>
		constexpr CharT* _find_first_not_of(CharT* begin, CharT* s2, size_type count)noexcept
		{

			while (*begin)
			{
				for (auto i = 0; i != count; ++i)
					if (char_traits<CharT>::eq(*(s2 + i), *begin))
						goto ll;
				return begin;
ll:				++begin;
			}
			return nullptr;
		}
		template<typename CharT, typename size_type, typename = typename char_traits<CharT>::char_type>
		constexpr CharT* _find_last_of(CharT* begin, CharT* end, CharT* s2, size_type count)noexcept
		{
			while (begin  < --end)
			{
				for (auto i = 0; i != count; ++i)
					if (char_traits<CharT>::eq(*(end), *begin))
						return end;
			}
			return nullptr;
		}
			
		template<typename CharT, typename size_type, typename = typename char_traits<CharT>::char_type>
		constexpr CharT* _find_last_not_of(CharT* begin, CharT* end, CharT* s2, size_type count)noexcept
		{
			while (begin  < --end)
			{
				for (auto i = 0; i != count; ++i)
					if (char_traits<CharT>::eq(*(end), *begin))
						goto ll;
				return end;
			ll:;
			}
			return nullptr;
		}
	}

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
			return _compare(data(), size(), v.data(), v.size());
		}
		constexpr int compare(size_type pos1, size_type count1,basic_string_view v) const
		{
			if (pos1 > size())
				throw(out_of_range(""));
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, v.data(), v.size());
		}
		constexpr int compare(size_type pos1, size_type count1, basic_string_view v,size_type pos2, size_type count2) const
		{
			if (pos1 > size() || pos2 > v.size())
				throw(out_of_range(""));
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1,
						v.data()+pos2, v.size() - pos2 > count2 ? count2 : v.size() - pos2);
		}
		constexpr int compare(const CharT* s) const
		{
			return _compare(data(), size(), s, Traits::length(s));
		}
		constexpr int compare(size_type pos1, size_type count1,const CharT* s) const
		{
			if (pos1 > size())
				throw(out_of_range(""));
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, s, Traits::length(s));
		}
		constexpr int compare(size_type pos1, size_type count1,const CharT* s, size_type count2) const
		{
			if (pos1 > size())
				throw(out_of_range(""));
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, s, count2);
		}

		constexpr size_type find(basic_string_view v, size_type pos = 0) const noexcept
		{
			return find(v.data(), pos, v.size());
		}
		constexpr size_type find(CharT c, size_type pos = 0) const noexcept
		{
			return Traits::find(data() + pos, size() - pos, c) - data();
		}
		constexpr size_type find(const CharT* s, size_type pos, size_type count) const
		{
			if (pos > size())
				throw(out_of_range(""));
			if (count > size())
				return npos;
			auto k = _find(data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		constexpr size_type find(const CharT* s, size_type pos = 0) const
		{
			return find(s, pos, Traits::length(s));
		}

		constexpr size_type rfind(basic_string_view v, size_type pos = npos) const noexcept
		{
			return rfind(v.data(), pos, v.size());
		}
		constexpr size_type rfind(CharT c, size_type pos = npos) const noexcept
		{
			if (pos == npos)
				pos = size();

			for (auto i = data() + pos - 1; i != data(); --i)
				if (Traits::eq(*i, c))
					return i - data();
			return Traits::eq(*data(), c) ? 0 : npos;
		}
		constexpr size_type rfind(const CharT* s, size_type pos, size_type count) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));
			if (count > size())
				return npos;

			auto k = _rfind(data(), data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		constexpr size_type rfind(const CharT* s, size_type pos = npos) const
		{
			return rfind(s, pos, Traits::length(s));
		}

		constexpr size_type find_first_of(basic_string_view v, size_type pos = 0) const noexcept
		{
			return find_first_of(v.data(), pos, v.size());
		}
		constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept
		{
			return find(c, pos);
		}
		constexpr size_type find_first_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_first_of(data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		constexpr size_type find_first_of(const CharT* s, size_type pos = 0) const
		{
			return find_first_of(s, pos, Traits::length(s));
		}

		constexpr size_type find_last_of(basic_string_view v, size_type pos = npos) const noexcept
		{
			return find_last_of(v.data(), pos, v.size());
		}
		constexpr size_type find_last_of(CharT c, size_type pos = npos) const noexcept
		{
			return rfind(c, pos);
		}
		constexpr size_type find_last_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_last_of(data(), data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		constexpr size_type find_last_of(const CharT* s, size_type pos = npos) const
		{
			return find_last_of(s, pos,Traits::length(s));
		}

		constexpr size_type find_first_not_of(basic_string_view sv, size_type pos = 0) const noexcept
		{
			return find_first_not_of(sv, pos, sv.size());
		}
		constexpr size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept
		{
			if (pos > size())
				throw(out_of_range(""));

			for (auto i = 0; i != size(); ++i)
			{
				if (Traits::eq(at(i), c))
					goto ll;
				return i;
			ll:;
			}
			return npos;
		}
		constexpr size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_first_not_of(data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		constexpr size_type find_first_not_of(const CharT* s, size_type pos = 0) const
		{
			return find_first_not_of(s, pos, Traits::length(s));
		}

		constexpr size_type find_last_not_of(basic_string_view sv,size_type pos = npos) const noexcept
		{
			return find_last_not_of(sv.data(), pos, sv.size());
		}
		constexpr size_type find_last_not_of(CharT ch, size_type pos = npos) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));

			for (auto i = pos - 1; i > 0; --i)
			{
				if (Traits::eq(at(i), ch))
					goto ll;
				return i;
			ll:;
			}
			return npos;
		}
		constexpr size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));
			if (count > size())
				return npos;

			auto k = _find_last_not_of(data(), data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		constexpr size_type find_last_not_of(const CharT* s, size_type pos = npos) const
		{
			return find_last_not_of(s, pos, Traits::length(s));
		}
		
		
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
		constexpr static size_t sso_size = 20 / sizeof(value_type);
	private:
		using view = basic_string_view<CharT, Traits>;


		union
		{
			CharT sso_data[sso_size];
			CharT* no_sso_data = nullptr;	
		};
		size_t maxsize = sso_size - 1;
		size_t ssize = 0;
		Allocator alloc;
	public:
		
		//C++17
		//Constructs new string from a variety of data sources and optionally using user supplied allocator alloc.
		//Calls to Allocator::allocate may throw.
		basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator())
		{
			//C++17
			//Default constructor. Constructs empty string (zero size and unspecified capacity).
			//constant
		}
		explicit basic_string(const Allocator& alloc) noexcept:alloc(alloc)
		{
			//C++17
			//Default constructor. Constructs empty string (zero size and unspecified capacity).
			//constant
		}
		basic_string(size_t count,CharT ch,const Allocator& alloc = Allocator()):basic_string(alloc)
		{
			//C++17
			//Constructs the string with count copies of character ch. The behavior is undefined if count >= npos.
			// linear in count
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
			//C++17
			//Constructs the string with a substring [pos, pos+count) of other. If count == npos,
			//if count is not specified, or if the requested substring lasts past the end of the string, 
			//the resulting substring is [pos, size()).
			//linear in count
			//std::out_of_range if pos > other.size()
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
			//C++17
			//Constructs the string with a substring [pos, pos+count) of other. If count == npos,
			//if count is not specified, or if the requested substring lasts past the end of the string, 
			//the resulting substring is [pos, size()).
			//linear in count
			//std::out_of_range if pos > other.size()
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
			//C++17
			//Constructs the string with the first count characters of character string pointed to by s.
			//s can contain null characters.
			//The length of the string is count.
			//The behavior is undefined if s does not point at an array of at least count elements of CharT,
			//including the case when s is a null pointer.
			//linear in count
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
			//C++17
			//Constructs the string with the contents initialized with a copy of the null-terminated character string pointed to by s. 
			//The length of the string is determined by the first null character. 
			//The behavior is undefined if s does not point at an array of at least Traits::length(s)+1 elements of CharT, 
			//including the case when s is a null pointer.
			//linear in length of s
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
			//C++17
			//Constructs the string with the contents of the range [first, last). 
			//If InputIt is an integral type, equivalent to basic_string(static_cast<size_type>(first), static_cast<value_type>(last), a).
			//linear in distance between first and last
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
		basic_string(const basic_string& other) :basic_string(other.alloc)
		{
			//C++17
			//Copy constructor. Constructs the string with the copy of the contents of other.
			//linear in size of other
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
			//C++17
			//Copy constructor.
			//Constructs the string with the copy of the contents of other.
			//linear in size of other
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
		basic_string(basic_string&& other) noexcept : basic_string(other.alloc)
		{
			//C++17
			//Move constructor. 
			//Constructs the string with the contents of other using move semantics.
			//other is left in valid, but unspecified state.
			//constant. If alloc is given and alloc != other.get_allocator(), then linear
			//Throws nothing if alloc == str.get_allocator()
			::memcpy(reinterpret_cast<char*>(this), reinterpret_cast<char*>(&other), sizeof(basic_string));
			other.maxsize = sso_size - 1;
			other.clear();
		}
		basic_string(basic_string&& other, const Allocator& alloc)noexcept(allocator_traits<Allocator>::is_always_equal::value) :basic_string(alloc)
		{
			//C++17
			//Move constructor. 
			//Constructs the string with the contents of other using move semantics.
			//other is left in valid, but unspecified state.
			//constant. If alloc is given and alloc != other.get_allocator(), then linear
			//Throws nothing if alloc == str.get_allocator()
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
			//C++17
			// Constructs the string with the contents of the initializer list init.
			// linear in size of init
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
			//C++17
			// Constructs the string with the contents of the string view sv,
			//as if by basic_string(sv.data(), sv.size(), alloc)
			//linear in size of sv
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
			typename = std::enable_if_t<std::is_convertible_v<const T&, basic_string_view<CharT, Traits>>>>
		basic_string(const T& t, size_t pos, size_t n,const Allocator& alloc = Allocator()) :basic_string(alloc)
		{
			//C++17
			//Implicitly converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t;, 
			//then initializes the string with the subrange [pos, pos + n) of sv as if by basic_string(sv.substr(pos, n), a).
			//This overload only participates in overload resolution if
			//std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> is true.
			//n
			try
			{
				assign(view(t), pos, n);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		
		//C++17
		//Replaces the contents of the string.
		//If the operation would result in size() > max_size(), throws std::length_error.
		//If an exception is thrown for any reason, this function has no effect (strong exception guarantee).
		basic_string& operator=(const basic_string& str)
		{
			//C++17
			//Replaces the contents with a copy of str.
			//If *this and str are the same object, this function has no effect.
			// linear in size of str
			if (this == &str) //相等滚蛋
				return *this;
			assign_str(str.data(), str.size());
			return *this;
		}
		basic_string& operator=(basic_string&& str)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			//我不知道这是什么傻逼标准，反正C++17这么说的
			//Replaces the contents with those of str using move semantics.
			//str is in a valid but unspecified state afterwards.
			//If std::allocator_traits<Allocator>::propagate_on_container_move_assignment()(std::allocator_traits没有这个函数，只有这个类型) is true, 
			//the target allocator is replaced by a copy of the source allocator.
			//If it is false and the source and the target allocators do not compare equal,
			//the target cannot take ownership of the source memory and must assign each character individually,
			//allocating additional memory using its own allocator as needed.Unlike other container move assignments, 
			//references, pointers, and iterators to str may be invalidated.
			//linear in the size of this (formally, each CharT has to be destroyed). 
			//If allocators do not compare equal and do not propagate, then also linear in the size of str (copy must be made)
			if (this == &str)
				return *this;
			if(str.sso()) //如果sso数据的话直接拷贝
			{
				Traits::copy(str.data(), data(), str.size());
				Traits::assign(str.data() + str.size(), Traits::eof());
				ssize = str.size();
				if constexpr(allocator_traits<Allocator>::propagate_on_container_move_assignment::value)
					alloc = str.alloc;
				return *this;
			}
			if constexpr(allocator_traits<Allocator>::propagate_on_container_move_assignment::value)
			{ //直接拷贝allocator
				clear();
				alloc = str.alloc;
				this->no_sso_data = str.no_sso_data;
				this->maxsize = str.maxsize;
				str.maxsize = sso_size - 1;
			}
			else
			{
				if (alloc == str.alloc)
				{
					clear();
					this->no_sso_data = str.no_sso_data;
					this->maxsize = str.maxsize;
					str.maxsize = sso_size - 1;
				}
				else
					//allocator不同转复制
					return this->operator=(str);
			}
			ssize = str.size();
			return *this;		
		}
		basic_string& operator=(const CharT* s)
		{
			//C++17
			//Replaces the contents with those of null - terminated character string pointed to by s as if by *this = basic_string(s), 
			//which involves a call to Traits::length(s).
			// linear in size of s
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
			//constant.
			Traits::assign(*data(), ch);
			Traits::assign(*(data() + 1), Traits::eof());
			ssize = 1;
			return *this;
		}
		basic_string& operator=(std::initializer_list<CharT> str)
		{
			//C++17
			//Replaces the contents with those of the initializer list ilist as if by *this = basic_string(ilist)
			//linear in size of init
			assign_iterator(str.begin(), str.end(), str.size());
			return *this;
		}
		basic_string& operator=(basic_string_view<CharT, Traits> sv)
		{
			//C++17
			//Replaces the contents with those of the string view sv as if by assign(sv)
			//linear in size of sv
			assign_str(sv.data(), sv.size());
			return *this;
		}

		//C++17
		//Replaces the contents of the string.
		//If an exception is thrown for any reason, this function has no effect (strong exception guarantee).
		//If the operation would result in size() > max_size(), throws std::length_error.
		basic_string& assign(size_t count, CharT ch)
		{
			//C++17
			// Replaces the contents with count copies of character ch.
			//linear in count
			if (capacity() >= count)
			{
				Traits::assign(data(), ch, count);
				Traits::assign(*(data() + count), Traits::eof());
			}
			else
			{
				auto p = make_no_sso(count);
				
				try
				{
					Traits::assign(p, ch, count);
					Traits::assign(*(p + count), Traits::eof());
				}
				catch (...)
				{
					allocator_traits<Allocator>::deallocate(alloc, p, count + 1);
					throw;
				}
				
				deal_no_sso();

				no_sso_data = p;
				maxsize = count;
			}
			ssize = count;
			return *this;
		}
		basic_string& assign(const basic_string& str)
		{
			//C++17
			//Replaces the contents with a copy of str
			//linear in size of str
			return operator=(str);
		}
		basic_string& assign(const basic_string& str,size_t pos,size_t count = npos)
		{
			//C++17
			//Replaces the contents with a substring [pos, pos+count) of str. 
			//If the requested substring lasts past the end of the string, or if count == npos, 
			//the resulting substring is [pos, str.size()). If pos > str.size(), std::out_of_range is thrown.
			//linear in count
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
			//constant. If alloc is given and alloc != other.get_allocator(), then linear.
			return this->operator=(std::move(str));
		}
		basic_string& assign(const CharT* s,size_t count)
		{
			//C++17
			//Replaces the contents with the first count characters of character string pointed to by s.
			//s can contain null characters.
			// linear in count
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
			// linear in size of s
			return this->operator=(s);
		}
		template< typename InputIt,typename  = is_input<value_type,InputIt>>
		basic_string& assign(InputIt first, InputIt last)
		{
			//C++17
			//Replaces the contents with copies of the characters in the range [first, last). 
			//This overload does not participate in overload resolution if InputIt does not satisfy InputIterator. 
			// linear in distance between first and last
			assign_iterator(first, last, distance(first, last));
			return *this;
		}
		basic_string& assign(std::initializer_list<CharT> ilist)
		{
			//C++17
			//Replaces the contents with those of the initializer list ilist.
			//inear in size of init
			return this->operator=(ilist);
		}
		basic_string& assign(basic_string_view<CharT, Traits> sv)
		{
			//C++17
			//Replaces the contents with those of the string view sv, 
			//as if by assign(sv.data(), sv.size())
			//linear in size of sv
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
		
		//C++17
		//Returns the allocator associated with the string.
		//Constant
		allocator_type get_allocator() const noexcept(noexcept(std::is_nothrow_copy_constructible_v<Allocator>))
		{
			//C++17
			//Returns the allocator associated with the string.
			return alloc;
		}

		//C++17
		//Returns a reference to the character at specified location pos. Bounds checking is performed, 
		//exception of type std::out_of_range will be thrown on invalid access.
		//Constant
		//Throws std::out_of_range if pos >= size().
		reference       at(size_type pos)
		{
			if (pos >= size())
				throw(out_of_range(""));
			return this->operator[](pos);
		}
		const_reference at(size_type pos) const
		{
			if (pos >= size())
				throw(out_of_range(""));
			return this->operator[](pos);
		}

		//C++17
		//If pos == size(), a reference to the character with value CharT() (the null character) is returned.
		//For the first(non - const) version, the behavior is undefined if this character is modified to any value other than charT()
		//Constant
		reference  operator[](size_type pos)noexcept
		{
			return *(data() + pos);
		}
		const_reference operator[](size_type pos) const noexcept
		{
			return *(data() + pos);
		}

		//C++17
		//Returns reference to the first character in the string. The behavior is undefined if empty() == true.
		//Constant
		CharT& front()noexcept
		{
			return *data();
		}
		const CharT& front() const noexcept
		{
			return *data();
		}

		//C++17
		//Returns reference to the last character in the string. The behavior is undefined if empty() == true.
		//Constant
		CharT& back()noexcept
		{
			return *(data()+(size()-1));
		}
		const CharT& back() const noexcept
		{
			return *(data() + (size() - 1));
		}

		//C++17
		//Returns a pointer to the underlying array serving as character storage.
		//The pointer is such that the range [data(); data() + size()) is valid and the values in it correspond to the values stored in the string.
		//The returned array is null-terminated, that is, data() and c_str() perform the same function.
		//If empty() returns true, the pointer points to a single null character.
		//The pointer obtained from data() may be invalidated by:
		//Passing a non - const reference to the string to any standard library function, or
		//	Calling non - const member functions on the string, excluding operator[](), at(), front(), back(), begin(), end(), rbegin(), rend().
		//	1) Modifying the character array accessed through the const overload of data has undefined behavior.
		//	2) Modifying the past - the - end null terminator stored at data() + size() has undefined behavior.
		//Constant
		const CharT* data() const noexcept
		{
			return c_str();
		}
		CharT* data()noexcept
		{
			if (sso())
				return sso_data;
			else
				return no_sso_data;
		}

		//C++17
		//c_str() and data() perform the same function.
		//Constant
		const CharT* c_str() const noexcept
		{
			if (sso())
				return sso_data;
			else
				return no_sso_data;
		}

		//C++17
		//Returns a std::basic_string_view, constructed as if by std::basic_string_view<CharT, Traits>(data(), size())
		operator basic_string_view<CharT, Traits>() const noexcept
		{
			return view(data(), size());
		}

		//C++17
		//Returns an iterator to the first character of the string.
		//begin() returns a mutable or constant iterator, depending on the constness of *this.
		//cbegin() always returns a constant iterator.It is equivalent to const_cast<const basic_string&>(*this).begin().
		//Constant
		iterator begin()noexcept
		{
			return iterator(data());
		}
		const_iterator begin() const noexcept
		{
			return const_cast<const basic_string&>(*this).begin();
		}
		const_iterator cbegin() const noexcept
		{
			return const_cast<const basic_string&>(*this).begin();
		}

		//C++17
		//Returns an iterator to the character following the last character of the string.
		//This character acts as a placeholder, attempting to access it results in undefined behavior.
		//Constant
		iterator end() noexcept
		{
			return iterator(data()+size());
		}
		const_iterator end() const noexcept
		{
			return const_cast<const basic_string&>(*this).end();
		}
		const_iterator cend() const noexcept
		{
			return const_cast<const basic_string&>(*this).end();
		}

		//C++17
		//Checks if the string has no characters, i.e. whether begin() == end().
		//Constant
		bool empty() const noexcept
		{
			return size() == 0;
		}

		//C++17
		//Returns the number of CharT elements in the string, i.e. std::distance(begin(), end()).
		//For std::string, the elements are bytes (objects of type char), which are not the same as characters if a multibyte encoding such as UTF-8 is used.
		//Constant
		size_type size() const noexcept
		{
			return ssize;
		}
		size_type length() const noexcept
		{
			return ssize;
		}

		//C++17
		//Returns the maximum number of elements the string is able to hold due to system or library implementation limitations, i.e. ​std::distance(begin(), end())​ for the largest string.
		//Constant
		size_type max_size() const noexcept
		{
			return allocator_traits<Allocator>::max_size();
		}

		//C++17
		//Informs a std::basic_string object of a planned change in size, so that it can manage the storage allocation appropriately.
		//If new_cap is greater than the current capacity(), new storage is allocated, and capacity() is made equal or greater than new_cap.
		//	If new_cap is less than the current capacity(), this is a non - binding shrink request.
		//	If new_cap is less than the current size(), this is a non - binding shrink - to - fit request equivalent to shrink_to_fit() (since C++11).
		//	If a capacity change takes place, all iterators and references, including the past - the - end iterator, are invalidated.
		//At most linear in the size() of the string
		//Throws std::length_error if new_cap is greater than max_size()
		//May throw any exceptions thrown by std::allocator_traits<Allocator>::allocate(), such as std::bad_alloc.
		void reserve(size_type _new = 0)
		{
			if (_new <= capacity())
				return;
			else
			{
				auto k = new_cap(_new);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), size());
					Traits::assign(*(temp + size()), Traits::eof());
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}

				deal_no_sso();

				no_sso_data = temp;
				maxsize = k;
			}
		}

		//C++17
		//Returns the number of characters that the string has currently allocated space for.
		//Constant
		size_type capacity() const noexcept
		{
			return maxsize;
		}

		//C++17
		//Requests the removal of unused capacity.
		//It is a non - binding request to reduce capacity() to size().It depends on the implementation if the request is fulfilled.
		//If reallocation takes place, all pointers, references, and iterators are invalidated.
		//Constant
		//有毛病吧，shrink_to_fit()复杂度竟然要求是Constant，标准委员会食屎了！
		void shrink_to_fit()
		{
			//其他都做不了了，吃屎的常数复杂度
			//其实我是觉得这个不是很重要233333
			if(!sso() && size()==0)
			{
				allocator_traits<Allocator>::deallocate(alloc, data(), maxsize + 1);
				maxsize = sso_size - 1;
				Traits::assign(*data(), Traits::eof());
			}
		}

		//C++17
		//Removes all characters from the string as if by executing erase(begin(), end()).
		//All pointers, references, and iterators are invalidated.
		//Unlike for std::vector::clear, the C++ standard does not explicitly require that capacity is unchanged by this function, but existing implementations do not change capacity.
		//Linear in the size of the string.
		void clear()noexcept
		{
			Traits::assign(*data(), Traits::eof());
			ssize = 0;
		}

		//C++17
		//Inserts characters into the string.
		//In any case, if an exception is thrown for any reason, this function has no effect (strong exception guarantee).
		basic_string& insert(size_type index, size_type count, CharT ch)
		{
			//C++17
			//Inserts count copies of character ch at the position index
			//std::out_of_range if index > size() and std::length_error if size() + count > max_size().
			if (index > size())
				throw(out_of_range(""));

			if(size()+count <= capacity())
			{
				Traits::move(data() + index + count, data() + index, size() - index);
				Traits::assign(data()+index, ch, count);
				Traits::assign(*(data() + size()), Traits::eof);
			}
			else
			{
				auto k = new_cap(size() + count);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), index);
					Traits::assign(data() + index, ch, count);
					Traits::move(temp + index + count, data() + index, size() - index);
					Traits::assign(*(temp + size()), Traits::eof);
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}

				deal_no_sso();

				no_sso_data = temp;
				maxsize = k;
			}
			ssize += count;
			return *this;
		}
		basic_string& insert(size_type index, const CharT* s)
		{
			//C++17
			//Inserts null-terminated character string pointed to by s at the position index. 
			//The length of the string is determined by the first null character (effectively calls Traits::length(s).
			//std::out_of_range if index > size() and std::length_error if size() + Traits::length(s) > max_size().
			if(s)
				insert_str(index, s, Traits::length(s));
			return *this;
		}
		basic_string& insert(size_type index, const CharT* s, size_type count)
		{
			//C++17
			//  Inserts string str at the position index
			//std::out_of_range if index > size() and std::length_error if size() + str.size() > max_size().
			if(s)
				insert_str(index, s, count);
			return *this;
		}
		basic_string& insert(size_type index, const basic_string& str)
		{
			//C++17
			//  Inserts string str at the position index
			//std::out_of_range if index > size() and std::length_error if size() + str.size() > max_size().
			return insert(index, str.data(), str.size());
		}
		basic_string& insert(size_type index, const basic_string& str,size_type index_str, size_type count = npos)
		{
			//C++17
			//Inserts a string, obtained by str.substr(index_str, count) at the position index
			// Throws exceptions on the following conditions:
			//  a) std::out_of_range if index > size().
			//	b) std::out_of_range if index_str > str.size().
			//	c) std::length_error if size() + ins_count > max_size() where ins_count is the number of characters that will be inserted.
			return insert(index, str.data() + index_str, count < str.size() - index ? count : str.size() - index);
		}
		iterator insert(const_iterator pos, CharT ch)
		{
			//C++17
			//Inserts character ch before the character pointed by pos
			insert(pos - begin(), 1, ch);
			return pos;
		}
		iterator insert(const_iterator pos, size_type count, CharT ch)
		{
			//C++17
			//Inserts count copies of character ch before the element pointed by pos
			insert(pos - begin(), count, ch);
			return pos;
		}
		template< typename InputIt, typename = is_input<value_type, InputIt>>
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			//C++17
			//Inserts count copies of character ch before the element pointed by pos
			//Inserts characters from the range [first, last) before the element pointed by pos. 
			//This overload does not participate in overload resolution if InputIt does not satisfy InputIterator. (since C++11)
			insert_iterator(pos - begin(), first, last, distance(first, last));
			return pos;
		}
		iterator insert(const_iterator pos, std::initializer_list<CharT> ilist)
		{
			//C++17
			//Inserts elements from initializer list ilist before the element pointed by pos
			insert_iterator(pos - begin(), ilist.begin(), ilist.end(), ilist.size());
			return pos;
		}
		basic_string& insert(size_type pos, basic_string_view<CharT, Traits> sv)
		{
			//C++17
			// Inserts the elements from the string view sv before the element pointed by pos, as if by insert(pos, sv.data(), sv.size())
			// std::out_of_range if index > size() and std::length_error if size() + sv.size() > max_size().
			return insert(pos, sv.data());
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, basic_string_view<CharT, Traits>>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string& insert(size_type index, const T& t,size_type index_str, size_type count = npos)
		{
			//C++17
			//Converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t; ,
			//then inserts, before the element pointed by pos, the characters from the subview[index_str, index_str + count) of sv.
			//If the requested subview lasts past the end of sv, or if count == npos, the resulting subview is[index_str, sv.size()).
			//If index_str > sv.size(), or if index > size(), 
			//std::out_of_range is thrown.This overload only participates in overload resolution if 
			//std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> is true and 
			//std::is_convertible_v<const T&, const CharT*> is false.
			//Throws exceptions on the following conditions:
			//a) std::out_of_range if index > size().
			//b) std::out_of_range if index_str > sv.size().
			//c) std::length_error if size() + ins_count > max_size() where ins_count is the number of characters that will be inserted.
			auto str = view(t);
			if (index_str > str.size())
				throw(out_of_range(""));
			insert_str(str.data() + index_str, str.size() - index_str > count ? count : str.size() - index_str);
			return *this;
		}

		//C++17
		//Removes specified characters from the string.
		//In any case, if an exception is thrown for any reason, this function has no effect (strong exception guarantee)
		basic_string& erase(size_type index = 0, size_type count = npos)
		{
			//C++17
			// Removes min(count, size() - index) characters starting at index.
			//std::out_of_range if index > size().
			if (index > size())
				throw(out_of_range(""));
			count = count < size() - index ? count : size() - index;
			Traits::move(data() + index, data() + count, size() - index - count);
			Traits(*(data() + size() - count), Traits::eof());
			ssize -= count;
			return *this;
		}
		iterator erase(const_iterator position)noexcept
		{
			// C++17
			// Removes the character at position.
			Traits::move(position.dat, position.dat + 1, end() - position);
			Traits(*(data() + size() - 1), Traits::eof());
			--ssize;
			return position;
		}
		iterator erase(const_iterator first, const_iterator last)noexcept
		{
			// C++17
			// Removes the characters in the range [first, last).
			auto k = last - first;
			Traits::move(first.dat, last.dat, end() - last);
			Traits(*(data() + size() - k), Traits::eof());
			ssize -= k;
			return first;
		}

		//C++17
		//Appends the given character ch to the end of the string.
		//Amortized constant.
		//If an exception is thrown for any reason, this function has no effect (strong exception guarantee). (since C++11)
		//If the operation would result in size() > max_size(), throws std::length_error.
		void push_back(CharT ch)
		{
			reserve(size() + 1);
			Traits::assign(at(size()), ch);
			Traits::assign(at(size() + 1), Traits::eof());
			++ssize;
		}

		//C++17
		//Removes the last character from the string.
		//Equivalent to erase(size() - 1, 1), except the behavior is undefined if the string is empty.
		//所以一个空的pop_back，不要怪我炸
		//Constant
		void pop_back()noexcept
		{
			Traits::assign(at(size()), Traits::eof());
			--ssize;
		}

		// C++17
		//Appends additional characters to the string.
		//There are no standard complexity guarantees, typical implementations behave similar to std::vector::insert.
		//If an exception is thrown for any reason, this function has no effect (strong exception guarantee). (since C++11)
		//If the operation would result in size() > max_size(), throws std::length_error.
		basic_string& append(size_type count, CharT ch)
		{
			// C++17
			//Appends count copies of character ch
			if(size()+count <= capacity())
			{
				Traits::assign(data() + size(), ch, count);
				Traits::assign(data() + size() + count, Traits::eof());
			}
			else
			{
				
				auto k = new_cap(size() + count);
				auto temp = make_no_sso(k);

				try
				{
					Traits::copy(temp, data(), size());
					Traits::assign(temp + size(), ch, count);
					Traits::assign(temp + size() + count, Traits::eof());
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}

				deal_no_sso();

				no_sso_data = temp;
				maxsize = k;
			}
			ssize += count;
			return *this;
		}
		basic_string& append(const basic_string& str)
		{
			// C++17
			//Appends string str
			append_str(str.data(), str.size());
			return *this;
		}
		basic_string& append(const basic_string& str,size_type pos,size_type count = npos)
		{
			// C++17
			//Appends a substring [pos, pos+count) of str. 
			//If the requested substring lasts past the end of the string, or if count == npos, the appended substring is [pos, size()). 
			//If pos >= str.size(), std::out_of_range is thrown.
			if (pos >= str.size())
				throw(out_of_range(""));
			return append( str.data() + pos, count < str.size() - pos ? count : str.size() - pos);
		}
		basic_string& append(const CharT* s, size_type count)
		{
			// C++17
			//Appends the first count characters of character string pointed to by s. 
			//s can contain null characters.
			if(s)
				append_str(s, count);
			return *this;
		}
		basic_string& append(const CharT* s)
		{
			// C++17
			//Appends the null-terminated character string pointed to by s. 
			//The length of the string is determined by the first null character.
			if(s)
				append_str(s, Traits::length(s));
			return *this;
		}
		template< typename InputIt, typename = is_input<value_type, InputIt>>
		basic_string& append(InputIt first, InputIt last)
		{
			// C++17
			// Appends characters in the range [first, last). 
			//This overload has the same effect as overload (1) if InputIt is an integral type.
			append_iterator(first, last, distance(first, last));
			return *this;
		}
		basic_string& append(std::initializer_list<CharT> ilist)
		{
			//C++17
			//Appends characters from the initializer list ilist.
			append_iterator(ilist.begin(), ilist.end(), ilist.size());
			return *this;
		}
		basic_string& append(basic_string_view<CharT, Traits> sv)
		{
			//C++17
			//Appends all characters from the string view sv as if by append(sv.data(), sv.size())
			append_str(sv.data(), sv.size());
			return *this;
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, basic_string_view<CharT, Traits>>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string& append(const T& t, size_type pos,size_type count = npos)
		{
			//C++17
			//Converts t to a string view sv as if by std::basic_string_view<charT, traits> sv = t;,
			//then appends the characters from the subview [pos, pos+count) of sv. 
			//If the requested subview extends past the end of sv, or if count == npos, the appended subview is [pos, sv.size()). 
			//If pos >= sv.size(), std::out_of_range is thrown. 
			//This overload only participates in overload resolution if 
			//std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> is true and 
			//std::is_convertible_v<const T&, const CharT*> is false.
			auto str = basic_string_view<CharT, Traits>(t);
			if (pos > str.size())
				throw(out_of_range(""));
			append_str(str.data() + pos, str.size() - pos > count ? count : str.size() - pos);
			return *this;
		}

		//C++17
		//same as append()
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

		//C++17
		//Compares two character sequences.
		int compare(const basic_string& str) const noexcept
		{
			//C++17
			// Compares this string to str. 
			//First, calculates the number of characters to compare, as if by size_type rlen = std::min(size(), str.size()). 
			//Then compares by calling Traits::compare(data(), str.data(), rlen). 
			//For standard strings this function performs character-by-character lexicographical comparison. 
			//If the result is zero (the strings are equal so far), then their sizes are compared as follows:
			return _compare(data(), size(), str.data(), str.size());
		}
		int compare(size_type pos1, size_type count1,const basic_string& str) const
		{
			//C++17
			//Compares a[pos1, pos1 + count1) substring of this string to str as if by 
			//compare(pos1, count1, std::basic_string_view<CharT, Traits>(str)) 

			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, str.data(), str.size());

		}
		int compare(size_type pos1, size_type count1,const basic_string& str,
												size_type pos2, size_type count2 = npos) const noexcept
		{
			//C++17
			//Compares a [pos1, pos1+count1) substring of this string to a substring [pos2, pos2+count2) of str as if by
			//compare(pos1, count1, std::basic_string_view<CharT, Traits>(str), pos2, count2)

			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1,
							str.data() + pos2, str.size() - pos2 > count2 ? count2 : str.size() - pos2);

		}
		int compare(const CharT* s) const noexcept
		{
			//C++17
			//Compares a [pos1, pos1+count1) substring of this string to a substring [pos2, pos2+count2) of str as if by
			//compare(basic_string(s))
			return _compare(data(), size(), s, Traits::length(s));
		}
		int compare(size_type pos1, size_type count1,const CharT* s) const noexcept
		{
			//C++17
			//Compares a [pos1, pos1+count1) substring of this string to the null-terminated character sequence beginning at the character pointed to by s, 
			//as if by basic_string(*this, pos, count1).compare(basic_string(s))
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, s, Traits::length(s));
		}
		int compare(size_type pos1, size_type count1,const CharT* s, size_type count2) const noexcept
		{
			//C++17
			//Compares a [pos1, pos1+count1) substring of this string to the first count2 characters of the character array whose first character is pointed to by s,
			//as if by basic_string(*this, pos, count1).compare(basic_string(s, count2)). 
			//(Note: the characters from s to s+count2 may include null characters))
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, s, count2);
		}
		int compare(std::basic_string_view<CharT, Traits> sv) const
		{
			//C++17
			// Compares this string to sv, similar to (1) except for using sv.size() and sv.data() instead of str.size() and str.data()
			return _compare(data(), size(), sv.data(), sv.size());
		}
		int compare(size_type pos1, size_type count1,basic_string_view<CharT, Traits> sv) const noexcept
		{
			//C++17
			// Compares a [pos1, pos1+count1) substring of this string to sv, as if by 
			//std::basic_string_view<CharT, Traits>(data(), size()).substr(pos1, count1).compare(sv)
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1, sv, sv.size());
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, basic_string_view<CharT, Traits>>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		int compare(size_type pos1, size_type count1,const T& t,
				size_type pos2, size_type count2 = npos) const
		{
			//C++17
			// Converts t to a string view sv as if by std::basic_string_view<CharT, Traits> sv = t;, 
			//then compares a [pos1, pos1+count1) substring of this string to a substring [pos2, pos2+count2) of sv as if by 
			//std::basic_string_view<CharT, Traits>(data(), size()).substr(pos1, count1).compare(sv.substr(pos2, count2));. 
			//This overload only participates in overload resolution if 
			//std::is_convertible_v<const T&, std::basic_string_view<CharT, Traits>> is true and 
			//std::is_convertible_v<const T&, const CharT*> is false.
			auto str = view(t);
			return _compare(data() + pos1, size() - pos1 > count1 ? count1 : size() - pos1,
				str.data() + pos2, str.size() - pos2 > count2 ? count2 : str.size() - pos2);
		}

		//C++17
		//Replaces the part of the string indicated by either [pos, pos + count) or [first, last) with a new string.
		//The new string can be one of :
		//In any case, if an exception is thrown for any reason, this function has no effect (strong exception guarantee). 
		basic_string& replace(size_type pos, size_type count,const basic_string& str)
		{
			return replace(pos, count, str.data(), str.size());
		}
		basic_string& replace(const_iterator first, const_iterator last,const basic_string& str)
		{
			return replace(first - begin(), distance(last - first), str.data(), str.size());
		}
		basic_string& replace(size_type pos, size_type count,const basic_string& str,size_type pos2, size_type count2 = npos)
		{
			if (pos2 > str.size())
				throw(out_of_range(""));

			if (count2 == npos)
				count2 = str.size() - pos2;
			else if (count2 >str.size() - pos2)
				count2 = size() - pos2;

			return replace(pos, count, str.data() + pos2, count2);
		}
		template< typename InputIt, typename = is_input<value_type, InputIt>>
		basic_string& replace(const_iterator first, const_iterator last,InputIt first2, InputIt last2)
		{
			_repalce(first, last, first2, last2, distance(first2, last2));
			return *this;
		}
		basic_string& replace(size_type pos, size_type count,const CharT* cstr, size_type count2)
		{
			if(size()-count+count2 <= capacity())
			{
				Traits::move(data() + pos + count2, data() + pos + count, size() - pos - count);
				Traits::copy(data() + pos, cstr, count2);
				Traits::assign(at(size() - count + count2), Traits::eof());
			}
			else
			{
				auto k = new_cap(size() - count + count2);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), pos);
					Traits::copy(temp + pos, cstr, count2);
					Traits::copy(temp + pos + count2, data() + pos + count, size() - pos - count);
					Traits::assign(*(temp + size() - count + count2), Traits::eof());
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}
				allocator_traits<Allocator>::deallocate(alloc, no_sso_data, maxsize + 1);

				no_sso_data = temp;
				maxsize = k;
			}
			ssize = ssize - count + count2;
			return *this;
		}
		basic_string& replace(const_iterator first, const_iterator last,const CharT* cstr, size_type count2)
		{
			return replace(first - begin(), distance(last - first), cstr, count2);
		}
		basic_string& replace(size_type pos, size_type count,const CharT* cstr)
		{
			return replace(pos, count, cstr, Traits::length(cstr));
		}
		basic_string& replace(const_iterator first, const_iterator last,const CharT* cstr)
		{
			return replace(first - begin(), distance(last - first), cstr, Traits::length(cstr));
		}
		basic_string& replace(size_type pos, size_type count,size_type count2, CharT ch)
		{
			if (size() - count + count2 <= capacity())
			{
				Traits::move(data() + pos + count2, data() + pos + count, size() - pos - count);
				Traits::assign(data() + pos, ch, count2);
				Traits::assign(at(size() - count + count2), Traits::eof());
			}
			else
			{
				auto k = new_cap(size() - count + count2);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), pos);
					Traits::assign(data() + pos, ch, count2);
					Traits::copy(temp + pos + count2, data() + pos + count, size() - pos - count);
					Traits::assign(*(temp + size() - count + count2), Traits::eof());
				}
				catch (...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}
				allocator_traits<Allocator>::deallocate(alloc, no_sso_data, maxsize + 1);

				no_sso_data = temp;
				maxsize = k;
			}
			ssize = ssize - count + count2;
			return *this;
		}
		basic_string& replace(const_iterator first, const_iterator last,size_type count2, CharT ch)
		{
			return replace(first - begin(), distance(last - first), count2, ch);
		}
		basic_string& replace(const_iterator first, const_iterator last,std::initializer_list<CharT> ilist)
		{
			return _repalce(first, last, ilist.begin(), ilist.end(),ilist.size());
		}
		basic_string& replace(size_type pos, size_type count,view sv)
		{
			return replace(pos, count, sv, sv.data());
		}
		basic_string& replace(const_iterator first, const_iterator last,view sv)
		{
			return replace(first - begin(), distance(last - first), sv.data(), sv.size());
		}
		template < typename T,
			typename = std::enable_if_t<std::is_convertible_v<const T&, view>
			&& !std::is_convertible_v<const T&, const CharT*>>>
		basic_string& replace(size_type pos, size_type count, const T& t,size_type pos2, size_type count2 = npos)
		{
			auto k = view(t);

			if (pos2 > k.data())
				throw(out_of_range(""));

			if (count2 == npos)
				count2 = k.size() - pos2;
			else if (count2 > k.size() - pos2)
				count2 = k.size() - pos2;

			return replace(pos, count, k.data() + pos2, count2);
		}


		basic_string substr(size_type pos = 0,size_type count = npos) const
		{
			if (pos > size())
				throw(out_of_range(""));

			return basic_string(data() + pos, count < size() - pos ? count : size() - pos);
		}

		size_type copy(CharT* dest,size_type count,size_type pos = 0) const
		{
			if (pos > size())
				throw(out_of_range(""));

			count = count < size() - pos ? count : size() - pos;
			Traits::copy(dest, data() + pos, count);
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

		size_type find(const basic_string& str, size_type pos = 0) const
		{
			return find(str.data(), pos, str.size());
		}
		size_type find(const CharT* s, size_type pos, size_type count) const
		{
			if (pos > size())
				throw(out_of_range(""));
			if (count > size())
				return npos;

			auto k = _find(data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		size_type find(const CharT* s, size_type pos = 0) const
		{
			return find(s, pos, Traits::length(s));
		}
		size_type find(CharT ch, size_type pos = 0) const
		{
			return Traits::find(data() + pos, size() - pos, ch) - data();
		}
		size_type find(view sv,size_type pos = 0) const
		{
			return find(sv.data(), pos, sv.size());
		}

		size_type rfind(const basic_string& str, size_type pos = npos) const
		{
			return rfind(str.data(), pos, str.size());
		}
		size_type rfind(const CharT* s, size_type pos, size_type count) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));
			if (count > size())
				return npos;

			auto k = _rfind(data(), data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		size_type rfind(const CharT* s, size_type pos = npos) const
		{
			return rfind(s, pos, Traits::length(s));
		}
		size_type rfind(CharT ch, size_type pos = npos) const
		{
			if (pos == npos)
				pos = size();

			for (auto i = data() + pos - 1; i != data(); --i)
				if (Traits::eq(*i, ch))
					return i - data();
			return Traits::eq(*data(), ch) ? 0 : npos;
		}
		size_type rfind(view sv,size_type pos = npos) const
		{
			return rfind(sv.data(), pos, sv.size());
		}

		size_type find_first_of(const basic_string& str, size_type pos = 0) const
		{
			return find_first_of(str.data(), pos, str.size());
		}
		size_type find_first_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_first_of(data() + pos, s,count);
			if (k)
				return k - data();
			return npos;
		}
		size_type find_first_of(const CharT* s, size_type pos = 0) const
		{
			return find_first_of(s, pos, Traits::length(s));
		}
		size_type find_first_of(CharT ch, size_type pos = 0) const
		{
			return find(ch, pos);
		}
		size_type find_first_of(view sv,size_type pos = 0) const
		{
			return find_first_of(sv, pos, sv.size());
		}

		size_type find_first_not_of(const basic_string& str, size_type pos = 0) const
		{
			return find_first_not_of(str, pos, str.size());
		}
		size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_first_not_of(data() + pos, s,count);
			if (k)
				return k - data();
			return npos;
		}
		size_type find_first_not_of(const CharT* s, size_type pos = 0) const
		{
			return find_first_not_of(s, pos, Traits::length(s));
		}
		size_type find_first_not_of(CharT ch, size_type pos = 0) const
		{
			if (pos > size())
				throw(out_of_range(""));

			for(auto i =0;i!=size();++i)
			{
				if (Traits::eq(at(i), ch))
					goto ll;
				return i;
ll:				;
			}
			return npos;
				
		}
		size_type find_first_not_of(view sv,size_type pos = 0) const
		{
			return find_first_not_of(sv, pos, sv.size());
		}

		size_type find_last_of(const basic_string& str, size_type pos = npos) const
		{
			return find_last_of(str.data(), pos, str.size());
		}
		size_type find_last_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_last_of(data(), data() +pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		size_type find_last_of(const CharT* s, size_type pos = npos) const
		{
			return find_last_of(s, pos, Traits::length(s));
		}
		size_type find_last_of(CharT ch, size_type pos = npos) const
		{
			return rfind(ch, pos);
		}
		size_type find_last_of(view sv,size_type pos = npos) const
		{
			return find_last_of(sv.data(), pos, sv.size());
		}

		size_type find_last_not_of(const basic_string& str, size_type pos = npos) const
		{
			return find_last_not_of(str.data(), pos, str.size());
		}
		size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));

			auto k = _find_last_not_of(data(), data() + pos, s, count);
			if (k)
				return k - data();
			return npos;
		}
		size_type find_last_not_of(const CharT* s, size_type pos = npos) const
		{
			return find_last_not_of(s, pos, Traits::length(s));
		}
		size_type find_last_not_of(CharT ch, size_type pos = npos) const
		{
			if (pos == npos)
				pos = size();

			if (pos > size())
				throw(out_of_range(""));

			for (auto i = size() - 1; i > 0; --i)
			{
				if (Traits::eq(at(i), ch))
					goto ll;
				return i;
			ll:;
			}
			return npos;
		}
		size_type find_last_not_of(view sv,size_type pos = npos) const
		{
			return find_last_not_of(sv.data(), pos, sv.size());
		}
		
		~basic_string()
		{
			if (!sso())
				deal_no_sso();
		}
	private:
		bool sso()const noexcept
		{
			return maxsize < sso_size;
		}

		size_t new_cap(size_t p)const noexcept
		{
			auto k = maxsize << 1;
			if (k < p)
				return p;
			if (k >= max_size())
				return p;
			return k;
		}

		CharT* make_no_sso(size_t p)
		{
			if (p >= max_size())
				throw(length_error(""));
			return allocator_traits<Allocator>::allocate(alloc, p + 1);
		}

		void deal_no_sso()
		{
			if (!sso())
				allocator_traits<Allocator>::deallocate(alloc, data(), maxsize + 1);
		}

		void assign_str(const_pointer p,size_t t)
		{
			if (capacity() >= t)//空间够了
			{
				Traits::copy(p, data(), t);
				Traits::assign(*(data() + t), Traits::eof());
			}
			else//空间不够
			{
				//先申请并复制，出错也安全
				auto pp = make_no_sso(t);
				try
				{
					Traits::copy(pp, p, t);
					Traits::assign(*(pp + t), Traits::eof());
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, pp, t + 1);
					throw;
				}

				//如果原来有内存资源就释放
				deal_no_sso();

				no_sso_data = pp;
				maxsize = t;
			}
			ssize = t;
		}
		template<typename It,typename = is_input<value_type,It>>
		void assign_iterator(It first,It end,size_t t)
		{
			if (capacity() >= t)//空间够了
			{
				uuz::copy(first, end, data());
				Traits::assign(*(data() + t), Traits::eof());	
			}
			else//空间不够
			{

				//先申请并复制，出错也安全
				auto p = make_no_sso(t);
				try
				{
					uuz::copy(first, end, p);
					Traits::assign(*(p + t), Traits::eof());
				}
				catch (...)
				{
					allocator_traits<Allocator>::deallocate(alloc, p, t + 1);
					throw;
				}

				//如果原来有内存资源就释放
				deal_no_sso();

				no_sso_data = p;
				maxsize = t;
			}
			ssize = t;
		}

		void insert_str(size_t index,const_pointer s ,size_t count)
		{
			if (index > size())
				throw(out_of_range(""));

			if (size() + count <= capacity())
			{
				Traits::move(data() + index + count, data() + index, size() - index);
				Traits::copy(data() + index, s, count);
				Traits::assign(*(data() + size()), Traits::eof);
			}
			else
			{
				auto k = new_cap(size() + count);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), index);
					Traits::copy(temp + index, s, count);
					Traits::move(temp + index + count, data() + index, size() - index);
					Traits::assign(*(temp + size()), Traits::eof);
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}

				deal_no_sso();

				no_sso_data = temp;
				maxsize = k;
			}
			ssize += count;
		}
		template<typename It, typename = is_input<value_type, It>>
		void insert_iterator(size_t index,It first, It end, size_t count)
		{
			if (index > size())
				throw(out_of_range(""));

			if (size() + count <= capacity())
			{
				Traits::move(data() + index + count, data() + index, size() - index);
				uuz::copy(first,end,data() + index);
				Traits::assign(*(data() + size()), Traits::eof);
			}
			else
			{
				auto k = new_cap(size() + count);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), index);
					uuz::copy(first, end, temp + index);
					Traits::move(temp + index + count, data() + index, size() - index);
					Traits::assign(*(temp + size()), Traits::eof);
				}
				catch (...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}
				
				deal_no_sso();
				
				no_sso_data = temp;
				maxsize = k;
			}
			ssize += count;
		}

		void append_str(const_pointer s, size_t count)
		{
			if (size() + count <= capacity())
			{
				Traits::copy(data() + size(), s, count);
				Traits::assign(data() + size() + count, Traits::eof());
			}
			else
			{
				auto k = new_cap(size() + count);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), size());
					Traits::copy(temp + size(), s, count);
					Traits::assign(temp + size() + count, Traits::eof());
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}

				deal_no_sso();

				no_sso_data = temp;
				maxsize = k;
			}
			ssize += count;
		}
		template<typename It, typename = is_input<value_type, It>>
		void append_iterator(It first, It end, size_t count)
		{
			if (size() + count <= capacity())
			{
				uuz::copy(first, end, data() + size());
				Traits::assign(data() + size() + count, Traits::eof());
			}
			else
			{
				auto k = new_cap(size() + count);
				auto temp = make_no_sso(k);

				try
				{
					Traits::copy(temp, data(), size());
					uuz::copy(first, end, temp + size());
					Traits::assign(temp + size() + count, Traits::eof());
				}
				catch(...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}

				deal_no_sso();

				no_sso_data = temp;
				maxsize = k;
			}
			ssize += count;
		}
		
		template<typename It, typename = is_input<value_type, It>>
		void _replace(const_iterator first, const_iterator last, It first2, It last2,size_type count2)
		{
			auto count = last - first;
			auto pos = first - begin();
			if (size() - count + count2 <= capacity())
			{
				Traits::move(data() + pos + count2, data() + pos + count, size() - pos - count);
				uuz::copy(first2, last2, data() + pos);
				Traits::assign(at(size() - count + count2), Traits::eof());
			}
			else
			{
				auto k = new_cap(size() - count + count2);
				auto temp = make_no_sso(k);

				try
				{
					Traits::move(temp, data(), pos);
					uuz::copy(first2, last2,temp + pos);
					Traits::copy(temp + pos + count2, data() + pos + count, size() - pos - count);
					Traits::assign(*(temp + size() - count + count2), Traits::eof());
				}
				catch (...)
				{
					allocator_traits<Allocator>::deallocate(alloc, temp, k + 1);
					throw;
				}
				allocator_traits<Allocator>::deallocate(alloc, no_sso_data, maxsize + 1);

				no_sso_data = temp;
				maxsize = k;
			}
			ssize = ssize - count + count2;
			return *this;
		}

	};
	
	
	using string = basic_string<char>;
	using string_view = basic_string_view<char>;

	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs,
													const basic_string<CharT, Traits, Alloc>& rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(lhs.size() + rhs.size());
		temp += lhs;
		temp += rhs;
		return temp;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(const CharT* lhs,
												const basic_string<CharT, Traits, Alloc>& rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(Traits::length(lhs) + rhs.size());
		temp += lhs;
		temp += rhs;
		return temp;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(CharT lhs,
												const basic_string<CharT, Traits, Alloc>& rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(1 + rhs.size());
		temp += lhs;
		temp += rhs;
		return temp;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs,
													const CharT* rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(Traits::length(rhs) + lhs.size());
		temp += lhs;
		temp += rhs;
		return temp;
	}
	template<class CharT, class Traits, class Alloc>
	basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs,CharT rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(Traits::length(lhs) + 1);
		temp += lhs;
		temp += rhs;
		return temp;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string<CharT, Traits, Alloc>&& lhs,
													const basic_string<CharT, Traits, Alloc>& rhs)
	{
		lhs += rhs;
		return std::move(lhs);
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs,
														basic_string<CharT, Traits, Alloc>&& rhs)
	{
		auto k = lhs.size() + rhs.size();
		if (k <= rhs.capacity())
			rhs.insert(rhs.begin(), lhs);
		else
			return lhs + rhs;
		return std::move(rhs);
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string<CharT, Traits, Alloc>&& lhs,
													basic_string<CharT, Traits, Alloc>&& rhs)
	{
		auto k = lhs.size() + rhs.size();
		if(k<=lhs.capacity())
		{
			lhs += rhs;
			return std::move(lhs);
		}
		else if(k<= rhs.capacity())
		{
			rhs.insert(rhs.begin(), lhs);
			return std::move(rhs);
		}
		
		return lhs + rhs;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(const CharT* lhs,basic_string<CharT, Traits, Alloc>&& rhs)
	{
		auto k = Traits::length(lhs) + rhs.size();
		if (k <= rhs.capacity())
		{
			rhs.insert(rhs.begin(), lhs);
			return std::move(rhs);
		}
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(k);
		temp += lhs;
		temp += rhs;
		return temp;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(CharT lhs,
												basic_string<CharT, Traits, Alloc>&& rhs)
	{
		auto k = Traits::length(lhs) + rhs.size();
		if (k <= rhs.capacity())
		{
			rhs.insert(rhs.begin(), lhs);
			return std::move(rhs);
		}
		return lhs + rhs;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string<CharT, Traits, Alloc>&& lhs,
													const CharT* rhs)
	{
		lhs += rhs;
		return std::move(lhs);
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string<CharT, Traits, Alloc>&& lhs,
													CharT rhs)
	{
		lhs += rhs;
		return std::move(lhs);
	}

	//uuz-lib only

	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(const basic_string<CharT, Traits, Alloc>& lhs,
												basic_string_view<CharT, Traits> rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(lhs.size() + rhs.size());
		temp += lhs;
		return temp += rhs;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string<CharT, Traits, Alloc>&& lhs,
												basic_string_view<CharT, Traits> rhs)
	{
		lhs += rhs;
		return std::move(lhs);
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string_view<CharT, Traits> lhs,
												const basic_string<CharT, Traits, Alloc>& rhs)
	{
		basic_string<CharT, Traits, Alloc> temp;
		temp.reserve(lhs.size() + rhs.size());
		temp += lhs;
		return temp += rhs;
	}
	template< class CharT, class Traits, class Alloc >
	basic_string<CharT, Traits, Alloc> operator+(basic_string_view<CharT, Traits> lhs,
												basic_string<CharT, Traits, Alloc>&& rhs)
	{
		auto k = lhs.size() + rhs.size();
		if(k <= rhs.capacity())
		{
			rhs.insert(rhs.begin(), lhs);
			return std::move(rhs);
		}
		return lhs + rhs;
	}


	template< class CharT, class Traits, class Alloc >
	bool operator==(const basic_string<CharT, Traits, Alloc>& lhs,
					const basic_string<CharT, Traits, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) == 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs,
					const basic_string<CharT, Traits, Alloc>& rhs)noexcept
	{
		return !(lhs == rhs);
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<(const basic_string<CharT, Traits, Alloc>& lhs,
					const basic_string<CharT, Traits, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) < 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<=(const basic_string<CharT, Traits, Alloc>& lhs,
					const basic_string<CharT, Traits, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>(const basic_string<CharT, Traits, Alloc>& lhs,
					const basic_string<CharT, Traits, Alloc>& rhs)noexcept
	{
		return rhs < lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>=(const basic_string<CharT, Traits, Alloc>& lhs,
					const basic_string<CharT, Traits, Alloc>& rhs)noexcept
	{
		return !(lhs < rhs);
	}

	template< class CharT, class Traits, class Alloc >
	bool operator==(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs.compare(lhs) == 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator==(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
	{
		return rhs == lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator!=(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}
	template< class CharT, class Traits, class Alloc >
	bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
	{
		return rhs != lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) < 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) > 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>=(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) >= 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<=(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs > lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<=(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs >= lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs < lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>=(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs <= lhs;
	}
	
	//uuz-lib only
	template< class CharT, class Traits, class Alloc >
	bool operator==(basic_string_view<CharT,Traits> lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs.compare(lhs) == 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator==(const basic_string<CharT, Traits, Alloc>& lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs == lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator!=(basic_string_view<CharT,Traits> lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}
	template< class CharT, class Traits, class Alloc >
	bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs != lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<(const basic_string<CharT, Traits, Alloc>& lhs, basic_string_view<CharT,Traits> rhs)
	{
		return lhs.compare(rhs) < 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>(const basic_string<CharT, Traits, Alloc>& lhs, basic_string_view<CharT,Traits> rhs)
	{
		return lhs.compare(rhs) > 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>=(const basic_string<CharT, Traits, Alloc>& lhs, basic_string_view<CharT,Traits> rhs)
	{
		return lhs.compare(rhs) >= 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<=(const basic_string<CharT, Traits, Alloc>& lhs, basic_string_view<CharT,Traits> rhs)
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<(basic_string_view<CharT,Traits> lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs > lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator<=(basic_string_view<CharT,Traits> lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs >= lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>(basic_string_view<CharT,Traits> lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs < lhs;
	}
	template< class CharT, class Traits, class Alloc >
	bool operator>=(basic_string_view<CharT,Traits> lhs, const basic_string<CharT, Traits, Alloc>& rhs)
	{
		return rhs <= lhs;
	}

	template< class CharT, class Traits>
	bool operator==(basic_string_view<CharT,Traits> lhs,
		basic_string_view<CharT,Traits> rhs)noexcept
	{
		return lhs.compare(rhs) == 0;
	}
	template< class CharT, class Traits>
	bool operator!=(basic_string_view<CharT,Traits> lhs,
		basic_string_view<CharT,Traits> rhs)noexcept
	{
		return !(lhs == rhs);
	}
	template< class CharT, class Traits>
	bool operator<(basic_string_view<CharT,Traits> lhs,
		basic_string_view<CharT,Traits> rhs)noexcept
	{
		return lhs.compare(rhs) < 0;
	}
	template< class CharT, class Traits>
	bool operator<=(basic_string_view<CharT,Traits> lhs,
		basic_string_view<CharT,Traits> rhs)noexcept
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class CharT, class Traits>
	bool operator>(basic_string_view<CharT,Traits> lhs,
		basic_string_view<CharT,Traits> rhs)noexcept
	{
		return rhs < lhs;
	}
	template< class CharT, class Traits>
	bool operator>=(basic_string_view<CharT,Traits> lhs,
		basic_string_view<CharT,Traits> rhs)noexcept
	{
		return !(lhs < rhs);
	}

	template< class CharT, class Traits>
	bool operator==(const CharT* lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs.compare(lhs) == 0;
	}
	template< class CharT, class Traits>
	bool operator==(basic_string_view<CharT,Traits> lhs, const CharT* rhs)
	{
		return rhs == lhs;
	}
	template< class CharT, class Traits>
	bool operator!=(const CharT* lhs, basic_string_view<CharT,Traits> rhs)
	{
		return !(lhs == rhs);
	}
	template< class CharT, class Traits>
	bool operator!=(basic_string_view<CharT,Traits> lhs, const CharT* rhs)
	{
		return rhs != lhs;
	}
	template< class CharT, class Traits>
	bool operator<(basic_string_view<CharT,Traits> lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) < 0;
	}
	template< class CharT, class Traits>
	bool operator>(basic_string_view<CharT,Traits> lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) > 0;
	}
	template< class CharT, class Traits>
	bool operator>=(basic_string_view<CharT,Traits> lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) >= 0;
	}
	template< class CharT, class Traits>
	bool operator<=(basic_string_view<CharT,Traits> lhs, const CharT* rhs)
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class CharT, class Traits>
	bool operator<(const CharT* lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs > lhs;
	}
	template< class CharT, class Traits>
	bool operator<=(const CharT* lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs >= lhs;
	}
	template< class CharT, class Traits>
	bool operator>(const CharT* lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs < lhs;
	}
	template< class CharT, class Traits>
	bool operator>=(const CharT* lhs, basic_string_view<CharT,Traits> rhs)
	{
		return rhs <= lhs;
	}

	template< class CharT, class Traits, class Alloc >
	void swap(basic_string<CharT, Traits, Alloc> &lhs,
				basic_string<CharT, Traits, Alloc> &rhs) noexcept(is_nothrow_swap_alloc<Alloc>::value)
	{
		lhs.swap(rhs);
	}

	template <class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
													basic_string_view <CharT, Traits> v)
	{
		for (auto i = 0; i != v.size(); ++i)
			os << v[i];
		return os;
	}

	template <class CharT, class Traits, class Allocator>
	std::basic_ostream<CharT, Traits>&
		operator<<(std::basic_ostream<CharT, Traits>& os,
			const std::basic_string<CharT, Traits, Allocator>& str)
	{

	}
}