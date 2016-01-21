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
#ifndef __NXENTITY_H__
#define __NXENTITY_H__

#include "NxPrerequisites.h"

namespace Nx {

//!  A class to manage basic entities in the 3d scene
class NxGraphics_Export NxEntity : public CountedObj<NxEntity>
{
public :
	//! NxScene3DObjectEntity Class Constructor.
	NxEntity();
	//! NxScene3DObjectEntity Class Constructor.
	NxEntity( NxNode * parentNode, const std::string & MeshName );
	//! Destructor
	virtual ~NxEntity();
	//! set NxEntity Lod
	bool SetLodDetails( std::vector<float> mLodValues, float reductionvalue );
	// set render group
	virtual void SetRenderQueueGroup( unsigned char renderGroup  );
	//! Update From Timestep.
	virtual void Update( float time );
	//! Get Entity Type, see NxType;
	const NxType GetNxType();
	//! Get Name.
	const std::string & GetName();
	//! set initial state.
	void SetInitialState();
	//set position
	virtual void SetPosition( const Nx::Vector3 & Position );
	//set orientation
	virtual void SetOrientation( const Nx::Quaternion & Orientation ); 
	//! Set Scale
	virtual void SetScale( const Nx::Vector3 & Scale );


	virtual const Nx::Quaternion & GetDerivatedOrientation() const;
	virtual const Nx::Quaternion & GetOrientation() const;
	virtual  const Nx::Vector3 & GetPosition() const;
	virtual  const Nx::Vector3 & GetDerivatedPosition() const;
	virtual  const Nx::Vector3 & GetDerivatedScale() const;
	virtual  const Nx::Vector3 & GetScale() const;
	void Translate( const Nx::Vector3 & Position, NxTransformSpace Space );
	void Rotate( const Nx::Quaternion & Orientation, NxTransformSpace Space );
	//! inherit orientation
	void SetInheritOrientation( bool inherit );
	//! inherit scale
	void SetInheritScale( bool inherit );
	//! set pivot transform
	void SetPivotTransform( const Nx::Vector3 & Position, const Nx::Quaternion & Rotation = Nx::Quaternion::ZERO,  const Nx::Vector3 & Scale = Nx::Vector3::ZERO );
	//! set autotracking
	void SetAutoTracking( bool enabled, NxNode* const target = 0, const Nx::Vector3& localDirectionVector = Nx::Vector3::NEGATIVE_UNIT_Z, const Nx::Vector3& offset = Nx::Vector3::ZERO );
	//! get Bounds Size in Local Coordinates.
	const Ogre::Vector3 & GetBoundsSize() const;
	//! set entity bounds infinite visibility;
	void SetBoundsInfinite();
	//! Get Entity Triangle Count;
	unsigned int GetTriangleCount();
	//! Set Entity Material Name.
	virtual void SetMaterialName( const std::string & MaterialName );
	//! Get Entity material Name.
	virtual const std::string & GetMaterialName();
	//! Set NxActor to NxEntity.
	bool SetNxActor( const std::string & ActorName );
	//! Set NxSound to NxEntity.
	bool SetNxSound( const std::string & SoundName );
	//! Set NxEntity To Cast Shadows.
	void SetCastShadows( bool CastShadows );
	//! Build tangent Vectors.
	void BuildTangentVectors();
	//! View debug content.
	virtual void SetDisplayDebug( bool Debug );
	//! set the entity visible
	virtual void SetVisible( bool Visible );
	//! Set Object Selected.
	void SetSelected( bool Selected );
	//!Enable Vertex Animation
	void SetVertexAnimation( const std::string & VertexAnimationName, bool Looped );
	//! Get Vertex Names
	void GetVertexAnimationNames( std::vector< std::string > & VertexAnimationNames );
	//! Get Ogre Entity.
	Ogre::Entity * GetEntity();
	//! Get Ogre Scene Node.
	Ogre::SceneNode * GetSceneNode();
	//! Get Parent Scene Node;
	Ogre::SceneNode * GetParentSceneNode();

	void AddChild( NxEntity * obj );


	
  
	//! OnMouseMoved
	virtual void OnMouseMoved( const OIS::MouseEvent &e );
	//! OnMousePressed
	virtual void OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id );
	//! OnMouseReleased
	virtual void OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id );
	//! OnKeyPressed
	virtual void OnKeyPressed( const NxKeyCode &e );
	//! OnKeyReleased
	virtual void OnKeyReleased( const NxKeyCode &e );
protected :
	//! Base Type.
	NxType mType;
	//! Parent scene Node
	NxNode * mNxNodeParent;
	//! Physic Actor, if any.
	NxPhysicsActor * mActor;
	//! NxSound, if any.
	NxScene3DObjectSound * mSound;
	//! Current Object Scene.
	Nx3D_Scene * mScene;
	//! Ogre Entity.
	Ogre::Entity * mEntity;
	//! Mesh Animations
	//std::vector<AnimationState*> mAnimationStates;
	//! Ogre Scene Node.
	Ogre::SceneNode * mNode;
	//! is the Object Selected ?
	bool mIsSelected;
};


}

#endif