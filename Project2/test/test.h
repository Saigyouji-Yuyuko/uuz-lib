//   感谢 https://github.com/Alinshans/MyTinySTL/blob/master/Test/test.h
#pragma once
#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#ifdef _WIN32
#include<windows.h>
#endif // WIN32

namespace uuz
{
	namespace
	{
		enum class Color
		{
			red, green, white
		};
#ifdef _WIN32
		std::ostream& operator<<(std::ostream &out, Color color);
		class SetColor
		{
		public:
			SetColor() {}
			~SetColor() {}
			//  friend std::ostream& operator <<(std::ostream &out,Color color);  

			BOOL SetConsoleTextColor(WORD wAttributes);
			std::ostream& GreenColor(std::ostream &out);
			std::ostream& RedColor(std::ostream &out);
			std::ostream& WhiteColor(std::ostream &out);
		protected:
		private:
		};

		inline BOOL SetColor::SetConsoleTextColor(WORD wAttributes)
		{
			if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes))
			{
				SetLastError(GetLastError());
				return FALSE;
			}
			return TRUE;
		}

		inline std::ostream& SetColor::GreenColor(std::ostream &out)
		{
			SetConsoleTextColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			return out;
		}

		inline std::ostream& SetColor::RedColor(std::ostream &out)
		{
			SetConsoleTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED);
			return out;
		}

		inline std::ostream& SetColor::WhiteColor(std::ostream &out)
		{
			SetConsoleTextColor(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			return out;
		}
		
#endif		
		std::ostream& operator<< (std::ostream &out, Color color)
		{
			SetColor setcolor;
			
			switch (color)
			{
#ifdef _WIN32
			case Color::red:
				return setcolor.RedColor(out);
			case Color::green:
				return setcolor.GreenColor(out);
			case Color::white:
				return setcolor.WhiteColor(out);
#endif
			}
			return out;
		}
	}
	class testcase
	{
	public:
		testcase(const char* p):name(p){}
		virtual void run() = 0;

		const char* name;
		int numcase = 0;
		int fail = 0;
		int pass = 0;
	};
	class unittest
	{
	public:
		static unittest* getsingleunit()noexcept;

		testcase* push(testcase* p)
		{
			testcases.push_back(p);
			return p;
		}
		void run()
		{
			for (auto i : testcases)
			{
				nowcase = i;
				i->numcase = 1;
				i->fail = 0;
				i->pass = 0;
				std::cout << Color::green << "============================================" << std::endl;
				std::cout << Color::green << " Run TestCase:" << nowcase->name << std::endl;
				nowcase->run();
				if (nowcase->fail == 0)
					std::cout << Color::green;
				else
					std::cout << Color::red;
				std::cout << " " << nowcase->pass << " / " << nowcase->pass + nowcase->fail << " Case passed ( " << (double)(nowcase->pass) / (nowcase->pass + nowcase->fail) * 100 << " % )" << std::endl;
				std::cout << Color::green << " End TestCase:" << nowcase->name << std::endl;
				if (nowcase->numcase)
					++pass;
				else
					++fail;
			}
			std::cout << Color::green << "============================================" << std::endl;
			std::cout << Color::green << "Total TestCase : " << pass + fail << std::endl;
			std::cout << Color::green << "Total Passed : " << pass << std::endl;
			std::cout << Color::red << "Total Failed " << fail << std::endl;
			std::cout << Color::green << " " << pass << " / " << pass + fail << " TestCase passed . ( " << (double)pass / (pass + fail) * 100 << "% )" << std::endl;
		}
	public:
		testcase* nowcase;
		int pass = 0;
		int fail = 0;
	private:
		std::vector<testcase*> testcases;
	};
	unittest* unittest::getsingleunit()noexcept
	{
		static unittest a;
		return &a;
	}
#define TESTCASE_NAME(testcase_name) \
    testcase_name##_TEST
