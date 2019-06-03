#include "highway_util.h"
#include "sample_comm_ive.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#define video_file_path "zhu_1080.yuv"
int video_total_frame = 3000;
int video_frame_height = 1080;
int video_frame_width = 1920;


int test_highway_sdk()
{
    // Read YUV video file frame by frame.
    FILE *video_fp;
    video_fp = fopen(video_file_path, "r");
    if (video_fp == NULL) {printf("Open Video File Error!\n");}

    uint8_t yuv420_video_buffer[video_frame_width * video_frame_height * 2];
    int read_len = fread(yuv420_video_buffer, video_frame_width * video_frame_height * 1.5, 1, video_fp);
    if(read_len == -1)
    {
        printf("Video File read error!\n");
    }
    else if(read_len == 0)
    {
        printf("Video File read Over!\n");
    }
    else 
    {
        printf("Read %d Byte From zhu_1080.yuv.\n",read_len);
    }

    // using IVE for cvt yuv to rgb
    IVE_IMAGE_S ive_src, ive_dst; 
    int ive_create_ret = SAMPLE_COMM_IVE_CreateImageByCached(&ive_src, IVE_IMAGE_TYPE_YUV420P, video_frame_width, video_frame_height);
    if (ive_create_ret != HI_SUCCESS) {printf("SAMPLE_COMM_IVE_CreateImageByCached failed!\n");}
    ive_create_ret = SAMPLE_COMM_IVE_CreateImageByCached(&ive_dst, IVE_IMAGE_TYPE_U8C3_PLANAR, video_frame_width, video_frame_height);
    if (ive_create_ret != HI_SUCCESS) {printf("SAMPLE_COMM_IVE_CreateImageByCached failed!\n");}

    IVE_HANDLE ive_handle;
    IVE_CSC_CTRL_S ive_csc_ctrl = {IVE_CSC_MODE_PIC_BT709_YUV2RGB};

    int ive_csc_ret = HI_MPI_IVE_CSC(&ive_handle, &ive_src, &ive_dst, &ive_csc_ctrl, HI_FALSE);
    if (ive_csc_ret != HI_SUCCESS) {printf("HI_MPI_IVE_CSC failed!\n");}

    

}

int highway_sdk_get_version()
{

}

int highway_sdk_process_init()
{

}

int highway_sdk_process_uninit()
{

}

int highway_sdk_thread_init()
{


}

int highway_sdk_thread_uninit()
{

}

int highway_sdk_match_init()
{

}

int highway_sdk_match_uninit()
{

}

int highway_sdk_image_detection()
{

}

int highway_sdk_image_recognization()
{

}

int highway_sdk_video_detection()
{

}

int highway_sdk_video_recognization()
{

}