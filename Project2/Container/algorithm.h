#pragma once
#include"container.h"
#include"pair.h"
#include"iterator.h"
//#include<execution>
namespace
{
	template<typename T>
	struct is_execution_policy_v;
}
namespace uuz
{
	namespace policy
	{
		template< typename ExecutionPolicy, typename ForwardIt, typename UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		bool all_of(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		bool any_of(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		bool none_of(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
		
		
		template< class ExecutionPolicy, class ForwardIt, class UnaryFunction2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		void for_each(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryFunction2 f);
		
		template< class ExecutionPolicy, class ForwardIt, class Size, class UnaryFunction2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt for_each_n(ExecutionPolicy&& policy, ForwardIt first, Size n, UnaryFunction2 f);

		template< class ExecutionPolicy, class ForwardIt, class T, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt find(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, const T& value);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt find_if(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last,UnaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt find_if_not(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last,UnaryPredicate q);
		
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt1 find_end(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,ForwardIt2 s_first, ForwardIt2 s_last);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>> >
		ForwardIt1 find_end(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt1 find_first_of(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,ForwardIt2 s_first, ForwardIt2 s_last);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt1 find_first_of(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p);
	
		template< class ExecutionPolicy, class ForwardIt, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt adjacent_find(ExecutionPolicy&& policy,ForwardIt first, ForwardIt last);
		template< class ExecutionPolicy, class ForwardIt, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt adjacent_find(ExecutionPolicy&& policy,ForwardIt first, ForwardIt last, BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt, class T, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		typename iterator_traits<ForwardIt>::difference_type
			count(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, const T &value);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		typename iterator_traits<ForwardIt>::difference_type
			count_if(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
		
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,ForwardIt2 first2);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,ForwardIt2 first2,BinaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,ForwardIt2 first2, ForwardIt2 last2);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,ForwardIt2 first2, ForwardIt2 last2,BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate >
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, BinaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, ForwardIt2 last2);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate >
		bool equal(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, ForwardIt2 last2,
			BinaryPredicate p);
	}

	template <typename InputIterator, typename UnaryPredicate>
	inline bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (!pred(*first))
				return false;
		return true;
	}
	template <typename InputIterator, typename UnaryPredicate>
	inline bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (pred(*first))
				return true;
		return false;
	}
	template <typename InputIterator, typename UnaryPredicate>
	inline bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (pred(*first))
				return false;
		return true;
	}

