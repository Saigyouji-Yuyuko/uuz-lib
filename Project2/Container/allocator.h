#pragma once
#include"../prepare.h"
namespace
{
	template<typename T>
	using has_pointer = typename T::pointer;

	template<typename T>
	using has_const_pointer = typename T::const_pointer;

	template<typename T>
	using has_void_pointer = typename T::void_pointer;

	template<typename T>
	using has_const_void_pointer = typename T::const_void_pointer;
	
	template<typename T>
	using has_difference_type = typename T::difference_type;

	template<typename T>
	using has_size_type = typename T::size_type;

	template<typename T>
	using has_propagate_on_container_copy_assignment = typename T::propagate_on_container_copy_assignment;

	template<typename T>
	using has_propagate_on_container_move_assignment = typename T::propagate_on_container_move_assignment;

	template<typename T>
	using has_propagate_on_container_swap = typename T::propagate_on_container_swap;

	template<typename T>
	using has_is_always_equal = typename T::is_always_equal;

	template<typename T,typename U>
	struct exchage_front;

	template<template<typename,typename...>class D,typename T,typename U,typename... Args>
	struct exchage_front<D<T, Args...>,U>
	{
		using type = D<U, Args...>;
	};

	template<template<typename>class D, typename T, typename U>
	struct exchage_front<D<T>, U>
	{
		using type = D<U>;
	};

	template<typename A,typename T>
	using what_fuck = typename A::template rebind<T>::other;

	template<typename A, typename S, typename H>
	using has_allocate_hint = decltype(std::declval<A>().allocate(std::declval<S>(), std::declval<H>()));

	template<typename A,typename T,typename...Args>
	using has_allocate_construct = decltype(std::declval<A>().construct(std::declval<T*>(), std::declval<Args>()...));

	template<typename A, typename T>
	using has_allocate_destroy = decltype(std::declval<A>().destroy(std::declval<T*>()));
	
	template<typename A>
	using has_allocate_max_size = decltype(std::declval<A>().max_size());

	template<typename A>
	using has_allocate_select_on_container_copy_construction = decltype(std::declval<A>().select_on_container_copy_construction());
}
namespace uuz
{
	template<typename T>
	class allocator
	{
	public:
		using value_type = T;
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal = std::true_type;
		
		//deprecated in C++17

		//using pointer = T*;
		//using const_pointer = const T*;
		//using reference = T&;
		//using const_reference = const reference;
		//using size_type = size_t;
		//using difference_type = ptrdiff_t;
		//template<typename U>
		//using rebind = struct { using other = allocator<U>; };


	public:
		

		allocator() = default;
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
	
		void deallocate(T* p, std::size_t n)const
		{
			::operator delete[](p);
		}


		//deprecated in C++17

		//pointer address(reference x) const;
		//const_pointer address(const_reference x) const;
		//size_type max_size() const;
		//template< class U, class... Args >
		//void construct(U* p, Args&&... args);
		//template< class U >
		//void destroy(U* p);
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
	struct is_nothrow_swap_alloc<T, typename std::enable_if_t<T::is_always_equal::value || T::propagate_on_container_move_assignment::value>>
	{
		static const bool value = true;
	};

	template< typename Alloc >
	struct allocator_traits
	{
		//真J8蠢，我为了这个写了个detected系列，写了个point_traits
	public:
		using allocator_type = Alloc;
		using value_type = typename allocator_type::value_type;
		using pointer = experimental::detected_or_t<value_type*, has_pointer, allocator_type>;
		using const_pointer = experimental::detected_or_t<typename pointer_traits<pointer>::template rebind<const value_type>,has_const_pointer, allocator_type>;
		using void_pointer = experimental::detected_or_t<typename pointer_traits<pointer>::template rebind<void>, has_void_pointer, allocator_type>;
		using const_void_pointer = experimental::detected_or_t<typename pointer_traits<pointer>::template rebind<const void>, has_const_void_pointer, allocator_type>;
		using difference_type = experimental::detected_or_t<typename pointer_traits<pointer>::difference_type, has_difference_type, allocator_type>;
		using size_type = experimental::detected_or_t<typename std::make_unsigned<difference_type>::type, has_size_type, allocator_type>;
		using propagate_on_container_move_assignment = experimental::detected_or_t<std::false_type, has_propagate_on_container_move_assignment, allocator_type>;
		using propagate_on_container_copy_assignment = experimental::detected_or_t<std::false_type, has_propagate_on_container_copy_assignment, allocator_type>;
		using propagate_on_container_swap = experimental::detected_or_t<std::false_type, has_propagate_on_container_swap, allocator_type>;
		using is_always_equal = experimental::detected_or_t<typename std::is_empty<Alloc>::type, has_is_always_equal, allocator_type>;

		template<typename T>
		using rebind_alloc = experimental::detected_or_t<typename exchage_front<Alloc, T>::type, what_fuck, allocator_type, T>;

		template<typename T>
		using rebind_traits = allocator_traits<rebind_alloc<T>>;

		static INLINE pointer allocate(Alloc& a, size_type n)noexcept(noexcept(a.allocate(n)))
		{
			return a.allocate(n);
		}
		static INLINE pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
		{
			if constexpr(experimental::is_detected_v<has_allocate_hint, Alloc, size_type, const_void_pointer>)
			{
				return a.allocate(n, hint);
			}
			else
			{
				return a.allocate(n);
			}
		}
	
		static INLINE void deallocate(Alloc& a, pointer p, size_type n)
		{
			return a.deallocate(p, n);
		}

		template< class T, class... Args >
		static INLINE void construct(Alloc& a, T* p, Args&&... args)
		{
			if constexpr(experimental::is_detected_v<has_allocate_construct, Alloc, T, Args...>)
			{
				a.construct(p, std::forward<Args>(args)...);
			}
			else
			{
				::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
			}
		}

		template< class T >
		static INLINE void destroy(Alloc& a, T* p)
		{
			if constexpr(experimental::is_detected_v<has_allocate_destroy, Alloc, T>)
			{
				a.destroy(p);
			}
			else
			{
				p->~T();
			}
		}

		static INLINE size_type max_size(const Alloc& a)
		{
			if constexpr(experimental::is_detected_v<has_allocate_destroy, Alloc>)
			{
				return a.max_size();
			}
			else
			{
				return std::numeric_limits<size_type>::max() / sizeof(value_type);
			}
		}

		static INLINE Alloc select_on_container_copy_construction(const Alloc& a)
		{
			if constexpr(experimental::is_detected_v<has_allocate_select_on_container_copy_construction, Alloc>)
			{
				return a.select_on_container_copy_construction();
			}
			else
			{
				return a;
			}
		}
	};

}