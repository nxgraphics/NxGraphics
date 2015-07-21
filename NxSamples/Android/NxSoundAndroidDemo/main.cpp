
#include <android/log.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

#define  LOG_TAG    "JNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace NxSound_Namespace;
NxSoundManager * mApi = NULL; 
NxSoundDeviceOutput *  DevOut = NULL;

static JavaVM* gVM = NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
	gVM = vm;
	return JNI_VERSION_1_4;
}

/*
enum NxSoundApi 
{
NX_UNSPECIFIED, 0    
NX_LINUX_ALSA,  1    
	NX_LINUX_OSS, 2      
	NX_UNIX_JACK,  3     
	NX_MACOSX_CORE, 4    
	NX_WIN_ASIO,    5
	NX_WIN_DS,      6
	NX_OPENSLES,      7
	NX_DUMMY    8
};

*/

JNIEXPORT void JNICALL Java_nx_graphics_sound_Example_CreateEngine(JNIEnv *env, jobject obj) 
{
	mApi = new NxSoundManager( (NxSoundApi) 7 ); // NX_OPEN_SL 
	return;
}


JNIEXPORT void JNICALL Java_nx_graphics_sound_Example_DeleteEngine(JNIEnv *env, jobject obj) 
{
	delete mApi;  
	mApi = NULL;
	return;
}


JNIEXPORT jint JNICALL Java_nx_graphics_sound_Example_GetNumDevices(JNIEnv *env, jobject thiz)
{
	return mApi->GetNumOutputDevices();
}

JNIEXPORT void JNICALL Java_nx_graphics_sound_Example_CloseDevice(JNIEnv *env, jobject obj ) {
	 
	mApi->DeleteDeviceOutput( DevOut );

}


JNIEXPORT void JNICALL Java_nx_graphics_sound_Example_OpenDevice(JNIEnv *env, jobject obj ) {

	DevOut = mApi->CreateDeviceOutput( 0 );

}

void GetJStringContent(JNIEnv *AEnv, jstring AStr, std::string &ARes) {

	if (!AStr) {
		ARes.clear();
		return;
	}

	const char *s = AEnv->GetStringUTFChars(AStr,NULL);
	ARes=s;
	AEnv->ReleaseStringUTFChars(AStr,s);
}

 std::string mSoundName = "";
 NxSoundMediaFile * mAudioFile = NULL;

JNIEXPORT void JNICALL Java_nx_graphics_sound_Example_OpenFile(JNIEnv *env, jobject obj, jstring filepath )
{ 
	LOGD("OpenFile Started");

	std::string str;
	GetJStringContent(env,filepath,str);

	 
	if( !mSoundName.empty() ){
		mApi->DeleteMediaFile( mSoundName );
		mAudioFile = NULL;
	}
	 
	mSoundName = "SoundName_"+NxSoundUtils::ToString(rand()%20000);

	mApi->AddMediaFile( mSoundName, DevOut, str, false );
	mApi->PlayNxSound( mSoundName );
	LOGD("GetMediaFileFromName..");
	mAudioFile = mApi->GetMediaFile( mSoundName );
	mAudioFile->SetRate( 1.0f ) ;// from 0.0f to 1.0f - 0.5 play half rate
	mAudioFile->SetVolume( 1.0f );// from 0.0f to 1.0f 


	LOGD("OpenFile Done");
 
}

JNIEXPORT void JNICALL Java_nx_graphics_sound_Example_SetRate(JNIEnv *env, jobject obj, jfloat rate ){
 
	if( mAudioFile )
	mAudioFile->SetRate( rate ) ;// from 0.0f to 1.0f - 0.5 play half rate

}

 

JNIEXPORT jstring JNICALL Java_nx_graphics_sound_Example_GetDeviceName(JNIEnv *env, jobject obj, jint firstAddend)
{ 

	std::vector<NxDeviceInfo> SoundOutputList;
	mApi->GetSoundDeviceList( SoundOutputList );
	string name = "none";
	for(int i = 0 ; i < SoundOutputList.size(); i++  ) {
		NxDeviceInfo OutputInfo = SoundOutputList[i] ;
		name = OutputInfo.name ;
	}
 
	return env->NewStringUTF( name.c_str() );//DevOutName.c_str() );
}  

 
 
 
