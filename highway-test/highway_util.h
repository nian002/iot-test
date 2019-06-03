#ifndef HIGHWAY_UTIL_H_
#define HIGHWAY_UTIL_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

int test_highway_sdk();

int highway_sdk_get_version();

int highway_sdk_process_init();
int highway_sdk_process_uninit();

int highway_sdk_thread_init();
int highway_sdk_thread_uninit();


int highway_sdk_match_init();
int highway_sdk_match_uninit();


int highway_sdk_image_detection();
int highway_sdk_image_recognization();

int highway_sdk_video_detection();
int highway_sdk_video_recognization();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif