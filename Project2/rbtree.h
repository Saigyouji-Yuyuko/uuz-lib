#pragma once
#include"prepare.h"
#include<functional>
namespace uuz
{
	template<typename T>
	struct rb_tree_node
	{
		rb_tree_node() = default;
		rb_tree_node(T* p,const T& value):data(new(p) T(value)){}
		rb_tree_node(T* p,T&& value) :data(new(p) T(std::move(value))) {}
		template<typename...Args>
		rb_tree_node(T* p, Args&& args) : data(new(p) T(std::forward<Args>(args)...)) {}
		void destroy(const std::function<void(T*)>& p)noexcept
		{
			(*data).~T();
			p(data);
			delete this;
			return;
		}

		T* data = nullptr;
		bool color = false;
		rb_tree_node* father = nullptr;
		rb_tree_node* left = nullptr;
		rb_tree_node* right = nullptr;
	};
	template<typename T, typename compare, typename A>
	class rb_tree_iterator
	{

	};
	template<typename T, typename compare=uuz::pre_less<T,nil>, typename A=uuz::allocator<T>>
	class rb_tree
	{
		using node = rb_tree_node<T>;
		using iterator = rb_tree_iterator<T, compare, A>;
		using Allocator = uuz::exchange<A, node>::type;
	public:
			
	private:
		template<typename... Args>
		node* make(Args&&...args)
		{
			A all{ alloc };
			return new(alloc.allocate()) node(all.allocate(), std::forward<Args>(args)...);
		}
		void destroy(node* t)noexcept
		{
			A all{ alloc };
			t->destroy([&all](T* a) {all.deallocate(a, 1); });
			alloc.deallocate(t, 1);
		}

		Allocator alloc;
		node* root;
		node nul;
	};
}