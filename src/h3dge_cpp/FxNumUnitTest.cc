#include "FxNum.hh"
#include "gtest/gtest.h"

TEST(fxnum, memlayout)
{
	EXPECT_EQ((unsigned)4, sizeof(fxnum<32>));

	fxnum<4> one = 1.0;
	uint32_t *oneInt = (uint32_t *)&one;
	EXPECT_EQ((uint32_t)0x10000000, *oneInt);

	fxnum<8> oneHalf = 0.5;
	uint32_t *oneHalfInt = (uint32_t *)&oneHalf;
	EXPECT_EQ((uint32_t)0x00800000, *oneHalfInt);

	fxnum<16> minusOne = -1.0;
	uint32_t *minusOneInt = (uint32_t *)&minusOne;
	EXPECT_EQ((uint32_t)0xffff0000, *minusOneInt);
}

TEST(fxnum, sum) {
	fxnum<4> f1 = 1.0;
	fxnum<8> f2 = 2.0;
	f2 += f1;

	EXPECT_EQ(3.0, f2.toFloat());

	fxnum<16> f3 = f1 + f2;
	EXPECT_EQ(4.0, f3.toFloat());

	fxnum<16> f4 = f1 + (-0.5);
	EXPECT_EQ(0.5, f4.toFloat());

	fxnum<16> f5 = -1.5 + f1;
	EXPECT_EQ(-0.5, f5.toFloat());
}
