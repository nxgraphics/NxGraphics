#include "NxSoundPreCompiled.h"
#include "STK/include/Echo.h"

namespace NxSound_Namespace {

NxSoundEffectEcho::NxSoundEffectEcho()
{
	mType = Effect_Echo ;
	EchoEffect = new stk::Echo();
	EchoEffect->setEffectMix( 1.0f );
}

NxSoundEffectEcho::~NxSoundEffectEcho()
{
	
}

void NxSoundEffectEcho::SetMix( float MixVal )
{
	EchoEffect->setEffectMix( MixVal );
}

float NxSoundEffectEcho::GetMix()
{
	return 0.0f ;
}

void NxSoundEffectEcho::Clear()
{
	EchoEffect->clear();
}

void NxSoundEffectEcho::SetMaximumDelay( unsigned long delay )
{
	EchoEffect->setMaximumDelay( delay );
}

void NxSoundEffectEcho::SetDelay( unsigned long delay )
{
	EchoEffect->setDelay( delay );
}

void NxSoundEffectEcho::Process( float ** BufIn, float ** BufOut, int BlockSize )
{
	for( int i = 0 ; i < BlockSize ; i++ )
	{
		for(int y = 0 ; y < 2 ; y++ )
		{
			float * in=  BufIn[y] ;
			float * out =  BufOut[y] ;
			out[i] = EchoEffect->tick( in[i] );
		}
	}

}


}//namespace