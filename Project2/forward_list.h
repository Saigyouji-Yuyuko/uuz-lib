#pragma once
#include"prepare.h"
namespace uuz
{
	template<typename T, typename Allocator = uuz::allocator>
	struct forward_list_node
	{
		forward_list_node() = default;
		forward_list_node(const T& p) :dat(new T(p)) {}
		forward_list_node(T&& p) :dat(new T(std::move(p))) {}
		template<typename...Args>
		forward_list_node(Args...args):dat(new T(uuz::forward<Args>(args)...)) {}
		void destroy()noexcept
		{
			if (next&&next->dat)
				next->destroy();
			delete this;
			return;
		}
		T* dat = nullptr;
		forward_list_node* next = nullptr;
	};

	template<typename T, typename Allocator>
	class forward_list;


	template<typename T, typename Allocator>
	class forward_list_iterator
	{
		using self = forward_list_iterator;
		friend forward_list<T, Allocator>;
	public:
		self& operator++() noexcept
		{
			t = t->next;
			return *this;
		}
		self operator++(int)noexcept
		{
			auto p{ *this };
			t = t->next;
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
		self(forward_list_node<T>* tt) :t(tt) {}
		self(const forward_list_node<T>* tt) :t(const_cast<forward_list_node<T>*>(tt)) {}
		self() = default;
		forward_list_node<T>* t = nullptr;
	};

	template<typename T, typename Allocator = uuz::allocator>
	class forward_list
	{
		using iterator = forward_list_iterator<T, Allocator>;
		using self = forward_list;
		using node = forward_list_node<T, Allocator>;
		using size_t = unsigned int;
	public:
		forward_list() /*: forward_list(Allocator()) {}*/ = default;
		//explicit forward_list(const Allocator& alloc);
		forward_list(size_t count, const T& value/*,const Allocator& alloc = Allocator()*/):forward_list()
		{
			if (count)
			{
				auto k = new node(value);
				nul.next = k;
				for (auto i = 1; i < count; ++i)
				{
					k->next = new node(value);
					k = k->next;
				}
			}
		}
		explicit forward_list(size_t count/*,const Allocator& alloc = Allocator()*/):forward_list(count, T{}) {}
		template<typename InputIt , typename = is_input<T,InputIt>>
		forward_list(const InputIt& first, const InputIt& last/*,const Allocator& alloc = Allocator()*/):forward_list()
		{
			if (first != last)
			{
				auto k = new node(*first);
				nul.next = k;
				auto i = first;
				++i;
				for (; i !=last; ++i)
				{
					k->next = new node(*i);
					k = k->next;
				}
			}
		}
		forward_list(const forward_list& other):forward_list(other.begin(),other.end()){}
		//forward_list(const forward_list& other/*,const Allocator& alloc = Allocator()*/));
		forward_list(forward_list&& other):forward_list{}
		{
			this->swap(other);
		}
		//forward_list(forward_list&& other, const Allocator& alloc);
		forward_list(std::initializer_list<T> init/*,const Allocator& alloc = Allocator()*/) :forward_list{init.begin(),init.end()}{}

		forward_list& operator=(const forward_list& other)
		{
			if (this == &other)
				return *this;
			auto temp(other);
			this->swap(temp);
			return *this;
		}
		forward_list& operator=(forward_list&& other) noexcept//(/* see below */)
		{
			if (this == &other)
				return *this;
			auto temp{ std::move(other) };
			this->swap(temp);
			return *this;

		}
		forward_list& operator=(std::initializer_list<T> ilist)
		{
			auto temp{ ilist };
			this->swap(temp);
			return *this;
		}

		void assign(size_t count, const T& value)
		{
			forward_list temp(count, value);
			this->swap(temp);
		}
		template<typename InputIt, typename = is_input<T, InputIt>>
		void assign(const InputIt& first,const InputIt& last)
		{
			forward_list temp(first, last);
			this->swap(temp);
		}
		void assign(std::initializer_list<T> ilist)
		{
			forward_list temp(ilist);
			this->swap(temp);
		}
		
		T& front()noexcept
		{
			return *(nul.next->dat);
		}
		const T& front() const noexcept
		{
			return *(nul.next->dat);
		}
		
		
		iterator before_begin() noexcept
		{
			return iterator{ &nul };
		}
		const iterator before_begin() const noexcept
		{
			return iterator{ &nul };
		}
		const iterator cbefore_begin() const noexcept
		{
			return iterator{ &nul };
		}

