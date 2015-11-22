cmake_minimum_required(VERSION 3.0)
project(NxGraphicsVuforia5Demo)

#set sample output destination
SET(NDKOUT "${CMAKE_SOURCE_DIR}/NxGraphicsSdk/NxSamples/NxGraphicsVuforia5Demo")

#create directories
file(MAKE_DIRECTORY "${NDKOUT}")
file(MAKE_DIRECTORY "${NDKOUT}/jni")
file(MAKE_DIRECTORY "${NDKOUT}/libs")
file(MAKE_DIRECTORY "${NDKOUT}/assets")
file(MAKE_DIRECTORY "${NDKOUT}/res")
file(MAKE_DIRECTORY "${NDKOUT}/res/layout")
file(MAKE_DIRECTORY "${NDKOUT}/src")
file(MAKE_DIRECTORY "${NDKOUT}/src/com")
file(MAKE_DIRECTORY "${NDKOUT}/src/com/hotstuff")
file(MAKE_DIRECTORY "${NDKOUT}/src/com/hotstuff/main")

file(MAKE_DIRECTORY "${NDKOUT}/src/com/apache")
file(MAKE_DIRECTORY "${NDKOUT}/src/com/hotstuff/ivy")

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

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/AbstractURLHandler.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/ApacheURLLister.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/BasicURLHandler.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CopyProgressEvent.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CopyProgressListener.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Credentials.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CredentialsStore.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/FileUtil.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/IvyAuthenticator.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/URLHandler.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/URLHandlerRegistry.java" DESTINATION "${NDKOUT}/src/com/apache/ivy")



#copy jni file
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/main.h" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Shaders.h" DESTINATION "${NDKOUT}/jni")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/VuforiaMath.h" DESTINATION "${NDKOUT}/jni")

#copy jar lib
file(COPY "${CMAKE_SOURCE_DIR}/nxdeps/lib/Vuforia5.jar" DESTINATION "${NDKOUT}/libs")

#generate default.properties
set(ANDROID_TARGET android-17 )
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")

#generate Application.mk
# Build both ARMv5TE and ARMv7-A machine code: armeabi armeabi-v7a
file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")

 

#generate AndroidManifest.xml
getManifest( MANIFEST "com.hotstuff.main" "Example" "NxGraphicsVuforia5Demo" )
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
LOCAL_MODULE := vuforia
LOCAL_SRC_FILES := ../../../nxdeps/lib/libVuforia5.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE           := NxGraphics
LOCAL_SRC_FILES        := main.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../nxdeps/include/Vuforia/include $(LOCAL_PATH)/../../../nxdeps/include/OGRE $(LOCAL_PATH)/../../../NxGraphics/physics/empty $(LOCAL_PATH)/../../../NxGraphics $(LOCAL_PATH)/../../../NxMocap $(LOCAL_PATH)/../../../NxDevice $(LOCAL_PATH)/../../../NxSound $(LOCAL_PATH)/../../../NxVideo $(LOCAL_PATH)/../../../NxNetwork $(LOCAL_PATH)/../../../nxdeps/include 
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_LDLIBS += -landroid -lc -lm -ldl -llog -lEGL -lGLESv2
LOCAL_CFLAGS := -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID -DZZIP_OMIT_CONFIG_H -DUSE_RTSHADER_SYSTEM=1 -DUSE_QUICKTIME=0
LOCAL_STATIC_LIBRARIES := vuforia cpufeatures zlib zzip freetype freeimage nxgraphics nxvideo nxsound nxmocap nxnetwork nxdevice
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/cpufeatures)")

#generate Android.mk
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

#copy res files
install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/res/" DESTINATION "${NDKOUT}/res")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/drawable" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/layout" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/values" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/values-hdpi" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/values-ldpi" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/values-mdpi" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/values-xhdpi" DESTINATION "${NDKOUT}/res")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/values-xxhdpi" DESTINATION "${NDKOUT}/res")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/assets/" DESTINATION "${NDKOUT}/assets")

