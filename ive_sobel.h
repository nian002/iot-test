#ifndef IVE_SOBEL_H_
#define IVE_SOBEL_H_

#include "hi_comm_ive.h"
#include "sample_comm_ive.h"

int iot_ive_sobel_proc(IVE_IMAGE_S *pstSrc, IVE_IMAGE_S *pstDstH, IVE_IMAGE_S *pstDstV);

int test_iot_ive_sobel_proc();

#endif