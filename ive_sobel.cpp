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
#include "sample_dsp_enca.h"
#include "opencv2/opencv.hpp"


int iot_ive_sobel_proc()
{
    // prepare dsp data and variable
    HI_S32 s32Ret;
    SVP_DSP_HANDLE hHandle;
    HI_BOOL bBlock = HI_TRUE;

    HI_S8 as8Kernel[25] = {0, 0, 0, 0, 0,
                           0, -1, 0, 1, 0,
                           0, -2, 0, 2, 0,
                           0, -1, 0, 1, 0,
                           0, 0, 0, 0, 0};
    // copy opencv mat to pstDilate
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
    // save the dst image

    return s32Ret;
}