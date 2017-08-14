#pragma once
#include"container.h"
#include"pair.h"
namespace uuz
{

	struct forward_list_node_base
	{
		forward_list_node_base* next = nullptr;
	}; 

	template<typename T>
	struct forward_list_node:public forward_list_node_base
	{

		forward_list_node() = default;
		forward_list_node(const T& p) :data(p) {}
		forward_list_node(T&& p) :data(std::move(p)) {}
		template<typename...Args>
		forward_list_node(Args...args):data(uuz::forward<Args>(args)...) {}
		  void destroy()noexcept
		{
			this->~forward_list_node();
		}
		T data;
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
			return ((forward_list_node<T>*)t)->data;
		}
		const T& operator*()const noexcept
		{
			return ((forward_list_node<T>*)t)->data;
		}
		T* operator->()noexcept
		{
			return &(((forward_list_node<T>*)t)->data);
		}
		const T* operator->()const noexcept
		{
			return &(((forward_list_node<T>*)t)->data);
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
		self(forward_list_node_base* tt) :t(tt) {}
		self(const forward_list_node_base* tt) :t(const_cast<forward_list_node_base*>(tt)) {}
		forward_list_node_base* t = nullptr;
	};

	template<typename T, typename A = uuz::allocator<T>>
	class forward_list
	{
	public:
		using iterator = forward_list_iterator<T, A>;
		
		using self = forward_list;
		using node = forward_list_node<T>;
		using Allocator = typename uuz::exchange<A, node>::type;
		using size_t = unsigned int;
	private:
		Allocator alloc;
		forward_list_node_base nul;
	public:
		forward_list() : forward_list(Allocator()) {}
		explicit forward_list(const A& alloc) :alloc(alloc){}
		forward_list(size_t count, const T& value, const A& alloc = A())
			noexcept(noexcept(forward_list(alloc))&& noexcept(this->alloc.allocate())&&std::is_nothrow_copy_constructible_v<T>)
			:forward_list(alloc)
		{
			if (count)
			{
				try
				{
					nul.next = make_list(value);
					auto k = nul.next;
					++i;
					for (auto i = 0; i < count; ++i)
					{
						k->next = make_list(value);
						k = k->next;
					}
				}
				catch (...)
				{
					clear();
					throw;
				}
			}
		}
		explicit forward_list(size_t count, const A& alloc = A())noexcept(noexcept(forward_list(count, T{}, alloc))) :forward_list(count, T{}, alloc) {}
		template<typename InputIt, typename = is_input<T, InputIt>>
		forward_list(const InputIt& first, const InputIt& last, const A& alloc = A()) : forward_list(alloc)
		{
			if (first != last)
			{
				auto i = first;
				try
				{
					nul.next = make_list(*i);
					auto k = nul.next;
					++i;
					for (; i != last; ++i)
					{
						k->next = make_list(*i);
						k = k->next;
					}
				}
				catch (...)
				{
					clear();
					throw;
				}
			}
		}
		forward_list(const forward_list& other) :forward_list(other.begin(), other.end(),A()) {}
		forward_list(const forward_list& other, const A& alloc = A()):forward_list(other.begin(), other.end(), alloc) {}
		forward_list(forward_list&& other):forward_list{}
		{
			this->swap(other);
		}
		forward_list(forward_list&& other, const A& alloc):forward_list(alloc)
		{
			this->swap(other);
		}
		forward_list(std::initializer_list<T> init,const A& alloc = A()) :forward_list{init.begin(),init.end(),alloc}{}

		forward_list& operator=(const forward_list& other)
		{
			if (this == &other)
				return *this;
			auto temp(other,alloc);
			this->swap(temp);
			return *this;
		}
		forward_list& operator=(forward_list&& other) noexcept//(/* see below */)
		{
			if (this == &other)
				return *this;
			auto temp{ std::move(other),alloc };
			this->swap(temp);
			return *this;

		}
		forward_list& operator=(std::initializer_list<T> ilist)
		{
			auto temp{ ilist ,alloc};
			this->swap(temp);
			return *this;
		}

		  void assign(size_t count, const T& value)
		{
			forward_list temp(count, value,alloc);
			this->swap(temp);
		}
		template<typename InputIt, typename = is_input<T, InputIt>>
		  void assign(const InputIt& first,const InputIt& last)
		{
			forward_list temp(first, last,alloc);
			this->swap(temp);
		}
		  void assign(std::initializer_list<T> ilist)
		{
			forward_list temp(ilist,alloc);
			this->swap(temp);
		}
		
