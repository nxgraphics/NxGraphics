#include "NxSoundPreCompiled.h"

namespace NxSound_Namespace {

NxSoundEffect::NxSoundEffect() : mMuted(false)
{

}

NxSoundEffect::~NxSoundEffect()
{

}

NxSound_Effects_Library_Types NxSoundEffect::GetType()
{
	return mType ; 
}

void NxSoundEffect::SetMuted( bool MuteEffect )
{
	mMuted = MuteEffect;
}

bool NxSoundEffect::IsMuted()
{
	return mMuted;
}

void NxSoundEffect::Process( float ** BufIn , float ** BufOut , int BlockSize )
{

}

void NxSoundEffect::SetMix( float MixVal )
{

}

float NxSoundEffect::GetMix()
{
	return 0.0f ;
}

void NxSoundEffect::SetNumChannels( unsigned int Channels )
{
	PluginChannelNumber = Channels ;
}

unsigned int NxSoundEffect::GetNumChannels()
{
	return PluginChannelNumber ;
}

}