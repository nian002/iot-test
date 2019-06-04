#include <iostream>
#include "gtest/gtest.h"
#include "highway_util.h"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "highway_sdk*";

    // test_highway_sdk();
    return RUN_ALL_TESTS();
    return 0;
}
