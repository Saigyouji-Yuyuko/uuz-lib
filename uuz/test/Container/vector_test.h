#pragma once
#include"../test.h"
#include"../../Container/vector.h"
namespace uuz
{
	TEST(vector_test)
	{
		//using namespace uuz;
		uuz::vector<int> p;
		uuz::vector<int> p1(allocator<int>());
		uuz::vector<int> p2(10,1);
		COUT(p2);
		uuz::vector<int>p3(p2);
		COUT(p2);
		COUT(p3);
		uuz::vector<int>p4(std::move(p3));
		COUT(p4);
		uuz::vector<int>p5{ 1,2,3,4,5,6,7,8,9,10 };
		COUT(p5);
		uuz::vector<int>p6(p5.begin(), p5.end());
		COUT(p6);
		uuz::vector<int>p7;
		p7 = p6;
		COUT(p7);
		uuz::vector<int>p8;
		p8 = std::move(p6);
		COUT(p8);
		uuz::vector<int>p9;
		p9.assign(p8.begin(), p8.end() - 1);
		COUT(p9);
		std::vector<int> pp = { 1,2,3,4,5,6,7,8,9,10 };
		EXPECT_EQ(p5.at(5), pp.at(5));
		EXPECT_EQ(p5[7], pp[7]);
		EXPECT_EQ(p5.front(), pp.front());
		EXPECT_EQ(p5.back(), pp.back());
		EXPECT_CON_EQ(p5, pp);
		EXPECT_EQ(*(p5.insert(p5.begin(),5)), *(pp.insert(pp.begin(), 5)));
		EXPECT_CON_EQ(p5, pp);
		EXPECT_EQ(*(p5.insert(p5.begin(), 5,5)), *(pp.insert(pp.begin(),5,5)));
		EXPECT_CON_EQ(p5, pp);
		EXPECT_EQ(*(p5.insert(p5.begin(), {1,2,3,4,5})), *(pp.insert(pp.begin(), { 1,2,3,4,5 })));
		EXPECT_CON_EQ(p5, pp);
		COUT(p5);
		COUT(pp);
		EXPECT_EQ(*(p5.erase(p5.begin(), p5.begin() + 4)), *(pp.erase(pp.begin(), pp.begin() + 4)));
		COUT(p5);
		COUT(pp);
		EXPECT_CON_EQ(p5, pp);
		p5.resize(6);
		pp.resize(6);
		EXPECT_CON_EQ(p5, pp);
		CON_TEST_P(vector<int>, push_back, 10000, 100000, 1000000, rand());

	}
}