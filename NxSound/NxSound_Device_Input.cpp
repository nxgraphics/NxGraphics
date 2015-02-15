#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"
#include "STK/include/Stk.h" 
#include "STK/include/RtAudio.h"

namespace NxSound_Namespace {

	stk::StkFrames FramesInput;
	static int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, NxAudioStreamStatus status, void *userData );

NxSoundDeviceInput::NxSoundDeviceInput( int mAudioIndex ) : NxSoundDevice(mAudioIndex), mEnableOutput(true) {

	//mAudioDevice = new RtAudio( RtAudio::Api::WINDOWS_DS );
	mAudioDevice = new RtAudio( ( RtAudio::Api ) NxSoundManager::getSingleton().GetNxSoundApiType() ); 

	mDevType = NxSoundTypeDeviceInput;
	if( mAudioIndex < 0 ){ Log( "NxSound : NxSoundRecorder could not find device at index "); return; }
 
	// Set the global sample rate before creating class instances.
	stk::Stk::setSampleRate( 44100.0f );
 
	RtAudio::DeviceInfo DevInInfo = mAudioDevice->getDeviceInfo( mAudioIndex );
	//RtAudio::DeviceInfo DevOutInfo = mAudioDevice->getDeviceInfo( NxSoundManager::getSingleton().GetDefaultOutputDeviceIndex() );

	unsigned int MaxInDuplexChannels = DevInInfo.duplexChannels;

	//input paramters
	int mNumInChannels = DevInInfo.inputChannels;
	RtAudio::StreamParameters * ParametersInput = new RtAudio::StreamParameters;
	ParametersInput->deviceId = mAudioIndex  ; 
	ParametersInput->nChannels = mNumInChannels;
	//ParametersInput->firstChannel = 0;

	RtAudio::StreamParameters * ParametersOutput = new RtAudio::StreamParameters;
	ParametersOutput->deviceId = NxSoundManager::getSingleton().GetDefaultOutputDeviceIndex();
	ParametersOutput->nChannels = mNumOutChannels = 2;
	//ParametersOutput->firstChannel = 0;

	RtAudioFormat format = RTAUDIO_FLOAT32;
	RtAudio::StreamOptions *options = new RtAudio::StreamOptions;
	options->numberOfBuffers = 1;

	CreateAudioBuffers();

	unsigned int bufferFrames = SND_BUFFER_SIZE;

	try {
		mAudioDevice->openStream(  ParametersOutput, ParametersInput, format, (unsigned int)stk::Stk::sampleRate(), &bufferFrames, &tick , (void *)this , options ); 
	}
	catch ( RtError &error ) {
		Log( "NxSound : could not open Input Stream" );
		error.printMessage();
		return ;
	  }

	//Resize the StkFrames object appropriately.
	FramesInput.resize( bufferFrames, mNumInChannels );

	try{
		mAudioDevice->startStream();
		Log( "NxSound : Input Stream Started");
	}
	catch ( RtError &error ){
		Log( "NxSound : could not Input startStream");
		error.printMessage();
		return;
	}

	mDeviceFFT = new NxSoundFFT();
	mDeviceFFT->Initialize( SND_BUFFER_SIZE, 1024 ); 
	Log( "NxSound : FFT created" );


}

NxSoundDeviceInput::~NxSoundDeviceInput(){
	if( mAudioDevice ){ Log( "Deleting Audio Input "  ); mAudioDevice->closeStream(); delete mAudioDevice; mAudioDevice = NULL; }
	if( mDeviceFFT ){ Log( "Deleting Audio Input FFT "  ); delete mDeviceFFT; }
}

void NxSoundDeviceInput::SetEnableOutput( bool Enable )
{
	mEnableOutput = Enable;
}

bool NxSoundDeviceInput::GetEnableOutput()
{
	return mEnableOutput;
}

int tick( void * outputBuffer, void * inputBuffer, unsigned int nBufferFrames, double streamTime, NxAudioStreamStatus status, void * userData ){
	
	NxSoundDeviceInput * InputDevice =  ( NxSoundDeviceInput * ) userData ;

	if( status == NX_INPUT_OVERFLOW ){ 
		Log("RTAUDIO_INPUT_OVERFLOW" );
	}
	else if( status == NX_OUTPUT_UNDERFLOW ){ 
		Log("RTAUDIO_OUTPUT_UNDERFLOW" );
	}

	FramesInput.resize( SND_BUFFER_SIZE, 2 ); 
	
	float * in = (float  *) inputBuffer;

	register float * samples = (float *) outputBuffer;

	
	for ( unsigned int i=0; i< nBufferFrames ; i++ ){
		FramesInput(i,0) = in[i] ;
	} 

	for ( unsigned int i=0; i< nBufferFrames ; i++ ){
		FramesInput(i,1) = in[i+nBufferFrames] ;
	} 


	time_t seconds;
	seconds = time (NULL);
	//NxSoundEffectVSTHost::getSingleton().SetTime( (double)timeGetTime() * 1000000.0L ); // windows
	 NxSoundEffectVSTHost::getSingleton().SetTime( (double)( seconds / 1000.0 ) * 1000000.0L ); // windows


	InputDevice->ProcessDeviceEffectPlugins( FramesInput  );


	if( InputDevice->GetEnableOutput() )
	{
		for ( unsigned int i=0; i< nBufferFrames ; i++ ) {
			float f = FramesInput(i,0) ; 	
			if (f > 1) f = 1.0f;//clipping
			if (f < -1) f = -1.0f;
			*samples++ = f ;
		} 

		for ( unsigned int i=0; i< nBufferFrames ; i++ ) {
			float f = FramesInput(i,1) ; 	
			if (f > 1) f = 1.0f;//clipping
			if (f < -1) f = -1.0f;
			*samples++ = f ;
		} 
	}

	InputDevice->CopyIn( FramesInput );// to non interleaved, for FFT

	return 0;
}
 
}//namespace