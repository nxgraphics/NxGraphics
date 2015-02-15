#include "NxGraphicsPreCompiled.h"
#include "../nxsound/NxSound_Headers.h"

namespace Nx {

NxScene3DObjectSound::NxScene3DObjectSound( const std::string & SoundName, const std::string & SoundFile )
{

	NxSoundDeviceOutput * DevOut = NxSoundManager::getSingleton().GetDeviceOutput( 0 );
	
	mSound = NxSoundManager::getSingleton().AddMediaFile( SoundName, DevOut, SoundFile );
	mSound->Play();
	mSound->SetRate( 1.0f ) ;// from 0.0f to 1.0f - 0.5 play half rate
	mSound->SetVolume( 1.0f );// from 0.0f to 1.0f
	mSound->AddAudioPlugin( new NxSoundEffect3D() );

	( ( NxSoundEffect3D * ) mSound->GetAudioPlugin( 0 ) )->SetPosition( 0, 0, 0);
	( ( NxSoundEffect3D * ) mSound->GetAudioPlugin( 0 ) )->SetRange( 1.0f, 20.0f );
}

NxScene3DObjectSound::~NxScene3DObjectSound()
{

	mSound->RemoveAudioPlugin( mSound->GetAudioPlugin( 0 ) ); 
	//NxSoundManager::getSingleton().DeleteMediaFile( mSound ); // should be name
	//NxSoundManager::getSingleton().RemoveMediaFile( mSound );
}

void NxScene3DObjectSound::SetRange( const Nx::Vector2 & MinMaxRange )
{
	( ( NxSoundEffect3D * ) mSound->GetAudioPlugin( 0 ) )->SetRange( MinMaxRange.x, MinMaxRange.y );
}

void NxScene3DObjectSound::SetDirection( const Nx::Vector3 & Direction )
{
	//Vector3 axes[3]; NodeObject->getOrientation().ToAxes(axes);
	//( ( NxSoundEffect3D * ) SoundFile->GetAudioPlugin( 0 ) )->SetDirection( axes[0].x , axes[1].y, axes[2].z );

	( ( NxSoundEffect3D * ) mSound->GetAudioPlugin( 0 ) )->SetDirection( Direction.x, Direction.y, Direction.z );
}

void NxScene3DObjectSound::SetPosition( const Nx::Vector3 & Position )
{
	( ( NxSoundEffect3D * ) mSound->GetAudioPlugin( 0 ) )->SetPosition( Position.x, Position.y, Position.z );
}

const Nx::Vector3 & NxScene3DObjectSound::GetPosition() const
{
	Nx::Vector3 vec = Nx::Vector3( 0,0,0 );
	return vec;
}

}//namespace