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

TEST(highway_sdk, test_thread_init)
{
    highway_sdk_thread_init();
}

/*
TEST(highway_sdk, test_image_recognition)
{
    highway_sdk_image_recognization();
}
*/

TEST(highway_sdk, test_video_detection)
{
    highway_sdk_video_detection();
}