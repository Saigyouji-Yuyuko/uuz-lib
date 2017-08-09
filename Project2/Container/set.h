#pragma once
#include"container.h"
#include"rbtree.h"
#include"pair.h"
namespace uuz
{
	template<typename Key,typename Compare = pre_less<Key,nil>,typename Allocator = uuz::allocator<Key>> 
	class set:public rb_tree<Key,Compare,Allocator>
	{
		using base = rb_tree<Key, Compare, Allocator>;
		using iterator = typename base::iterator;
	public:
		set() : set(Compare()) {}
		explicit set(const Compare& comp,const Allocator& alloc = Allocator()):base(comp,alloc){}
		explicit set(const Allocator& alloc) :base(alloc){}
		template< typename InputIt ,typename = is_input<Key,InputIt>>
		set(const InputIt& first, const InputIt& last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : base(comp, alloc)
		{
			inid(first, last);
		}
		template< class InputIt, typename = is_input<Key, InputIt>>
		set(InputIt first, InputIt last, const Allocator& alloc): set(first, last, Compare(), alloc) {}
		set(const set& other):base(other){}
		set(const set& other, const Allocator& alloc):base(other,alloc){}
		set(set&& other):base(std::move(other)){}
		set(set&& other, const Allocator& alloc):base(std::move(other),alloc){}
		set(std::initializer_list<Key> init,const Compare& comp = Compare(),const Allocator& alloc = Allocator()):set(init.begin(),init.end(),comp,alloc){}
		set(std::initializer_list<Key> init, const Allocator& alloc): set(init, Compare(), alloc) {}

		set& operator=(const set& other)
		{
			if (this != &other)
			{
				auto temp(other);
				this->swap(other);
			}
			return *this;
		}
		set& operator=(set&& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if (this != &other)
			{
				auto temp(std::move(other));
				this->swap(other);
			}
			return *this;
		}
		set& operator=(std::initializer_list<Key> ilist)
		{
			if (this != &other)
			{
				auto temp(ilist);
				this->swap(other);
			}
			return *this;
		}

		using base::insert;
		insert_return_type insert(node_type&& nh)
		{

		}
		iterator insert(const iterator& hint, node_type&& nh)
		{
				
		}
		
		using base::erase;
		size_type erase(const key_type& key)
		{
			auto k = truefind(key);
			if (k)
			{
				dele(k);
				return 1;
			}
			return 0;	
		}

		iterator find(const Key& key)noexcept
		{
			return iterator(truefind(key));
		}
		const iterator find(const Key& key) const noexcept
		{
			return iterator(truefind(key));
		}
		template< typename K,typename = std::enable_if_t<std::is_constructible_v<Key,K&>> > 
		iterator find(const K& x)noexcept
		{
			return iterator(truefind(Key(x)));
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>> >
		const iterator find(const K& x) const noexcept
		{
			return iterator(truefind(Key(x)));
		}

		iterator lower_bound(const Key& key)noexcept
		{
			return iterator(low_bound(key));
		}
		const iterator lower_bound(const Key& key) const noexcept
		{
			return iterator(low_bound(key));
		}
		template<typename K ,typename = std::enable_if_t<std::is_constructible_v<Key,K&>>>
		iterator lower_bound(const K& x)noexcept
		{
			return iterator(low_bound(Key(x)));
		}
		template<typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>>>
		const iterator lower_bound(const K& x) const noexcept
		{
			return iterator(low_bound(Key(x)));
		}

		iterator upper_bound(const Key& key)noexcept
		{
			return iterator(up_bound(key));
		}
		const iterator upper_bound(const Key& key) const
		{
			return iterator(up_bound(key));
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>>>
		iterator upper_bound(const K& x)
		{
			return iterator(up_bound(Key(x)));
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>>>
		const_iterator upper_bound(const K& x) const
		{
			return iterator(up_bound(Key(x)));
		}







	};
	template< class Key, class Compare, class Alloc >
	bool operator==(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs);
		template< class Key, class Compare, class Alloc >
	bool operator!=(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs);
		template< class Key, class Compare, class Alloc >
	bool operator<(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs);
		template< class Key, class Compare, class Alloc >
	bool operator<=(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs);
		template< class Key, class Compare, class Alloc >
	bool operator>(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs);
		template< class Key, class Compare, class Alloc >
	bool operator>=(const set<Key, Compare, Alloc>& lhs,
		const set<Key, Compare, Alloc>& rhs);

	template< class Key, class Compare, class Alloc >
	void swap(set<Key, Compare, Alloc>& lhs,
		set<Key, Compare, Alloc>& rhs) noexcept(/* see below */);
}