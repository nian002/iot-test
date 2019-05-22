#include "ive_sobel.h"
#include "hi_type.h"
#include "mpi_dsp.h"
#include "mpi_ive.h"
#include "mpi_sys.h"
#include "hi_dsp.h"
#include "hi_ive.h"
#include "hi_comm_video.h"
#include "hi_comm_ive.h"
#include "sample_comm_svp.h"
#include "opencv2/opencv.hpp"

int test_iot_ive_sobel_proc()
{
    SVP_IMAGE_S stSrc, stDstH, stDstV;
    HI_S32 s32Ret;

    cv::Mat cv_image = cv::imread("pkx.jpg");
    cv::cvtColor(cv_image, cv_image, CV_BGR2GRAY);

    int16_t *pDstH = 0;
    int16_t *pDstV = 0;

    cv::Mat cv_dstH = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);
    cv::Mat cv_dstV = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);

    // SRC IMAGE
    s32Ret = SAMPLE_COMM_SVP_CreateImage(&stSrc, SVP_IMAGE_TYPE_U8C1, 1280, 854, 0);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_IVE_CreateImage failed!\n", s32Ret);
    memcpy((uint8_t *)(stSrc.au64VirAddr[0]) , cv_image.ptr(), cv_image.cols * cv_image.rows);
   
    // DST IMAGE
    s32Ret = SAMPLE_COMM_SVP_CreateImage(&stDstH, SVP_IMAGE_TYPE_S16C1, 1280, 854, 0);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_IVE_CreateImage failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_SVP_CreateImage(&stDstV, SVP_IMAGE_TYPE_S16C1, 1280, 854, 0);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_IVE_CreateImage failed!\n", s32Ret);

    IVE_IMAGE_S stIVESrc, stIVEDstH, stIVEDstV;

    stIVESrc.au32Stride[0] = stSrc.au32Stride[0];
    stIVESrc.au64PhyAddr[0] = stSrc.au64PhyAddr[0];
    stIVESrc.au64VirAddr[0] = stSrc.au64VirAddr[0];
    stIVESrc.enType = IVE_IMAGE_TYPE_U8C1;
    stIVESrc.u32Height = stSrc.u32Height;
    stIVESrc.u32Width = stSrc.u32Width;

    stIVEDstH.au32Stride[0] = stDstH.au32Stride[0];
    stIVEDstH.au64PhyAddr[0] = stDstH.au64PhyAddr[0];
    stIVEDstH.au64VirAddr[0] = stDstH.au64VirAddr[0];
    stIVEDstH.enType = IVE_IMAGE_TYPE_S16C1;
    stIVEDstH.u32Height = stDstH.u32Height;
    stIVEDstH.u32Width = stDstH.u32Width;

    stIVEDstV.au32Stride[0] = stDstV.au32Stride[0];
    stIVEDstV.au64PhyAddr[0] = stDstV.au64PhyAddr[0];
    stIVEDstV.au64VirAddr[0] = stDstV.au64VirAddr[0];
    stIVEDstV.enType = IVE_IMAGE_TYPE_S16C1;
    stIVEDstV.u32Height = stDstV.u32Height;
    stIVEDstV.u32Width = stDstV.u32Width;

    s32Ret = iot_ive_sobel_proc(&stIVESrc, &stIVEDstH, &stIVEDstV);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):iot_ive_sobel_proc failed!\n", s32Ret);

    // save the dst image
    pDstH = (int16_t *)(stIVEDstH.au64VirAddr[0]);
    memcpy((int16_t *)(cv_dstH.ptr()), pDstH, 1280 * 853 * 2);

    pDstV = (int16_t *)(stIVEDstV.au64VirAddr[0]);
    memcpy((int16_t *)(cv_dstV.ptr()), pDstV, 1280 * 853 * 2);

    cv::imwrite("dstH.png", cv_dstH);
    cv::imwrite("dstV.png", cv_dstV);

    return s32Ret;
END_1:
    printf("Error!");
    return 1;
}

int iot_ive_sobel_proc(IVE_IMAGE_S *pstSrc, IVE_IMAGE_S *pstDstH, IVE_IMAGE_S *pstDstV)
{
    // prepare data and variable
    HI_S32 s32Ret;
    IVE_HANDLE hHandle;
    HI_BOOL bBlock = HI_TRUE;

    HI_S8 as8Kernel[25] = {0, 0, 0, 0, 0,
                           0, -1, 0, 1, 0,
                           0, -2, 0, 2, 0,
                           0, -1, 0, 1, 0,
                           0, 0, 0, 0, 0};

    IVE_SOBEL_CTRL_S stSobelCtrl;
    stSobelCtrl.enOutCtrl = IVE_SOBEL_OUT_CTRL_BOTH;
    memcpy(stSobelCtrl.as8Mask, as8Kernel, 25);

    while(HI_ERR_SVP_DSP_QUERY_TIMEOUT == (s32Ret =  HI_MPI_IVE_Sobel(&hHandle, pstSrc, pstDstH, pstDstV, &stSobelCtrl, bBlock)))
    {
        usleep(100);
    }

    s32Ret = HI_MPI_SYS_MflushCache(pstDstH->au64PhyAddr[0], (int16_t*)(pstDstH->au64VirAddr[0]), pstDstH->u32Height * pstDstH->u32Height * 2);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):HI_MPI_SYS_MflushCache failed!\n", s32Ret);
    
    s32Ret = HI_MPI_SYS_MflushCache(pstDstV->au64PhyAddr[0], (int16_t*)(pstDstV->au64VirAddr[0]), pstDstV->u32Height * pstDstV->u32Height * 2);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_1, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):HI_MPI_SYS_MflushCache failed!\n", s32Ret);

    return s32Ret;

END_1:
    return s32Ret;
}