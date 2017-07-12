#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"prepare.h"
#include<initializer_list>
#include<cstring>
#include<cassert>
#include<vector>
#ifdef _DEBUG
#include"debug.h"
#endif
#pragma warning(disable:4996)
namespace uuz
{
	namespace
	{
		template<typename T>
		inline int stoi(const T* p)
		{
			int sign = 1;
			int l = 0;
			if (*p == '-')
			{
				sign = -1;
				++p;
			}
			else if (*p == '+')
				++p;
			while (*p != 0)
			{
				if (*p >= '0'&&*p <= '9')
					l = l * 10 + (*p++) - '0';
				else
					throw("It\'s not a int");
			}
			return l*sign;
		}
		template<typename T>
		inline long long stoll(const T* p)
		{
			int sign = 1;
			long long l = 0;
			if (*p == '-')
			{
				sign = -1;
				++p;
			}
			else if (*p == '+')
				++p;
			while (*p != 0)
			{
				if (*p >= '0'&&*p <= '9')
					l = l * 10 + (*p++) - '0';
				else
					throw("It\'s not a long long");
			}
			return l*sign;
		}
		template<typename T>
		inline float stof(const T* p)
		{
			int sign = 1;
			float d = 0, j = 10;
			bool flag = true;
			if (*p == '-')
			{
				sign = -1;
				++p;
			}
			else if (*p == '+')
				++p;
			while (*p != 0)
			{
				if (*p >= '0'&&*p <= '9')
				{
					if (flag)
						d = d * 10 + (*p++) - '0';
					else
					{
						d += (float)((*p++) - '0') / j;
						j *= 10;
					}
				}
				else if (*p == '.')
				{
					flag = false;
					++p;
				}
				else
					throw("It\'s not a float");
			}
			return d*sign;
		}
		template<typename T>
		inline double stod(const T* p)
		{
			int sign = 1;
			double d = 0, j = 10;
			bool flag = true;
			if (*p == '-')
			{
				sign = -1;
				++p;
			}
			else if (*p == '+')
				++p;
			while (*p != 0)
			{
				if (*p >= '0'&&*p <= '9')
				{
					if (flag)
						d = d * 10 + (*p++) - '0';
					else
					{
						d += (double)((*p++) - '0') / j;
						j *= 10;
					}
				}
				else if (*p == '.')
				{
					flag = false;
					++p;
				}
				else
					throw("It\'s not a double");
			}
			return d*sign;
		}
	}
	constexpr uint32_t sso_size = 32;

	template<typename T>
	class string_base;


	template<typename T>
	class string_base_iterator
	{
		using value_type = T;
		using Chartype = T;
		using self = string_base_iterator;
		using container = string_base<T>;
		friend string_base<T>;
		using size_t = uint32_t;
	public:
		self(const self&) = default;
		self(self&&) = default;
		self& operator=(const self&) = default;
		self& operator=(self&&) = default;
		self& operator+=(const size_t opps)
		{
			//assert(static_cast <size_t>(data) + opps > opps && static_cast<size_t>(data) + opps > static_cast<size_t>(data));
			data += opps;
			return *this;
		}
		self& operator-=(const size_t opps)
		{
			assert(opps <= static_cast <size_t>(data));
			data -= opps;
			return *this;
		}
		Chartype& operator*()noexcept { return *data; }
		const Chartype& operator*()const noexcept { return *data; }
		self& operator++() { *this += 1; return *this; }
		self operator++(int) { auto p{ *this } *this += 1; return p; }
		self& operator--() { *this -= 1; return *this; }
		self operator--(int) { auto p{ *this } *this -= 1; return p; }

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.data == b.data;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return !(a == b);
		}
		friend bool operator<(const self& a, const self& b)noexcept
		{
			return a.data < b.data;
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
		friend self operator+(const self& a, const size_t b)
		{
			self c{ a };
			c += b;
			return c;
		}
		friend size_t operator-(const self& a, const self& b)noexcept
		{
			return a.data - b.data;
		}
		friend self operator-(const self& a, const size_t b)
		{
			self c{ a };
			c -= b;
			return c;
		}
		~string_base_iterator() = default;
	private:
		self(Chartype *p) :data{ p } {  }
		self(const Chartype *p) :data{ const_cast<Chartype*>(p) } { }
		Chartype* data = nullptr;
	};

