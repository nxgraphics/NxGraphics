/*===============================================================================================
 NxGraphics Engine Example

 This example shows how to basically instance the engine
===============================================================================================*/

#include <android/log.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

#include <EGL/egl.h>
#include <android/api-level.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "../../nxdeps/include/OIS/OIS.h"
 
#define  LOG_TAG    "JNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
 
using namespace Nx;
 
 
 
 
 
