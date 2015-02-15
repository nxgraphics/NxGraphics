#include "NxGraphicsPreCompiled.h"

#include "../NxVideo/NxVideo_Player.h"
#include "../NxVideo/NxVideo_Video.h"
#include "../NxVideo/NxVideo_Utilities.h"
using namespace NxVideo_Namespace;

 #include <fstream>


#include "../NxSound/NxSound_Headers.h"
using namespace NxSound_Namespace;

namespace Nx {

	//ofstream * myFile;

NxFrameBufferVideo::NxFrameBufferVideo( NxTexture * Layer ) : NxFrameBuffer( Layer )
{

	// myFile = new ofstream("data.bin", ios::out | ios::binary);

	//unsigned long posFrame = ((NxTextureVideo* )mLayer)->GetVideo()->GetPositionFrame();
	//mAudioSampleindex = ceil(44100.0f/25.0f) * posFrame;
	
}

NxFrameBufferVideo::~NxFrameBufferVideo()
{
	LogMsg("~NxFrameBufferVideo() ... ");

}
 
bool NxFrameBufferVideo::UpdateTexture( float time )
{
	// check if new frame is available.
	if( ((NxTextureVideo* )mLayer)->GetVideo()->NewFrame() )
	{
 
		if( ((NxTextureVideo* )mLayer)->GetVideo()->HasAudio()){
			// get audio data
			NxVideoStreamFlags flags;
			unsigned long posFrame = ((NxTextureVideo* )mLayer)->GetVideo()->GetPositionFrame();
			float framerate = ((NxTextureVideo* )mLayer)->GetVideo()->GetFramerate();
			unsigned int numSamples = ((NxTextureVideo* )mLayer)->GetVideo()->GetAudioBufferSize();
			unsigned long SampleRate = ((NxTextureVideo* )mLayer)->GetVideo()->GetAudioSampleRate();
			unsigned short SourceNumChannels = ((NxTextureVideo* )mLayer)->GetVideo()->GetAudioNumChannels();
 
			if( posFrame == 1 ){ 
				mAudioSampleindex = 0;
				mSound->Flush(); 
			}
 
			float AudioSampleindexTmp = ceil((float)SampleRate/framerate) * (float)posFrame+1;
			unsigned int boundMax = 0;// SampleRate / 16 ;
 

			double format = 44100.0 /(double) SampleRate;
			numSamples /= format;
		
			//synchro audio !??
			if( mAudioSampleindex > (AudioSampleindexTmp + boundMax ) ){ mSound->Flush(); mAudioSampleindex = AudioSampleindexTmp;   }
	 
			unsigned int NumFramesOut = numSamples; 
			if( ((NxTextureVideo* )mLayer)->GetVideo()->GetAudioData( mAudioSampleindex, &mAudioBuffer[0], &NumFramesOut, &flags ) ) {
				mAudioSampleindex += NumFramesOut;
				mSound->Feed( (char *)  &mAudioBuffer[0] , NumFramesOut, (double) SampleRate, 32 );	 
			}

		}
 
		// get video data
		((NxTextureVideo* )mLayer)->GetVideo()->GetPixelData( &LayerBuffer[0] );
		if( !((NxTextureVideo* )mLayer)->GetVideo()->GetOpenGLID() ){ ProcessVideoPlugins(); }
		

		//LARGE_INTEGER lpFrequency;
		//QueryPerformanceFrequency(&lpFrequency);

		//LARGE_INTEGER start , stop;
		//QueryPerformanceCounter(&start );

		//QueryPerformanceCounter(&stop);
		//printf("lock time (1) = %5.2f\n", ((double)(stop.LowPart-start.LowPart))/((double)lpFrequency.LowPart)*1000.f);


	}

	return true;
}

}//namespace

 