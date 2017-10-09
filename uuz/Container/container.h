#pragma once
#include"../prepare.h"
#include"iterator.h"
#include"algorithm.h"
#include"allocator.h"
#include<initializer_list>
#include"pair.h"
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
		storage(Args&&... args)
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

	template< typename ForwardIt >
	std::enable_if_t<std::is_trivially_destructible_v<typename iterator_traits<ForwardIt>::value_type>> 
		destroy(ForwardIt , ForwardIt )noexcept
	{
	}

	template< typename ForwardIt >
	std::enable_if_t<!std::is_trivially_destructible_v<typename iterator_traits<ForwardIt>::value_type>>
		destroy(ForwardIt first, ForwardIt last)noexcept
	{
		for (; first != last; ++first)
			destroy_at(addressof(*first));
	}

	


}