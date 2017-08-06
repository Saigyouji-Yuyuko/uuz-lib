#pragma once
#include"container.h"
#include"rbtree.h"
#include"pair.h"
namespace uuz
{
	template<typename Key,typename Compare = std::less<Key>,typename Allocator = std::allocator<Key>> 
	class set
	{
		using iterator = rb_tree_iterator<Key, Compare, Allocator>;
	private:
		rb_tree<Key> tree;
	public:
		set() : set(Compare()) {}
		explicit set(const Compare& comp,const Allocator& alloc = Allocator()):tree{comp,alloc}{}
		explicit set(const Allocator& alloc):tree{alloc}{}
		template< typename InputIt , typename = is_input<Key,InputIt>>
		set(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : tree{ comp,alloc }
		{
			try
			{
				auto i = first;
				for (; i != last; ++i)
					tree.insert(*i);
			}
			catch (...)
			{
				tree.clear();
				throw;
			}
		}
		template< class InputIt >
		set(InputIt first, InputIt last, const Allocator& alloc): set(first, last, Compare(), alloc) {}
		set(const set& other):tree{other.tree}{}
		set(const set& other, const Allocator& alloc):tree{other.tree,alloc}{}
		set(set&& other):tree{std::move(other.tree)}{}
		set(set&& other, const Allocator& alloc):tree(other.tree,alloc){}
		set(std::initializer_list<Key> init,const Compare& comp = Compare(),const Allocator& alloc = Allocator()):tree(init,comp,alloc){}
		set(std::initializer_list<Key> init, const Allocator& alloc): set(init, Compare(), alloc) {}

		set& operator=(const set& other)
		{
			if (this != &other)
			{
				auto k(other.tree);
				tree.swap(k);
			}
			return *this;
		}
		set& operator=(set&& other) noexcept(/* see below */)
		{
			if (this != &other)
			{
				auto k(std::move(other.tree));
				tree.swap(k);
			}
			return *this;
			}
		set& operator=(std::initializer_list<Key> ilist)
		{
			if (this != &other)
			{
				auto k(ilist);
				tree.swap(k);
			}
			return *this;
		}

		decltype(auto) get_allocator() const
		{
			return tree.alloc;
		}

		iterator begin() noexcept
		{
			return tree.begin();
		}
		const iterator begin() const noexcept
		{
			return tree.begin();
			}
		const iterator cbegin() const noexcept
		{
			return tree.begin();
			}

		iterator end() noexcept
		{
			return tree.end();
		}
		const iterator end() const noexcept
		{
			return tree.end();
			}
		const iterator cend() const noexcept
		{
			return tree.end();
			}

		bool empty() const noexcept
		{
			return tree.empty();
		}

		size_t size() const noexcept
		{
			return tree.size();
		}

		//size_type max_size() const noexcept;

		void clear() noexcept
		{
			return tree.clear();
		}

		std::pair<iterator, bool> insert(const Key& value)
		{

		}
		std::pair<iterator, bool> insert(value_type&& value);
		iterator insert(const_iterator hint, const value_type& value);
		iterator insert(const_iterator hint, value_type&& value);
		template< class InputIt >
		void insert(InputIt first, InputIt last);
		void insert(std::initializer_list<value_type> ilist);
		insert_return_type insert(node_type&& nh);	
		iterator insert(const_iterator hint, node_type&& nh);

		template<typename... Args>
		std::pair<iterator, bool> emplace(Args&&... args)
		{
			auto k = tree.make(std::forward<Args>(args)...);
			auto l = tree.find(k->get());
			auto s = tree.Insert(k, l);
			if (l == s)
				return make_pair<iterator, bool>(iterator(l), false);
			return make_pair<iterator, bool>(iterator(k), true);
		}

		template <typename... Args>
		iterator emplace_hint(const iterator& hint, Args&&... args)
		{
			auto k = tree.make(std::forward<Args>(args)...);
			auto tk = tree.check(hint, k);
			if (tk)
				return iterator(tree.Insert(k, tk));
			tree.destroy(k);
			return tree.end();
		}

		
		iterator erase(const iterator& pos)
		{
			auto k{ pos };
			++k;
			tree.dele(pos.t);
			return k;
		}
		iterator erase(iterator pos)
		{
			auto k{ pos };
			++k;
			tree.dele(pos.t);
			return k;
		}
		iterator erase(const iterator& first, const iterator& last)
		{
			auto k = first;
			while (k != last)
				k = erase(k);
			return k;
		}
		size_t erase(const Key& key)
		{

		}

		void swap(set& other) noexcept(/* see below */);

		node_type extract(const_iterator position);
		node_type extract(const key_type& x);

		template<class C2>
		void merge(std::set<Key, C2, Allocator>& source);
			template<class C2>
		void merge(std::set<Key, C2, Allocator>&& source);
			template<class C2>
		void merge(std::multiset<Key, C2, Allocator>& source);
			template<class C2>
		void merge(std::multiset<Key, C2, Allocator>&& source);

		size_type count(const Key& key) const;
		template< class K >
		size_type count(const K& x) const;

		iterator find(const Key& key);
		const_iterator find(const Key& key) const;	
		template< class K >
		iterator find(const K& x);
		template< class K > 
		const_iterator find(const K& x) const;

		std::pair<iterator, iterator> equal_range(const Key& key);
		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;
		template< class K >
		std::pair<iterator, iterator> equal_range(const K& x);
		template< class K >
		std::pair<const_iterator, const_iterator> equal_range(const K& x) const;

		key_compare key_comp() const;

		set::value_compare value_comp() const;


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