#pragma once
#include<type_traits>
namespace uuz
{
	template<typename T1, typename T2>
	struct pair
	{
		using first_type = T1;
		using second_type = T2;
		
		template<typename U1 = T1,typename U2 =T2,typename = std::enable_if_t<std::is_default_constructible_v<U1>&&std::is_default_constructible_v<U2>>
			, std::enable_if_t<std::is_trivially_constructible_v<U1> || std::is_trivially_constructible_v<U2>, int> = 0>
		constexpr pair()noexcept(std::is_nothrow_default_constructible_v<T1> && std::is_nothrow_default_constructible_v<T2>)
			:first(), second() {}

		template<typename U1 = T1, typename U2 = T2, typename = std::enable_if_t<std::is_default_constructible_v<U1>&&std::is_default_constructible_v<U2>>
			, std::enable_if_t<!std::is_trivially_constructible_v<U1> && !std::is_trivially_constructible_v<U2>, int> = 0>
		constexpr explicit pair()noexcept(std::is_nothrow_default_constructible_v<T1> && std::is_nothrow_default_constructible_v<T2>)
					:first(), second() {}

		pair(const pair& p) = default;
		pair(pair&& p) = default;

		template<typename U1 = T1, typename U2 = T2, typename = std::enable_if_t<std::is_copy_constructible_v<U1>&&std::is_copy_constructible_v<U2>>
			, std::enable_if_t<!std::is_constructible_v<U1,const U1&>&&!std::is_constructible_v<U2,const U2&>,int> = 0>
		explicit constexpr pair(const T1& x, const T2& y)noexcept(std::is_nothrow_copy_constructible_v<T1> && std::is_nothrow_copy_constructible_v<T2>)
				:first(x),second(y){}
		
		template<typename U1 = T1, typename U2 = T2, typename = std::enable_if_t<std::is_copy_constructible_v<U1>&&std::is_copy_constructible_v<T2>>
			, std::enable_if_t<std::is_constructible_v< U1,const U1&> || std::is_constructible_v<U2,const U2& >,int> = 0>
		constexpr pair(const T1& x, const T2& y)noexcept(std::is_nothrow_copy_constructible_v<T1> && std::is_nothrow_copy_constructible_v<T2>)
					:first(x), second(y) {}

		template< typename U1, typename U2 , typename = std::enable_if_t<std::is_constructible_v<T1, U1&&> && std::is_constructible_v<T2, U2&&>>
			,std::enable_if_t<!std::is_convertible_v<U1&&, T1> && !std::is_convertible_v<U2&&, T2>,int> = 0>
		explicit constexpr pair(U1&& x, U2&& y)noexcept(std::is_nothrow_constructible_v<T1, U1&&> && std::is_nothrow_constructible_v<T2, U2&&>)
				:first(std::forward<U1>(x)), second(std::forward<U2>(y)){}

		template< typename U1, typename U2, typename = std::enable_if_t<std::is_constructible_v<T1, U1&&> && std::is_constructible_v<T2, U2&&>>
			, std::enable_if_t<std::is_convertible_v<U1&&, T1> || std::is_convertible_v<U2&&, T2>,int> = 0>
		constexpr pair(U1&& x, U2&& y)noexcept(std::is_nothrow_constructible_v<T1, U1&&> && std::is_nothrow_constructible_v<T2, U2&&>)
				:first(std::forward<U1>(x)), second(std::forward<U2>(y)) {}

		template< typename U1, typename U2, typename = std::enable_if_t<std::is_constructible_v<T1, const U1&> && std::is_constructible_v<T2, const U2&>>
			, std::enable_if_t< !std::is_convertible_v<const U1&, T1> && !std::is_convertible_v<const U2&, T2>,int> = 0>
		explicit constexpr pair(const pair<U1, U2>& p)noexcept(std::is_nothrow_constructible_v<T1,const U1&> && std::is_nothrow_constructible_v<T2,const U2&>)
				:first(p.first),second(p.second){}

		template< typename U1, typename U2, typename = std::enable_if_t<std::is_constructible_v<T1, const U1&> && std::is_constructible_v<T2, const U2&>>
			,std::enable_if_t<std::is_convertible_v<const U1&, T1> || std::is_convertible_v<const U2&, T2>, int> = 0 >
		constexpr pair(const pair<U1, U2>& p)noexcept(std::is_nothrow_constructible_v<T1, const U1&> && std::is_nothrow_constructible_v<T2, const U2&>)
				:first(p.first), second(p.second) {}
		
		template< typename U1, typename U2, typename = std::enable_if_t<std::is_constructible_v<T1, U1&&> && std::is_constructible_v<T2, U2&&>>
			, std::enable_if_t<!std::is_convertible_v<U1&&, T1> && !std::is_convertible_v<U2&&, T2>,int> = 0>
		explicit constexpr pair(pair<U1, U2>&& p)noexcept(std::is_nothrow_constructible_v<T1, U1&&> && std::is_nothrow_constructible_v<T2, U2&&>)
				:first(std::forward<U1>(p.first)), second(std::forward<U2>(p.second)) {}

		template< typename U1, typename U2, typename = std::enable_if_t<std::is_constructible_v<T1, U1&&> && std::is_constructible_v<T2, U2&&>>
			, std::enable_if_t<std::is_convertible_v<U1&&, T1> || std::is_convertible_v<U2&&, T2>,int> = 0>
		constexpr pair(pair<U1, U2>&& p)noexcept(std::is_nothrow_constructible_v<T1, U1&&> && std::is_nothrow_constructible_v<T2, U2&&>)
				:first(std::forward<U1>(p.first)), second(std::forward<U2>(p.second)) {}

