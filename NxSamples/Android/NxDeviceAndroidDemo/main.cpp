/*===============================================================================================
 NxDevice Example

 This example shows how to basically instance the module
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

 
#define  LOG_TAG    "NxDeviceDemo"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
 


bool mLostFocus = false;
static JavaVM* gVM = NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
	gVM = vm;
	return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL Java_nx_graphics_device_NxDevice_createNxDevice( JNIEnv * env, jobject obj ) {


	LOGE("createNxDevice CALLED !" );


}


static int32_t handleInput( struct android_app* app, AInputEvent* event ) {

	 
	return 0;
}

 


static void handleCmd(struct android_app* app, int32_t cmd)
{
 
	switch (cmd) 
	{
	case APP_CMD_SAVE_STATE:

		LOGD("APP_CMD_SAVE_STATE called" );
 
		break;
	case APP_CMD_INIT_WINDOW:

		LOGD("APP_CMD_INIT_WINDOW called" );
		 
		if( app->window != NULL )
		{
			AConfiguration* config = AConfiguration_new();
			AConfiguration_fromAssetManager(config, app->activity->assetManager);
	 
			//initialize your module here

			AConfiguration_delete(config);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		LOGD("APP_CMD_TERM_WINDOW called" );
		 
		break;
	case APP_CMD_GAINED_FOCUS:
		LOGD("APP_CMD_GAINED_FOCUS called" );
		mLostFocus = false;
		break;
	case APP_CMD_LOST_FOCUS:
		LOGD("APP_CMD_LOST_FOCUS called" );
		mLostFocus = true;
		break;
	case APP_CMD_CONFIG_CHANGED:
		LOGD("APP_CMD_CONFIG_CHANGED called could be ORIENTATION !" );
		break;
	case APP_CMD_PAUSE:
		LOGD("APP_CMD_PAUSE called" );
		break;
	case APP_CMD_STOP:
		LOGD("APP_CMD_STOP called" );
		break;
	case APP_CMD_DESTROY:
		LOGD("APP_CMD_DESTROY called" );
		break;
	}
}

void android_main(struct android_app* state) { 
 
	app_dummy();
 
	JNIEnv *env;
	gVM->AttachCurrentThread(&env, 0);
  
	state->onAppCmd = &handleCmd;
	state->onInputEvent = &handleInput;
 
	int ident, events;
	struct android_poll_source* source;
	while( 1 ) { 
	 
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) // poll input events
		{
			if (source != NULL)
				source->process(state, source);

			if (state->destroyRequested != 0) { 
			
				//release here
				gVM->DetachCurrentThread();
				gVM = 0;
				exit(1) ;
				return;
			}
	 
		}

	 

	}
 
} 
 
 
 
 
 
