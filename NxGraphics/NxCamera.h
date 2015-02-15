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
#ifndef __NXCAMERA_H__
#define __NXCAMERA_H__

#include "NxPrerequisites.h"
/*#include "NxEntity.h"*/

namespace Nx {

class NxGraphics_Export NxCamera //: public NxScene3DObjectEntity 
{
public :
	//! NxCamera Class Constructor.
	//NxCamera( NxScene3DObject * ParentNode, const std::string & CameraName );
	NxCamera( NxContentManager * SceneMgr, const std::string & CameraName );
	//! NxCamera Class Destructor.
	~NxCamera();
	//! set camera near clip distance.
	void SetNearClipDistance( float neardist );
	//! set camera far clip distance.
	void SetFarClipDistance( float fardist );
	//! set camera position.
	void SetPosition( float x, float y, float z );
	//! Look at camera
	void LookAt( const Ogre::Vector3 & Position );
	//! Roll Camera
	void Roll( float AngleDegrees );
	//! Yaw Camera
	void Yaw( float AngleDegrees );
	//! Pitch Camera
	void Pitch( float AngleDegrees );
	//! Set Camera Aspect Ratio.
	void SetAspectRatio( float Ratio );
	//! Set polygon Mode
	void SetPolygonMode( NxPolygonMode Mode );
	//! set camera fovy
	void SetFOVy( float degrees );
	//! Get Direction;
	Ogre::Vector3 GetDirection() const;
	//! Get Right.
	Ogre::Vector3 GetRight() const;
	//! Get Ogre Camera.
	Ogre::Camera * GetCamera();
private :
	Ogre::Camera * mCamera;

};


}

#endif
