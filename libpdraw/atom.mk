
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libpdraw
LOCAL_DESCRIPTION := Parrot Drones Awesome Video Viewer library
LOCAL_CATEGORY_PATH := libs
LOCAL_SRC_FILES := \
	src/pdraw_wrapper.cpp \
	src/pdraw_session.cpp \
	src/pdraw_settings.cpp \
	src/pdraw_media_video.cpp \
	src/pdraw_demuxer_stream.cpp \
	src/pdraw_demuxer_stream_net.cpp \
	src/pdraw_demuxer_stream_mux.cpp \
	src/pdraw_demuxer_record.cpp \
	src/pdraw_socket_inet.cpp \
	src/pdraw_utils.cpp \
	src/pdraw_metadata_session.cpp \
	src/pdraw_metadata_videoframe.cpp \
	src/pdraw_avcdecoder.cpp \
	src/pdraw_gles2_hud.cpp \
	src/pdraw_gles2_video.cpp \
	src/pdraw_gles2_hmd.cpp \
	src/pdraw_gles2_hmd_shaders.cpp \
	src/pdraw_gles2_hmd_cockpitglasses_indices.cpp \
	src/pdraw_gles2_hmd_cockpitglasses_colors.cpp \
	src/pdraw_gles2_hmd_cockpitglasses_positions.cpp \
	src/pdraw_gles2_hmd_cockpitglasses_texcoords_red.cpp \
	src/pdraw_gles2_hmd_cockpitglasses_texcoords_green.cpp \
	src/pdraw_gles2_hmd_cockpitglasses_texcoords_blue.cpp \
	src/pdraw_gles2_hmd_cockpitglasses2_indices.cpp \
	src/pdraw_gles2_hmd_cockpitglasses2_colors.cpp \
	src/pdraw_gles2_hmd_cockpitglasses2_positions.cpp \
	src/pdraw_gles2_hmd_cockpitglasses2_texcoords_red.cpp \
	src/pdraw_gles2_hmd_cockpitglasses2_texcoords_green.cpp \
	src/pdraw_gles2_hmd_cockpitglasses2_texcoords_blue.cpp \
	src/pdraw_renderer.cpp \
	src/pdraw_renderer_gles2.cpp \
	src/pdraw_renderer_videocoreegl.cpp \
	src/pdraw_filter_videoframe.cpp
LOCAL_EXPORT_CXXFLAGS := -std=c++0x
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LIBRARIES := \
	libulog \
	libfutils \
	libpomp \
	libvideo-buffers \
	libvideo-buffers-generic \
	libvideo-decode \
	libvideo-metadata \
	libvideo-streaming \
	libmp4 \
	librtsp \
	libsdp \
	libh264 \
	eigen
LOCAL_CONDITIONAL_LIBRARIES := \
	OPTIONAL:libmux


ifeq ("$(TARGET_OS)-$(TARGET_OS_FLAVOUR)","linux-native")
  LOCAL_CFLAGS += -DUSE_FFMPEG -DUSE_GLES2
  LOCAL_LIBRARIES += \
	ffmpeg-libav \
	opengl \
	glfw3
else ifeq ("$(TARGET_OS)-$(TARGET_OS_FLAVOUR)","linux-android")
  LOCAL_CFLAGS += -DUSE_MEDIACODEC -DUSE_GLES2
  LOCAL_LDLIBS += -lEGL -lGLESv2 -landroid
  LOCAL_LIBRARIES += \
	libmediacodec-wrapper
else ifeq ("$(TARGET_OS)-$(TARGET_OS_FLAVOUR)","darwin-native")
  LOCAL_CFLAGS += -DUSE_VIDEOTOOLBOX -DUSE_GLES2
  LOCAL_LDLIBS += \
	-framework Foundation \
	-framework CoreMedia \
	-framework CoreVideo \
	-framework VideoToolbox \
	-framework OpenGL
  LOCAL_LIBRARIES += \
	glfw3
