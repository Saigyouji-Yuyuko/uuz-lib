#pragma once
#include"pair.h"
#include"rbtree.h"
#include"container.h"
namespace
{
	template<typename Key, typename T, typename compare>
	struct map_less
	{
		map_less() :cmp(compare()) {}
		map_less(const compare& t) :cmp(t) {}

		bool operator()(const Key& a, const pair<const Key, T>& b)noexcept(noexcept(cmp(a, a)))
		{
			return cmp(a, b.first);
		}

		bool operator()(const pair<const Key, T>& a, const pair<const Key, T>& b)noexcept(noexcept(cmp(a.first, a.first)))
		{
			return cmp(a.first, b.first);
		}

		bool operator()(const pair<const Key, T>& a, const Key& b)noexcept(noexcept(cmp(b, b)))
		{
			return cmp(a.first, b);
		}

		bool operator()(const Key& a, const Key& b)noexcept(noexcept(cmp(a, a)))
		{
			return cmp(a, b);
		}

		compare cmp;
	};

}
namespace uuz
{
	template<typename Key,typename T,typename C=less<Key>,typename A = uuz::allocator<pair<const Key,T>>>
	class map: rb_tree<pair<const Key,T>,map_less<Key,T,C>,A>
	{
		
	};
}
