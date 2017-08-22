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

		bool operator()(const Key& a, const uuz::pair<const Key, T>& b)noexcept(noexcept(cmp(a, a)))
		{
			return cmp(a, b.first);
		}

		bool operator()(const uuz::pair<const Key, T>& a, const uuz::pair<const Key, T>& b)noexcept(noexcept(cmp(a.first, a.first)))
		{
			return cmp(a.first, b.first);
		}

		bool operator()(const uuz::pair<const Key, T>& a, const Key& b)noexcept(noexcept(cmp(b, b)))
		{
			return cmp(a.first, b);
		}

		bool operator()(const Key& a, const Key& b)noexcept(noexcept(cmp(a, a)))
		{
			return cmp(a, b);
		}

		template<typename K>
		bool operator()(const K& a, const uuz::pair<const Key, T>& b)noexcept(noexcept(cmp(a, a)))
		{
			return cmp(a, b.first);
		}

		template<typename K>
		bool operator()(const uuz::pair<const Key, T>& a,const K&  b)noexcept(noexcept(cmp(a, a)))
		{
			return cmp(a.first, b);
		}

		compare cmp;
	};

}
namespace uuz
{

	template<typename Key, typename T, typename Compare, typename A>
	class map;
	template<typename Key, typename T, typename Compare, typename A>
	class multimap;

	template<typename Key,typename T, typename Allocator>
	class map_node :public to_node<pair<const Key,T>, Allocator>
	{
	public:
		template<typename K, typename a, typename Compare, typename A>
		friend class map;
		template<typename K, typename a, typename Compare, typename A>
		friend class multimap;

		using key_type = Key;
		using mapped_type = T;
		using allocator_type = Allocator;
		constexpr map_node() noexcept:to_node() {}
		map_node(map_node&& nh) noexcept : to_node(std::move(nh)) {}
		map_node(rb_tree_node<T>* a, const allocator_type& b) :to_node(a, b) {}

		map_node& operator=(map_node&& nh)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			to_node::operator=(std::move(nh));
			return *this;
		}

		key_type& key() const
		{
			return t->get().first;
		}

