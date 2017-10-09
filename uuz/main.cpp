#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
//#include"ptr.h"
#include"test\test.h"
//#include"test\debug.h"
//#include"Container/set.h"
//#include"Container\allocator.h"
//#include"Container\array.h"
//#include"Container\vector.h"
//#include"Container\list.h"
//#include"Container\forward_list.h"
//#include"Container\set.h"
//#include"test\Container\vector_test.h"
//#include"Container\deque.h"
//#include"Container/string.h"
//#include"Container/pair.h"
//#include"Container/list.h"
//#include"Container/vector.h"
//#include"Container/forward_list.h"
//#include"Container/array.h"
//#include"Container/priority_queue.h"
//#include"Container/rbtree.h"
//#include"Container/set.h"
#include<future>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<fstream>
#include<deque>
#include<map>
#include<set>
#include<string>
#include<future>
#include<iostream>
#include<vector>
#include<memory>
#include<ctime>
#include<queue>
#include<list>
#include<functional>
#include<forward_list>
#include<array>
#include<numeric>
#include<iomanip>
#include<set>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<sstream>
#include<algorithm>
#include"Container\pair.h"
using namespace std;
vector<pair<int, int>> num;
int ddd[200][200];
int ccc[200];
int a, b, c, v, d;
int dij()
{
	memset(ccc, 0, sizeof(ccc));
	for (auto i = 2; i <= a; ++i)
		if (ddd[1][i])
			ccc[i] = ddd[1][i];
	for (auto i = 2; i <= a; ++i)
	{
		int maxx = (std::numeric_limits<int>::max)();
		int p=0;
		for (auto j = 2; j <= a; ++j)
			if (ccc[j] && ccc[j] < maxx)
			{
				maxx = ccc[j];
				p = j;
			}
		if(p)
			for (auto j = 2; j <= a; ++j)
				if (ddd[p][j] && (ccc[p] + ddd[p][j] < ccc[j] || !ccc[j]))
					ccc[j] = ccc[p] + ddd[p][j];
	}
	for (auto i = 2; i <= a; ++i)
		cout << ccc[i] << " ";
	cout << endl;
	return ccc[a]==0 ? (std::numeric_limits<int>::max)() : ccc[a];
}
int main()
{
	scanf("%d %d", &a, &b);
	for (auto i = 0; i != b; ++i)
	{
		scanf("%d%d%d", &c, &v, &d);
		num.push_back(pair<int, int>(c, v));
		ddd[c][v] = d;
		ddd[v][c] = d;
		if (c == 1)
			ccc[v] = d;
	}
	auto maxx = dij();
	cout << maxx<<endl;
	auto kss = (std::numeric_limits<int>::max)();
	for (auto i : num)
	{
		auto k = i.first;
		auto l = i.second;
		auto c = ddd[k][l];
		ddd[k][l] = 0;
		ddd[l][k] = 0;
		auto p = dij();
		cout << p << endl;
		ddd[k][l] = c;
		ddd[l][k] = c;
		if (p != maxx)
			if (p < kss)
				kss = p;
	}
	cout << kss;
	system("pause");
	return 0;
}