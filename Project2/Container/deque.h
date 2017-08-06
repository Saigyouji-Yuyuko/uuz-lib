#pragma once
#include"container.h"
namespace uuz
{
	template<typename T,typename A>
	class deque;
	template<typename T>
	class deque_node
	{

	};
	template<typename T,typename A>
	class deque_iterator
	{

	};

	template<typename T, typename A = uuz::allocator<T>>
	class deque
	{
        using Allocator = uuz::exchange<A, deque_node<T>>::type;
		using iterator = deque_iterator;
		using node = deque_node;
	public:
		deque() : deque(Allocator()) {}
		explicit deque(const Allocator& alloc);
		explicit deque(size_type count,const T& value = T(),const Allocator& alloc = Allocator());
		deque(size_type count,const T& value,const Allocator& alloc = Allocator());
		explicit deque(size_type count, const Allocator& alloc = Allocator());
		template< class InputIt >
		deque(InputIt first, InputIt last,const Allocator& alloc = Allocator());
		deque(const deque& other);
		deque(const deque& other, const Allocator& alloc);
		deque(deque&& other);
		deque(deque&& other, const Allocator& alloc);
		deque(std::initializer_list<T> init,const Allocator& alloc = Allocator());

		deque& operator=(const deque& other);		
		deque& operator=(deque&& other) noexcept(/* see below */);
		deque& operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T& value);
		template< class InputIt >
		void assign(InputIt first, InputIt last);
		void assign(std::initializer_list<T> ilist);

		Allocator get_allocator() const;

		T& at(size_t pos);
		const T& at(size_t pos) const;

		T& operator[](size_type pos);
		const T& operator[](size_type pos) const;

		T& front();
		const T& front() const;

		T& back();
		const T& back() const;

		iterator begin() noexcept;
		const iterator begin() const noexcept;	
		const iterator cbegin() const noexcept;

		iterator end() noexcept;
		const iterator end() const noexcept;
		const iterator cend() const noexcept;

		bool empty() const noexcept;

		size_t size() const noexcept;

		size_t max_size() const noexcept;

		void shrink_to_fit();

		void clear() noexcept;

		iterator insert(const_iterator pos, const T& value);
		iterator insert(const_iterator pos, T&& value);
		iterator insert(const_iterator pos, size_type count, const T& value);
		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last);
		iterator insert(const_iterator pos, std::initializer_list<T> ilist);

		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		void push_back(const T& value);
		void push_back(T&& value);

		template< class... Args >
		T& emplace_back(Args&&... args);

		void pop_back();

		void push_front(const T& value);
		void push_front(T&& value);

		template< class... Args >
		T& emplace_front(Args&&... args);

		void pop_front();

		void resize(size_type count);
		void resize(size_type count, const value_type& value);

		void swap(deque& other) noexcept//();
		{

		}

		~deque()noexcept
		{
			clear();
		}
	private:
		Allocator alloc;





	};
	template< class T, class Alloc >
	bool operator==(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator!=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator<(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator<=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator>(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);
	template< class T, class Alloc >
	bool operator>=(const deque<T, Alloc>& lhs,const deque<T, Alloc>& rhs);

	template< class T, class Alloc >
	void swap(deque<T, Alloc>& lhs,deque<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

}