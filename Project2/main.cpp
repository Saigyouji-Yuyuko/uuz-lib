#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
//#include"ptr.h"
#include"test\test.h"
//#include"test\debug.h"
//#include"Container/set.h"
//#include"Container\allocator.h"
//#include"Container\array.h"
//#include"Container\vector.h"
//#include"Container\list.h"
//#include"Container\forward_list.h"
//#include"Container\set.h"
//#include"test\Container\vector_test.h"
//#include"Container\deque.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"
#include<future>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<fstream>
#include<deque>
#include<map>
#include<set>
#include<string>
#include<future>
#include<iostream>
#include<vector>
#include<memory>
#include<ctime>
#include<queue>
#include<list>
#include<functional>
#include<forward_list>
#include<array>
#include<numeric>
#include<iomanip>
#include<set>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<sstream>

// www.cnblogs.com/flyinghearts

#include <vector>
#include <algorithm>
#include <iterator>
#include <ctime>

template <class _ForwardIterator>
__forceinline
_ForwardIterator
__rotate_left(_ForwardIterator __first, _ForwardIterator __last)
{
	typedef typename std::iterator_traits<_ForwardIterator>::value_type value_type;
	value_type __tmp = std::move(*__first);
	_ForwardIterator __lm1 = std::move(std::next(__first), __last, __first);
	*__lm1 = std::move(__tmp);
	return __lm1;
}

template <class _BidirectionalIterator>
__forceinline
_BidirectionalIterator
__rotate_right(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
	typedef typename std::iterator_traits<_BidirectionalIterator>::value_type value_type;
	_BidirectionalIterator __lm1 = std::prev(__last);
	value_type __tmp = std::move(*__lm1);
	_BidirectionalIterator __fp1 = std::move_backward(__first, __lm1, __last);
	*__first = std::move(__tmp);
	return __fp1;
}

template <class _ForwardIterator>
__forceinline
_ForwardIterator
__rotate_forward(_ForwardIterator __first, _ForwardIterator __middle, _ForwardIterator __last)
{
	_ForwardIterator __i = __middle;
	while (true)
	{
		std::swap(*__first, *__i);
		++__first;
		if (++__i == __last)
			break;
		if (__first == __middle)
			__middle = __i;
	}
	_ForwardIterator __r = __first;
	if (__first != __middle)
	{
		__i = __middle;
		while (true)
		{
			std::swap(*__first, *__i);
			++__first;
			if (++__i == __last)
			{
				if (__first == __middle)
					break;
				__i = __middle;
			}
			else if (__first == __middle)
				__middle = __i;
		}
	}
	return __r;
}

template<typename _Integral>
__forceinline 
_Integral
__algo_gcd(_Integral __x, _Integral __y)
{
	do
	{
		_Integral __t = __x % __y;
		__x = __y;
		__y = __t;
	} while (__y);
	return __x;
}

template<typename _RandomAccessIterator>
__forceinline _RandomAccessIterator
__rotate_gcd(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;

	const difference_type __m1 = __middle - __first;
	const difference_type __m2 = __last - __middle;
	if (__m1 == __m2)
	{
		std::swap_ranges(__first, __middle, __middle);
		return __middle;
	}
	const difference_type __g = __algo_gcd(__m1, __m2);
	for (_RandomAccessIterator __p = __first + __g; __p != __first;)
	{
		value_type __t(std::move(*--__p));
		_RandomAccessIterator __p1 = __p;
		_RandomAccessIterator __p2 = __p1 + __m1;
		do
		{
			*__p1 = std::move(*__p2);
			__p1 = __p2;
			const difference_type __d = __last - __p2;
			if (__m1 < __d)
				__p2 += __m1;
			else
				__p2 = __first + (__m1 - __d);
		} while (__p2 != __p);
		*__p1 = std::move(__t);
	}
	return __first + __m2;
}

template <class _ForwardIterator>
__forceinline
_ForwardIterator
__rotate(_ForwardIterator __first, _ForwardIterator __middle, _ForwardIterator __last,
	std::forward_iterator_tag)
{
	typedef typename std::iterator_traits<_ForwardIterator>::value_type value_type;
	if (std::is_trivially_move_assignable<value_type>::value)
	{
		if (std::next(__first) == __middle)
			return __rotate_left(__first, __last);
	}
	return __rotate_forward(__first, __middle, __last);
}

template <class _BidirectionalIterator>
__forceinline
_BidirectionalIterator
__rotate(_BidirectionalIterator __first, _BidirectionalIterator __middle, _BidirectionalIterator __last,
	std::bidirectional_iterator_tag)
{
	typedef typename std::iterator_traits<_BidirectionalIterator>::value_type value_type;
	if (std::is_trivially_move_assignable<value_type>::value)
	{
		if (std::next(__first) == __middle)
			return __rotate_left(__first, __last);
		if (std::next(__middle) == __last)
			return __rotate_right(__first, __last);
	}
	return __rotate_forward(__first, __middle, __last);
}

template <class _RandomAccessIterator>
__forceinline
_RandomAccessIterator
__rotate(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last,
	std::random_access_iterator_tag)
{
	typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
	if (std::is_trivially_move_assignable<value_type>::value)
	{
		if (std::next(__first) == __middle)
			return __rotate_left(__first, __last);
		if (std::next(__middle) == __last)
			return __rotate_right(__first, __last);
		return __rotate_gcd(__first, __middle, __last);
	}
	return __rotate_forward(__first, __middle, __last);
}

