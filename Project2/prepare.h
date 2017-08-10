#pragma once
#include<type_traits>
#include<cassert>
namespace uuz
{

	class nil {};

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