#define MYTINYSTL_TEST_(testcase_name)                        \
class TESTCASE_NAME(testcase_name) : public uuz::testcase {        \
public:                                                       \
    TESTCASE_NAME(testcase_name)(const char* case_name)       \
        : uuz::testcase(case_name) {};                             \
    virtual void run()override;                                       \
private:                                                      \
     const static uuz::testcase* testcasess;                         \
};                                                            \
                                                              \
testcase* const TESTCASE_NAME(testcase_name)::testcasess = uuz::unittest::getsingleunit()->push(  \
        new TESTCASE_NAME(testcase_name)(#testcase_name));    \
void TESTCASE_NAME(testcase_name)::run()

#define EXPCET_TRUE(CONDITION)do try{							\
	if(CONDITION){											\
		unittest::getsingleunit()->nowcase->pass++;			\
		std::cout<<Color::green<<"EXPCET_TRUE succeeded!"<<std::endl;}\
	else{													\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPCET_TRUE failed!"<<std::endl;}}\
	catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPCET_TRUE failed and some excption throw !"<<std::endl;}\
		while(0)
#define EXPCET_FALSE(CONDITION)do try{							\
	if(!(CONDITION)){											\
		unittest::getsingleunit()->nowcase->pass++;			\
		std::cout<<Color::green<<"EXPCET_FALSE succeeded!"<<std::endl;}\
	else{													\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPCET_FALSE failed!"<<std::endl;}}\
	catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPCET_FALSE failed and some excption throw !"<<std::endl;}\
		while(0)
#define EXPECT_EQ(v1, v2) do try { \
  if ((v1) == (v2)) {                                               \
    unittest::getsingleunit()->nowcase->pass++;					\
    std::cout << Color::green << " EXPECT_EQ succeeded!"<<std::endl;}\                                                             \
  else {                                                        \
   unittest::getsingleunit()->nowcase->fail++;					\
		unittest::getsingleunit()->nowcase->numcase = 0;		\
    std::cout << Color::red << " EXPECT_EQ failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << (v1) << std::endl;               \
    std::cout << Color::red << " Actual:" << (v2) << std::endl;}}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_EQ failed and some excption throw !"<<std::endl;}\
 while(0)
#define EXPECT_NE(v1, v2) do try{                                  \
  if ((v1) != (v2)) {                                               \
    unittest::getsingleunit()->nowcase->pass++;        \
   std::cout << Color::green << " EXPECT_NE succeeded!"<<std::endl;             \
  }                                                             \
  else {                                                        \
     unittest::getsingleunit()->nowcase->fail++;					\
		unittest::getsingleunit()->nowcase->numcase = 0;		\
    std::cout << Color::red << " EXPECT_NE failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << (v1) << std::endl;               \
    std::cout << Color::red << " Actual:" << (v2) << std::endl; }}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_NE failed and some excption throw !"<<std::endl;}\
 while(0)
#define EXPECT_LT(v1, v2) do try{                                  \
  if ((v1) < (v2)) {                                                \
    unittest::getsingleunit()->nowcase->pass++;        \
    std::cout << Color::green << " EXPECT_LT succeeded!"<<std::endl;            \
  }                                                             \
  else {                                                        \
    unittest::getsingleunit()->nowcase->fail++;					\
		unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_LT failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << (v1) << std::endl;               \
    std::cout << Color::red << " Actual:" << (v2) << std::endl;}}               \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_LT failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_LE(v1, v2) do try{                                  \
  if ((v1) <= (v2)) {                                               \
    unittest::getsingleunit()->nowcase->pass++;        \
    std::cout << Color::green << " EXPECT_LE succeeded!"<<std::endl;            \
  }                                                             \
  else {                                                        \
     unittest::getsingleunit()->nowcase->fail++;					\
		unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_LE failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << (v1) << std::endl;               \
    std::cout << Color::red << " Actual:" << (v2) << std::endl;}}               \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_LE failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_GT(v1, v2) do try{                                  \
  if ((v1) > (v2)) {								                  \
    unittest::getsingleunit()->nowcase->pass++;        \
    std::cout << Color::green << " EXPECT_GT succeeded!"<<std::endl;            \
  }                                                             \
  else {                                                        \
     unittest::getsingleunit()->nowcase->fail++;					\
		unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_GT failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << (v1) << std::endl;               \
    std::cout << Color::red << " Actual:" << (v2) << std::endl;}}               \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_GT failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_GE(v1, v2) do try {                                  \
  if ((v1) >= (v2)) {                                               \
    unittest::getsingleunit()->nowcase->pass++;       \
    std::cout << Color::green << " EXPECT_GE succeeded!"<<std::endl;            \
  }                                                             \
  else {                                                        \
    unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_GE failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << v1 << std::endl;               \
    std::cout << Color::red << " Actual:" << v2 << std::endl;}}               \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_GE failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_STREQ(s1, s2) do try{                                 \
  if (s1 == nullptr || s2 == nullptr) {                                 \
    if (s1 == nullptr && s2 == nullptr) {                               \
      unittest::getsingleunit()->nowcase->pass++;        \
      std::cout << Color::green << " EXPECT_STRED succeeded!"<<std::endl;         \
    }                                                             \
    else {                                                        \
       unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;        \
      std::cout << Color::red << " EXPECT_STRED failed!"<<std::endl;              \
      if(s1 == nullptr) std::cout << " Expect: NULL"<<std::endl;              \
      else std::cout << " Expect:" << s1 << std::endl;             \
      if(s2 == nullptr) std::cout << "Actual: NULL"<<std::endl;              \
      else std::cout << " Actual: " << s2 << std::endl;             \
    }                                                             \
  }                                                               \
  else if (strcmp(s1, s2) == 0) {                                 \
    unittest::getsingleunit()->nowcase->pass++;          \
    std::cout << Color::green << " EXPECT_STRED succeeded!"<<std::endl;           \
  }                                                               \
  else {                                                          \
    unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;          \
    std::cout << Color::red << " EXPECT_STRED failed!"<<std::endl;                \
    std::cout << Color::red << " Expect: " << s1 << std::endl;             \
    std::cout << Color::red << " Actual: " << s2 << std::endl; }}            \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_STREQ failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_STRNE(s1, s2) do try{                                 \
  if (s1 == nullptr || s2 == nullptr) {                                 \
    if (s1 != nullptr || s2 != nullptr) {                               \
      unittest::getsingleunit()->nowcase->pass++;         \
      std::cout << Color::green << " EXPECT_STRNE succeeded!"<<std::endl;         \
    }                                                             \
    else {                                                        \
     unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;       \
      std::cout << Color::red << " EXPECT_STRNE failed!"<<std::endl;              \
      if(s1 == nullptr) std::cout << " Expect: NULL"<<std::endl;              \
      else std::cout << " Expect: " << s1 << std::endl;             \
      if(s2 == nullptr) std::cout << " Actual: NULL"<<std::endl;              \
      else std::cout << " Actual: " << s2 << std::endl;             \
    }                                                             \
  }                                                               \
  else if (strcmp(s1, s2) != 0) {                                 \
    unittest::getsingleunit()->nowcase->pass++;          \
    std::cout << Color::green << " EXPECT_STRNE succeeded!"<<std::endl;           \
  }                                                               \
  else {                                                          \
    unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;         \
    std::cout << Color::red << " EXPECT_STRNE failed!"<<std::endl;                \
    std::cout << Color::red << " Expect: " << s1 <<  std::endl;             \
    std::cout << Color::red << " Actual: " << s2 <<  std::endl;}}             \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_STRNE failed and some excption throw !"<<std::endl;}\
 while(0)
#define EXPECT_PTR_EQ(p1, p2) do try{                              \
  if (*p1 == *p2) {                                             \
    unittest::getsingleunit()->nowcase->pass++;        \
    std::cout << Color::green << " EXPECT_PTR_EQ succeeded!"<<std::endl;        \
  }                                                             \
  else {                                                        \
    unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_PTR_EQ failed!"<<std::endl;             \
    std::cout << Color::red << " Expect:" << *p1 << std::endl;              \
    std::cout << Color::red << " Actual:" << *p2 << std::endl;}}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_PTR_EQ failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_PTR_NE(p1, p2) do try{                              \
  if (*p1 != *p2) {                                             \
    unittest::getsingleunit()->nowcase->pass++;        \
    std::cout << Color::green << " EXPECT_PTR_NE succeeded!"<<std::endl;        \
  }                                                             \
  else {                                                        \
   unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_PTR_NE failed!"<<std::endl;             \
    std::cout << Color::red << " Expect:" << *p1 << std::endl;              \
    std::cout << Color::red << " Actual:" << *p2 << std::endl;}}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_PTR_NE failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_PTR_RANGE_EQ(p1, p2, len) do try{                   \
  if (std::equal(p1, p1 + len, p2)) {                           \
    unittest::getsingleunit()->nowcase->pass++;        \
    std::cout << Color::green << " EXPECT_PTR_RANGE_EQ succeeded!"<<std::endl;  \
  }                                                             \
  else {                                                        \
   unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout << Color::red << " EXPECT_PTR_RANGE_EQ failed!"<<std::endl; }}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_PTR_RANGE_EQ failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_PTR_RANGE_NE(p1, p2, len) do try{                   \
  if (!std::equal(p1, p1 + len, p2)) {                          \
    unittest::getsingleunit()->nowcase->pass++;      \
    std::cout << Color::green << " EXPECT_PTR_RANGE_NE succeeded!"<<std::endl;  \
  }                                                             \
  else {                                                        \
    unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;        \
    std::cout <<Color::red << " EXPECT_PTR_RANGE_NE failed!"<<std::endl; }}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_PTR_RANGE_NE failed and some excption throw !"<<std::endl;}\
 while(0)
		
