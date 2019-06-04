#include "gtest/gtest.h"
#include "highway_util.h"

TEST(highway_sdk, test_get_version)
{
    highway_sdk_get_version();
}

TEST(highway_sdk, test_process_init)
{
    highway_sdk_process_init();
}