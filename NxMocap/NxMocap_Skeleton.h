/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       __  ___                         
			   / | / /_  __ /  |/  /____   _____ ____ _ ____ 
			  /  |/ /| |/_// /|_/ // __ \ / ___// __ `// __ \
			 / /|  /_>  < / /  / // /_/ // /__ / /_/ // /_/ /
			/_/ |_//_/|_|/_/  /_/ \____/ \___/ \__,_// .___/ 
													/_/      
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __NXMOCAPSKELETON_H__
#define __NXMOCAPSKELETON_H__

#include "NxMocap_Prerequisites.h"

namespace NxMocap_Namespace {

typedef enum NxSkeletonJoint
{
	NX_SKEL_HEAD			= 1,
	NX_SKEL_NECK			= 2,
	NX_SKEL_SPINE3			= 3,
	NX_SKEL_SPINE2			= 4,
	NX_SKEL_SPINE1		    = 5,
	NX_SKEL_SPINE	        = 6,
	NX_SKEL_PELVIS   		= 7,
	NX_SKEL_ROOT    		= 8,
	NX_SKEL_L_THIGH 		= 9,
	NX_SKEL_R_THIGH     	=10,
	NX_SKEL_L_CALF      	=11,
	NX_SKEL_R_CALF      	=12,
	NX_SKEL_L_FOOT  		=13,
	NX_SKEL_R_FOOT   		=14,
	NX_SKEL_L_CLAVICLE 		=15,
	NX_SKEL_R_CLAVICLE  	=16,
	NX_SKEL_L_UPPERARM		=17,
	NX_SKEL_R_UPPERARM		=18,
	NX_SKEL_L_FOREARM 		=19,
	NX_SKEL_R_FOREARM		=20,
	NX_SKEL_L_HAND		    =21,
	NX_SKEL_R_HAND		    =22,

} NxSkeletonJoint;

struct NxMocapJoint{
	NxMocapJoint(){
		mPosition[0] = mPosition[1] = mPosition[2] = 0.0f;
		mOrientation[0] = mOrientation[1] = mOrientation[2] = mOrientation[3] = 0.0f;
		mConfidence = 0.0f;
		mName = "Default Joint";
		mIsRoot = false;
	}
	float mPosition[3];
	float mOrientation[4];
	float mConfidence;
	std::string mName;
	NxSkeletonJoint mType;
	bool mIsRoot;
};

//! Skeleton definition for all avatars.
class NxMocap_Export NxMocapSkeleton
{
protected :
	//! NxMocapSkeleton Class Constructor.
	NxMocapSkeleton( unsigned int UserId );
public:
	//! NxMocapSkeleton Class Destructor.
	~NxMocapSkeleton();
	//! get user ID
	const unsigned int GetUserId();
	//! Get Skeleton Joint for Information: Tranlation, Rotation.
	inline bool GetSkeletonJoint( NxSkeletonJoint eJoint, NxMocapJoint& Joint) const;
	//! Get Full Skeleton Joint Information: Tranlation, Rotation.
	inline bool GetSkeletonJoints( std::vector<NxMocapJoint> & JointList ) const;
	//! Get Total Joint Number.
	unsigned int GetNumJoints();
	//! Is current joint the root.
	bool IsRoot( NxMocapJoint Joint );
	//! create a joint.
	void CreateJoint( const std::string& JointName, NxSkeletonJoint Type );
	//! Set Joint Position.
	void SetJointPosition( NxSkeletonJoint Type, float Position[3], float Confidence );
	//! Set joint Orientation.
	void SetJointOrientation( NxSkeletonJoint Type, float Orientation[4] );
private:
	//! Skeleton Joint List.
	std::vector<NxMocapJoint*> mJointList;
	//! User Id
	unsigned int mUserId;
};


class NxMocap_Export NxMocapUser : public NxMocapSkeleton
{
public :
	NxMocapUser( unsigned int UserId );
	~NxMocapUser();
	const unsigned int GetUserId();
	void GetUserPosition( float &X, float &Y, float &Z  );
	void SetUserPosition( float Position[3] );
	void SetSkeletonTracked( bool Tracked );
	//! User has a skeleton ?.
	bool IsSkeletonTracked();
	void SetValid( bool Valid );
	bool IsValid();


private:
	void CreateSkeleton();
	//! User has a skeleton ?.
	bool mSkeletonTracked;
	//! User position.
	float mUserPosition[3];
	bool mValid;
};


}

#endif