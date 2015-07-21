 
 
#include <android/log.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

#define  LOG_TAG    "JNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace NxNetwork;
NxNetworkManager * mApi = NULL; 
 

static JavaVM* gVM = NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
	gVM = vm;
	return JNI_VERSION_1_4;
}

 
JNIEXPORT void JNICALL Java_nx_graphics_main_Example_CreateEngine(JNIEnv *env, jobject obj) 
{
	mApi = new NxNetworkManager();

	NxNetworkHttpClient * client = new NxNetworkHttpClient();

	std::string url = "http://www.perspective-s.org/wp-content/themes/perspectives/images/vignettes/04.jpg";

	LOGD("Getting image url  ...");

	std::size_t DstSize = 0;
	long DstRetCode = 0;
	if( !client->ResourceExists( url, &DstRetCode, &DstSize ) ) { LOGD("ResourceExists : resource doesnt exist !"); delete client; return  ; } 

	if( DstRetCode == 404 | DstRetCode == 403 ) { 
		LOGD("GetImage : error 404"); 
		delete client; 
		return; } 
	 

	char * Buffer = new char[DstSize];
	std::size_t Size = 0;
	long retCode = 0;
	if(client->GetResourceBuffer(url, Buffer, &Size, &retCode)) {

		LOGD("==> Got image Buffer !");
		//Log("==> Buffer Size : " + Ogre::StringConverter::toString( Size )  );
		//Log("==> Ret CoDE : " + Ogre::StringConverter::toString( retCode )  );
	}else{

		LOGD(" NO BUFFER !");
	}
 
	return;
}


JNIEXPORT void JNICALL Java_nx_graphics_main_Example_DeleteEngine(JNIEnv *env, jobject obj) 
{
	 
	return;
}

 
 
 
 
