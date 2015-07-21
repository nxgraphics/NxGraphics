cmake_minimum_required(VERSION 3.0)
project(NxGraphicsAndroidDemo)

#set sample output destination
SET(NDKOUT "${CMAKE_SOURCE_DIR}/NxGraphicsSdk/NxSamples/NxGraphicsAndroidDemo")

#create directories
file(MAKE_DIRECTORY "${NDKOUT}")
file(MAKE_DIRECTORY "${NDKOUT}/jni")
file(MAKE_DIRECTORY "${NDKOUT}/libs")
file(MAKE_DIRECTORY "${NDKOUT}/assets")
file(MAKE_DIRECTORY "${NDKOUT}/res")
file(MAKE_DIRECTORY "${NDKOUT}/res/layout")
file(MAKE_DIRECTORY "${NDKOUT}/src")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx/graphics")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx/graphics/main")

#copy java file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Example.java" DESTINATION "${NDKOUT}/src/nx/graphics/main")

#copy jni file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.h" DESTINATION "${NDKOUT}/jni")

 

#generate default.properties
set(ANDROID_TARGET android-17 )
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")

#generate Application.mk
# Build both ARMv5TE and ARMv7-A machine code: armeabi armeabi-v7a
file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")

#generate AndroidManifest.xml
getManifest( MANIFEST "nx.graphics.main" "Example" "NxGraphicsAndroidDemo" )
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)

#set NxDevice dependencies
getNxDevice( NXDEVICE_INCLUDE )

#set NxNetwork dependencies
getNxNetwork( NXNETWORK_INCLUDE )

#set NxSound dependencies
getNxSound( NXSOUND_INCLUDE )

#set NxMocap dependencies
getNxMocap( NXMOCAP_INCLUDE )

#set NxVideo dependencies
getNxVideo( NXVIDEO_INCLUDE )

#set NxGraphics dependencies
getNxGraphics( NXGRAPHICS_INCLUDE )

 
#set main application dependencies
set(APPLICATION_INCLUDE "
include $(CLEAR_VARS)
LOCAL_MODULE           := main
LOCAL_SRC_FILES        := main.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../nxdeps/include/OGRE $(LOCAL_PATH)/../../../NxGraphics/physics/empty $(LOCAL_PATH)/../../../NxGraphics $(LOCAL_PATH)/../../../NxMocap $(LOCAL_PATH)/../../../NxDevice $(LOCAL_PATH)/../../../NxSound $(LOCAL_PATH)/../../../NxVideo $(LOCAL_PATH)/../../../NxNetwork $(LOCAL_PATH)/../../../nxdeps/include 
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_LDLIBS += -landroid -lc -lm -ldl -llog -lEGL -lGLESv2
LOCAL_CFLAGS := -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID -DZZIP_OMIT_CONFIG_H -DUSE_RTSHADER_SYSTEM=1 -DUSE_QUICKTIME=0
LOCAL_STATIC_LIBRARIES := cpufeatures zlib zzip freetype freeimage nxgraphics nxvideo nxsound nxmocap nxnetwork nxdevice
LOCAL_STATIC_LIBRARIES += android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
$(call import-module,cpufeatures)")

#generate Android.mk
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

#copy res files
install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/res/" DESTINATION "${NDKOUT}/res")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/assets/" DESTINATION "${NDKOUT}/assets")

