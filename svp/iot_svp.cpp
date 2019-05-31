#include "iot_svp.h"
#include "hi_type.h"
#include "mpi_dsp.h"
#include "mpi_ive.h"
#include "mpi_sys.h"
#include "hi_dsp.h"
#include "hi_ive.h"
#include "hi_comm_video.h"
#include "hi_comm_ive.h"
#include "sample_comm_svp.h"
#include "sample_dsp_enca.h"
#include "opencv2/opencv.hpp"


SAMPLE_SVP_DSP_DILATE_S s_stDilate = {0};

int iot_svp_init(SAMPLE_SVP_DSP_DILATE_S *pstDilate,
    HI_U32 u32Width,HI_U32 u32Height,SVP_DSP_ID_E enDspId,SVP_DSP_PRI_E enPri)
{
    HI_S32 s32Ret;
    HI_U32 u32Size = sizeof(SVP_IMAGE_S) * 2;
    memset(pstDilate,0,sizeof(*pstDilate));

    pstDilate->stSrc.u32Width  = u32Width;
    pstDilate->stSrc.u32Height = u32Height;
    pstDilate->stSrc.enType    = SVP_IMAGE_TYPE_U8C1;

    s32Ret = SAMPLE_COMM_SVP_CreateImage(&(pstDilate->stSrc),SVP_IMAGE_TYPE_U8C1,u32Width,u32Height,0);
    SAMPLE_SVP_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_SVP_CreateImage failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_SVP_CreateImage(&(pstDilate->stDst),SVP_IMAGE_TYPE_S16C1,u32Width,u32Height,0);
    SAMPLE_SVP_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_SVP_CreateImage failed!\n", s32Ret);

    s32Ret = SAMPLE_COMM_SVP_CreateMemInfo(&(pstDilate->stAssistBuf),u32Size,0);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, FAIL_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_SVP_CreateMemInfo failed!\n", s32Ret);

    pstDilate->enDspId = enDspId;
    pstDilate->enPri   = enPri;
    return s32Ret;
FAIL_0:
    SAMPLE_COMM_SVP_DestroyImage(&(pstDilate->stDst),0);
    memset(pstDilate,0,sizeof(*pstDilate));
    return s32Ret;
}

int iot_svp_deinit()
{
    SAMPLE_SVP_DSP_DILATE_S *pstDilate = &s_stDilate;
    SAMPLE_COMM_SVP_DestroyMemInfo(&(pstDilate->stAssistBuf),0);
    SAMPLE_COMM_SVP_DestroyImage(&(pstDilate->stSrc),0);
    SAMPLE_COMM_SVP_DestroyImage(&(pstDilate->stDst),0);
    return 0;
}

int iot_svp_load_core_binary()
{
    /*Load bin*/
    HI_S32 s32Ret;
    SVP_DSP_ID_E enDspId = SVP_DSP_ID_0;
    s32Ret = SAMPLE_COMM_SVP_LoadCoreBinary(enDspId);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret,END_DSP_0, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_SVP_LoadCoreBinary failed!\n", s32Ret);

    return s32Ret;

END_DSP_0:
    printf("SAMPLE_COMM_SVP_LoadCoreBinary Error!");
    return -1;
}

void iot_svp_unload_core_binary()
{
    SVP_DSP_ID_E enDspId = SVP_DSP_ID_0;
    SAMPLE_COMM_SVP_UnLoadCoreBinary(enDspId);
}


