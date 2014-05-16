
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE   := cxEngine

LOCAL_CFLAGS   := -std=gnu99 -O2 -DANDROID -I$(LOCAL_PATH)

LOCAL_SRC_FILES := \
	cxArray.c \
	cxCore.c \
	cxHash.c \
	cxList.c \
	cxMemPool.c \
	cxProperty.c \
	cxStack.c \
	cxType.c

include $(BUILD_STATIC_LIBRARY)

