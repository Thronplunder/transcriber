#include "pch.h"
#include "CppUnitTest.h"
#include <WindowFunction.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace transcriberTests
{
	TEST_CLASS(transcriberTests)
	{
	public:
		
		TEST_METHOD(TestWindowSize)
		{
			Window<float> testwindow{ 256 };

			Assert::AreEqual<int>(256, testwindow.getSize());
		}

		TEST_METHOD(TestZeroInput)
		{
			Window<float> testWindow{ 256 };
			float testDataSine[265] = {}; //0 initialised test data
			float testDataHann[265] = {}; //0 initialised test data
			float testDataTri[265] = {}; //0 initialised test data

			testWindow.applyWindow(testDataSine, 256, windowType::Sine);
			testWindow.applyWindow(testDataHann, 256, windowType::Hann);
			testWindow.applyWindow(testDataTri, 256, windowType::Triangle);

			for (int i = 0; i < 256; ++i) {
				Assert::AreEqual<float>(testDataSine[i], 0);
				Assert::AreEqual<float>(testDataHann[i], 0);
				Assert::AreEqual<float>(testDataTri[i], 0);
			}
		}

		TEST_METHOD(TestResize)
		{
			Window<float> testWindow{ 265 };

			testWindow.resize(512);

			Assert::AreEqual<int>(testWindow.getSize(), 512);
		}
	};
}
