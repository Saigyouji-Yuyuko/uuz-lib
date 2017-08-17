#pragma once
#include"container.h"
#include"rbtree.h"
#include"pair.h"
namespace uuz
{
	template<typename Key, typename Compare, typename Allocator>
	class set;

	template<typename Key, typename Compare, typename Allocator>
	class multiset;

	template<typename T,typename Allocator>
	class set_node:public to_node<T,Allocator>
	{
	public:
		template<typename Key, typename Compare, typename A>
		friend class set;
		template<typename Key, typename Compare, typename A>
		friend class multiset;

		using value_type = T;
		using allocator_type = Allocator;
		constexpr set_node() noexcept:to_node() {}
		set_node(set_node&& nh) noexcept :to_node(std::move(nh)){}
		set_node(rb_tree_node<T>* a, const allocator_type& b) :to_node(a,b){}

		set_node& operator=(set_node&& nh)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			return to_node::operator=(std::move(nh));
		}

		value_type& value() const
		{
			return t->get();
		}

	};



	template<typename Key,typename Compare = less<Key>,typename Allocator = uuz::allocator<Key>> 
	class set:public rb_tree<Key,Compare,Allocator>
	{
		using base = rb_tree<Key, Compare, Allocator>;
		
	public:
		using allocator_type = typename base::Allocator;
		using iterator = typename base::iterator;
		using node_type = set_node<Key, allocator_type>;
		using insert_return_type = struct
		{
			iterator position;
			bool inserted;
			node_type node;
		};

		set() : rb_tree(Compare(), Allocator()) {}
		explicit set(const Compare& comp, const Allocator& alloc = Allocator()) :rb_tree(comp, alloc) {}
		explicit set(const Allocator& alloc) :rb_tree(alloc) {}
		template< typename InputIt ,typename =uuz::is_input<Key,InputIt>>
		set(const InputIt& first, const InputIt& last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : rb_tree(comp, alloc)
		{
			inid(first, last);
		}
		template< class InputIt, typename = is_input<Key, InputIt>>
		set(const InputIt& first,const InputIt& last, const Allocator& alloc): set(first, last, Compare(), alloc) {}
		set(const set& other):rb_tree(other){}
		set(const set& other, const Allocator& alloc):rb_tree(other,alloc){}
		set(set&& other):rb_tree(std::move(other)){}
		set(set&& other, const Allocator& alloc):rb_tree(std::move(other),alloc){}
		set(std::initializer_list<Key> init,const Compare& comp = Compare(),const Allocator& alloc = Allocator()):set(init.begin(),init.end(),comp,alloc){}
		set(std::initializer_list<Key> init, const Allocator& alloc): set(init, Compare(), alloc) {}

		set& operator=(const set& other)
		{
			rb_tree::operator=(other);
			return *this;
		}
		set& operator=(set&& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			rb_tree::operator=(std::move(other));
			return *this;
		}
		set& operator=(std::initializer_list<Key> ilist)
		{
			set temp(ilist,alloc);
			this->swap(temp);
			return *this;
		}

		using rb_tree::insert;
		insert_return_type insert(node_type&& nh)
		{
			auto z = ifind(nh.value());
			if(!z &&( cmp(z->get(),nh.value())|| cmp(nh.value(),z->get())))
			{
				if(get_allcoator() == nh.get_allocator())
				{
					auto k = Insert(nh.t, z);
					nh.t = nullptr;
					return insert_return_type{ iterator(k),true,std::move(nh) };
				}
				else
				{
#ifdef DEBUG
					assert(false, "It's undefined that allocator is not equeal");
#else
					auto k = insert(hint, std::move(nh.t->get()));
					nh->~to_node();
					return insert_return_type{k,true,std::move(nh) };
#endif 
				}
			}
			return insert_return_type{ iterator(z),false,std::move(nh) };
		}
		
		node_type extract(const iterator position)
		{
			if (position == end())
				return node_type();
			auto k = rb_tree::extract(position);
			return node_type(k, get_allocator());
		}
		node_type extract(const Key& x)
		{
			auto k = find(x);
			return extract(k);
		}

		using rb_tree::erase;
		size_t erase(const Key& key)
		{
			auto k = truefind(key);
			if (!isnul(k))
			{
				dele(k);
				return 1;
			}
			return 0;	
		}

		size_t count(const Key& key) const noexcept
		{
			return find(key) == end() ? 0 : 1;
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>> >
		size_t count(const K& x) const noexcept
		{
			return find(x) == end() ? 0 : 1;
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
		const iterator upper_bound(const K& x) const
		{
			return iterator(up_bound(Key(x)));
		}

		pair<iterator, iterator> equal_range(const Key& key)noexcept
		{
			auto k = eqrange(key);
			return make_pair(iterator(k.first),iterator(k.second));
		}
		pair<const iterator, const iterator> equal_range(const Key& key) const noexcept
		{
			auto k = eqrange(key);
			return make_pair(iterator(k.first), iterator(k.second));
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>>>
		pair<iterator, iterator> equal_range(const K& x)noexcept
		{
			auto k = eqrange(Key(x));
			return make_pair(iterator(k.first), iterator(k.second));
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>>>
		pair<const iterator, const iterator> equal_range(const K& x) const noexcept
		{
			auto k = eqrange(Key(x));
			return make_pair(iterator(k.first), iterator(k.second));
		}
	};
	template< class Key, class Compare, class Alloc >
	bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return  lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
	}
	template< class Key, class Compare, class Alloc >
	bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return !(lhs == rhs);
	}
	template< class Key, class Compare, class Alloc >
	bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) == -1;
	}
	template< class Key, class Compare, class Alloc >
	bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class Key, class Compare, class Alloc >
	bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) == 1;
	}
	template< class Key, class Compare, class Alloc >
	bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) >= 0;
	}

	template< class Key, class Compare, class Alloc >
	void swap(set<Key, Compare, Alloc>& lhs, set<Key, Compare, Alloc>& rhs) noexcept(is_nothrow_swap_alloc<Alloc>)
	{
		lhs->swap(rhs);
	}
}
namespace uuz
{
	template<typename Key, typename Compare = less<Key>, typename Allocator = uuz::allocator<Key>>
	class multiset :public set<Key, Compare, Allocator>
	{
		using base = rb_tree<Key, Compare, Allocator>;
		using iterator = typename set::iterator;
		using node_type = typename set::node_type;
		using allocator_type = typename set::allocator_type;
	public:
		multiset() : multiset(Compare()) {}
		explicit multiset(const Compare& comp,const Allocator& alloc = Allocator()):set(comp,alloc){}
		explicit multiset(const Allocator& alloc):set(alloc){}
		template< typename InputIt, typename = is_input<Key, InputIt>>
		multiset(const InputIt& first,const InputIt& last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : set(comp, alloc)
		{
			mutilinid(first, last);
		}
		template< typename InputIt, typename = is_input<Key, InputIt>>
		multiset(const InputIt& first, const InputIt& last, const Allocator& alloc):set(first,last,Compare(),alloc){}
		multiset(const multiset& other):set(other){}
		multiset(const multiset& other, const Allocator& alloc):set(other,alloc){}
		multiset(multiset&& other):set(std::move(other)){}
		multiset(multiset&& other, const Allocator& alloc):set(std::move(other),alloc){}
		multiset(std::initializer_list<Key> init,const Compare& comp = Compare(),const Allocator& alloc = Allocator()):multiset(init.begin(),init.end(),comp,alloc){}
		multiset(std::initializer_list<Key> init,const Allocator& alloc):mutilmap(init.begin(),init.end(),alloc){}

		multiset& operator=(const multiset& other)
		{
			rb_tree::operator=(other);
			return *this;
		}
		multiset& operator=(multiset&& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			rb_tree::operator=(std::move(other));
			return *this;
		}
		multiset& operator=(std::initializer_list<Key> ilist)
		{
			multiset temp( ilist,alloc );
			this->swap(temp);
			return *this;
		}

		iterator insert(const Key& value)
		{
			return emplace(value);
		}
		iterator insert(Key&& value)
		{
			return emplace(std::move(value));
		}
		iterator insert(const iterator hint, const Key& value)
		{
			return emplace_hint(hint, value);
		}
		iterator insert(const iterator hint, Key&& value)
		{
			return emplace_hint(hint, std::move(value));
		}
		template< typename InputIt, typename = is_input<Key, InputIt>>
		void insert(const InputIt& first, const InputIt& last)
		{
			auto i = first;
			try
			{
				for (; i != last; ++i)
					emplace(*i);
			}
			catch (...)
			{
				for (auto j = first; j != i; ++j)
					dele(truefind(*i));
				throw;
			}
		}
		void insert(std::initializer_list<Key> ilist)
		{
			return insert(ilist.begin(), ilist.end());
		}
		iterator insert(node_type&& nh)
		{
			auto z = ifind(nh.value());
			if (!z)
			{
				if (get_allcoator() == nh.get_allocator())
				{
					auto k = Insert(nh.t, z, false);
					nh.t = nullptr;
					return iterator(k);
				}
				else
				{
#ifdef DEBUG
					assert(false, "It's undefined that allocator is not equeal");
#else
					auto k = insert(std::move(nh.t->get()));
					nh->~to_node();
					return k;
#endif 
				}
			}
			return end();
		}
		iterator insert(const iterator hint, node_type&& nh)
		{
			auto k = check(hint, nh.t->get());
			if (!k)
			{
				if (get_allocator() == nh.get_allocator())
				{
					auto x = iterator(Insert(nh.t, k, false));
					nh.t = nullptr;
					return x;
				}
				else
				{
#ifdef DEBUG
					assert(false, "It's undefined that allocator is not equeal");
#else
					auto x = insert(hint, std::move(nh.t->get()));
					nh->~to_node();
					return x;
#endif 
				}
			}
			return end();
		}

		template< class... Args >
		iterator emplace(Args&&... args)
		{
			auto t = make(std::forward<Args>(args)...);
			auto k = ifind(t->get());
			if (!cmp(t->get(), k->get()) && !cmp(k->get(), t->get()))
				k = nextnode(k);
			return iterator(Insert(t, k, false));
		}

		template <class... Args>
		iterator emplace_hint(const iterator hint, Args&&... args)
		{
			auto t = make(std::forward<Args>(args)...);
			auto k = check(hint, t->get());
			if (!k)
				return end();
			if (!cmp(t->get(), k->get()) && !cmp(k->get(), t->get()))
				k = nextnode(k);
			return iterator(Insert(t, k, false));
		}
		using rb_tree::erase;
		size_t erase(const Key& key)
		{
			auto k = truefind(key);
			size_t t = 0;
			while (!isnul(k))
			{
				dele(k);
				k = truefind(key);
				++t;
			}
			return t;
		}


		size_t count(const Key& key) const noexcept
		{
			auto k = eqrange(key);
			auto f = k.first;
			auto l = k.second;
			if (f == l || f == &nul)
				return 0;
			else
			{
				size_t t = 0;
				while (f != l)
				{
					++f;
					++t;
				}
				return t;
			}
		}
		template< typename K, typename = std::enable_if_t<std::is_constructible_v<Key, K&>> >
		size_t count(const K& x) const noexcept
		{
			return count(Key(x));
		}
	};
	template< class Key, class Compare, class Alloc >
	bool operator==(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return  lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
	}
	template< class Key, class Compare, class Alloc >
	bool operator!=(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return !(lhs == rhs);
	}
	template< class Key, class Compare, class Alloc >
	bool operator<(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) == -1;
	}
	template< class Key, class Compare, class Alloc >
	bool operator<=(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) <= 0;
	}
	template< class Key, class Compare, class Alloc >
	bool operator>(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) == 1;
	}
	template< class Key, class Compare, class Alloc >
	bool operator>=(const multiset<Key, Compare, Alloc>& lhs, const multiset<Key, Compare, Alloc>& rhs)noexcept(noexcept(lhs.compare(rhs)))
	{
		return lhs.compare(rhs) >= 0;
	}
	
	template< class Key, class Compare, class Alloc >
	void swap(multiset<Key, Compare, Alloc>& lhs,multiset<Key, Compare, Alloc>& rhs) noexcept(is_nothrow_swap_alloc<Alloc>::value)
	{
		lhs->swap(rhs);
	}
}