LOCAL_PATH := $(call my-dir)
SOURCE_PATH := ../../source

include $(CLEAR_VARS)

LOCAL_MODULE    := libOpenGLRunner
LOCAL_SRC_FILES := OpenGLRunner.cpp \
				   $(SOURCE_PATH)/Game.cpp
LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)