#define EXPECT_CON_EQ(c1, c2) do try{                                  \
  auto first1 = std::begin(c1), last1 = std::end(c1);               \
  auto first2 = std::begin(c2), last2 = std::end(c2);               \
  for (; first1 != last1 && first2 != last2; ++first1,(void)++first2) {  \
    if (*first1 != *first2)  break;}                                    \
  if (first1 == last1 && first2 == last2) {                         \
    unittest::getsingleunit()->nowcase->pass++;            \
    std::cout << Color::green << " EXPECT_CON_EQ succeeded!"<<std::endl;            \
  }                                                                 \
  else {                                                            \
    unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;            \
    std::cout << Color::red << " EXPECT_CON_EQ failed!"<<std::endl;                 \
    std::cout << Color::red << " Expect:" << *first1 << std::endl;              \
    std::cout << Color::red << " Actual:" << *first2 << std::endl;}}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_CON_EQ failed and some excption throw !"<<std::endl;}\
 while(0)

#define EXPECT_CON_NE(c1, c2) do {                                  \
  auto first1 = std::begin(c1), last1 = std::end(c1);               \
  auto first2 = std::begin(c2), last2 = std::end(c2);               \
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {  \
    if (*first1 != *first2)  break;                                 \
  }                                                                 \
  if (first1 != last1 || first2 != last2) {                         \
    unittest::getsingleunit()->nowcase->pass++;            \
    std::cout << Color::green << " EXPECT_CON_NE succeeded!"<<std::endl;            \
  }                                                                 \
  else {                                                            \
    unittest::getsingleunit()->nowcase->fail++;					\
	unittest::getsingleunit()->nowcase->numcase = 0;            \
    std::cout << Color::red << " EXPECT_CON_NE failed!"<<std::endl;      }}              \
