cmake_minimum_required(VERSION 3.0)
project(NxDeviceAndroidDemo)

#set sample output destination
SET(NDKOUT "${CMAKE_SOURCE_DIR}/NxGraphicsSdk/NxSamples/NxDeviceAndroidDemo")

#create directories
file(MAKE_DIRECTORY "${NDKOUT}")
file(MAKE_DIRECTORY "${NDKOUT}/jni")
file(MAKE_DIRECTORY "${NDKOUT}/assets")
file(MAKE_DIRECTORY "${NDKOUT}/res")
file(MAKE_DIRECTORY "${NDKOUT}/src")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx/graphics")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx/graphics/device")

#copy java file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/NxDevice.java" DESTINATION "${NDKOUT}/src/nx/graphics/device")

#copy jni file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.h" DESTINATION "${NDKOUT}/jni")

#generate default.properties
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")

#generate Application.mk
file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")

#generate AndroidManifest.xml
getManifest( MANIFEST "nx.graphics.device" "NxDevice" )
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)

#set NxDevice dependencies
getNxDevice( NXDEVICE_INCLUDE )
 
#set main application dependencies
set(APPLICATION_INCLUDE "
include $(CLEAR_VARS)
LOCAL_MODULE           := NxDevice
LOCAL_SRC_FILES        := main.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../NxDevice  $(LOCAL_PATH)/../../../nxdeps/include 
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_LDLIBS += -landroid -lc -lm -ldl -llog -lEGL -lGLESv2
LOCAL_CFLAGS := -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID
LOCAL_STATIC_LIBRARIES := cpufeatures nxdevice
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/cpufeatures)")

#generate Android.mk
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

#copy res files
install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/res/" DESTINATION "${NDKOUT}/res")