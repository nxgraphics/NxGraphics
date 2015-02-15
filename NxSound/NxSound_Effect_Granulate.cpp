#include "NxSoundPreCompiled.h"
#include "STK/include/Granulate.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundEffectGranulate::NxSoundEffectGranulate()
{
	mType = Effect_Granulate ;

	granule = new stk::Granulate();

	granule->setGrainParameters ( 60, 0,  0,  0);//Set global grain parameters used to determine individual grain settings.
	//unsigned int duration, unsigned int rampPercent, int offset, unsigned int delay )
	//Set global grain parameters used to determine individual grain settings.
	//
	//Each grain is defined as having a length of duration milliseconds which must be greater than zero.
	//For values of rampPercent (0 - 100) greater than zero,
	//a linear envelope will be applied to each grain. If rampPercent = 100,
	//the resultant grain "window" is triangular while rampPercent = 50 produces a trapezoidal window.

	//In addition, each grain can have a time delay of length delay and a grain pointer increment of length offset,
	//which can be negative, before the next ramp onset (in milliseconds).
	//The offset parameter controls grain pointer jumps between enveloped grain segments,
	//while the delay parameter causes grain calculations to pause between grains.
	//The actual values calculated for each grain will be randomized by a factor set using the setRandomFactor() function.


	granule->setStretch ( 1 );//Set the stretch factor used for grain playback (1 - 1000). : 5 rapid : 10 moins
	//Set the stretch factor used for grain playback (1 - 1000).
	//Granular synthesis allows for time-stetching without affecting the original pitch of a sound.
	//A stretch factor of 4 will produce a resulting sound of length 4 times the orignal sound.
	//The default parameter of 1 produces no stretching.

	granule->setRandomFactor ( 1.0 ) ; //This factor is used when setting individual grain parameters (0.0 - 1.0). 

	//This factor is used when setting individual grain parameters (0.0 - 1.0).
	//
	//This random factor is applied when all grain state durations are calculated.
	//If set to 0.0, no randomness occurs.
	//When randomness = 1.0, a grain segment of length duration will be 
	//randomly augmented by up to +- duration seconds (i.e., a 30 millisecond length will be augmented by an extra length of up to +30 or -30 milliseconds).

	granule->setVoices( 1 );// 64 * 30(grain length) = 2 second sound.
	//Set the number of simultaneous grain "voices" to use.
	//Multiple grains are offset from one another in time by grain duration / nVoices. 
	//For this reason, it is best to set the !!! grain parameters before calling this function (during initialization). !!!!

}

NxSoundEffectGranulate::~NxSoundEffectGranulate()
{

}

void NxSoundEffectGranulate::SetSoundFile( std::string path )
{
	granule->openFile( path );

}

void NxSoundEffectGranulate::SetVoices( unsigned int nVoices )
{
	granule->setVoices( nVoices );
}

void NxSoundEffectGranulate::SetStretch( unsigned int stretchFactor )
{
	granule->setStretch( stretchFactor );
}

void NxSoundEffectGranulate::SetGrainParameters( unsigned int duration, unsigned int rampPercent, int offset, unsigned int delay )
{
	granule->setGrainParameters( duration, rampPercent, offset, delay );
}

void NxSoundEffectGranulate::SetRandomFactor( double randomness )
{
	granule->setRandomFactor( randomness );
}

void NxSoundEffectGranulate::Reset()
{
	granule->reset();
}

void NxSoundEffectGranulate::Process( float ** BufIn , float ** BufOut , int BlockSize  )
{
	const StkFrames& lastframe = granule->lastFrame();
	unsigned int nChannels = lastframe.channels();

	unsigned int j;
	for ( unsigned int i=0; i< BlockSize ; i++ )
	{
		granule->tick();
		
		for (int j=0; j< nChannels; j++ )
		{
			float * out = BufOut[j] ;
			float f  = granule->lastOut(j); 
			if (f > 1) f = 1.0f;//clipping
			if (f < -1) f = -1.0f;
			out[i] = f  ;
		}
	  
	}
}


}//namespace