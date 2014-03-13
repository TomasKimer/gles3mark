LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libgles3mark
LOCAL_CFLAGS	:= -Werror
LOCAL_CPPFLAGS  := -std=c++11 #-fexceptions
LOCAL_CPP_FEATURES := exceptions
LOCAL_SRC_FILES := baseapp.cpp \
				   gles3mark_jni.cpp \
				   ../gles3mark_src/gles3mark.cpp \
				   ../gles3mark_src/gl3stub.c \
				   ../gles3mark_src/log.cpp \
				   ../gles3mark_src/assetmanager.cpp
LOCAL_LDLIBS    := -llog -lGLESv2 -lEGL -landroid
LOCAL_STATIC_LIBRARIES:= android_native_app_glue
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include/ #LOCAL_CFLAGS += -I/Users/Tomáš/Desktop/gles3mark/include/

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)