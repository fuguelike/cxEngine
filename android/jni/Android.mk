

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cxEngineAndroid
LOCAL_CFLAGS := -std=gnu99 -O2 -DANDROID -I$(CX_ENGINE_PATH) -I$(CX_ENGINE_PATH)/libs/android/include
LOCAL_WHOLE_STATIC_LIBRARIES += cxengine
LOCAL_STATIC_LIBRARIES := pcre jpeg png uv jansson
LOCAL_LDLIBS := -llog -lGLESv2 -lz -landroid -lEGL -lOpenSLES

LOCAL_SRC_FILES := ../../test/main.c


include $(BUILD_SHARED_LIBRARY)
$(call import-module,libs/android/lib/$(TARGET_ARCH_ABI))
$(call import-module,.)