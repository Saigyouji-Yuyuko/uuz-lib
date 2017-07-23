#pragma once
#include"prepare.h"
#include<cassert>
namespace uuz
{
	template<typename T,typename A>
	class list;

	template<typename T,typename A>
	struct list_node
	{
		list_node() = default;
		list_node(const T& p):dat{p}{}
		list_node(T&& p):dat{std::move(p)}{}
		T dat;
		self* next = nullptr;
		self* last = nullptr;

		void destroy()
		{
			if (next == nullptr)
			{
				delete this;
				return;
			}
			next->destroy();
			delete this;
			return;
		}

		void tuoli()
		{
			last->next = next;
			next->last = last;
			next = nullptr;
			last = nullptr;
		}

		void insert(list_node* a)
		{
			auto p = a->next;
			a->next = this;
			p->last = this;
			this->last = a;
			this->next = p;
		}
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
			return t->dat;
		}
		const T& operator*()const noexcept
		{
			return t->dat;
		}
		T* operator->()noexcept
		{
			return &(t->dat);
		}
		const T* operator->()const noexcept
		{
			return &(t->dat);
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
		self(list_node* tt):t(tt){}
		self(const list_node* tt):t(const_cast<list_node*>(tt)){}
		list_node* t = nullptr;
	};

	template<typename T,typename Allocator = uuz::allocator>
	class list
	{
		using iterator = list_iterator<T, Allocator>;
		using self = list;
		using node = list_node<T, Allocator>;
		using size_t = _Uint32t;
	public:
		list() = default;
		explicit list(size_t t)
		{
			first = new node();
			assert(first);
			auto k = first;
			for (auto i = 1; i < t; ++i)
			{
				auto p = new node();
				p->last = k;
				k->next = p;
				k = p;
			}
			last = k;
		}
		list(size_t t, const T& p)
		{
			first = new node(p);
			assert(first);
			auto k = first;
			for (auto i = 1; i < t; ++i)
			{
				auto p = new node(p);
				p->last = k;
				k->next = p;
				k = p;
			}
			last = k;
		}
		template< typename InputIt,typename = decltype(*(std::declval<InputIt>()))>
		list(const InputIt& _first, const InputIt& _last)
		{
			first = new node(_first);
			auto k = ++_first;
			auto temp = first;
			for (auto i = k; i != last;++i)
			{
				auto p = new node(*k);
				temp->next = p;
				p->last = temp;
				temp = p;
			}
			last = temp;
		}
		list(const std::initializer_list<T>& init) :list(init.begin(), init.end()) {}
		list(const self& t):list(t.cbegin(),t.cend()){}
		list(self&& t):self()
		{
			this->swap(t);
		}

		list& operator=(const list& other)noexcept
		{
			if (this == &other)
				return *this;
			auto temp(other);
			this->swap(other);
			return *this;
		}
		list& operator=(list&& other)noexcept
		{
			if (this == &other)
				return *this;
			auto temp(std::move(other));
			this->swap(other);
			return *this;
		}
		list& operator=(const std::initializer_list<T>& ilist)noexcept
		{
			auto temp(ilist);
			this->swap(other);
			return *this;
		}

		void assign(size_t count, const T& value)
		{
			auto temp(count, value);
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
			return first->dat;
		}
		const T& front()const
		{
			return first->dat;
		}

		T& back()
		{
			return last->dat;
		}
		const T& back()const
		{
			return last->dat;
		}

		iterator begin()noexcept
		{
			return iterator(first);
		}
		const iterator begin()const noexcept
		{
			return iterator{ first };
		}
		const iterator cbegin()const noexcept
		{
			return iterator{ first };
		}

		iterator end()noexcept
		{
			return iterator{ nullptr };
		}
		const iterator end()const noexcept
		{
			return iterator{ nullptr };
		}
		const iterator end()const noexcept
		{
			return iterator{ nullptr };
		}

		bool empty()const noexcept
		{
			return first == nullptr;
		}

		size_t size()const
		{
			size_t t = 0;
			auto k = first;
			while (k != nullptr)
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
			first->destorr();
			first = nullptr;
			last = nullptr;
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

		}
		template<typename InputIt ,typename = decltype(*(std::declval<InputIt>()))>
		iterator insert(const iterator& pos,const InputIt& first,const InputIt& last);
		iterator insert(const iterator& pos, std::initializer_list<T> ilist);

		template< class... Args >
		iterator emplace(const iterator& pos, Args&&... args)
		

		iterator erase(const iterator& pos)
		iterator erase(const iterator& first, const iterator& last)

		void push_back(const T& value)
		void push_back(T&& value)

		template< typename... Args >
		T& emplace_back(Args&&... args)

		void pop_back()

		void push_front(const T& value)
		void push_front(T&& value)

		template< class... Args >
		T& emplace_front(Args&&... args)

		void pop_front()

		void resize(size_t count);
		void resize(size_t count, const T& value);

		void swap(list& other)
		{
			using std::swap;
			swap(first, other.first);
			swap(last, other.last);
		}

		void merge(list& other);
		void merge(list&& other);
		template <class Compare>
		void merge(list& other, Compare comp);
		template <class Compare>
		void merge(list&& other, Compare comp);

		void splice(const_iterator pos, list& other);
		void splice(const_iterator pos, list&& other);
		void splice(const_iterator pos, list& other, const_iterator it);
		void splice(const_iterator pos, list&& other, const_iterator it);
		void splice(const_iterator pos, list& other,
			const_iterator first, const_iterator last);
		void splice(const_iterator pos, list&& other,
			const_iterator first, const_iterator last);

		void remove(const T& value);
		template< class UnaryPredicate >
		void remove_if(UnaryPredicate p);

		void reverse();

		void unique();
		template< class BinaryPredicate >
		void unique(BinaryPredicate p);

		void sort();
		template< class Compare >
		void sort(Compare comp);

		template< class T, class Alloc >
		bool operator==(const list<T, Alloc>& lhs,
			const list<T, Alloc>& rhs);
	
			template< class T, class Alloc >
		bool operator!=(const list<T, Alloc>& lhs,
			const list<T, Alloc>& rhs);
		
			template< class T, class Alloc >
		bool operator<(const list<T, Alloc>& lhs,
			const list<T, Alloc>& rhs);
		
			template< class T, class Alloc >
		bool operator<=(const list<T, Alloc>& lhs,
			const list<T, Alloc>& rhs);
		
			template< class T, class Alloc >
		bool operator>(const list<T, Alloc>& lhs,
			const list<T, Alloc>& rhs);
		
			template< class T, class Alloc >
		bool operator>=(const list<T, Alloc>& lhs,
			const list<T, Alloc>& rhs);

		template< typename T, typename Alloc >
		void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs)
		{
			lhs.swap(rhs);
		}

		~list()
		{
			clear();
		}
	private:
		node* first = nullptr;
		node* last = nullptr;
	};
}