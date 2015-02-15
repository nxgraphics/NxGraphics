#include "NxMocapPreCompiled.h"

namespace NxMocap_Namespace {

NxMocapUser::NxMocapUser( unsigned int UserId ) : NxMocapSkeleton( UserId ), mSkeletonTracked( false ){

	CreateSkeleton();
	mUserPosition[0] = mUserPosition[1] = mUserPosition[2] = 0.0f;
	mValid = true;
}

NxMocapUser::~NxMocapUser()
{

}

const unsigned int NxMocapUser::GetUserId()
{
	return NxMocapSkeleton::GetUserId();
}

void NxMocapUser::CreateSkeleton()
{

	//CreateJoint( "Bip01 Spine1", NxSkeletonJoint::NX_SKEL_SPINE1 );
	//CreateJoint( "Bip01 Spine", NxSkeletonJoint::NX_SKEL_SPINE );
	//CreateJoint( "Bip01 Pelvis", NxSkeletonJoint::NX_SKEL_PELVIS );
	CreateJoint( "Bip01 Spine2",  NX_SKEL_SPINE2 );
	CreateJoint( "Bip01 L UpperArm",  NX_SKEL_L_UPPERARM );
	CreateJoint( "Bip01 R UpperArm",  NX_SKEL_R_UPPERARM );
	CreateJoint( "Bip01 L Forearm",  NX_SKEL_L_FOREARM );
	CreateJoint( "Bip01 R Forearm",  NX_SKEL_R_FOREARM );
	CreateJoint( "Bip01 L Thigh",  NX_SKEL_L_THIGH );
	CreateJoint( "Bip01 R Thigh",  NX_SKEL_R_THIGH );
	CreateJoint( "Bip01 L Calf",  NX_SKEL_L_CALF );
	CreateJoint( "Bip01 R Calf",  NX_SKEL_R_CALF );
	CreateJoint( "Bip01 L Hand",  NX_SKEL_L_HAND );
	CreateJoint( "Bip01 R Hand",  NX_SKEL_R_HAND );
	CreateJoint( "Bip01 L Foot",  NX_SKEL_L_FOOT );
	CreateJoint( "Bip01 R Foot",  NX_SKEL_R_FOOT );
	CreateJoint( "Bip01 Head",  NX_SKEL_HEAD );
	CreateJoint( "Bip01",  NX_SKEL_ROOT ); //root is XN_SKEL_WAIST
}

void NxMocapUser::GetUserPosition( float &X, float &Y, float &Z )
{
	X = mUserPosition[0];
	Y = mUserPosition[1];
	Z = mUserPosition[2];
}

void NxMocapUser::SetUserPosition( float Position[3] )
{
	mUserPosition[0] = Position[0];
	mUserPosition[1] = Position[1];
	mUserPosition[2] = Position[2];
}

void NxMocapUser::SetSkeletonTracked( bool Tracked ){
	mSkeletonTracked = Tracked ;
}

bool NxMocapUser::IsSkeletonTracked(){
	return mSkeletonTracked;
}

void NxMocapUser::SetValid( bool Valid ){
	mValid = Valid;

	if(!Valid){
		std::vector<NxMocapJoint> JointList;
		GetSkeletonJoints( JointList );
		for( int i = 0 ; i < JointList.size(); i++ ){
			JointList[i].mPosition[0] = 0.0f;
			JointList[i].mPosition[1] = 0.0f;
			JointList[i].mPosition[2] = 0.0f;
		}
	}
}

bool NxMocapUser::IsValid()
{
	return mValid;
}


/*
void Reset()
{
	xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap();
	m_UserGenerator.GetSkeletonCap().Reset( m_candidateID );
	pUserSkel.StopTracking(m_candidateID);
	m_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", m_candidateID);
}
*/


}