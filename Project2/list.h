#pragma once
#include"prepare.h"
#include"pair.h"
#include<cassert>
namespace uuz
{
	template<typename T,typename A>
	class list;

	template<typename T>
	struct list_node
	{
		list_node() = default;
		list_node(const T& p):dat{new T(p)}{}
		list_node(T&& p):dat{new T(std::move(p))}{}
		template<typename...Args>
		list_node(Args&&... args) : dat{ new T(std::forward<Args>(args)...) }{}
		void destory()noexcept
		{
			if (next != nullptr)
				next->destory();
			delete this;
			return;
		}
		list_node* next = nullptr;
		list_node* last = nullptr;
		T* dat = nullptr;
		
	};

	template<typename T, typename Allocator = uuz::allocator>
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
			return *(t->dat);
		}
		const T& operator*()const noexcept
		{
			return *(t->dat);
		}
		T* operator->()noexcept
		{
			return (t->dat);
		}
		const T* operator->()const noexcept
		{
			return (t->dat);
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
		self(list_node<T>* tt):t(tt){}
		self(const list_node<T>* tt):t(const_cast<list_node<T>*>(tt)){}
		list_node<T>* t = nullptr;
	};

	template<typename T,typename Allocator = uuz::allocator>
	class list
	{
	public:
		using iterator = list_iterator<T, Allocator>;
		using self = list;
		using node = list_node<T>;
		using size_t = _Uint32t;
	public:
		list()
		{
 			nul.next = &nul;
			nul.last = &nul;
		}
		list(size_t t, const T& p):list()
		{
			if (t != 0)
			{
				auto k = new node(p);
				auto pp = k;
				for (auto i = 1; i < t; ++i)
				{
					auto temp = new node(p);
					pp->next = temp;
					temp->last = pp;
					pp = temp;
				}
				charu(&nul, k, pp);
			}
		}
		explicit list(size_t t) :list(t, T{}) {}
		template< typename InputIt,typename = decltype(*(std::declval<InputIt>()))>
		list(const InputIt& _first, const InputIt& _last):list()
		{
			if (_first != _last)
			{
				auto k = new node(*_first);
				auto p = k;
				auto ios = _first;
				++ios;
				for (auto i = ios; i != _last; ++i)
				{
					auto temp = new node(*i);
					p->next = temp;
					temp->last = p;
					p = temp;
				}
				charu(&nul, k, p);
			}
		}
		list(const std::initializer_list<T>& init) :list(init.begin(), init.end()) {}
		list(const self& t):list(t.begin(),t.end()){}
		list(self&& t):self()
		{
			this->swap(t);
		}

		list& operator=(const list& other)
		{
			if (this == &other)
				return *this;
			auto temp(other);
			this->swap(temp);
			return *this;
		}
		list& operator=(list&& other)noexcept
		{
			if (this == &other)
				return *this;
			auto temp(std::move(other));
			this->swap(temp);
			return *this;
		}
		list& operator=(const std::initializer_list<T>& ilist)
		{
			auto temp(ilist);
			this->swap(temp);
			return *this;
		}

		void assign(size_t count, const T& value)
		{
			self temp(count, value);
			this->swap(temp);
		}
		template< typename InputIt ,typename = decltype(*(std::declval<InputIt>()))>
		void assign(const InputIt& _first, const InputIt& _last)
		{
			auto temp(_first, _last);
			this->swap(temp);
		}
		void assign(const std::initializer_list<T>& ilist)
		{
			auto temp{ ilist };
			this->swap(temp);
		}

		T& front()
		{
			return *(nul.next->dat);
		}
		const T& front()const
		{
			return *(nul.next->dat);
		}

		T& back()
		{
			return *(nul.last->dat);
		}
		const T& back()const
		{
			return *(nul.last->dat);
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
			return nul.next == &nul;
		}

		size_t size()const
		{
			size_t t = 0;
			auto k = nul.next;
			while (k != &nul)
			{
				k = k->next;
				++t;
			}
			return t;
		}
		
		size_t max_size()const
		{
			return size();
		}

		void clear()
		{
			if (empty())
				return;
			nul.last->next = nullptr;
			nul.next->destory();
			nul.last = nullptr;
			nul.next = nullptr;
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
				auto k = new node(value);
				auto t = k;
				for (auto i = 1; i != count; ++i)
				{
					auto temp = new node(value);
					t->next = temp;
					temp->last = t;
					t = last;
				}
				charu(*pos, k, t);
				return iterator{ t };
			}
			return pos;
		}
		template<typename InputIt ,typename = decltype(*(std::declval<InputIt>()))>
		iterator insert(const iterator& pos, const InputIt& _first, const InputIt& _last)
		{

			auto k = new node(*_first);
			auto p = _first;
			++p;
			auto t = k;
			for (auto i = p; i != _last; ++i)
			{
				auto temp = new node(*p);
				t->next = temp;
				temp->last = t;
				t = last;
			}
			charu(*pos, k, t);
			return iterator{ t };
		}
		iterator insert(const iterator& pos, std::initializer_list<T> ilist)
		{
			return insert(pos, ilist.begin(), ilist.end());
		}

		template< class... Args >
		iterator emplace(const iterator& pos, Args&&... args)
		{
			auto k = new node(std::forward<Args>(args)...);
			charu(pos.t, k, k);
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
			first.t->last->next = last.t;
			last.t->last->next = nullptr;
			last.t->last = first.t->last;
			first.t->destory();
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
		void resize(size_t count, const T& value)
		{
			if (t == 0)
				return clear();
			auto p = nul.next;
			while (p != &nul && count != 0)
			{
				p = p->next;
				--count;
			}
			if (p == &nul && count != 0)
			{
				auto k = new node(value);
				--count;
				auto l = k;
				while (count--)
				{
					auto temp = new node(value);
					l->next = temp;
					temp->last = l;
					l = temp;
				}
				charu(end().t, k, l);
			}
			else if (p != &nul&&count == 0)
				erase(iterator(p), end());
		}

		void swap(list& other)noexcept
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
			if (this->empty())
				return this->swap(other);
			auto ob = other.nul.next;
			auto oe = other.nul.last;
			auto q = nul.next;
			while (q != &nul&&ob != &other.nul)
			{
				if (comp(*(ob->dat), *(q->dat)))
				{
					auto k = ob;
					while (k != &other.nul && comp(*(k->dat), *(q->dat)))
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
		template <typename Compare>
		void merge(list&& other, Compare comp)
		{
			auto t{ std::move(other) };
			return merge(t, comp);
		}

		void splice(const iterator& pos, list& other)
		{
			splice(pos,other, other.begin(), other.end());
		}
		void splice(const iterator& pos, list&& other)
		{
			auto temp{ std::move(other) };
			splice(pos, other,other.begin(), other.end());
		}
		void splice(const iterator& pos, list& other, const iterator& it)
		{
			auto temp{ it };
			splice(pos, other, it, ++temp);
		}
		void splice(const iterator& pos, list&& other, const iterator& it)
		{
			auto temp{ it };
			splice(pos, other, it, ++temp);
		}
		void splice(const iterator& pos, list& other, const iterator& first, const iterator& last)
		{
			if (other == *this || first == last)
				return;
			auto k = first.t;	
			auto p = last.t->last;
			k->last->next = last.t;
			p->next->last = first.t->last;
			charu(pos.t, k, p);
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
				if (p(*(l->dat)))
				{
					l->last->next = l->next;
					l->next->last = l->last;
					auto w = l->next;
					delete l;
					l = w;
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
				if (p(*(k->dat), *(k->next->dat)))
				{
					auto t = k->next;
					k->next = k->next->next;
					k->next->last = k;
					delete t;
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
			auto be = nul.next;
			auto en = nul.last;
			nul.last = nul.next = &nul;
			be->last = nullptr;
			en->next = nullptr;
			auto mid = midden(be);
			auto k = mid->next;
			list t1{ be,mid };
			list t2{ k,en };
			t1.sort(comp);
			t2.sort(comp);
			t1.merge(t2, comp);
			this->swap(t1);
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
			//	std::cout << *(q->dat) << std::endl;
			}
			return m;
		}
		list(node*a, node*b):list()
		{
			charu(nullptr, a, b);
		}
		void charu(node* d,node* b, node* e)
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
		int comp(const list& t)const noexcept
		{
			auto k = nul.next;
			auto p = t.nul.next;
			while (k != &nul&&p != &(t.nul))
			{
				if (*(k->dat) < *(p->dat))
					return -1;
				else if (*(k->dat) > *(p->dat))
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
		
		node nul;
	};
	template< class T, class Alloc >
	 bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) == 0;
	}

	template< class T, class Alloc >
	 bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) != 0;
	}

	template< class T, class Alloc >
	 bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) < 0;
	}

	template< class T, class Alloc >
	 bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) <= 0;
	}

	template< class T, class Alloc >
	 bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return lhs.comp(rhs) > 0;
	}

	template< class T, class Alloc >
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