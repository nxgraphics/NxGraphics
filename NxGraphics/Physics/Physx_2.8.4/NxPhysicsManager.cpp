#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

template<> NxScene3DPhysicsManager* NxSingleton<NxScene3DPhysicsManager>::ms_Singleton = 0;

NxScene3DPhysicsManager::NxScene3DPhysicsManager()  : physicsSDK(0), nControllerManager(0)
{
	CreatePhysicsManager();
 
}

NxScene3DPhysicsManager::~NxScene3DPhysicsManager(){


}

void NxScene3DPhysicsManager::CreatePhysicsManager() 
{
	NxPhysicsSDKDesc desc;
	desc.setToDefault();
	desc.gpuHeapSize = 128; //128 mb dedicated for the sdk
	//desc.meshCacheSize = 0;
	desc.flags &= ~NX_SDKF_NO_HARDWARE;
	//desc.flags |= NX_SDKF_PER_SCENE_BATCHING;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	LogMsg("Creating Physx SDK...");
	physicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, &errorCode); 
	if(!physicsSDK)
	{
		if( errorCode == NxSDKCreateError::NXCE_PHYSX_NOT_FOUND ) LogMsg("NXCE_PHYSX_NOT_FOUND");
		if( errorCode == NxSDKCreateError::NXCE_WRONG_VERSION ) LogMsg("NXCE_WRONG_VERSION");
		if( errorCode == NxSDKCreateError::NXCE_DESCRIPTOR_INVALID ) LogMsg("NXCE_DESCRIPTOR_INVALID");
		if( errorCode == NxSDKCreateError::NXCE_BUNDLE_ERROR ) LogMsg("NXCE_BUNDLE_ERROR");
		if( errorCode == NxSDKCreateError::NXCE_IN_USE_ERROR ) LogMsg("NXCE_IN_USE_ERROR");
		if( errorCode == NxSDKCreateError::NXCE_DESCRIPTOR_INVALID) LogMsg("NXCE_DESCRIPTOR_INVALID");
		LogMsg("Creating Physx SDK : Failed !");
		return;
	}
	LogMsg("Creating Physx SDK : Done");

	NxHWVersion hwCheck = physicsSDK->getHWVersion();
	if (hwCheck == NX_HW_VERSION_NONE) 
	{
		LogMsg("Warning: Unable to find a PhysX card, or PhysX card used by other application.");
		LogMsg("The soft bodies will be simulated in software.");
		mHardwareSimulation = false;
	}
	else
	{
		mHardwareSimulation = true;
	}

	NxReal scale = 1.0f;// scale is meters per PhysX units = 1.0f; 
	physicsSDK->setParameter( NX_SKIN_WIDTH, 0.05 * ( 1 / scale) );
	physicsSDK->setParameter( NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, 0.15 * 0.15 * ( 1 / scale ) * ( 1 / scale) );
	physicsSDK->setParameter( NX_BOUNCE_THRESHOLD, -2*( 1 / scale ) );
	physicsSDK->setParameter( NX_VISUALIZATION_SCALE, 0.5 * ( 1 / scale) );
	physicsSDK->setParameter( NX_CONTINUOUS_CD, true );
	physicsSDK->setParameter( NX_CCD_EPSILON, 0.01 );
	physicsSDK->setParameter( NX_VISUALIZE_WORLD_AXES, true );
	physicsSDK->setParameter( NX_VISUALIZE_COLLISION_SHAPES, true);
	physicsSDK->setParameter( NX_VISUALIZE_ACTOR_AXES, true);

	physicsSDK->setParameter( NX_VISUALIZE_JOINT_LOCAL_AXES, false); 
	physicsSDK->setParameter( NX_VISUALIZE_JOINT_WORLD_AXES, true );
	physicsSDK->setParameter( NX_VISUALIZE_JOINT_LIMITS, false );

	physicsSDK->setParameter( NX_VISUALIZE_FORCE_FIELDS, true);

	physicsSDK->setParameter( NX_VISUALIZE_FLUID_EMITTERS, true );
	physicsSDK->setParameter( NX_VISUALIZE_FLUID_POSITION, true );

	//physx memory allocator
	PhysicsAllocator = new UserAllocator();
	nControllerManager = NxCreateControllerManager( PhysicsAllocator );
}

void NxScene3DPhysicsManager::DeletePhysicsManager()
{
	NxReleaseControllerManager( nControllerManager );
	if( physicsSDK )
	{
		NxReleasePhysicsSDK( physicsSDK );
		physicsSDK = 0;
	}
}

bool NxScene3DPhysicsManager::DebugPhysics3D( const std::string & TargetIpAddress, int TargetPort )
{
	if( !physicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected() ){
		physicsSDK->getFoundationSDK().getRemoteDebugger()->connect ( TargetIpAddress.c_str(), TargetPort );
		return true ;
	}
	return false;
}

NxPhysicsSDK * NxScene3DPhysicsManager::GetPhysicsSdk()
{
	if(!physicsSDK){ return 0;}
	return physicsSDK;
}

float NxScene3DPhysicsManager::GetSkinWidth()
{
	return physicsSDK->getParameter( NX_SKIN_WIDTH );
}

bool NxScene3DPhysicsManager::HasPhysxHardwareAcceleration()
{
	return mHardwareSimulation;
}

UserAllocator * NxScene3DPhysicsManager::GetPhysics3DMemoryAllocator()
{
	if(!PhysicsAllocator){ return 0;}
	return PhysicsAllocator;
}