catch(...){\
		unittest::getsingleunit()->nowcase->fail++;			\
		unittest::getsingleunit()->nowcase->numcase = 0;	\
		std::cout<<Color::red<<"EXPECT_CON_NE failed and some excption throw !"<<std::endl;}\
 while(0)
#if defined(_DEBUG) || defined(DEBUG)
#define LEN1    10000
#define LEN2    100000
#define LEN3    1000000
#else
#define LEN1    100000
#define LEN2    1000000
#define LEN3    10000000
#endif

#define _LLL * 20
#define _LL  * 10
#define _L   * 5
#define _M
#define _s   / 5
#define _SS  / 10
#define _SSS / 20

#define WIDE    14

// 输出通过提示
#define PASSED    std::cout << "[ PASSED ]"<<std::endl;

#define COUT(container) do {                             \
  std::string con_name = #container;                     \
  std::cout <<typeid(decltype(container)).name()<< " " << con_name << " :";                  \
  for (auto it : container)                              \
    std::cout << " " << it;                              \
  std::cout << std::endl;                                     \
} while(0)

#define STR_COUT(str) do {                               \
  std::string str_name = #str;                           \
  std::cout << " " << str_name << " : " << str << std::endl;  \
} while(0)

// 输出容器调用函数后的结果
#define FUN_AFTER(con, fun) do {                         \
  std::string fun_name = #fun;                           \
  std::cout << " After " << fun_name << " :"<<std::endl;          \
  fun;                                                   \
  COUT(con);                                             \
} while(0)

