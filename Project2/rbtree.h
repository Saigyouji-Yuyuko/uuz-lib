#pragma once
#include"prepare.h"
#include<functional>
namespace uuz
{
	template<typename T>
	struct rb_tree_node
	{
		rb_tree_node() = default;
		rb_tree_node(const T& value):data(value){}
		rb_tree_node(T&& value) :data(std::move(value)) {}
		template<typename...Args>
		rb_tree_node( Args&&... args) : data(std::forward<Args>(args)...) {}
		
		void destroy()noexcept
		{
			data.data.destroy();
			this->~rb_tree_node();
			return;
		}

		rb_tree_node* grandfather()const noexcept
		{
			return father->father;
		}

		rb_tree_node* brother()const noexcept
		{
			if (father->left == this)
				return father->right;
			return father->left;
		}

		rb_tree_node* uncle()const noexcept
		{
			return father->brother();
		}

		T& get()noexcept
		{
			return data.data.get();
		}

		union dat
		{
			dat() = default;
			dat(const T& p):data(p){}
			dat(T&& p):data(std::move(p)){}
			template<typename...Args>
			dat(Args&&...args) : data(std::forward<Args>(args)...) {}
			storage<T> data;
			rb_tree_node* end = nullptr;
		} data;
		
		bool color = false;
		rb_tree_node* father = nullptr;
		rb_tree_node* left = nullptr;
		rb_tree_node* right = nullptr;
	};

	template<typename T, typename compare, typename A>
	class rb_tree;

	template<typename T, typename compare, typename A>
	class rb_tree_iterator
	{
		using self = rb_tree_iterator;
		friend rb_tree<T, compare, A>;
	public:
		self& operator++() noexcept
		{
			increase();
			return *this;
		}
		self operator++(int)noexcept
		{
			auto p{ t };
			increase();
			return p;
		}
		self& operator--()noexcept
		{
			decrease();
			return *this;
		}
		self operator--(int) noexcept
		{
			auto p{ t };
			decrease();
			return p;
		}

		T& operator*()noexcept
		{
			return t->get();
		}
		const T& operator*()const noexcept
		{
			return t->get();
		}
		T* operator->()noexcept
		{
			return &(t->get());
		}
		const T* operator->()const noexcept
		{
			return &(t->get());
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
		void increase()noexcept
		{
			if (!isnul(t->right))
			{
				t = t->right;
				while (!isnul(t->left))
					t = t->left;
				return;
			}
			auto y = t->father;
			while (!isnul(y) && t == y->right)
			{
				t = y;
				y = y->father;
			}
			t = y;
		}
		void decrease()noexcept
		{
			if (!isnul(t->left))
			{
				t = t->left;
				while (!isnul(t->right))
					t = t->right;
				return;
			}
			auto y = t->father;
			while (!isnul(y) && t == y->left)
			{
				t = y;
				y = y->father;
			}
			t = y;
		}

		bool isnul(rb_tree_node<T>* t)const noexcept
		{
			return t == nullptr || (t->left == t->right && t->left != nullptr);
		}
		self(rb_tree_node<T>* tt)noexcept : t(tt) {}
		self(const rb_tree_node<T>* tt)noexcept : t(const_cast<rb_tree_node<T>*>(tt)) {}
		rb_tree_node<T>* t = nullptr;
	};

	template<typename T, typename compare=uuz::pre_less<T,nil>, typename A=uuz::allocator<T>>
	class rb_tree
	{
		using node = rb_tree_node<T>;
		using iterator = rb_tree_iterator<T, compare, A>;
		using Allocator = typename uuz::exchange<A, node>::type;
	public:
		rb_tree()noexcept(std::is_nothrow_default_constructible_v<Allocator>) = default;
		rb_tree(const A& a):alloc(a){}
		rb_tree(rb_tree&& t)noexcept:rb_tree()
		{
			this->swap(t);
		}
		rb_tree(rb_tree&& t,const A& a)noexcept:rb_tree(a)
		{
			if(alloc==t.alloc)
				this->swap(t);
			else
			{

			}
		}
		rb_tree(const rb_tree& t):rb_tree()
		{
			try
			{
				if(!t.empty())
					root = copy(t.root,&t.nul);
			}
			catch (...)
			{
				destroy(root);
				root = nul.data.end = nul.father = nul.left = nul.right = nullptr;
				throw;
			}
			nul.left = nul.right = root;
			root->father = &nul;
			ssize = t.ssize;
		}
		rb_tree(const rb_tree& t, const A& a) :rb_tree(a)
		{
			try
			{
				if (!t.empty())
					root = copy(t.root, &t.nul);
			}
			catch (...)
			{
				destroy(root);
				root = nul.data.end = nul.father = nul.left = nul.right = nullptr;
				throw;
			}
			nul.left = nul.right = root;
			root->father = &nul;
			ssize = t.ssize;
		}

		rb_tree& operator=(const rb_tree& t)
		{
			auto temp{ t };
			this->swap(t);
			return *this;
		}
		rb_tree& operator=(rb_tree&& t)
		{
			auto temp{ std::move(t) };
			this->swap(t);
			return *this;
		}