NxControllerManager * NxScene3DPhysicsManager::GetPhysicsControllerManager()
{
	if(!nControllerManager){ return 0;}
	return nControllerManager;
}

bool NxScene3DPhysicsManager::UpdateControllers()
{
	GetPhysicsControllerManager()->updateControllers();
	return true;
}

bool NxScene3DPhysicsManager::UpdatePhysics( unsigned long elapsedMS )
{
	UpdateControllers();
	for( size_t i = 0; i < NxScene3DManager::getSingleton().GetNumNxScenes(); i++) {
		NxScene3DManager::getSingleton().GetNx3DScene( i )->UpdatePhysics( 0.0f );
	}
	return true;
}

Nx3D_Scene * NxScene3DPhysicsManager::GetNx3DSceneFromPhysxScene( NxScene * Scene )
{
	Nx3D_Scene * Tmp = 0;
	for( size_t i = 0; i < NxScene3DManager::getSingleton().GetNumNxScenes(); i++) {
		Nx3D_Scene * CurrentScene = NxScene3DManager::getSingleton().GetNx3DScene( i );
		if( Scene == &CurrentScene->GetPhysicScene()->GetPhysicsScene3D() ) {
			Tmp = CurrentScene;
			break;
		} 
	}
	LogMsg("Could not find physx scene." );
	return Tmp;
}

NxScene * NxScene3DPhysicsManager::CreatePhysicsWorld3D()
{
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3(0.0f, -9.81, 0.0f);
	sceneDesc.simType = NX_SIMULATION_HW;
	//sceneDesc.flags |= NX_SF_SEQUENTIAL_PRIMARY; //for cloth
	sceneDesc.flags |= NX_SF_ENABLE_MULTITHREAD;
	//Instruct the SDK to allocate internal threads to each logical processor, skipping the first.
	sceneDesc.threadMask=0xfffffffe;
	sceneDesc.backgroundThreadCount = 1; 
	//The SDK creates two additional worker threads.
	sceneDesc.internalThreadCount = 2;

	/**
	\brief Enables faster but less accurate fluid collision with static geometry.

	If the flag is set static geometry is considered one simulation step late, which 
	can cause particles to leak through static geometry. In order to prevent this, 
	NxFluidDesc.collisionDistanceMultiplier can be increased.
	
	<b>Default:</b> False

	<b>Platform:</b>
	\li PC SW: Yes
	\li GPU  : Yes
	\li PS3  : Yes
	\li XB360: Yes
	\li WII	 : Yes
	*/
	sceneDesc.flags |= NX_SF_FLUID_PERFORMANCE_HINT;

	/**
	\brief Enables multi-threaded version of force field for fluids.

	<b>Default:</b> False

	<b>Platform:</b>
	\li PC SW: Yes
	\li GPU  : Yes [SW]
	\li PS3  : No
	\li XB360: No
	*/
	sceneDesc.flags |= NX_SF_MULTITHREADED_FORCEFIELD;

	if( !mHardwareSimulation )
	{ 
		sceneDesc.simType = NX_SIMULATION_SW;
		LogMsg("Scene Created In Software Mode.");
	}
	else
	{
		LogMsg("Scene Created In Hardware Mode.");
	}

	if( !physicsSDK )
	{
		LogMsg("Physx SDK not found... Please Install");
		return 0;
	}

	LogMsg("Creating Physx Scene " + mHardwareSimulation ? "Hardware" : "Software" );
	 
	LogMsg("Checking Physx Version...");
	NxHWVersion VersionPhysx = physicsSDK->getHWVersion();

	LogMsg("Checking Physx Version done.");
	if( VersionPhysx != NX_HW_VERSION_NONE)  {
		if( NX_HW_VERSION_ATHENA_1_0 == VersionPhysx ) {
			sceneDesc.simType = NX_SIMULATION_HW;
			LogMsg("Physx set to hardware mode ( NX_HW_VERSION_ATHENA_1_0 ) ");
		}
		else {
			LogMsg("Physx set to Software mode");
		}
	} 
	else {
		LogMsg("Physx engine = NX_HW_VERSION_NONE, no hardware acceleration. ");
	}
	 
	NxScene * nScene = physicsSDK->createScene(sceneDesc);

	NxU32 maxIter = 8;
	nScene->setTiming( 1.0f / 60.0f, maxIter, NxTimeStepMethod::NX_TIMESTEP_FIXED ); 

	if(!nScene) return 0;
	NxMaterial*	defaultMaterial = nScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.05f);
	defaultMaterial->setStaticFriction(0.4f);
	defaultMaterial->setDynamicFriction(0.4f);
	PhysicsWorld3DList.push_back( nScene );

	return nScene ;
}

bool NxScene3DPhysicsManager::DeletePhysicsWorld3D( NxScene * Physics3D )
{
	if(!Physics3D) return false;
	Physics3D->simulate( 1.0f / 60.0f );
	Physics3D->flushStream();
	Physics3D->fetchResults( NX_RIGID_BODY_FINISHED, true );
	PhysicsWorld3DList.remove( Physics3D );

	NxU32 nbActors = Physics3D->getNbActors();
	NxActor ** actors = Physics3D->getActors();            
	while ( nbActors-- ) {
		int i = 0;
		NxActor* actor = *actors++;   
		Physics3D->releaseActor( *actor );
	}

	physicsSDK->releaseScene( *Physics3D );
	Physics3D = NULL;
	return true;
}


}