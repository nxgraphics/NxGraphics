#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsDraw.h>
#include <NxPhysxCallBack.h>
#include <NxPhysxCallBack.h>

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

NxReal ContactObjectThreshold = 0.1f;

void NxScenePhysics::CreatePhysicsEngine3D()
{
	nScene = NxScene3DPhysicsManager::getSingleton().CreatePhysicsWorld3D();
	SetSceneCollisionFlags();
	assert( nScene );
	if( DebugPhysics ) delete DebugPhysics;
	DebugPhysics = new NxPhysics3DDebugDraw( mScene );


	SceneCallback = new Nx3D_SceneCallback( mScene );
	nScene->setUserContactReport( SceneCallback );// Contact Reports
	nScene->setUserTriggerReport( SceneCallback );// Trigger Reports

	
}

void NxScenePhysics::SetSceneCollisionFlags() 
{

	//setGroupCollisionFlag - Specifies if collision should be performed by a pair of shape groups.
	nScene->setGroupCollisionFlag( GROUP_COLLIDABLE_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, true); //collision
	nScene->setGroupCollisionFlag( GROUP_COLLIDABLE_NON_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, true);//collision

	//nScene->setGroupCollisionFlag( GROUP_CHARACTER, GROUP_COLLIDABLE_NON_PUSHABLE , false);//collision


	//setActorGroupPairFlags - With this method one can set contact reporting flags between actors belonging to a pair of groups.
	nScene->setActorGroupPairFlags( GROUP_NON_COLLIDABLE, GROUP_NON_COLLIDABLE, NX_IGNORE_PAIR );
	nScene->setActorGroupPairFlags( GROUP_COLLIDABLE_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, NX_NOTIFY_FORCES | NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD | NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD );
	nScene->setActorGroupPairFlags( GROUP_COLLIDABLE_NON_PUSHABLE, GROUP_COLLIDABLE_PUSHABLE, NX_NOTIFY_FORCES | NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD | NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD );


	//NxConstraintDominance debrisDominance( 0.0f, 1.0f );
	//nScene->setDominanceGroupPair( 1, 2, debrisDominance );   
	//setDominanceGroupPair(NxDominanceGroup group1, NxDominanceGroup group2, NxConstraintDominance & dominance) = 0;

}



void NxScenePhysics::DeletePhysicsEngine3D()
{
 
	NxScene3DPhysicsManager::getSingleton().DeletePhysicsWorld3D( nScene );
	nScene = 0 ;
 
}

NxCCDSkeleton * createCCDfromActor ( NxActor * newActor )
{
	assert(newActor->getNbShapes() == 1);
	NxShape * const * grabShapes = newActor->getShapes();

	// use a couple of casts to get the pointer I need
	NxShape * grabShape = const_cast<NxShape *>(grabShapes[0]);
	NxConvexShape * grabConvex = (NxConvexShape * )grabShape;
	NxConvexMesh * grabMesh = & grabConvex->getConvexMesh();

	// Turn the mesh into a descriptor to read the details
	NxConvexMeshDesc meshDesc;
	grabMesh->saveToDesc(meshDesc);
	NxSimpleTriangleMesh stm;

	stm.numVertices = meshDesc.numVertices;
	stm.numTriangles = meshDesc.numTriangles;
	stm.pointStrideBytes = sizeof(NxVec3);
	stm.triangleStrideBytes = sizeof(NxU32) * 3;

	stm.points = meshDesc.points;
	stm.triangles = meshDesc.triangles;
	stm.flags |= NX_MF_FLIPNORMALS; // if you need.
	return NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk()->createCCDSkeleton(stm);
}

void NxScenePhysics::EnablePhysics( bool Enable )
{
	//Lock();
	nPhysxEnable = Enable ;
	//Unlock();
}

void NxScenePhysics::ResetPhysicsEngine3D()
{
	//Lock();
	//DeletePhysxActors(); // delete Character Controller
	DeletePhysicsEngine3D();
	CreatePhysicsEngine3D();
	int res = LoadPhysxScene( nPhysxScenePath, nScene,  NxFileType::NXFT_XML );
	//CreatePhysxActors();
	//ResetPhysxActors();
	LogMsg("Reloaded Physx Scene");
	//SetNxCameraNavigationMode( GetNxCameraNavigationMode() );
	nScene->simulate( 1.0f/60.0f );
	nScene->flushStream();
	nScene->fetchResults( NX_RIGID_BODY_FINISHED, true );
	//Unlock(); 
}

