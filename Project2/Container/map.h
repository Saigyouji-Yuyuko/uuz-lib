#pragma once
#include"pair.h"
#include"rbtree.h"
#include"container.h"
namespace uuz
{
	namespace
	{
		template<typename compare>
		struct compare_to_pair
		{
			compare_to_pair() = default;
			compare_to_pair(const compare& t):cmp{t}{}
			compare_to_pair(compare&& t) :cmp{ std::move(t) } {}
			template<typename T>
			bool operator()(const T& a, const T& b)noexcept(noexcept(compare(a.first,b.first)))
			{
				return cmp(a.first, b.first);
			}
			compare cmp;
		};
	}
	template<typename T1,typename T2,
		typename Compare =pre_less<pair<std::add_const_t<T1>,T2>,nil>,typename Allocator = uuz::allocator<pair<std::add_const_t<T1>,T2>>>
		class map
	{
		using T = pair<std::add_const_t<T1>, T2>;
		using iterator = rb_tree<T, compare_to_pair<Compare>, Allocator>;
	public:
		map() : map(Compare()) {}
		explicit map(const Compare& comp, const Allocator& alloc = Allocator()) :tree(comp, alloc) {}
		explicit map(const Allocator& alloc) :tree(alloc) {}
		template<typename InputIterator, typename = is_input<InputIterator, T>>
		map(const InputIterator& first, const InputIterator& last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : map(comp, alloc)
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
		template<typename InputIterator, typename = is_input<InputIterator, T>>
		map(InputIterator first, InputIterator last, const Allocator& alloc) :map(alloc)
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
		map(const map& other) :tree(other.tree) {}
		map(const map& other, const Allocator& alloc) :tree(other.tree, alloc) {}
		map(map&& other) :tree(std::move(other.tree)) {}
		map(map&& other, const Allocator& alloc) :tree(std::move(other.tree), alloc) {}
		map(std::initializer_list<T> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) :map(init.begin(), init.end(), comp, alloc) {}
		map(std::initializer_list<T> init, const Allocator& a) :map(init.begin(), init.end(), a) {}

		map& operator=(const map& other)
		{
			if (this == &other)
			{
				auto temp(other);
				this->swap(temp);
			}
			return *this;
		}
		map& operator=(map&& other) noexcept(is_nothrow_swap_alloc<Allocator>)
		{
			if (this == &other)
			{
				auto temp(std::move(other));
				this->swap(temp);
			}
			return *this;
		}
		map& operator=(std::initializer_list<T> ilist)
		{
			if (this == &other)
			{
				auto temp(ilist.begin(), ilist.end());
				this->swap(temp);
			}
			return *this;
		}

		Allocator get_allocator() const
		{
			return tree.alloc;
		}

		T& at(const T1& key)
		{
			auto k = tree.find(key);
			if (k&& key == k->get().first)
				return k->get().second;
			throw(out_of_range(""));
		}
		const T& at(const T1& key) const
		{
			auto k = tree.find(key);
			if (k&& key == k->get().first)
				return k->get().second;
			throw(out_of_range(""));
		}

		T& operator[](const Key& key)
		{
			auto k = tree.find(key);
			if (k&& key == k->get().first)
				return k->get().second;
			else
			{
				auto l = tree.insert(T(key, T2{}), k);
				return l->get().second;
			}
		}
		T& operator[](Key&& key)
		{
			auto k = tree.find(key);
			if (k&& key == k->get().first)
				return k->get().second;
			else
			{
				auto l = tree.insert(T(std::move(key), T2{}), k);
				return l->get().second;
			}
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

		//size_t max_size() const noexcept{}

		void clear() noexcept
		{
			return tree.clear();
		}

		pair<iterator, bool> insert(const T& value)
		{
			auto k = tree.find(value.first);
			if (k&& k->get().first == value.first)
			{
				k->get().second = value.second;
				return make_pair<iterator, bool>(iterator(k), false);
			}
			else
			{
				auto l = tree.insert(value, k);
				return make_pair<iterator, bool>(iterator(l), true);
			}
		}
		template< typename P,typename = std::enable_if_t<std::is_constructible_v<T, P&&>>>
		pair<iterator, bool> insert(P&& value)
		{
			auto l = tree.emplace(std::forward<P>(value));
			return make_pair<iterator, bool>(iterator(l), true);
		}
		pair<iterator, bool> insert(T&& value)
		{
			auto k = tree.find(value.first);
			if (k&& k->get().first == value.first)
			{
				k->get().second = std::move(value.second);
				return make_pair<iterator, bool>(iterator(k), false);
			}
			else
			{
				auto l = tree.insert(std::move(value), k);
				return make_pair<iterator, bool>(iterator(l), true);
			}
		}
		template< typename InputIt ,typename = is_input<T,InputIt>>
		void insert(const InputIt& first, const InputIt& last)
		{
			auto i = first;
			try
			{
				for (; i != last; ++i)
					insert(*i);
			}
			catch (...)
			{
				for (auto j = first; j != i; ++i)
					tree.dele(tree.find(j));
				throw;
			}
		}
		void insert(std::initializer_list<T> ilist)
		{
			return insert(ilist.begin(), ilist.end()));
		}
		/*template< class P >
		iterator insert(const_iterator hint, P&& value);
		iterator insert(const_iterator hint, value_type&& value);
		insert_return_type insert(node_type&& nh);
		iterator insert(const_iterator hint, node_type&& nh);*/

		template <typename M,typename = std::enable_if_t<std::is_constructible_v<T2,M&&>>>
		pair<iterator, bool> insert_or_assign(const T1& k, M&& obj)
		{
			return insert(make_pair<T1, T2>(k, T2{ std::forward<args>(obj) }));
		}
		template <typename M, typename = std::enable_if_t<std::is_constructible_v<T2, M&&>>>
		pair<iterator, bool> insert_or_assign(T1&& k, M&& obj)
		{
			return insert(make_pair<T1, T2>(std::move(k), T2{ std::forward<args>(obj) }));
		}
		/*template <class M>
		iterator insert_or_assign(const_iterator hint, const T1& k, M&& obj);
		template <class M>
		iterator insert_or_assign(const_iterator hint, T1&& k, M&& obj);*/

		template< class... Args >
		std::pair<iterator, bool> emplace(Args&&... args)
		{
			auto k = tree.make(std::forward<Args>(args)...);
			auto ll = tree.find(k.get());
			
		}


		//~map(){}
	private:
		rb_tree<T, compare_to_pair<Compare>,Allocator> tree;
	};

}