	template<typename Chartype>
	struct string_base_impl
	{
		using size_t = uint32_t;
		using self = string_base_impl;


		string_base_impl() = default;
		string_base_impl(const self& t)
		{
			if (t.flag)
			{
				reserve(t.size());
				std::copy(t.c_str(), t.c_str() + t.size(), c_str());
				//memcpy(c_str(), t.c_str(), t.size());
				setsize(t.size());
			}
			else
				memcpy(this, &t, sizeof(self));
		}
		string_base_impl(self&& t)
		{
			swap(core, t.core);
			swap(falg, t.flag);
		}
		string_base_impl(const size_t t)
		{
			reserve(t);
		}

		self& operator=(const self& t)
		{
			if (this == &t)
				return *this;
			self dd{ t };
			swap(dd.core, core);
			swap(dd.flag, flag);
		}
		self& operator=(self&& t)
		{
			if (this == &t)
				return;
			auto dd{ std::move(t) };
			swap(dd.core, core);
			swap(dd.flag, flag);
		}

		size_t size()const noexcept
		{
			return flag ? core.more.size : core.less.size;
		}
		size_t max_size()const noexcept
		{
			return flag ? core.more.maxsize : sso_size - 1;
		}
		Chartype* c_str()
		{
			
			return flag ? core.more.data : core.less.data;
		}
		const Chartype* c_str()const
		{

			return flag ? core.more.data : core.less.data;
		}
		void reserve(const size_t t)
		{
			if (t <= max_size())
				return;
			else
			{
				if (flag)
					delete[] core.more.data;
				else
					flag = true;
				core.more.data = new Chartype[t + 1];
				assert(core.more.data);
				setsize(0);
				core.more.maxsize = t;
			}
		}
		void setsize(const size_t t)noexcept
		{
			if (flag)
				core.more.size = t;
			else
				core.less.size = t;
			*(c_str() + size()) = 0;
		}
		void clean()
		{
			setsize(0);
		}

		union impl
		{
			impl()
			{
				less.size = 0;
			}
			~impl() = default;

			struct
			{
				Chartype data[sso_size];
				size_t size;
			}less;
			struct
			{
				Chartype* data;
				size_t size;
				size_t maxsize;
			}more;
		} core;
		bool flag = false;
	};



	template<typename T>
	class string_base_core
	{
	public:
		using Chartype = T;
		using self = string_base_core<T>;
		using size_t = uint32_t;
		using impl = string_base_impl<T>;

		impl* core = nullptr;


		self() :core{ new impl() } {}
		self(const size_t t) :core{ new impl(t);} {}
		self(const self& t) :core{ new impl(*(t.core)) } {}
		self(self&& t) :core{ new impl(std::move(t)) } {}
		self(const Chartype* t, const size_t s) :core{ new impl(s) }
		{
			//memcpy(core->c_str(), t, s);
			std::copy(t, t + s, core->c_str());
			core->setsize(s);
		}


