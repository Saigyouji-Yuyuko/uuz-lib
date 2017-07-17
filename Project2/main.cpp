//#define USING_UUZ
//#define _CRT_SECURE_NO_WARNINGS
//#include<string>
//#include<future>
//#include<iostream>
//#include<vector>
//#include<memory>
//#include <ctime>
//#include<queue>
//#include<list>
//#include"time.h"
//#include"ptr.h"
//#include<functional>
//#include<forward_list>
//#include<type_traits>
//#include"string.h"
//#include"debug.h"
//#include<thread>
//#include<mutex>
//#include<array>
//#include<algorithm>
//#include<numeric>
//#pragma warning(disable:4996)
//template<typename Iterator, typename T>
//struct accumulate_block
//{
//	void operator()(Iterator first, Iterator last, T& result)
//	{
//		result = std::accumulate(first, last, result);
//	}
//};
//template<typename Iterator, typename T>
//T async_accumulate(Iterator first, Iterator last, T init)
//{
//	auto length = std::distance(first, last);
//	auto a_size = length / 8;
//	std::vector<std::future<T>> fu(8);
//	auto a_begin = first;
//	auto a_end = first;
//
//	for (auto& i : fu)
//	{
//		a_end += a_size;
//		i = std::async(std::accumulate<Iterator,T>, a_begin, a_end, init);
//		a_begin = a_end;
//	}
//	auto p = init;
//	for (auto& i : fu)
//		p += i.get();
//	return p;
//}
//template<typename Iterator, typename T>
//T thread_accumulate(Iterator first, Iterator last, T init)
//{
//	unsigned long const length = std::distance(first, last);
//	if (!length) // 1
//		return init;
//	unsigned long const min_per_thread = 25;
//	unsigned long const max_threads =
//		(length + min_per_thread - 1) / min_per_thread; // 2
//	unsigned long const hardware_threads =
//		std::thread::hardware_concurrency();
//	unsigned long const num_threads = // 3
//		std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
//	unsigned long const block_size = length / num_threads; // 4
//	std::vector<T> results(num_threads);
//	std::vector<std::thread> threads(num_threads - 1); // 5
//	Iterator block_start = first;
//	for (unsigned long i = 0; i < (num_threads - 1); ++i)
//	{
//		Iterator block_end = block_start;
//		std::advance(block_end, block_size); // 6
//		threads[i] = std::thread( // 7
//			accumulate_block<Iterator, T>(),
//			block_start, block_end, std::ref(results[i]));
//		block_start = block_end; // 8
//	}
//	accumulate_block<Iterator, T>()(
//		block_start, last, results[num_threads - 1]); // 9
//	std::for_each(threads.begin(), threads.end(),
//		std::mem_fn(&std::thread::join)); // 10
//	return std::accumulate(results.begin(), results.end(), init); // 11
//}
//void pp()
//{
//	std::cout << std::this_thread::get_id() << std::endl;
//}
//
//std::array<long long, 5000000> ppl;
//long long main()
//{
//	for (long long i = 1; i < ppl.size(); ++i)
//		ppl[i] = i;
//	{
//		uuz::time d{"std:"};
//		std::cout << accumulate(ppl.begin(), ppl.end(), 0);
//	}
//	{
//		uuz::time d{"thread:"};
//		std::cout << thread_accumulate(ppl.begin(), ppl.end(), 0);
//	}
//
//	system("pause");
//}
#include<iostream>
#include<array>
#include<set>
using namespace std;
//struct car
//{
//	long long begin;
//	long long pinlv;
//	long long time;
//};
//std::set<std::pair<long long, long long>>old, now;
//std::array<car, 3000> cars;
//long long gettime(long long k, long long t)
//{
//	if (t <= cars[k].begin)
//		return cars[k].begin;
//	auto p = ((1.0*t) - cars[k].begin) / cars[k].pinlv;
//	return cars[k].begin + cars[k].pinlv*ceil(p);
//
//}
int main()
{
	//freopen("C:/Users/99650/Desktop/123.in", "r", stdin);
	//freopen("C:/Users/99650/Desktop/123.txt", "w", stdout);
	//long long n;
	//scanf("%lld", &n);
	//auto c = n;
	//while (c--)
	//{
	//	long long city, time, latest_time;
	//	scanf("%lld%lld%lld", &city, &time, &latest_time);
	//	for (long long i = 0; i < city-1; ++i)
	//		scanf("%lld%lld%lld", &cars[i].begin, &cars[i].pinlv, &cars[i].time);
	//	old.clear();
	//	old.insert(pair<long long,long long>{ 0,0 });
	//	now.clear();
	//	for (long long i = 0; i < city-1; ++i)
	//	{
	//		for (const auto& j :old)
	//		{
	//			pair<long long, long long> a = pair<long long, long long>(j.first, gettime(i, j.second)+cars[i].time);
	//			pair<long long, long long> b = pair<long long, long long>(j.first + 1, gettime(i, j.second + time) + cars[i].time);
	//			if (a.second <= latest_time)
	//				now.insert(a);
	//			if (b.second <= latest_time)
	//				now.insert(b);
	//		}
	//		old = std::move(now);
	//
	//	}
	//	pair<long long, long long> k{-1,0};
	//	for (const auto& i : old)
	//		if (i.first > k.first)
	//			k = i;
	//	if (k.first == -1)
	//		//printf ("Case #%d: IMPOSSIBLE\n", n - c);
	//		cout << "IMPOSSIBLE" << endl;
	//	else
	//		//printf("Case #%d: %lld\n", n - c, k.first);
	//		cout << k.first << endl;
	//}
	//system("pause");
	std::array<int, 100000> A;
	std::array<int, 100000> B;
	int n;
	int c = n;
	while (c--)
	{
		int ss,k, a1, b1, c, d, e1, e3, f;
		cin >>ss>>k>>A[1]>>B[1]>>c>>d>>e1>>e3>>f;
		for (int i = 2; i <= ss; ++i)
		{
			A[i] = (C*A)
		}
			
		
	}
	return 0;

}