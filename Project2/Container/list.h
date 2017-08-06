#pragma once
#include"container.h"
#include"pair.h"
#include<cassert>
#include<functional>
namespace uuz
{
	template<typename T,typename A>
	class list;

	template<typename T>
	struct list_node
	{
		list_node() = default;
		list_node(const T& p):data{T(p)}{}
		list_node(T&& p):data{T(std::move(p))}{}
		template<typename...Args>
		list_node(Args&&... args) : data{T(std::forward<Args>(args)...) }{}
		void destroy()noexcept
		{
			data.destroy();
			this->~list_node();
			return;
		}
		list_node* next = nullptr;
		list_node* last = nullptr;
		storage<T> data;
	};

	template<typename T, typename Allocator>
	class list_iterator
	{
		using self = list_iterator;
		friend list<T, Allocator>;
	public:
		self& operator++() noexcept
		{
			t= t->next;
			return *this;
		}
		self operator++(int)noexcept
		{
			auto p{ *this };
			t = t->next;
			return p;
		}
		self& operator--()noexcept
		{
			t = t->last;
			return *this;
		}
		self operator--(int) noexcept
		{
			auto p{ *this };
			t = t->last;
			return p;
		}

		T& operator*()noexcept
		{
			return t->data.get();
		}
		const T& operator*()const noexcept
		{
			return t->data.get();
		}
		T* operator->()noexcept
		{
			return t->data.get_point();
		}
		const T* operator->()const noexcept
		{
			return t->data.get_point();
		}

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.t == b.t;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return !(a == b);
		}
		friend bool operator<(const self& a, const self& b)noexcept
		{
			return a.t < b.t;
		}
		friend bool operator>(const self& a, const self& b)noexcept
		{
			return b < a;
		}
		friend bool operator<=(const self& a, const self& b)noexcept
		{
			return a < b || a == b;
		}
		friend bool operator>=(const self& a, const self& b)noexcept
		{
			return a > b || a == b;
		}

