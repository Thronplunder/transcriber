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
			float testData1[265]= {};

			testwindow.applyWindow(testData1, 256, windowType::Sine);

			Assert::AreEqual<int>(256, testwindow.getSize());
			for (int i = 0; i < 256; ++i) {
				Assert::AreEqual<float>(testData1[i], 0);
			}
		}
	};
}
