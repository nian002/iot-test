#ifndef SEEMMO_SIM_MATCH_LIB_H
#define SEEMMO_SIM_MATCH_LIB_H
#include <cstdlib>

#ifdef WIN32
#ifdef SEEMMO_SIM_MATCH_EXPORTS
#define SEEMMO_SIM_MATCH_API __declspec(dllexport)
#else
#define SEEMMO_SIM_MATCH_API __declspec(dllimport)
#endif
#else
#ifdef SEEMMO_SIM_MATCH_EXPORTS
#define SEEMMO_SIM_MATCH_API __attribute__((visibility("default")))
#else
#define SEEMMO_SIM_MATCH_API
#endif
#define __stdcall
#endif 

extern "C"
{
    // ��ʼ��  �Ƿ�����beta������Ĭ�Ͽ���
    SEEMMO_SIM_MATCH_API long __stdcall seemmo_match_init(bool beta_swith);
    
    // �������ƶ�
    SEEMMO_SIM_MATCH_API long __stdcall seemmo_pvc_match(
            const char* feature_src,
            int feature_src_len,
            const char* feature_to,
            int feature_to_len,
            int type,
            float* score,
            float* proi=NULL); 
}
#endif
