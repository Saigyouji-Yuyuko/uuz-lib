#pragma once
#include<type_traits>
#include<cassert>
#include<exception>
#include<stdexcept>
namespace uuz
{
	//默认less仿函数
	template<typename T>
	struct less
	{
		constexpr bool operator()(const T& a, const T& b)const noexcept(noexcept(a<b))
		{
			return a < b;
		}
	};


	template<typename T,typename InputIt>//测试InputIt是否为T类型的迭代器或指针
	using is_input = std::enable_if_t<std::is_same_v<T, std::decay_t<decltype(*std::declval<InputIt>())>>>;


	//考虑将src中的T个位置尽量移动到to中，其中to是脏数据
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
	[[noreturn]] void move_or_copy_con(T* src, size_t t, T* to,
		typename std::enable_if_t<!std::is_copy_constructible_v<T>&& !std::is_move_constructible_v<T>>* = nullptr)
	{
		static_assert(!std::is_copy_assignable_v<T> && !std::is_move_assignable_v<T>, "T can't move or copy\n");
	}

	//考虑将src中的T个位置尽量移动到to中，其中to是有效数据
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
		typename std::enable_if_t<(!std::is_nothrow_move_assignable_v<T> && std::is_nothrow_copy_assignable_v<T>)
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
		typename std::enable_if_t<!std::is_copy_assignable_v<T> && !std::is_move_assignable_v<T>>* = nullptr)
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
}
