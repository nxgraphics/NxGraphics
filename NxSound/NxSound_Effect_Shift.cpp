#include "NxSoundPreCompiled.h"
#include "STK/include/PitShift.h"

namespace NxSound_Namespace {

NxSoundEffectPitShift::NxSoundEffectPitShift()
{
	mType = Effect_PitShift ;
	shifter = new stk::PitShift();
	shifter->setEffectMix( 1.0f );
}

NxSoundEffectPitShift::~NxSoundEffectPitShift()
{
	
}

void NxSoundEffectPitShift::SetMix( float MixVal )
{
	shifter->setEffectMix( MixVal );
}

float NxSoundEffectPitShift::GetMix()
{
	return 0.0f ;
}

void NxSoundEffectPitShift::SetShift( float Shift )
{
	//float temp = 0.0f ;
	//shifter.setShift( 1.4 * temp + 0.3) ;
	shifter->setShift(  Shift ) ;
}

void NxSoundEffectPitShift::Process( float ** BufIn, float ** BufOut, int BlockSize )
{
	for( int i = 0 ; i < BlockSize ; i++ )
	{
		for(int y = 0 ; y < 2 ; y++ )
		{
			float * in=  BufIn[y] ;
			float * out =  BufOut[y] ;
			out[i] = shifter->tick( in[i] );
		}
	}
}


}//namespace