		~string_base_core()
		{
			if (core->flag)
				delete[] core->c_str();
			delete core;
		}
		void insert(const size_t pos, const Chartype* t, const size_t s)
		{
			if (core->size() + s <= core->max_size())
			{
				Chartype * temp = new Chartype[core->size() - pos + 1];
				//memcpy(temp, core->c_str() + pos, size() - pos);
				//memmove(core->c_str() + pos, t, s);
				//memcpy(core->c_str() + pos + s, temp, size() - pos);
				std::copy(t, t + s, core->c_str() + pos);
				std::copy(core->c_str() + pos, core->c_str() + core->size(), temp);
				std::copy(temp, temp + core->size() - pos, core->c_str() + pos + s);
				delete[] temp;
				core->setsize(core->size() + s);
			}
			else
			{
				Chartype * temp = new Chartype[core->size() + s + sso_size];
				//memcpy(temp, core->c_str(), pos);
				//memcpy(temp + pos, t, s);
				//memcpy(temp + pos + s, core->c_str() + pos, size() - pos);
				std::copy(core->c_str(), core->c_str() + pos, temp);
				std::copy(t, t + s, temp + pos);
				std::copy(core->c_str() + pos, core->c_str() + core->size(), temp + pos + s);
				auto p = core->size() + s;
				if (core->flag)
					delete[] core->c_str();
				else
					core->flag = true;
				core->core.more.data = temp;
				core->setsize(p);
				core->core.more.maxsize = p + sso_size;
			}
		}
		void assign(const Chartype* t, const size_t s)
		{
			if (s <= core->max_size())
			{
				//memcpy(core->c_str(), t, s);
				std::copy(t, t + s, core->c_str());
				core->setsize(s);
			}
			else
			{
				Chartype * temp = new Chartype[s + sso_size];
				//memcpy(temp, t, s);
				std::copy(t, t + s, temp);
				if (flag)
					delete[] core->c_str();
				else
					flag = true;
				core->core.more.data = temp;
				core->setsize(p);
				core->core.more.maxsize = s + sso_size;
			}
		}
		void append(const Chartype* t, const size_t s)
		{
			if (core->size() + s <= core->max_size())
			{
				//memcpy(core->c_str() + core->size(), t, s);
				std::copy(t, t + s, core->c_str() + core->size());
				core->setsize(core->size() + s);
			}
			else
			{
				Chartype * temp = new Chartype[core->size() + s + sso_size];
				//memcpy(temp, core->c_str(), core->size());
				//memcpy(temp + core->size(), t, s);
				std::copy(core->c_str(), core->c_str() + core->size(), temp);
				std::copy(t, t + s, temp + core->size());
				auto p = core->size() + s;
				if (core->flag)
					delete[] core->c_str();
				else
					core->flag = true;
				core->core.more.data = temp;
				core->setsize(p);
				core->core.more.maxsize = p + sso_size;
			}
		}
		void erase(const size_t pos1, const size_t pos2)
		{
			//memcpy(core->c_str() + pos1, core->c_str() + pos2, core->size() - pos2);
			std::copy(core->c_str() + pos2, core->c_str() + core->size(), core->c_str() + pos1);
			core->setsize(core->size() - pos2);
		}
		void getmore(const size_t t)
		{
			if (core->max_size() < t)
			{
				Chartype * temp = new Chartype[t + sso_size];
				//memcpy(temp, core->c_str(), core->size());
				std::copy(core->c_str(), core->c_str() + core->size(), temp);
				auto p = core->size();
				if (core->flag)
					delete[] core->c_str();
				else
					core->flag = true;
				core->core.more.data = temp;
				core->setsize(p);
				core->core.more.maxsize = t + sso_size;

			}
			return;
		}
		void clear()noexcept
		{
			if (flag)
				delete[] core->c_str();
			core->flag = false;
			core->clean();
		}

	};
	
	template<typename T>
	class string_base
	{
		using Chartype = T;
		using self = string_base;
		using size_t = uint32_t;
		using iterator = string_base_iterator<T>;

		friend iterator;

		string_base_core<T>* core = nullptr;
		static iterator npos;

	public:
		self() :core{ new string_base_core<T>()} {}
		self(const Chartype* p) :core{ new string_base_core<T>(p,strlen(p)) } {}
		self(const self& p) :core{ new string_base_core<T>(*(p.core)) } {}
		self(self&& p) :core{ p.core } { p.core = nullptr; }
		self(const iterator& a, const iterator& b) :core{ new string_base_core<T>(a.data,b - a) } {}
		self(const Chartype& s) :core{ new string_base_core<T>(&s,1) } {}

		self& operator=(const self& p)
		{
			if (this != &p)
			{
				auto temp{ p };
				swap(this->core, temp->core);
			}
			return *this;
		}
		self& operator=(self&& p)
		{

			if (this != &p)
			{
				using std::swap;
				auto temp{ std::move(p) };
				swap(this->core, temp.core);
			}
			return *this;
		}
		self& operator=(const Chartype* p)
		{
			auto temp{ p };
			swap(this->core, temp->core);
			return *this;
		}
		self& operator=(const Chartype& p)
		{
			auto temp{ p };
			swap(this->core, temp->core);
			return *this;
		}

