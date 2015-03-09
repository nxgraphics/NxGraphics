
function(getNxDevice str )
set( ${str} "
include $(CLEAR_VARS)
LOCAL_MODULE := nxdevice
LOCAL_SRC_FILES := ../../../Lib/libNxDevice.a
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../NxDevice $(LOCAL_PATH)/../../nxdeps/include
include $(PREBUILT_STATIC_LIBRARY)" PARENT_SCOPE)
endfunction()

 

function(getManifest str package activityname appname )
set( ${str} "<?xml version=\"1.0\" encoding=\"utf-8\"?>
<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"
    android:versionCode=\"1\"
    android:versionName=\"1.0\" 
    package=\"${package}\">
    <uses-sdk android:minSdkVersion=\"9\"/>

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