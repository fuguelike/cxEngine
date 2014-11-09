
LOCAL_PATH := $(call my-dir)

LIBS_DIR        := ../../libs/android/lib

CX_ENGINE_DIR	:= ../..

include $(CLEAR_VARS)

LOCAL_MODULE   := cxEngineTest

LOCAL_CFLAGS   := -std=gnu99 -O2 -DANDROID -I$(CX_ENGINE_DIR) -I$(CX_ENGINE_DIR)/libs/android/include

LOCAL_SRC_FILES := ../../test/main.c 

LOCAL_WHOLE_STATIC_LIBRARIES += cxEngine

LOCAL_STATIC_LIBRARIES := pcre event jpeg png charset iconv luajit

LOCAL_LDLIBS := -llog -lGLESv2 -lz -landroid

include $(BUILD_SHARED_LIBRARY)

$(call import-module,libs/android/lib/$(TARGET_ARCH_ABI))
$(call import-module,.)