		Chartype& operator[](const size_t p)noexcept
		{
			return *(c_str() + p);
		}
		const Chartype& operator[](const size_t p)const noexcept
		{
			return *(c_str() + p);
		}
		const Chartype& at(const size_t p)
		{
			check(p);
			return this->operator[](p);
		}
		const Chartype& at(const size_t p)const
		{
			check(p);
			return this->operator[](p);
		}

		self& operator+=(const self& t)
		{
			core->append(t.c_str(), t.size());
			return *this;
		}
		self& operator+=(const Chartype* p)
		{
			core->append(p, strlen(p));
			return *this;
		}
		self& operator+=(const Chartype& p)
		{
			core->append(&p, 1);
			return *this;
		}
		self& operator*=(const size_t p)
		{
			if (p == 0)
			{
				core->core->clean();
			}
			else if (p != 1)
			{
				core->getmore(p*size());
				auto k = size();
				for (int i = 1; i < p; ++i)
					core->append(c_str(), k);
			}
			return *this;
		}

		friend self operator+(const self& a, const self& b)
		{
			self temp{ a };
			temp += b;
			return temp;
		}
		friend self operator+(const self& a, const Chartype* b)
		{
			self temp{ a };
			temp += b;
			return temp;
		}
		friend self operator+(const self& a, const Chartype& b)
		{
			self temp{ a };
			temp += b;
			return temp;
		}
		friend self operator*(const self& a, const size_t b)
		{
			self temp{ a };
			temp *= b;
			return temp;
		}

		friend bool operator==(const self& a, const self& b)noexcept
		{
			return a.comp(b.c_str()) == 0;
		}
		friend bool operator==(const self& a, const char* b)noexcept
		{
			return a.comp(b) == 0;
		}
		friend bool operator!=(const self& a, const self& b)noexcept
		{
			return a.comp(b.c_str()) != 0;
		}
		friend bool operator!=(const self& a, const char* b)noexcept
		{
			return a.comp(b) != 0;
		}
		friend bool operator<(const self& a, const self& b)noexcept
		{
			return a.comp(b.c_str()) < 0;
		}
		friend bool operator<(const self& a, const char* b)noexcept
		{
			return a.comp(b) < 0;
		}
		friend bool operator>(const self& a, const self& b)noexcept
		{
			return a.comp(b.c_str()) > 0;
		}
		friend bool operator>(const self& a, const char* b)noexcept
		{
			return a.comp(b) > 0;
		}
		friend bool operator<=(const self& a, const self& b)noexcept
		{
			return a.comp(b.c_str()) <= 0;
		}
		friend bool operator<=(const self& a, const char* b)noexcept
		{
			return a.comp(b) <= 0;
		}
		friend bool operator>=(const self& a, const self& b)noexcept
		{
			return a.comp(b.c_str()) >= 0;
		}
		friend bool operator>=(const self& a, const char* b)noexcept
		{
			return a.comp(b) >= 0;
		}

		friend std::ostream& operator<<(std::ostream& os, const self& b)
		{
			os << b.c_str();
			return os;
		}
		friend std::istream& operator>>(std::istream& is, self& b)	
		{
			Chartype p[1024];
			typename std::istream::sentry sentry(is);
			size_t extracted = 0;
			size_t top = 0;
			int err = std::istream::goodbit;
			if (sentry)
			{
				size_t n = is.width();
				if (n <= 0)
					n = -1;
				b.core->core->clean();
				for (auto got = is.rdbuf()->sgetc(); extracted != n; ++extracted, ++top) {
					if (got == EOF) 
					{
						err |= int( std::istream::eofbit);
						is.width(0);
						break;
					}
					if (isspace(got))
						break;
					p[top] = got;
					if (top == 1023)
					{
						b.core->append(p, 1024);
						top = 0;
					}
					got = is.rdbuf()->snextc();
				}
				if (top != 0)
					b.core->append(p, top);
				if (!extracted) 
					err |= int(std::istream::failbit);
				if (err) 
					is.setstate(err);
			}
			return is;
		}
		

