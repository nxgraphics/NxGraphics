


# ------------ NxDevice --------------
function(getNxDevice str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := nxdevice
LOCAL_SRC_FILES := ../../../Lib/libNxDevice.a
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../NxDevice $(LOCAL_PATH)/../../nxdeps/include
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()

# ------------ NxMocap --------------
function(getNxMocap str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := libmysqlclient
LOCAL_SRC_FILES := ../../../nxdeps/lib/libmysqlclient.a
include $(PREBUILT_STATIC_LIBRARY)
  
include $(CLEAR_VARS)
LOCAL_MODULE := nxmocap
LOCAL_SRC_FILES := ../../../Lib/libNxMocap.a
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../NxMocap $(LOCAL_PATH)/../../nxdeps/include
LOCAL_WHOLE_STATIC_LIBRARIES :=  libmysqlclient
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()

# ------------ NxNetwork --------------
function(getNxNetwork str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := ../../../nxdeps/lib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE := nxnetwork
LOCAL_SRC_FILES := ../../../Lib/libNxNetwork.a
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../NxNetwork $(LOCAL_PATH)/../../nxdeps/include
LOCAL_WHOLE_STATIC_LIBRARIES :=  libcurl
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()

# ------------ NxSound --------------
function(getNxSound str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := kissfft
LOCAL_SRC_FILES := ../../../nxdeps/lib/libkissfft.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := flac
LOCAL_SRC_FILES := ../../../nxdeps/lib/liblibflac.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := mad
LOCAL_SRC_FILES := ../../../nxdeps/lib/liblibmad.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogg
LOCAL_SRC_FILES := ../../../nxdeps/lib/libogg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := ../../../nxdeps/lib/libopenal.a
LOCAL_EXPORT_LDLIBS := -llog -lOpenSLES 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vorbis
LOCAL_SRC_FILES := ../../../nxdeps/lib/libvorbis.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vorbisfile
LOCAL_SRC_FILES := ../../../nxdeps/lib/libvorbisfile.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := nxsound
LOCAL_SRC_FILES := ../../../Lib/libNxSound.a
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../NxSound $(LOCAL_PATH)/../../nxdeps/include
LOCAL_WHOLE_STATIC_LIBRARIES := libutils liblog libOpenSLES flac kissfft mad openal ogg vorbis vorbisfile
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()

# ------------ NxVideo --------------
function(getNxVideo str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := theora
LOCAL_SRC_FILES := ../../../nxdeps/lib/liblibtheora.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := nxvideo
LOCAL_SRC_FILES := ../../../Lib/libNxVideo.a
LOCAL_CPPFLAGS  := -fexceptions -frtti -x c++ -D___ANDROID___ -DANDROID -DZZIP_OMIT_CONFIG_H -DUSE_RTSHADER_SYSTEM=1 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../NxVideo $(LOCAL_PATH)/../../../nxdeps/include
LOCAL_WHOLE_STATIC_LIBRARIES := theora
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()

 
# ------------ NxGraphics --------------
function(getNxGraphics str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := ois
LOCAL_SRC_FILES := ../../../nxdeps/lib/armeabi/libOIS.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := freeimage
LOCAL_SRC_FILES := ../../../nxdeps/lib/armeabi/libFreeImage.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := ../../../nxdeps/lib/armeabi/libfreetype.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := ../../../nxdeps/lib/armeabi/libzlib.a
include $(PREBUILT_STATIC_LIBRARY)

 
include $(CLEAR_VARS)
LOCAL_MODULE := zzip
LOCAL_SRC_FILES := ../../../nxdeps/lib/armeabi/libzzip.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogre
LOCAL_SRC_FILES := ../../../nxdeps/lib/libOgreMainStatic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogreoverlay
LOCAL_SRC_FILES := ../../../nxdeps/lib/libOgreOverlayStatic.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := ogreterrain
LOCAL_SRC_FILES := ../../../nxdeps/lib/libOgreTerrainStatic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogrepaging
LOCAL_SRC_FILES := ../../../nxdeps/lib/libOgrePagingStatic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogrertshader
LOCAL_SRC_FILES := ../../../nxdeps/lib/libOgreRTShaderSystemStatic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogreoctree
LOCAL_SRC_FILES := ../../../nxdeps/lib/OGRE/libPlugin_OctreeSceneManagerStatic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogreparticles
LOCAL_SRC_FILES := ../../../nxdeps/lib/OGRE/libPlugin_ParticleFXStatic.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ogregles2
LOCAL_SRC_FILES := ../../../nxdeps/lib/OGRE/libRenderSystem_GLES2Static.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := nxgraphics
LOCAL_SRC_FILES := ../../../Lib/libNxGraphics.a
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)../../../NxGraphics $(LOCAL_PATH)../../../nxdeps/include
LOCAL_WHOLE_STATIC_LIBRARIES :=  ogre ogreoctree ogreparticles ogregles2 ogrertshader ogreoverlay ogreterrain ogrepaging ois 
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()
 

 

function(getManifest str package activityname appname )
set( ${str} "<?xml version=\"1.0\" encoding=\"utf-8\"?>
<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"
    android:versionCode=\"1\"
    android:versionName=\"1.0\" 
    package=\"${package}\">
    <uses-sdk android:minSdkVersion=\"9\"/>
	<uses-feature android:glEsVersion=\"0x00020000\"/>
    <uses-permission android:name=\"android.permission.WRITE_EXTERNAL_STORAGE\"/> 
    <uses-permission android:name=\"android.permission.READ_EXTERNAL_STORAGE\"/> 
    <uses-permission android:name=\"android.permission.INTERNET\"/>
    <uses-permission android:name=\"android.permission.ACCESS_NETWORK_STATE\" /> 
	<uses-feature android:name=\"android.hardware.camera\" />
    <uses-permission android:name=\"android.permission.CAMERA\" />

    <application 
        android:icon=\"@drawable/ic_launcher\" 
        android:label=\"${appname}\"
        android:launchMode=\"singleTask\">
        <activity 
            android:noHistory=\"true\"
            android:launchMode=\"singleTask\"
            android:name=\"${package}.${activityname}\"
            android:label=\"${appname}\"
            android:screenOrientation=\"landscape\"
			android:configChanges=\"orientation\"
			android:theme=\"@android:style/Theme.Black.NoTitleBar.Fullscreen\">
            <intent-filter>
                <action android:name=\"android.intent.action.MAIN\" />
                <category android:name=\"android.intent.category.LAUNCHER\" />
            </intent-filter>
        </activity>
    </application>
</manifest>" PARENT_SCOPE)
endfunction()

add_subdirectory(NxDeviceAndroidDemo)
add_subdirectory(NxGraphicsVuforiaDemo)
add_subdirectory(NxGraphicsAndroidDemo)
add_subdirectory(NxNetworkAndroidDemo)