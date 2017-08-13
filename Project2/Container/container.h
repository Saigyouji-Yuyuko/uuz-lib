#pragma once
#include"../prepare.h"
#include"allocator.h"
#include<initializer_list>
namespace uuz
{
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