		mapped_type& mapped() const
		{
			return t->get().second;
		}
	};


	template<typename Key,typename T,typename Compare =less<Key>,typename Allocator = uuz::allocator<pair<const Key,T>>>
	class map: public rb_tree<pair<const Key,T>,map_less<Key,T, Compare>, Allocator>
	{
	public:
		using iterator =typename rb_tree<pair<const Key, T>, map_less<Key, T, Compare>, Allocator>::iterator;
		using value_type = pair<const Key, T>;
		using allocator_type = typename rb_tree::Allocator;
		using node_type = map_node<Key, T, allocator_type>;
		using insert_return_type = struct
		{
			iterator position;
			bool inserted;
			node_type node;
		};


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

		using rb_tree::insert;
		template< typename P ,typename = std::enable_if_t<std::is_constructible_v<value_type,P&&>>>
		std::pair<iterator, bool> insert(P&& value)
		{
			return emplace(std::forward<P>(value));
		}
		template< typename P, typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
		iterator insert(const iterator hint, P&& value)
		{
			return emplace_hint(hint, std::forward<P>(value));
		}
		insert_return_type insert(node_type&& nh)
		{
			auto z = ifind(nh.value());
			if (!z && (cmp(z->get(), nh.value()) || cmp(nh.value(), z->get())))
			{
				if (get_allcoator() == nh.get_allocator())
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
					return insert_return_type{ k,true,std::move(nh) };
#endif 
				}
			}
			return insert_return_type{ iterator(z),false,std::move(nh) };
		}

		node_type extract(const iterator position)
		{
			if (position == end())
				return node_type();
			auto k = extract(position);
			return node_type(k, rb_tree::get_allocator());
		}
		node_type extract(const Key& x)
		{
			auto k = find(x);
			return extract(k);
		}


		template <typename M>
		pair<iterator, bool> insert_or_assign(const Key& k, M&& obj)
		{
			auto temp = ifind(k);
			if(!temp || cmp(k,temp->get()) || cmp(temp->get(),k))
			{
				auto l = make(value_type(k,T( std::forward<M>(obj))));
				auto t = Insert(l, temp);
				return pair<iterator, bool>(iterator(t), true);
			}
			temp->get().second = std::forward<M>(obj);
			return pair<iterator, bool>(iterator(temp), false);
		}
		template <typename M>
		pair<iterator, bool> insert_or_assign(Key&& k, M&& obj)
		{
			auto temp = ifind(k);
			if (!temp || cmp(k, temp->get()) || cmp(temp->get(), k))
			{
				auto l = make(value_type(std::move(k), T(std::forward<M>(obj))));
				auto t = Insert(l, temp);
				return pair<iterator, bool>(iterator(t), true);
			}
			temp->get().second = std::forward<M>(obj);
			return pair<iterator, bool>(iterator(temp), false);
		}
		template <typename M>
		iterator insert_or_assign(const iterator, const Key& k, M&& obj)
		{
			return insert_or_assign(k, std::forward<M>(obj)).first;
		}
		template <typename M>
		iterator insert_or_assign(const iterator hint, Key&& k, M&& obj)
		{
			return insert_or_assign(std::move(k), std::forward<M>(obj)).first;
		}

		template <typename... Args>
		pair<iterator, bool> try_emplace(const Key& k, Args&&... args)
		{
			auto temp = ifind(k);
			if (!temp || cmp(k, temp->get()) || cmp(temp->get(), k))
			{
				auto l = make(value_type(k, T(std::forward<Args>(args)...)));
				auto t = Insert(l, temp);
				return pair<iterator, bool>(iterator(t), true);
			}
			return pair<iterator, bool>(iterator(temp), false);
		}
		template <typename... Args>
		pair<iterator, bool> try_emplace(Key&& k, Args&&... args)
		{
			auto temp = ifind(k);
			if (!temp || cmp(k, temp->get()) || cmp(temp->get(), k))
			{
				auto l = make(value_type(std::move(k), T(std::forward<Args>(args)...)));
				auto t = Insert(l, temp);
				return pair<iterator, bool>(iterator(t), true);
			}
			return pair<iterator, bool>(iterator(temp), false);
		}
		template <typename... Args>
		iterator try_emplace(const iterator hint, const Key& k, Args&&... args)
		{
			return try_emplace(k, std::forward<Args>(args)...);
		}
		template <typename... Args>
		iterator try_emplace(const iterator hint, Key&& k, Args&&... args)
		{
			return try_emplace(std::move(k), std::forward<Args>(args)...);
		}

		using rb_tree::erase;
		size_t erase(const Key& key)
		{
			auto k = truefind(key);
			if(!isnul(k))
			{
				dele(k);
				return 1;
			}
			return 0;
		}

		size_t count(const Key& key) const noexcept
		{
			if (!isnul(truefind(key)))
				return 1;
			return 0;
		}
		template< typename K >
		size_t count(const K& x) const noexcept
		{
			if (!isnul(truefind(x)))
				return 1;
			return 0;
		}

		iterator find(const Key& key)noexcept
		{
			auto k = truefind(key);
			return iterator(k);
		}
		const iterator find(const Key& key) const noexcept
		{
			auto k = truefind(key);
			return iterator(k);
		}
		template< typename K > 
		iterator find(const K& x)noexcept
		{
			auto k = truefind(x);
			return iterator(k);
		}
		template< class K > 
		const iterator find(const K& x) const
		{
			auto k = truefind(x);
			return iterator(k);
		}

		pair<iterator, iterator> equal_range(const Key& key)noexcept
		{
			auto k = eqrange(key);
			return make_pair(iterator(k.first), iterator(k.second));
		}
		pair<const iterator, const iterator> equal_range(const Key& key) const noexcept
		{
			auto k = eqrange(key);
			return make_pair(iterator(k.first), iterator(k.second));
		}
		template< typename K >
		pair<iterator, iterator> equal_range(const K& x)noexcept
		{
			auto k = eqrange(K(x));
			return make_pair(iterator(k.first), iterator(k.second));
		}
		template< typename K >
		pair<const iterator, const iterator> equal_range(const K& x) const noexcept
		{
			auto k = eqrange(K(x));
			return make_pair(iterator(k.first), iterator(k.second));
		}

		iterator lower_bound(const Key& key)noexcept
		{
			return iterator(low_bound(key));
		}
		const iterator lower_bound(const Key& key) const noexcept
		{
			return iterator(low_bound(key));
		}
		template< typename K >
		iterator lower_bound(const K& x)noexcept
		{
			return iterator(low_bound(K(x)));
		}
		template< typename K >
		const iterator lower_bound(const K& x) const noexcept
		{
			return iterator(low_bound(K(x)));
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
	};

	template< typename Key, typename T ,typename Compare, typename Alloc >
	bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T,Compare, Alloc>& rhs)noexcept
	{
		return  lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
	}
	template< typename Key, typename T ,typename Compare, typename Alloc >
	bool operator!=(const map<Key, T,Compare, Alloc>& lhs, const map<Key, T,Compare, Alloc>& rhs)noexcept
	{
		return !(lhs == rhs);
	}
	template< typename Key, typename T ,typename Compare, typename Alloc >
	bool operator<(const map<Key, T,Compare, Alloc>& lhs, const map<Key, T,Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) == -1;
	}
	template< typename Key, typename T ,typename Compare, typename Alloc >
	bool operator<=(const map<Key, T,Compare, Alloc>& lhs, const map<Key, T,Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) <= 0;
	}
	template< typename Key, typename T ,typename Compare, typename Alloc >
	bool operator>(const map<Key, T,Compare, Alloc>& lhs, const map<Key, T,Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) == 1;
	}
	template< typename Key, typename T ,typename Compare, typename Alloc >
	bool operator>=(const map<Key, T,Compare, Alloc>& lhs, const map<Key, T,Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	template< typename Key, typename T ,typename Compare, typename Alloc >
	void swap(map<Key, T,Compare, Alloc>& lhs, map<Key, T,Compare, Alloc>& rhs) noexcept(is_nothrow_swap_alloc<Alloc>::value)
	{
		lhs->swap(rhs);
	}
}
namespace uuz
{
	template<typename Key, typename T, typename Compare = less<Key>, typename Allocator = uuz::allocator<pair<const Key, T>>>
	class multimap :public map< Key, T,Compare, Allocator>
	{
	public:
		using value_type = typename map::value_type;
		using allocator_type = typename map::allocator_type;
		using iterator = typename map::iterator;
		using node_type = typename map::node_type;
	public:
		multimap() : multimap(Compare()) {}
		explicit multimap(const Compare& comp,const Allocator& alloc = Allocator()):map(comp,alloc){}
		template< typename InputIterator,typename = is_input<value_type,InputIterator>>
		multimap(InputIterator first, InputIterator last,const Compare& comp = Compare(),const Allocator& alloc = Allocator())
			:rb_tree(comp,alloc)
		{
			mutilinid(first, last);
		}
		template< typename InputIterator, typename = is_input<value_type, InputIterator>>
		multimap(InputIterator first, InputIterator last,const Allocator& alloc)
			:rb_tree(alloc)
		{
			mutilinid(first, last);
		}
		multimap(const multimap& other):rb_tree(other){}
		multimap(const multimap& other, const Allocator& alloc):rb_tree(other,alloc){}
		multimap(multimap&& other):rb_tree(std::move(other)){}
		multimap(multimap&& other, const Allocator& alloc):rb_tree(std::move(other),alloc){}
		multimap(std::initializer_list<value_type> init,const Compare& comp = Compare(),const Allocator& alloc = Allocator())
			:multimap(init.begin(),init.end(),comp,alloc){}
		multimap(std::initializer_list<value_type> init,const Allocator& alloc)
			:multimap(init.begin(),init.end(),alloc){}
	
