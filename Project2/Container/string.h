#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"container.h"
#include<cstring>
#include<cassert>
#include"vector.h"
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
			while (count--)
				*(p + count) = a;
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
			return static_cast<char_type*>(memmove(dest, src, count));
		}

		static char_type* copy(char_type* dest, const char_type* src, std::size_t count)noexcept
		{
			return move(dest, src, count);
		}

		static constexpr int compare(const char_type* s1, const char_type* s2, std::size_t count)noexcept
		{
			return strcmp(s1, s2);
		}

		static constexpr std::size_t length(const char_type* s)noexcept
		{
			return strlen(s);
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
		constexpr basic_string_view(const CharT* s):dat(s),ssize(::strlen(dat)){}

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

		constexpr void remove_prefix(size_type n);

		constexpr void remove_suffix(size_type n);

		constexpr void swap(basic_string_view& v) noexcept;

		size_type copy(CharT* dest,size_type count,size_type pos = 0) const;

		constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const;

		constexpr int compare(basic_string_view v) const noexcept;
		constexpr int compare(size_type pos1, size_type count1,
			basic_string_view v) const;
		constexpr int compare(size_type pos1, size_type count1, basic_string_view v,
			size_type pos2, size_type count2) const;
		constexpr int compare(const CharT* s) const;
		constexpr int compare(size_type pos1, size_type count1,
			const CharT* s) const;
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
		using iterator = basic_string_iterator<CharT, Traits>;
		const static size_t npos;
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
		}data;
		bool sso_flag = false;
		size_t ssize = 0;
		Allocator alloc;
	public:
		basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) {}
		explicit basic_string(const Allocator& alloc) noexcept:alloc(alloc){}
		basic_string(size_t count,CharT ch,const Allocator& alloc = Allocator()):basic_string(alloc)
		{
			
		}
		basic_string(const basic_string& other,size_t pos,const Allocator& a = Allocator());
		basic_string(const basic_string& other,size_t pos,size_t count,const Allocator& alloc = Allocator());
		basic_string(const CharT* s,size_t count,const Allocator& alloc = Allocator());
		basic_string(const CharT* s,const Allocator& alloc = Allocator());
		template< class InputIt >
		basic_string(InputIt first, InputIt last,const Allocator& alloc = Allocator());
		basic_string(const basic_string& other);
		basic_string(const basic_string& other, const Allocator& alloc);
		basic_string(basic_string&& other) noexcept;
		basic_string(basic_string&& other, const Allocator& alloc);
		basic_string(std::initializer_list<CharT> init,
			const Allocator& alloc = Allocator());
		explicit basic_string(std::basic_string_view<CharT, Traits> sv,
			const Allocator& alloc = Allocator());
		template < class T >
		basic_string(const T& t, size_t pos, size_t n,
			const Allocator& alloc = Allocator());

		basic_string& operator=(const basic_string& str);
		basic_string& operator=(basic_string&& str);
		basic_string& operator=(const CharT* s);
		basic_string& operator=(CharT ch);
		basic_string& operator=(std::initializer_list<CharT> ilist);
		basic_string& operator=(std::basic_string_view<CharT, Traits> sv);

		basic_string& assign(size_t count, CharT ch);
		basic_string& assign(const basic_string& str);
		basic_string& assign(const basic_string& str,
			size_t pos,
			size_t count = npos);
		basic_string& assign(basic_string&& str);
		basic_string& assign(const CharT* s,
			size_t count);
		basic_string& assign(const CharT* s);
		template< class InputIt >
		basic_string& assign(InputIt first, InputIt last);
		basic_string& assign(std::initializer_list<CharT> ilist);
		basic_string& assign(std::basic_string_view<CharT, Traits> sv);
		template < class T >
		basic_string& assign(const T& t,
			size_t pos,
			size_t count = npos);

		allocator_type get_allocator() const;

		reference       at(size_type pos);
		const_reference at(size_type pos) const;

		reference       operator[](size_type pos);
		const_reference operator[](size_type pos) const;

		CharT& front();
		const CharT& front() const;

		CharT& back();
		const CharT& back() const;

		const CharT* data() const;
		CharT* data();

		const CharT* c_str() const;

		operator std::basic_string_view<CharT, Traits>() const noexcept;

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;

		iterator end();
		const_iterator end() const;
		const_iterator cend() const;

		bool empty() const;

		size_type size() const;
		size_type length() const;

		size_type max_size() const;

		void reserve(size_type new_cap = 0);

		size_type capacity() const;

		void shrink_to_fit();

		void clear();

		basic_string& insert(size_type index, size_type count, CharT ch);
		basic_string& insert(size_type index, const CharT* s);
		basic_string& insert(size_type index, const CharT* s, size_type count);
		basic_string& insert(size_type index, const basic_string& str);
		basic_string& insert(size_type index, const basic_string& str,
			size_type index_str, size_type count = npos);
		iterator insert(const_iterator pos, CharT ch);
		iterator insert(const_iterator pos, size_type count, CharT ch);
		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last);
		iterator insert(const_iterator pos, std::initializer_list<CharT> ilist);
		basic_string& insert(size_type pos, basic_string_view<CharT, Traits> sv);
		template < class T >
		basic_string& insert(size_type index, const T& t,
			size_type index_str, size_type count = npos);

		basic_string& erase(size_type index = 0, size_type count = npos);
		iterator erase(const_iterator position);
		iterator erase(const_iterator first, const_iterator last);

		void push_back(CharT ch);

		void pop_back();

		basic_string& append(size_type count, CharT ch);
		basic_string& append(const basic_string& str);
		basic_string& append(const basic_string& str,
			size_type pos,
			size_type count = npos);
		basic_string& append(const CharT* s, size_type count);
		basic_string& append(const CharT* s);
		template< class InputIt >
		basic_string& append(InputIt first, InputIt last);
		basic_string& append(std::initializer_list<CharT> ilist);
		basic_string& append(std::basic_string_view<CharT, Traits> sv);
		template < class T >
		basic_string& append(const T& t, size_type pos,
			size_type count = npos);

		basic_string& operator+=(const basic_string& str);
		basic_string& operator+=(CharT ch);		
		basic_string& operator+=(const CharT* s);	
		basic_string& operator+=(std::initializer_list<CharT> ilist);		
		basic_string& operator+=(std::basic_string_view<CharT, Traits> sv);

		int compare(const basic_string& str) const;
		int compare(size_type pos1, size_type count1,
			const basic_string& str) const;

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

		basic_string substr(size_type pos = 0,
			size_type count = npos) const;

		size_type copy(CharT* dest,
				size_type count,
				size_type pos = 0) const;

		void resize(size_type count);
		void resize(size_type count, CharT ch);

		void swap(basic_string& other) noexcept(/* see below */);

		size_type find(const basic_string& str, size_type pos = 0) const
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
				size_type pos = 0) const

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


	};
	using string = basic_string<char>;
	using string_view = basic_string_view<char>;
}