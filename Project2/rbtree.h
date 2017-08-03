#pragma once
#include"prepare.h"
#include<functional>
namespace uuz
{
	template<typename T>
	struct rb_tree_node
	{
		rb_tree_node() = default;
		rb_tree_node(T* p,const T& value):data(value){}
		rb_tree_node(T* p,T&& value) :data(std::move(value)) {}
		template<typename...Args>
		rb_tree_node(T* p, Args&& args) : data(std::forward<Args>(args)...) {}
		
		void destroy(const std::function<void(T*)>& p)noexcept
		{
			data.destroy();
			delete this;
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

		storage<T> data;
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
				root = nullptr;
				nul.father = nul.left = nul.right = nullptr;
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
				root = nullptr;
				nul.father = nul.left = nul.right = nullptr;
				throw;
			}
			nul.left = nul.right = root;
			root->father = &nul;
			ssize = t.ssize;
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
			k.color = true;
			auto b = find(k->data.get());
			if (!b)
			{
				root = k;
				k.color = false;
				k->father=k->left = &nul;
				nul.father = nul.left = nul.right = k;
			}
			else
			{
				if (!compare()(k->data.get(), b->data.get()) && !compare()(b->data.get(), k->data.get()))
					return b;
				else 
				{
					k->father = b;
					if (compare()(k->data.get(), b->data.get()))
					{
						b->left = k;
						if (b->left == &nul)
						{
							k->left = &nul;
							nul.father = k;
						}
					}
					else
						b->right = k;
					fixinsert(k);
				}
			}
			return k;
		}

		node* copy(node* a,node* n)
		{
			auto b = make(a->data.get());
			b->color = a->color;
			if (a->left == n )
			{
				b->left = &nul;
				nul->father = &b;
			}
			else
			{
				try
				{
					if (a->left)
						b->left = copy(a->left);
					if (a->right)
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
			}
			return b;

		}

		void dele(node* p)
		{
			node* y = p;
			if (isnul(p->left) && isnul(p->right))
			{
				if (p->left == &nul)
				{
					if (p == root)
					{
						root = nullptr;
						nul.left = nul.father = nul.right = nullptr;
					}
					else
					{
						nul->father = p->father;
						nul->father->left = &nul;
					}
				}
				else
				{
					if (p->father->left == p)
						p->father->left = nullptr;
					else 
						p->father->right == nullptr;
				}
			}
			else if (isnul(p->left) || isnul(p->right) && !(isnul(p->left) && isnul(p->right)))
			{
				if (isnul(p->left))
				{
					if (p->left == &nul)
					{
						if (p == root)
						{
							root = nullptr;
							nul.left = nul.father = nul.right = nullptr;
						}
						else
						{
							nul.father = p->father;
							nul.father->left = &nul;
						}
					}
					else
					{
						p->left->father = p->father;
						if (p->father->left == p)
							p->father->left = p->left;
						else 
							p->father->right = p->left;
					}
				}
				else
				{
					if (p == root)
					{
						root = nullptr;
						nul.left = nul.father = nul.right = nullptr;
					}
					else
					{
						p->right->father = p->father;
						if (p->father->left == p)
							p->father->left = p->right;
						else 
							p->father->right = p->right;
					}
				}
			}
			else
			{
				y = successor(p);
				auto k = y->right;
				k->father = y->father;
				if (y == y->father->left)
					y->father->left = k;
				else
					y->father->right = k;
				using std::swap;
				swap(y->data.get(), p->data.get());
			}
			if(y->color == false)
				fixdel()
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
			swap(ssize, t.ssize);
			if (empty() && !t.empty())
			{
				nul.father->left = &nul;
				nul.father->right = &nul;
				nul.left->father = &nul;
			}
			else if (!empty() && t.empty())
			{
				t.nul.father->left = &t.nul;
				t.nul.father->right = &t.nul;
				t.nul.left->father = &t.nul;
			}
			else if (!empty() && !t.empty())
			{
				swap(t.nul.father->left, nul.father->left);
				swap(t.nul.father->right, nul.father->right);
				swap(t.nul.left->father, nul.left->father);
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
			A all{ alloc };
			return new(alloc.allocate()) node(all.allocate(), std::forward<Args>(args)...);
		}
		
		void destroy(node* t)noexcept
		{
			A all{ alloc };
			t->destroy([&all](T* a) {all.deallocate(a, 1); });
			alloc.deallocate(t, 1);
		}
		
		node* ifind(const T& a, node* b)const noexcept
		{
			if (!compare()(a, b->data.get()) && !compare()(b->data.get(), a))
				return b;
			else if (compare(a, b->data.get()))
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

		node* minimum(node* x)noexcept
		{
			while (!isnul(x->left))
				x = x->left;
			return x;
		}

		node* successor(node* x)noexcept
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