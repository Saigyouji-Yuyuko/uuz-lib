#pragma once
#include"pair.h"
#include"rbtree.h"
namespace uuz
{
	template<typename T1,typename T2,typename compare = nil>
	class map
	{
	private:

		using self = map<T1, T2, compare>;
		using self_p = self*;
		using node = uuz::pair<T1, T2>;
		using tree = rb_tree<node, compare>;
		using tree_p = tree*;

	public:

		using value_type = node;
		using iterator = typename tree::iterator;

		map() = default;
		~map()noexcept
		{
			clear();
		}

		void insert(const node& temp)
		{
			iterator p = that->find(temp);
			if (p.isnull())
				that->insert(temp);
			else
				p->second() = temp.second();
		}
		void insert(node&& temp)
		{
			iterator p = that->find(temp);
			if (p.isnull())
				that->insert(std::move(temp));
			else
				p->second() = temp.second();
		}
		template<typename U>
		iterator find(const U& temp)const noexcept(noexcept(that->find(temp)))
		{
			return that->find(temp);
		}
		template<typename U>
		void erase(const U& temp)noexcept(noexcept(that->erase(temp)))
		{
			that->erase(temp);
		}

		const T2& operator[](const T1& pp)const noexcept
		{
			auto temp = find(pp);
			if (temp.isnull())
				insert(node(pp, T2{}));
			return (*find(pp)).second();
		}
		T2& operator[](const T1& pp)noexcept
		{
			auto temp = find(pp);
			if (temp.isnull())
				insert(node(pp, T2{}));
			return (*find(pp)).second();
		}
		T2& at(const T1& pp)&
		{
			iterator temp = find(pp);
			if (temp.isnull())
				throw("error");
			return (*temp).second();
		}
		const T2& at(const T1& pp)const & noexcept
		{
			iterator temp = find(pp);
			if (temp.isnull())
				throw("error");
			return (*temp).second();
		}

		iterator begin()noexcept{ return that->begin(); }
		iterator end()noexcept { return that->end(); }
		const iterator cbegin()const noexcept { return that->cbegin(); }
		const iterator cend()const noexcept { return that->cend(); }
		bool empty()const noexcept { return that->empty(); }
		void clear()const noexcept { that->clear(); }
		

		int size()const noexcept { return that->s(); }

	private:
		 tree* that = new tree{};	
	};

}
