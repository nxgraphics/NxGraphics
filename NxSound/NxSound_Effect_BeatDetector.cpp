#include "NxSoundPreCompiled.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define FREQ_LP_BEAT 500.0f    // Low Pass filter frequency
#define T_FILTER 1.0f/(2.0f*M_PI*FREQ_LP_BEAT)  // Low Pass filter time constant
#define BEAT_RTIME 0.02f  // Release time of enveloppe detector in second

namespace NxSound_Namespace {

NxSoundEffectBeatDetector::NxSoundEffectBeatDetector()
{
	mType = Effect_Beat ;	
	Filter1Out=0.0;
	Filter2Out=0.0;
	PeakEnv=0.0;
	BeatTrigger=false;
	PrevBeatPulse=false;
	float sampleRate = 44100.0f ;
	KBeatFilter=1.0/(sampleRate*T_FILTER);
	BeatRelease=(float)exp(-1.0f/(sampleRate*BEAT_RTIME));
}

NxSoundEffectBeatDetector::~NxSoundEffectBeatDetector()
{
	
}

void NxSoundEffectBeatDetector::Process( float ** BufIn , float ** BufOut , int BlockSize  )
{
	float * tmp = BufIn[0];
	float input = tmp[ BlockSize / 2 ] ;
	float EnvIn;
	// Step 1 : 2nd order low pass filter (made of two 1st order RC filter)
	Filter1Out=Filter1Out+(KBeatFilter*(input-Filter1Out));
	Filter2Out=Filter2Out+(KBeatFilter*(Filter1Out-Filter2Out));
	// Step 2 : peak detector
	EnvIn=fabs(Filter2Out);
	if (EnvIn>PeakEnv) PeakEnv=EnvIn;  // Attack time = 0
	else
	{
		PeakEnv*=BeatRelease;
		PeakEnv+=(1.0f-BeatRelease)*EnvIn;
	}

	float PeakThreshold = 0.30; //0.30 ;

	// Step 3 : Schmitt trigger
	if (!BeatTrigger)
	{
		if( PeakEnv > PeakThreshold )
		{
			BeatTrigger = true;
		}
	}
	else
	{
		if( PeakEnv < 0.15 )
		{
			BeatTrigger = false;
		}
	}

	// Step 4 : rising edge detector
	BeatPulse=false;
	if ((BeatTrigger)&&(!PrevBeatPulse))
	BeatPulse=true;
	PrevBeatPulse=BeatTrigger;

	if( BeatPulse )
	{
		//cout << "pulse" ;
	}
}

}//namespace