#include <NxGraphicsPreCompiled.h>
//Terrain
#include "../../nxdeps/include/OGRE/Terrain/OgreTerrain.h"

namespace Nx {

NxScenePhysics::NxScenePhysics( Nx3D_Scene * Scene ) : mScene( Scene ), nScene(0), DebugPhysics(0), nPhysxEnable(false)
{
	nPhysxScenePath = "";
}

NxScenePhysics::~NxScenePhysics()
{

}

void NxScenePhysics::CreatePhysicsEngine3D()
{
 	
}

void NxScenePhysics::SetSceneCollisionFlags() 
{

}

void NxScenePhysics::DeletePhysicsEngine3D()
{
 
}

void NxScenePhysics::EnablePhysics( bool Enable )
{
	 
}

void NxScenePhysics::ResetPhysicsEngine3D()
{
 
}

int NxScenePhysics::LoadPhysxScene( std::string pFilename, NxScene * PhysxScene, NxFileType type )
{
	return false;
}

bool NxScenePhysics::UpdatePhysics( float Time )
{
	return true;
}

void NxScenePhysics::SetDebugPhysics3D( bool Debug )
{
	 
}

void NxScenePhysics::DrawDebug() 
{
 
}
 
NxScene & NxScenePhysics::GetPhysicsScene3D()
{
	return *nScene;
}

void NxScenePhysics::SetGravity( const Nx::Vector3 & GravityVector )
{
	 
}

unsigned int NxScenePhysics::GetNumFluids()
{
	return 0;
}

void NxScenePhysics::SetGroupCollisionFlag( int groupA, int groupB, bool collide )
{
	 
}

NxPhysicsRevoluteJoint * NxScenePhysics::CreateRevoluteJoint( const NxJointRevoluteDesc & JointDesc )
{
	return new NxPhysicsRevoluteJoint( JointDesc );
}

NxPhysicsD6Joint * NxScenePhysics::CreateD6Joint( const NxJointD6Desc & JointDesc )
{
	return new NxPhysicsD6Joint( JointDesc );
}

void NxScenePhysics::CreateVortexForceField(const Nx::Vector3& pos, NxActor* actor )
{
	 
}

void NxScenePhysics::CreateHeightFiled( Ogre::Terrain * terr, const Ogre::Vector3 & TerrainPosition )
{
 
}

}