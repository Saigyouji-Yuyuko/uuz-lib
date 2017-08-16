#pragma once
#include"container.h"
#include"pair.h"
#include<cassert>
#include<functional>
namespace uuz
{
	template<typename T,typename A>
	class list;

	struct list_node_base
	{
		list_node_base* next;
		list_node_base* last;
	};

	template<typename T>
	struct list_node:public list_node_base
	{
		list_node() = default;
		list_node(const T& p):data{p}{}
		list_node(T&& p):data{std::move(p)}{}
		template<typename...Args>
		list_node(Args&&... args) : data{std::forward<Args>(args)...}{}
		void destroy()noexcept
		{
			this->~list_node();
		}
		T data;
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
			return (list_node<T>*)t->data;
		}
		const T& operator*()const noexcept
		{
			return (list_node<T>*)t->data;
		}
		T* operator->()noexcept
		{
			return &((list_node<T>*)t->data);
		}
		const T* operator->()const noexcept
		{
			return &((list_node<T>*)t->data);
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
		self(list_node_base* tt)noexcept:t(tt){}
		self(const list_node_base* tt)noexcept :t(const_cast<list_node_base*>(tt)){}
		list_node_base* t = nullptr;
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

	private:

		Allocator alloc;
		size_t ssize = 0;
		list_node_base nul;

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
			if (t)
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
				auto ss = 0;
				for (auto i = _first; i != last; ++i)
					++ss;
				auto k = makelist(_first, _last);
				ssize = ss;
				charu(&nul, k.first, k.second);
			}
		}
		list(const std::initializer_list<T>& init, const A& alloc = A()) :list(init.begin(), init.end(),alloc) {}
		list(const self& t):list(t.begin(),t.end(),A()){}
		list(const list& other, const A& alloc):list(other.begin(),other.end(),alloc){}
		list(self&& t)noexcept(is_nothrow_swap_alloc<Allocator>::value) :self()
		{
			this->swap(t);
		}
		list(list&& other, const A& alloc):list(alloc)
		{
			this->swap(other);	
		}

		list& operator=(const list& other)
		{
			if (this == &other)
				return *this;
			auto temp(other,alloc);
			this->swap(temp);
			return *this;
		}
		list& operator=(list&& other)noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if (this == &other)
				return *this;
			auto temp(std::move(other),alloc);
			this->swap(temp);
			return *this;
		}	
		list& operator=(const std::initializer_list<T>& ilist)
		{
			auto temp(ilist,alloc);
			this->swap(temp,alloc);
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
			return ((node*)(nul.next))->data;
		}
		const T& front()const
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return ((node*)(nul.next))->data;
		}

		T& back()
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return ((node*)(nul.last))->data;
		}
		const T& back()const
		{
			if (empty())
				throw(out_of_range("it's empty"));
			return ((node*)(nul.last))->data;
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
			nul.last->next = nullptr;
			if(nul.next != &nul)
				list_destroy(nul.next);
			nul.last = &nul;
			nul.next = &nul;
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
				auto ss = 0;
				for (auto i = _first; i != _last; ++i)
					++ss;
				auto k = makelist(_first, _last);
				charu(*pos, k.first, k.second);
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

		iterator erase(const iterator& pos)noexcept
		{
			return erase(pos, iterator{ pos.t->next });
		}
		iterator erase(const iterator& first, const iterator& last)noexcept
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
			auto k = make(std::forward<Args>(args)...);
			nul.last->next = k;
			k->last = nul.last;
			k->next = &nul;
			nul.last = k;
			++ssize;
			return k->data;
		}

		  void pop_back()
		{
			auto k = nul.last;
			nul.last = k->last;
			nul.last->next = &nul;
			k->next = nullptr;
			list_destroy(k);
			--ssize;
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
			auto k = make(std::forward<Args>(args)...);
			nul.next->last = k;
			k->next = nul.next;
			k->last = &nul;
			nul.next = k;
			++ssize;
			return k->data;
		}

		  void pop_front()
		{
			auto k = nul.next;
			nul.next = k->next;
			nul.next->last = &nul;
			k->next = nullptr;
			list_destroy(k);
			--ssize;
		}

		  void resize(size_t count)
		{
			 resize(count, T{});
		}
		  void resize(size_t count, const T& value)
		{
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
				charu(nul.last, k, l);
			}
			else if (p != &nul&&count == 0)
				erase(iterator(p), end());
			ssize = count;
		}

		  void swap(list& other)	noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			if (alloc == other.alloc)
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
				swap(other.ssize, ssize);
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's undefined that allocate is not equal");
#else
				self temp1(other, alloc);
				self temp2(*this, other.alloc);
				this->swap(temp1);
				other.swap(temp2);
