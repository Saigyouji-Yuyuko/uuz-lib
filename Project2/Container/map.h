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
	template<typename Key,typename T,typename Compare =less<Key>,typename Allocator = uuz::allocator<pair<const Key,T>>>
	class map: rb_tree<pair<const Key,T>,map_less<Key,T, Compare>, Allocator>
	{
	public:
		using iterator = rb_tree<pair<const Key, T>, map_less<Key, T, Compare>, Allocator>::iterator;
		using value_type = pair<const Key, T>;
		map() : map(Compare()) {}
		explicit map(const Compare& comp,const Allocator& alloc = Allocator()):rb_tree(comp,alloc){}
		explicit map(const Allocator& alloc):rb_tree(alloc){}
		template< class InputIterator,typename = is_input<value_type,InputIterator>>
		map(InputIterator first, InputIterator last,const Compare& comp = Compare(),const Allocator& alloc = Allocator()):rb_tree(comp,alloc)
		{
			inid(first, last);
		}
		template< class InputIterator, typename = is_input<value_type, InputIterator>>
		map(InputIterator first, InputIterator last,const Allocator& alloc):rb_tree(alloc)
		{
			inid(first, last);
		}
		map(const map& other):rb_tree(other){}
		map(const map& other, const Allocator& alloc):rb_tree(other,alloc){}
		map(map&& other):rb_tree(std::move(other)){}
		map(map&& other, const Allocator& alloc):rb_tree(std::move(other),alloc) {}
		map(std::initializer_list<value_type> init,const Compare& comp = Compare(),const Allocator& alloc = Allocator())
			:map(init.begin(),init.end(),comp,alloc){}
		map(std::initializer_list<value_type> init,const Allocator& alloc)
			:map(init.begin(),init.end(),alloc){}

		map& operator=(const map& other)
		{
			rb_tree::operator=(other);
			return *this;
		}

		map& operator=(map&& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			rb_tree::operator=(std::move(other));
			return *this;
		}
		map& operator=(std::initializer_list<value_type> ilist)
		{
			map t(ilist, alloc);
			this->swap(t);
			return *this;
		}

		T& at(const Key& key)
		{
			auto k = truefind(key);
			if (k != &nul)
				return (k->get()).second;
			throw(out_of_range(""));
		}
		const T& at(const Key& key) const
		{
			auto k = truefind(key);
			if (k != &nul)
				return (k->get()).second;
			throw(out_of_range(""));
		}

		T& operator[](const Key& key)
		{
			auto k = ifind(key);
			if(!k || cmp(key,k->get()) || cmp(k->get(),key))
			{
				auto l = make(value_type(key, T{}));
				auto t = Insert(l, k);
				return t->get().second;
			}
			return k->get().second;
		}
		T& operator[](Key&& key)
		{
			auto k = ifind(key);
			if (!k || cmp(key, k->get()) || cmp(k->get(), key))
			{
				auto l = make(value_type(std::move(key), T{}));
				auto t = Insert(l, k);
				return t->get().second;
			}
			return k->get().second;
		}


	};
}
