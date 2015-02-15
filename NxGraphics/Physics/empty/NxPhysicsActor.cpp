#include <NxGraphicsPreCompiled.h>

namespace Nx {

NxPhysicsActor::NxPhysicsActor( NxNode * Node, const std::string & Name, const PhysicGroup Property ) : mPhysicProperty( Property ), mName( Name )
{


}

NxPhysicsActor::~NxPhysicsActor()
{
	

}

bool NxPhysicsActor::IsDynamic()
{
	return false;
}

NxPhysicsBoxShape * NxPhysicsActor::CreateBoxShape( const NxBoxDesc & BoxDesc )
{
	NxPhysicsBoxShape * c = new NxPhysicsBoxShape( this, BoxDesc );
	return c;
}

NxPhysicsCapsuleShape * NxPhysicsActor::CreateCapsuleShape( const NxCapsuleDesc & CapsuleDesc )
{
	NxPhysicsCapsuleShape * c = new NxPhysicsCapsuleShape( this, CapsuleDesc );
	return c;
}

NxPhysicsPlaneShape * NxPhysicsActor::CreatePlaneShape( const NxPlaneDesc & PlaneDesc )
{
	NxPhysicsPlaneShape * c = new NxPhysicsPlaneShape( this, PlaneDesc );
	return c;
}

NxPhysicsMeshShape * NxPhysicsActor::CreateMeshShape( const NxEntityDesc & MeshDesc )
{
	NxPhysicsMeshShape * c = new NxPhysicsMeshShape( this, MeshDesc );
	return c;
}

NxPhysicsFluid * NxPhysicsActor::CreateFluids( const NxParticleDesc & FluidDesc )
{
	NxPhysicsFluid * c = new NxPhysicsFluid( this, FluidDesc );
	mFluids.push_back(c);
	return c;
}

NxPhysicsForceField * NxPhysicsActor::CreateForceField()
{
	NxPhysicsForceField * c = new NxPhysicsForceField( this );
	mForceFields.push_back(c);
	return c;
}
 
const std::string & NxPhysicsActor::GetName() const
{
	return mName;
}

void NxPhysicsActor::SetUserData( void * Data )
{
 
}

void NxPhysicsActor::SetMass( float mass )
{
	 
} 

void * NxPhysicsActor::GetUserData()
{
	return 0;
}

void NxPhysicsActor::RaiseBodyFlag( int flags )
{
	 
}
 
void NxPhysicsActor::SetPosition( const Nx::Vector3 & Position )
{
	 
}

const Nx::Vector3 & NxPhysicsActor::GetPosition() const
{
	return Nx::Vector3(0,0,0);
}

void NxPhysicsActor::SetOrientation( const Nx::Quaternion & Orientation )
{
	 
}

const Nx::Quaternion & NxPhysicsActor::GetOrientation() const
{
	return  Nx::Quaternion();
}

void NxPhysicsActor::AddForceAtPos( const Nx::Vector3& force, const Nx::Vector3& pos, NxForceTypes mode, bool wakeup )
{
	 
}

void NxPhysicsActor::AddForceAtLocalPos( const Nx::Vector3& force, const Nx::Vector3& pos, NxForceTypes mode, bool wakeup )
{
	 
}

void NxPhysicsActor::AddForce( const Nx::Vector3 & Force, NxForceTypes mode, bool wakeup )
{
	 
}

void NxPhysicsActor::AddTorque( const Nx::Vector3& torque, NxForceTypes mode, bool wakeup )
{
	 
}
 
void NxPhysicsActor::SetBodyProperties( const PhysicGroup Property )
{
	 
}
 
const PhysicGroup NxPhysicsActor::GetBodyProperty() const
{
	return GROUP_NONE; 
}

void NxPhysicsActor::SetActorPairFlags( NxPhysicsActor * actor, int flags )
{
	 
}

NxActor * NxPhysicsActor::GetNxActor()
{
	return 0;
}

}//namespace