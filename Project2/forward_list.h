#pragma once
#include"prepare.h"
namespace uuz
{
	template<typename T, typename Allocator = uuz::allocator>
	class forward_list_iterator
	{

	};
	template<typename T,typename Allocator = uuz::allocator>
	class forward_list
	{
		using iterator = forward_list_iterator<T, Allocator>;
		using self = forward_list;
	public:
		forward_list() : forward_list(Allocator()) {}
		explicit forward_list(const Allocator& alloc);
		forward_list(size_type count,const T& value,const Allocator& alloc = Allocator());
		explicit forward_list(size_type count, const Allocator& alloc = Allocator());
		template< class InputIt >
		forward_list(InputIt first, InputIt last,const Allocator& alloc = Allocator());
		forward_list(const forward_list& other);
		forward_list(const forward_list& other, const Allocator& alloc);		
		forward_list(forward_list&& other);
		forward_list(forward_list&& other, const Allocator& alloc);
		forward_list(std::initializer_list<T> init,const Allocator& alloc = Allocator());

		forward_list& operator=(const forward_list& other);
		forward_list& operator=(forward_list&& other) noexcept(/* see below */);
		forward_list& operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T& value);
		template< class InputIt >
		void assign(InputIt first, InputIt last);
		void assign(std::initializer_list<T> ilist);
		
		T& front();
		const T& front() const;
		
		
		iterator before_begin() noexcept;
		const iterator before_begin() const noexcept;
		const iterator cbefore_begin() const noexcept;

		iterator begin() noexcept;
		const iterator begin() const noexcept; 
		const iterator cbegin() const noexcept;

		iterator end() noexcept;
		const iterator end() const noexcept;
		const iterator cend() const noexcept;
	
		bool empty() const noexcept;

		size_type max_size() const noexcept;

		void clear() noexcept;

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

		void push_front(const T& value);
		void push_front(T&& value);
	
		template< class... Args >
		reference emplace_front(Args&&... args);

		void pop_front();

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
		void swap(forward_list<T, Alloc>& lhs,
			forward_list<T, Alloc>& rhs) noexcept(lhs.swap(rhs))
		{

		}

		~forward_list()
		{
			clear();
		}
	private:


	};
}