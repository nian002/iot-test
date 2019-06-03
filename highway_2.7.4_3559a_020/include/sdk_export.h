#ifndef SEEMMO_GPU_SDK_EXPORT_H
#define SEEMMO_GPU_SDK_EXPORT_H
#include <stdint.h>

#ifdef WIN32
#ifdef SEEMMO_GPU_EXPORTS
#define SEEMMO_GPU_API __declspec(dllexport)
#else
#define SEEMMO_GPU_API __declspec(dllimport)
#endif
#else
#ifdef SEEMMO_GPU_EXPORTS
#define SEEMMO_GPU_API __attribute__((visibility("default")))
#else
#define SEEMMO_GPU_API
#endif
#define __stdcall
#endif  // WIN32

#define SEEMMO_SUCC                          0
#define SEEMMO_INIT_LOG_DIR_FAIL            -1
#define SEEMMO_INIT_LOG_INIT_FAIL           -2
#define SEEMMO_INIT_REPREAT_INIT            -3
#define SEEMMO_INIT_SDK_INIT_FAIL           -4
#define SEEMMO_INIT_SDK_NEW_FAIL            -5
#define SEEMMO_INIT_LOAD_MODEL_FAIL         -6
#define SEEMMO_RECOG_NULL_POINTER           -7
#define SEEMMO_RECOG_MAT_FAIL               -8
#define SEEMMO_RECOG_EMPTY_IMAGE            -9
#define SEEMMO_RECOG_ALGPROC_FAIL           -10
#define SEEMMO_RECOG_RSPBUF_SMALL           -11
#define SEEMMO_RECOG_NOT_INIT               -12
#define SEEMMO_THREAD_NOT_INIT              -13
#define SEEMMO_BASE64_DECODE                -14
#define SEEMMO_EMPTY_REQUEST                -15
#define SEEMMO_INIT_LIMIT_CAPACITY          -16
#define SEEMMO_PARAM_TYPE_FAIL				-17
#define SEEMMO_ERROR_INVALID_LICENCE        -110

//线程加载type定义
#define SEEMMO_LOAD_TYPE_FILTER     1 
#define SEEMMO_LOAD_TYPE_RECOG      2 
#define SEEMMO_LOAD_TYPE_ALL        3   
#define SEEMMO_LOAD_TYPE_FACE       4   

// 人车分离 type 定义
static const int UNKNOWN = 0;               // 未知
static const int PEDESTRAIN = 1;            // 行人
static const int BIKE = 2;                  // 自行车
static const int MOTOBIKE = 3;              // 摩托车，电动车
static const int CAR = 4;                   // 小汽车
static const int TRICYCLE = 5;              // 三轮车
static const int BUS = 6;                   // 大巴
static const int VAN = 7;                   // 面包车
static const int TRUCK = 8;                 // 卡车

extern "C" {
    // 获取版本信息
    SEEMMO_GPU_API const char * __stdcall  seemmo_version(void); 

    // 进程级初始化
    SEEMMO_GPU_API int32_t __stdcall seemmo_process_init(
            const char* base_dir,
            uint32_t img_core_num, 
            uint32_t video_core_num, 
            const char* auth_server, 
            int32_t auth_type,
            bool log_init_swith);

    // 线程级初始化，线程与device与模型绑定

    /**
     * @brief seemmo_thread_init 
     *
     * @param device
     * @param batch
     * @param type: 1,检测跟踪 2，异步识别 3，同时加载
     *
     * @return 
     */
    SEEMMO_GPU_API int32_t __stdcall seemmo_thread_init(int type, int device, int batch = 0);

    // 线程级清理
    SEEMMO_GPU_API int32_t __stdcall seemmo_thread_uninit();

    // 退出
    SEEMMO_GPU_API int32_t __stdcall seemmo_uninit(void); 
       
    // 视频结构化
    SEEMMO_GPU_API int32_t __stdcall seemmo_video_pvc(
            int32_t frame_num,
            const int32_t* video_chn,
            const uint64_t* timestamp,
            const uint8_t** pbgr24,
            const uint32_t* height,
            const uint32_t* width,
            const char** calc_params,
            char* rsp_buf,
            int32_t* buff_len,
            int32_t timeout);

    // 视频结构化结束帧, 单独调空帧
    SEEMMO_GPU_API int32_t __stdcall seemmo_video_pvc_end(
            int32_t frame_num,
            const int32_t* video_chn,
            const char** calc_params,
            char* rsp_buf,
            int32_t* buff_len,
            int32_t timeout);
 
    // 用于图片的pvc识别
    SEEMMO_GPU_API int32_t __stdcall seemmo_image_pvc_recog(
            int32_t image_num,
            const uint8_t** ppbgr24,
            const uint32_t* height,
            const uint32_t* width,
            const char* calc_param, 
            char* rsp_buf,
            int32_t* buff_len,
            int32_t timeout);
   
    // 用于图片的人脸识别(每个图片只有一个人脸)
    SEEMMO_GPU_API int32_t __stdcall seemmo_image_face_recog(
            int32_t image_num, 
            const uint8_t** ppbgr24, 
            const uint32_t *height, 
            const uint32_t *width, 
            const char* calc_params, 
            char* rsp_buf, 
            int32_t* buff_len,
            int32_t timeout = 0);

    // 用于视频流的pvc识别
    SEEMMO_GPU_API int32_t __stdcall seemmo_video_pvc_recog(
            int32_t image_num,
            const char** guids,
            const char* calc_param,
            char* rsp_buf,
            int32_t* buff_len,
            uint8_t** ppbgr24_buf,
            int32_t timeout = 0);

    // 用于视频流的人脸识别
    SEEMMO_GPU_API int32_t __stdcall seemmo_video_face_recog(
            int32_t image_num,
            const char** guids,
            const char* calc_param,
            char* rsp_buf,
            int32_t* buff_len,
            uint8_t** ppbgr24_buf,
            int32_t timeout = 0);

    // 函数返回函数调用状态, 0表示成功，非0表示失败
    // 该接口负责获取跟踪的图片，同时允许去除跟踪过程中的图片缓存
    // input: 
    //      image_num 与guids对应的图片数量
    //      guids 需要获取的跟踪物体的guids
    //      image_flags 与guids对应的获取的图片的状态，0表示获取图片失败，1表示成功
    //      ppbgr24_buf 用于存储跟踪过程的图片，若设置为NULL，则用于删除跟踪过程缓存的图片
    //      type 0---获取pvc过程的跟踪物体 1---获取face跟踪过程中的人脸
    SEEMMO_GPU_API int32_t __stdcall seemmo_get_filter_images(
            int32_t image_num,
            const char** guids,
            uint8_t* image_flags,
            uint8_t** ppbgr24_buf,
            int type = 0,
            int32_t timeout = 0);

}

#endif
