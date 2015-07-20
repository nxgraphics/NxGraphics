cmake_minimum_required(VERSION 3.0)
project(NxGraphicsVuforiaDemo)

#set sample output destination
SET(NDKOUT "${CMAKE_SOURCE_DIR}/NxGraphicsSdk/NxSamples/NxGraphicsVuforiaDemo")

#create directories
file(MAKE_DIRECTORY "${NDKOUT}")
file(MAKE_DIRECTORY "${NDKOUT}/jni")
file(MAKE_DIRECTORY "${NDKOUT}/assets")
file(MAKE_DIRECTORY "${NDKOUT}/res")
file(MAKE_DIRECTORY "${NDKOUT}/res/layout")
file(MAKE_DIRECTORY "${NDKOUT}/src")
file(MAKE_DIRECTORY "${NDKOUT}/src/com")
file(MAKE_DIRECTORY "${NDKOUT}/src/com/hotstuff")
file(MAKE_DIRECTORY "${NDKOUT}/src/com/hotstuff/main")

#copy java file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Example.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/NxVideoSurfaceView.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/OgreActivityJNI.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaControl.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaExceptions.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaFreeframe.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaFreeframeGL.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaGlView.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaLoadingDialogHandler.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMenu.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMenuAnimator.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMenuGroup.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMenuInterface.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMenuView.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMesh.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMeshObject.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaSession.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaShaders.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaTargetRenderer.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaTeapot.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaTexture.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaUtils.java" DESTINATION "${NDKOUT}/src/com/hotstuff/main")

#copy jni file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.h" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Shaders.h" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMath.h" DESTINATION "${NDKOUT}/jni")

#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.xml" DESTINATION "${NDKOUT}/res/layout")

#generate default.properties
set(ANDROID_TARGET android-17 )
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")

#generate Application.mk
# Build both ARMv5TE and ARMv7-A machine code: armeabi armeabi-v7a
file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")

 

#generate AndroidManifest.xml
getManifest( MANIFEST "com.hotstuff.main" "Example" "NxGraphicsVuforiaDemo" )
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
LOCAL_STATIC_LIBRARIES += android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
$(call import-module,cpufeatures)")

#generate Android.mk
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

#copy res files
install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/res/" DESTINATION "${NDKOUT}/res")