#pragma once
#include<type_traits>
#include<cassert>
#include<exception>
#include<stdexcept>
namespace
{
	template<typename D, typename Void, template<typename...>class Op, typename... T>
	struct is_detected_impl
	{
		using value_t = std::false_type;
		using type = D;
	};
	template<template<typename...>class op, typename... T>
	struct is_detected_impl<void, std::void_t<op<T...>>, op, T>
	{
		using value_t = std::true_type;
		using type = op<T...>;
	};


}
namespace uuz
{
#ifdef _MSC_VER 
#define INLINE __forceinline
#elif __GNUC__ or __clang__
#define INLINE  __inline __attribute__((always_inline))
#else
#define INLINE inline
#endif 

	template<typename T >
	constexpr T* addressof(T& arg) noexcept
	{
		return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
	}
	template <typename T>
	const T* addressof(const T&&) = delete;


	
	using size_t = uint32_t;
	using ptrdiff_t = int32_t;

	namespace experimental
	{
		struct nonesuch {
			nonesuch() = delete;
			~nonesuch() = delete;
			nonesuch(nonesuch const&) = delete;
			void operator=(nonesuch const&) = delete;
		};

		template <template<class...> class Op, class... Args>
		using is_detected = typename is_detected_impl<nonesuch, void, Op, Args...>::value_t;

		template <template<class...> class Op, class... Args>
		using detected_t = typename is_detected_impl<nonesuch, void, Op, Args...>::type;

		template <class Default, template<class...> class Op, class... Args>
		using detected_or = is_detected_impl<Default, void, Op, Args...>;

		template< template<class...> class Op, class... Args >
		constexpr bool is_detected_v = is_detected<Op, Args...>::value;

		template< class Default, template<class...> class Op, class... Args >
		using detected_or_t = typename detected_or<Default, Op, Args...>::type;

		template <class Expected, template<class...> class Op, class... Args>
		using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;
		
		template <class Expected, template<class...> class Op, class... Args>
		constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;
	
		template <class To, template<class...> class Op, class... Args>
		using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;
		
		template <class To, template<class...> class Op, class... Args>
		constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;
		
}
	
	//老子不写了！！！
	template< typename Ptr >
	struct pointer_traits
	{
		
		using element_type = Ptr;
		using pointer = typename std::pointer_traits<Ptr>::pointer;
		using difference_type = ptrdiff_t;

		template<class Other>
		using rebind = typename std::pointer_traits<Ptr>::template rebind<Other>;

		static pointer pointer_to(element_type& r)
		{
			return std::pointer_traits<Ptr>::pointer_to(r);
		}

	};
	
	template<typename T>
	struct pointer_traits<T*>
	{
		using element_type = T;
		using pointer = T*;
		using difference_type = ptrdiff_t;

		template<class _Other>
		using rebind = _Other *;

		static pointer pointer_to(element_type& p)noexcept
		{	
			return std::addressof(p);
		}
	};


	//默认less仿函数
	template<typename T1,typename T2 = T1>
	struct less
	{
		constexpr bool operator()(const T1& a, const T2& b)const noexcept(noexcept(a<b))
		{
			return a < b;
		}
	};

	template<typename T>
	std::enable_if_t<std::is_trivially_destructible_v<T>> destroy_at(T*)noexcept
	{

	}

	template<typename T>
	std::enable_if_t<!std::is_trivially_destructible_v<T>> destroy_at(T* p)noexcept
	{
		return p->~T();
	}

	template<typename T,typename Size>
	std::enable_if_t<std::is_trivially_destructible_v<T>> destroy_n(T*,Size)noexcept
	{

	}

	template<typename T, typename Size>
	std::enable_if_t<!std::is_trivially_destructible_v<T>> destroy_n(T* first, Size n)noexcept
	{
		for (; n > 0;  ++first,(void) --n)
			destroy_at(addressof(*first));
		return first;
	}


	


	template<typename T,typename InputIt>//测试InputIt是否为T类型的迭代器或指针
	using is_input = std::enable_if_t<std::is_same_v<T, std::decay_t<decltype(*std::declval<InputIt>())>>>;

