#include "highway_util.h"
#include "sample_comm_ive.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sdk_export.h"
#include <opencv2/opencv.hpp>

#define detect_param "{\"Detect\":{\"DetectRegion\":[],\"IsDet\":true}}"
#define recogn_param "{\"Detect\":{\"DetectRegion\":[],\"IsDet\":true,\"MaxCarWidth\":0,\"MinCarWidth\":0,\"Mode\":0,\"Threshold\":20,\"Version\":1001},\"Recognize\":{\"Person\":{\"IsRec\":true},\"Feature\":{\"IsRec\":true},\"Vehicle\":{\"Brand\":{\"IsRec\":true},\"Plate\":{\"IsRec\":true},\"Color\":{\"IsRec\":true},\"Marker\":{\"IsRec\":true}}}}"

#define video_file_path "rgb24.rgb"
int video_total_frame = 3000;
int video_frame_height = 1080;
int video_frame_width = 1920;

int test_highway_sdk()
{
    // Read YUV video file frame by frame.
    FILE *video_fp;
    video_fp = fopen(video_file_path, "r");
    if (video_fp == NULL) {printf("Open Video File Error!\n");}

    uint8_t rgb24_video_buffer[video_frame_width * video_frame_height * 3];
    int read_len = fread(rgb24_video_buffer, sizeof(uint8_t), video_frame_width * video_frame_height * 3, video_fp);
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
        printf("Read %d Byte From rgb24.rgb.\n",read_len);
    }

    // using IVE for cvt yuv to rgb
    // IVE_IMAGE_S ive_src, ive_dst; 
    // int ive_create_ret = SAMPLE_COMM_IVE_CreateImageByCached(&ive_src, IVE_IMAGE_TYPE_YUV420P, video_frame_width, video_frame_height);
    // if (ive_create_ret != HI_SUCCESS) {printf("SAMPLE_COMM_IVE_CreateImageByCached failed!\n");}
    // ive_create_ret = SAMPLE_COMM_IVE_CreateImageByCached(&ive_dst, IVE_IMAGE_TYPE_U8C3_PLANAR, video_frame_width, video_frame_height);
    // if (ive_create_ret != HI_SUCCESS) {printf("SAMPLE_COMM_IVE_CreateImageByCached failed!\n");}

    // IVE_HANDLE ive_handle;
    // IVE_CSC_CTRL_S ive_csc_ctrl = {IVE_CSC_MODE_PIC_BT709_YUV2RGB};

    // int ive_csc_ret = HI_MPI_IVE_CSC(&ive_handle, &ive_src, &ive_dst, &ive_csc_ctrl, HI_FALSE);
    // if (ive_csc_ret != HI_SUCCESS) {printf("HI_MPI_IVE_CSC failed!\n");}
}

int highway_sdk_get_version()
{
    const char *pVersion = seemmo_version();
    printf("highway sdk version %s \n", pVersion);
}

int highway_sdk_process_init()
{
    int ret = seemmo_process_init("highway_2.7.4_3559a_020", 1, 1, "192.168.201.66:12821", 1, true);
    printf("seemmo_process_init ret: %d \n", ret);
}

int highway_sdk_uninit()
{
    int ret = seemmo_uninit();
    printf("seemmo_uninit ret: %d \n", ret);
}

int highway_sdk_thread_init()
{
    int ret = seemmo_thread_init(SEEMMO_LOAD_TYPE_FILTER, 0, 1);
    printf("seemmo_thread_init FILTER ret: %d \n", ret);
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
    /*
    // Read YUV video file frame by frame.
    FILE *video_fp;
    video_fp = fopen(video_file_path, "r");
    if (video_fp == NULL) {printf("Open Video File Error!\n");}

    uint8_t *rgb24_video_buffer = (uint8_t *)malloc(1920 * 1080 * 3);
    uint8_t **prgb24 = &rgb24_video_buffer;
    int read_len = fread(rgb24_video_buffer, video_frame_width * video_frame_height * 3, 1, video_fp);
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
        printf("Read %d Byte From rgb24.rgb.\n",read_len);
    }

    const uint32_t width = video_frame_width;
    const uint32_t height = video_frame_height;
    const char *cal_param = recogn_param;
    char rsp_buffer[5 * 1024 * 1024];
    int32_t buff_len = 5 * 1024 *1024;
    int32_t timeout = 2000;

    int ret = seemmo_image_pvc_recog(1, 
                                    const_cast<const uint8_t**>(prgb24), 
                                    const_cast<const uint32_t*>(&width), 
                                    const_cast<const uint32_t*>(&height), 
                                    cal_param, rsp_buffer, &buff_len, timeout);
    printf("image_pvc_recog ret %d \n", ret);

    printf("rsp_buffer result : %s \n", rsp_buffer);

    */
}

int highway_sdk_video_detection()
{
    // Read YUV video file frame by frame.
    FILE *video_fp;
    video_fp = fopen(video_file_path, "rb+");
    if (video_fp == NULL) {printf("Open Video File Error!\n");}

    uint8_t *rgb24_video_buffer = (uint8_t *)malloc(1920 * 1080 * 3);
    uint8_t *rgb24_planar = (uint8_t *)malloc(1920 * 1080 * 3);
    uint8_t **prgb24 = &rgb24_video_buffer;
    uint8_t **prgb24_planar = &rgb24_planar;

    const int32_t video_chn = 1;
    const uint32_t width = video_frame_width;
    const uint32_t height = video_frame_height;
    const char *cal_param = recogn_param;
    const char **pcal_param = &cal_param;
    char rsp_buffer[5 * 1024 * 1024];
    int32_t buff_len = 5 * 1024 *1024;
    int32_t timeout = 2000;

    for(int i = 0; i < 3000; i++)
    {
        const uint64_t timestamp = i;
        memset(rgb24_video_buffer, 0, 1920 * 1080 * 3);
        int read_len = fread(rgb24_video_buffer, video_frame_width * video_frame_height * 3, 1, video_fp);
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
            printf("Read ret : %d From rgb24.rgb. Frame Index : %d\n",read_len, i);
        }

        cv::Mat image_src = cv::Mat(1080, 1920, CV_8UC3);
        std::vector<cv::Mat> image_chns;

        memcpy(image_src.ptr(), rgb24_video_buffer, 1920 * 1080 * 3);

        cv::cvtColor(image_src, image_src, CV_RGB2BGR);
        cv::split(image_src, image_chns);

        memcpy(rgb24_planar,                   image_chns[0].ptr(), 1920 * 1080);
        memcpy(rgb24_planar + 1920 * 1080,     image_chns[1].ptr(), 1920 * 1080);
        memcpy(rgb24_planar + 1920 * 1080 * 2, image_chns[2].ptr(), 1920 * 1080);

        int ret = seemmo_video_pvc(1, &video_chn, &timestamp, 
                                        const_cast<const uint8_t**>(prgb24_planar), 
                                        const_cast<const uint32_t*>(&width), 
                                        const_cast<const uint32_t*>(&height), 
                                        pcal_param, rsp_buffer, &buff_len, timeout);
        printf("seemmo_video_pvc ret %d \n", ret);

        printf("rsp_buffer result : %s \n", rsp_buffer);

    }
}

int highway_sdk_video_recognization()
{

}