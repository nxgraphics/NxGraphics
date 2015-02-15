#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsD6Joint::NxPhysicsD6Joint( const NxJointD6Desc & JointDesc )
{

	LogMsg("Creating NxPhysicsD6Joint ... ");
	NxReal gLinearLimit = 1.0f;
	NxReal gSwing1Limit = NxPiF32 / 180.0f * 30.0f;
	NxReal gSwing2Limit = NxPiF32 / 180.0f * 70.0f;
	NxReal gTwistLowLimit = NxPiF32 / 180.0f * -90.0f;
	NxReal gTwistHighLimit = NxPiF32 / 180.0f * 45.0f;

	NxD6JointDesc d6Desc;
	d6Desc.actor[0] = JointDesc.mActor[0]->GetNxActor();
	d6Desc.actor[1] = JointDesc.mActor[1]->GetNxActor();
	d6Desc.setGlobalAnchor( NxVec3ToPhysx( JointDesc.mGlobalAnchor) );
	d6Desc.setGlobalAxis( NxVec3ToPhysx( JointDesc.mGlobalAxis ) );
 
	d6Desc.twistMotion = (NxD6JointMotion)JointDesc.twistMotion;//  NX_D6JOINT_MOTION_FREE; 
	d6Desc.swing1Motion = (NxD6JointMotion)JointDesc.swing1Motion;//NX_D6JOINT_MOTION_FREE; 
	d6Desc.swing2Motion = (NxD6JointMotion)JointDesc.swing2Motion;//NX_D6JOINT_MOTION_FREE; 

	d6Desc.xMotion = (NxD6JointMotion)JointDesc.xMotion;//   NX_D6JOINT_MOTION_LOCKED;
	d6Desc.yMotion = (NxD6JointMotion)JointDesc.yMotion;//  NX_D6JOINT_MOTION_LOCKED;
	d6Desc.zMotion = (NxD6JointMotion)JointDesc.zMotion;//  NX_D6JOINT_MOTION_LOCKED;

	d6Desc.linearLimit.value = gLinearLimit;
	d6Desc.swing1Limit.value = gSwing1Limit;
	d6Desc.swing2Limit.value = gSwing2Limit;
	d6Desc.twistLimit.low.value =  gTwistLowLimit;
	d6Desc.twistLimit.high.value = gTwistHighLimit;

	d6Desc.projectionMode = NX_JPM_POINT_MINDIST;
	//	d6Desc.jointFlags |= NX_JF_COLLISION_ENABLED;

	LogMsg("Creating joint ....");

	NxJoint* d6Joint = JointDesc.mActor[0]->GetNxActor()->getScene().createJoint(d6Desc);

	//Log( "Joint Type : " + Ogre::StringConverter::toString( d6Joint->getType()  ) );
	

	mJoint = (NxD6Joint*)d6Joint->is(NX_JOINT_D6);

 
	LogMsg("Creating NxPhysicsD6Joint : Done");


}

NxPhysicsD6Joint::~NxPhysicsD6Joint()
{

}

}