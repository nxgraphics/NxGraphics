#include "NxSoundPreCompiled.h"
#include "STK/include/ModalBar.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaInstModal::NxSoundMediaInstModal() 
{
	mType = TYPE_STK_INSTRUMENT ;
	mMuted = false ;
	mStopped = false ;
	mPaused = false ;
	mVolume = 1.0f ;
	Inst = new stk::ModalBar();
	mNumChannels = Inst->channelsOut();
	Inst->setStickHardness( 1.0 );
	Inst->setPreset( 1 );
}

NxSoundMediaInstModal::~NxSoundMediaInstModal()
{
 
}

void NxSoundMediaInstModal::Play()
{
	Inst->strike( 1.0f );
	Inst->setFrequency(100);
}

void NxSoundMediaInstModal::Stop()
{

}

void NxSoundMediaInstModal::SetPreset( int Preset )
{
	Inst->setPreset( Preset );
	preset = Preset ;
}
int NxSoundMediaInstModal::GetPreset()
{
	return preset ;
}

StkFrames& NxSoundMediaInstModal::Update( StkFrames& frames )
{
	for ( unsigned int i=0; i< frames.size() ; i++ )
	{
		for ( int j=0; j< mNumChannels; j++ )
		{
			frames(i,j) = Inst->tick( j );
		}
	}

	return frames ;
}


}//namespace