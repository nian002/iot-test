#include <iostream>
#include "gtest/gtest.h"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "add_weighted_test*";
    return RUN_ALL_TESTS();
}
