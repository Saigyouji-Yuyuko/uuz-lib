#pragma once
#include"prepare.h"
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
		list()
		{
			null->next = null->last = &null;
		}
		explicit list(size_t t):list()
		{
			for (int i = 0; i < t; ++i)
			{
				auto k = new node();
				k->insert(null);
			}
		}
		list(size_t t, const T& p):list()
		{
			for (int i = 0; i < t; ++i)
			{
				auto k = new node(p);
				k->insert(null);
			}
		}
		template< typename InputIt,typename = decltype(*(std::declval<InputIt>()))>
		list(const InputIt& first,const InputIt& last):list()
		{
			for (const auto& i = first; i != last; ++i)
			{
				auto k = new node(*i);
				k->insert(null->last);
			}
		}
		list(const std::initializer_list<T>& init)
		{
			list(init.begin(), init.end()));
		}
		list(const self& t)
		{
			list(t.begin(), t.end());
		}
		list(self&& t):list()
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
		void assign(const InputIt& first, const InputIt& last)
		{
			auto temp(first, last);
			this->swap(temp);
		}
		void assign(const std::initializer_list<T>& ilist)
		{
			auto temp{ ilist };
			this->swap(temp);
		}

		T& front()noexcept
		{
			return *(begin());
		}
		const T& front()const noexcept
		{
			return *(begin());
		}

		T& back()noexcept
		{
			return *(rbein());
		}
		const T& back()const noexcept
		{
			return *(rbegin());
		}

		iterator begin()noexcept
		{
			return iterator{ null->next };
		}
		const iterator begin()const noexcept
		{
			return iterator{ null->next };
		}
		const iterator cbegin()const noexcept
		{
			return iterator{ null->next };
		}

		iterator end()noexcept
		{
			return iterator{ null };
		}
		const iterator end()const noexcept
		{
			return  iterator{ null };
		}
		const iterator end()const noexcept
		{
			return iterator{ null };
		}

		bool empty()const noexcept
		{
			return null->last == &null;
		}

		size_t size()const noexcept
		{
			size_t t = 0;
			auto k = null->next;
			while (k != null)
			{
				k = k->next;
				++t;
			}
			return t;
		}
		
		size_t max_size()const noexcept
		{
			return size();
		}

		void clear()noexcept
		{
			if (!empty())
			{
				auto k = null->next;
				null->last->next = nullptr;
				null->next = null->last = &null;
				k->destroy();
			}
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
		{
			auto k = new node(T(std::forward<Args>(args)...));
			k->insert(pos->t);
			return iterator{ k };
		}

		iterator erase(const iterator& pos)
		{
			return erase(pos, pos->next);
		}
		iterator erase(const iterator& first, const iterator& last)
		{
			if (first == begin() && last == end())
			{
				clear();
				return end();
			}
			last.t->last->next = nullptr;
			last.t->last = first.t->last;
			first.t->last->next = last;
			first.t->destroy();
			return last;
		}

		void push_back(const T& value)
		{
			emplace_back(std::move(temp));
		}
		void push_back(T&& value)
		{
			emplace_back(std::move(value));
		}

		template< typename... Args >
		T& emplace_back(Args&&... args)
		{
			return emplace(iterator{ null->last }, std::forward<Args>(args)...)->dat;
		}

		void pop_back()
		{
			null->last->last->next = null;
			null->last->next = nullptr;
			auto k = null->last;
			null->last = null->last->last;
			k->destroy();
		}

		void push_front(const T& value)
		{
			emplace_front(temp);
		}
		void push_front(T&& value)
		{
			emplace_front(std::move(temp));
		}

		template< class... Args >
		T& emplace_front(Args&&... args)
		{
			return emplace(iterator{ &null }, std::forward<Args>(args)...)->dat;
		}

		void pop_front()
		{
			null->next->next->last = null;
			null->next->next = nullptr;
			auto k = null->next;
			null->next = null->next->next;
			k->destroy();
		}

		void resize(size_t count);
		void resize(size_t count, const T& value);

		void swap(list& other)noexcept
		{
			using std::swap;
			swap(null, other.null);
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
		void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs)noexcept
		{
			lhs.swap(rhs);
		}

		~list()
		{
			clear();
		}
	private:
		list_node null;
	};
}