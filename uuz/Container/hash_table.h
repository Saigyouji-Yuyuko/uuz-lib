#pragma once
#include"container.h"
#include<functional>
#include"vector.h"
#include<cmath>
#include <minwindef.h>

namespace uuz
{
	template<typename T>
	struct bucket_node
	{
		T data;
		size_t hash;
		bucket_node<T>* next = nullptr;
	};


	template<typename key, typename value_type, typename hash, typename equal, typename Alloc, bool multi>
	class hash_table;
	
	template<typename T>
	class hash_table_iterator
	{
		using vec_it = typename vector<bucket_node<T>*>::iterator;
		template<typename key, typename value_type, typename hash, typename equal, typename Alloc, bool multi>
		friend hash_table<key,T,  hash,  equal,  Alloc,  multi>;
	public:
		hash_table_iterator& operator++() noexcept
		{	
			t = t->next;
			while (!t)
			{
				++it;
				if (it == end)
					break;
				t = *it;
			} 
			return *this;
		}
		hash_table_iterator operator++(int)noexcept
		{
			auto p{ *this };
			++*t;
			return p;
		}

		T& operator*()noexcept
		{
			return t->data;
		}
		const T& operator*()const noexcept
		{
			return t->data;
		}
		T* operator->()noexcept
		{
			return &(t->data);
		}
		const T* operator->()const noexcept
		{
			return &(t->data);
		}

		friend bool operator==(const hash_table_iterator& a, const hash_table_iterator& b)noexcept
		{
			return a.t == b.t || a.it == a.end && b.it == b.end;
		}
		friend bool operator!=(const hash_table_iterator& a, const hash_table_iterator& b)noexcept
		{
			return !(a == b);
		}
		friend bool operator<(const hash_table_iterator& a, const hash_table_iterator& b)noexcept
		{
			if(a.it == b.it)
			{
				if (a.it == a.end)
					return false;
				return a.t < b.t;
			}
			return a.it < b.it;
		}
		friend bool operator>(const hash_table_iterator& a, const hash_table_iterator& b)noexcept
		{
			return b < a;
		}
		friend bool operator<=(const hash_table_iterator& a, const hash_table_iterator& b)noexcept
		{
			return a < b || a == b;
		}
		friend bool operator>=(const hash_table_iterator& a, const hash_table_iterator& b)noexcept
		{
			return a > b || a == b;
		}

	private:
		hash_table_iterator(const vec_it it, const vec_it end,const bucket_node<T>* t)
			:it(const_cast<vec_it>(it)), end(const_cast<vec_it>(end)),t(const_cast<bucket_node<T>*>(end)) {}

		vec_it it;
		vec_it end;
		bucket_node<T>* t;
	};

	template<typename key,typename value_type,typename hash_type,typename equal_type,typename Alloc,bool multi>
	class hash_table
	{
	public:
		using iterator = hash_table_iterator<value_type>;
		using node_Allocator = typename exchange<Alloc, bucket_node<value_type>>::type;
		using node = bucket_node<value_type>;
		using bucket = node*;
		constexpr static size_t least_size = 16;
		constexpr static float bucket_size = 5.0f;
	private:
		mutable hash_type hash;
		mutable equal_type equal;
		node_Allocator alloc;
		vector<bucket, typename exchange<Alloc, bucket>::type> buckets{ least_size ,nullptr };
		size_t ssize = 0;
	public:
		hash_table() :hash(hash_type()), equal(equal_type()), alloc(Alloc()) {}
		hash_table(size_t buckets_size,
			const hash_type& hash = hash_type(),
			const equal_type& equal = equal_type(),
			const Alloc& alloc = Alloc()) :hash(hash), equal(equal), alloc(alloc), buckets(min(buckets_size, least_size), nullptr) {}
		hash_table(const hash_table& t, const Alloc& alloc) :hash(t.hash), equal(t.equal), alloc(alloc), buckets(t.buckets.size(), nullptr), ssize(t.ssize)
		{
			try
			{
				for (auto i = 0; i != t.buckets.size(); ++i)
				{
					if (i)
					{
						buckets[i] = makenode(*(t.buckets[i]));
						auto k = buckets[i];
						for (auto j = t.buckets[i].next; j != nullptr; ++j)
						{
							k->next = makenode(*j);
							k = k->next;
						}
					}
				}
			}
			catch (...)
			{
				clear();
				throw;
			}
		}
		hash_table(const hash_table& t) :hash_table(t, Alloc()) {}
		hash_table(hash_table&& t, const Alloc& alloc) :alloc(alloc)
		{
			try
			{
				this->swap(t);
				t.clear();
			}
			catch (...)
			{
				clear();
				throw;
			}

		}
		hash_table(hash_table&& t) :hash_table(t, Alloc()) {}