		node* find(const T& a)const noexcept
		{
			if (empty())
				return nullptr;
			return ifind(a,root);
		}

		template<typename...Args>
		node* emplace(Args&&... args)
		{
			auto k = make(std::forward<Args>(args)...);
			k->color = true;
			auto b = find(k->get());
			if (!b)
			{
				root = k;
				k->color = false;
				k->father=k->left=k->right = &nul;
				nul.data.end=nul.father = nul.left = nul.right = k;
			}
			else
			{
				if (!compare()(k->get(), b->get()) && !compare()(b->get(), k->get()))
					return b;
				else 
				{
					k->father = b;
					if (compare()(k->get(), b->get()))
					{
						if (b->left == &nul)
						{
							k->left = &nul;
							nul.father = k;
						}
						b->left = k;
					}
					else
					{
						if (b->right == &nul)
						{
							k->right = &nul;
							nul.data.end = k;
						}
						b->right = k;
					}
						
					fixinsert(k);
				}
			}
			++ssize;
			return k;
		}

		node* copy(node* a,node* n)
		{
			auto b = make(a->get());
			b->color = a->color;
			if (a->left == n )
			{
				b->left = &nul;
				nul->father = b;
			}
			else if (a->right)
			{
				b->right = &nul;
				nul.data.end = b;
			}
			try
			{
				if (a->left && a->left !=n)
					b->left = copy(a->left);
				if (a->right && a->right !=n)
					b->right = copy(a->right);
			}
			catch (...)
			{
				if (b->left)
					destroy(b->left);
				if (b->right)
					destroy(b->right);
				throw;
			}		
			return b;
		}

		iterator begin()noexcept
		{
			return iterator(nul.father);
		}
		const iterator begin()const noexcept
		{
			return iterator(nul.father);
		}

		const iterator end()const noexcept
		{
			return iterator(&nul);
		}
		iterator end()noexcept
		{
			return iterator(&nul);
		}

		void dele(node* x)
		{
			node* child = nullptr;
			node* p = nullptr;
			if (!isnul(x->left) && !isnul(x->right))
			{
				auto replace = successor(x);
				if (x != root)
				{
					if (x->father->left == x)
						x->father->left = replace;
					else
						x->father->right = replace;
				}
				else
				{
					root = replace;
					nul.left = nul.right = replace;
				}
				child = replace->right;
				p = replace->father;
				auto c = replace->color;
				if (p == x)
					p = replace;
				else
				{
					if (!isnul(child))
						child->father = p;
					p->left = child;
					replace->right = x->right;
					x->right->father = replace;
				}
				replace->father = x->father;
				replace->color = x->color;
				replace->left = x->left;
				x->left->father = replace;
				if (!c)
					fixdele(child, p);
				destroy(x);
				return;
			}
			if (!isnul(x->left))
				child = x->left;
			else if(!isnul(x->right))
				child = x->right;
			p = x->father;
			if (!isnul(child))
				child->father = p;
			if (!isnul(p))
			{
				if (p->left == x)
					p->left = child;
				else
					p->right = child;
			}
			else
			{
				root = child;
				nul.left = nul.right = child;
			}
			if (!x->color)
				fixdele(child, p);
			if (x->left == &nul && x->right == &nul)
				nul.father = nul.left = nul.right = nul.data.end = root = nullptr;
			else if (x->left == &nul)
			{
				auto k = minimum(p);
				k->left = &nul;
				nul.father = k;
			}
			else if (x->right == &nul)
			{
				auto k = maximum(p);
				k->right = &nul;
				nul.data.end = k;
			}
			destroy(x);
		}
			
		void clear()noexcept
		{	
			tree_destroy(root);
			root = nul.father = nul.left = nul.right = nullptr;
			ssize = 0;
		}

		size_t size()const noexcept
		{
			return ssize;
		}

		bool empty()const noexcept
		{
			return root == nullptr;
		}

		void swap(rb_tree& t)noexcept
		{
			using std::swap;
			swap(root, t.root);
			swap(nul.father, t.nul.father);
			swap(nul.left, t.nul.left);
			swap(nul.right, t.nul.right);
			swap(nul.data.end, t.nul.data.end);
			swap(ssize, t.ssize);
			if (empty() && !t.empty())
			{
				nul.father->left = &nul;
				nul.left->father = &nul;
				nul.data.end->right = &nul;
			}
			else if (!empty() && t.empty())
			{
				t.nul.father->left = &t.nul;
				t.nul.left->father = &t.nul;
				t.nul.data.end->right = &t.nul;
			}
			else if (!empty() && !t.empty())
			{
				swap(t.nul.father->left, nul.father->left);
				swap(t.nul.left->father, nul.left->father);
				swap(t.nul.data.end->right, nul.data.end->right);
			}
		}

		void print()const noexcept
		{
			auto k = nul.father;
			while (!isnul(k))
			{
				uuz::print(k->get());
				k = successor(k);
			}
			
		}

		~rb_tree()noexcept
		{
			clear();
		}
	private:
		template<typename... Args>
		node* make(Args&&...args)
		{
			return new(alloc.allocate()) node(std::forward<Args>(args)...);
		}
		
