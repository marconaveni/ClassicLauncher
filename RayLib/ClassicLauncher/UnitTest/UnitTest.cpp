
#include "CppUnitTest.h"
#include "../Source/Date.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(TestDateTrue)
	{
	public:

		TEST_METHOD(EqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 == dateTime1), true);
		}
		TEST_METHOD(DifferentEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20261228T230138" };
			Assert::AreEqual((dateTime0 != dateTime1), true);
		}
		TEST_METHOD(GreaterValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20271228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 > dateTime1), true);
		}
		TEST_METHOD(LessValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20211228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 < dateTime1), true);
		}
	};

	TEST_CLASS(TestDateFalse)
	{
	public:

		TEST_METHOD(EqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20291228T230138" };
			Assert::AreEqual((dateTime0 == dateTime1), false);
		}
		TEST_METHOD(DifferentEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 != dateTime1), false);
		}
		TEST_METHOD(GreaterEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20291228T230138" };
			Assert::AreEqual((dateTime0 >= dateTime1), false);
		}
		TEST_METHOD(LessEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20291228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 <= dateTime1), false);
		}
		TEST_METHOD(GreaterValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20201228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 > dateTime1), false);
		}
		TEST_METHOD(LessValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20271228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 < dateTime1), false);
		}
	};

	TEST_CLASS(TestDateEquals)
	{
	public:
		TEST_METHOD(GreaterEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 >= dateTime1), true);
		}
		TEST_METHOD(LessEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 <= dateTime1), true);
		}
		TEST_METHOD(GreaterEqualValue2)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20241228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 >= dateTime1), true);
		}
		TEST_METHOD(LessEqualValue2)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20201228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 <= dateTime1), true);
		}
		TEST_METHOD(NotGreaterEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20231228T230138" };
			dateTime1 = std::string{ "20291228T230138" };
			Assert::AreEqual((dateTime0 >= dateTime1), false);
		}
		TEST_METHOD(NotLessEqualValue)
		{
			DateTime dateTime0;
			DateTime dateTime1;
			dateTime0 = std::string{ "20291228T230138" };
			dateTime1 = std::string{ "20231228T230138" };
			Assert::AreEqual((dateTime0 <= dateTime1), false);
		}

	};
}
