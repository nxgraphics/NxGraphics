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
#ifndef __NXNODE_H__
#define __NXNODE_H__

#include "NxPrerequisites.h"
#include "NxVector3.h"

namespace Nx {

typedef map<std::string, NxCamera* >::type NxCameraMap;
typedef NxMapIterator<NxCameraMap> NxCameraMapIterator;

typedef map<std::string, NxPhysicsActor*>::type NxActorList;
typedef NxMapIterator<NxActorList> NxActorListIterator;

typedef map<std::string, NxEntity*>::type NxEntityList;
typedef NxMapIterator<NxEntityList> NxEntityListIterator;

typedef map<std::string, NxScene3DObjectLight*>::type NxLightList;
typedef NxMapIterator<NxLightList> NxLightListIterator;

typedef map<std::string, NxCharacterController* >::type NxControllerMap;
typedef NxMapIterator<NxControllerMap> NxControllerMapIterator;

typedef map<std::string, NxScene3DObjectSound* >::type NxSoundList;
typedef NxMapIterator<NxSoundList> NxSoundMapIterator;



class NxGraphics_Export NxNode
{
public :
	//! NxNode Class Constructor.
	NxNode( Nx3D_Scene * NxManager, const std::string & NodeName );
	//! NxNode Class Destructor.
	~NxNode();
	//! Create NxEntity With Provided Ogre Mesh;
	NxEntity * CreateNxEntity( const std::string & MeshName );
	//! Create NxBox with provided name;
	NxEntity * CreateNxBox( const std::string & BoxName, const Nx::Vector3 & Dimensions, const Nx::Vector3 & NumSegments = Nx::Vector3( 5,5,5 ) );
	//! Create NxSphere with provided name;
	NxEntity * CreateNxSphere( const std::string & SphereName, float Radius, unsigned int numRings = 15, unsigned int numSegments = 16, float uTile = 5., float vTile = 5. );
	//! Create NxCapsule with provided name;
	NxEntity * CreateNxCapsule( const std::string & CapsuleName, const Nx::Vector2 & Dimensions );
	//! Create NxCapsule with provided name;
	NxEntity * CreateNxPlane( const std::string & PlaneName, const Nx::Vector2 & Dimensions, const Nx::Vector2 & NumSegments = Nx::Vector2( 5,5 ) );
	//! Create NxCylinder with provided name;
	NxEntity * CreateNxCylinder( const std::string & CylinderName, float Radius, float Height );
	//! Create NxCone with provided name;
	NxEntity * CreateNxCone( const std::string & ConeName, float Radius, float Height );


	NxRectangle2D * CreateNxRectangle2D( const std::string & RectangleName, bool includeTextureCoordinates=false );


	//! Create Particles;
	Nx3DObjectPhysicFluid * CreateNxParticles( const std::string & ParticlesName, unsigned long NumParticles, bool UseVertexColor );
	//! create ogre particle system
	NxParticleSystem * CreateParticleSystem( const std::string & ParticleSystemName, unsigned long NumParticles  );
	//! Create Clothe;
	Nx3DObjectPhysicClothe * CreateNxCloth( const std::string & ClotheName, const NxClotheDesc & ClotheDesc );
	//! Create a line.
	Nx3DObjectPhysicLine * CreateNxLine( const std::string & LineName, bool UseVertexColor = false );
	//! Get NxLine by Name.
	Nx3DObjectPhysicLine * GetNxLine( const std::string & Name ) const;
	//! Delete NxLine by Name.
	void DeleteNxLine( const std::string & LineName );
	//! Create a billboard text.
	Nx3DObjectText * CreateNxText( const std::string & LineName, const std::string &caption, const std::string & FontName= "BlueHighway-8", float charHeight = 1.0, const NxColourValue &color = NxColourValue::White );
	//!delete object
	void DeleteObject( const std::string & Name );
	//! Create a NxCharacter
	NxScene3DObjectCharacter * CreateNxCharacter( const std::string & CharacterMesh );
	//! Get Attached Object By Name.
	NxEntity * GetNxEntity( const std::string& MeshName ) const;
	//! Get Attached Objects.
	NxEntityListIterator GetNxEntityIterator();
	//! Create NxActor.
	NxPhysicsActor * CreateNxActor( const std::string & ActorName, const PhysicGroup Property );
	//! Get NxActor iterator;
	NxActorListIterator GetNxActorIterator();
	//! Get NxActor By Name.
	NxPhysicsActor * GetNxActor( const std::string & ActorName ) const;
	//! Create NxLight
	NxScene3DObjectLight * CreateNxLight( const std::string & LightName, const NxLightType Type );
	//! Get NxLight Iterator.
	NxLightListIterator GetNxLightIterator();
	//! Get NxLight By Name.
	NxScene3DObjectLight * GetNxLight( const std::string & LightName ) const;
	//! Create NxCamera by Name.
	//NxCamera * CreateNxCamera( const std::string & CameraName );////cant create here , should be attached to node
	//! Get Camera By Name.
	//NxCamera * GetNxCamera( const std::string & CameraName ) const;//cant create here , should be attached to node
	//! Get NxCamera Objects Iterator.
	//NxCameraMapIterator GetNxCameraIterator();
	//! Create NxController by Name.
	NxCharacterController * CreateNxController( const std::string & ControllerName );
	//! Get NxController By Name.
	NxCharacterController * GetNxController( const std::string & ControllerName ) const;
	//! Get NxController Objects Iterator.
	NxControllerMapIterator GetNxControllerIterator();
	//! Create NxSound by Name.
	NxScene3DObjectSound * CreateNxSound( const std::string & SoundName, const std::string & SoundFile );
	//! Get NxSound By Name.
	NxScene3DObjectSound * GetNxSound( const std::string & SoundName ) const;
	//! Get NxSound Objects Iterator.
	NxSoundMapIterator GetNxSoundIterator();
	//! set inherit scale.
	void SetInheritScale( bool inherit );
	//! set inherit orientation.
	void SetInheritOrientation( bool inherit );
	//! detach from parent.
	void DetachFromParent();
	//! add child.
	void AddChild( NxNode * obj );

