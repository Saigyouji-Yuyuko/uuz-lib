#pragma once
#include<iostream>
#include"prepare.h"
namespace uuz
{
	/*template<typename T, typename compare >
	class rb_tree;

	template<typename T, typename compare>
	class rb_iterator;

	template<typename T, typename compare >//compare可否及支持仿函数，lamda，函数指针？
	class rb_tree_node
	{
	private:

		using this_type = rb_tree_node<T, compare>;
		using this_type_p = rb_tree_node<T, compare>*;
		enum
		{
			red = false,
			black = true
		};
		using com = pre_less<T, compare>;
		friend rb_tree<T, compare>;
		friend rb_iterator<T, compare>;

	public:

		using value_type = T;

		this_type(const T& w, const int c)noexcept :what(w), color(c) {}
		this_type(T&& w, const int c)noexcept : what(std::move(w)), color(c) {}

		this_type(const this_type&) = delete;
		this_type(this_type&&) = delete;
		this_type& operator=(const this_type&) = delete;
		this_type& operator=(this_type&&) = delete;
		
		friend std::ostream& operator<<(std::ostream& os, const this_type& it)noexcept(noexcept(os << (it.what)))
		{
			os << (it.what);
			return os;
		}

	private:
		explicit this_type() noexcept : color(black) {}
		~rb_tree_node()noexcept
		{
			//std::cout << what;
			if (left != null)
				delete left;
			if (right != null)
				delete right;
		}

		static this_type_p nul()noexcept
		{
			static this_type lll;
			return &lll;
		}
		static this_type_p null;

		this_type_p minnmum()noexcept
		{
			if (left == null)
				return this;
			else return left->minnmum();
		}
		this_type_p maxnmun() noexcept
		{
			if (right == null)
				return this;
			else return right->maxnmun();
		}

		this_type_p insert(this_type_p temp)noexcept(noexcept(com()(temp->what, what)))
		{
			if (!com()((temp->what), what) && !com()(what, (temp->what)))
				return null;
			if (com()((temp->what), what))
			{
				if (left == null)
				{
					temp->father = this;
					left = temp;
					return temp;
				}
				return left->insert(temp);
			}
			else
			{
				if (right == null)
				{
					temp->father = this;
					right = temp;
					return temp;
				}
				return right->insert(temp);
			}
		}
		template<typename U>
		this_type_p find(const U& temp)const noexcept(noexcept(com()(temp, what)))
		{
			if (!com()(temp, what) && !com()(what, temp))
				return this;
			else if (com()(temp, what))
			{
				if (this->left == null)
					return null;
				else
					return left->find(temp);
			}
			else
			{
				if (this->right == null)
					return null;
				else
					return right->find(temp);
			}
		}
		
		T what;
		bool color = black;
		this_type_p father = null;
		this_type_p left = null;
		this_type_p right = null;
	};

	template<typename T, typename compare>
	rb_tree_node<T, compare>*  rb_tree_node<T, compare>::null = rb_tree_node<T, compare>::nul();

	template<typename T, typename compare>
	class rb_iterator
	{
	private:
		
		using node_p = rb_tree_node<T, compare>*;
		using node = rb_tree_node<T, compare>;
		using self = rb_iterator<T, compare>;
		using self_p = rb_iterator<T, compare>*;
		friend rb_tree<T, compare>;

	public:

		using value_type = T;

		~rb_iterator() = default;
		explicit rb_iterator(const node_p t)noexcept :what(t) {}

		 T& operator*()noexcept { return what->what; }
		 const T& operator*() const noexcept { return what->what; }
		 T* operator->()& noexcept { return &(what->what); }
		 const T* operator->() const & noexcept { return &(what->what); }

		self& operator++()noexcept { increase(); return *this; }
		self operator++(int)noexcept { node_p t = this->what; increase(); return self(t); }
		self& operator--()noexcept { decrease(); return *this; }
		self operator--(int)noexcept { node_p t = this->what; decrease(); return self(t); }

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.what == b.what;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return !(a == b);
		}

		bool isnull() const noexcept { return what == node::null; }

	private:

		rb_iterator() = default;
		
		void increase()noexcept
		{
			if (what->right != node::null)
			{
				what = what->right->minnmum();
			}
			else
			{
				auto p = what->father;
				while (p != node::null&&p->right == what)
				{
					what = p;
					p = p->father;
				}
				if (what->right != p)
					what = p;
			}
			//cout << *what;
		}
		void decrease()noexcept
		{
			if (what->left != node::null)
			{
				auto p = what->left;
				while (p->right != node::null)
					p = p->right;
				what = p;
			}
			else
			{
				auto p = what->father;
				while (p != node::null&&p->father->right != p)
					p = p->father;
				what = p;
			}
		}

		node_p  what = node::null;
	};

	template<typename T, typename compare = nil>
	class rb_tree
	{
	private:

		using node_p = rb_tree_node<T, compare>*;
		using node = rb_tree_node<T, compare>;
		using self = rb_tree<T, compare>;
		friend node;

	public:

		using value_type = T;
		using iterator = rb_iterator<T, compare>;
		
		rb_tree() = default;
		~rb_tree()noexcept
		{
			clear();
		}
	
		template<typename U>
		iterator find(const U& temp)const noexcept(noexcept(root->find(temp)))
		{
			if (root != node::null)
				return iterator(root->find(temp));
			return iterator();
		}
		
		void insert(T&& temp)
		{
			if (root != node::null)
			{
				node_p use = root->insert(new node(std:move(temp), node::red));
				if (use != node::null)
				{
					fixup_insert(use);
					++s;
				}
			}
			else
			{
				root = new node(std::move(temp), node::black);
				++s;
			}
			//blackhigh();
		}
		void insert(const T& temp)
		{
			if (root != node::null)
			{
				node_p use = root->insert(new node(temp, node::red));
				if (use != node::null)
				{
					fixup_insert(use);
					++s;
				}
			}
			else
			{
				root = new node(temp, node::black);
				++s;
			}
		}

		void erase(node_p p)noexcept
		{
			if (p == node::null || root == node::null)
				return;
			help_eraser(p);
			//blackhigh();
			delete p;
			--s;
			//cout << endl;
		}
		void erase(const iterator& p)noexcept
		{
			return	erase(p.what);
		}
		template<typename U>
		void erase(const U& p)noexcept(noexcept(find(p)))
		{
			return erase(find(p));
		}

		bool empty() const noexcept
		{
			return s == 0;
		}
		void clear()noexcept(noexcept(delete root))
		{
			if (root != node::null)
				delete root;
			root = node::null;
			s = 0;
		}	
		unsigned int size()const noexcept
		{
			return s;
		}

		iterator begin()noexcept { return iterator(root->minnmum()); }
		const iterator begin() const noexcept { return const_cast<self*>(this)->begin(); }
		const iterator cbegin() const noexcept { return const_cast<self*>(this)->begin(); }
		iterator end()noexcept { return iterator(); }
		const iterator end() const noexcept { return iterator(); }
		const iterator cend() const noexcept { return iterator(); }

	private:

		void lrotate(node_p x)noexcept
		{
			auto y = x->right;
			x->right = y->left;
			if (y->left != node::null)
				y->left->father = x;
			y->father = x->father;
			if (x->father == node::null)
				root = y;
			else if (x == x->father->left)
				x->father->left = y;
			else
				x->father->right = y;
			y->left = x;
			x->father = y;
		}
		void rrotate(node_p y)noexcept
		{
			auto x = y->left;
			y->left = x->right;
			if (x->right != node::null)
				x->right->father = y;
			x->father = y->father;
			if (y->father == node::null)
				root = x;
			else if (y->father->right == y)
				y->father->right = x;
			else
				y->father->left = x;
			x->right = y;
			y->father = x;
		}

		void help_eraser(node_p)noexcept;
		void translate(node_p, node_p)noexcept;
		void fixup_insert(node_p)noexcept;
		void fixup_delete(node_p)noexcept;

		unsigned int s = 0;
		node_p root = node::null;
	};

	template<typename T, typename compare >
	inline void rb_tree<T, compare>::help_eraser(node_p z)noexcept
	{
		node_p y = z;
		auto color = y->color;
		node_p x;
		if (z->left == node::null)
		{
			x = z->right;
			translate(z, z->right);
		}
		else if (z->right == node::null)
		{
			x = z->left;
			translate(z, z->left);
		}
		else
		{
			y = z->right->minnmum();
			color = y->color;
			x = y->right;
			if (y->father == z)
				x->father = y;
			else
			{
				translate(y, y->right);
				y->right = z->right;
				y->right->father = y;
			}
			translate(z, y);
			y->left = z->left;
			y->left->father = y;
			y->color = z->color;
		}
		//std::cout << *y << std::endl;
		if (color == node::black)
			fixup_delete(x);
		z->left = node::null;
		z->right = node::null;

	}

	template<typename T, typename compare >
	inline void rb_tree<T, compare>::translate(node_p a, node_p  b)noexcept
	{
		if (a->father == node::null)
			root = b;
		else if (a->father->left == a)
			a->father->left = b;
		else
			a->father->right = b;
		b->father = a->father;
	}

	template<typename T, typename compare >
	inline void rb_tree<T, compare>::fixup_insert(node_p  p)noexcept
	{
		if (p == root)
		{
			root->color = node::black;
			return;
		}
		if (p->father->color == node::red)
		{
			if (p->father == p->father->father->left)
			{
				auto uncle = p->father->father->right;
				if (uncle->color == node::red)
				{
					p->father->color = node::black;
					uncle->color = node::black;
					p->father->father->color = node::red;
					return fixup_insert(uncle->father);
				}
				else
				{
					if (p == p->father->right)
					{
						p = p->father;
						lrotate(p);
					}
					p->father->color = node::black;
					p->father->father->color = node::red;
					rrotate(p->father->father);
					return;
				}
			}
			else
			{
				auto uncle = p->father->father->left;
				if (uncle->color == node::red)
				{
					p->father->color = node::black;
					uncle->color = node::black;
					p->father->father->color = node::red;
					return fixup_insert(uncle->father);
				}
				else
				{
					if (p == p->father->left)
					{
						p = p->father;
						rrotate(p);
					}
					p->father->color = node::black;
					p->father->father->color = node::red;
					lrotate(p->father->father);
					return;
				}
			}
		}
		else return;
	}

	template<typename T, typename compare >
	inline void rb_tree<T, compare>::fixup_delete(node_p x)noexcept
	{
		while (x != root && x->color == node::black)
		{
			if (x == x->father->left)
			{
				auto w = x->father->right;
				if (w != node::null&&w->color == node::red)
				{
					w->color = node::black;
					x->father->color = node::red;
					lrotate(x->father);
					w = x->father->right;
				}
				if (w->left->color == node::black&& w->right->color == node::black)
				{
					w->color = node::red;
					x = x->father;
				}
				else
				{
					if (w->right->color == node::black)
					{
						w->left->color = node::black;
						w->color = node::red;
						rrotate(w);
						w = x->father->right;
					}
					w->color = x->father->color;
					x->father->color = node::black;
					w->right->color = node::black;
					lrotate(x->father);
					x = root;
				}
			}
			else
			{
				auto w = x->father->left;
				if (w->color == node::red)
				{
					w->color = node::black;
					x->father->color = node::red;
					rrotate(x->father);
					w = x->father->left;
				}
				if (w->left->color == node::black&&w->right->color == node::black)
				{
					w->color = node::red;
					x = x->father;
				}
				else
				{
					if (w->left->color == node::black)
					{
						w->right->color = node::black;
						w->color = node::red;
						lrotate(w);
						w = x->father->left;
					}
					w->color = x->father->color;
					x->father->color = node::black;
					w->left->color = node::black;
					rrotate(x->father);
					x = root;
				}
			}
		}
		x->color = node::black;
	}*/
	template<typename T, typename compare>
	class rb_tree_iterator;
	template<typename T,typename compare>
	class rb_tree
	{
		using node = rb_tree<T, compare>;
		using self = rb_tree<T, compare>;
		using value_type = T;
		using iterator = rb_tree_iterator<T, compare>;
		enum { red = 0, black = 1 };
	public:
		self() = default;
		explicit self(const T& p):what(p){}
		explicit self(T&& p):what(move(p)){}
		self(const self& p)
		{
			if (p.left)
			{
				left = new self(*(p.left));
				left->parent = this;
			}			
			if (p.right)
			{
				right = new self(*(p.right));
				right->parent = this;
			}
			color = p.color;
			what = p.what;
		}
		self(self&& p)
		{
			if (p.left)
			{
				left = p.left;
				left->parent = this;
				p.left = nullptr;
			}
			if (p.right)
			{
				right = p.right;
				right->parent = this;
				p.right = nullptr;
			}
			color = move(p.color);
			what = move(p.what);
		}

		self& operator=(const self& p)
		{
			if (this == &p)
				return *this;
			auto temp{ p };
			*this = move(temp);
			return *this;
		}
		self& operator=(self&& p)
		{
			if (this == &p)
				return *this;
			auto t1 = left;
			auto t2 = right;
			if (p.left)
			{
				left = p.left;
				left->parent = this;
				p.left = nullptr;
			}
			if (p.right)
			{
				right = p.right;
				right->parent = this;
				p.right = nullptr;
			}
			color = move(p.color);
			what = move(p.what);
			left->clear();
			right->clear();
			return *this;
		}


		void clear()noexcept
		{
			if (left)
				delete left;
			if (right)
				delete right;
		}
		void swap(self& p )noexcept
		{
			using std::swap;
			swap(p.parent, parent);
			swap(p.left, left);
			swap(p.right, right);
			swap(color, p.color);
			swap(what, p.what);
		}

		~rb_tree()
		{
			clear();
		}
	private:
		node* parent = nullptr;
		node* left = nullptr;
		node* right = nullptr;
		bool color = black;
		T what;
	};
}
