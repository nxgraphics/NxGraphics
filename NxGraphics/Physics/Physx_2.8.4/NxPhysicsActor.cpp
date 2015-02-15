#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsActor::NxPhysicsActor( NxNode * Node, const std::string & Name, const PhysicGroup Property ) : mPhysicProperty( Property ), mName( Name )
{
	NxBodyDesc nxBoxBodyDesc;
	nxBoxBodyDesc.angularDamping = 0.05f;
	nxBoxBodyDesc.linearVelocity = NxVec3(0,0,0);
	 
	//default shape.
	NxSphereShapeDesc SphereShapeDesc;
	SphereShapeDesc.radius = 0.5f;

	NxActorDesc nxBoxActorDesc;
	nxBoxActorDesc.shapes.pushBack(&SphereShapeDesc);

	nxBoxActorDesc.body = Property == PhysicGroup::GROUP_STATIC ? NULL : &nxBoxBodyDesc; 
	nxBoxActorDesc.density = 10.0f;
	nxBoxActorDesc.globalPose.t = NxVec3( 0, 0, 0 );

	//mName = Name;
	nxBoxActorDesc.name = mName.c_str(); 

	mActor = Node->GetNxScene().GetPhysicScene()->GetPhysicsScene3D().createActor(nxBoxActorDesc);

	//if(!mActor) {
	//	Log("Could not create physx Actor !");
	//}
	//mActor->setName( Name.c_str() );
	mActor->setGlobalPosition( NxVec3ToPhysx ( Node->GetPosition() ) );

	NxShape* const *shape = mActor->getShapes();
	NxSphereShape *sphereShape = shape[0]->isSphere();
	assert(sphereShape);
	mActor->releaseShape(*sphereShape);
 
	mActor->setContactReportFlags( NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );

	if( Property != PhysicGroup::GROUP_STATIC ) {
		mActor->raiseBodyFlag( NxBodyFlag::NX_BF_ENERGY_SLEEP_TEST );
		mActor->setSleepEnergyThreshold( 0.5f );
	}

	SetBodyProperties( Property );

}

NxPhysicsActor::~NxPhysicsActor()
{
	LogMsg("Deleting NxActor....");
 
	LogMsg("Deleting Fluids if any....");
	for (std::vector<NxPhysicsFluid*>::const_iterator i = mFluids.begin(), e = mFluids.end(); i != e; ++i) { 
		delete *i; // releasefluid
	}
	mFluids.clear();

	LogMsg("Deleting Magnets if any....");
	for (std::vector<NxPhysicsForceField*>::const_iterator i = mForceFields.begin(), e = mForceFields.end(); i != e; ++i) { 
		delete *i;
	}
	mForceFields.clear();

	NxScene & mScene = mActor->getScene();

	LogMsg("Releasing Actor... ");
	mScene.releaseActor( *mActor );
	mActor = NULL;
	LogMsg("Deleting NxActor : DONE ");

}

bool NxPhysicsActor::IsDynamic()
{
	return mActor->isDynamic();
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
	mActor->userData = Data;
}

void NxPhysicsActor::SetMass( float mass )
{
	mActor->setMass( mass );
} 

void * NxPhysicsActor::GetUserData()
{
	return mActor->userData;
}

void NxPhysicsActor::RaiseBodyFlag( int flags )
{
	mActor->raiseBodyFlag( ( NxBodyFlag )  flags );
}
 
void NxPhysicsActor::SetPosition( const Nx::Vector3 & Position )
{
	mActor->setGlobalPosition( NxVec3ToPhysx( Position ));
}

const Nx::Vector3 & NxPhysicsActor::GetPosition() const
{
	return PhysxToNxVec3( mActor->getGlobalPosition() );
}

void NxPhysicsActor::SetOrientation( const Nx::Quaternion & Orientation )
{
	mActor->setGlobalOrientationQuat(  NxQuatToPhysx(  Orientation ) );
}

const Nx::Quaternion & NxPhysicsActor::GetOrientation() const
{
	return PhysxToQuatNx(  mActor->getGlobalOrientationQuat() );
}

void NxPhysicsActor::AddForceAtPos( const Nx::Vector3& force, const Nx::Vector3& pos, NxForceTypes mode, bool wakeup )
{
	mActor->addForceAtPos( NxVec3ToPhysx( force ), NxVec3ToPhysx( pos ), (NxForceMode) mode, wakeup );
}

