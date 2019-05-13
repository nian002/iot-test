#include <limits>
#include "func.h"
#include "gtest/gtest.h"

TEST(iot_test, input_negative)
{
EXPECT_EQ(-1, fac(-1));
EXPECT_EQ(-1, fac(-2));
EXPECT_EQ(-1, fac(-5));
}
TEST(iot_test, input_zero)
{
EXPECT_EQ(1, fac(0));
}
TEST(iot_test, input_positive)
{
EXPECT_EQ(1, fac(1));
EXPECT_EQ(2, fac(2));
EXPECT_EQ(6, fac(3));
EXPECT_EQ(3628800, fac(10));
}