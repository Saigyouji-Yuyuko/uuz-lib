#define USING_UUZ
#define _CRT_SECURE_NO_WARNINGS
//#include"ptr.h"
//#include"test\test.h"
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
//#pragma warning(disable:4996)
using namespace std;
vector<int>minn;
vector<int>kkk;
string p; 
vector<string> dict;
map<int, vector<int>> ss;
void mincut()
{
	ss.insert({ 0,vector<int>() });
	for (auto i = 0; i != p.size(); ++i)
	{
		if(ss.count(i) !=0)
		{
			for (auto j = 0; j != dict.size(); ++j)
			{
				auto k = p.find(dict[j], i);
				if(k == i)
				{
					if(ss.count(i+ dict[j].size()) == 0)
					{
						std::vector<int> p(ss[i]);
						p.push_back(j);
						ss.insert({ i + dict[j].size(), p });
					}
					else
					{
						auto kk = ss[i + dict[j].size()].size();
						if(kk > ss[i].size()+1)
						{
							std::vector<int> p(ss[i]);
							p.push_back(j);
							ss[i] = p;
						}
					}
				}
			}
		}
	}
	if (ss.count(p.size()) == 0)
		cout << "";
	else
	{
		for (auto i: ss[p.size()])
			cout << dict[i] << " ";
	}
}
int main()
{
	
	int n;
	cin >> p;
	string temp;
	scanf("%d", &n);
	dict.resize(n);
	while(n--)
	{
		cin >> temp;
		dict.push_back(temp);
	}
	minn.resize(p.size());
	mincut();
	system("pause");
	return 0;
}