		hash_table& operator=(const hash_table& other)
		{
			if (this != &other)
			{
				hash_table temp(other, alloc);
				this->swap(temp);
			}
			return *this;
		}
		hash_table& operator=(hash_table&& other)
		{
			if (this != &other)
			{
				hash_table temp(std::move(other), alloc);
				this->swap(temp);
			}
			return *this;
		}

		node_Allocator get_allocator()const noexcept
		{
			return alloc;
		}

		void clear()noexcept
		{
			for(auto i:buckets)
			{
				deallocbucket(i);
				i = nullptr;
			}	
			ssize = 0;
		}

		iterator begin() noexcept
		{
			return iterator()
		}
		const iterator begin() const noexcept;
		const iterator cbegin() const noexcept;


		size_t size()const noexcept
		{
			return ssize;
		}

		bool empty()const noexcept
		{
			return size() == 0;
		}

		void swap(hash_table& other)noexcept(is_nothrow_swap_alloc<Alloc>::value)
		{
			if(alloc == other.alloc)
			{
				char temp[sizeof(hash_table)];
				memcpy(temp, this, sizeof(hash_table));
				memcpy(this, &other, sizeof(hash_table));
				memcpy(&other, temp, sizeof(hash_table));
			}
			else
			{
#ifdef DEBUG
				assert(false, "It's not defined that allocator is not equal!");
#else // DEBUG
				hash_table t1(other, alloc);
				hash_table t2(*this, other.alloc);
				this->swap(t1);
				other.swap(t2);
#endif
			}
		}

		template<typename InputIt,typename = is_input<value_type,InputIt>>
		void init(InputIt first,InputIt last)
		{
			auto k = last - first;
			buckets.resize(trans(k));
			auto i = first;
			try
			{
				for(;i!=last;++i)
				{
					if constexpr(multi)
					{
						node_insert(make(*i));
					}
					else
					{
						auto k = bucket_wrap(hash(*i));
						bool flag = true;
						for (auto j = buckets[k]; j != nullptr; j = j->next)
							if(equal(j->data,*i))
							{
								flag = false;
								break;
							}
						if (!flag)
							continue;
						node_insert(make(*i));
					}

				}
			}
			catch(...)
			{
				clear();
				throw;
			}
			ssize = k;
		}



	protected:
		
		node* makenode(const node& t)
		{
			node* k = nullptr;
			try
			{
				k = alloc.allocate();
				new(k) node{ t.data,t.hash };
			}
			catch(bad_alloc&)
			{
				throw;
			}
			catch(...)
			{
				alloc.deallocate(k, 1);
				throw;
			}
			return k;
			
		}
		template<typename...Args>
		node* makenode(Args&&...args)
		{
			node* k = nullptr;
			try
			{
				k = alloc.allocate();
				new(k) node{ value_type(std::forward<Args>(args)...),0 };
				k->hash = hash(k->data);
			}
			catch (bad_alloc&)
			{
				throw;
			}
			catch (...)
			{
				alloc.deallocate(k, 1);
				throw;
			}
			return k;
		}
		
		void deallocbucket(node* i)noexcept
		{
			node* k = nullptr;
			while(!i)
			{
				k = i->next;
				(*i).~node();
				alloc.deallocate(i, 1);
				i = k;
			}
		}

		size_t bucket_wrap(size_t t)
		{
			return t&(buckets.size() - 1);
		}

		void node_insert(node* k)noexcept
		{
			auto dd = bucket_wrap(k->hash);
			if (buckets[dd])
				k->next = buckets[dd];
			buckets[dd] = k;
		}


	private:
		static size_t trans(size_t t)noexcept
		{
			if (!t)
				return 0;
#ifdef _MSC_VER
			DWORD index;
			auto k = _BitScanReverse(&index, t - 1);
			if (k)
				return 1 << (index + 1);
			//throw("");
#elif __GNUC__
			auto k = __builtin_clz(t - 1);
			return 1 << (k + 1);
#else
			size_t k = 1;
			while (k < t)
				k << 1;
			return k;
#endif
		}
	
	};
}