int iot_dsp_proc()
{
    SAMPLE_SVP_DSP_DILATE_S *pstDilate = &s_stDilate;

    // preapare opencv mat image
    cv::Mat cv_image = cv::imread("pkx.jpg");
    std::vector<cv::Mat> mv;
    cv::split(cv_image,mv);

    cv::Mat cv_dstB = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);
    cv::Mat cv_dstG = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);
    cv::Mat cv_dstR = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);
    //cv::Mat stSrcY = cv::Mat(1280, 853 ,CV_8UC1);
    //FILE *fp = fopen("dst.dat", "wb+");
    // cv::imwrite("B.jpg", mv[0]);
    // cv::imwrite("G.jpg", mv[1]);
    // cv::imwrite("R.jpg", mv[2]);

    cv::Mat b_chn = mv[0];
    cv::Mat g_chn = mv[1];
    cv::Mat r_chn = mv[2];

    uint8_t *pb_chn = mv[0].ptr();
    uint8_t *pg_chn = mv[1].ptr();
    uint8_t *pr_chn = mv[2].ptr();

    // uint8_t *pSrcB = (uint8_t *)pstDilate->stSrc.au64VirAddr[0];
    // uint8_t *pSrcG = (uint8_t *)pstDilate->stSrc.au64VirAddr[1];
    // uint8_t *pSrcR = (uint8_t *)pstDilate->stSrc.au64VirAddr[2];
    uint8_t *pSrcB = 0;
    uint8_t *pSrcG = 0;
    uint8_t *pSrcR = 0;

    // uint8_t *pDstB= (uint8_t *)pstDilate->stDst.au64VirAddr[0];
    // uint8_t *pDstG = (uint8_t *)pstDilate->stDst.au64VirAddr[0];
    // uint8_t *pDstR = (uint8_t *)pstDilate->stDst.au64VirAddr[0];

    int16_t *pDstB = 0;
    int16_t *pDstG = 0;
    int16_t *pDstR = 0;

    // prepare dsp data and variable
    HI_S32 s32Ret;
    SVP_DSP_ID_E enDspId = SVP_DSP_ID_0;
    SVP_DSP_PRI_E enPri = SVP_DSP_PRI_0;
    SIZE_S stSize;
    stSize.u32Width = 1280;
    stSize.u32Height = 853;

    SVP_DSP_HANDLE hHandle;
    HI_BOOL bFinish;
    HI_BOOL bBlock = HI_TRUE;

    /*Load bin*/
    s32Ret = SAMPLE_COMM_SVP_LoadCoreBinary(enDspId);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret,END_DSP_2, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_COMM_SVP_LoadCoreBinary failed!\n", s32Ret);
    /*Init*/
    s32Ret = iot_svp_init(&s_stDilate,stSize.u32Width,stSize.u32Height,enDspId,enPri);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_DSP_2, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_SVP_DSP_DilateInit failed!\n", s32Ret);

    // copy opencv mat to pstDilate
    pSrcB = (uint8_t *)(pstDilate->stSrc.au64VirAddr[0]);
    // pSrcG = (uint8_t *)(pstDilate->stSrc.au64VirAddr[1]);
    // pSrcR = (uint8_t *)(pstDilate->stSrc.au64VirAddr[2]);
    memcpy(pSrcB, pb_chn, cv_image.cols * cv_image.rows);
    // memcpy(pSrcG, pg_chn, cv_image.cols * cv_image.rows);
    // memcpy(pSrcR, pr_chn, cv_image.cols * cv_image.rows);
    //fwrite((void*)(pstDilate->stSrc.au64VirAddr[0]), 1, 1280 * 853, fp);
    //fclose(fp);

    /*Call enca mpi*/
    s32Ret = SAMPLE_SVP_DSP_ENCA_Dilate3x3(&hHandle, pstDilate->enDspId,pstDilate->enPri, &pstDilate->stSrc, &pstDilate->stDst, &(pstDilate->stAssistBuf));
    SAMPLE_SVP_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):HI_MPI_SVP_DSP_ENCA_Dilate3x3 failed!\n", s32Ret);
    /*Wait dsp finish*/
    while(HI_ERR_SVP_DSP_QUERY_TIMEOUT == (s32Ret = HI_MPI_SVP_DSP_Query(pstDilate->enDspId, hHandle, &bFinish, bBlock)))
    {
        usleep(100);
    }

    // save the dst image
    pDstB = (int16_t *)(pstDilate->stDst.au64VirAddr[0]);
    // pDstG = (uint8_t *)(pstDilate->stDst.au64VirAddr[1]);
    // pDstR = (uint8_t *)(pstDilate->stDst.au64VirAddr[2]);
    memcpy(cv_dstB.ptr(), pDstB, stSize.u32Height * stSize.u32Width * 2);
    // memcpy(cv_dstG.ptr(), pDstG, stSize.u32Height * stSize.u32Width);
    // memcpy(cv_dstR.ptr(), pDstR, stSize.u32Height * stSize.u32Width);
    cv::imwrite("dstB.png", cv_dstB);
    // cv::imwrite("dstG.jpg", cv_dstG);
    // cv::imwrite("dstR.jpg", cv_dstR);

    SAMPLE_COMM_SVP_UnLoadCoreBinary(enDspId);
    iot_svp_deinit();


    return s32Ret;
END_DSP_2:
    SAMPLE_COMM_SVP_UnLoadCoreBinary(enDspId);
    return -1;
}


