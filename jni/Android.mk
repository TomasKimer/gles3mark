LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := assimp
LOCAL_SRC_FILES := ../libs/prebuilt/$(TARGET_ARCH_ABI)/libassimp.so  #$(LOCAL_PATH)/
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/ 
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
FILE_LIST_JNI := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST_LIB := $(wildcard $(LOCAL_PATH)/../gles3mark_src/*.cpp)

LOCAL_MODULE    := libgles3mark
LOCAL_CFLAGS	:= -Werror
LOCAL_CPPFLAGS  := -std=c++11 #-fexceptions
LOCAL_CPP_FEATURES := exceptions

LOCAL_SRC_FILES := $(FILE_LIST_JNI:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(FILE_LIST_LIB:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += ../gles3mark_src/gl3stub_egl.c

LOCAL_LDLIBS    := -llog -lGLESv2 -lEGL -landroid
LOCAL_STATIC_LIBRARIES:= android_native_app_glue
LOCAL_SHARED_LIBRARIES := assimp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include/ #LOCAL_CFLAGS += -I/Users/Tomáš/Desktop/gles3mark/include/
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)