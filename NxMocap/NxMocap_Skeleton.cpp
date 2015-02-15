#include "NxMocapPreCompiled.h"

namespace NxMocap_Namespace {

NxMocapSkeleton::NxMocapSkeleton( unsigned int UserId ) : mUserId( UserId ){

}

NxMocapSkeleton::~NxMocapSkeleton(){

}

const unsigned int NxMocapSkeleton::GetUserId(){
	return mUserId;
}

bool NxMocapSkeleton::GetSkeletonJoint( NxSkeletonJoint Type, NxMocapJoint& Joint) const{

	for( int i = 0; i < mJointList.size(); i++){
		if( Type == mJointList[i]->mType ){ memcpy( &Joint, mJointList[i], sizeof( NxMocapJoint ) ); }
	}
	return true;
}

bool NxMocapSkeleton::GetSkeletonJoints( std::vector<NxMocapJoint> & JointList ) const{
	for( int i = 0; i < mJointList.size(); i++ ){
		JointList.push_back( *mJointList[i] );
	}
	return true;
}

void NxMocapSkeleton::CreateJoint( const std::string& JointName, NxSkeletonJoint Type ){
	NxMocapJoint * Joint = new NxMocapJoint();
	Joint->mName = JointName;
	Joint->mType = Type;
	if( JointName.compare("Bip01") == 0 ) Joint->mIsRoot = true;
	mJointList.push_back(Joint);
}

bool NxMocapSkeleton::IsRoot( NxMocapJoint Joint ){
	return Joint.mIsRoot;
}

unsigned int NxMocapSkeleton::GetNumJoints(){
	return mJointList.size();
}

void NxMocapSkeleton::SetJointPosition( NxSkeletonJoint Type, float Position[3], float Confidence ){

	for( int i = 0; i < mJointList.size(); i++){
		if( Type == mJointList[i]->mType ){
			mJointList[i]->mPosition[0] = Position[0];
			mJointList[i]->mPosition[1] = Position[1];
			mJointList[i]->mPosition[2] = Position[2];
			mJointList[i]->mConfidence = Confidence;
		}
	}
}

void NxMocapSkeleton::SetJointOrientation( NxSkeletonJoint Type, float Orientation[4] ){

	for( int i = 0; i < mJointList.size(); i++){
		if( Type == mJointList[i]->mType ){
			mJointList[i]->mOrientation[0] = Orientation[0];
			mJointList[i]->mOrientation[1] = Orientation[1];
			mJointList[i]->mOrientation[2] = Orientation[2];
			mJointList[i]->mOrientation[3] = Orientation[3];
		}
	}
}

	





}

