#pragma once
#include"../prepare.h"
#include"allocator.h"
namespace uuz
{

	template<typename T>
	size_t distance(const T& a, const T& b, typename decltype(std::declval<T>() - std::declval<T>())* = nullptr)noexcept(noexcept(b - a))
	{
		return b - a;
	}
	template<typename T>
	size_t distance(const T& a, const T& b)noexcept(noexcept(++b) && noexcept(a != b) && std::is_nothrow_constructible_v<T>)
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
	//
	template<typename T>
	struct storage
	{
		storage() = default;
		storage(const T& p)
		{
			new((void*)data) T(p);
		}
		storage(T&& p)
		{
			new((void*)data) T(std::move(p));
		}
		template<typename... Args>
		storage(const Args&&... args)
		{
			new((void*)data) T(std::forward<Args>(args)...);
		}

		char data[sizeof(T)];

		T* get_point()const noexcept
		{
			return (T*)data;
		}
		T& get()const noexcept
		{
			return *((T*)data);
		}
		T&& getmove()noexcept
		{
			return std::move(get());
		}

		void destroy()noexcept
		{
			get().~T();
		}
	};

}