	template<typename It, typename T = std::decay_t<decltype(*std::declval<It>())>>
	void move_or_copy_con(It src, size_t t, It to,
		typename std::enable_if_t<std::is_nothrow_move_constructible_v<T> ||
		(!std::is_nothrow_copy_constructible_v<T>&&std::is_move_constructible_v<T>)>* = nullptr)noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		if (src == to)
			return;
		auto k = src;
		auto k2 = to;
		auto i = 0;
		try
		{
			for (; i != t; ++i, (void)++k, (void)++k2)
				new(&*(k2)) T(std::move(*(k)));
		}
		catch (...)
		{
			auto k3 = to;
			for (auto j = 0; j != i; ++j)
			{
				(*k3).~T();
				++k3;
			}
			throw;
		}
		return;
	}
	template<typename It, typename T = std::decay_t<decltype(*std::declval<It>())>>
	void move_or_copy_con(It src, size_t t, It to,
		typename std::enable_if_t<(!std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
		|| (!std::is_move_constructible_v<T> && std::is_copy_constructible_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_copy_constructible_v<T>)
	{
		if (src == to)
			return;
		auto k = src;
		auto k2 = to;
		auto i = 0;
		try
		{
			for (; i != t; ++i, (void)++k, (void)++k2)
				new(&*(k2)) T(*(k));
		}
		catch (...)
		{
			auto k3 = to;
			for (auto j = 0; j != i; ++j)
			{
				(*k3).~T();
				++k3;
			}
			throw;
		}
		return;
	}


	//考虑将src中的T个位置尽量移动到to中，其中to是脏数据
	template<typename T>
	void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<std::is_trivially_constructible_v<T>>* = nullptr)
		noexcept
	{
		//uuz::println("///");
		//static_assert(false,"123");
		if (src == to)
			return;
		/*if (to >= src + t || to <= src)
			memcpy(to, src, t * sizeof(T));
		else*/
			memmove(to, src, t * sizeof(T));
		return;
	}
	template<typename T>
	void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_trivially_constructible_v<T> && 
								std::is_nothrow_move_constructible_v<T> || 
		(!std::is_nothrow_copy_constructible_v<T>&&std::is_move_constructible_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		//static_assert(false,"123");
		if (src == to)
			return;
		int i;
		try
		{
			for (i = 0; i != t; ++i)
				new(to + i) T(std::move(*(src + i)));
		}
		catch (...)
		{
			for (int j = 0; j != i; ++j)
				(to + j)->~T();
			throw;
		}
		return;
	}
	template<typename T>
	void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_trivially_constructible_v<T> && 
								(!std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
								||(!std::is_move_constructible_v<T> && std::is_copy_constructible_v<T> )>* = nullptr)
		noexcept( std::is_nothrow_copy_constructible_v<T>)
	{
		//static_assert(false, "13");
		if (src == to)
			return;
		int i;
		try
		{
			for (i = 0; i != t; ++i)
				new(to + i) T(*(src + i));
		}
		catch (...)
		{
			for (int j = 0; j != i; ++j)
				*(to + i).~T();
			throw;
		}
		return;
	}
	template<typename T>
	[[noreturn]] void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_trivially_constructible_v<T> && 
								!std::is_copy_constructible_v<T> &&
								!std::is_move_constructible_v<T>>* = nullptr)
	{
		static_assert(!std::is_trivially_constructible_v<T> && !std::is_copy_assignable_v<T> && !std::is_move_assignable_v<T>, "T can't move or copy\n");
	}

	template<typename It,typename T = std::decay_t<decltype(*std::declval<It>())>>
	void move_or_copy_ass_for(It src,size_t t,It to, 
		typename std::enable_if_t<std::is_nothrow_move_constructible_v<T> ||
		(!std::is_nothrow_copy_constructible_v<T>&&std::is_move_constructible_v<T>)>* = nullptr)noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		if (src == to)
			return;
		auto k = src;
		auto k2 = to;
		auto i = 0;
		try
		{
			for (; i != t; ++i, (void)++k, (void)++k2)
				*(k2) = std::move(*(k));
		}
		catch (...)
		{
			auto k3 = to;
			for(auto j =0;j!=i;++j)
			{
				(*k3).~T();
				++k3;
			}
			throw;
		}
		return;
	}
	template<typename It, typename T = std::decay_t<decltype(*std::declval<It>())>>
	void move_or_copy_ass_for(It src, size_t t, It to,
		typename std::enable_if_t<(!std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
							|| (!std::is_move_constructible_v<T> && std::is_copy_constructible_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_copy_constructible_v<T>)
	{
		if (src == to)
			return;
		auto k = src;
		auto k2 = to;
		auto i = 0;
		try
		{
			for (; i != t; ++i, (void)++k, (void)++k2)
				*(k2) = *(k);
		}
		catch (...)
		{
			auto k3 = to;
			for (auto j = 0; j != i; ++j)
			{
				(*k3).~T();
				++k3;
			}
			throw;
		}
		return;
	}

	template<typename It, typename T = std::decay_t<decltype(*std::declval<It>())>>
	void move_or_copy_ass_back(It src, size_t t, It to,
		typename std::enable_if_t<std::is_nothrow_move_constructible_v<T> ||
		(!std::is_nothrow_copy_constructible_v<T>&&std::is_move_constructible_v<T>)>* = nullptr)noexcept(std::is_nothrow_move_constructible_v<T>)
	{
		if (src == to)
			return;
		auto k = src + (t - 1);
		auto k2 = to + (t - 1);
		auto i = t;
		try
		{
			for (; i != t; ++i, (void)--k, (void)--k2)
				*(k2) = std::move(*(k));
		}
		catch (...)
		{
			auto k3 = to + (t - 1);
			for (auto j = 0; j != i; ++j)
			{
				(*k3).~T();
				--k3;
			}
			throw;
		}
		return;
	}
	template<typename It, typename T = std::decay_t<decltype(*std::declval<It>())>>
	void move_or_copy_ass_back(It src, size_t t, It to,
		typename std::enable_if_t<(!std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
		|| (!std::is_move_constructible_v<T> && std::is_copy_constructible_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_copy_constructible_v<T>)
	{
		if (src == to)
			return;
		auto k = src + (t - 1);
		auto k2 = to + (t - 1);
		auto i = t;
		try
		{
			for (; i != t; ++i, (void)--k, (void)--k2)
				*(k2) = *(k);
		}
		catch (...)
		{
			auto k3 = to + (t - 1);
			for (auto j = 0; j != i; ++j)
			{
				(*k3).~T();
				--k3;
			}
			throw;
		}
		return;
	}





	//考虑将src中的T个位置尽量移动到to中，其中to是有效数据
	template<typename T>
	void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<std::is_trivially_copy_assignable_v<T>>* = nullptr)
		noexcept
	{
		//uuz::println("///");
		if (src == to)
			return;
		/*if (to >= src + t || to <= src)
			memcpy(to, src, t*sizeof(T));
		else*/
			memmove(to, src, t * sizeof(T));
	}
	template<typename T>
	void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_trivially_copy_assignable_v<T> &&
		std::is_nothrow_move_assignable_v<T> || 
		(!std::is_nothrow_copy_assignable_v<T>&&std::is_move_assignable_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_move_assignable_v<T>)
	{
		if (src == to)
			return;
		int i;
		try
		{
			if(to >= src +t || to <= src)
				for (i = 0; i != t; ++i)
					*(to + i) = std::move(*(src + i));
			else
				for (i = t-1; i >= 0; --i)
					*(to + i) = std::move(*(src + i));
		}
		catch (...)
		{
			if (to >= src + t)
				for (int j = 0; j != i; ++j)
					(*(to + i)).~T();
			else
				for (int j = t - 1; j != i; --j)
					(*(to + i)).~T();
			throw;
		}
		return;
	}
	template<typename T>
	void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_trivially_copy_assignable_v<T> &&
									(!std::is_nothrow_move_assignable_v<T> && std::is_nothrow_copy_assignable_v<T>)
								|| (!std::is_move_assignable_v<T> && std::is_copy_assignable_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_copy_assignable_v<T>)
	{
		if (src == to)
			return;
		int i;
		try
		{
			if (to >= src + t || to <= src)
				for (i = 0; i != t; ++i)
					*(to + i) = *(src + i);
			else
				for (i = t - 1; i >= 0; --i)
					*(to + i) = *(src + i);
		}
		catch (...)
		{
			if (to >= src + t)
				for (int j = 0; j != i; ++j)
					(*(to + i)).~T();
			else
				for (int j = t - 1; j != i; --j)
					(*(to + i)).~T();
			throw;
		}
		return;
	}
	template<typename T>
	[[noreturn]] void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_trivially_copy_assignable_v<T> &&
		!std::is_copy_assignable_v<T> && !std::is_move_assignable_v<T>>* = nullptr)
	{
		static_assert(!std::is_copy_assignable_v<T> && !std::is_move_assignable_v<T>, "T can't move or copy\n");
	}


	//考虑将src中的T个位置尽量移动到to中，其中to在src到 t 之间
	template<typename T>
	void move_or_copy(T* src, size_t t, T* to)noexcept((std::is_nothrow_copy_assignable_v<T> || std::is_nothrow_move_assignable_v<T>)
													&& (std::is_nothrow_copy_constructible_v<T> || std::is_nothrow_move_constructible_v<T>))
	{
		if (src == to)
			return;
		if (to < src)
			return move_or_copy_con(src, t, to);
		if(src +t < to)
			return move_or_copy_con(src, t, to);
		move_or_copy_con(src + t - (to - src), to - src, src + t);
		try
		{
			move_or_copy_ass(src, t - (to - src), to);
		}
		catch (...)
		{
			for (auto i = src + t; i != to + t; ++i)
				(*i).~T();
			throw;
		}
	}
	
	//将 A<B> 替换为 A<C>
	template<typename AB,typename C>
	struct exchange;
	template<template<typename...>class A,typename B, typename C>
	struct exchange<A<B>, C>
	{
		using type = A<C>;
	};

	//使用std::excption
	using std::bad_alloc;
	using std::out_of_range;
	using std::runtime_error;
	using std::length_error;
}
