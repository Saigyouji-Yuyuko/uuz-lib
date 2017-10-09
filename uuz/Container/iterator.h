#pragma once
#include<type_traits>
#include<utility>
#include"../prepare.h"
namespace
{
	template<typename T,typename U>
	using have_push_back = decltype(std::declval<T>().push_back(std::declval<U>()));
	template<typename T, typename U>
	using have_push_front = decltype(std::declval<T>().push_front(std::declval<U>()));
	template<typename T>
	using move_iterator_reference = std::conditional_t<>
}

namespace uuz
{
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };

	template<typename Iterator>
	struct iterator_traits
	{
		using difference_type = typename Iterator::difference_type;
		using value_type = typename Iterator::value_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
		using iterator_category = typename Iterator::iterator_category;
	};

	template<typename T>
	struct iterator_traits<T*>
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = random_access_iterator_tag;
	};

	template<typename T>
	struct iterator_traits<const T*>
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = random_access_iterator_tag;

	};

	template<typename Category, typename T, typename Distance = ptrdiff_t,
		typename Pointer = T*, typename Reference = T&>
		struct iterator
	{
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
		using iterator_category = Category;
	};

	// Requires: n shall be negative only for bidirectional and random access iterators.
	// Effects: Increments(or decrements for negative n) iterator reference i by n.
	template <typename InputIterator, typename Distance>
	void advance(InputIterator& i, Distance n)
	{
		if constexpr(std::is_same_v<random_access_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>)
		{
			i += n;
		}
		else if constexpr(std::is_same_v<bidirectional_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>)
		{
			if (n >= 0)
				while(n--)
					++i;
			else
				while(n++)
					--i;
		}
		else if constexpr(std::is_same_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>)
		{
			if (n >= 0)
				while (n--)
					++i;
			else
#ifdef DEBUG
				assert(false, "only bidirectional_iterator can sub");
#else
				throw("n shall be negative only for bidirectional and random access iterators");
#endif
		}
		else
		{
			static_assert(false);
		}
	}

	//Effects: If InputIterator meets the requirements of random access iterator, returns (last - first);
	//otherwise, returns the number of increments needed to get from first to last.
	//Requires: If InputIterator meets the requirements of random access iterator, last shall be reachable
	//from first or first shall be reachable from last; otherwise, last shall be reachable from first.
	template <typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		if constexpr(std::is_same_v<random_access_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>)
		{
			return last - first;
		}
		else if constexpr(std::is_same_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>)
		{
			typename iterator_traits<InputIterator>::difference_type temp{ 0 };
			for (; first != last; ++first, (void)++temp)
				;
			return temp;
		}
		else
		{
			static_assert(false);
		}
	}

	//Effects: Equivalent to: advance(x, n); return x;
	template <typename InputIterator>
	InputIterator next(InputIterator x,
		typename std::iterator_traits<InputIterator>::difference_type n = 1)
	{
		advacne(x, n);
		return x;
	}

	//Effects: Equivalent to: advance(x, -n); return x;
	template <typename BidirectionalIterator>
	BidirectionalIterator prev(BidirectionalIterator x,
		typename std::iterator_traits<BidirectionalIterator>::difference_type n = 1)
	{
		std::advance(x, -n);
		return x;
	}

	//C++17 N4687
	// The template parameter Iterator shall meet all the requirements of a Bidirectional Iterator
	//Additionally, Iterator shall meet the requirements of a random access iterator.if any of the members
	//operator+, operator- , operator+= , operator-= ,
	//operator[], or the non-member operators operator< , operator> ,
	//operator<= , operator>= , operator-  or operator+  are referenced in a way that requires instantiation.
	template <typename Iterator>
	class reverse_iterator
	{
	public:
		using iterator_type = Iterator;
		using iterator_category = typename iterator_traits<Iterator>::iterator_category;
		using value_type = typename iterator_traits<Iterator>::value_type;
		using difference_type = typename iterator_traits<Iterator>::difference_type;
		using pointer = typename iterator_traits<Iterator>::pointer;
		using reference = typename iterator_traits<Iterator>::reference;

		//Effects: Value-initializes current. Iterator operations applied to the resulting iterator have defined
		//behavior if and only if the corresponding operations are defined on a value - initialized iterator of type Iterator.
		constexpr reverse_iterator() = default;

		//Effects: Initializes current with x.
		constexpr explicit reverse_iterator(Iterator x)noexcept(noexcept(currnet(x))) :current(x) {}

		//Effects: Initializes current with u.current.
		template <typename U>
		constexpr reverse_iterator(const reverse_iterator<U>& u)noexcept(noexcept(current(u.current))) : current(u.current) {}

		//Effects: Assigns u.base() to current.
		//Returns : *this.
		template <typename U>
		constexpr reverse_iterator& operator=(const reverse_iterator<U>& u)
		{
			current = u.base();
			return *this;
		}

		//Returns: current.
		constexpr Iterator base() const // explicit
		{
			return current;
		}

		//Effects: As if by :
		//Iterator tmp = current;
		// return *--tmp;
		constexpr reference operator*() const
		{
			auto tmp = current;
			return *--tmp;
		}

		//Returns: addressof(operator*()).
		constexpr pointer operator->() const
		{
			return addressof(this->operator*());
		}

		//Effects: As if by:
		//reverse_iterator tmp = *this;
		//--current;
		//return tmp;
		constexpr reverse_iterator operator++(int)
		{
			auto temp = *this;
			--current;
			return temp;
		}

		//Effects: As if by ++current.
		//Returns : *this.
		constexpr reverse_iterator& operator++()
		{
			--current;
			return *this;
		}

		//Effects: As if by ++current.
		//Returns: *this.
		constexpr reverse_iterator& operator--()
		{
			++current;
			return *this;
		}

		//Effects: As if by:
		//reverse_iterator tmp = *this;
		//++current;
		//return tmp;
		constexpr reverse_iterator operator--(int)
		{
			auto tmp = current;
			++current;
			return tmp;
		}

		//Returns: reverse_iterator(current - n).
		constexpr reverse_iterator operator+ (difference_type n) const
		{
			return reverse_iterator(current - n);
		}

		//Effects: As if by: current -= n;
		//Returns: *this.
		constexpr reverse_iterator& operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}

		//Returns: reverse_iterator(current+n).
		constexpr reverse_iterator operator- (difference_type n) const
		{
			return reverse_iterator(current + n);
		}

		//Effects: As if by: current += n;
		//Returns: *this.
		constexpr reverse_iterator& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}

		//Returns: current[-n-1].
		constexpr auto operator[](difference_type n)const
			->decltype(current[-n - 1])
		{
			return current[-n - 1];
		}
	protected:
		Iterator current;
	};

	//Returns: x.current == y.current.
	template <typename Iterator1, typename Iterator2>
	bool operator==(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() == y.base();
	}

	//Returns: x.current > y.current.
	template <typename Iterator1, typename Iterator2>
	bool operator<(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() > y.base();
	}

	//x.current != y.current
	template <typename Iterator1, typename Iterator2>
	bool operator!=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return !(x == y);
	}

	//Returns: x.current >= y.current.
	template <typename Iterator1, typename Iterator2>
	bool operator<=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() >= y.base();
	}

	//Returns: x.current < y.current
	template <typename Iterator1, typename Iterator2>
	bool operator>(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() < y.base();
	}

	//Returns: x.current <= y.current.
	template <typename Iterator1, typename Iterator2>
	bool operator>=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() <= y.base();
	}

	//Returns: y.current - x.current.
	template <typename Iterator1, typename Iterator2>
	auto operator-(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y) ->decltype(y.base() - x.base())
	{
		return y.base() - x.base();
	}

	//Returns: reverse_iterator<Iterator> (x.current - n).
	template <typename Iterator>
	reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x)
	{
		return reverse_iterator<Iterator>(x.base() - n).
	}

	//Returns: reverse_iterator<Iterator>(i).
	template <typename Iterator>
	reverse_iterator<Iterator> make_reverse_iterator(Iterator i)
	{
		return reverse_iterator<Iterator>(i);
	}

	template <typename Container>
	class back_insert_iterator
	{
	protected:
		Container* container;
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
		using container_type = Container;

		//Effects: Initializes container with addressof(x).
		explicit back_insert_iterator(Container& x):container(addressof(x)){}

		//Effects: As if by: container->push_back(value);
		//Returns: *this.
		back_insert_iterator& operator=(const typename Container::value_type& value)
		{
			if constexpr(experimental::is_detected_v<have_push_back,Container, typename Container::value_type>)
			{
				container->push_back(value);
			}
			else
			{
				container->insert(end(), value);
			}
			return *this;
		}

		//Effects: As if by: container->push_back(std::move(value));
		//Returns: *this.
		back_insert_iterator& operator=(typename Container::value_type&& value)
		{
			
			if constexpr(std::is_same_v<std::void_t<decltype(container->push_back(std::move(value)))>, void>)
			{
				container->push_back(std::move(value));
			}
			else
			{
				container->insert(end(), std::move(value));
			}
			return *this;
		}

		//Returns: *this.
		back_insert_iterator& operator*()
		{
			return *this;
		}

		//Returns: *this.
		back_insert_iterator& operator++()
		{
			return *this;
		}
		//Returns: *this.
		back_insert_iterator operator++(int)
		{
			return *this;
		}
	};

	//Returns: back_insert_iterator<Container>(x).
	template <typename Container>
	back_insert_iterator<Container> back_inserter(Container& x)
	{
		return back_insert_iterator<Container>(x);
	}

	template <typename Container>
	class front_insert_iterator 
	{
	protected:
		Container* container;
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
		using container_type = Container;
		
		//Effects: Initializes container with addressof(x).
		explicit front_insert_iterator(Container& x):container(addressof(x)){}

		front_insert_iterator& operator=(const typename Container::value_type& value)
		{
			if constexpr(experimental::is_detected_v<have_push_front, Container, typename Container::value_type>)
			{
				container->push_front(value);
			}
			else
			{
				container->insert(begin(), value);
			}
			return *this;
		}

		front_insert_iterator& operator=(typename Container::value_type&& value)
		{
			if constexpr(experimental::is_detected_v<have_push_front, Container, typename Container::value_type>)
			{
				container->push_front(std::move(value));
			}
			else
			{
				container->insert(begin(), std::move(value));
			}
			return *this;
		}


		front_insert_iterator& operator*()
		{
			return *this;
		}

		front_insert_iterator& operator++()
		{
			return *this;
		}

		front_insert_iterator operator++(int)
		{
			return *this;
		}
	};

	template <typename Container>
	front_insert_iterator<Container> front_inserter(Container& x)
	{
		return front_insert_iterator<Container>(x);
	}

	template <typename Container>
	class insert_iterator {
	protected:
		Container* container;
		typename Container::iterator iter;
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
		using container_type = Container;

		insert_iterator(Container& x, typename Container::iterator i):container(x),iter(i){}
		
		insert_iterator& operator=(const typename Container::value_type& value)
		{
			container->insert(iter, value);
			return *this;
		}

		insert_iterator& operator=(typename Container::value_type&& value)
		{
			container->insert(iter, std::move(value));
			return *this;
		}
		
		insert_iterator& operator*()
		{
			return *this;
		}

		insert_iterator& operator++()
		{
			return *this;
		}
		insert_iterator& operator++(int)
		{
			return *this;
		}
	};
	template <typename Container>
	insert_iterator<Container> inserter(Container& x, typename Container::iterator i)
	{
		return insert_iterator<Container>(x, i);
	}

	template <typename Iterator>
	class move_iterator {
	public:
		using iterator_type = Iterator;
		using iterator_category = typename iterator_traits<Iterator>::iterator_category;
		using value_type = typename iterator_traits<Iterator>::value_type;
		using difference_type = typename iterator_traits<Iterator>::difference_type;
		using pointer = Iterator;
		using reference = std::conditional_t<std::is_reference_v<typename iterator_traits<Iterator>::reference>,
											std::remove_reference_t<typename iterator_traits<Iterator>::reference>&&,
											typename iterator_traits<Iterator>::reference>;

		constexpr move_iterator() = default;

		constexpr explicit move_iterator(Iterator i):current(i){}
		
		template <typename U>
		constexpr move_iterator(const move_iterator<U>& u):current(u.base()){}

		template <typename U> 
		constexpr move_iterator& operator=(const move_iterator<U>& u)
		{
			current = u.base();
			return *this;
		}

		constexpr iterator_type base() const
		{
			return current;
		}

		constexpr reference operator*() const
		{
			return static_cast<reference>(*current);
		}

		constexpr pointer operator->() const
		{
			return current;
		}

		constexpr move_iterator& operator++()
		{
			++current;
			return *this;
		}

		constexpr move_iterator operator++(int)
		{
			auto t = *this;
			++current;
			return t;
		}
		
		constexpr move_iterator& operator--()
		{
			--current;
			return *this;
		}
		
		constexpr move_iterator operator--(int)
		{
			auto t = *this;
			--current;
			return t;
		}

		constexpr move_iterator operator+(difference_type n) const
		{
			return move_iterator(current + n);
		}

		constexpr move_iterator& operator+=(difference_type n)
		{
			current += n;
			return *this;
		}

		constexpr move_iterator operator-(difference_type n) const
		{
			return move_iterator(current - n);
		}

		constexpr move_iterator& operator-=(difference_type n)
		{
			current -= n;
			return *this;
		}
		
		constexpr decltype(auto) operator[](difference_type n) const ->decltype(std::move(current[n]))
		{
			return std::move(current[n]);
		}
	private:
		Iterator current; // exposition only
	};

	template <typename Iterator1, typename Iterator2>
	constexpr bool operator==(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
	{
		return x.base() == y.base();
	}
	template <class Iterator1, class Iterator2>
	constexpr bool operator!=(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
	{
		return x.base() != y.base();
	}
	template <class Iterator1, class Iterator2>
	constexpr bool operator<(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
	{
		return x.base() < y.base();
	}
	template <class Iterator1, class Iterator2>
	constexpr bool operator<=(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
	{
		return x.base() <= y.base();
	}
	template <class Iterator1, class Iterator2>
	constexpr bool operator>(
		const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
	{
		return x.base() > y.base();
	}
	template <class Iterator1, class Iterator2>
	constexpr bool operator>=(
		const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
	{
		return x.base() >= y.base();
	}
	template <class Iterator1, class Iterator2>
	constexpr auto operator-(
		const move_iterator<Iterator1>& x,
		const move_iterator<Iterator2>& y) -> decltype(x.base() - y.base())
	{
		return x.base() - y.base();
	}
	template <class Iterator>
	constexpr move_iterator<Iterator> operator+(
		typename move_iterator<Iterator>::difference_type n, const move_iterator<Iterator>& x)
	{
		return x + n;
	}
	template <class Iterator>
	constexpr move_iterator<Iterator> make_move_iterator(Iterator i)
	{
		return move_iterator<Iterator>(i);
	}
	
	template<typename T>
	struct char_traits;

	template<typename T,typename U>
	class basic_istream;

	template<typename T, typename U>
	class basic_ostream;

	template<typename T, typename U>
	class basic_streambuf;

	template <typename T, typename charT = char, typename traits = char_traits<charT>,
		typename Distance = ptrdiff_t>
	class istream_iterator 
	{
		public:
			using iterator_category = input_iterator_tag;
			using value_type = T;
			using difference_type = Distance;
			using pointer = const T*;
			using reference = const T&;
			using char_type = charT;
			using traits_type = traits;
			using istream_type = basic_istream<charT, traits>;

			constexpr istream_iterator():in_stream(nullptr){}

			istream_iterator(istream_type& s):in_stream(addressof(s)){}
			
			istream_iterator(const istream_iterator& x) = default;
			~istream_iterator() = default;

			const T& operator*() const
			{
				return value;
			}

			const T* operator->() const
			{
				return addressof(operator*());
			}

			istream_iterator& operator++()
			{
				*in_stream >> value;
				return *this;
			}

			istream_iterator operator++(int)
			{
				auto tmp = *this;
				*in_stream >> value;
				return (tmp);
			}
		private:
			basic_istream<charT, traits>* in_stream; // exposition only
			T value; // exposition only

			friend bool operator==(const istream_iterator<T, charT, traits, Distance>& x,
									const istream_iterator<T, charT, traits, Distance>& y);
	};
	template <typename T, typename charT, typename traits, typename Distance>
	bool operator==(const istream_iterator<T, charT, traits, Distance>& x,
					const istream_iterator<T, charT, traits, Distance>& y)
	{
		return x.in_stream == y.in_stream;
	}
	template <typename T, typename charT, typename traits, typename Distance>
	bool operator!=(const istream_iterator<T, charT, traits, Distance>& x,
		const istream_iterator<T, charT, traits, Distance>& y)
	{
		return !(x == y);
	}

	template <typename T, typename charT = char, typename traits = char_traits<charT>>
	class ostream_iterator 
	{
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
		using char_type = charT;
		using traits_type = traits;
		using ostream_type = basic_ostream<charT, traits>;

		ostream_iterator(ostream_type& s):out_stream(addressof(s)),delim(nullptr){}

		ostream_iterator(ostream_type& s, const charT* delimiter) :out_stream(addressof(s)), delim(delimiter) {}
		
		ostream_iterator(const ostream_iterator& x) :out_stream(addressof(x.out_stream)), delim(x.delim) {}
		
		~ostream_iterator() = default;

		ostream_iterator& operator=(const T& value)
		{
			*out_stream << value;
			if (delim != 0)
				*out_stream << delim;
			return *this;
		}

		ostream_iterator& operator*()
		{
			return *this
		}
		ostream_iterator& operator++()
		{
			return *this
		}
		ostream_iterator& operator++(int)
		{
			return *this
		}
	private:
		basic_ostream<charT, traits>* out_stream; // exposition only
		const charT* delim; // exposition only
	};
		
	//template<typename charT, typename traits = char_traits<charT>>
	//class istreambuf_iterator {
	//public:
	//	using iterator_category = input_iterator_tag;
	//	using value_type = charT;
	//	using difference_type = typename traits::off_type;
	//	using pointer = const charT*;
	//	using reference = charT;
	//	using char_type = charT;
	//	using traits_type = traits;
	//	using int_type = typename traits::int_type;
	//	using streambuf_type = basic_streambuf<charT, traits>;
	//	using istream_type = basic_istream<charT, traits>;
	//	class proxy; // exposition only

	//	constexpr istreambuf_iterator() noexcept;
	//	istreambuf_iterator(const istreambuf_iterator&) noexcept = default;
	//	~istreambuf_iterator() = default;
	//	istreambuf_iterator(istream_type& s) noexcept;
	//	istreambuf_iterator(streambuf_type* s) noexcept;
	//	istreambuf_iterator(const proxy& p) noexcept;
	//	charT operator*() const;
	//	istreambuf_iterator& operator++();
	//	proxy operator++(int);
	//	bool equal(const istreambuf_iterator& b) const;
	//private:
	//	streambuf_type* sbuf_; // exposition only
	//};
	//template <class charT, class traits>
	//bool operator==(const istreambuf_iterator<charT, traits>& a,
	//	const istreambuf_iterator<charT, traits>& b);
	//template <class charT, class traits>
	//bool operator!=(const istreambuf_iterator<charT, traits>& a,
	//	const istreambuf_iterator<charT, traits>& b);

	//template <class charT, class traits = char_traits<charT>>
	//class ostreambuf_iterator {
	//public:
	//	using iterator_category = output_iterator_tag;
	//	using value_type = void;
	//	using difference_type = void;
	//	using pointer = void;
	//	using reference = void;
	//	using char_type = charT;
	//	using traits_type = traits;
	//	using streambuf_type = basic_streambuf<charT, traits>;
	//	using ostream_type = basic_ostream<charT, traits>;
	//	ostreambuf_iterator(ostream_type& s) noexcept;
	//	ostreambuf_iterator(streambuf_type* s) noexcept;
	//	ostreambuf_iterator& operator=(charT c);
	//	ostreambuf_iterator& operator*();
	//	ostreambuf_iterator& operator++();
	//	ostreambuf_iterator& operator++(int);
	//	bool failed() const noexcept;
	//private:
	//	streambuf_type* sbuf_; // exposition only
	//};

	template< typename C >
	constexpr auto begin(C& c)noexcept(noexcept(c.begin())) -> decltype(c.begin())
	{
		return c.begin();
	}
	template< typename C >
	constexpr auto begin(const C& c)noexcept(noexcept(c.begin())) -> decltype(c.begin())
	{
		return c.begin();
	}
	template< typename T, std::size_t N >
	constexpr T* begin(T(&array)[N]) noexcept
	{
		return static_cast<T*>(array);
	}
	template< typename C >
	constexpr auto cbegin(const C& c) noexcept(noexcept(begin(c)))
		-> decltype(begin(c))
	{
		return begin(c);
	}

	template< typename C >
	constexpr auto end(C& c)noexcept(noexcept(c.end())) -> decltype(c.end())
	{
		return c.end();
	}
	template< typename C >
	constexpr auto end(const C& c)noexcept(noexcept(c.end())) -> decltype(c.end())
	{
		return c.end();
	}
	template< typename T, std::size_t N >
	constexpr T* end(T(&array)[N]) noexcept
	{
		return begin(array) + N;
	}
	template< typename C >
	constexpr auto cend(const C& c) noexcept(noexcept(end(c)))
		-> decltype(end(c))
	{
		return end(c);
	}

	template <typename C>
	constexpr auto rbegin(C& c)noexcept(noexcept(c.rbegin())) -> decltype(c.rbegin())
	{
		return c.rbegin();
	}
	template <typename C>
	constexpr auto rbegin(const C& c)noexcept(noexcept(c.rbegin())) -> decltype(c.rbegin())
	{
		return c.rbegin();
	}
	template <typename C>
	constexpr auto rend(C& c)noexcept(noexcept(c.rend())) -> decltype(c.rend())
	{
		return c.rend();
	}
	template <typename C>
	constexpr auto rend(const C& c)noexcept(noexcept(c.rend())) -> decltype(c.rend())
	{
		return c.rend();
	}
	template <typename T, size_t N>
	constexpr reverse_iterator<T*> rbegin(T(&array)[N])
	{
		return make_reverse_iterator<T*>(end(array) - 1);
	}
	template <typename T, size_t N> constexpr reverse_iterator<T*> rend(T(&array)[N]);
	template <typename E> constexpr reverse_iterator<const E*> rbegin(std::initializer_list<E> il);
	template <typename E> constexpr reverse_iterator<const E*> rend(std::initializer_list<E> il);
	template <typename C> constexpr auto crbegin(const C& c) -> decltype(rbegin(c));
	template <typename C> constexpr auto crend(const C& c) -> decltype(rend(c));


	// 24.8, container access:
	template <typename C>
	constexpr auto size(const C& c)noexcept(noexcept(c.size())) -> decltype(c.size())
	{
		return c.size();
	}
	template <typename T, size_t N>
	constexpr size_t size(const T(&array)[N]) noexcept
	{
		return N;
	}

	template <typename C>
	constexpr auto empty(const C& c)noexcept(noexcept(c.empty())) -> decltype(c.empty())
	{
		return c.empty();
	}
	template <typename T, size_t N>
	constexpr bool empty(const T(&array)[N]) noexcept
	{
		return N == 0;
	}
	template <typename E>
	constexpr bool empty(std::initializer_list<E> il) noexcept
	{
		return il.empty()
	}

	template <typename C>
	constexpr auto data(C& c)noexcept(noexcept(c.data())) -> decltype(c.data())
	{
		return c.data();
	}
	template <typename C>
	constexpr auto data(const C& c)noexcept(noexcept(c.data())) -> decltype(c.data())
	{
		return c.data();
	}
	template <typename T, size_t N>
	constexpr T* data(T(&array)[N]) noexcept
	{
		return array;
	}
	template <typename E>
	constexpr const E* data(std::initializer_list<E> il) noexcept
	{
		return il.begin();
	}

}