		Chartype* c_str()noexcept
		{
			return core->core->c_str();
		}
		const Chartype* c_str()const noexcept
		{
			return core->core->c_str();
		}
		Chartype* data()noexcept
		{
			return core->core->c_str();
		}
		const Chartype* data()const noexcept
		{
			return core->core->c_str();
		}

		bool empty()const noexcept
		{
			return size() == 0;
		}

		const size_t size()const noexcept
		{
			return core->core->size();
		}
		const size_t length()const noexcept
		{
			return size();
		}

		void swap(self& p)noexcept
		{
			using std::swap;
			if (this == &p)
				return;
			swap(core, p.core);
		}

		iterator begin()noexcept
		{
			return iterator{ c_str() };
		}
		const iterator begin()const noexcept
		{
			return iterator(c_str());
		}

		iterator end()noexcept
		{
			return iterator{ c_str()+size() };
		}
		const iterator end()const noexcept
		{
			return iterator{ c_str() + size() };
		}

		iterator erase(const iterator& a)
		{
			core->erase(a - begin(), a - begin() + 1);
			return a;
		}
		iterator erase(const iterator& a, const iterator& b)
		{
			core->erase(a - begin(), b - begin());
			return a;
		}
		iterator erase(const size_t a)
		{
			core->erase(a , a + 1);
			return a;
		}
		iterator erase(const size_t a, const size_t b)
		{
			core->erase(a, b);
			return a;
		}

		void insert(const size_t pos, const Chartype* t)
		{
			core->insert(pos, t, strlen(t));
		}
		void insert(const size_t pos, const Chartype t)
		{
			core->insert(pos, &t, 1);
		}
		void insert(const size_t pos, const self& t)
		{
			core->insert(pos, t.c_str(), size());
		}
		void insert(const iterator& pos, const Chartype* t)
		{
			core->insert(pos-begin(), t, strlen(t));
		}
		void insert(const iterator& pos, const Chartype t)
		{
			core->insert(pos - begin(), &t, 1);
		}
		void insert(const iterator& pos, const self& t)
		{
			core->insert(pos - begin(), t.c_str(), size());
		}

		void replace(const Chartype* a, const Chartype* b, size_t sa = 0, size_t sb = 0)
		{
			assert(a);
			assert(b);
			sa = sa == 0 ? strlen(a) : sa;
			sb = sb == 0 ? strlen(b) : sb;
			assert(sa);
			auto p = find(a);
			while (p != npos)
			{
				//print(*this);
				auto d = p - begin();
				*this = substr(begin(), p) + b + substr(p + sa, end());
				p = find(a, d + sb);
				//print(*this);
			}	
		}
		void replace(const self& a, const self& b)
		{
			return replace(a.c_str(), b.c_str(), a.size(), b.size());
		}

		self& strip(const Chartype* p = " \t\n\r")
		{
			if (p == nullptr)
				throw("..");
			iterator i = begin();
			for (; i != end(); ++i)
				for (auto j = 0; *(j + p) != 0; ++j)
					if (*i != *(j + p))
						break;
			erase(begin(), i);
			i = end() - 1;
			for (; i != begin(); --i)
				for (auto j = 0; *(j + p) != 0; ++j)
					if (*i != *(j + p))
						break;
			erase(i + 1, end());
		}
		self& strip(const self& p)
		{
			return strip(p.c_str());
		}

		std::vector<self> split(const Chartype* p = " \t\n\r")const
		{
			vector<self> pp;
			if (p == nullptr)
				throw("");
			if (*p == 0)
			{
				for (auto i = begin(); i != end(); ++i)
					pp.push_back(string{ *i });
			}
			else
			{
				auto t = find_first_of(p);
				auto l = begin();
				while (t != npos)
				{
					pp.push_back(substr(l, t));
					auto c = find_first_of(p);
					while (c - 1 == t)
					{
						t = c;
						c = find_first_of(t);
					}
					l = t + 1;
					t = c;
				}
				if (l != end())
					pp.push_back(substr(l, end()));
			}
			return pp;
		}
		std::vector<self> split(const self& p)const
		{
			return split(p.c_str());
		}

