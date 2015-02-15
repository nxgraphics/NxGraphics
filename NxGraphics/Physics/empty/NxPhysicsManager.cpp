#include <NxGraphicsPreCompiled.h>

namespace Nx {

template<> NxScene3DPhysicsManager* NxSingleton<NxScene3DPhysicsManager>::ms_Singleton = 0;

NxScene3DPhysicsManager::NxScene3DPhysicsManager()  : physicsSDK(0), nControllerManager(0)
{
	
}

NxScene3DPhysicsManager::~NxScene3DPhysicsManager(){


}

void NxScene3DPhysicsManager::CreatePhysicsManager() 
{
	
}

void NxScene3DPhysicsManager::DeletePhysicsManager()
{
	
}

bool NxScene3DPhysicsManager::DebugPhysics3D( const std::string & TargetIpAddress, int TargetPort )
{
	
	return false;
}

NxPhysicsSDK * NxScene3DPhysicsManager::GetPhysicsSdk()
{
	return 0;
}

float NxScene3DPhysicsManager::GetSkinWidth()
{
	return 0.0f;
}

bool NxScene3DPhysicsManager::HasPhysxHardwareAcceleration()
{
	return false;
}

UserAllocator * NxScene3DPhysicsManager::GetPhysics3DMemoryAllocator()
{
	return 0;
}

NxControllerManager * NxScene3DPhysicsManager::GetPhysicsControllerManager()
{
	return 0;
}

bool NxScene3DPhysicsManager::UpdateControllers()
{
	return true;
}

bool NxScene3DPhysicsManager::UpdatePhysics( unsigned long elapsedMS )
{

	return true;
}

Nx3D_Scene * NxScene3DPhysicsManager::GetNx3DSceneFromPhysxScene( NxScene * Scene )
{
	
	return 0;
}

NxScene * NxScene3DPhysicsManager::CreatePhysicsWorld3D()
{


	return 0 ;
}

bool NxScene3DPhysicsManager::DeletePhysicsWorld3D( NxScene * Physics3D )
{
	
	return true;
}


}