	private:
		self(list_node<T>* tt)noexcept:t(tt){}
		self(const list_node<T>* tt)noexcept :t(const_cast<list_node<T>*>(tt)){}
		list_node<T>* t = nullptr;
	};

	template<typename T,typename A = uuz::allocator<T>>
	class list
	{
	public:
		
		using self = list;
		using iterator = list_iterator<T, A>;
		using node = list_node<T>;
		using Allocator = typename uuz::exchange<A, node>::type;
		using size_t = _Uint32t;
	public:
		list(): alloc(Allocator())
		{
			nul.next = &nul;
			nul.last = &nul;
		}
		explicit list(const A& alloc):alloc(alloc)
		{
			nul.next = &nul;
			nul.last = &nul;
		}
		list(size_t t, const T& p, const A& alloc = A()):list(alloc)
		{
			if (t != 0)
			{
				int i = 0;
				node *k, *pp;
				try
				{
					k = list_make(p);
					++i;
					pp = k;
					for (; i < t; ++i)
					{
						auto temp = list_make(p);
						pp->next = temp;
						temp->last = pp;
						pp = temp;
					}
				}
				catch (...)
				{
					if (i > 0)
						list_destroy(k);
					throw;
				}
				ssize = t;
				charu(&nul, k, pp);
			}
		}
		explicit list(size_t t, const A& alloc = A()) :list(t, T{},alloc) {}
		template< typename InputIt,typename = is_input<T, InputIt>>
		list(const InputIt& _first, const InputIt& _last, const A& alloc = A()):list(alloc)
		{
			if (_first != _last)
			{
				auto i = _first;
				auto ss = 0;
				node* k, *p;
				try
				{
					k = list_make(*_first);
					++i;
					++ss;
					p = k;
					for (; i != _last; ++i)
					{
						auto temp = list_make(*i);
						p->next = temp;
						temp->last = p;
						p = temp;
						++ss;
					}
				}
				catch (...)
				{
					if(i!=_first)
						list_destroy(k);
					throw;
				}		
				ssize = ss;
				charu(&nul, k, p);
			}
		}
		list(const std::initializer_list<T>& init, const A& alloc = A()) :list(init.begin(), init.end(),alloc) {}
		list(const self& t):list(t.begin(),t.end(),t.alloc){}
		list(const list& other, const A& alloc):list(other.begin(),other.end(),alloc){}
		list(self&& t)noexcept(is_nothrow_swap_alloc<Allocator>::value) :self()
		{
			this->swap(t);
		}
		list(list&& other, const A& alloc):list()
		{
			if (alloc == this->alloc)
				this->swap(other);
			else if(!other.empty())
			{
				auto i = other.begin();
				try
				{
					auto k = list_make(std::move(*(other.begin())));
					++i;
					auto p = k;
					for (; i != other.end(); ++i)
					{
						auto temp = list_make(std::move(*i));
						p->next = temp;
						temp->last = p;
						p = temp;
					}
				}
				catch (...)
				{
					if (i != other.begin())
						list_destroy(k);
					throw;
				}
				ssize = other.ssize;
				charu(&nul, k, p);
				other.clear();
			}
		}

		list& operator=(const list& other)
		{
			if (this == &other)
				return *this;
			auto temp(other);
			this->swap(temp);
			return *this;
		}
		list& operator=(list&& other)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if (this == &other)
				return *this;
			auto temp(std::move(other));
			this->swap(temp);
			return *this;
		}	
		list& operator=(const std::initializer_list<T>& ilist)
		{
			auto temp(ilist,alloc);
			this->swap(temp);
			return *this;
		}

		void assign(size_t count, const T& value)
		{
			self temp(count, value,alloc);
			this->swap(temp);
		}
		template< typename InputIt ,typename = is_input<T, InputIt>>
		void assign(const InputIt& _first, const InputIt& _last)
		{
			auto temp(_first, _last,alloc);
			this->swap(temp);
		}
		void assign(const std::initializer_list<T>& ilist)
		{
			auto temp{ ilist ,alloc};
			this->swap(temp);
		}

		Allocator get_allocator() const noexcept(std::is_nothrow_move_constructible_v<Allocator> && std::is_nothrow_copy_constructible_v<Allocator>)
		{
			return alloc;
		}


		T& front()
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return nul.next->data.get();
		}
		const T& front()const
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return nul.next->dat.get();
		}

		T& back()
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return nul.last->data.get();
		}
		const T& back()const
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return nul.last->data.get();
		}

		iterator begin()noexcept
		{
			return iterator(nul.next);
		}
		const iterator begin()const noexcept
		{
			return iterator{ nul.next };
		}
		const iterator cbegin()const noexcept
		{
			return iterator{ nul.next };
		}

		iterator end()noexcept
		{
			return iterator{ &nul };
		}
		const iterator end()const noexcept
		{
			return iterator{ &nul };
		}
		const iterator cend()const noexcept
		{
			return iterator{ &nul };
		}

		bool empty()const noexcept
		{
			return nul.next==&nul;
		}

		size_t size()const noexcept
		{
			return ssize;
		}
		
		size_t max_size()const noexcept
		{
			return size();
		}

		void clear()noexcept
		{
			if (empty())
				return;
			nul.last->next = nullptr;
			if(nul.next)
				list_destroy(nul.next);
			nul.last = nullptr;
			nul.next = nullptr;
			ssize = 0;
		}

		iterator insert(const iterator& pos, const T& value)
		{
			return emplace(pos, value);
		}
		iterator insert(const iterator& pos, T&& value)
		{
			return emplace(pos, std::move(value));
		}
		iterator insert(const iterator& pos, size_t count, const T& value)
		{
			if (count)
			{
				int i = 0;
				try
				{
					auto k = list_make(value);
					++i;
					auto t = k;
					for (; i != count; ++i)
					{
						auto temp = list_make(value);
						t->next = temp;
						temp->last = t;
						t = last;
					}
				}
				catch (...)
				{
					if (i > 0)
						list_destroy(k);
					throw;
				}
				
				charu(*pos, k, t);
				ssize += count;
				return iterator{ t };
			}
			return pos;
		}
		template<typename InputIt ,typename = is_input<T, InputIt>>
		iterator insert(const iterator& pos, const InputIt& _first, const InputIt& _last)
		{
			if (_first != _last)
			{
				auto i = _first;
				try
				{
					auto k = list_make(*_first);
					auto ss = 1;
					++i;
					auto t = k;
					for (; i != _last; ++i)
					{
						auto temp = list_make(*p);
						t->next = temp;
						temp->last = t;
						t = last;
						++ss;
					}
				}
				catch (...)
				{
					if (i != _first)
						list_destroy(k);
					throw;
				}
				charu(*pos, k, t);
				ssize += ss;
				return iterator{ t };
			}
			return pos;
		}
		iterator insert(const iterator& pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		template< class... Args >
		iterator emplace(const iterator& pos, Args&&... args)
		{
			auto k = list_make(std::forward<Args>(args)...);
			charu(pos.t, k, k);
			++ssize;
			return iterator{ k };
		}

		iterator erase(const iterator& pos)
		{
			return erase(pos, iterator{ pos.t->next });
		}
		iterator erase(const iterator& first, const iterator& last)
		{
			if (first == last)
				return first;
			if (first == begin() && last == end())
			{
				clear();
				return end();
			}
			auto ss = 0;
			for (auto i = first; i != last; ++i)
				++ss;
			first.t->last->next = last.t;
			last.t->last->next = nullptr;
			last.t->last = first.t->last;
			list_destroy(first.t);
			ssize -= ss;
		}

		void push_back(const T& value)
		{
			emplace_back(value);
		}
		void push_back(T&& value)
		{
			emplace_back(std::move(value));
		}

		template< typename... Args >
		T& emplace_back(Args&&... args)
		{
			return *(emplace(end(), std::forward<Args>(args)...));
		}

		void pop_back()
		{
			erase(--end());
		}

		void push_front(const T& value)
		{
			emplace_front(value);
		}
		void push_front(T&& value)
		{
			emplace_front(std::move(value));
		}

		template< class... Args >
		T& emplace_front(Args&&... args)
		{
			return *(emplace(begin(), std::forward<Args>(args)...));
		}

		void pop_front()
		{
			erase(begin());
		}

		void resize(size_t count)
		{
			return resize(count, T{});
		}
		void resize(size_t count, const T& value)//反正有erase无法保证安全
		{
			if (count == 0)
				return clear();
			auto p = nul.next;
			while (p != &nul && count != 0)
			{
				p = p->next;
				--count;
			}
			if (p == &nul && count != 0)
			{
				node* k = nullptr;
				try
				{
					k = list_make(value);
					--count;
					auto l = k;
					while (count--)
					{
						auto temp = list_make(value);
						l->next = temp;
						temp->last = l;
						l = temp;
					}
				}
				catch (...)
				{
					list_destroy(k);
					throw;
				}
				charu(end().t, k, l);
			}
			else if (p != &nul&&count == 0)
				erase(iterator(p), end());
			ssize = count;
		}

		void swap(list& other)	noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			using std::swap;
			auto k = nul.next;
			auto p = nul.last;
			auto k2 = other.nul.next;
			auto p2 = other.nul.last;
			if (k != &nul)
			{
				k->last = &other.nul;
				other.nul.next = k;
				p->next = &other.nul;
				other.nul.last = p;
			}
			else
				other.nul.last = other.nul.next = &other.nul;
			if (k2 != &other.nul)
			{
				k2->last = &nul;
				nul.next = k2;
				p2->next = &nul;
				nul.last = p2;
			}
			else
				nul.last = nul.next = &nul;
			swap(alloc, other.alloc);
			swap(other.ssize, ssize);
		}

		void merge(list& other)
		{
			return merge(other, pre_less<T, nil>());
		}
		void merge(list&& other)
		{
			return merge(std::move(other), pre_less<T, nil>());
		}
		template <typename Compare>
		void merge(list& other, Compare comp)
		{
			if (other.empty() || *this == other)
				return;
			if (this->empty() && alloc == other.alloc)
				return this->swap(other);
			if (alloc != other.alloc)
			{
				throw(std::runtime_error("allocator is not equal"));
			}
			auto ob = other.nul.next;
			auto oe = other.nul.last;
			auto q = nul.next;
			while (q != &nul&&ob != &other.nul)
			{
				if (comp(ob->data.get(), q->data.get()))
				{
					auto k = ob;
					while (k != &other.nul && comp(k->data.get(), q->data.get()))
						k = k->next;
					charu(q, ob, k->last);
					q = k->last;
					ob = k;
				}
				else
					q = q->next;
			}
			if (q == &nul && ob != &other.nul)
				charu(end().t, ob, oe);
			other.nul.next = other.nul.last = &other.nul;
			ssize += other.ssize;
		}
		template <typename Compare>
		void merge(list&& other, Compare comp)
		{
			auto t{ std::move(other) };
			return merge(t, comp);
		}

		void splice(const iterator& pos, list& other)
		{
			splice(pos,other, other.begin(), other.end(),other.size());
		}
		void splice(const iterator& pos, list&& other)
		{
			auto temp{ std::move(other) };
			splice(pos, other,other.begin(), other.end(),other.size());
		}
		void splice(const iterator& pos, list& other, const iterator& it)
		{
			auto temp{ it };
			splice(pos, other, it, ++temp,1);
		}
		void splice(const iterator& pos, list&& other, const iterator& it)
		{
			auto temp{ it };
			splice(pos, other, it, ++temp,1);
		}
		void splice(const iterator& pos, list& other, const iterator& first, const iterator& last,int  t=-1)noexcept
		{
			if (other == *this || first == last)
				return;
			if (t == -1)
			{
				t = 0;
				for (auto i = first; i != last; ++i)
					++t;
			}
			auto k = first.t;	
			auto p = last.t->last;
			k->last->next = last.t;
			p->next->last = first.t->last;
			charu(pos.t, k, p);
			ssize += t;
		}
		void splice(const iterator& pos, list&& other, const iterator& first, const iterator& last)
		{
			splice(pos, other, first, last);
		}

		void remove(const T& value)
		{
			return remove_if([&](const T& i) {return i == value; });
		}
		template< typename UnaryPredicate >
		void remove_if(const UnaryPredicate& p)
		{
			auto l = nul.next;
			while (l != &nul)
			{
				if (p(l->data.get()))
				{
					l->last->next = l->next;
					l->next->last = l->last;
					auto w = l->next;
					l->next = nullptr;

					list_destroy(l);
					l = w;
					--ssize;
				}
				else
					l = l->next;
			}
		}

		void reverse()noexcept
		{
			auto k = nul.next;
			while (k != &nul)
			{
				using std::swap;
				std::swap(k->next, k->last);
				k = k->last;
			}
			std::swap(nul.last, nul.next);
		}

		void unique()
		{
			return unique([](const T& a, const T& b) {return a == b; });
		}
		template< typename BinaryPredicate >
		void unique(const BinaryPredicate& p)
		{
			auto k = nul.next;
			while (k->next != &nul)
			{
				if (p(k->data.get(), k->next->data.get()))
				{
					auto t = k->next;
					k->next = k->next->next;
					k->next->last = k;
					t->next = nullptr;
					list_destroy(t);
					--ssize;
				}
				else
					k = k->next;
			}
		}

		void sort()
		{
			return sort([](const T& a, const T&b) {return a < b; });
		}
		template<typename Compare >
		void sort(const Compare& comp)
		{
			if (empty() || nul.next == nul.last)
				return;
			auto kk = ssize;
			auto be = nul.next;
			auto en = nul.last;
			nul.last = nul.next = &nul;
			be->last = nullptr;
			en->next = nullptr;
			auto mid = midden(be);
			auto k = mid->next;
			list t1{ be,mid,alloc };
			list t2{ k,en ,alloc};
			t1.sort(comp);
			t2.sort(comp);
			t1.merge(t2, comp);
			this->swap(t1);
			this->ssize = kk;
		}

		int comp(const list& t)const noexcept
		{
			auto k = nul.next;
			auto p = t.nul.next;
			while (k != &nul&&p != &(t.nul))
			{
				if (k->data.get() < p->data.get())
					return -1;
				else if (k->data.get() > p->data.get())
					return 1;
				k = k->next;
				p = p->next;
			}
			if (k == &nul&&p == &(t.nul))
				return 0;
			else if (k != &nul&&p == &(t.nul))
				return 1;
			return -1;
		}

		~list()
		{
			clear();
		}
	private:
		static node* midden(node* a)noexcept
		{
			auto q = a;
			auto m = a;
			while (q->next&&q->next->next)
			{
				q = q->next->next;
				m = m->next;
			}
			return m;
		}
		void list_destroy(node* p)noexcept
		{
			if (!p)
				return;
			if (p->next)
				list_destroy(p->next);
			p->destroy();
			alloc.deallocate(p,1);
		}
		template<typename...Args>
		node* list_make(Args...args)
		{
			node *t;
			try
			{
				return t = new(alloc.allocate()) node(std::forward<Args>(args)...);
			}
			catch (const bad_alloc& e)
			{
				throw;
			}
			catch (...)
			{
				alloc.deallocate(t, 1);
				throw;
			}
		}

		list(node*a, node*b,const Allocator& alloc):list(alloc)
		{
			charu(nullptr, a, b);
		}
		void charu(node* d,node* b, node* e)noexcept
		{
			if (empty())
			{
				nul.next = b;
				b->last = &nul;
				nul.last = e;
				e->next = &nul;
			}
			else if (end().t == d)
			{
				nul.last->next = b;
				b->last = nul.last;
				nul.last = e;
				e->next = &nul;
			}
			else if (d == begin().t)
			{
				nul.next->last = e;
				e->next = nul.next;
				nul.next = b;
				b->last = &nul;
			}
			else
			{
				auto kk = d->last;
				kk->next =b;
				b->last = kk;
				e->next = d;
				d->last = e;
			}
		}
		
		Allocator alloc;
		size_t ssize = 0;
		node nul;
	};
	template< typename T, typename Alloc >
	 bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) == 0;
	}

	template< typename T, typename Alloc >
	 bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) != 0;
	}

	template< typename T, typename Alloc >
	 bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) < 0;
	}

	template< typename T, typename Alloc >
	 bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) <= 0;
	}

	template< typename T, typename Alloc >
	 bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) > 0;
	}

	template< typename T, typename Alloc >
	 bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) >= 0;
	}

	template< typename T, typename Alloc >
	 void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}