		void push_back(const Chartype p)
		{
			core->append(&p, 1);
		}
		void pop_back()noexcept
		{
			setsize(size() - 1);
		}

		int compare(const self& b)const noexcept
		{
			return this->comp(b);
		}

		void clear()noexcept
		{
			core->core->clean();
		}

		size_t count(const Chartype* s)const
		{
			if (s == nullptr)
				throw("");
			size_t p = 0;
			auto ll = find(s);
			while (ll != npos)
			{
				++p;
				ll = find(s, ll + 1);
			}
			return p;
		}
		size_t count(const self& s)const
		{
			return count(s.c_str());
		}

		self substr(const size_t a, const size_t b)const
		{
			check(a);
			check(b);
			return self{ a + begin(),b + begin() };
		}
		self substr(const size_t a)const
		{
			check(a);
			return self{ a + begin(),end() };
		}
		self substr(const iterator& a, const iterator& b)const
		{
			check(a-begin());
			check(b-begin());
			return self{ a,b };
		}
		self substr(const iterator& a)const
		{
			check(a - begin());
			return self{ a,end()-begin() };
		}

		iterator find(const Chartype* p, const size_t a, const size_t b)const noexcept
		{
			if (a > b || a > size() || b > size()||p == nullptr)
				return npos;
			for (auto i = begin() + a; i!= begin()+b; ++i)
			{
				for (auto j = 0;; ++j)
				{
					if (*(p + j) == 0)
						return i;
					if (*(i + j) == 0)
						return npos;
					if (*(i + j) != *(p + j))
						break;
				}
			}
			return npos;
		}
		iterator find(const Chartype* p, const size_t a = 0)const noexcept
		{
			return find(p, a, size());
		}
		iterator find(const self& p, const size_t a, const size_t b)const noexcept
		{
			return find(p.c_str(), a, b);
		}
		iterator find(const self& p, const size_t a = 0)const noexcept
		{
			return find(p.c_str(), a, size());
		}
		iterator find(const Chartype* p, const iterator& a, const iterator& b)const noexcept
		{
			return find(p, a - begin(), b - begin());
		}
		iterator find(const Chartype* p, const iterator& a)const noexcept
		{
			return find(p, a - begin(), size());
		}
		iterator find(const self* p, const iterator& a, const iterator& b)const noexcept
		{
			return find(p.c_str(), a - begin(), b - begin());
		}
		iterator find(const self* p, const iterator& a)const noexcept
		{
			return find(p.c_str(), a - begin(), size());
		}

		iterator rfind(const Chartype* p, const size_t a, const size_t b,const size_t leng=0)const noexcept
		{
			if (a > b || a > size() || b > size() || p == nullptr)
				return npos;
			leng = leng == 0 ? strlen(p) : leng;
			for (auto i = begin() + b - 1; i != begin() + a; --i)
			{
				bool flag = true;
				for (auto j = leng; j>=1; --j)
				{
					if (i - (leng - j) < begin())
						return npos;
					if (*(i - (leng -j)) != p[j-1])
					{
						flag = false;
						break;
					}
				}
				if (flag)
					return i - leng + 1;
			}
			return npos;
		}
		iterator rfind(const Chartype* p, const size_t a = 0)const noexcept
		{
			return rfind(p, a, size());
		}
		iterator rfind(const self& p, const size_t a, const size_t b)const noexcept
		{
			return rfind(p.c_str(), a, b,size());
		}
		iterator rfind(const self& p, const size_t a = 0)const noexcept
		{
			return rfind(p.c_str(), a, size(),size());
		}
		iterator rfind(const Chartype* p, const iterator& a, const iterator& b)const noexcept
		{
			return rfind(p, a - begin(), b - begin());
		}
		iterator rfind(const Chartype* p, const iterator& a)const noexcept
		{
			return rfind(p, a - begin(), size());
		}
		iterator rfind(const self* p, const iterator& a, const iterator& b)const noexcept
		{
			return rfind(p.c_str(), a - begin(), b - begin(),size());
		}
		iterator rfind(const self* p, const iterator& a)const noexcept
		{
			return rfind(p.c_str(), a - begin(), size(),size());
		}