#endif 
			}
		}

		  void merge(list& other)
		{
			merge(other, pre_less<T, nil>());
		}
		  void merge(list&& other)
		{
			 merge(std::move(other), pre_less<T, nil>());
		}
		template <typename Compare>
		void merge(list& other, Compare comp)
		{
			if (other.empty() || *this == other)
				return;
			if (this->empty())
				return this->swap(other);
			auto ob = other.nul.next;
			auto oe = other.nul.last;
			auto q = nul.next;
			if (alloc != other.alloc)
			{
#ifdef DEBUG
				assert(false, "It's undefined that allocate is not equal");
#else	
				while (q != &nul&&ob != &other.nul)
				{
					if (comp(((node*)(ob))->data, ((node*)(q))->data))
					{
						auto k = ob;
						while (k != &other.nul && comp(((node*)(k))->data, ((node*)(q))->data))
							k = k->next;
						auto t = makelist(ob, k);
						charu(q, t.first, t.second);
						q = k->last;
						ob = k;
					}
					else
						q = q->next;
				}
				if (q == &nul && ob != &other.nul)
				{
					auto t = makelist(ob, oe);
					charu(nul.last, t.first, t.second);
				}
#endif 
			}
			else 
			{
				while (q != &nul&&ob != &other.nul)
				{
					if (comp(((node*)(ob))->data, ((node*)(q))->data))
					{
						auto k = ob;
						while (k != &other.nul && comp((node*)(k)->data, (node*)(q)->data))
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
			}
			ssize += other.ssize;
			other.clear();
		}
		template <typename Compare>
		  void merge(list&& other, Compare comp)
		{
			auto t{ std::move(other) };
			 merge(t, comp);
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
			if (other.alloc == alloc)
			{
				auto k = first.t;
				auto p = last.t->last;
				k->last->next = last.t;
				p->next->last = first.t->last;
				charu(pos.t, k, p);
				ssize += t;
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's undefined that allocate is not equal");
#else
				auto k = makelist(first, last);
				charu(pos.t, k.first, k.second);
				other.erase(first, last);
				ssize += t;
#endif
			}
		}
		  void splice(const iterator& pos, list&& other, const iterator& first, const iterator& last)
		{
			splice(pos, other, first, last);
		}

		  void remove(const T& value)
		{
			 remove_if([&](const T& i) {return i == value; });
		}
		template< typename UnaryPredicate >
		  void remove_if(const UnaryPredicate& p)
		{
			auto l = nul.next;
			while (l != &nul)
			{
				if (p(((node*)l)->data))
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
			using std::swap;
			while (k != &nul)
			{
				swap(k->next, k->last);
				k = k->last;
			}
			swap(nul.last, nul.next);
		}

		  void unique()
		{
			unique([](const T& a, const T& b) {return a == b; });
		}
		template< typename BinaryPredicate >
		  void unique(const BinaryPredicate& p)
		{
			auto k = nul.next;
			while (k->next != &nul)
			{
				if (p(((node*)k)->data, ((node*)(k->next))->data))
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
			sort([](const T& a, const T&b) {return a < b; });
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

		~list()
		{
			clear();
		}
	private:
		static node* midden(list_node_base* a)noexcept
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
		void list_destroy(list_node_base* p)noexcept
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
			node *t = nullptr;
			try
			{
				t = alloc.allocate();
				new(t) node(std::forward<Args>(args)...);
				return t;
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

		list(node*a, node*b, const Allocator& alloc) :list(alloc)
		{
			charu(nullptr, a, b);
		}

		pair<node*, node*> makelist(const node* a, const node* b)
		{
			node* k = nullptr;
			try
			{
				node* p = make(a->data);
				for (auto i = a->next; i != b->next; i = i->next)
				{
					p->next = make(((node*)(i))->data);
					p->next->last = p;
					p = p->next;
				}
			}
			catch (...)
			{
				list_destroy(k);
				throw;
			}
			return make_pair(k, p);
		}
		template<typename InputIt, typename = is_input<T, InputIt>>
		pair<node*, node*> makelist(InputIt a, InputIt b)
		{
			node* k = nullptr;
			try
			{
				node* p = make(*a);
				auto i = a;
				++i;
				for (; i != b; ++i)
				{
					p->next = make(*i);
					p->next->last = p;
					p = p->next;
				}
			}
			catch (...)
			{
				list_destroy(k);
				throw;
			}
			return make_pair(k, p);
		}
		  void charu(node* d, node* b, node* e)noexcept
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
				kk->next = b;
				b->last = kk;
				e->next = d;
				d->last = e;
			}
		}
	};

	namespace
	{
		template<typename T, typename U>
		int comp(const list<T, U>& a, const list<T, U>& b) noexcept
		{
			auto i = a.begin();
			auto j = b.begin();
			for (; i != a.end() && j != b.end(); ++i, (void) ++j)
			{
				if (*i < *j)
					return -1;
				else if (*j < *i)
					return 1;		
			}
			if (a.size() > b.size())
				return 1;
			else if (a.size() < b.size())
				return -1;
			return 0;
		}
	}

	template< typename T, typename Alloc >
	 bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)noexcept
	{
		 if (lhs.size() != rhs.size())
			return false;
		return comp(lhs,rhs) == 0;
	}

	template< typename T, typename Alloc >
	 bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)noexcept
	{
		 return !(lhs == rhs);
	}

	template< typename T, typename Alloc >
	 bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)noexcept
	{
		return comp(lhs, rhs) < 0;
	}

	template< typename T, typename Alloc >
	 bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)noexcept
	{
		return comp(lhs, rhs) <= 0;
	}

	template< typename T, typename Alloc >
	 bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)noexcept
	{
		return comp(lhs, rhs) > 0;
	}

	template< typename T, typename Alloc >
	 bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)noexcept
	{
		return comp(lhs, rhs) >= 0;
	}

	template< typename T, typename Alloc >
	 void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}