#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

NxParticleEmitter::NxParticleEmitter( NxParticleSystem * Parent, const std::string & ParticleSystemName, NxParticleEmitterType Type )  
{ 

	mParticleEmitter = NULL;

	std::string TypeStr = "";

	if( Type == NxParticleEmitterPoint ) {//NxParticleEmitterTypes::
		TypeStr = "Point";
	}
	else if ( Type == NxParticleEmitterJong ) {//NxParticleEmitterTypes::
		TypeStr = "Jong";
	}
	else
	{
		LogMsg("No affector type defined !");
	}
	if(!TypeStr.empty())
	{
		mParticleEmitter = Parent->GetParentSceneNode()->getCreator()->getParticleSystem(ParticleSystemName)->addEmitter(TypeStr);
	}
 	 

}

NxParticleEmitter::~NxParticleEmitter()
{
 
}

void NxParticleEmitter::SetColourFades( const NxColourValue& colourStart, const NxColourValue& colourEnd )
{
	mParticleEmitter->setColour( NxColourtoOgre( colourStart), NxColourtoOgre(colourEnd));
}

void NxParticleEmitter::SetEnabled(bool enabled)
{
	mParticleEmitter->setEnabled( enabled);
}

void NxParticleEmitter::SetStartTime( float startTime)
{
	mParticleEmitter->setStartTime(startTime);
}

void NxParticleEmitter::SetDuration( float duration)
{
	mParticleEmitter->setDuration(duration);
}

void NxParticleEmitter::SetRepeatDelay( float duration)
{
	mParticleEmitter->setRepeatDelay(duration);
}

void NxParticleEmitter::SetAngle( float Degree )
{
	mParticleEmitter->setAngle(Ogre::Degree(Degree) );
}

void NxParticleEmitter::SetTimeToLive( float SecondsTime )
{
	mParticleEmitter->setTimeToLive( SecondsTime );
}

void NxParticleEmitter::SetEmissionRate( float particlesPerSecond )
{
	mParticleEmitter->setEmissionRate( particlesPerSecond );
}

void NxParticleEmitter::SetParticleVelocity( float Velocity )
{
	mParticleEmitter->setParticleVelocity( Velocity );
}

void NxParticleEmitter::SetDirection( const Ogre::Vector3 & Direction )
{
	mParticleEmitter->setDirection(Direction);
}

void NxParticleEmitter::SetPosition( const Ogre::Vector3 & Position )
{
	mParticleEmitter->setPosition( Position );
}

void NxParticleEmitter::SetParameter( const std::string & Param,  const std::string & Value )
{
	mParticleEmitter->setParameter(Param,Value);
}
 

}//namespace