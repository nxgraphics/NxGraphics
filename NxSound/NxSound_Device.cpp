#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"
#include "STK/include/Stk.h" 
#include "STK/include/RtAudio.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundDevice::NxSoundDevice( int AudioDeviceId ) : mAudioIndex( AudioDeviceId )
{
	//GenerateID();
}

NxSoundDevice::~NxSoundDevice()
{
	 
}

const std::string NxSoundDevice::GetDeviceName() const
{
	if(mAudioDevice)
	{
		RtAudio::DeviceInfo DeviceInfo ;
		DeviceInfo = mAudioDevice->getDeviceInfo( mAudioIndex ) ;
		return DeviceInfo.name ;
	}
	return "Empty";
}

unsigned int NxSoundDevice::GetNumChannels() const
{
	return mNumOutChannels;
}

unsigned int NxSoundDevice::GetFFTNumBins() const
{
	return mDeviceFFT->GetNumBins();
}

float * NxSoundDevice::GetFFTAmplitude()
{
	mDeviceFFT->setData( inBufs[0] );
	return mDeviceFFT->getAmplitude();
}

// Meter level limits (in dB).
#define QTRACTOR_AUDIO_METER_MAXDB			+6.0f
#define QTRACTOR_AUDIO_METER_MINDB			-70.0f
// Possible 20 * log10(x) optimization
// (borrowed from musicdsp.org)
static inline float log10f2_opt ( float x )
{

#	define M_LOG10F20 6.0205999132796239042f // (= 20.0f * M_LN2 / M_LN10)
	// Avoid strict-aliasing optimization (gcc -O2).
	union { float f; int i; } u;
	u.f = x;
	return M_LOG10F20 * ((((u.i & 0x7f800000) >> 23) - 0x7f)
		+ (u.i & 0x007fffff) / (float) 0x800000);

}
//from BitPattern(http://www.google.fr/url?sa=t&rct=j&q=10.0f%20*%20log10%20decibel&source=web&cd=26&ved=0CFMQFjAFOBQ&url=http%3A%2F%2Fgnu.ethz.ch%2Fdebian%2Fqtractor%2Fqtractor-0.4.3%2Fsrc%2FqtractorAudioMeter.cpp&ei=awThT5G4AYyV0QWW7bXZDA&usg=AFQjCNF2yg8MJ7L8HJ6tQcTiZjMpxl-Sbg&cad=rja)
// IEC standard dB scaling -- as borrowed from meterbridge (c) Steve Harris
//(Note: the range of 96 dB comes from the formula 20 * log10(2^16), where 16 is the number of bits.)
static inline float IEC_Scale ( float dB )
{
	float fScale = 1.0f;

	if (dB < -70.0f)
		fScale = 0.0f;
	else if (dB < -60.0f)
		fScale = (dB + 70.0f) * 0.0025f;
	else if (dB < -50.0f)
		fScale = (dB + 60.0f) * 0.005f + 0.025f;
	else if (dB < -40.0)
		fScale = (dB + 50.0f) * 0.0075f + 0.075f;
	else if (dB < -30.0f)
		fScale = (dB + 40.0f) * 0.015f + 0.15f;
	else if (dB < -20.0f)
		fScale = (dB + 30.0f) * 0.02f + 0.3f;
	else if (dB < -0.001f || dB > 0.001f)  /* if (dB < 0.0f) */
		fScale = (dB + 20.0f) * 0.025f + 0.5f;

	return fScale;
}

float * NxSoundDevice::GetFFTDecibels()
{
	mDeviceFFT->setData( inBufs[0] );
	float * Amp = mDeviceFFT->getAmplitude();

	for(unsigned int i = 0 ; i < mDeviceFFT->GetNumBins(); i++ )
	{
		float dB = QTRACTOR_AUDIO_METER_MINDB;
		if (Amp[i] > 0.0f)
			dB = log10f2_opt( Amp[i]  ); // // dB = 20.0f * log10 ( sqrtf( Amp[i] ) );//
		if (dB < QTRACTOR_AUDIO_METER_MINDB)
			dB = QTRACTOR_AUDIO_METER_MINDB;
		else if (dB > QTRACTOR_AUDIO_METER_MAXDB)
			dB = QTRACTOR_AUDIO_METER_MAXDB;

		 Amp[i] = IEC_Scale(dB);  // float iValue

	}
	return Amp;
}

float * NxSoundDevice::GetFFTData()
{
	mDeviceFFT->setData( inBufs[0] );
	return mDeviceFFT->getData();
}

bool NxSoundDevice::AddAudioPlugin( NxSoundEffect * Effect )
{
	if(!Effect) return false ;
	mDevicePluginList.push_back(Effect);
	return true ;
}

void NxSoundDevice::AddListener( NxSoundManagerListener * listener )
{
	NxSoundManager::getSingleton().AddListener(listener); 
}

void NxSoundDevice::CreateAudioBuffers()
{
	int kBlockSize = SND_BUFFER_SIZE * mNumOutChannels  ;
	
	inBufs = new float*[mNumOutChannels];
	for (int f = 0; f < mNumOutChannels; f++)
	{
		inBufs[f] = new float[kBlockSize];
		memset( inBufs[f] , 0 ,  kBlockSize * sizeof(float) );
	}

	outBufs = new float*[mNumOutChannels];
	for (int i = 0; i < mNumOutChannels; i++)
	{
		outBufs[i] = new float[kBlockSize];
		memset (outBufs[i], 0, kBlockSize * sizeof (float));
	}
}

void NxSoundDevice::CopyIn( StkFrames & Frame )
{
	//interleaved to non interleaved
	for (int f = 0; f < Frame.channels()  ; f++)
	{
		for ( unsigned int i=0; i< Frame.frames()  ; i++ )
		{  inBufs[f][i] = Frame(i , f); } 
	}
}

void NxSoundDevice::CopyOut( StkFrames & Frame )
{
	//non interleaved to interleaved
	for(int f = 0 ; f < 2 ; f++)
	{
		for ( unsigned int i=0; i< SND_BUFFER_SIZE ; i++ )
		{
			float * ptr = outBufs[f] ;
			Frame(i,f) = ptr[i]  ;
		} 
	}
}

void NxSoundDevice::ProcessDeviceEffectPlugins( StkFrames & Frame )
{
	for( std::list< NxSoundEffect * >::iterator iter = mDevicePluginList.begin(); iter != mDevicePluginList.end(); iter++)
	{
		NxSoundEffect * Plugin = *iter; 
		if( Plugin->GetType() == Effect_VST_Effect )  
		{
			CopyIn( Frame );
			Plugin->Process(inBufs, outBufs, SND_BUFFER_SIZE);
			CopyOut( Frame );
		}
		else if( Plugin->GetType() == Effect_3D  )
		{
			NxSoundEffect3D * Spatialplug = ( NxSoundEffect3D * ) Plugin ;
			CopyIn( Frame ) ;
			//provide a interleaved buffer 
			Spatialplug->Process( inBufs, outBufs, SND_BUFFER_SIZE );
			//CopyOut( Frame );
		}
	}
}


}//namespace