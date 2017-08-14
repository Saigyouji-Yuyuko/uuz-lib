#pragma once
#include"container.h"
namespace uuz
{
	template<typename T1, typename T2>
	class pair
	{
	private:

		using self = pair<T1, T2>;

	public:

		pair() = default;
		constexpr pair(const T1& aa, const T2& bb)noexcept :a(aa), b(bb) {} //noexcept?
		pair(T1&& aa, T2&& bb) :a(std::move(aa)), b(std::move(bb)) {}  //noexcept?

		template<typename U1, typename U2>
		pair(pair<U1, U2>&& p) : a(std::move(p.a)), b(std::move(p.b)) {}  //noexcept?
		template<typename U1, typename U2>
		constexpr pair(const pair<U1, U2>& p) : a(p.a), b(p.b) {}  //noexcept?

		friend constexpr bool operator<(const self& aa, const self&bb)noexcept(noexcept(aa.a < bb.a) && noexcept(aa.b < bb.b))
		{
			return aa.a < bb.a || (!(bb.a < aa.a) && aa.b < bb.b);
		}
		friend constexpr bool operator<(const T1& aa, const self&bb)noexcept(noexcept(aa < bb.a))
		{
			return aa < bb.a;
		}
		friend constexpr bool operator==(const self& aa, const self& bb)noexcept(noexcept(aa.a == bb.a) && noexcept(aa.b == bb.b))
		{
			return aa.a == bb.a && aa.b == bb.b;
		}
		friend constexpr bool operator>(const self& aa, const self& bb)noexcept(noexcept(aa < bb))
		{
			return bb < aa;
		}
		friend constexpr bool operator!=(const self& aa, const self& bb)noexcept(noexcept(aa == bb))
		{
			return !(aa == bb);
		}
		friend constexpr bool operator<=(const self& aa, const self&bb)noexcept(noexcept(aa > bb))
		{
			return !(aa > bb);
		}
		friend constexpr bool operator>=(const self& aa, const self&bb)noexcept(noexcept(aa < bb))
		{
			return !(aa < bb);
		}

	
		T1 first;
		T2 second;
	};

	template<typename T1, typename T2>
	constexpr pair<T1, T2> make_pair(const T1& a, const T2& b) noexcept(noexcept(pair<T1, T2>(a, b)))
	{
		return pair<T1, T2>(a, b);
	}

	template<typename T1, typename T2>
	constexpr pair<T1, T2> make_pair(T1&& a, T2&& b)noexcept(noexcept(pair<T1, T2>(std::forward<T1>(a), std::forward<T1>(b))))
	{
		return pair<T1, T2>(std::forward<T1>(a), std::forward<T1>(b));
	}

}
