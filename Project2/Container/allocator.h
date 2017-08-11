#pragma once
namespace uuz
{
	template<typename T>
	class allocator
	{
	public:
		using value_type = T;
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal = std::true_type;

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
	struct is_nothrow_swap_alloc<T, typename std::enable_if_t<T::is_always_equal::value || T::propagate_on_container_move_assignment::value>>
	{
		static const bool value = true;
	};
}