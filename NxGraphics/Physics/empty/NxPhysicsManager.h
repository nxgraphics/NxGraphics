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
#ifndef __NXPHYSICSMANAGER_H__
#define __NXPHYSICSMANAGER_H__

#include <NxPrerequisites.h>

class UserAllocator;
class NxPhysicsSDK;
class NxControllerManager;

namespace Nx {

class NxGraphics_Export NxScene3DPhysicsManager : public NxSingleton<NxScene3DPhysicsManager>  
{
	
public:
 
	NxScene3DPhysicsManager();
 
	~NxScene3DPhysicsManager();
 
	//! Create the 3D Physics Manager
	void CreatePhysicsManager() ;
 
	//! Deletes the 3D Physics Manager
	void DeletePhysicsManager();
 
	//! Get Abstract singleton factory class used for instancing objects in the Physics SDK.
	NxPhysicsSDK * GetPhysicsSdk();
	//! Get Skin Width.
	float GetSkinWidth();
	//!Physx Hardware acceleration Enabled ?
	bool HasPhysxHardwareAcceleration();
 
	//! Get Physx Memory allocator.
	UserAllocator * GetPhysics3DMemoryAllocator();
	//! Get Physx Controller Manager.
	NxControllerManager * GetPhysicsControllerManager();
	//! Creates Physx Scene with default plane set to level 0.
    /*!
      \return Physx Scene
      \sa DeletePhysicsWorld3D
    */
	NxScene * CreatePhysicsWorld3D();
	//! Deletes a Physx Scene.
    /*!
      \param PhysxScene the Physx scene to delete.
	  \return true if successfull.
    */
	bool DeletePhysicsWorld3D( NxScene * Physics3D );

	//! Connect to Physx Visual Remote Debugger.
    /*!
      \param IpAddress the ip address to connect
      \return true if successful
    */
	bool DebugPhysics3D( const std::string & TargetIpAddress, int TargetPort = 5425 );

	bool UpdateControllers();

	bool  UpdatePhysics( unsigned long elapsedMS );

	Nx3D_Scene * GetNx3DSceneFromPhysxScene( NxScene * Scene );


private:
	//! 3D Physics Scene Container.
	std::list< NxScene * > PhysicsWorld3DList;
	//! Physx Hardware Support.
	bool mHardwareSimulation;
	//! Physx Sdk.
	NxPhysicsSDK * physicsSDK;
	//! 3D Physx memory allocator.
	UserAllocator * PhysicsAllocator;
	//! Physx Controller Manager.
	NxControllerManager * nControllerManager;


};


}

#endif