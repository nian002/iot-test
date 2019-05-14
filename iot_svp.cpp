#include "iot_svp.h"
#include "hi_type.h"
#include "mpi_dsp.h"
#include "hi_dsp.h"
#include "sample_comm_svp.h"

int iot_svp_init()
{
    return 0;
}

int iot_svp_deinit()
{
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

int iot_svp_unload_core_binary()
{
    return 0;
}