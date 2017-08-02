#pragma once
#include<type_traits>
#include<cassert>
namespace uuz
{

	class nil {};
	template<typename T>
	class allocator 
	{
	public:
		using value_type = T;
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal = std::true_type;

		allocator()=default;
		allocator(const allocator& other)noexcept
		{
			;
		}
		template< typename U >
		allocator(const allocator<U>& other)noexcept
		{
			;
		}

		T* allocate(std::size_t n = 1)
		{
			auto k = ::operator new[](n * sizeof(T));
			return (T*)k;
		}
		void deallocate(T* p, std::size_t n)
		{
			::operator delete[](p);
		}
	};
	template< class T1, class T2 >
	bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs)
	{
		return true;
	}
	template< class T1, class T2 >
	bool operator!=(const allocator<T1>& lhs, const allocator<T2>& rhs)
	{
		return false;
	}

	template<typename T, typename F = void>
	struct is_nothrow_swap_alloc
	{
		static const bool value = false;
	};
	
	template<typename T>
	struct is_nothrow_swap_alloc<T,typename std::enable_if_t<T::is_always_equal::value || T::propagate_on_container_move_assignment::value>>
	{
		static const bool value = true;
	};
	
	

	template<typename T, typename b>
	struct pre_less
	{
		template<typename T1, typename T2>
		constexpr decltype(auto) operator()(const T1& a1, const T2& a2)const noexcept(noexcept(b()(a1,a2)))
		{
			return b()(a1, a2);
		}
	};
	template<typename T>
	struct pre_less<T, nil>
	{
		template<typename T1, typename T2>
		constexpr decltype(auto) operator()(const T1& a1, const T2& a2)const noexcept(noexcept(a1 < a2))
		{
 			return (a1 < a2);
		}
	};
	template<typename T>
	struct remove_reference
	{
		using type = T;
	};
	template<typename T>
	struct remove_reference<T&>
	{
		using type = T;
	};
	template<typename T>
	struct remove_reference<T&&>
	{
		using type = T;
	};
	template<typename T>
	typename uuz::remove_reference<T>::type&& move(T&& what) noexcept
	{
		return static_cast<typename uuz::remove_reference<T>::type&&>(what);
	}
	template<typename T>
	void mulitmove(const T* b,const T* e,const T*d) noexcept
	{
		for (auto it = b; it != e; ++it)
			auto p = new(d + (it - b)) T(std::move_if_noexcept(*it));
	}
	template<typename T,typename InputIt>
	using is_input = std::enable_if_t<std::is_same_v<T, std::decay_t<decltype(*std::declval<InputIt>())>>>;

	

	template<typename T>
	void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<std::is_nothrow_move_constructible_v<T> || (!std::is_nothrow_copy_constructible_v<T>&&std::is_move_constructible_v<T>)>* = nullptr)
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
		typename std::enable_if_t<(!std::is_nothrow_move_constructible_v<T> && std::is_nothrow_copy_constructible_v<T>)
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
	void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_copy_constructible_v<T>&& !std::is_move_constructible_v<T>>* = nullptr)
	{
		static_assert(false, "T can't move or copy\n");
	}

	
	template<typename T>
	void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<std::is_nothrow_move_assignable_v<T> || (!std::is_nothrow_copy_assignable_v<T>&&std::is_move_assignable_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_move_assignable_v<T>)
	{
		if (src == to)
			return;
		int i;
		try
		{
			for (i = 0; i != t; ++i)
				*(to + i) = std::move(*(src + i));
		}
		catch (...)
		{
			for (int j = 0; j != i; ++j)
				(to + i)->~T();
			throw;
		}
		return;
	}
	template<typename T>
	void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<(!std::is_nothrow_move_assignable_v<T> && std::is_nothrow_copy_assignable_v<T>)
								|| (!std::is_move_assignable_v<T> && std::is_copy_assignable_v<T>)>* = nullptr)
		noexcept(std::is_nothrow_copy_assignable_v<T>)
	{
		if (src == to)
			return;
		int i;
		try
		{
			for (i = 0; i != t; ++i)
				*(to + i) = *(src + i);
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
	void move_or_copy_ass(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_copy_assignable_v<T> && !std::is_move_assignable_v<T>>* = nullptr)
	{
		static_assert(false, "T can't move or copy\n");
	}

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
		move_or_copy_ass(src, t - (to - src), to);
	}
	template<typename T>
	size_t distance(const T& a, const T& b, typename decltype(std::declval<T>() - std::declval<T>())* =nullptr)noexcept(noexcept(b-a))
	{
		return b - a;
	}
	template<typename T>
	size_t distance(const T& a, const T& b)noexcept(noexcept(++b)&&noexcept(a!=b)&&std::is_nothrow_constructible_v<T>)
	{
		size_t aa = 0;
		auto k{ a };
		while (a != b)
		{
			++a;
			++aa;
		}
		return aa;
	}

	template<typename AB,typename C>
	struct exchange;
	template<template<typename...>class A,typename B, typename C>
	struct exchange<A<B>, C>
	{
		using type = A<C>;
	};


	using std::bad_alloc;
	using std::out_of_range;

}
