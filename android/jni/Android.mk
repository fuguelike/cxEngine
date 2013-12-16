
LOCAL_PATH := $(call my-dir)

LIBS_DIR    := ../../libs/android/lib
CX_ENGINE_DIR	:= ../..

include $(CLEAR_VARS)

LOCAL_MODULE    := cxEngineTest

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS    := -std=gnu99 -O2 -DANDROID -I$(CX_ENGINE_DIR) -I$(CX_ENGINE_DIR)/libs/android/include

LOCAL_SRC_FILES := cxMain.c

LOCAL_WHOLE_STATIC_LIBRARIES += cxEngine

LOCAL_STATIC_LIBRARIES := db pcre event jpeg mpg123 png freetype charset iconv libxml2 jansson

LOCAL_LDLIBS := -llog -lGLESv2 -lz -lEGL -landroid -lOpenSLES

include $(BUILD_SHARED_LIBRARY)

$(call import-module,libs/android/lib/arm)
$(call import-module,./)


