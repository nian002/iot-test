.PHONY : clean all

#SDK_INC := ../include
HIGHWAY_SDK_PATH:= ./highway_2.7.4_3559a_020

MPP_DIR := mpp
MPP_INC_DIR := $(MPP_DIR)/out/linux/multi-core/include
MPP_LIB_DIR := $(MPP_DIR)/out/linux/multi-core/lib

MPP_SAMPLE_DIR := $(MPP_DIR)/sample
MPP_SAMPLE_COMM_DIR := $(MPP_SAMPLE_DIR)/common
MPP_SAMPLE_SVP_DIR := $(MPP_SAMPLE_DIR)/svp/multi-core/common

#BASE64_INC_DIR := libb64/include
#BASE64_LIB_DIR := libb64/lib
#GFLAGS_INC_DIR := gflags/include
#GFLAGS_LIB_DIR := gflags/lib
GTEST_INC_DIR := gtest-pub/include
GTEST_LIB_DIR := gtest-pub/lib
OPENCV_INC_DIR := opencv/include
OPENCV_LIB_DIR := opencv/lib
#RAPIDJSON_INC_DIR := rapidjson/include

#FFMPEG_INC_DIR := ffmpeg/include
#FFMPEG_LIB_DIR := ffmpeg/lib

SENSOR0_TYPE ?= SONY_IMX334_MIPI_8M_30FPS_12BIT
SENSOR1_TYPE ?= SONY_IMX334_MIPI_8M_30FPS_12BIT
SENSOR2_TYPE ?= SONY_IMX334_MIPI_8M_30FPS_12BIT
SENSOR3_TYPE ?= SONY_IMX334_MIPI_8M_30FPS_12BIT
SENSOR4_TYPE ?= SONY_IMX277_SLVS_8M_30FPS_12BIT
SENSOR5_TYPE ?= SONY_IMX277_SLVS_8M_30FPS_12BIT
SENSOR6_TYPE ?= SONY_IMX277_SLVS_8M_30FPS_12BIT
SENSOR7_TYPE ?= SONY_IMX277_SLVS_8M_30FPS_12BIT

CC := aarch64-himix100-linux-gcc  
CXX := aarch64-himix100-linux-g++

CFLAGS += -g
CFLAGS += -I ./
CFLAGS += -I ./svp
CFLAGS += -I ./highway-test
CFLAGS += -I $(HIGHWAY_SDK_PATH)/include
CFLAGS += -I $(MPP_INC_DIR)
CFLAGS += -I $(MPP_SAMPLE_COMM_DIR)
CFLAGS += -I $(MPP_SAMPLE_SVP_DIR)
#CFLAGS += -I $(BASE64_INC_DIR)
#CFLAGS += -I $(GFLAGS_INC_DIR)
CFLAGS += -I $(GTEST_INC_DIR)
CFLAGS += -I $(OPENCV_INC_DIR)
#CFLAGS += -I $(RAPIDJSON_INC_DIR)
#CFLAGS += -I $(SDK_INC)
#CFLAGS += -I $(FFMPEG_INC_DIR)

CFLAGS += -DSENSOR0_TYPE=$(SENSOR0_TYPE)
CFLAGS += -DSENSOR1_TYPE=$(SENSOR1_TYPE)
CFLAGS += -DSENSOR2_TYPE=$(SENSOR2_TYPE)
CFLAGS += -DSENSOR3_TYPE=$(SENSOR3_TYPE)
CFLAGS += -DSENSOR4_TYPE=$(SENSOR4_TYPE)
CFLAGS += -DSENSOR5_TYPE=$(SENSOR5_TYPE)
CFLAGS += -DSENSOR6_TYPE=$(SENSOR6_TYPE)
CFLAGS += -DSENSOR7_TYPE=$(SENSOR7_TYPE)

CXXFLAGS := $(CFLAGS)

#LDFLAGS += -L $(BASE64_LIB_DIR)
LDFLAGS += -L ./highway_2.7.4_3559a_020/3559a/lib
LDFLAGS += -L $(MPP_LIB_DIR)
LDFLAGS += -L $(OPENCV_INC_DIR)../lib
#LDFLAGS += -L $(SDK_LIB)
#LDFLAGS += -L $(FFMPEG_LIB_DIR)
#LDFLAGS += -L $(GFLAGS_LIB_DIR)
LDFLAGS += -L $(GTEST_LIB_DIR)
LDFLAGS += -L $(OPENCV_LIB_DIR)
LDFLAGS += -Wl,-rpath=$(HIGHWAY_SDK_PATH)/3559a/lib
LDFLAGS += -Wl,-rpath=$(MPP_LIB_DIR)

LIBS += -lexport_sdk
LIBS += -lopencv_highgui
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
#LIBS += -lavformat
#LIBS += -lavcodec
#LIBS += -lavutil
LIBS += -ldsp
LIBS += -lisp
LIBS += -live
LIBS += -lhdmi
LIBS += -lmpi
LIBS += -lnnie
LIBS += -lsns_imx277_slvs
LIBS += -lsns_imx290
LIBS += -lsns_imx334
LIBS += -lsns_imx334_slave
LIBS += -lsns_imx477
LIBS += -ldnvqe
LIBS += -lupvqe
LIBS += -lVoiceEngine
LIBS += -lsecurec
LIBS += -l_hiae
LIBS += -l_hiawb
LIBS += -l_hidehaze
LIBS += -l_hidrc
LIBS += -l_hildci
LIBS += -lglog
#LIBS += -lgflags
#LIBS += -lb64
LIBS += -lsecurec
LIBS += -lpthread
LIBS += -lm
LIBS += -ldl
LIBS += -lstdc++
LIBS += -fopenmp
LIBS += -lgtest

SAMPLE_COMM_SRCS := $(wildcard $(MPP_SAMPLE_COMM_DIR)/*.c)
SAMPLE_COMM_OBJS := $(SAMPLE_COMM_SRCS:%.c=%.o)

SVP_SAMPLE_COMM_SRCS := $(wildcard $(MPP_SAMPLE_SVP_DIR)/*.c)
SVP_SAMPLE_COMM_OBJS := $(SVP_SAMPLE_COMM_SRCS:%.c=%.o)

SRCS := $(wildcard *.cpp)
SRCS += $(wildcard test/*.cpp)
SRCS += $(wildcard svp/*.cpp)
SRCS += $(wildcard highway-test/*.cpp)
OBJS := $(SRCS:%.cpp=%.o)

all: unit_test

unit_test: $(OBJS) $(SAMPLE_COMM_OBJS) $(SVP_SAMPLE_COMM_OBJS) 
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
#	rm $(OBJS) $(SAMPLE_COMM_OBJS) $(SVP_SAMPLE_COMM_OBJS) 

clean:
	rm -f unit_test $(OBJS) $(SAMPLE_COMM_OBJS) $(SVP_SAMPLE_COMM_OBJS)