		iterator find_first_of(const Chartype* p, const size_t a, const size_t b)const noexcept
		{
			if (a > b || a > size() || b > size() || p == nullptr)
				return npos;
			for (auto i = begin() + a; i != begin() + b; ++i)
				for (auto j = 0; *(p + j) != 0; ++j)
					if (*(p + j) == *i)
						return i;
			return npos;
		}
		iterator find_first_of(const Chartype* p, const size_t a = 0)const noexcept
		{
			return find_first_of(p, a, size());
		}
		iterator find_first_of(const self& p, const size_t a, const size_t b)const noexcept
		{
			return find_first_of(p.c_str(), a, b);
		}
		iterator find_first_of(const self& p, const size_t a = 0)const noexcept
		{
			return find_first_of(p.c_str(), a, size());
		}
		iterator find_first_of(const Chartype* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_first_of(p, a - begin(), b - begin());
		}
		iterator find_first_of(const Chartype* p, const iterator& a)const noexcept
		{
			return find_first_of(p, a - begin(), size());
		}
		iterator find_first_of(const self* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_first_of(p.c_str(), a - begin(), b - begin());
		}
		iterator find_first_of(const self* p, const iterator& a)const noexcept
		{
			return find_first_of(p.c_str(), a - begin(), size());
		}

		iterator find_first_not_of(const Chartype* p, const size_t a, const size_t b)const noexcept
		{
			if (a > b || a > size() || b > size() || p == nullptr)
				return npos;
			for (auto i = begin() + a; i != begin() + b; ++i)
			{
				bool flag = false;
				for (auto j = 0; *(p + j) != 0; ++j)
					if (*(p + j) == *i)
					{
						flag = true;
						break;
					}
				if (!flag)
					return i;
			}
				
			return npos;
		}
		iterator find_first_not_of(const Chartype* p, const size_t a = 0)const noexcept
		{
			return find_first_not_of(p, a, size());
		}
		iterator find_first_not_of(const self& p, const size_t a, const size_t b)const noexcept
		{
			return find_first_not_of(p.c_str(), a, b);
		}
		iterator find_first_not_of(const self& p, const size_t a = 0)const noexcept
		{
			return find_first_not_of(p.c_str(), a, size());
		}
		iterator find_first_not_of(const Chartype* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_first_not_of(p, a - begin(), b - begin());
		}
		iterator find_first_not_of(const Chartype* p, const iterator& a)const noexcept
		{
			return find_first_not_of(p, a - begin(), size());
		}
		iterator find_first_not_of(const self* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_first_not_of(p.c_str(), a - begin(), b - begin());
		}
		iterator find_first_not_of(const self* p, const iterator& a)const noexcept
		{
			return find_first_not_of(p.c_str(), a - begin(), size());
		}

		iterator find_last_of(const Chartype* p, const size_t a, const size_t b)const noexcept
		{
			if (a > b || a > size() || b > size() || p == nullptr)
				return npos;
			for (auto i = begin() + b -1; i >= begin() + a; --i)
				for (auto j = 0; *(p + j) != 0; ++j)
					if (*(p + j) == *i)
						return i;
			return npos;
		}
		iterator find_last_of(const Chartype* p, const size_t a = 0)const noexcept
		{
			return find_last_of(p, a, size());
		}
		iterator find_last_of(const self& p, const size_t a, const size_t b)const noexcept
		{
			return find_last_of(p.c_str(), a, b);
		}
		iterator find_last_of(const self& p, const size_t a = 0)const noexcept
		{
			return find_last_of(p.c_str(), a, size());
		}
		iterator find_last_of(const Chartype* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_last_of(p, a - begin(), b - begin());
		}
		iterator find_last_of(const Chartype* p, const iterator& a)const noexcept
		{
			return find_last_of(p, a - begin(), size());
		}
		iterator find_last_of(const self* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_last_of(p.c_str(), a - begin(), b - begin());
		}
		iterator find_last_of(const self* p, const iterator& a)const noexcept
		{
			return find_last_of(p.c_str(), a - begin(), size());
		}

