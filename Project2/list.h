#pragma once
#include"prepare.h"
namespace uuz
{
	template<typename T,typename A>
	class list;

	template<typename T,typename A>
	struct list_node
	{
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
		using size_t = _Uint32t;
	public:
		explicit list(size_t t);
		list(size_t t,const T& p);
		template< typename InputIt,typename = decltype(*(std::declval<InputIt>()))>
		list(const InputIt& first,const InputIt& last);
		list(const std::initializer_list<T>& init);
		list(const self& t);
		list(self&& t);

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
			return *(end() - 1);
		}
		const T& back()const noexcept
		{
			return *(end() - 1);
		}

		iterator begin()noexcept
		{
			return iterator{ head };
		}
		const iterator begin()const noexcept
		{
			return iterator{ head };
		}
		const iterator cbegin()const noexcept
		{
			return iterator{ head };
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
			return head == null;
		}

		size_t size()const noexcept
		{
			size_t t = 0;
			auto p = head;
			while (p != null)
			{
				p = p->next;
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
			null->next = nullptr;
			head->destory();	
			head = null = nullptr;
		}

		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);
		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last);
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args);

		iterator erase(const iterator& pos)
		{
			return erase(pos, pos->next);
		}
		iterator erase(const iterator& first, const iterator& last)
		{
			if (first == begin() && last == end())
				clear();
			else
			{
				first->last->next = last;
				last->last->next = nullptr;
				last->last = first->last;
				first.destroy();
			}
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
			T temp(std::forward<Args>(args)...);
			auto  k = new list_node();
			k.dat = std::move(temp);
			k->insert(nill);
			return k->dat;
		}

		void pop_back();

		void push_front(const T& value);
		void push_front(T&& value);

		template< class... Args >
		reference emplace_front(Args&&... args);

		void pop_front();

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(list& other)noexcept
		{
			using std::swap;
			swap(head, other.head);
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
		list_node* head = nullptr;
		list_node* nill = nullptr;
		list_node null;
	};
}