#include <limits>
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"

TEST(resize_test, read_pkx_image)
{
    cv::Mat image = cv::imread("pkx.jpg");
    EXPECT_EQ(1280, image.cols);
    EXPECT_EQ(853, image.rows);
}
