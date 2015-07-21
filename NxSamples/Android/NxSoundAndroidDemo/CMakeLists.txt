cmake_minimum_required(VERSION 3.0)
project(NxSoundAndroidDemo)

#set sample output destination
SET(NDKOUT "${CMAKE_SOURCE_DIR}/NxGraphicsSdk/NxSamples/NxSoundAndroidDemo")

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
file(MAKE_DIRECTORY "${NDKOUT}/src/nx/graphics/sound")

#copy java file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Example.java" DESTINATION "${NDKOUT}/src/nx/graphics/sound")

#copy jni file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.h" DESTINATION "${NDKOUT}/jni")
 
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.xml" DESTINATION "${NDKOUT}/res/layout")

#generate default.properties
set(ANDROID_TARGET android-17 )
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")

#generate Application.mk
# Build both ARMv5TE and ARMv7-A machine code: armeabi armeabi-v7a
file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")

#generate AndroidManifest.xml
getManifest( MANIFEST "nx.graphics.sound" "Example" "NxSoundAndroidDemo" )
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)
 
#set NxNetwork dependencies
getNxSound( NXSOUND_INCLUDE )
  
#set main application dependencies
set(APPLICATION_INCLUDE "
include $(CLEAR_VARS)
LOCAL_MODULE           := main
LOCAL_SRC_FILES        := main.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../NxSound $(LOCAL_PATH)/../../../nxdeps/include
LOCAL_EXPORT_LDLIBS := -llog 
LOCAL_STATIC_LIBRARIES := nxsound
include $(BUILD_SHARED_LIBRARY)")

#generate Android.mk
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

#copy res files
install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/res/" DESTINATION "${NDKOUT}/res")
 

