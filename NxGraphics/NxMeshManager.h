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
#ifndef __NXMESHMANAGER_H__
#define __NXMESHMANAGER_H__

#include "NxPrerequisites.h"
#include <OgreMesh.h>

namespace Nx {

class NxGraphics_Export NxMeshManager : public NxSingleton<NxMeshManager>
{
public :
	//! Class Constructor .	
	NxMeshManager();
	//! Class Destructor .	
	~NxMeshManager();
	//! Class Singleton .	
	static NxMeshManager& getSingleton();
	//! Capsule Generator.
	Ogre::MeshPtr CreateCapsule( const std::string& name , float Radius, float Height);
	//! Box Generator.
	Ogre::MeshPtr CreateBox( const std::string& name, Nx::Vector3 Size, Nx::Vector3 Segments);
	//! Sphere Generator.	
	Ogre::MeshPtr CreateSphere( const std::string& name, float Radius, unsigned int numRings = 15, unsigned int numSegments = 16, float uTile = 5., float vTile = 5.);
	//! Plane Generator.
	Ogre::MeshPtr CreatePlane( const std::string& name, Nx::Vector2 Size, Nx::Vector2 Segments);
	//! Cylinder Generator.
	Ogre::MeshPtr CreateCylinder( const std::string& name , float Radius, float Height );
	//! Cone Generator.
	Ogre::MeshPtr CreateCone( const std::string& name , float Radius, float Height );
	//! Set Pivot Point Position within the mesh.
	//void SetPivotPosition( Ogre::MeshPtr mesh, const Ogre::Vector3 & Position );
	void SetPivotTransform( Ogre::MeshPtr mesh, const Nx::Vector3 & Position, const Nx::Quaternion & Rotation, const Nx::Vector3 & Scale );
};

} // namespace

#endif