void NxPhysicsActor::AddForceAtLocalPos( const Nx::Vector3& force, const Nx::Vector3& pos, NxForceTypes mode, bool wakeup )
{
	mActor->addForceAtLocalPos( NxVec3ToPhysx( force ), NxVec3ToPhysx( pos ), (NxForceMode) mode, wakeup );
}

void NxPhysicsActor::AddForce( const Nx::Vector3 & Force, NxForceTypes mode, bool wakeup )
{
	/* NxF32 coeff = mActor->getMass() * Force;
	mActor->addForceAtLocalPos( VecToPhysx( Direction ) * coeff, NxVec3(0,0,0), NX_IMPULSE);*/
	mActor->addForce( NxVec3ToPhysx( Force ), (NxForceMode) mode, wakeup );
}

void NxPhysicsActor::AddTorque( const Nx::Vector3& torque, NxForceTypes mode, bool wakeup )
{
	mActor->addTorque( NxVec3ToPhysx(torque), (NxForceMode) mode, wakeup );
}
 
void NxPhysicsActor::SetBodyProperties( const PhysicGroup Property )
{
	if( Property == GROUP_DYNAMIC ) // Dynamic
	{
		mActor->clearBodyFlag( NX_BF_KINEMATIC );
		mActor->setContactReportThreshold(0.1);
		mActor->setGroup( GROUP_COLLIDABLE_PUSHABLE );
		NxShape*const* shapes = mActor->getShapes();
		NxU32 nShapes = mActor->getNbShapes();
		while( nShapes-- )
		{
			shapes[nShapes]->setGroup( GROUP_COLLIDABLE_PUSHABLE );	

			NxGroupsMask groupsMask;
			groupsMask.bits0 = 1; 
			groupsMask.bits1 = 1; 
			groupsMask.bits2 = 1; 
			groupsMask.bits3 = 1; 
			shapes[nShapes]->setGroupsMask(groupsMask);
		}
	}
	else if( Property == GROUP_KINEMATIC ) // kinematic
	{
		mActor->raiseBodyFlag( NX_BF_KINEMATIC );
		mActor->setContactReportThreshold(0.1);
		mActor->setGroup( GROUP_COLLIDABLE_NON_PUSHABLE );
		NxShape*const* shapes = mActor->getShapes();
		NxU32 nShapes = mActor->getNbShapes();
		while (nShapes--)
		{
			shapes[nShapes]->setGroup( GROUP_COLLIDABLE_NON_PUSHABLE );	

			NxGroupsMask groupsMask;
			groupsMask.bits0 = 1; 
			groupsMask.bits1 = 1; 
			groupsMask.bits2 = 1; 
			groupsMask.bits3 = 1; 
			shapes[nShapes]->setGroupsMask(groupsMask);


		}
	}
	else if( Property == GROUP_STATIC )
	{
		mActor->setGroup( GROUP_COLLIDABLE_NON_PUSHABLE );

		//mActor->clearBodyFlag( NX_BF_KINEMATIC );
		NxShape*const* shapes = mActor->getShapes();
		NxU32 nShapes = mActor->getNbShapes();
		while (nShapes--)
		{
			shapes[nShapes]->setGroup( GROUP_COLLIDABLE_NON_PUSHABLE );

			NxGroupsMask groupsMask;
			groupsMask.bits0 = 1; 
			groupsMask.bits1 = 1; 
			groupsMask.bits2 = 1; 
			groupsMask.bits3 = 1; 
			shapes[nShapes]->setGroupsMask(groupsMask);
		}
	}
	mPhysicProperty = Property;

}
 
const PhysicGroup NxPhysicsActor::GetBodyProperty() const
{
	return mPhysicProperty; 
}

void NxPhysicsActor::SetActorPairFlags( NxPhysicsActor * actor, int flags )
{
	mActor->getScene().setActorPairFlags(*mActor,*actor->mActor, flags);//  ShapePairFlags(*mShape, *shape->mShape, (NxContactPairFlag)flags );//NX_IGNORE_PAIR
}

NxActor * NxPhysicsActor::GetNxActor()
{
	return mActor;
}

}//namespace