int NxScenePhysics::LoadPhysxScene( std::string pFilename, NxScene * PhysxScene, NxFileType type )
{
	NX_BOOL success = false;

	if ( NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk() )
	{
		LogMsg("Loading Physx Collection..." );
		NXU::NxuPhysicsCollection * c = NXU::loadCollection( pFilename.c_str(), (NXU::NXU_FileType)type );

		if ( c )
		{
			float RotXOffset = -90.0f ; // offset rotation for 3dsmax loading
			NxVec3 euler( RotXOffset , 0 , 0);

			NxQuat q1, q2, q3;
			q1.fromAngleAxis(euler.x, NxVec3(1, 0, 0));
			q2.fromAngleAxis(euler.y, NxVec3(0, 1, 0));
			q3.fromAngleAxis(euler.z, NxVec3(0, 0, 1));
			NxQuat q = q3 * q2 * q1;

			NxMat33 nxrotx, nxroty, nxrotz;
			nxrotx.fromQuat(q1);
			nxroty.fromQuat(q2);
			nxrotz.fromQuat(q3);

			NxMat33 nxrot;
			nxrot = nxrotz * nxroty * nxrotx;

			NxVec3  * trans = new NxVec3(0,0,0);
			const NxMat34 * rootNode = new NxMat34( nxrot , *trans );

			if( !NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk() || !PhysxScene  )
			{
				LogMsg("LoadPhysxScene : Physx Sdk Not Initialized" );
			}

			nNotification = new NxScene3DPhysics3DNotify();//do parameter Nx3D_Scene

			success = NXU::instantiateCollection( c, *NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk(), PhysxScene, rootNode , nNotification );
			if( !success ) MessageBox(0,"NXU::instantiateCollection Failed!","",0);
			NXU::releaseCollection(c);

			PhysxScene->setFilterBool(false);
			PhysxScene->setFilterOps(NX_FILTEROP_AND, NX_FILTEROP_AND, NX_FILTEROP_AND);

			NxMaterial*	defaultMaterial = PhysxScene->getMaterialFromIndex(0);
			defaultMaterial->setRestitution(0.05f);
			defaultMaterial->setStaticFriction(0.4f);
			defaultMaterial->setDynamicFriction(0.4f);

			nPhysxScenePath = pFilename ;//backup file path info for nx scene

			LogMsg("Loading Physx Collection...Done" );
		}
		else
		{
			LogMsg("Nx3D_Scene::LoadPhysxScene : Failed! for : " + pFilename);

		}
	}
	else
	{
		LogMsg("Nx3D_Scene::LoadPhysxScene : Physx Sdk Not Initialized !" );

	}

	return success;
}

bool NxScenePhysics::UpdatePhysics( float Time )
{
	//if(!&GetPhysicsScene3D() || !nPhysxEnable ) return false ;

	nScene->simulate( 1.0f / 60.0f );
	nScene->flushStream();
	nScene->fetchResults( NX_ALL_FINISHED, true);
	return true;
}

void NxScenePhysics::SetDebugPhysics3D( bool Debug )
{
	DebugPhysics->SetViewDebug( Debug );
}

void NxScenePhysics::DrawDebug() {

	DebugPhysics->Draw(); 
}
 
NxScene & NxScenePhysics::GetPhysicsScene3D()
{
	return *nScene;
}
/*
NxPhysicsSDK * NxScenePhysics::GetPhysicsSdk3D()
{
	return NxScene3DManager::getSingleton().GetPhysicsSdk();
}*/

void NxScenePhysics::SetGravity( const Nx::Vector3 & GravityVector )
{
	nScene->setGravity( NxVec3ToPhysx( GravityVector ) );
}

unsigned int NxScenePhysics::GetNumFluids()
{
	return nScene->getNbFluids();
}

void NxScenePhysics::SetGroupCollisionFlag( int groupA, int groupB, bool collide )
{
	nScene->setGroupCollisionFlag( groupA, groupB, collide ); //collision
}

NxPhysicsRevoluteJoint * NxScenePhysics::CreateRevoluteJoint( const NxJointRevoluteDesc & JointDesc )
{
	return new NxPhysicsRevoluteJoint( JointDesc );
}

