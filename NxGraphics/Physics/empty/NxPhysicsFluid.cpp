#include <NxGraphicsPreCompiled.h>

namespace Nx {

NxPhysicsFluid::NxPhysicsFluid( NxPhysicsActor * Actor, const NxParticleDesc & Desc )
{
	 
}

NxPhysicsFluid::~NxPhysicsFluid()
{
	 
}

unsigned int NxPhysicsFluid::GetMaxParticles()
{
	return 0;
}

NxPhysicsFluidEmitter * NxPhysicsFluid::CreateEmitter( NxPhysicsShape * Shape )
{
	NxPhysicsFluidEmitter * c = new NxPhysicsFluidEmitter( this, Shape );
	mNxEmitters.push_back( c );
	return c;	
}

NxPhysicsFluid::NxEmitterVectorIterator NxPhysicsFluid::GetNxEmitterIterator()
{
	return NxPhysicsFluid::NxEmitterVectorIterator( mNxEmitters.begin(), mNxEmitters.end());
}

NxFluid * NxPhysicsFluid::GetNxFluid()
{
	return 0;//mFluid->getNxFluid();
}

void NxPhysicsFluid::SetExternalAcceleration( const Nx::Vector3& acceleration )
{
	 
}

void NxPhysicsFluid::SetGravity( bool GravityAffected )
{
	 
}

void NxPhysicsFluid::SetViscosity( float Viscosity )
{
	 
}

void NxPhysicsFluid::SetRestitutionForStaticShapes( float rest )
{
	 
}

void NxPhysicsFluid::SetDynamicFrictionForStaticShapes( float Friction )
{
	 
}

void NxPhysicsFluid::SetStaticFrictionForStaticShapes( float Friction )
{
	 
}

void NxPhysicsFluid::SetRestitutionForDynamicShapes( float rest )
{
	 
}

void NxPhysicsFluid::SetDynamicFrictionForDynamicShapes( float friction )
{
 
}

void NxPhysicsFluid::SetStaticFrictionForDynamicShapes( float friction )
{
	 
}

void NxPhysicsFluid::SetCollisionResponseCoefficient( float coefficient )
{
	 
}

const float * NxPhysicsFluid::GetParticlePositions() const
{
	return new float[1];
}

unsigned int NxPhysicsFluid::GetNumParticles()
{
	return 0;
}

}//namepsace
