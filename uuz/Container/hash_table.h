#pragma once
#include"container.h"
#include"list.h"
#include"vector.h"

namespace uuz
{

	template<typename value>
	struct hash_node
	{
	public:
		template<typename... Args>
		hash_node(Args&&... args) :dat{ value(std::forward<Args>(args)...),nullptr}{}

		value& data()noexcept { return dat.first(); }
		const value& data() const noexcept { return dat.first(); }

		hash_node*& next()noexcept { return dat.second(); }
		const hash_node*& next()const noexcept { return dat.second(); }

	private:
		compressed_pair<value, hash_node*> dat;
	};

	template<typename value, typename alloc, typename hash, typename equal>
	struct hash_set_traits
	{
		using key_value = value;
		using value_type = value;
		using alloc_type = allocator_traits<alloc>::template rebind_alloc<value_type>;
		using hash_type = exchange_front<hash, key_value>::typename type;
		using equal_type = exchange_front<equal, key_value>::typename type;
	};

	template<typename key,typename value, typename alloc, typename hash, typename equal>
	struct hash_set_traits
	{
		using key_value = std::add_const_t<key>;
		using value_type = pair<key_value, value>;
		using alloc_type = allocator_traits<alloc>::template rebind_alloc<value_type>;
		using hash_type = exchange_front<hash, key_value>::typename type;
		using equal_type = exchange_front<equal, key_value>::typename type;
	};



	template<typename hash_traits,bool multi = false>
	class hash_type
	{
		constexpr static float default_factor = 5.0f;
		using alloc_type = hash_traits::typename alloc_type;
		using key_value = hash_traits::typename key_value;
		using value_type = hash_traits::typename value_type;
		using hash_type = hash_traits::typename hash_type;
		using equal_type = hash_traits::typename equal_type;
		using vec_type = vector<hash_node<value_type>*, typename allocator_traits<alloc_type>::template rebind_alloc<hash_node<value_type>*>>;
		using size_type = size_t;
	public:
		hash_type hash_function() const
		{
			return get_hash();
		}

		equal_type key_eq() const
		{
			return get_eq();
		}

		size_type size()const noexcept
		{
			return get_size();
		}

		alloc_type get_allocator() const
		{
			return get_al();
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}

		size_type max_size() const noexcept
		{
			return allocator_traits<alloc_type>::max_size(get_al());
		}

		float max_load_factor() const
		{
			return get_factor();
		}
		void max_load_factor(float ml)
		{
			get_factor() = ml;
			_rehash();
		}

		void rehash(size_type count)
		{
			get_vec().reserve(count < size() / max_load_factor() ? size() / max_load_factor() : count);
			_rehash();
		}
		void reserve(size_type count)
		{
			rehash(std::ceil(count / max_load_factor()));
		}

	protected:

	private:

		hash_type& get_hash() 
		{
			return data.first();
		}
		const hash_type& get_hash() const
		{
			return data.first();
		}

		equal_type& get_eq() 
		{
			return data.second().second().first();
		}
		const equal_type& get_eq() const
		{
			return data.second().second().first();
		}

		alloc_type& get_al()
		{
			return data.second().first();
		}
		const alloc_type& get_al() const
		{
			return data.second().first();
		}

		vec_type& get_vec()
		{
			return data.second().second().second().bu;
		}
		const vec_type& get_vec() const
		{
			return data.second().second().second().bu;
		}

		size_type& get_size()
		{
			return data.second().second().second().size;
		}
		const size_type& get_size() const
		{
			return data.second().second().second().size;
		}

		float& get_factor()
		{
			return data.second().second().second().factor;
		}
		const float& get_factor() const
		{
			return data.second().second().second().factor;
		}

		struct impl
		{
			vec_type bu;
			size_type size = 0;
			float factor = default_factor;
		};
		compressed_pair<hash_type, compressed_pair<alloc_type, compressed_pair<equal_type, impl>>> data;
	};
}