NxPhysicsD6Joint * NxScenePhysics::CreateD6Joint( const NxJointD6Desc & JointDesc )
{
	return new NxPhysicsD6Joint( JointDesc );
}

void NxScenePhysics::CreateVortexForceField(const Nx::Vector3& poser, NxActor* actor )
{

	 NxVec3 pos;
	 pos.x = poser.x;
	 pos.y = poser.y;
	 pos.z = poser.z;


	 


	bool m_forceFieldMoved;
	bool m_switchExcludeShape;
	bool m_excludeGroupEnabled;
	NxActor* m_kinematicActor;
	NxForceFieldShape* m_excludeShape;
	NxForceFieldShapeGroup* m_excludeGroup;
	NxForceField* gForceField;
	NxActor* gForceFieldActor;
	NxMat34 m_forceFieldTransform;
	m_forceFieldTransform.id();
	NxForceFieldDesc ffDesc;
	NxForceFieldLinearKernelDesc	lKernelDesc;
	NxForceFieldLinearKernel*		linearKernel;

	ffDesc.coordinates = NX_FFC_CYLINDRICAL;
	//Attach the vortex in an actor (which we use for moving the field around in the world)
	ffDesc.actor = actor;
	//attach the force field at the center of the actor
	m_forceFieldTransform.id(); 
	ffDesc.pose = m_forceFieldTransform;

	//constant force of 30 towards the center (which is then counter-acted by radial forces specified below)
	//constant force of 4 upwards (creating a constant lift on the objects)
	lKernelDesc.constant = NxVec3(-30, 4.0f, 0);  // ori

	//The target where we want the objects to end up is at radius 3 from the center. We use
	//Y=0 as the target in along the y-axis together with the m(0,1)=-5 to create a force
	//directed outwards from the center of the vortex when objects are floating towards the
	//top of the vortex.
	lKernelDesc.positionTarget = NxVec3(3,0,0);

	//Setup radial forces, depending on where the objects are positioned
	NxMat33 m;
	m.zero();
	m(0,0) = 10; //radial error -> radial force. If outside of target radius, act with a force of 10*distance inwards
	m(0,1) = -5; //axial error -> radial force. If the y component of the object position is above the target y position (0), 
	//then act with a force of 5*distance outwards. This reduces the force of 30 inwards that we setup earlier,
	//making the vortex broaden out in the top
	m(0,2) = 0;  //there is no tangential error in cylindrical coordinates, so we just set this to 0

	lKernelDesc.positionMultiplier = m;
	lKernelDesc.noise = NxVec3(1.3,1.3,1.3); //adds a random noise on the forces to make the objects a little more chaotic

	//Set target velocity along the tangent of the vortex to 30 (the other directions to 0)
	lKernelDesc.velocityTarget = NxVec3(0,0,30);
	m.diagonal(NxVec3(1,1,1)); //Acts with a force relative to the current velocity to reach the
	//target velocities. If the velocity is above 0 in radial direction, then
	//the radial velocity is decreased. If the velocity is below 30 in tangential
	//direction, then the velocity is increased until it reaches that velocity.
	lKernelDesc.velocityMultiplier = m;

	//You can try some fall-off forces if you e.g. want the vortex to lose power 
	//along the radial direction when the distance from its center increases:

	//lKernelDesc.falloffLinear = NxVec3(5.0f, 0, 0);
	//lKernelDesc.falloffQuadratic = NxVec3(5.0f, 0, 0);

	linearKernel = nScene->createForceFieldLinearKernel(lKernelDesc);
	ffDesc.kernel = linearKernel;
	ffDesc.flags = 0;

	gForceField = nScene->createForceField(ffDesc);

	//Attach an include shape, we position this so that it covers the vortex specified above
	NxForceFieldShape* s = NULL;
	NxBoxForceFieldShapeDesc b;
	b.dimensions = NxVec3(5, 7, 5);
	b.pose.t = NxVec3(0, 3.5f, 0);
	s = gForceField->getIncludeShapeGroup().createShape(b);

	//Create an exclude shape, positioned around the shed
	NxForceFieldShapeGroupDesc sgDesc;
	sgDesc.flags = NX_FFSG_EXCLUDE_GROUP;
	m_excludeGroup = nScene->createForceFieldShapeGroup(sgDesc);

	NxBoxForceFieldShapeDesc exclude;
	exclude.dimensions = NxVec3(2.25f, 1.5f, 1.75f);
	exclude.pose.t = NxVec3(8.85f, 1.5f, -10.3f);
	m_excludeShape = m_excludeGroup->createShape(exclude);

	gForceField->addShapeGroup(*m_excludeGroup);
}

