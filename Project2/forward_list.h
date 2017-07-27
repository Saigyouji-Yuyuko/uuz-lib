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
		forward_list_node(Args...args)dat(new T(std::forward<Args>(args)...)) {}
		void destory()noexcept
		{
			if (next)
				next->destory();
			delete this;
			return;
		}
		T* dat = nullptr;
		forward_list_node* next = nullptr;
	};

	template<typename T, typename Allocator = uuz::allocator>
	class forward_list_iterator
	{

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
		forward_list(size_t count, const T& value/*,const Allocator& alloc = Allocator()*/)
		{
			if (count)
			{
				auto k = new node(value);
				nul->next = k;
				for (auto i = 1; i < count; ++i)
				{
					auto temp = new node(value);
					k->next = temp;
					k = temp;
				}
				k->next = &nul;
				last = k;
			}
		}
		explicit forward_list(size_t count/*,const Allocator& alloc = Allocator()*/):forward_list(count, T{}) {}
		template<typename InputIt, typename = decltype(*(std::declval(InputIt)))>
		forward_list(const InputIt& first, const InputIt& last/*,const Allocator& alloc = Allocator()*/)
		{
			if (first != last)
			{
				auto k = new node(*first);
				nul->next = k;
				auto i = first;
				++i;
				for (; i !=last; ++i)
				{
					auto temp = new node(*i);
					k->next = temp;
					k = temp;
				}
				k->next = &nul;
				last = k;
			}
		}
		forward_list(const forward_list& other):forward_list(other.begin(),other.end()){}
		//forward_list(const forward_list& other/*,const Allocator& alloc = Allocator()*/));
		forward_list(forward_list&& other):list{}
		{
			this->swap(other);
		}
		//forward_list(forward_list&& other, const Allocator& alloc);
		forward_list(std::initializer_list<T> init/*,const Allocator& alloc = Allocator()*/) :forward_list{init.begin(),init.end()}{}

		forward_list& operator=(const forward_list& other)
		{
			if (*this == other)
				return *this;
			auto temp(other);
			this->swap(temp);
			return *this;
		}
		forward_list& operator=(forward_list&& other) noexcept(/* see below */)
		{
			if (*this == other)
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
		template<typename InputIt, typename = decltype(*(std::declval(InputIt)))>
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
			return *(nul->next->dat);
		}
		const T& front() const noexcept
		{
			return *(nul->next->dat);
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
			return iterator{ nul->next };
		}
		const iterator begin() const noexcept
		{
			return iterator{ nul -> next };
		}
		const iterator cbegin() const noexcept
		{
			return iterator{ nul - next };
		}

		iterator end() noexcept
		{
			return iterator{ last };
		}
		const iterator end() const noexcept
		{
			return iterator{ last };
		}
		const iterator cend() const noexcept
		{
			return iterator{ last };
		}
	
		bool empty() const noexcept
		{
			return nul->next == &nul;
		}

		//size_t max_size() const noexcept;

		void clear() noexcept
		{
			last->next = nullptr;
			last = nullptr;
			nul->next->destory();
			nul->next = &nul;
		}

		iterator insert_after(const_iterator pos, const T& value);
		iterator insert_after(const_iterator pos, T&& value);
		iterator insert_after(const_iterator pos, size_type count, const T& value);
		template< class InputIt >
		iterator insert_after(const_iterator pos, InputIt first, InputIt last);
		iterator insert_after(const_iterator pos, std::initializer_list<T> ilist);
		
		template< class... Args >
		iterator emplace_after(const_iterator pos, Args&&... args);
		iterator erase_after(const_iterator pos);
		iterator erase_after(const_iterator first, const_iterator last);

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
			auto k = new node(std::forward<Args>(args)...);
			k->next = nul->next;
			nul->next = 
		}

		void pop_front()
		{
			auto k = nul->next;
			nul->next = nul->next->next;
			if (!empty())
				delete k;
		}

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(forward_list& other) noexcept(/* see below */);

		void merge(forward_list& other);
		void merge(forward_list&& other);
		template <class Compare>
		void merge(forward_list& other, Compare comp);
			template <class Compare>
		void merge(forward_list&& other, Compare comp);

		void splice_after(const_iterator pos, forward_list& other);
		void splice_after(const_iterator pos, forward_list&& other);
		void splice_after(const_iterator pos, forward_list& other,
			const_iterator it);
		void splice_after(const_iterator pos, forward_list&& other,
			const_iterator it);

		void splice_after(const_iterator pos, forward_list& other,
			const_iterator first, const_iterator last);
		void splice_after(const_iterator pos, forward_list&& other,
			const_iterator first, const_iterator last);

		void remove(const T& value);
		template< class UnaryPredicate >
		void remove_if(UnaryPredicate p);

		void reverse() noexcept;

		void unique();
		template< class BinaryPredicate >
		void unique(BinaryPredicate p);

		void sort();
		template< class Compare >
		void sort(Compare comp);

		template< class T, class Alloc >
		bool operator==(const forward_list<T, Alloc>& lhs,
			const forward_list<T, Alloc>& rhs);
			template< class T, class Alloc >
		bool operator!=(const forward_list<T, Alloc>& lhs,
			const forward_list<T, Alloc>& rhs);
			template< class T, class Alloc >
		bool operator<(const forward_list<T, Alloc>& lhs,
			const forward_list<T, Alloc>& rhs);
			template< class T, class Alloc >
		bool operator<=(const forward_list<T, Alloc>& lhs,
			const forward_list<T, Alloc>& rhs);
			template< class T, class Alloc >
		bool operator>(const forward_list<T, Alloc>& lhs,
			const forward_list<T, Alloc>& rhs);
			template< class T, class Alloc >
		bool operator>=(const forward_list<T, Alloc>& lhs,
			const forward_list<T, Alloc>& rhs);

		template< class T, class Alloc >
		void swap(forward_list<T, Alloc>& lhs,forward_list<T, Alloc>& rhs) noexcept(lhs.swap(rhs))
		{
			lhs.swap(rhs);
		}

		~forward_list()
		{
			clear();
		}
	private:
		node nul;
		node* last=nullptr;
	};
}