#ifndef IOT_SVP_H_
#define IOT_SVP_H_

#include "hi_comm_svp.h"
#include "hi_dsp.h"

typedef struct hiSAMPLE_SVP_DSP_DILATE_S
{
    SVP_SRC_IMAGE_S stSrc;
    SVP_DST_IMAGE_S stDst;
    SVP_MEM_INFO_S  stAssistBuf;
    SVP_DSP_PRI_E   enPri;
    SVP_DSP_ID_E    enDspId;
}SAMPLE_SVP_DSP_DILATE_S;

int iot_svp_init(SAMPLE_SVP_DSP_DILATE_S *pstDilate,
    HI_U32 u32Width,HI_U32 u32Height,SVP_DSP_ID_E enDspId,SVP_DSP_PRI_E enPri);
int iot_svp_deinit();

int iot_svp_load_core_binary();
void iot_svp_unload_core_binary();

int iot_dsp_proc();

int iot_ive_sobel_proc();

#endif