#define STR_FUN_AFTER(str, fun) do {                     \
  std::string fun_name = #fun;                           \
  std::cout << " After " << fun_name << " :"<<std::endl;          \
  fun;                                                   \
  STR_COUT(str);                                         \
} while(0)

// 输出容器调用函数的值
#define FUN_VALUE(fun) do {                              \
  std::string fun_name = #fun;                           \
  std::cout << " " << fun_name << " : " << fun << std::endl;  \
} while(0)

// 输出测试数量级
void test_len(size_t len1, size_t len2, size_t len3, size_t wide)
{
	std::string str1, str2, str3;
	std::stringstream ss;
	ss << len1 << " " << len2 << " " << len3;
	ss >> str1 >> str2 >> str3;
	str1 += "   |";
	std::cout << std::setw(wide) << str1;
	str2 += "   |";
	std::cout << std::setw(wide) << str2;
	str3 += "   |";
	std::cout << std::setw(wide) << str3 << "\n";
}

#define TEST_LEN(len1, len2, len3, wide) \
  test_len(len1, len2, len3, wide)


#define FUN_TEST_FORMAT(mode, fun, count, ...) do {         \
  srand((int)time(0));                                       \
  clock_t start, end;                                        \
  mode c;                                                    \
  char buf[10];                                              \
  start = clock();                                           \
  for (size_t i = 0; i < count; ++i)                         \
    c.fun(##__VA_ARGS__);                                              \
  end = clock();                                             \
  int n = static_cast<int>(static_cast<double>(end - start)  \
      / CLOCKS_PER_SEC * 1000);                              \
  std::snprintf(buf, sizeof(buf), "%d", n);                  \
  std::string t = buf;                                       \
  t += "ms    |";                                            \
  std::cout << std::setw(WIDE) << t;                         \
} while(0)
#define CON_TEST_P(con, fun, len1,len2, len3,...)         \
  TEST_LEN(len1, len2, len3, WIDE);                          \
  std::cout << "|         std         |";                    \
  FUN_TEST_FORMAT(std::con, fun, len1,##__VA_ARGS__);                \
  FUN_TEST_FORMAT(std::con, fun, len2,##__VA_ARGS__);                \
  FUN_TEST_FORMAT(std::con, fun, len3, ##__VA_ARGS__);                \
  std::cout << "\n|        uuz         |";                  \
  FUN_TEST_FORMAT(uuz::con, fun, len1,##__VA_ARGS__);              \
  FUN_TEST_FORMAT(uuz::con, fun, len2, ##__VA_ARGS__);              \
  FUN_TEST_FORMAT(uuz::con, fun, len3, ##__VA_ARGS__);
#define TEST(testcase_name) \
  MYTINYSTL_TEST_(testcase_name)

// 运行所有测试案例
#define RUN_ALL_TESTS() \
  uuz::unittest::getsingleunit()->run()
}	