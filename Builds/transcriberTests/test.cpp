#include "pch.h"
#include "WindowFunction.h"

TEST(WindowTest, ConstrctuctionWorks) {
	Window<float> testWindow{256};
  EXPECT_EQ(testWindow.getSize(), 256);
  EXPECT_TRUE(true);
}

TEST(WindowTest, resizeWorks) {
	Window<float> testWindow{ 256 };
	testWindow.resize(512);
	EXPECT_EQ(testWindow.getSize(), 512);
}