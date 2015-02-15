#include "NxSoundPreCompiled.h"
#include "STK/include/Chorus.h"

namespace NxSound_Namespace {

NxSoundEffectChorus::NxSoundEffectChorus()
{
	mType = Effect_Chorus ;
	ChorusEffect = new stk::Chorus();
	ChorusEffect->setEffectMix( 1.0f );
}

NxSoundEffectChorus::~NxSoundEffectChorus()
{
	
}

void NxSoundEffectChorus::SetMix( float MixVal )
{
	ChorusEffect->setEffectMix( MixVal );
}

float NxSoundEffectChorus::GetMix()
{
	return 0.0f ;
}

void NxSoundEffectChorus::SetModFrequency( float Shift )
{
	 ChorusEffect->setModFrequency( Shift );
}

void NxSoundEffectChorus::SetModDepth( float depth )
{
	ChorusEffect->setModDepth( depth );
}

void NxSoundEffectChorus::Process( float ** BufIn , float ** BufOut , int BlockSize  )
{
	for( int i = 0 ; i < BlockSize ; i++ )
	{
		for(int y = 0 ; y < 2 ; y++ )
		{
			float * in=  BufIn[y] ;
			float * out =  BufOut[y] ;
			out[i] = ChorusEffect->tick( in[i] );
		}
	}
}


}//namespace