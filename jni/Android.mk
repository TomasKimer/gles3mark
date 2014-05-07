LOCAL_PATH := $(call my-dir)

# assimp - prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := assimp
LOCAL_SRC_FILES := ../libs/prebuilt/$(TARGET_ARCH_ABI)/libassimp.so  #$(LOCAL_PATH)/
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/ 
include $(PREBUILT_SHARED_LIBRARY)

# ktx - prebuilt
#include $(CLEAR_VARS)
#LOCAL_MODULE := ktx
#LOCAL_SRC_FILES := ../libs/prebuilt/$(TARGET_ARCH_ABI)/libktx.so
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/ 
#include $(PREBUILT_SHARED_LIBRARY)

#ktx - build
include $(CLEAR_VARS)
LOCAL_MODULE := ktx
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/
FILE_LIST_KTX := $(wildcard $(LOCAL_PATH)/../libs/ktx/*.c)
LOCAL_SRC_FILES := $(FILE_LIST_KTX:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += ../libs/ktx/etcdec.cxx \
#                   ../libs/ktx/etcunpack.cxx
# LOCAL_LDLIBS   = -lGLESv2
LOCAL_CFLAGS   = -std=c99 -DKTX_OPENGL_ES2=1 -DSUPPORT_SOFTWARE_ETC_UNPACK=0
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/ 
include $(BUILD_STATIC_LIBRARY)


# gles3mark lib - main
include $(CLEAR_VARS)
FILE_LIST_JNI := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST_LIB := $(wildcard $(LOCAL_PATH)/../gles3mark/*.cpp)

LOCAL_MODULE    := libgles3mark
LOCAL_CFLAGS	:= -Werror
LOCAL_CPPFLAGS  := -std=c++11 -D_DEBUG -DGLM_FORCE_RADIANS #-O3 -DNDEBUG #-fexceptions
LOCAL_CPP_FEATURES := exceptions

LOCAL_SRC_FILES := $(FILE_LIST_JNI:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(FILE_LIST_LIB:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += ../gles3mark/gl3stub_egl.c

LOCAL_LDLIBS    := -llog -lGLESv2 -lEGL -landroid
LOCAL_STATIC_LIBRARIES:= android_native_app_glue ktx
LOCAL_SHARED_LIBRARIES := assimp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include/ #LOCAL_CFLAGS += -I/Users/Tomáš/Desktop/gles3mark/include/
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)