		void destroy(node* t)noexcept
		{
			t->destroy();
			alloc.deallocate(t, 1);
		}
		
		node* ifind(const T& a, node* b)const noexcept
		{
			if (!compare()(a, b->get()) && !compare()(b->get(), a))
				return b;
			else if (compare()(a, b->get()))
			{
				if (isnul(b->left))
					return b;
				else return ifind(a, b->left);
			}
			else
			{
				if (isnul(b->right))
					return b;
				else return ifind(a, b->right);
			}
		}

		bool isnul(node* p)const noexcept
		{
			return !p || p == &nul;
		}

		void fixinsert(node* b)noexcept
		{
			if (!b->father->color || b->father == &nul)
				return;
			else if(b->father->color &&(!isnul(b->uncle())&&b->uncle()->color))
			{
				b->father->color = b->uncle()->color = false;
				b->grandfather()->color = true;
				return fixinsert(b->grandfather());
			}
			if (b == b->father->right && b->father == b->grandfather()->left)
			{
				lrotate(b->father);
				b = b->left;
			}
			else if(b == b->father->left && b->father == b->grandfather()->right)
			{
				rrotate(b->father);
				b = b->right;
			}
			b->father->color = false;
			b->grandfather()->color = true;
			if (b == b->father->left && b->father == b->grandfather()->left)
				rrotate(b->grandfather());
			else
				lrotate(b->grandfather());
			return;
		}

		node* minimum(node* x)const noexcept
		{
			while (!isnul(x->left))
				x = x->left;
			return x;
		}
		node* maximum(node* x)const noexcept
		{
			while (!isnul(x->right))
				x = x->right;
			return x;
		}

		node* successor(node* x)const noexcept
		{
			if (!isnul(x->right))
				return minimum(x->right);
			auto y = x->father;
			while (!isnul(y) && x == y->right)
			{
				x = y;
				y = y->father;
			}
			return isnul(y) ? nullptr : y;
		}
		node* predecessor(node* x)const noexcept
		{
			if (!isnul(x->left))
				return maximum(x->left);
			auto y = x->father;
			while (!isnul(y) && x == y->left)
			{
				x = y;
				y = y->father;
			}
			return isnul(y) ? nullptr : y;
		}

		void fixdele(node* x, node* p)noexcept
		{
			while ((isnul(x)|| !x->color) && x != root)
			{
				if (x == p->left)
				{
					auto s = p->right;
					if (s->color == true)
					{
						s->color = false;
						p->color = true;
						lrotate(p);
						s = p->right;
					}
					if ((isnul(s->left) || !s->left->color) && (isnul(s->right) || !s->right->color))
					{
						s->color = true;
						x = p;
						p = p->father;
					}
					else
					{
						if (isnul(s->right) || !s->right->color)
						{
							s->left->color = false;
							s->color = true;
							rrotate(s);
							s = p->right;
						}
						s->color = p->color;
						p->color = false;
						s->right->color = false;
						lrotate(p);
						x = root;
						break;
					}
				}
				else
				{
					auto s = p->left;
					if (s->color == true)
					{
						s->color = false;
						p->color = true;
						rrotate(p);
						s = p->left;
					}
					if ((isnul(s->left) || !s->left->color) && (isnul(s->right) || !s->right->color))
					{
						s->color = true;
						x = p;
						p = p->father;
					}
					else
					{
						if (isnul(s->left) || !s->left->color)
						{
							s->right->color = false;
							s->color = true;
							lrotate(s);
							s = p->left;
						}
						s->color = p->color;
						p->color = false;
						s->left->color = false;
						rrotate(p);
						x = root;
						break;
					}
				}
			}
			if (x)
				x->color = false;
		}

		void tree_destroy(node* k)noexcept
		{
			if (!isnul(k))
			{
				tree_destroy(k->left);
				tree_destroy(k->right);
				destroy(k);
			}
		}

		int blackhigh(node* k)
		{
			if (isnul(k))
				return 1;
			auto l = blackhigh(k->left);
			auto r = blackhigh(k->right);
			if (l != r)
				std::cout<<l<<"  "<<r<<std::endl;
			return k->color ? l : l + 1;
		}

		void rrotate(node* p)noexcept
		{
			auto x = p->left;
			auto c = x->right;
			p->left = c;
			if(c)
				c->father = p;
			x->right = p;
			x->father = p->father;
			p->father = x;
			if (x->father->left == p)
				x->father->left = x;
			if (x->father->right == p)
				x->father->right = x;
			if (x->father == &nul)
				root = x;
		}
		void lrotate(node* p)noexcept
		{
			auto x = p->right;
			auto b = x->left;
			p->right = b;
			if (b)
				b->father = p;
			x->left = p;
			x->father = p->father;
			p->father = x;
			if (x->father->left == p)
				x->father->left = x;
			if (x->father->right == p)
				x->father->right = x;
			if (x->father == &nul)
				root = x;

		}	
		
		size_t ssize = 0;
		Allocator alloc;
		node* root=nullptr;
		node nul;
	};
}