		T& front()
		{
			if (empty())
				throw(out_of_range(""));
			return ((node*)nul.next)->data;
		}
		const T& front() const 
		{
			if (empty())
				throw(out_of_range(""));
			return ((node*)nul.next)->data;
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
			return iterator{ nul.next };
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
			list_destroy(nul.next);
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
				int i = 0;
				node* k = nullptr;
				try
				{
					k = make_list(value);
					auto p = k;
					++i;
					for (; i != count; ++i)
					{
						p->next = make_list(value);
						p = p->next;
					}
				}
				catch (...)
				{
					list_destroy(k);
					throw;
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
				auto i = first;
				node* k = nullptr;
				try
				{
					k = make_list(*first);
					auto p = k;
					++i;
					for (; i != last; ++i)
					{
						p->next = make_list(*i);
						p = p->next;
					}
				}
				catch (...)
				{
					list_destroy(k);
					throw;
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
			auto k = make_list(uuz::forward<Args>(args)...);
			k->next = pos.t->next;
			pos.t->next = k;
			return iterator{ k };
		}


		iterator erase_after(const iterator& pos)
		{
			auto k = pos.t->next;
			pos.t->next = k->next;
			k->next = nullptr;
			list_destroy(k);
			return pos;
		}
		iterator erase_after(const iterator& first, const iterator& last)
		{
			if (first == last || first.t->next == last.t)
				return last;
			if (last == end())
			{
				list_destroy(first.t->next);
				first.t->next = nullptr;
				return end();
			}
			auto k = first.t->next;
			while (k->next != last)
				k = k->next;
			k->next = nullptr;
			list_destroy(first.t->next);
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
			auto k = make_list(uuz::forward<Args>(args)...);
			k->next = nul.next;
			nul.next = k;
			return ((node*)k)->data;
		}

		  void pop_front()
		{
			auto k = nul.next;
			nul.next = nul.next->next;
			k->next = nullptr;
			list_destroy(k);
		}

		  void resize(size_t count)
		{
			 resize(count, T{});
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
				try
				{
					k->next = make(value);
					--count;
					auto t = k->next;
					while (count--)
					{
						t->next = make_list(value);
						k = k->next;
					}
				}
				catch (...)
				{
					list_destroy(k->next);
					throw;
				}
				
			}
			else if (k->next != nullptr)
			{
				list_destroy(k->next);
				k->next = nullptr;
			}
		}

		  void swap(forward_list& other) noexcept(is_nothrow_swap_alloc<Allocator>::value)
		{
			using std::swap;
			if(alloc == other.alloc)
				swap(nul.next, other.nul.next);
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
			if (alloc != other.alloc)
			{
#ifdef DEBUG
				assert(false, "It's undefined that allocate is not equal");
#else
				while (k&&t->next)
				{
					if (comp(k->data.get(), t->next->data.get()))
					{
						auto p = k;
						while (p->next&&comp(p->next->data.get(), t->next->data.get()))
							p = p->next;
						auto ff = makelist(k, p);
						ff.second->next = t->next;
						t->next = ff.first;
						t = ff.second->next;
					}
					else
						t = t->next;
				}
				if (t->next == nullptr && k)
				{
					auto ff = makelist(iterator(k), other.end());
					t->next = ff.first;
				}
				other.clear();
#endif // DEBUG
			}
			else
			{
				while (k&&t->next)
				{
					if (comp(k->data.get(), t->next->data.get()))
					{
						auto p = k;
						while (p->next&&comp(p->next->data.get(), t->next->data.get()))
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
			if (alloc == other.alloc)
			{
				auto p = pos.t;
				auto p1 = p->next;
				p->next = first.t->next;
				auto k = first.t->next;
				first.t->next = last.t;
				while (k->next != last.t)
					k = k->next;
				k->next = p1;
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's undefined that allocate is not equal");
#else
				auto pp = first;
				++pp;
				auto f = makelist(pp, last);
				f.second->next = pos.t->next;
				pos.t->next = f.first;
				other.erase_after(first, last);
#endif 
			}
			
			
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
				if (p(((node*)k->next)->data))
				{
					auto temp = k->next;
					k->next = temp->next;
					temp->next = nullptr;
					list_destroy(temp);
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
				if (p(((node*)k)->data, ((node*)(k->next))->data))
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
			forward_list t1( k ,alloc);
			forward_list t2( d ,alloc);
			t1.sort(comp);
			t2.sort(comp);
			t1.merge(t2,comp);
			this->nul.next = nullptr;
			this->swap(t1);
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
		
		template<typename...Args>
		node* make_list(Args...args)
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

		void list_destroy(forward_list_node_base* p)noexcept
		{
			if (!p)
				return;
			if (p->next)
				list_destroy(p->next);
			p->destroy();
			alloc.deallocate(p, 1);
		}

		pair<node*, node*>makelist(node* a, node* b)
		{
			node* k = nullptr;
			try
			{
				k = make_list(a->data);
				auto p = k;
				for (auto i = a->next; i != b->next; i = i->next)
				{
					p->next = make((node*(i))->data);
					p = p->next;
				}
				return make_pair(k, p);
			}
			catch(...)
			{
				list_destroy(k);
				throw;
			}
		}

		template<typename InputIt,typename = is_input<T,InputIt>>
		pair<node*, node*>makelist(InputIt a, InputIt b)
		{
			node* k = nullptr;
			try
			{
				k = make_list(*a);
				auto p = k;
				auto i = a;
				++i;
				for (; i != b; ++i)
				{
					p->next = make((node*(i))->data);
					p = p->next;
				}
				return make_pair(k, p);
			}
			catch (...)
			{
				list_destroy(k);
				throw;
			}
		}
		
	};
	namespace
	{
		template<typename T,typename U>
		int compare(const forward_list<T,U>& a, const forward_list<T, U>& b)noexcept
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
			if (i == a.end() && j != b.end())
				return -1
			else if (i != a.end() && j == b.end())
				return 1;
			return 0;
		}
	}
	template< typename T, typename Alloc >
	bool operator==(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return compare(lhs,rhs) == 0;
	}
	template< typename T, typename Alloc >
	bool operator!=(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) != 0;
	}
	template< typename T, typename Alloc >
	bool operator<(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) < 0;
	}
	template< typename T, typename Alloc >
	bool operator<=(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) <= 0;
	}
	template< typename T, typename Alloc >
	bool operator>(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) > 0;
	}
	template< typename T, typename Alloc >
	bool operator>=(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs)noexcept
	{
		return compare(lhs, rhs) >= 0;
	}

	template< typename T, typename Alloc >
	void swap(forward_list<T, Alloc>& lhs, forward_list<T, Alloc>& rhs) noexcept(lhs.swap(rhs))
	{
		lhs.swap(rhs);
	}
}