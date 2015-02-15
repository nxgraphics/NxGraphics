#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxParticleSystem::NxParticleSystem( NxNode * parentNode, unsigned int NumParticles ) : NxEntity(), mNumParticles( NumParticles) 
{
	mType = NxTypeParticleSystem;
	Nx3D_Scene * CurrentScene = &parentNode->GetNxScene(); 
	 
	mParticleSystem = CurrentScene->GetNxSceneManager()->createParticleSystem( NumParticles );
	 

	mParticleSystem->setMaterialName("PointSprite");
	mParticleSystem->setDefaultDimensions(25, 25);
 
	mNode = CurrentScene->GetNxSceneManager()->createSceneNode();
 
	mNode->attachObject( mParticleSystem );

	mNxNodeParent = parentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );
 

}

NxParticleSystem::~NxParticleSystem()
{
	mNxNodeParent->GetNxSceneNode()->removeChild( mNode );
	LogMsg("Deleting Node : "+ mNode->getName());
	mNxNodeParent->GetNxScene().GetNxSceneManager()->destroySceneNode( mNode );
	LogMsg("~Nx3DObjectParticleSystem() DONE ");
}

void NxParticleSystem::FastForward( float time, float interval  )
{
	mParticleSystem->fastForward( time, interval );
}

unsigned int NxParticleSystem::GetNumParticles()
{
	return mParticleSystem->getNumParticles();
}

void NxParticleSystem::Clear()
{
	mParticleSystem->clear();
}

void NxParticleSystem::SetSortingEnabled( bool sort  )
{
	mParticleSystem->setSortingEnabled( sort );
}

void NxParticleSystem::SetCullIndividually( bool Cull  )
{
	mParticleSystem->setCullIndividually( Cull );
}

void NxParticleSystem::SetSpeedFactor( float speed )
{
	mParticleSystem->setSpeedFactor( speed);
}

void NxParticleSystem::CreateParticle( const Ogre::Vector3 & pos, const Ogre::Vector3 & dir, float Velocity, float TTL, float RotSpeed )
{
	Ogre::Particle* particle = mParticleSystem->createParticle();
	if(particle == NULL){ LogMsg( "Could not create particle, increase your quota "); return; }
	particle->particleType = Ogre::Particle::Visual;//Ogre::Particle::ParticleType::Visual;
	particle->position = pos;
	particle->direction = dir * Velocity;
	particle->timeToLive = particle->totalTimeToLive = TTL;
	//particle->rotation = Ogre::Degree(0).valueRadians();
	particle->rotationSpeed = Ogre::Degree(RotSpeed).valueRadians();
}

void NxParticleSystem::SetParticlePosition( unsigned int index, const Ogre::Vector3 & pos )
{
	mParticleSystem->getParticle(index)->position = pos;
}

NxParticleEmitter * NxParticleSystem::CreateEmitter( NxParticleEmitterType  Type )
{
	return new NxParticleEmitter( this, mParticleSystem->getName(), Type );
}

NxParticleSystemAffector * NxParticleSystem::CreateAffector( NxParticleSystemAffectorTypes Type )
{
	return new NxParticleSystemAffector( this, mParticleSystem->getName(), Type );
}

void NxParticleSystem::SetVisible( bool Visible )
{
	mParticleSystem->setVisible( Visible );
}

void NxParticleSystem::Update( float time )
{

}

void NxParticleSystem::SetParticlesSize( float SizeX, float SizeY )
{
	mParticleSystem->setDefaultDimensions( SizeX, SizeY );
}

void NxParticleSystem::SetMaterialName( std::string MaterialName )
{
	mParticleSystem->setMaterialName( MaterialName );
}

const std::string & NxParticleSystem::GetMaterialName()
{
	return mParticleSystem->getMaterialName();
}


}//namespace