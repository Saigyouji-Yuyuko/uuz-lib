#pragma once
#include"container.h"
#include<functional>
#include"list.h"
#include<cmath>
#include <minwindef.h>

namespace uuz
{
	template<typename T>
	struct bucket_node
	{
		T data;
		bucket_node<T>* next = nullptr;
	};

	template<typename T>
	struct bucket
	{
		std::size_t hash = 0;
		bucket_node<T>* next = nullptr;;
	};

	template<typename key, typename value_type, typename hash, typename equal, typename Alloc, bool multi>
	class hash_table;
	
	template<typename T>
	class hash_table_iterator
	{
	public:
	private:
		//hash_table_iterator
	};

	template<typename key,typename value_type,typename hash_type,typename equal_type,typename Alloc,bool multi>
	class hash_table
	{
	public:
		using iterator = hash_table_iterator<value_type>;
		using list_Allocator = typename exchange<Alloc, bucket<value_type>>::type;
		using node_Allocator = typename exchange<Alloc, bucket_node<value_type>>::type;
		using node = bucket_node<value_type>;
		using bucket = bucket<value_type>;
		constexpr static size_t least_size = 16;
	private:
		mutable hash_type hash;
		mutable equal_type equal;
		node_Allocator alloc;
		list<bucket, list_Allocator> bucket_list;
		size_t ssize = 0;
	public:
		hash_table():hash(hash_type()),equal(equal_type()),alloc(Alloc()),bucket_list(least_size,alloc){}
		explicit hash_table(size_t bucket_count ,
			const hash_type& hash = hash_type(),
			const equal_type& equal = equal_type(),
			const Alloc& alloc = Alloc()):hash(hash), equal(equal), alloc(alloc), bucket_list(max(least_size, bucket_count),alloc) {}
		hash_table(size_t bucket_count,const Alloc& alloc):hash(hash_type()), equal(equal_type()), alloc(alloc), bucket_list(max(least_size, bucket_count), alloc) {}
		hash_table(size_t bucket_count,const hash_type& hash,const Alloc& alloc)
			: hash_table(bucket_count, hash, equal_type(), alloc) {}
		explicit hash_table(const Alloc& alloc): hash_table( hash_type(), equal_type(), alloc) {}
		template< typename InputIt,typename = is_input<value_type,InputIt> >
		hash_table(InputIt first, InputIt last,
			size_t bucket_count = least_size,
			const hash_type& hash = hash_type(),
			const equal_type& equal = equal_type(),
			const Alloc& alloc = alloc()): hash_table
			{
			
			}
	};
}