int iot_ive_sobel_proc()
{
    SAMPLE_SVP_DSP_DILATE_S *pstDilate = &s_stDilate;

    // preapare opencv mat image
    cv::Mat cv_image = cv::imread("pkx.jpg");
    std::vector<cv::Mat> mv;
    cv::split(cv_image,mv);

    cv::Mat cv_dstB = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);
    cv::Mat cv_dstG = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);
    cv::Mat cv_dstR = cv::Mat(cv_image.rows, cv_image.cols, CV_16SC1);

    cv::Mat b_chn = mv[0];
    cv::Mat g_chn = mv[1];
    cv::Mat r_chn = mv[2];

    uint8_t *pb_chn = mv[0].ptr();
    uint8_t *pg_chn = mv[1].ptr();
    uint8_t *pr_chn = mv[2].ptr();

    uint8_t *pSrcB = 0;
    uint8_t *pSrcG = 0;
    uint8_t *pSrcR = 0;

    int16_t *pDstB = 0;
    int16_t *pDstG = 0;
    int16_t *pDstR = 0;

    // prepare dsp data and variable
    HI_S32 s32Ret;
    SVP_DSP_ID_E enDspId = SVP_DSP_ID_0;
    SVP_DSP_PRI_E enPri = SVP_DSP_PRI_0;
    SIZE_S stSize;
    stSize.u32Width = 1280;
    stSize.u32Height = 854;

    SVP_DSP_HANDLE hHandle;
    HI_BOOL bBlock = HI_TRUE;

    HI_S8 as8Kernel[25] = {0, 0, 0, 0, 0,
                           0, -1, 0, 1, 0,
                           0, -2, 0, 2, 0,
                           0, -1, 0, 1, 0,
                           0, 0, 0, 0, 0};
    /*Init*/
    s32Ret = iot_svp_init(&s_stDilate,stSize.u32Width,stSize.u32Height,enDspId,enPri);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_DSP_2, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):SAMPLE_SVP_DSP_DilateInit failed!\n", s32Ret);

    // copy opencv mat to pstDilate
    pSrcB = (uint8_t *)(pstDilate->stSrc.au64VirAddr[0]);
    memcpy(pSrcB, pb_chn, cv_image.cols * cv_image.rows);

    IVE_IMAGE_S stIVESrc, stIVEDstH, stIVEDstV;

    stIVESrc.au32Stride[0] = pstDilate->stSrc.au32Stride[0];
    stIVESrc.au64PhyAddr[0] = pstDilate->stSrc.au64PhyAddr[0];
    stIVESrc.au64VirAddr[0] = pstDilate->stSrc.au64VirAddr[0];
    stIVESrc.enType = IVE_IMAGE_TYPE_U8C1;
    stIVESrc.u32Height = pstDilate->stSrc.u32Height;
    stIVESrc.u32Width = pstDilate->stSrc.u32Width;
    
    stIVEDstH.au32Stride[0] = pstDilate->stDst.au32Stride[0];
    stIVEDstH.au64PhyAddr[0] = pstDilate->stDst.au64PhyAddr[0];
    stIVEDstH.au64VirAddr[0] = pstDilate->stDst.au64VirAddr[0];
    stIVEDstH.enType = IVE_IMAGE_TYPE_S16C1;
    stIVEDstH.u32Height = pstDilate->stDst.u32Height;
    stIVEDstH.u32Width = pstDilate->stDst.u32Width;

    IVE_SOBEL_CTRL_S stSobelCtrl;
    stSobelCtrl.enOutCtrl = IVE_SOBEL_OUT_CTRL_HOR;
    memcpy(stSobelCtrl.as8Mask, as8Kernel, 25);

    while(HI_ERR_SVP_DSP_QUERY_TIMEOUT == (s32Ret =  HI_MPI_IVE_Sobel(&hHandle, &stIVESrc, &stIVEDstH, &stIVEDstV, &stSobelCtrl, bBlock)))
    {
        usleep(100);
    }

    s32Ret = HI_MPI_SYS_MflushCache(stIVEDstH.au64PhyAddr[0], (int16_t*)(stIVEDstH.au64VirAddr[0]), stIVEDstH.u32Height * stIVEDstH.u32Height * 2);
    SAMPLE_SVP_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, END_DSP_2, SAMPLE_SVP_ERR_LEVEL_ERROR, "Error(%#x):HI_MPI_SYS_MflushCache failed!\n", s32Ret);
    // save the dst image
    pDstB = (int16_t *)(pstDilate->stDst.au64VirAddr[0]);
    memcpy(cv_dstB.ptr(), pDstB, stSize.u32Height * stSize.u32Width * 2);
    cv::imwrite("dstB.png", cv_dstB);

    iot_svp_deinit();

    return s32Ret;

END_DSP_2:
    return -1;
}