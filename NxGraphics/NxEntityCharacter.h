/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
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
#ifndef __NXENTITYCHARACTER_H__
#define __NXENTITYCHARACTER_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage characters in the 3d scene
class NxGraphics_Export NxScene3DObjectCharacter : public NxEntity 
{
public :
	//! NxScene3DObjectCharacter class Constructor.
	NxScene3DObjectCharacter( NxNode * node );
	//! NxScene3DObjectCharacter class Deconstructor.
	~NxScene3DObjectCharacter();
	//! Create Character with provided mesh and skeleton.
	void CreateCharacter( const std::string & CharacterMesh );
	//! Set Characters Position.
	void SetPosition( const Nx::Vector3 & Position );
	//! Get Characters Position.
	const Nx::Vector3 & GetPosition() const;
	//! Set Characters Orientation.
	void SetOrientation( const Nx::Quaternion & Orientation );
	//! Get Characters Orientation.
	const Nx::Quaternion & GetOrientation() const;
	//! Set Characters Scale
	void SetScale( const Nx::Vector3 & Scale );
	//! Get Scale.
	const Nx::Vector3 & GetScale() const;
	//! Update Animation Time.
	void Update( float time );
	//! Set Character navigation mode. see NavigationMode
	void SetNavigationMode( NavigationMode Mode);
	//! Set Character deplacement Speed, Meters / second
	void SetCharacterSpeed( float Speed );
	//! Set Character Animation.
	void SetAnimation( const std::string & AnimationName, bool Looped );
	//! get camera bundle associated with this character
	NxCharacterController * GetNxCamera();
	//! Set Characters Mocap Data.
	void SetMocapData( std::vector< NxMocapJoint *> MocapData );
	//! Set Character Bone Orientation.
	void SetBoneOrientation( NxMocapJoint Joint );
	//! Set Character Bone Position.
	void SetBonePosition( NxMocapJoint Joint );
	//! Calibrate Character from Current Pose.
	void Calibrate( bool Calibrate );
	//!kill
	void Kill( bool Kill );
	void SetRagdollVisible( bool Visible );
	void SetVisible( bool Visible );
	void SetMagnetic( bool Magnetic );
	void AddProp( NxEntity * prop, std::string BoneName, Ogre::Vector3 Offset ); 
	void AjustClothVertexToBone( Nx3DObjectPhysicCloth * Cloth, int VertexIndex, const std::string& BoneName, Ogre::Vector3 Offset );
	void AttachClothVertexToBone( Nx3DObjectPhysicCloth * Cloth, int VertexIndex, const std::string& BoneName, Ogre::Vector3 Offset );
	void SetMaterialName( const std::string & MaterialName );
	Ogre::Skeleton * GetSkeleton();
	Ogre::Entity * GetEntity();
	void UpdatePhysx();

private:

	//!mouse and keys callback
	void OnMouseMoved( const OIS::MouseEvent &e );
	void OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id );
	void OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id );
	void OnKeyPressed( const OIS::KeyEvent &e );
	void OnKeyReleased( const OIS::KeyEvent &e );

	//! Create Character ragdoll.
	void CreatePhysXRagdoll();
	//! Delete Character ragdoll.
	void DeletePhysXRagdoll();
	//! Create Character Joints.
	void CreatePhysxJoints();

	void InitializeBoneState( const std::string& Name, const Nx::Radian& Angle, const Nx::Vector3 Axis );
	void InitializeBoneState( const std::string& name, const Nx::Degree& yaw, const Nx::Degree& pitch,const Nx::Degree& roll );
	void InitializeBoneState( const std::string& name, const Nx::Quaternion& q );
	void TransformBone( const std::string& BoneName, Nx::Matrix3 RotInfo, bool flip );

	Ogre::Entity * mCharacterEntity;
	Ogre::SkeletonInstance * mCharacterSkeleton;
	Nx3D_Scene * mCharacterScene;
	float mCharacterPosOffsetX;
	float mCharacterPosOffsetY;
	float mCharacterPosOffsetZ;
	float mCharacterPosOffsetScale;

	typedef std::map< Ogre::Bone *, NxEntity * > BindingBoneActors;
	BindingBoneActors NxPropsCollection;// for props
	BindingBoneActors BonesActorsMap;//for bones ragdoll
	bool IsDead;
	std::vector< NxActor * > CharacterPhysxNodes;

	typedef struct
	{
		Nx::Quaternion QuatParent ;
		Nx::Quaternion QuatOffset ;
		Ogre::Bone * CurrentBone ;
		Nx::Quaternion CurrentOrientation;
		Nx::Vector3 CurrentPosition;
		bool IsRoot;
	} MocapBoneData ;

	typedef std::map< std::string, MocapBoneData > MasterMocapBones;
	MasterMocapBones  BoneFinal;
	typedef std::map< std::string, int > MasterMocapList;
	MasterMocapList  mocapdef;
	std::vector<std::string> BoneListNames ;
	NxCharacterController * mCharacterCamera;

};

}

#endif