else ifeq ("$(TARGET_OS)-$(TARGET_OS_FLAVOUR)","darwin-iphoneos")
  LOCAL_CFLAGS += -DUSE_VIDEOTOOLBOX -DUSE_GLES2
  LOCAL_LDLIBS += \
	-framework Foundation \
	-framework CoreMedia \
	-framework CoreVideo \
	-framework VideoToolbox \
	-framework OpenGLES
  LOCAL_INSTALL_HEADERS := \
	$(LOCAL_PATH)/include/pdraw/pdraw.h:usr/include/pdraw/ \
	$(LOCAL_PATH)/include/pdraw/pdraw.hpp:usr/include/pdraw/ \
	$(LOCAL_PATH)/include/pdraw/pdraw_defs.h:usr/include/pdraw/
else ifeq ("$(TARGET_OS)-$(TARGET_OS_FLAVOUR)-$(TARGET_PRODUCT_VARIANT)","linux-generic-raspi")
  LOCAL_CFLAGS += -DBCM_VIDEOCORE -DUSE_VIDEOCOREEGL -DUSE_GLES2
  LOCAL_LDLIBS += -L$(SDKSTAGE)/opt/vc/lib -lbrcmGLESv2 -lbrcmEGL
endif

include $(BUILD_LIBRARY)


ifneq ("$(shell which python-config)","")
ifneq ("$(shell which swig)","")

include $(CLEAR_VARS)

LOCAL_MODULE := libpdraw_python
LOCAL_DESCRIPTION := PDrAW wrapper to python using SWIG
LOCAL_CATEGORY_PATH := multimedia

LOCAL_LIBRARIES := libpdraw
LOCAL_CONFIG_FILES := python/aconfig.in
$(call load-config)

PDRAW_PYTHON_SWIG_WRAPPER := pdraw_python.cpp
PDRAW_PATH=$(LOCAL_PATH)
PDRAW_PYTHON_BUILD_DIR := $(call local-get-build-dir)

LOCAL_C_INCLUDES := $(shell find $(PDRAW_PATH)/include $(PDRAW_PATH)/src -type d)

PDRAW_PYTHON_SWIG_C_INCLUDES := $(foreach f, $(LOCAL_C_INCLUDES), -I$(f))

LOCAL_GENERATED_SRC_FILES := $(PDRAW_PYTHON_SWIG_WRAPPER)
LOCAL_MODULE_FILENAME := _$(LOCAL_MODULE).so
LOCAL_DESTDIR := usr/lib/python

PDRAW_PYTHON_DESTDIR := $(TARGET_OUT_STAGING)/usr/lib/python

ifeq ($(CONFIG_LIBPDRAW_PYTHON_PYTHON3),y)
  PDRAW_PYTHON_PYTHONCONFIG := python3-config
else
  PDRAW_PYTHON_PYTHONCONFIG := python-config
endif

LOCAL_CXXFLAGS := -std=c++11
LOCAL_CXXFLAGS += $(shell $(PDRAW_PYTHON_PYTHONCONFIG) --includes)

LOCAL_LDLIBS := $(shell $(PDRAW_PYTHON_PYTHONCONFIG) --ldflags)

PDRAW_PYTHON_SWIG_SOURCE := python/pdraw_python.i
PDRAW_PYTHON_NAME := $(LOCAL_MODULE)

PDRAW_PYTHON_SWIG_DEPENDS := $(addsuffix /*,$(LOCAL_C_INCLUDES)) $(shell find $(LOCAL_PATH)/python/*i)
$(info $(PDRAW_PYTHON_SWIG_DEPENDS))

$(PDRAW_PYTHON_BUILD_DIR)/$(PDRAW_PYTHON_SWIG_WRAPPER): $(LOCAL_PATH)/$(PDRAW_PYTHON_SWIG_SOURCE) $(PDRAW_PYTHON_SWIG_DEPENDS)
	$(call print-banner1,"$(PRIVATE_MODE_MSG)Swig",$(PDRAW_PYTHON_SWIG_WRAPPER),$(PDRAW_PYTHON_SWIG_SOURCE))
	$(Q) swig -python -c++ $(PDRAW_PYTHON_SWIG_C_INCLUDES) -o $@ $<
	$(Q) mkdir -p $(PDRAW_PYTHON_DESTDIR)
	$(Q) cp $(PDRAW_PYTHON_BUILD_DIR)/$(PDRAW_PYTHON_NAME).py $(PDRAW_PYTHON_DESTDIR)


include $(BUILD_SHARED_LIBRARY)

endif
endif
