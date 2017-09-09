#pragma once
#include"container.h"
#include"pair.h"
#include"iterator.h"
#include<random>
//#include<execution>
namespace
{
	template<typename T>
	struct is_execution_policy_v;

	template<typename T1,typename T2,typename = decltype((std::declval<T1>()) == (std::declval<T2>()))>
	class _equal
	{
		bool operator()(const T1& a,const T2& b)const noexcept(noexcept(a==b))
		{
			return a == b;
		}
	};
	template<typename T>
	constexpr bool is_random_access_iterator = std::is_base_of_v<uuz::random_access_iterator_tag, std::decay_t<T>>;
	template<typename T>
	constexpr bool is_forward_iterator = std::is_base_of_v<uuz::forward_iterator_tag, std::decay_t<T>>;
	template<typename T>
	constexpr bool is_bidirectional_iterator = std::is_base_of_v<uuz::bidirectional_iterator_tag, std::decay_t<T>>;
	template<typename T>
	constexpr bool is_output_iterator = std::is_base_of_v<uuz::output_iterator_tag, std::decay_t<T>>;
	template<typename T>
	constexpr bool is_input_iterator = std::is_base_of_v<uuz::input_iterator_tag, std::decay_t<T>>;
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
		ForwardIt find_if(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt find_if_not(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate q);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt1 find_end(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>> >
		ForwardIt1 find_end(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt1 find_first_of(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt1 find_first_of(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last, ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt adjacent_find(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last);
		template< class ExecutionPolicy, class ForwardIt, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		ForwardIt adjacent_find(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt, class T, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		typename iterator_traits<ForwardIt>::difference_type
			count(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, const T &value);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		typename iterator_traits<ForwardIt>::difference_type
			count_if(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2, BinaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2, ForwardIt2 last2);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate, typename = is_execution_policy_v<std::decay_t<ExecutionPolicy>>>
		std::pair<ForwardIt1, ForwardIt2> mismatch(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2, ForwardIt2 last2, BinaryPredicate p);

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

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		ForwardIt1 search(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 s_first, ForwardIt2 s_last);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate >
		ForwardIt1 search(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p);


		template< class ExecutionPolicy, class ForwardIt, class Size, class T >
		ForwardIt search_n(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, Size count, const T& value);
		template< class ExecutionPolicy, class ForwardIt, class Size, class T, class BinaryPredicate >
		ForwardIt search_n(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, Size count, const T& value,
			BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		ForwardIt2 copy(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class UnaryPredicate >
		ForwardIt2 copy_if(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 d_first,
			UnaryPredicate pred);
		template< class ExecutionPolicy, class ForwardIt1, class Size, class ForwardIt2 >
		ForwardIt2 copy_n(ExecutionPolicy&& policy, ForwardIt1 first, Size count, ForwardIt2 result);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		ForwardIt2 move(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		ForwardIt2 swap_ranges(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class UnaryOperation >
		ForwardIt2 transform(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 d_first, UnaryOperation unary_op);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class ForwardIt3, class BinaryOperation >
		ForwardIt3 transform(ExecutionPolicy&& policy, ForwardIt1 first1, ForwardIt1 last1,
			ForwardIt2 first2, ForwardIt3 d_first, BinaryOperation binary_op);

		template< class ExecutionPolicy, class ForwardIt, class T >
		void replace(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last,
			const T& old_value, const T& new_value);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate, class T >
		void replace_if(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last,
			UnaryPredicate p, const T& new_value);

		template< class ExecutionPolicy, class ForwardIt, class T >
		void fill(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, const T& value);
		template< class ExecutionPolicy, class ForwardIt, class Size, class T >
		ForwardIt fill_n(ExecutionPolicy&& policy, ForwardIt first, Size count, const T& value);

		template< class ExecutionPolicy, class ForwardIt, class Generator >
		void generate(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, Generator g);
		template< class ExecutionPolicy, class ForwardIt, class Size, class Generator >
		ForwardIt generate_n(ExecutionPolicy&& policy, ForwardIt first, Size count, Generator g);

		template< class ExecutionPolicy, class ForwardIt, class T >
		ForwardIt remove(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, const T& value);
		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate >
		ForwardIt remove_if(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class T >
		ForwardIt2 remove_copy(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 d_first, const T& value);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class UnaryPredicate >
		ForwardIt2 remove_copy_if(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 d_first, UnaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt >
		ForwardIt unique(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last);
		template< class ExecutionPolicy, class ForwardIt, class BinaryPredicate >
		ForwardIt unique(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, BinaryPredicate p);

		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2 >
		ForwardIt2 unique_copy(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 d_first);
		template< class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate >
		ForwardIt2 unique_copy(ExecutionPolicy&& policy, ForwardIt1 first, ForwardIt1 last,
			ForwardIt2 d_first, BinaryPredicate p);

		template< class ExecutionPolicy, class BidirIt >
		void reverse(ExecutionPolicy&& policy, BidirIt first, BidirIt last);
		template< class ExecutionPolicy, class BidirIt, class ForwardIt >
		ForwardIt reverse_copy(ExecutionPolicy&& policy, BidirIt first, BidirIt last, ForwardIt d_first);

		template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate >
		bool is_partitioned(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p);
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

	template<typename InputIterator, typename T, typename = decltype((*std::decay<InputIterator>()) == std::decay<T>())>
	inline InputIterator find(InputIterator first, InputIterator last, const T& value)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (*first == value)
				return first;
		return last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	inline InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (pred(*first))
				return first;
		return last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	inline InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");
		for (; first != last; ++first)
			if (!pred(*first))
				return first;
		return last;
	}


	template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
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
		else if constexpr(std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
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

			for (;;)
			{
				for (;; ++first1)
				{
					if (first1 == last1)
						return answer;
					if (pred(*first1, *first2))
						break;
				}
				auto kk = first2;
				for (auto l = first1;;)
				{
					if (++kk == last2)
					{
						answer = first1;
						++first1;
						break;
					}
					if (++l == last1)
						return answer;
					if (!pred(*l, *kk))
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
			_equal< typename iterator_traits<ForwardIterator1>::value_type,
			typename iterator_traits<ForwardIterator2>::value_type>());
	}

	template<typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
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
			_equal< typename iterator_traits<InputIterator>::value_type,
			typename iterator_traits<ForwardIterator>::value_type>());
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

		for (; next != last; first = next, (void)++next)
			if (pred(*first, *next))
				return first;

		return last;
	}

	template<typename ForwardIterator>
	inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last)
	{
		return adjacent_find(first, last,
			_equal< typename iterator_traits<ForwardIterator>::value_type,
			typename iterator_traits<ForwardIterator>::value_type>());
	}

	template<typename InputIterator, typename Predicate>
	inline typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator first, InputIterator last, Predicate pred)
	{
		typename iterator_traits<InputIterator>::difference_type ss{ 0 };

		for (; first != last; ++first)
			if (pred(*first))
				++ss;
		return ss;
	}
	template<typename InputIterator, typename T>
	inline typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& value)
	{
		typename iterator_traits<InputIterator>::difference_type ss{ 0 };

		for (; first != last; ++first)
			if (*first == value)
				++ss;
		return ss;
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
			_equal< typename iterator_traits<InputIterator1>::value_type,
			typename iterator_traits<InputIterator2>::value_type>());
	}
	template< class InputIt1, class InputIt2 >
	inline pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2)
	{
		return mismatch(first1, last1, first2, last2,
			_equal< typename iterator_traits<InputIt1>::value_type,
			typename iterator_traits<InputIt2>::value_type>());
	}

	template< class InputIt1, class InputIt2, class BinaryPredicate >
	__forceinline bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, BinaryPredicate p)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt1>::iterator_category>
			, "iterator must be a input_iterator");
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIt2>::iterator_category>
			, "iterator must be a input_iterator");

		for (; first1 != last1; ++first1, (void)++first2)
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
		return equal(first1, last1, first2, _equal< typename iterator_traits<InputIt1>::value_type,
			typename iterator_traits<InputIt2>::value_type>());
	}
	template< class InputIt1, class InputIt2 >
	__forceinline bool equal(InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2)
	{
		return equal(first1, last1, first2, last2, _equal< typename iterator_traits<InputIt1>::value_type,
			typename iterator_traits<InputIt2>::value_type>());
	}

	template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
	__forceinline bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
			, "iterator must be a forward_iterator");
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>
			, "iterator must be a forward_iterator");

		for (; first1 != last1; ++first1, (void)++first2)
			if (!pred(first1, first2))
				goto keke;
		return true;

	keke:	auto k = distance(first1, last1);
		if (k == 1)
			return false;
		auto last2 = next(first2, k);

		for (auto i = first1; i != last1; ++i)
		{
			for (auto j = first1; j != i; ++j)
				if (pred(*j, *i))
					goto llll;

			decltype(k) dis = 1;
			for (auto j = next(i); j != last1; ++j)
				if (pred(*i, *j))
					++dis;
			for (auto j = first2; j != last2; ++j)
				if (pred(*i, *j) && --dis < 0)
					return false;
			if (dis > 0)
				return false;
		llll:;
		}
		return true;
	}
	template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
	__forceinline bool is_permutation(ForwardIt1 first1, ForwardIt1 last1,
		ForwardIt2 first2, ForwardIt2 last2,
		BinaryPredicate p)
	{
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIt1>::iterator_category>
			, "iterator must be a forward_iterator");
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIt2>::iterator_category>
			, "iterator must be a forward_iterator");

		if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIt1>::iterator_category>
			&& std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIt2>::iterator_category>)
		{
			if (distance(first1, last1) != distance(first2, last2))
				return false;
		}

		for (; first1 != last1 && first2 != last2; ++first1, (void)++first2)
			if (!pred(first1, first2))
				goto keke;
		return first1 == last1 && first2 == last2;

	keke:	auto k = distance(first1, last1);
		auto k2 = distanc(first2, last2);
		if (k != k2 || k == 1)
			return false;

		for (auto i = first1; i != last1; ++i)
		{
			for (auto j = first1; j != i; ++j)
				if (pred(*j, *i))
					goto llll;

			decltype(k) dis = 1;
			for (auto j = next(i); j != last1; ++j)
				if (pred(*i, *j))
					++dis;
			for (auto j = first2; j != last2; ++j)
				if (pred(*i, *j) && --dis < 0)
					return false;
			if (dis > 0)
				return false;
		llll:;
		}
		return true;

	}
	template<typename ForwardIterator1, typename ForwardIterator2>
	__forceinline bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2)
	{
		return equal(first1, last1, first2, _equal< typename iterator_traits<ForwardIterator1>::value_type,
			typename iterator_traits<ForwardIterator2>::value_type>());
	}
	template< class ForwardIt1, class ForwardIt2 >
	__forceinline bool is_permutation(ForwardIt1 first1, ForwardIt1 last1,
		ForwardIt2 first2, ForwardIt2 last2)
	{
		return equal(first1, last1, first2, last2, _equal< typename iterator_traits<ForwardIt1>::value_type,
			typename iterator_traits<ForwardIt2>::value_type>());
	}

	template<typename ForwardIterator1, typename ForwardIterator2,
		typename BinaryPredicate>
		ForwardIterator1 search(
			ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2, ForwardIterator2 last2,
			BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
			, "iterator must be a forward_iterator");
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>
			, "iterator must be a forward_iterator");

		if (first1 == last1 || first2 == last2)
			return first1;

		if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIterator1>::iterator_category>
			&& std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIterator2>::iterator_category>)
		{
			auto c1 = distance(first1, last1);
			auto c2 = distance(first2, last2);
			if (c2 > c1)
				return last1;

			auto s = last1 - (c2 - 1);

			for (;;)
			{
				for (;;)
				{
					if (first1 == s)
						return last1;
					if (pred(*first1, *first2))
						break;
					++first1;
				}
				auto t1 = first1;
				auto t2 = first2;
				for (;;)
				{
					if (++t2 == last2)
						return first1;
					if (!pred(*++t1, *t2))
					{
						++first1;
						break;
					}

				}
			}
		}
		else
		{
			for (;;)
			{
				for (;;)
				{
					if (first1 == last1)
						return last1;
					if (pred(*first1, *first2))
						break;
					++first1;
				}
				auto t1 = first1;
				auto t2 = first2;
				for (;;)
				{
					if (++t2 == last2)
						return first1;
					if (++t1 == last1)
						return last1;
					if (!pred(*t1, *t2))
					{
						++first1;
						break;
					}

				}
			}
		}

	}
	template<typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2)
	{
		return search(first1, last1, first2, last2, _equal< typename iterator_traits<ForwardIterator1>::value_type,
			typename iterator_traits<ForwardIterator2>::value_type>());

	}
	template<class ForwardIterator, class Searcher>
	ForwardIterator search(ForwardIterator first, ForwardIterator last,
		const Searcher& searcher);

	template<typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value,
		BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<ForwardIterator>::iterator_category>
			, "iterator must be a forward_iterator");

		if (count <= 0)
			return first;
		if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<ForwardIterator>::iterator_category>)
		{
			auto ss = distance(first, last);
			if (ss < count)
				return last;

		}
		else
		{
			auto count1 = 0;
			for (;;)
			{
				for (;;)
				{
					if (first == last)
						return last;
					if (pred(*first, value))
						break;
					++first;
				}
			}
			decltype(distance(first, last)) s{ 0 };
			for (auto kk = first;;)
			{
				if (++s == count)
					return first;
				if (++kk == last)
					return last;
				if (!pred(*kk, value))
				{
					first = ++kk;
					break;
				}
			}
		}
	}
	template<typename ForwardIterator, typename Size, typename T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
		Size count, const T& value)
	{
		return search_n(first, last, count, value, _equal< typename iterator_traits<ForwardIterator>::value_type,
			T>());
	}


	// modifying sequence operations:

	// copy:
	template<typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		using T1 = typename iterator_traits<InputIterator>::value_type;
		using T2 = typename iterator_traits<OutputIterator>::value_type;
		if constexpr(std::is_pointer_v<InputIterator> && std::is_pointer_v<OutputIterator>
			&&  std::is_same_v<typename std::decay_t<T1>, typename std::decay_t<T2>>
			&& std::is_trivially_copy_assignable_v<std::decay_t<T2>>)
		{
			auto k = last - first;
			if (k > 0)
				memmove(result, first, k * sizeof(T1));
			return result + k;
		}
		else
		{
			for (; first != last; ++result, (void)++first)
				*result = *first;
			return result;
		}
	}
	template<typename InputIterator, typename Size, typename OutputIterator>
	OutputIterator copy_n(InputIterator first, Size n, OutputIterator result)
	{
		using T1 = typename iterator_traits<InputIterator>::value_type;
		using T2 = typename iterator_traits<OutputIterator>::value_type;
		if (n == 0)
			return result;
		if constexpr(std::is_pointer_v<InputIterator> && std::is_pointer_v<OutputIterator>
			&&  std::is_same_v<typename std::decay_t<T1>, typename std::decay_t<T2>>
			&& std::is_trivially_copy_assignable_v<std::decay_t<T2>>)
		{
			memmove(result, first, n * sizeof(T1));
			return result + n;
		}
		else if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			&& std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<OutputIterator>::iterator_category>)
		{
			auto last = first + n;
			for (; first != last; ++result, (void)++first)
				*result = *first;
			return result;
		}
		else
		{
			*result = *first;
			++result;
			for (--n; n > 0; --n)
			{
				++first;
				*result = *first;
				++result;
			}
			return result;
		}


	}
	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred)
	{
		while (first != last)
			if (pred(*first))
				*result++ = *first++;
		return result;
	}
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		static_assert(std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<BidirectionalIterator1>::iterator_category>
			, "iterator must be a bidirectional_iterator");
		static_assert(std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<BidirectionalIterator2>::iterator_category>
			, "iterator must be a bidirectional_iterator");

		using T1 = typename iterator_traits<BidirectionalIterator1>::value_type;
		using T2 = typename iterator_traits<BidirectionalIterator2>::value_type;
		if constexpr(std::is_pointer_v<BidirectionalIterator2> && std::is_pointer_v<BidirectionalIterator1>
			&&  std::is_same_v<typename std::decay_t<T1>, typename std::decay_t<T2>>
			&& std::is_trivially_copy_assignable_v<std::decay_t<T2>>)
		{
			auto k = last - first;
			result -= k;
			if (k > 0)
				memmove(result, first, k * sizeof(T1));
			return result;
		}
		else
		{
			while (first != last)
				*--result = *--last;
			return result;
		}


	}

	// move:
	template<typename InputIterator, typename OutputIterator>
	OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
	{
		using T1 = typename iterator_traits<InputIterator>::value_type;
		using T2 = typename iterator_traits<OutputIterator>::value_type;

		if constexpr(std::is_pointer_v<InputIterator> && std::is_pointer_v<InputIterator>
			&&  std::is_same_v<typename std::decay_t<T1>, typename std::decay_t<T2>>
			&& std::is_trivially_copy_assignable_v<std::decay_t<T2>>)
		{
			auto k = last - first;
			if (k > 0)
				memmove(result, first, k * sizeof(T1));
			return result + k;
		}
		else
		{
			for (; first != last; ++first, (void) ++result)
				*result = std::move(*first);
			return result;
		}

	}
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	BidirectionalIterator2 move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		static_assert(std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<BidirectionalIterator1>::iterator_category>
			, "iterator must be a bidirectional_iterator");
		static_assert(std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<BidirectionalIterator2>::iterator_category>
			, "iterator must be a bidirectional_iterator");

		using T1 = typename iterator_traits<BidirectionalIterator1>::value_type;
		using T2 = typename iterator_traits<BidirectionalIterator2>::value_type;
		if constexpr(std::is_pointer_v<BidirectionalIterator2> && std::is_pointer_v<BidirectionalIterator1>
			&&  std::is_same_v<typename std::decay_t<T1>, typename std::decay_t<T2>>
			&& std::is_trivially_copy_assignable_v<std::decay_t<T2>>)
		{
			auto k = last - first;
			result -= k;
			if (k > 0)
				memmove(result, first, k * sizeof(T1));
			return result;
		}
		else
		{
			while (first != last)
				*--result = std::move(*--last);
			return result;
		}

	}

	// swap:
	template<typename ForwardIterator1, typename ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		using std::swap;
		swap(*a, *b);
	}
	template<typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 first1,
		ForwardIterator1 last1, ForwardIterator2 first2)
	{
		while (first1 != last1)
			std::swap(*first1++, *first2++);
		return first2;
	}

	template<typename InputIterator, typename OutputIterator, typename UnaryOperation>
	OutputIterator transform(InputIterator first, InputIterator last,
		OutputIterator result, UnaryOperation op)
	{
		while (first != last)
			*result++ = op(*first++);
		return result;
	}

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename BinaryOperation>
		OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, OutputIterator result,
			BinaryOperation binary_op)
	{
		while (first1 != last1)
			*result++ = binary_op(*first1++, *first2++);
		return result;
	}

	template<typename ForwardIterator, typename T>
	void replace(ForwardIterator first, ForwardIterator last,
		const T& old_value, const T& new_value)
	{
		for (; first != last; ++first)
			if (*first == old_value)
				*first = new_value;
	}
	template<typename ForwardIterator, typename Predicate, typename T>
	void replace_if(ForwardIterator first, ForwardIterator last,
		Predicate pred, const T& new_value)
	{
		for (; first != last; ++first)
			if (pred(*first))
				*first = new_value;
	}
	template<typename InputIterator, typename OutputIterator, typename T>
	OutputIterator replace_copy(InputIterator first, InputIterator last,
		OutputIterator result, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first, (void)++result)
			if (*first == old_value)
				*result = new_value;
			else
				*result = *first;
		return result;
	}
	template<typename InputIterator, typename OutputIterator, typename Predicate, typename T>
	OutputIterator replace_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred, const T& new_value)
	{
		for (; first != last; ++first, (void)++result)
			if (pred(*first))
				*result = new_value;
			else
				*result = *first;
		return result;
	}


	template<typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		using T1 = typename iterator_traits<OutputIterator>::value_type;
		if (n <= 0)
			return first;
		if constexpr(std::is_pointer_v<OutputIterator> &&
			is_integral_v<T1> && sizeof(T1) == 1 && !is_same_v<T, bool> &&is_integral_v<T> && sizeof(T) == 1)
		{
			::memset(first, (unsigned char)value, (size_t)n);
			return first + n;
		}
		else
		{
			for (; n != 0; --n, (void)++first)
				*first = value;
			return first;
		}
	}
	template<typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		using T1 = typename iterator_traits<ForwardIterator>::value_type;

		if constexpr(std::is_pointer_v<ForwardIterator> &&
			is_integral_v<T1> && sizeof(T1) == 1 && !is_same_v<T, bool> &&is_integral_v<T> && sizeof(T) == 1)
		{
			auto n = last - first;
			if (n > 0)
				::memset(first, (unsigned char)value, (size_t)n);
		}
		else
		{
			for (; first != last; ++first)
				*first = value;
		}
	}

	template<typename ForwardIterator, typename Generator>
	void generate(ForwardIterator first, ForwardIterator last,
		Generator gen)
	{
		for (; first != last; ++first)
			*first = gen();
	}
	template<typename OutputIterator, typename Size, typename Generator>
	OutputIterator generate_n(OutputIterator first, Size n, Generator gen)
	{
		auto t{ n };
		for (; n > 0; ++first, (void)--n)
			*first = gen();
		return first;
	}

	template<typename ForwardIterator, typename T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last,
							const T& value)
	{
		first = find(first, last, value);
		if(first!=last)
		{
			auto i = first;
			while(++i!=last)
				*first++ = std::move(*i);
		}
		return first;
	}
	template<typename ForwardIterator, typename Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
		Predicate pred)
	{
		first = find(first, last, pred);
		if (first != last)
		{
			auto i = first;
			while (++i != last)
				*first++ = std::move(*i);
		}
		return first;
	}
	template<typename InputIterator, typename OutputIterator, typename T>
	OutputIterator remove_copy(InputIterator first, InputIterator last,
		OutputIterator result, const T& value)
	{
		while(first!=last)
		{
			if (*first != value)
				*result++ = *first;
			++first;
		}
		return result;
	}
	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last,
		OutputIterator result, Predicate pred)
	{
		while (first != last)
		{
			if (!pred(*first))
				*result++ = *first;
			++first;
		}
		return result;
	}

	template<typename ForwardIterator, typename BinaryPredicate>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last,
		BinaryPredicate pred)
	{
		first = adjacent_find(first, last, pred);
		if (first != last)
		{
			auto i = first;
			for (++i; i != last; ++i)
				if (!pred(*first, *i))
					*++first = std::move(*i);
			++first;
		}
		return first;
	}
	template<typename ForwardIterator>
	ForwardIterator unique(ForwardIterator first, ForwardIterator last)
	{
		first = adjacent_find(first, last);
		if (first != last)
		{
			auto i = first;
			for (++i; i != last; ++i)
				if (*first != *i)
					*++first = std::move(*i);
			++first;
		}
		return first;
	}
	
	template<typename InputIterator, typename OutputIterator, typename BinaryPredicate>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
		OutputIterator result, BinaryPredicate pred)
	{
		static_assert(std::is_base_of_v<input_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>
			, "iterator must be a input_iterator");

		if (first != last)
		{
			if constexpr(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<OutputIterator>::iterator_category>)
			{
				*result = *first;
				while(++first != last)
					if (!pred(*result, *first))
						*++result = *first;
				++result;
			}
			else if constexpr(std::is_same_v<output_iterator_tag, typename iterator_traits<OutputIterator>::iterator_category>)
			{
				if constexpr(std::is_base_of_v<forward_iterator_tag, typename iterator_traits<InputIterator>::iterator_category>)
				{
					auto i = first;
					*result = *i;
					++result;
					while(++first != last)
					{
						if(!pred(*i,*first))
						{
							*result = *first;
							++result;
							i = first;
						}
					}
				}
				else
				{
					auto k(*first);
					*result = k;
					++result;
					while (++first != last)
					{
						if (!pred(k, *first))
						{
							k = *first;
							*result = k;
							++result;
						}
					}
				}
			}
			else
			{
				static_assert(false);
			}
		}
		return result;
	}
	template<typename InputIterator, typename OutputIterator>
	OutputIterator unique_copy(InputIterator first, InputIterator last,
								OutputIterator result)
	{
		return unique_copy(first, last, result, _equal<typename iterator_traits<InputIterator>::value_type,
														typename iterator_traits<InputIterator>::value_type>());
	}
	

	template<typename BidirectionalIterator>
	void reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		static_assert(std::is_base_of_v<bidirectional_iterator_tag, typename iterator_traits<BidirectionalIterator>::iterator_category>
			, "iterator must be a bidirectional_iterator");
		if(first!=last)
		{
			if constexpr(std::is_base_of_v<random_access_iterator_tag, typename iterator_traits<BidirectionalIterator>::iterator_category>)
			{
				for (;first < --last; ++first)
					iter_swap(first, last);
			}
			else
			{
				while(first != --last)
				{
					iter_swap(first, last);
					++first;
				}
			}
		}

	}
	template<typename BidirectionalIterator, typename OutputIterator>
	OutputIterator reverse_copy(BidirectionalIterator first,
							BidirectionalIterator last,
							OutputIterator result)
	{
		for (; first != last; ++result)
			*result = *--last;
		return result;
	}


	//ÓÅ»¯¾¯¸æ http://www.cnblogs.com/flyinghearts/archive/2011/05/27/2060265.html
	template<typename ForwardIterator>
	ForwardIterator rotate(ForwardIterator first, ForwardIterator middle,
						ForwardIterator last)
	{
		
	}
	template<typename ForwardIterator, typename OutputIterator>
	OutputIterator rotate_copy(
					ForwardIterator first, ForwardIterator middle,
					ForwardIterator last, OutputIterator result)
	{
		return copy(first, middle, copy(middle, last, result));
	}



	template<typename RandomAccessIterator, typename UniformRandomNumberGenerator>
	void shuffle(RandomAccessIterator first,RandomAccessIterator last,
					UniformRandomNumberGenerator&& rand)
	{

		using dif = typename iterator_traits<RandomAccessIterator>::difference_type;
		using ddp = std::uniform_int_distribution<dif>;

		ddp D;
		using std::swap;
		for (auto i = (last - first) - 1; i > 0; --i)
			swap(first[i], first[D(rand, ddp::param_t(0, i))]);
	}

	// partitions:
	template <typename InputIterator, typename Predicate>
	bool is_partitioned(InputIterator first, InputIterator last, Predicate pred)
	{
		for (; first != last; ++first)
			if (!pred(*first))
				break;
		for (; first != last; ++first)
			if (pred(*first))
				return false;
		return true;
	}

	template<typename ForwardIterator, typename Predicate>
	ForwardIterator partition(ForwardIterator first,ForwardIterator last,
								Predicate pred)
	{
		static_assert(!is_forward_iterator<ForwardIterator>, "!!!");
		
		using std::swap;

		if constexpr(is_bidirectional_iterator<ForwardIterator>)
		{
			for (;;++first)
			{
				if (first == last)
					return last;
				if (!pred(*first))
				{
					do
					{
						if (first == --last)
							return last;
					} while (!pred(*last));
					iter_swap(first, last);
				}
			}
		}
		else
		{
			for (;; ++first)
			{
				if (first == last)
					return last;
				if (!pred(*first))
					break;
			}
			for (auto p = first; ++p != last;)
				if (pred(*p))
					swap(*first++, *p);
		}
		return first;
	}

	template<typename BidirectionalIterator, typename Predicate>
	BidirectionalIterator stable_partition(BidirectionalIterator first,BidirectionalIterator last,
										Predicate pred)
	{
		static_assert(!is_bidirectional_iterator<BidirectionalIterator>, "!!!");



	}

	template <typename InputIterator, typename OutputIterator1,
				typename OutputIterator2, typename Predicate>
	pair<OutputIterator1, OutputIterator2> partition_copy(InputIterator first, InputIterator last,
														OutputIterator1 out_true, OutputIterator2 out_false,
														Predicate pred)
	{
		for(;first!=last;++first)
		{
			if (pred(first))
				*out_true++ = *first;
			else
				*out_false++ = *first;
		}
		return pair<OutputIterator1, OutputIterator2>(out_true, out_false);
	}

	template<typename ForwardIterator, typename Predicate>
	ForwardIterator partition_point(ForwardIterator first,ForwardIterator last,Predicate pred)
	{
		
	}

	// sorting and related operations:

	// sorting:
	template<typename RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return sort(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	void sort(RandomAccessIterator first, RandomAccessIterator last,Compare comp)
	{
		
	}

	template<typename RandomAccessIterator, typename Compare>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);
	template<typename RandomAccessIterator>
	void stable_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return stable_sort(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}
	

	template<typename RandomAccessIterator, typename Compare>
	void partial_sort(RandomAccessIterator first,RandomAccessIterator middle,RandomAccessIterator last, Compare comp);
	template<typename RandomAccessIterator>
	void partial_sort(RandomAccessIterator first,RandomAccessIterator middle,RandomAccessIterator last)
	{
		return partial_sort(first, middle, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}
	
	template<typename InputIterator, typename RandomAccessIterator, typename Compare>
	RandomAccessIterator partial_sort_copy(
		InputIterator first, InputIterator last,
		RandomAccessIterator result_first,
		RandomAccessIterator result_last,
		Compare comp);
	template<typename InputIterator, typename RandomAccessIterator>
	RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,RandomAccessIterator result_first,
											RandomAccessIterator result_last)
	{
		return partial_sort(first, last, result_first, result_last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}
	

	template<typename ForwardIterator, typename Compare>
	bool is_sorted(ForwardIterator first, ForwardIterator last,Compare comp)
	{
		static_assert(!is_forward_iterator<ForwardIterator>);

		if(first!=last)
			for (auto old = first++; first != last; old = first, (void)++first)
				if (!pred(*old, *first))
					return false;
		return true;
	}
	template<typename ForwardIterator>
	bool is_sorted(ForwardIterator first, ForwardIterator last)
	{
		return is_sorted(first, last, less<typename iterator_traits<ForwardIterator>::value_type>());
	}
	template<typename ForwardIterator, typename Compare>
	ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last,Compare comp)
	{
		static_assert(!is_forward_iterator<ForwardIterator>);

		if (first != last)
			for (auto old = first++; first != last; old = first, (void)++first)
				if (!pred(*old, *first))
					return first;
		return last;
	}
	template<typename ForwardIterator>
	ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last)
	{
		return is_sorted_until(first, last, less<typename iterator_traits<ForwardIterator>::value_type>());
	}
	

	template<typename RandomAccessIterator>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last);
	template<typename RandomAccessIterator, typename Compare>
	void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
		RandomAccessIterator last, Compare comp);
	// binary search:
	template<typename ForwardIterator, typename T, typename Compare>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,const T& value, Compare comp)
	{
		static_assert(!is_forward_iterator<ForwardIterator>);

		auto k = distance(first, last);
		while(k)
		{
			auto l2 = k >> 1;
			auto m = first;
			advance(m, l2);
			if (comp(*m, value))
			{
				first = ++m;
				k -= l2 + 1;
			}
			else
				k = l2;
		}
		return first;
	}
	template<typename ForwardIterator, typename T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value)
	{
		return lower_bound(first, last, value, _equal<typename iterator_traits<ForwardIterator>::value_type, T>());
	}

	template<typename ForwardIterator, typename T, typename Compare>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,const T& value, Compare comp)
	{
		static_assert(!is_forward_iterator<ForwardIterator>);

		auto k = distance(first, last);
		while (k)
		{
			auto l2 = k >> 1;
			auto m = first;
			advance(m, l2);
			if (comp(value, *m))
				k = l2;
			else
			{
				first = m + 1;
				k -= l2 + 1;
			}
		}

		return first;
	}
	template<typename ForwardIterator, typename T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,const T& value)
	{
		return lower_bound(first, last, value, _equal<typename iterator_traits<ForwardIterator>::value_type, T>());
	}
	
	template<typename ForwardIterator, typename T, typename Compare>
	pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last,
														const T& value, Compare comp)
	{
		static_assert(!is_forward_iterator<ForwardIterator>);
		
		if(first!=last)
		{
			auto k = lower_bound(first, last, value, comp);
			auto t = last;
			if (k != last && !comp(value, *k))
				t = upper_bound(k, last, value, comp);

			return pair<ForwardIterator, ForwardIterator>(k, t);
		}
		
		return pair<ForwardIterator, ForwardIterator>(first, first);
	}
	template<typename ForwardIterator, typename T>
	pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last,const T& value)
	{
		return equal_range(first, last, value, _equal<typename iterator_traits<ForwardIterator>::value_type, T>());
	}
	

	template<typename ForwardIterator, typename T, typename Compare>
	bool binary_search(ForwardIterator first, ForwardIterator last,const T& value, Compare comp)
	{
		first = lower_bound(first, last, value, comp);
		return (!(first == last) && !(comp(value, *first)));
	}
	template<typename ForwardIterator, typename T>
	bool binary_search(ForwardIterator first, ForwardIterator last,const T& value)
	{
		return binary_search(first, last, value, _equal<typename iterator_traits<ForwardIterator>::value_type, T>());
	}
	

	// merge:
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,typename Compare>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, InputIterator2 last2,
						OutputIterator result, Compare comp)
	{
		for(;first1!=last1 && first2!=last2;++result)
		{
			if(comp(*first1,*first2))
			{
				*result = *first1;
				++first1;
			}
			else
			{
				*result = *first2;
				++first2;
			}
		}
		if (first1 != last1)
			return copy(first1, last1, result);
		if (first2 != last2)
			return copy(first2, last2, result);
		return result;
	}
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, InputIterator2 last2,
						OutputIterator result)
	{
		return merge(first1, last1, first2, last2, result, less<typename iterator_traits<InputIterator1>::value_type,
																typename iterator_traits<InputIterator2>::value_type>());
	}
	
	template<typename BidirectionalIterator, typename Compare>
	void inplace_merge(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last, Compare comp)
	{
		
	}
	template<typename BidirectionalIterator>
	void inplace_merge(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last)
	{
		return inplace_merge(first, middle, last, less<typename iterator_traits<BidirectionalIterator>::value_type,
														typename iterator_traits<BidirectionalIterator>::value_type>());
	}
	

	// set operations:
	template<typename InputIterator1, typename InputIterator2, typename Compare>
	bool includes(InputIterator1 first1, InputIterator1 last1,
				InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		for (; first2 != last2; ++first1)
			if (first1 == last1 || comp(*first1, *first2))
				return false;
			else if (!comp(*first2, *first1))
				++first2;
		return true;
	}
	template<typename InputIterator1, typename InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2)
	{
		return includes(first1, last1, first2, last2, less<typename iterator_traits<InputIterator1>::value_type,
															typename iterator_traits<InputIterator2>::value_type>());
	}
	
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2,
							OutputIterator result, Compare comp)
	{
		for(;first1!=last1;++result)
		{
			if (first2 == last2)
				return copy(first1, last1, result);
			if (comp(*first1, *first2))
				*result = *first1++;
			else if(comp(*first2,*first1))
				*result = *first2++;
			else
			{
				*result = *first1++;
				++first2;
			}
		}
		return copy(first2, last2.result);
	}
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
							InputIterator2 first2, InputIterator2 last2,
							OutputIterator result)
	{
		return set_union(first1, last1, first2, last2, result, less<typename iterator_traits<InputIterator1>::value_type,
															typename iterator_traits<InputIterator2>::value_type>());
	}
	

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,typename Compare>
		OutputIterator set_intersection(
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp)
	{
		for(;first1!=last1 && last2 != first2;++first1)
		{
			if (comp(*first1, *first2))
				++first1;
			else if (comp(*first2, *first1))
				++first2;
			else
			{
				*result++ = *first1++;
				++first2;
			}
		}
		return result;
	}
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_intersection(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		return set_intersection(first1, last1, first2, last2, result, less<typename iterator_traits<InputIterator1>::value_type,
																			typename iterator_traits<InputIterator2>::value_type>());
	}

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator set_difference(
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp)
	{
		for (; first1 != last1 ;)
		{
			if (first2 == last2)
				return copy(first1, last1, result);
			if (comp(*first1, *first2))
				*result++ = *first1++;
			else if (comp(*first2, *first1))
				++first2;
			else
				++first1, (void)++first2;
		}
		return result;
	}
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		return set_difference(first1, last1, first2, last2, result, less<typename iterator_traits<InputIterator1>::value_type,
			typename iterator_traits<InputIterator2>::value_type>());
	}
	

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator set_symmetric_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		for (; first1 != last1;)
		{
			if (first2 == last2)
				return copy(first1, last1, result);
			if (comp(*first1, *first2))
				*result++ = *first1++;
			else if (comp(*first2, *first1))
				*result++ = *first2++;
			else
				++first1, (void)++first2;
		}
		return copy(first2, last2, result);
	}
	template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
		typename Compare>
		OutputIterator set_symmetric_difference(
			InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			OutputIterator result, Compare comp)
	{
		return set_symmetric_difference(first1, last1, first2, last2, result, less<typename iterator_traits<InputIterator1>::value_type,
																	typename iterator_traits<InputIterator2>::value_type>());
	}

	// heap operations:
	template<typename RandomAccessIterator, typename Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp)
	{
		static_assert(!is_random_access_iterator<RandomAccessIterator>);

		auto l = distance(first, last);
		if (!l || l == 1)
			return;
		l = (l >> 1) - 1;
		for()
	}
	template<typename RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		return set_symmetric_difference(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type,
																		typename iterator_traits<RandomAccessIterator>::value_type>());
	}
	



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