		iterator begin() noexcept
		{
			return iterator{ nul.next };
		}
		const iterator begin() const noexcept
		{
			return iterator{ nul.next };
		}
		const iterator cbegin() const noexcept
		{
			return iterator{ nul .next };
		}

		iterator end() noexcept
		{
			return iterator() ;
		}
		const iterator end() const noexcept
		{
			return iterator{ };
		}
		const iterator cend() const noexcept
		{
			return iterator{ };
		}
	
		bool empty() const noexcept
		{
			return nul.next == nullptr;
		}

		//size_t max_size() const noexcept;

		void clear() noexcept
		{
			if(nul.next)
				nul.next->destroy();
			nul.next = nullptr;
		}

		iterator insert_after(const iterator& pos, const T& value)
		{
			return emplace_after(pos, value);
		}
		iterator insert_after(const iterator& pos, T&& value)
		{
			return emplace_after(pos, std::move(value));
		}
		iterator insert_after(const iterator& pos, size_t count, const T& value)
		{
			if (count)
			{
				auto k = new node(value);
				auto p = k;
				for (int i = 1; i != count; ++i)
				{
					p->next = new node(value);
					p = p->next;
				}
				p->next = pos.t->next;
				pos.t->next = k;
				return { p };
			}
			return pos;
		}
		template< class InputIt >
		iterator insert_after(const iterator& pos,const InputIt& first,const InputIt& last)
		{
			if (first != last)
			{
				auto k = new node(*first);
				auto p = k;
				auto i = first;
				++i;
				for (; i != last; ++i)
				{
					p->next = new node(*i);
					p = p->next;
				}
				p->next = pos.t->next;
				pos.t->next = k;
				return { p };
			}
			return pos;
		}
		iterator insert_after(const iterator& pos, std::initializer_list<T> ilist)
		{
			return insert_after(pos, ilist.begin(), ilist.end());
		}
		
		template< typename... Args >
		iterator emplace_after(const iterator& pos, Args&&... args)
		{
			auto k = new node(uuz::forward<Args>(args)...);
			k->next = pos.t->next;
			pos.t->next = k;
			return iterator{ k };
		}


		iterator erase_after(const iterator& pos)
		{
			auto k = pos.t->next;
			pos.t->next = k->next;
			delete k;
			return pos;
		}
		iterator erase_after(const iterator& first, const iterator& last)
		{
			if (first == last || first.t->next == last.t)
				return last;
			if (last == end())
			{
				first.t->next->destroy();
				return end();
			}
			auto k = first.t->next;
			while (k->next != last)
				k = k->next;
			k->next = nullptr;
			first.t->next->destroy();
			first.t->next = last.t;
		}

		void push_front(const T& value)
		{
			emplace_front(value);
		}
		void push_front(T&& value)
		{
			emplace_front(std::move(value));
		}
	
		template< typename... Args >
		T& emplace_front(Args&&... args)
		{
			auto k = new node(uuz::forward<Args>(args)...);
			k->next = nul.next;
			nul.next = k;
			return *(k->dat);
		}

		void pop_front()
		{
			auto k = nul.next;
			nul.next = nul.next->next;
			delete k;
		}

		void resize(size_t count)
		{
			return resize(count, T{});
		}
		void resize(size_t count, const T& value)
		{
			if (count == 0)
				return clear();
			auto k = &nul;
			while (k->next&&count)
			{
				--count;
				k = k->next;
			}
			if (k->next == nullptr&&count)
			{
				while (count--)
				{
					k->next = new node(value);
					k = k->next;
				}
			}
			else if (k->next != nullptr)
			{
				k->next->destroy();
				k->next = nullptr;
			}
		}

		void swap(forward_list& other) noexcept//(/* see below */)
		{
			using std::swap;
			swap(nul.next, other.nul.next);
		}

		void merge(forward_list& other)
		{
			merge(other, [](const T& a, const T& b) {return a < b; });
		}
		void merge(forward_list&& other)
		{
			merge(std::move(other), [](const T& a, const T& b) {return a < b; });
		}
		template <typename Compare>
		void merge(forward_list& other, const Compare& comp)
		{
			if (other.empty() || this == &other)
				return;
			if (this->empty())
				return this->swap(other);
			auto k = other.nul.next;
			auto t = &nul;
			while (k&&t->next)
			{
				if (comp(*(k->dat), *(t->next->dat)))
				{
					auto p = k;
					while (p->next&&comp(*(p->next->dat), *(t->next->dat)))
						p = p->next;
					auto tt = p->next;
					p->next = t->next;
					t->next = k;
					t = p->next;
					k = tt;
				}
				else
					t = t->next;
			}
			if (t->next == nullptr && k)
				t->next = k;
			other.nul.next = nullptr;
		}
		template <typename Compare>
		void merge(forward_list&& other,const Compare& comp)
		{
			auto temp{ std::move(other) };
			merge(temp, comp);
		}

