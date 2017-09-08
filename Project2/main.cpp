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
using namespace std;
bool whatfind(int* t,int f,int l,int n)
{
	if (n == 0)
		return true;
	if (n < 0)
		return false;
	for (auto i = f; i != l - 1; ++i)
		if (whatfind(t, i + 1, l, n - t[i]))
			return true;
	return false;
}
bool iswanmei(int k)
{
	int t[10],sum=0;
	while(k)
	{
		t[sum++] = k % 10;
		k /= 10;
	}

	for (auto i = 0; i != sum; ++i)
		k += t[i];
	if (k & 1 == 1)
		return false;

	 return whatfind(t, 0, sum, k>>1);
			
}
int main()
{
	int a, b;
	cin >> a >> b;
	int sum = 0;
	for(auto i = max(a,11);i<=b;++i)
		if (iswanmei(i))
			++sum;		
	cout << sum;
	system("pause");
	return 0;
}