		template< class... Args1, class... Args2 >
		pair(std::piecewise_construct_t, std::tuple<Args1...> first_args, std::tuple<Args2...> second_args)
			: pair(first_args, second_args, std::index_sequence_for<Args1...>(), std::index_sequence_for<Args2...>()) {}
		

		pair& operator=(const pair& other)noexcept(std::is_nothrow_copy_assignable_v<T1> && std::is_nothrow_copy_assignable_v<T2>)
		{
			static_assert(!std::is_copy_assignable_v<T1> || !std::is_copy_assignable_v<T2>, "this function no defined");
			first = other.first;
			second = other.second;
			return *this;
		}

		template< typename U1, typename U2>
		pair& operator=(const pair<U1, U2>& other)noexcept(std::is_nothrow_assignable_v<T1&,const U1&> && std::is_nothrow_assignable_v<T2&,const U2&>)
		{
			static_assert(!std::is_assignable_v<T1&, const U1&> || !std::is_assignable_v<T2&, const U2&>, "this function no defined");
			first = other.first;
			second = other.second;
			return *this;
		}

		pair& operator=(pair&& other) noexcept(std::is_nothrow_move_assignable_v<T1> && std::is_nothrow_move_assignable_v<T2>)
		{
			static_assert(!std::is_move_assignable_v<T1> || !std::is_move_assignable_v<T2>, "this function no defined");
			first = std::move(other.first);
			second = std::move(other.second);
			return *this;
		}

		template<typename U1, typename U2>
		pair& operator=(pair<U1, U2>&& other)noexcept(std::is_nothrow_assignable_v<T1&,U1&&> && std::is_nothrow_assignable_v<T2&,U2&&>)
		{
			static_assert(!std::is_assignable_v<T1&, U1&&> || !std::is_assignable_v<T2&, U2&&>, "this function no defined");
			first = std::forward<U1>(other.first);
			second = std::forward<U2>(other.second);
			return *this;
		}
		

		template<typename = std::enable_if_t<std::is_swappable_v<first_type> && std::is_swappable_v<second_type>>>
		void swap(pair& other)noexcept(std::is_nothrow_swappable_v<first_type> &&std::is_nothrow_swappable_v<second_type>)
		{
			swap(other.first, first);
			swap(other.second, second);
		}

		T1 first;
		T2 second;
	private:
		template<class _Tuple1,class _Tuple2,std::size_t... _Indexes1,std::size_t... _Indexes2>
		pair(_Tuple1& _Val1,_Tuple2& _Val2,std::index_sequence<_Indexes1...>,std::index_sequence<_Indexes2...>)
			:first(std::get<_Indexes1>(std::move(_Val1))...),second(std::get<_Indexes2>(std::move(_Val2))...){}
	};
	namespace
	{
		template<typename T>
		struct help_type
		{
			using type = T;
		};
		template<typename T>
		struct help_type<std::reference_wrapper<T>>
		{
			using type = T&;
		};
		
		template<typename T>
		using make_type = typename help_type<std::decay_t<T>>::type;

		template<typename T,typename U>
		constexpr int compare(const pair<T, U>& lhs, const pair<T, U>& rhs)noexcept(noexcept(lhs.first < rhs.first) && noexcept(lhs.second < rhs.second) && noexcept(lhs.first > rhs.first) && noexcept(lhs.second > rhs.second))
		{
			if (lhs.first < rhs.first)
				return -1;
			else if (lhs.first > rhs.first)
				return 1;
			else
			{
				if (lhs.second < rhs.second)
					return -1;
				else if (lhs.second > rhs.second)
					return 1;
			}
			return 0;
		}

	}
	template<typename T1, typename T2>
	constexpr pair<make_type<T1>, make_type<T2>> make_pair(T1&& a, T2&& b)noexcept(noexcept(pair<make_type<T1>, make_type<T2>>(std::forward<T1>(a), std::forward<T1>(b))))
	{
		return pair<make_type<T1>, make_type<T2>>(std::forward<T1>(a), std::forward<T2>(b));
	}

	template< typename T1, typename T2 >
	constexpr bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)noexcept(noexcept(compare(lhs, rhs)))
	{
		return compare(lhs, rhs) == 0;
	}
	template< typename T1, typename T2 >
	constexpr bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)noexcept(noexcept(compare(lhs, rhs)))
	{
		return compare(lhs, rhs) != 0;
	}
	template< typename T1, typename T2 >
	constexpr bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)noexcept(noexcept(compare(lhs, rhs)))
	{
		return compare(lhs, rhs) < 0;
	}
	template< typename T1, typename T2 >
	constexpr bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)noexcept(noexcept(compare(lhs, rhs)))
	{
		return compare(lhs, rhs) <= 0;
	}
	template< typename T1, typename T2 >
	constexpr bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)noexcept(noexcept(compare(lhs, rhs)))
	{
		return compare(lhs, rhs) > 0;
	}
	template< typename T1, typename T2 >
	constexpr bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)noexcept(noexcept(compare(lhs, rhs)))
	{
		return compare(lhs, rhs) >= 0;
	}

	template< typename T1, typename T2 >
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

	/*template<typename T1, typename T2, bool = >
	struct compressed_pair
	{

	};*/




}

