LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := assimp
LOCAL_SRC_FILES := ../libs/prebuilt/$(TARGET_ARCH_ABI)/libassimp.so  #$(LOCAL_PATH)/
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include/ 
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := libgles3mark
LOCAL_CFLAGS	:= -Werror
LOCAL_CPPFLAGS  := -std=c++11 #-fexceptions
LOCAL_CPP_FEATURES := exceptions
LOCAL_SRC_FILES := baseapp.cpp \
				   app.cpp \
				   main.cpp \
				   ../gles3mark_src/gl3context_egl.cpp \
				   ../gles3mark_src/gles3mark.cpp \
				   ../gles3mark_src/gl3stub.c \
				   ../gles3mark_src/log.cpp \
				   ../gles3mark_src/assetmanager.cpp
LOCAL_LDLIBS    := -llog -lGLESv2 -lEGL -landroid
LOCAL_STATIC_LIBRARIES:= android_native_app_glue assimp
LOCAL_SHARED_LIBRARIES := assimp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include/ #LOCAL_CFLAGS += -I/Users/Tomáš/Desktop/gles3mark/include/
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)