		multimap& operator=(const multimap& other)
		{
			rb_tree::operator=(other);
			return *this;
		}
		multimap& operator=(multimap&& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			rb_tree::operator=(other);
			return *this;
		}
		multimap& operator=(std::initializer_list<value_type> ilist)
		{
			multimap temp(ilist, alloc);
			this->swap(temp);
			return *this;
		}
		
		iterator insert(const value_type& value)
		{
			return emplace(value);
		}
		template< typename P ,typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
		iterator insert(P&& value)
		{
			return emplace(std::forward<P>(value));
		}
		iterator insert(value_type&& value)
		{
			return emplace(std::move(value));
		}
		iterator insert(const iterator hint, const value_type& value)
		{
			return emplace_hint(hint, value);
		}
		template< typename P, typename = std::enable_if_t<std::is_constructible_v<value_type, P&&>>>
		iterator insert(const iterator hint, P&& value)
		{
			return emplace_hint(hint, std::forward<P>(value));
		}
		iterator insert(const iterator hint, value_type&& value)
		{
			return emplace_hint(hint, std::move(value));
		}
		template< class InputIt, typename = is_input<value_type, InputIt>>
		void insert(InputIt first, InputIt last)
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
		void insert(std::initializer_list<value_type> ilist)
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

	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator==(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)noexcept
	{
		return  lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
	}
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator!=(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)noexcept
	{
		return !(lhs == rhs);
	}
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator<(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) == -1;
	}
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator<=(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) <= 0;
	}
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator>(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) == 1;
	}
	template< typename Key, typename T, typename Compare, typename Alloc >
	bool operator>=(const multimap<Key, T, Compare, Alloc>& lhs, const multimap<Key, T, Compare, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	template< typename Key, typename T, typename Compare, typename Alloc >
	void swap(multimap<Key, T, Compare, Alloc>& lhs, multimap<Key, T, Compare, Alloc>& rhs) noexcept(is_nothrow_swap_alloc<Alloc>::value)
	{
		lhs->swap(rhs);
	}
}