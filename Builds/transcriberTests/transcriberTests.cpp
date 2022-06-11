#include "pch.h"
#include "CppUnitTest.h"
#include <WindowFunction.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace transcriberTests
{
	TEST_CLASS(transcriberTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Window<float> testwindow{ 256 };
			Assert::AreSame<int>(256, testwindow.getSize());
		}
	};
}