	void SetFixedYawAxis( bool useFixed, const Nx::Vector3& fixedAxis = Nx::Vector3::UNIT_Y );

	void Yaw(const Nx::Radian& angle, NxTransformSpace relativeTo = NxLocal);

	void SetDirection(const Nx::Vector3& vec, NxTransformSpace relativeTo = NxLocal, const Nx::Vector3& localDirectionVector = Nx::Vector3::NEGATIVE_UNIT_Z);
 	
	void LookAt( const Nx::Vector3& targetPoint, NxTransformSpace relativeTo, const Nx::Vector3& localDirectionVector = Nx::Vector3::NEGATIVE_UNIT_Z);
 
	void SetAutoTracking(bool enabled, NxNode* const target = 0, const Nx::Vector3& localDirectionVector = Nx::Vector3::NEGATIVE_UNIT_Z, const Nx::Vector3& offset = Nx::Vector3::ZERO );

	//! translate node
	void Translate( const Nx::Vector3 & Position,  NxTransformSpace Space );
	//! rotate node
	void Rotate( const Nx::Quaternion & Orientation, NxTransformSpace Space );
	//! Set Position.
	void SetPosition( const Nx::Vector3 & Position );
	//! Get derivated Position
	const Nx::Vector3 & GetDerivatedPosition() const;
	//! GetPosition.
	const Nx::Vector3 & GetPosition() const;
	//! Set Orientation.
	void SetOrientation( const Nx::Quaternion & Orientation );
	//! Get derivated orientation
	const Nx::Quaternion & GetDerivatedOrientation() const;
	//! Get Orientation.
	const Nx::Quaternion & GetOrientation() const;
	//! Set Scale.
	void SetScale( const Nx::Vector3 & Scale );
	//! Get derivated Scale
	const Nx::Vector3 & GetDerivatedScale() const;
	//! Get Scale.
	const Nx::Vector3 & GetScale() const;
	//! View debug content.
	void SetDebugEnabled( bool Debug );
	//! Object lifetime;
	void SetLifeTime( float milliseconds );
	//! Get object lifetime.
	float GetLifetime();
	//! is object valid ?
	bool IsValid();
	//! Enable / Disable SceneNode Animation by Name.
	void SetNodeAnimation( const std::string& AnimationName, bool Looped );
	//! Update
	void Update( const float time );
	//! Get Name.
	std::string GetName();
	//! Get Scene Reference
	Nx3D_Scene & GetNxScene();
	//! Set NxActor to node.
	bool SetNxActor( const std::string & ActorName );
	//! Get SceneNode.
	Ogre::SceneNode * GetNxSceneNode();
	//! OnMouseMoved
	void OnMouseMoved( const OIS::MouseEvent &e );
	//! OnMousePressed
	void OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id );
	//! OnMouseReleased
	void OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id );
	//! OnKeyPressed
	void OnKeyPressed( const NxKeyCode &e );
	//! OnKeyReleased
	void OnKeyReleased( const NxKeyCode &e );

private :
	//! NxActor List.
	NxActorList mNxActors;
	//! NxLight List.
	NxLightList mNxLights;
	//! NxCameras List.
	NxCameraMap mNxCameras;
	//! NxEntity List.
	NxEntityList mNxEntities;
	//! NxCamera List
	NxControllerMap mNxControllers;
	//! NxSound List.
	NxSoundList mNxSounds;
	//! Base Scene Node.
	Ogre::SceneNode * mSceneNode;
	//! Physic Actor, if any.
	NxPhysicsActor * mActor;
	//! object lifetime.
	float mLifetime;
	//! Current time.
	float mCurrentTime;
	//! object valid.
	bool mValid;
	//! NxScene
	Nx3D_Scene * mScene;
};

}

#endif