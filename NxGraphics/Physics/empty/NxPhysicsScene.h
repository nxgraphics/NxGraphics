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
#ifndef __NXPHYSICSSCENE_H__
#define __NXPHYSICSSCENE_H__

#include <NxPrerequisites.h>

namespace Nx {

class NxGraphics_Export NxScenePhysics   
{
public:
	//! NxScenePhysics Class Constructor.
	NxScenePhysics( Nx3D_Scene * Scene );
	//! NxScenePhysics Class Destructor.
	~NxScenePhysics();
	void CreatePhysicsEngine3D();
	void SetSceneCollisionFlags() ;
	void DeletePhysicsEngine3D();
	void EnablePhysics( bool Enable );
	void ResetPhysicsEngine3D();
	int  LoadPhysxScene( std::string pFilename, NxScene * PhysxScene, NxFileType type );
	bool UpdatePhysics( float Time );
	NxScene & GetPhysicsScene3D();
	//NxPhysicsSDK * GetPhysicsSdk3D();//should be done in physic manager
	void SetDebugPhysics3D( bool Debug );
	void DrawDebug(); 
	//! Set Gravity Force.
	void SetGravity( const Nx::Vector3 & GravityVector );
	//! get Number of Fluids in Scene.
	unsigned int GetNumFluids();
	//! set group collision flag
	void SetGroupCollisionFlag( int groupA, int groupB, bool collide );
	//! Create Vortex.
	//void CreateVortexForceField(const NxVec3& pos, NxActor* actor );
	void CreateVortexForceField(const Nx::Vector3& pos, NxActor* actor );
	//! Create d6 Joint
	NxPhysicsD6Joint * CreateD6Joint( const NxJointD6Desc & JointDesc );
	//! Create a revolute joint
	NxPhysicsRevoluteJoint * CreateRevoluteJoint( const NxJointRevoluteDesc & JointDesc );
	//! create height field
	void CreateHeightFiled( Ogre::Terrain * terr, const Ogre::Vector3 & TerrainPosition );

private :
	//! 3d scene
	Nx3D_Scene * mScene;
	//! physx scene
	NxScene * nScene;
	//! 3d physx Scene Callback
	Nx3D_SceneCallback * SceneCallback;
	//! 3D Physx Notification
	NxScene3DPhysics3DNotify  * nNotification;
	//! Enable / Disble 3D Physx
	bool nPhysxEnable;
	//! Last Physx Scene if Any
	std::string nPhysxScenePath ;
	//! Physx Draw Debug.
	NxPhysics3DDebugDraw * DebugPhysics ;

};

} // namespace

#endif