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
#ifndef __NXGRAPHICSLAYER_H__
#define __NXGRAPHICSLAYER_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxLayer
{
public:
	//! NxLayer constructor.
	NxLayer();
	//! NxLayer constructor.
	NxLayer( int Width, int Height );
	//! NxLayer destructor
	virtual ~NxLayer();
	//! set position
	void SetPosition( Nx::Vector3 Position );
	//! get position
	Nx::Vector2 GetPosition();
	//! set scale
	void SetScale( Nx::Vector2 Scale );
	//! get scale
	Nx::Vector2 GetScale();
	//! set orientation
	void SetRotation( Nx::Quaternion Rotation, NxTransformSpace TrSpace );
	//! get orientation
	Nx::Vector3 GetRotation();
	//! set visible
	void SetVisible( bool Visible );
	//! Set Surface Type
	void Set3DObjectType(  Nx3DSceneObjectTypes Type );
	//! set object material name;
	void SetMaterialName( const std::string & MaterialName );
	//! set z order
	void SetRenderZorder( int RenderingOrder );

protected:
	//Ogre::PatchMeshPtr NxPatch;
	//! Layer Entity.
	Ogre::Entity * NxEntity ; 
	//! Layer Scene Node.
	Ogre::SceneNode * NxSceneNode;
	//! Pivot Visible Object;
	Ogre::Entity * mPivotSphere;
	//! Pivot Point.
	Ogre::SceneNode * NxPivot;
	//! layer Position.
	Nx::Vector3 Position;
	//! layer Rotation.
	Nx::Vector3 Rotation;
	//! layer Scale.
	Nx::Vector3 Scale;	
	bool mIsLocked ;
	//! Layer Shapes Container.
	std::vector< Nx3DSceneObject * > Nx3DObjectLayerList ;

	int mWidth;
	int mHeight; 
};

class NxGraphics_Export NxLayerVideo : public NxLayer
{
public:
	NxLayerVideo( int Width, int Height );
	~NxLayerVideo();
	NxTextureVideo * GetTexture();
};

class NxGraphics_Export NxLayer3D : public NxLayer
{
public:
	NxLayer3D( int Width, int Height);
	~NxLayer3D();
	NxTexture3D * GetTexture();
};

class NxGraphics_Export NxLayerCamera : public NxLayer
{
public:
	NxLayerCamera( int Width, int Height);
	~NxLayerCamera();
};

class NxGraphics_Export NxLayerWeb : public NxLayer
{
public:
	NxLayerWeb(int Width, int Height );
	~NxLayerWeb();
};

class NxGraphics_Export NxLayerImage : public NxLayer
{
public:
	NxLayerImage( int Width, int Height);
	~NxLayerImage();
};

class NxGraphics_Export NxLayerStream : public NxLayer
{
public:
	NxLayerStream(int Width, int Height );
	~NxLayerStream();
};



} // namespace

#endif