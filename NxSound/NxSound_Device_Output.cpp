#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"
#include "STK/include/Stk.h" 
#include "STK/include/RtAudio.h"

namespace NxSound_Namespace {

	stk::StkFrames FramesRecorder; // main sound buffer
	stk::StkFrames Frames[255];   // multi track sound buffers
	std::list< NxSoundMediaFile * > mMediaList;
	static int tick( void * outputBuffer, void * inputBuffer, unsigned int nBufferFrames,double streamTime, NxAudioStreamStatus status, void * userData );

NxSoundDeviceOutput::NxSoundDeviceOutput( int AudioDeviceId ) : NxSoundDevice(AudioDeviceId) {

	Log( "Creating Audio Device Output");
	mDevType = NxSoundTypeDeviceOutput;
	mAudioDevice = new RtAudio( ( RtAudio::Api ) NxSoundManager::getSingleton().GetNxSoundApiType() ); 
	if( AudioDeviceId < 0 ){ Log( "NxSoundDeviceOutput : Could not find device at index ");  }
	stk::Stk::setSampleRate( 44100.0f );// Set the global sample rate before creating class instances.

	RtAudio::DeviceInfo DevInfo = mAudioDevice->getDeviceInfo( AudioDeviceId );
	RtAudio::StreamParameters * parameters = new RtAudio::StreamParameters;
	parameters->deviceId =	AudioDeviceId  ; 
	parameters->nChannels = mNumOutChannels = DevInfo.outputChannels; 
	//parameters->firstChannel = 0;
 
	mResolutionOut = NX_FLOAT32;
	RtAudioFormat format = RTAUDIO_FLOAT32; // Normalized between plus/minus 1.0.
	RtAudio::StreamOptions *options = new RtAudio::StreamOptions;
	options->numberOfBuffers = 1;
	//options->flags |= RTAUDIO_NONINTERLEAVED;// Use non-interleaved buffers (default = interleaved).
 
	CreateAudioBuffers();

	unsigned int bufferFrames = SND_BUFFER_SIZE;

	try{
		mAudioDevice->openStream(  parameters, NULL, format, (unsigned int) stk::Stk::sampleRate(), &bufferFrames, &tick, (void *)this, options );
	}
	catch ( RtError &error ){
		Log(  "NxSound : could not open Output Stream"   );
		error.printMessage();
		return ;
	}

	//Resize the StkFrames object appropriately.
	FramesRecorder.resize( bufferFrames, parameters->nChannels );

	Log("NxSound : Stream Opened with Buffer Size : " + NxSoundUtils::ToString( bufferFrames ) + ". Num Buffers : " + NxSoundUtils::ToString( options->numberOfBuffers )  ); 

	try {
		mAudioDevice->startStream();
		Log( "NxSound : Stream Started" );
	}
	catch ( RtError &error ){
		Log( "Could not startStream" );
		error.printMessage();
		return ;
	}

	mDeviceFFT = new NxSoundFFT();
	//mDeviceFFT->Initialize( SND_BUFFER_SIZE, 1024 ); 
	Log( "NxSound : FFT created" );
}

NxSoundDeviceOutput::~NxSoundDeviceOutput(){
 
	if( mAudioDevice ){ mAudioDevice->closeStream(); delete mAudioDevice; mAudioDevice = NULL; }
	RemoveAllMediaFiles();
}

void NxSoundDeviceOutput::AddMediaFile( NxSoundMediaFile * Media ) {

	Log("AddMediaFile( NxSoundMediaFile * Media begin )");
	Media->SetResolution( GetOutputResolution() );
	mMediaList.push_back( Media );


	Log("AddMediaFile( NxSoundMediaFile * Media )");

}

NxAudioFormat NxSoundDeviceOutput::GetOutputResolution()
{
	return mResolutionOut;
}

bool NxSoundDeviceOutput::RemoveMediaFile( NxSoundMediaFile * Media  ){
	for( std::list< NxSoundMediaFile * >::iterator iter =  mMediaList.begin(); iter !=  mMediaList.end(); iter++){
		NxSoundMediaFile  * File = *iter;
		if( File == Media ){
			mMediaList.remove( File ); return true;
		}
	}
	return false ;
}

bool NxSoundDeviceOutput::RemoveAllMediaFiles(){
	bool HasMediaFiles = mMediaList.empty() ;
	mMediaList.clear();
	return HasMediaFiles ;
}

#if defined(__ANDROID__)
#include <android/log.h>
#define  LOG_TAG    "NXSOUND NxSoundManager"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,double streamTime, NxAudioStreamStatus status, void *userData )
{

	//LOGD("TICK STARTED");

	NxSoundDeviceOutput * DeviceOutput =  ( NxSoundDeviceOutput * ) userData ;
	if( status == NX_INPUT_OVERFLOW){ Log("NX_INPUT_OVERFLOW"); }
	else if( status == NX_OUTPUT_UNDERFLOW ){ Log("NX_OUTPUT_UNDERFLOW"); Log("nBufferFrames = " + NxSoundUtils::ToString( nBufferFrames ) );  }

	register float * samples = (float *) outputBuffer;

	//LOGD("FRAME RESIZE :");
	//LOGE("NUM FRAME: %d", nBufferFrames );
	//LOGE("NUM CHANNELS: %d", DeviceOutput->GetNumChannels() );

	time_t seconds;
	seconds = time (NULL);
	NxSoundEffectVSTHost::getSingleton().SetTime( (double)(seconds/1000.0) * 1000000.0L );// windows


	FramesRecorder.resize( nBufferFrames, DeviceOutput->GetNumChannels(), 0.0f );

	int StreamIndex = 0;
	for( std::list< NxSoundMediaFile * >::iterator iter = mMediaList.begin(); iter != mMediaList.end(); iter++ ) {

		NxSoundMediaFile * File = *iter; 
		//LOGD("filling audio ..");
		if(!File->isPaused()) File->Update( Frames[StreamIndex] ); 
		//LOGD("filling audio DONE");
 
		// process audio stream plugins if any.
		File->ProcessEffectPlugins( Frames[StreamIndex] );
		// get volume.
		float Volume = File->isStopped() || File->isPaused() ? 0.0f : File->GetVolume();

		/*
		NxAudioFormat FileFormat = File->GetResolution();
		double NormalizeTo32BitsFloat = 1.0;

		//Log( "File resolution : " + NxSoundUtils::ToString( FileFormat )  );
 
		switch( FileFormat ) {

			case NX_SINT8 : // 8-bit signed integer.
				NormalizeTo32BitsFloat = 1.0 / 128.0;
				break;
			case NX_SINT16 : // 16-bit signed integer.
				NormalizeTo32BitsFloat = 1.0 / 32768.0;
				break;
			case NX_SINT24 : // Lower 3 bytes of 32-bit signed integer.
				NormalizeTo32BitsFloat = 1.0 / 2147483648.0;
				break;
			case NX_SINT32 : break; // 32-bit signed integer.				 
			case NX_FLOAT32 : break; // Normalized between plus/minus 1.0.
			case NX_FLOAT64 : break; // Normalized between plus/minus 1.0.
			default: Log("Error : no file format !");break;

		}
		*/

		//LOGD(" mixing buffer ");
		for ( unsigned int i=0; i< nBufferFrames ; i++ ) {
			//FramesRecorder(i,0) += (Frames[StreamIndex]( i, 0 ) * NormalizeTo32BitsFloat) * Volume ;//* 0.5f;
			//FramesRecorder(i,1) += (Frames[StreamIndex]( i, File->IsStereo() ? 1 : 0 ) * NormalizeTo32BitsFloat ) * Volume ;//* 0.5f;	

			FramesRecorder(i,0) += (Frames[StreamIndex]( i, 0 )) * Volume ;//* 0.5f;  ori
			FramesRecorder(i,1) += (Frames[StreamIndex]( i, File->IsStereo() ? 1 : 0 )) * Volume ;//* 0.5f;	

			//FramesRecorder(i,0) += (Frames[StreamIndex]( i, 0 ))   ;//* 0.5f;
			//FramesRecorder(i,1) += (Frames[StreamIndex]( i, 0))  ;//* 0.5f;	

			//FramesRecorder(i,0) += NxSoundUtils::GetRandom( -1.0f , 1.0f ) ;//* 0.5f;
			//FramesRecorder(i,1) += NxSoundUtils::GetRandom( -1.0f , 1.0f ) ;//* 0.5f;



		}

		//LOGD(" mixing buffer : DONE");

		StreamIndex++ ; 
	}


	//LOGD("ProcessDeviceEffectPlugins");
	// process global plugin effects here
	DeviceOutput->ProcessDeviceEffectPlugins( FramesRecorder );

	//DeviceOutput->ProcessFFT();


	//LOGD("interleaved to output");
 
	//stereo interleaved to output
	for ( unsigned int i=0; i< nBufferFrames; i++ ){	
		for( int mChannels = 0; mChannels < DeviceOutput->GetNumChannels(); mChannels++ ){
			float f = FramesRecorder( i, mChannels ) / mMediaList.size() ;
			if (f > 1) f = 1.0f;//clipping
			if (f < -1) f = -1.0f;
			*samples++ = f ;
		}
	} 

	//LOGD("CopyIn");

	DeviceOutput->CopyIn( FramesRecorder );// to non interleaved, for FFT


	//LOGD("ALL DONE !");

	return 0;
}

}//namespace