		iterator find_last_not_of(const Chartype* p, const size_t a, const size_t b)const noexcept
		{
			if (a > b || a > size() || b > size() || p == nullptr)
				return npos;
			for (auto i = begin() + b-1; i >= begin() + a; --i)
			{
				bool flag = false;
				for (auto j = 0; *(p + j) != 0; ++j)
					if (*(p + j) == *i)
					{
						flag = true;
						break;
					}
				if (!flag)
					return i;
			}

			return npos;
		}
		iterator find_last_not_of(const Chartype* p, const size_t a = 0)const noexcept
		{
			return find_last_not_of(p, a, size());
		}
		iterator find_last_not_of(const self& p, const size_t a, const size_t b)const noexcept
		{
			return find_last_not_of(p.c_str(), a, b);
		}
		iterator find_last_not_of(const self& p, const size_t a = 0)const noexcept
		{
			return find_last_not_of(p.c_str(), a, size());
		}
		iterator find_last_not_of(const Chartype* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_last_not_of(p, a - begin(), b - begin());
		}
		iterator find_last_not_of(const Chartype* p, const iterator& a)const noexcept
		{
			return find_last_not_of(p, a - begin(), size());
		}
		iterator find_last_not_of(const self* p, const iterator& a, const iterator& b)const noexcept
		{
			return find_last_not_of(p.c_str(), a - begin(), b - begin());
		}
		iterator find_last_not_of(const self* p, const iterator& a)const noexcept
		{
			return find_last_not_of(p.c_str(), a - begin(), size());
		}

		explicit operator int()const { return stoi(c_str()); }
		explicit operator long long()const { return stoll(c_str()); }
		explicit operator float()const { return stof(c_str()); }
		explicit operator double()const { return stod(c_str()); }

		~string_base()
		{
			delete core;
		}
	private:
		int comp(const Chartype* p)const noexcept
		{
			auto p1 = static_cast<std::make_unsigned<Chartype>*>(c_str());
			auto p2 = static_cast<std::make_unsigned<Chartype>*>(p);
			for (; *p1 == *p2; ++p1, ++p2)
				if (*p1 == 0)
					return 0;
			return *p1 - *p2;
		}
		void check(const size_t s)const
		{
			if (s > size())
				throw("d");
		}

	};

	template<typename T>
	string_base_iterator<T> string_base<T>::npos((T*)nullptr );

	using string = string_base<char>;

	string tostring(const string& p)
	{
		//auto temp{ p };
		return p;
	}

	string tostring(const char* p)
	{
		return string{ p };
	}

	inline string tostring(const int p)
	{
		return tostring((long long)p);
	}

	inline string tostring(const long long p)
	{
		char d[30];
		d[29] = '\0';
		char* t = &d[29];
		long long dd = abs(p);
		while (dd > 0)
		{
			--t;
			*t = (dd % 10) + '0';
			dd /= 10;
		}
		if (p < 0)
			*--t = '-';
		return string{ t };
	}

	inline string tostring(const double p)
	{
		char d[20];
		sprintf_s(d, "%f", p);
		return string{ d };
	}

	inline string tostring(const float p)
	{
		return tostring(double(p));
	}

	inline string tostring(const char p)
	{
		return string{ p };
	}

	inline string tostring(const bool p)noexcept
	{
		if (p)
			return string{ "true" };
		return string{ "false" };
	}

	template<typename T>
	string tostring(const T& p)noexcept(p.tostring())
	{
		return p.tostring();
	}

	template<typename T>
	string join(const T& p)
	{
		auto b1 = begin(p);
		auto b2 = end(p);
		string t{ "" };
		while (auto i = b1; i != b2; ++i)
			t += tostring(*i) + " ";
		if (*(t.end() - 1) == ' ')
			erase(t.end() - 1);
		return t;
	}
}