template <class _ForwardIterator>
__forceinline
_ForwardIterator
rrotate(_ForwardIterator __first, _ForwardIterator __middle, _ForwardIterator __last)
{
	if (__first == __middle)
		return __last;
	if (__middle == __last)
		return __first;
	return __rotate(__first, __middle, __last,
		typename std::iterator_traits<_ForwardIterator>::iterator_category());
}



template<typename T, typename Iterator_tag>
void test1(T beg, T mid, T end, const Iterator_tag& iterator_tag)
{
	unsigned sum = 0;
	for (unsigned i = 0; i != 100; ++i)
		__rotate(beg, mid, end, iterator_tag);
		
	
}

template<typename T, typename Iterator_tag>
void test(T beg, T mid, T end, const Iterator_tag& iterator_tag)
{
	unsigned sum = 0;
	for (unsigned i = 0; i != 100; ++i)
		std::_Rotate_unchecked1(beg, mid, end, iterator_tag);


}
template < typename T >
struct Is_Void
{
	//enum nCon { value = std::is_void_v<T> ? 3 : 0 };
	constexpr static bool value = std::is_void_v<T> ;
};

template < typename T >
auto f()
{
	if constexpr (Is_Void<T>::value)
	{
		return(0);
	}
	else
	{
		T a{1.333};

		return(a);
	}
}

int main()
{
	const int N = 1e7;
	const int M = 1024;
	//uuz::println(f<void>());
	//uuz::println(f<double>());
//	const int M = 777;
	/*std::vector<std::vector<double>> msvc_random{8};
	std::vector<std::vector<double>> msvc_bidirectional{ 8 };
	std::vector<std::vector<double>> msvc_forward{ 8 };
	std::vector<std::vector<double>> libcxx_random{ 8 };
	std::vector<std::vector<double>> libcxx_bidirectional{ 8 };
	std::vector<std::vector<double>> libcxx_forward{ 8 };*/
	auto kk = 0;
	std::allocator_traits
	for(auto i =10;i<=1e4;i*=10,++kk)
	{
		std::vector<int> vec(i);
		for(int k=1;k<=10;++k)
		{
			std::vector<int>::iterator beg(vec.begin()), mid(vec.begin() + vec.size() * 9 / 10), end(vec.end());
			uuz::println(i);
			uuz::println(k);
			{

				uuz::println("random");
				{
					uuz::block_time a{ "msvc " };
					int j = 100;
					while(j--)
					test(beg, mid, end, std::random_access_iterator_tag());
					//msvc_random[kk].push_back(a.getms());

				}
				uuz::println("");
				{
					uuz::block_time a{ "libcxx " };
					int j = 100;
					while (j--)
					test1(beg, mid, end, std::random_access_iterator_tag());
					//libcxx_random[kk].push_back(a.getms());
				}
			}
			uuz::println("");
			{
				uuz::println("bidirectional");

				{
					uuz::block_time a{ "msvc " };
					int j = 100;
					while (j--)
					test(beg, mid, end, std::bidirectional_iterator_tag());
					//msvc_bidirectional[kk].push_back(a.getms());
				}
				uuz::println("");
				{
					uuz::block_time a{ "libcxx " };
					int j = 100;
					while (j--)
					test1(beg, mid, end, std::bidirectional_iterator_tag());
					//libcxx_bidirectional[kk].push_back(a.getms());
				}
			}
			uuz::println("");
			{
				uuz::println("forward");
				{
					uuz::block_time a{ "msvc " };
					int j = 100;
					while (j--)
					test(beg, mid, end, std::forward_iterator_tag());
					//msvc_forward[kk].push_back(a.getms());
				}
				uuz::println("");
				{
					uuz::block_time a{ "libcxx " };
					int j = 100;
					while (j--)
					test1(beg, mid, end, std::forward_iterator_tag());
					//libcxx_forward[kk].push_back(a.getms());
				}
			}
			uuz::println("");
			uuz::println("-------------------------------");
		}
		
		
		
	}
	//uuz::println("msvc_random");
	//for(auto i: msvc_random)
	//{
	//	for (auto j : i)
	//		std::cout << j << " ";
	//	std::cout << std::endl;
	//}
	//uuz::println("libcxx_random");
	//for (auto i : libcxx_random)
	//{
	//	for (auto j : i)
	//		std::cout << j << " ";
	//	std::cout << std::endl;
	//}
	//uuz::println("msvc_bidirectional");
	//for (auto i : msvc_bidirectional)
	//{
	//	for (auto j : i)
	//		std::cout << j << " ";
	//	std::cout << std::endl;
	//}
	//uuz::println("libcxx_bidirectional");
	//for (auto i : libcxx_bidirectional)
	//{
	//	for (auto j : i)
	//		std::cout << j << " ";
	//	std::cout << std::endl;
	//}
	//uuz::println("msvc_forward");
	//for (auto i : msvc_forward)
	//{
	//	for (auto j : i)
	//		std::cout << j << " ";
	//	std::cout << std::endl;
	//}
	//uuz::println("libcxx_forward");
	//for (auto i : libcxx_forward)
	//{
	//	for (auto j : i)
	//		std::cout << j << " ";
	//	std::cout << std::endl;
	//}
	system("pause");
	
}