		void splice_after(const iterator& pos, forward_list& other)
		{
			splice_after(pos, other.begin(), other.end());

		}
		void splice_after(const iterator& pos, forward_list&& other)
		{
			auto temp{ std::move(other) };
			splice_after(pos, temp.begin(), temp.end());
		}
		void splice_after(const iterator& pos, forward_list& other,const iterator& it)
		{
			auto t{ it };
			splice_after(pos, other, it, ++t);
		}
		void splice_after(const iterator& pos, forward_list&& other,const iterator& it)
		{
			auto t{ it };
			splice_after(pos, other, it, ++t);
		}
		void splice_after(const iterator& pos, forward_list& other,const iterator& first, const iterator& last)
		{
			if (first == last || first.t->next == last || &other == this)
				return;
			auto p = pos.t;
			auto p1 = p->next;
			p->next = first.t->next;
			auto k = first.t->next;
			first.t->next = last.t;
			while (k->next != last.t)
				k = k->next;
			k->next = p1;
			
		}
		void splice_after(const iterator& pos, forward_list&& other,const iterator& first, const iterator& last)
		{
			splice_after(pos, other, first, last);
		}

		void remove(const T& value)
		{
			return remove_if([&](const T& a) {return a == value; });
		}
		template< typename UnaryPredicate >
		void remove_if(const UnaryPredicate& p)
		{
			auto k = &nul;
			while (k->next)
			{
				if (p(*(k->next->dat)))
				{
					auto temp = k->next;
					k->next = temp->next;
					delete temp;
				}
				else
					k = k->next;
			}
		}

		void reverse() noexcept
		{
			auto k = nul.next;
			nul.next = nullptr;
			while (k)
			{
				auto temp = k->next;
				k->next = nul.next;
				nul.next = k;
				k = temp;
			}
		}

		void unique()
		{
			return unique([](const T& a, const T& b) {return a == b; });
		}
		template< typename BinaryPredicate >
		void unique(const BinaryPredicate& p)
		{
			auto k = nul.next;
			while(k->next)
			{
				if (p(*(k->dat), *(k->next->dat)))
				{
					auto t = k->next;
					k->next = k->next->next;
					delete t;
				}
				else
					k = k->next;
			}
		}

		void sort()
		{
			return sort([](const T&a, const T& b) {return a < b; });
		}
		template< typename Compare >
		void sort(const Compare& comp)
		{
			if (empty() || nul.next->next == nullptr)
				return;
			auto k = nul.next;
			auto q = k;
			auto p = k;
			while (q->next&&q->next->next)
			{
				q = q->next->next;
				p = p->next;
				
			}
			auto d = p->next;
			p->next = nullptr;
			forward_list t1{ k };
			forward_list t2{ d };
			t1.sort(comp);
			t2.sort(comp);
			t1.merge(t2,comp);
			this->nul.next = nullptr;
			this->swap(t1);
		}

		int compare(const forward_list& a)noexcept
		{
			auto k = nul.next;
			auto p = a.nul.next;
			while (k&&p)
			{
				if (*(k->dat) < *(p->dat))
					return -1;
				else if (*(p->dat) < *(k->dat))
					return 1;
				k = k->next;
				p = p->next;
			}
			if (k && !p)
				return 1;
			else if (!k&&p)
				return -1;
			return 0;
		}

		~forward_list()
		{
			clear();
		}
	private:
		forward_list(node*a)
		{
			nul.next = a;
		}
		node nul;
	};
	template< typename T, typename Alloc >
	bool operator==(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) == 0;
	}
	template< typename T, typename Alloc >
	bool operator!=(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) != 0;
	}
	template< typename T, typename Alloc >
	bool operator<(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) < 0;
	}
	template< typename T, typename Alloc >
	bool operator<=(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) <= 0;
	}
	template< typename T, typename Alloc >
	bool operator>(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) > 0;
	}
	template< typename T, typename Alloc >
	bool operator>=(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return lhs.compare(rhs) >= 0;
	}

	template< typename T, typename Alloc >
	void swap(forward_list<T, Alloc>& lhs, forward_list<T, Alloc>& rhs) noexcept(lhs.swap(rhs))
	{
		lhs.swap(rhs);
	}
}