void NxScenePhysics::CreateHeightFiled( Ogre::Terrain * terr, const Ogre::Vector3 & TerrainPosition )
{
	bool Visualize = false;
	NxHeightFieldDesc heightFieldDesc; 
	heightFieldDesc.nbColumns = terr->getSize(); 
	heightFieldDesc.nbRows = terr->getSize(); 
	heightFieldDesc.convexEdgeThreshold = 0; 
	heightFieldDesc.thickness = 0; 
	heightFieldDesc.format = NX_HF_S16_TM;
	heightFieldDesc.verticalExtent = -1000;
	heightFieldDesc.samples = new NxU32[(terr->getSize()) * (terr->getSize())]; 
	heightFieldDesc.sampleStride = sizeof(NxU32); 

	char* currentByte = (char*)heightFieldDesc.samples; 

	for (NxU32 row = 0; row < terr->getSize(); row++)  {
		for (NxU32 col = 0; col < terr->getSize(); col++)  {
			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte; 
			currentSample->height =(NxI16)*terr->getHeightData(row,terr->getSize() - 1 - col); // ori
			currentSample->materialIndex0 = 0;
			currentSample->materialIndex1 = 0;
			currentSample->tessFlag = 0;
			currentByte += heightFieldDesc.sampleStride;          
		}
	}

	NxPhysicsSDK * PhysxSdk = NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk();
	NxHeightField* heightField = PhysxSdk->createHeightField(heightFieldDesc); 
	delete [] heightFieldDesc.samples; 

	NxHeightFieldShapeDesc heightFieldShapeDesc; 
	heightFieldShapeDesc.heightField = heightField; 
	heightFieldShapeDesc.heightScale =  1.0f; 

	heightFieldShapeDesc.shapeFlags = NX_SF_FEATURE_INDICES; 
	heightFieldShapeDesc.meshFlags   = NX_MESH_SMOOTH_SPHERE_COLLISIONS;

	if(Visualize) heightFieldShapeDesc.shapeFlags|= NX_SF_VISUALIZATION; // slow rendering

	heightFieldShapeDesc.rowScale = terr->getWorldSize() / NxReal(terr->getSize()-1); 
	heightFieldShapeDesc.columnScale = terr->getWorldSize() / NxReal(terr->getSize()-1); 

	heightFieldShapeDesc.materialIndexHighBits = 0;   
	heightFieldShapeDesc.holeMaterial = 2;

	NxActorDesc actorDesc; 
	assert(heightFieldShapeDesc.isValid());      
	actorDesc.shapes.pushBack(&heightFieldShapeDesc); 
	actorDesc.body = NULL; 
	actorDesc.globalPose.t.setx( TerrainPosition.x + ( -terr->getWorldSize() / 2.0f ) );
	actorDesc.globalPose.t.sety( TerrainPosition.y ); 
	actorDesc.globalPose.t.setz( TerrainPosition.z + ( -terr->getWorldSize() / 2.0f ) ); 
	assert(actorDesc.isValid());  

	Nx3D_Scene * Scene = NxScene3DManager::getSingleton().GetNx3DSceneFromOgreSceneManager( terr->getSceneManager() );
	NxActor* mActor = Scene->GetPhysicScene()->GetPhysicsScene3D().createActor(actorDesc);

	// Add Collisions
	mActor->setGroup( GROUP_COLLIDABLE_NON_PUSHABLE );
	NxShape*const* shapes = mActor->getShapes();
	NxU32 nShapes = mActor->getNbShapes();
	while (nShapes--){
		shapes[nShapes]->setGroup( GROUP_COLLIDABLE_NON_PUSHABLE );
		NxGroupsMask groupsMask;
		groupsMask.bits0 = 1; 
		groupsMask.bits1 = 1; 
		groupsMask.bits2 = 1; 
		groupsMask.bits3 = 1; 
		shapes[nShapes]->setGroupsMask(groupsMask);
	}
	;//return mActor;
}




}