	template<typename InputIterator, typename UnaryPredicate>
	inline UnaryPredicate for_each(InputIterator first, InputIterator last, UnaryPredicate f)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first) 
			f(*first);
		return f;
	}
	
	template< class InputIt, class Size, class UnaryFunction >
	inline InputIt for_each_n(InputIt first, Size n, UnaryFunction f)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt>::iterator_category>
			, "iterator must be a input_iterator");
		for (Size i = 0; i != n; ++i, (void)++first)
			f(*first);
		return first;
	}
	
	template<typename InputIterator, typename T,typename = decltype((*std::decay<InputIterator>()) == std::decay<T>())>
	inline InputIterator find(InputIterator first, InputIterator last,const T& value)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first) 
			if (*first == value) 
				return first;
		return last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	inline InputIterator find_if(InputIterator first, InputIterator last,UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (pred(*first)) 
				return first;
		return last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	inline InputIterator find_if_not(InputIterator first, InputIterator last,UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first) 
			if (!pred(*first)) 
				return first;
		return last;
	}


	template<typename ForwardIterator1, typename ForwardIterator2,typename BinaryPredicate>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
								ForwardIterator2 first2, ForwardIterator2 last2,
									BinaryPredicate pred)
	{
		if (first2 == last2 || first1 == last1)
			return last1;
		if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
			&& std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>)
		{
			auto count1 = distance(first1, last1);
			auto count2 = distance(first2, last2);

			if (count2 > count1)
				return last1;

			auto s = first1 + (count2 - 1);
			auto temp1 = last1;
			auto temp2 = last2;
			--temp2;
			for (;;)
			{
				for (;;)
				{
					if (s == temp1)
						return last1;
					if (pred(*--temp1, *temp2))
						break;
				}
				auto kk = temp2;
				for (auto l = temp1;;)
				{
					if (kk == first2)
						return l;
					if (!pred(*--l, *--kk))
						break;
				}
			}
		}
		else if constexpr(std::is_base_of_v<bidirectional_iterator_tag,typename iterator_traits<ForwardIterator1>::iterator_category> 
					&& std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>)
		{

			auto temp1 = last1;
			auto temp2 = last2;

			--temp2;
			for (;;)
			{
				for (;;)
				{
					if (first1 == temp1)
						return last1;
					if (pred(*--temp1, *temp2))
						break;
				}
				auto kk = temp2;
				for (auto l = temp1;;)
				{
					if (kk == first2)
						return l;
					if (l == first1)
						return last1;
					if (!pred(*--l, *--kk))
						break;
				}
			}
		}
		else if constexpr(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
			&& std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>)
		{
		
			auto answer = last1;

			for(;;)
			{
				for(;;++first1)
				{
					if (first1 == last1)
						return answer;
					if (pred(*first1, *first2))
						break;
				}
				auto kk = first2;
				for (auto l = first1;;)
				{
					if(++kk == last2)
					{
						answer = first1;
						++first1;
						break;
					}
					if(++l == last1)
						return answer;
					if(!pred(*l,*kk))
					{
						++first1;
						break;
					}
				}
			}
			return answer;
		}
		else
		{
			static_assert(false, "iterator must be a forward_iterator");
		}
	}
	template<typename ForwardIterator1, typename ForwardIterator2, typename = decltype((*std::decay<ForwardIterator1>()) == (*std::decay<ForwardIterator2>()))>
	inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
								ForwardIterator2 first2, ForwardIterator2 last2)
	{
		return find_end(first1, last1, first2, last2, 
			[](const typename iterator_traits<ForwardIterator1>::value_type& x, 
				const typename iterator_traits<ForwardIterator2>::value_type& y) {return x == y; });
	}
	
	template<typename InputIterator, typename ForwardIterator,typename BinaryPredicate>
	inline InputIterator find_first_of(InputIterator first1, InputIterator last1,
							ForwardIterator first2, ForwardIterator last2,
							BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator>::iterator_category>
			, "iterator must be a forward_iterator");

		for (; first1 != last1; ++first1) 
			for (auto it = first2; it != last2; ++it) 
				if (pred(*first1, *it))
					return first1;
		return last1;
	}
	template<typename InputIterator, typename ForwardIterator>
	inline InputIterator find_first_of(InputIterator first1, InputIterator last1,
								ForwardIterator first2, ForwardIterator last2)
	{
		return find_first_of(first1, last1, first2, last2,
			[](const typename iterator_traits<InputIterator>::value_type& x,
				const typename iterator_traits<ForwardIterator>::value_type& y) {return x == y; });
	}

	template<typename ForwardIterator, typename BinaryPredicate>
	inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator>::iterator_category>
			, "iterator must be a forward_iterator");

		if (first == last) 
			return last;

		auto next = first;
		++next;

		for (; next != last; first = next,(void)++next)
			if (pred(*first,*next))
				return first;
			
		return last;
	}

	template<typename ForwardIterator>
	inline ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last)
	{
		return adjacent_find(first, last,
			[](const typename iterator_traits<ForwardIterator>::value_type& x,
				const typename iterator_traits<ForwardIterator>::value_type& y) {return x == y; });
	}
	
	template<typename InputIterator, typename Predicate>
	inline typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Predicate pred)
	{
		typename iterator_traits<InputIterator>::difference_type ss{0};

		for(;first!=last;++first)
			if (pred(*first))
				++ss;
		return ss;
	}
	template<typename InputIterator, typename T>
	inline typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value)
	{
		return count_if(first, last, 
			[&value](const typename iterator_traits<InputIterator>::value_type& x) {return *x == value; });
	}
	
	template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
	inline pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
													InputIterator2 first2, BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator1>::iterator_category>
			, "iterator must be a input_iterator");
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator2>::iterator_category>
			, "iterator must be a input_iterator");

		for (; first1 != last1; ++first1, (void)++first2)
			if (!pred(*first1, *first2))
				return pair<InputIterator1, InputIterator2>(first1, first2);
		return pair<InputIterator1, InputIterator2>(first1, first2);
	}
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	inline pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1,
										InputIt2 first2, InputIt2 last2,
										BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt1>::iterator_category>
			, "iterator must be a input_iterator");
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt2>::iterator_category>
			, "iterator must be a input_iterator");

		for (; first1 != last1 && first2 != last2; ++first1, (void)++first2)
			if (!pred(*first1, *first2))
				return pair<InputIt1, InputIt2>(first1, first2);
		return pair<InputIt1, InputIt2>(first1, first2);
	}
	template<typename InputIterator1, typename InputIterator2>
	inline pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,
													InputIterator2 first2)
	{
		return mismatch(first1, last1, first2, 
			[](const typename iterator_traits<InputIterator1>::value_type& x,
				const typename iterator_traits<InputIterator2>::value_type& y) {return x == y; });
	}
	template< class InputIt1, class InputIt2 >
	inline pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1,
										InputIt2 first2, InputIt2 last2)
	{
		return mismatch(first1, last1, first2, last2,
			[](const typename iterator_traits<InputIt1>::value_type& x,
				const typename iterator_traits<InputIt2>::value_type& y) {return x == y; });
	}
	
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	__forceinline bool equal(InputIt1 first1, InputIt1 last1,
				InputIt2 first2, BinaryPredicate p)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt1>::iterator_category>
			, "iterator must be a input_iterator");
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt2>::iterator_category>
			, "iterator must be a input_iterator");

		for(;first1!=last1;++first1,(void)++first2)
			if (!p(*first1, *first2))
				return false;
		return true;
	}
	template< class InputIt1, class InputIt2, class BinaryPredicate >
	__forceinline bool equal(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, InputIt2 last2,
			BinaryPredicate p)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt1>::iterator_category>
			, "iterator must be a input_iterator");
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt2>::iterator_category>
			, "iterator must be a input_iterator");
		
		if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<InputIt1>::iterator_category>
				&& std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<InputIt2>::iterator_category>)
		{
			if (distance(first1, last1) != distance(first2, last2))
				return false;
		}
		for (; first1 != last1 && first2 != last2; ++first1, (void)++first2)
			if (!p(*first1, *first2))
				return false;
		return first1 == last1 && first2 == last2;

	}
	template< class InputIt1, class InputIt2 >
	__forceinline bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2)
	{
		return equal(first1, last1, first2, [](const typename iterator_traits<InputIt1>::value_type& x,
										const typename iterator_traits<InputIt2>::value_type& y) {return x == y; });
	}
	template< class InputIt1, class InputIt2 >
	__forceinline bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2)
	{
		return equal(first1, last1, first2,last2, [](const typename iterator_traits<InputIt1>::value_type& x,
			const typename iterator_traits<InputIt2>::value_type& y) {return x == y; });
	}
	
	template<typename ForwardIterator1, typename ForwardIterator2,typename BinaryPredicate>
	__forceinline bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
											ForwardIterator2 first2, BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
			, "iterator must be a forward_iterator");
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>
			, "iterator must be a forward_iterator");

		auto k = distance(first1, last1);
		auto last2 = next(first2, k);

		for(;first1!=last1;)


	}
	template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
	__forceinline bool is_permutation(ForwardIt1 first1, ForwardIt1 last1,
									ForwardIt2 first2, ForwardIt2 last2,
									BinaryPredicate p)
	{
		
	}
	template<typename ForwardIterator1, typename ForwardIterator2>
	__forceinline bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2)
	{
		return equal(first1, last1, first2, [](const typename iterator_traits<ForwardIterator1>::value_type& x,
			const typename iterator_traits<ForwardIterator2>::value_type& y) {return x == y; });
	}
	template< class ForwardIt1, class ForwardIt2 >
	__forceinline bool is_permutation(ForwardIt1 first1, ForwardIt1 last1,
		ForwardIt2 first2, ForwardIt2 last2)
	{
		return equal(first1, last1, first2, last2, [](const typename iterator_traits<ForwardIt1>::value_type& x,
			const typename iterator_traits<ForwardIt2>::value_type& y) {return x == y; });
	}


	template<typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 search(
		ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2);
	template< class ForwardIt1, class ForwardIt2 >
	bool is_permutation(ForwardIt1 first1, ForwardIt1 last1,
		ForwardIt2 first2, ForwardIt2 last2);
	template<typename ForwardIterator1, typename ForwardIterator2,
		typename BinaryPredicate>
		ForwardIterator1 search(
			ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2,
			BinaryPredicate pred);

	template<typename ForwardIterator, typename Size, typename T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value);
	template<typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value,
		BinaryPredicate pred);

	// modifying sequence operations:

	// copy:
	template<typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last,
		OutputIterator result);
	template<typename InputIterator, typename Size, typename OutputIterator>
	OutputIterator copy_n(InputIterator first, Size n,
		OutputIterator result);
	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred);
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(
		BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result);

	// move:
	template<typename InputIterator, typename OutputIterator>
	OutputIterator move(InputIterator first, InputIterator last,
		OutputIterator result);
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 move_backward(
		BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result);

	// swap:
	template<typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1,
		ForwardIterator1 last1, ForwardIterator2 first2);
	template<typename ForwardIterator1, typename ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b);
	template<typename InputIterator, typename OutputIterator, typename UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last,
		OutputIterator result, UnaryOperation op);

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename BinaryOperation>
		OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, OutputIterator result,
			BinaryOperation binary_op);

	template<typename ForwardIterator, typename T>
	void replace(ForwardIterator first, ForwardIterator last,
		const T& old_value, const T& new_value);
	template<typename ForwardIterator, typename Predicate, typename T>
	void replace_if(ForwardIterator first, ForwardIterator last,
		Predicate pred, const T& new_value);
	template<typename InputIterator, typename OutputIterator, typename T>
	OutputIterator replace_copy(InputIterator first, InputIterator last,
		OutputIterator result,
		const T& old_value, const T& new_value);
	template<typename InputIterator, typename OutputIterator, typename Predicate, typename T>
	OutputIterator replace_copy_if(InputIterator first, InputIterator last,
		OutputIterator result,
		Predicate pred, const T& new_value);

	template<typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value);
	template<typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value);
	template<typename ForwardIterator, typename Generator>
	void generate(ForwardIterator first, ForwardIterator last,
		Generator gen);
	template<typename OutputIterator, typename Size, typename Generator>
	OutputIterator generate_n(OutputIterator first, Size n, Generator gen);

	template<typename ForwardIterator, typename T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<typename ForwardIterator, typename Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
		Predicate pred);
	template<typename InputIterator, typename OutputIterator, typename T>
	OutputIterator remove_copy(InputIterator first, InputIterator last,
		OutputIterator result, const T& value);
	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred);

	template<typename ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last);
	template<typename ForwardIterator, typename BinaryPredicate>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last,
		BinaryPredicate pred);
	template<typename InputIterator, typename OutputIterator>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result);
	template<typename InputIterator, typename OutputIterator, typename BinaryPredicate>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, BinaryPredicate pred);

	template<typename BidirectionalIterator>
	void reverse(BidirectionalIterator first, BidirectionalIterator last);
	template<typename BidirectionalIterator, typename OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator first,
		BidirectionalIterator last,
		OutputIterator result);

	template<typename ForwardIterator>
	ForwardIterator rotate(ForwardIterator first, ForwardIterator middle,
		ForwardIterator last);
	template<typename ForwardIterator, typename OutputIterator>
	OutputIterator rotate_copy(
		ForwardIterator first, ForwardIterator middle,
		ForwardIterator last, OutputIterator result);

	template<typename RandomAccessIterator>
	void random_shuffle(RandomAccessIterator first,
		RandomAccessIterator last);
	template<typename RandomAccessIterator, typename RandomNumberGenerator>
	void random_shuffle(RandomAccessIterator first,
		RandomAccessIterator last,
		RandomNumberGenerator&& rand);
	template<typename RandomAccessIterator, typename UniformRandomNumberGenerator>
	void shuffle(RandomAccessIterator first,
		RandomAccessIterator last,
		UniformRandomNumberGenerator&& rand);

	// partitions:
	template <typename InputIterator, typename Predicate>
	bool is_partitioned(InputIterator first, InputIterator last, Predicate pred);

	template<typename ForwardIterator, typename Predicate>
	ForwardIterator partition(ForwardIterator first,
		ForwardIterator last,
		Predicate pred);

	template<typename BidirectionalIterator, typename Predicate>
	BidirectionalIterator stable_partition(BidirectionalIterator first,
		BidirectionalIterator last,
		Predicate pred);

	template <typename InputIterator, typename OutputIterator1,
		typename OutputIterator2, typename Predicate>
		pair<OutputIterator1, OutputIterator2>
		partition_copy(InputIterator first, InputIterator last,
			OutputIterator1 out_true, OutputIterator2 out_false,
			Predicate pred);

	template<typename ForwardIterator, typename Predicate>
	ForwardIterator partition_point(ForwardIterator first,
		ForwardIterator last,
		Predicate pred);

	// sorting and related operations:

	// sorting:
	template<typename RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void sort(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);

	template<typename RandomAccessIterator>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);

	template<typename RandomAccessIterator>
	void partial_sort(RandomAccessIterator first,
		RandomAccessIterator middle,
		RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void partial_sort(RandomAccessIterator first,
		RandomAccessIterator middle,
		RandomAccessIterator last, Compare comp);
	template<typename InputIterator, typename RandomAccessIterator>
	RandomAccessIterator partial_sort_copy(
		InputIterator first, InputIterator last,
		RandomAccessIterator result_first,
		RandomAccessIterator result_last);
	template<typename InputIterator, typename RandomAccessIterator, typename Compare>
	RandomAccessIterator partial_sort_copy(
		InputIterator first, InputIterator last,
		RandomAccessIterator result_first,
		RandomAccessIterator result_last,
		Compare comp);

	template<typename ForwardIterator>
	bool is_sorted(ForwardIterator first, ForwardIterator last);
	template<typename ForwardIterator, typename Compare>
	bool is_sorted(ForwardIterator first, ForwardIterator last,
		Compare comp);
	template<typename ForwardIterator>
	ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last);
	template<typename ForwardIterator, typename Compare>
	ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last,
		Compare comp);

	template<typename RandomAccessIterator>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last, Compare comp);
	// binary search:
	template<typename ForwardIterator, typename T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<typename ForwardIterator, typename T, typename Compare>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);

	template<typename ForwardIterator, typename T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<typename ForwardIterator, typename T, typename Compare>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);

	template<typename ForwardIterator, typename T>
	pair<ForwardIterator, ForwardIterator>
		equal_range(ForwardIterator first, ForwardIterator last,
			const T& value);
	template<typename ForwardIterator, typename T, typename Compare>
	pair<ForwardIterator, ForwardIterator>
		equal_range(ForwardIterator first, ForwardIterator last,
			const T& value, Compare comp);

	template<typename ForwardIterator, typename T>
	bool binary_search(ForwardIterator first, ForwardIterator last,
		const T& value);
	template<typename ForwardIterator, typename T, typename Compare>
	bool binary_search(ForwardIterator first, ForwardIterator last,
		const T& value, Compare comp);

	// merge:
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp);

	template<typename BidirectionalIterator>
	void inplace_merge(BidirectionalIterator first,
		BidirectionalIterator middle,
		BidirectionalIterator last);
	template<typename BidirectionalIterator, typename Compare>
	void inplace_merge(BidirectionalIterator first,
		BidirectionalIterator middle,
		BidirectionalIterator last, Compare comp);

	// set operations:
	template<typename InputIterator1, typename InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2);
	template<typename InputIterator1, typename InputIterator2, typename Compare>
	bool includes(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, Compare comp);

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp);

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_intersection(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator set_intersection(
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp);

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator set_difference(
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp);

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_symmetric_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result);
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator set_symmetric_difference(
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp);

	// heap operations:
	template<typename RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	template<typename RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);

	template<typename RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);

	template<typename RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);

	template<typename RandomAccessIterator>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp);
	template<typename RandomAccessIterator>
	RandomAccessIterator is_heap_until(RandomAccessIterator first, RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	RandomAccessIterator is_heap_until(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp);
	// minimum and maximum:
	template<typename T> const T& min(const T& a, const T& b);
	template<typename T, typename Compare>
	const T& min(const T& a, const T& b, Compare comp);
	template<typename T>
	T min(std::initializer_list<T> t);
	template<typename T, typename Compare>
	T min(std::initializer_list<T> t, Compare comp);

	template<typename T> const T& max(const T& a, const T& b);
	template<typename T, typename Compare>
	const T& max(const T& a, const T& b, Compare comp);
	template<typename T>
	T max(std::initializer_list<T> t);
	template<typename T, typename Compare>
	T max(std::initializer_list<T> t, Compare comp);

	template<typename T> pair<const T&, const T&> minmax(const T& a, const T& b);
	template<typename T, typename Compare>
	pair<const T&, const T&> minmax(const T& a, const T& b, Compare comp);
	template<typename T>
	pair<T, T> minmax(std::initializer_list<T> t);
	template<typename T, typename Compare>
	pair<T, T> minmax(std::initializer_list<T> t, Compare comp);

	template<typename ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last);
	template<typename ForwardIterator, typename Compare>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
		Compare comp);

	template<typename ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last);
	template<typename ForwardIterator, typename Compare>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
		Compare comp);

	template<typename ForwardIterator>
	pair<ForwardIterator, ForwardIterator>
		minmax_element(ForwardIterator first, ForwardIterator last);
	template<typename ForwardIterator, typename Compare>
	pair<ForwardIterator, ForwardIterator>
		minmax_element(ForwardIterator first, ForwardIterator last, Compare comp);

	template<typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2);
	template<typename InputIterator1, typename InputIterator2, typename Compare>
	bool lexicographical_compare(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		Compare comp);

	// permutations:
	template<typename BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first,
		BidirectionalIterator last);
	template<typename BidirectionalIterator, typename Compare>
	bool next_permutation(BidirectionalIterator first,
		BidirectionalIterator last, Compare comp);

	template<typename BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first,
		BidirectionalIterator last);
	template<typename BidirectionalIterator, typename Compare>
	bool prev_permutation(BidirectionalIterator first,
		BidirectionalIterator last, Compare comp);
}