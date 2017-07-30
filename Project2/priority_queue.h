#pragma once
#include"prepare.h"
#include"vector.h"
namespace uuz
{
	template<typename T,typename Container = uuz::vector<T>,typename Compare = uuz::pre_less<T,nil>> 
	class priority_queue
	{
	public:
		priority_queue(const Compare& compare, const Container& cont) :data(cont), comp(compare) { gouzao(); }
		explicit priority_queue(const Compare& compare = Compare(),Container&& cont = Container()):data(std::move(cont)),comp(compare){ gouzao(); }
		priority_queue(const priority_queue& other):data(other.data),comp(other.comp){ }
		priority_queue(priority_queue&& other) 
		{
			data.swap(other.data);
		}
		/*	template< class Alloc >
		explicit priority_queue(const Alloc& alloc);
			template< class Alloc >
		priority_queue(const Compare& compare, const Alloc& alloc);
			template< class Alloc >
		priority_queue(const Compare& compare, const Container& cont,
			const Alloc& alloc);
			template< class Alloc >
		priority_queue(const Compare& compare, Container&& cont,
			const Alloc& alloc);
			template< class Alloc >
		priority_queue(const priority_queue& other, const Alloc& alloc);*/
		/*	template< class Alloc >
		priority_queue(priority_queue&& other, const Alloc& alloc);*/
		template< typename InputIt ,typename = is_input<T,InputIt>>
		priority_queue(const InputIt& first,const InputIt& last, const Compare& compare, const Container& cont)
			:priority_queue(compare,cont)
		{
			cont.assign(first, last);
			gouzao();
		}
		template< typename InputIt, typename = is_input<T, InputIt>>
		priority_queue(const InputIt& first, const InputIt& last, const Compare& compare = Compare(), Container&& cont = Container())
			: priority_queue(compare, std::move(cont))
		{
			cont.assign(first, last);
			gouzao();
		}

		priority_queue& operator=(const priority_queue& other)
		{
			auto k{ other };
			this->swap(k);
			return *this;
		}
		priority_queue& operator=(priority_queue&& other)
		{
			auto k{ std::move(other) };
			this->swap(k);
			return *this;
		}

		const T& top() const noexcept
		{
			return data[0];
		}

		size_t size()const noexcept
		{
			return data.size();
		}

		bool empty()const noexcept
		{
			return data.empty();
		}

		void push(const T& value)
		{
			emplace(value);
		}
		void push(T&& value)
		{
			emplace(std::move(value));
		}

		template< typename... Args >
		void emplace(Args&&... args)
		{
			data.emplace_back(std::forward<Args>(args)...);
			auto k = data.size() - 1;
			auto d = ((k + 1) / 2) - 1;
			while (k!=0&&!comp(data[k], data[d]))
			{
				swap(data[k], data[d]);
				k = d;
				d = ((k + 1) / 2) - 1;
			}
		}

		void pop()
		{
			using std::swap;
			swap(data[0], data[data.size() - 1]);
			data.pop();
			auto k = 0;
			auto p1 = ((k + 1) << 1) -1;
			auto p2 = ((k + 1) << 1);
			while (p1 < size() || p2 < size())
			{
				if (p2 < size())
				{
					if (comp(data[p1], data[p2]))
						p1 = p2;
					if (!comp(data[p1], data[k]))
					{
						swap(data[p1], data[k]);
						k = p1;
					}
				}
				else
				{
					swap(data[k], data[p1]);
					k = p1;
					break;
				}
				p1 = ((k + 1) << 1) - 1;
				p2 = ((k + 1) << 1);
			}
		}

	private:
		void gouzao()noexcept
		{
			auto k = data.size() - 1;
			for(;k!=0;--k)
			{
				auto d = ((k + 1) / 2) - 1;
				if (!comp(data[k], data[d]))
				{
					using std::swap;
					swap(data[k], data[d]);
				}
			}
		}
		Container data;
		Compare comp;
	};
}