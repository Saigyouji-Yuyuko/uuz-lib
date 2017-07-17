#pragma once
#include<type_traits>
namespace uuz
{

	class nil {};
	class allocator {};
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
	using std::swap;
	using std::forward;	
}
