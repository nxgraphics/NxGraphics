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
#ifndef __NXSCREEN_H__
#define __NXSCREEN_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxScreen
{
public : 
	//! NxScreen class Constructor;
	NxScreen( int MonitorID, bool Fullscreen, unsigned int Width, unsigned int Height );
	//! NxScreen class Destructor;
	~NxScreen();

	void SetTextureName( const std::string & TextureName );

	void SetCamera( Ogre::Camera * cam );
	//! Get Screen Title Name.
	std::string GetScreenTitleName();
	//! Get Screen Identifier Name.
	std::string GetScreenIdentifierName();
	//! Get Screen Index.
	unsigned int GetScreenIndex();
	//! get Native Window Handle.
	void * GetWindowHandle(); 
	//! Set Fullscreen.
	void SetFullscreen( bool FullScreen );
	//! Is FullScreen ?.
	bool IsFullscreen();
	//! Get X Position.
	long GetX();
	//! Get Y Position.
	long GetY();
	//! Get Width.
	long GetWidth();	
	//! Get Height.
	long GetHeight();
	//! Update Screen ViewPort.
	void Update();
	//! Set Screen Polygon View Mode.
	void SetPolygonView( NxPolygonMode Mode );
	//! Get Screen Polygon View Mode.
	NxPolygonMode GetPolygonView();
	//! Set Screen Opacity.
	void SetOpacity( float Opacity );
	//! Get Screen Opacity.
	float GetOpacity();
	//! Get Screen Device attached name.
	const std::string GetDeviceName();
	//! Get Screen Texture Unit.
	Ogre::TextureUnitState * GetTextureUnit();
	//! Get Screen ViewPort.
	Ogre::Viewport * GetViewport();
	//! Set Control point position
	void SetPointPosition( unsigned int Point, Ogre::Vector3 Position );
	//! Get Patch Mesh.
	Nx3DSceneObjectPatch & GetPatchMesh();

	long ScreenPosX ;

private :
	//! Window Handle.
	void * mWindowHandle;
	//! Screen Camera.
	Ogre::Camera * mCamera;
	//! Screen Viewport.
	Ogre::Viewport * mViewport;
	//! Screen RenderWindow.
	Ogre::RenderWindow * mRenderWindow;
	//! Screen SceneManager.
	Ogre::SceneManager * mSceneManager;
	//! PatchMesh Material.
	//Ogre::MaterialPtr mMaterial;
	Ogre::Material * mMaterial;
	//! PatchMesh Texture.
	//Ogre::TexturePtr mTexture;
	Ogre::Texture * mTexture;
	//! Material Texture Unit.
	Ogre::TextureUnitState * mTextureUnitState;
	//! Screen Entity.
	Ogre::Entity * mEntity;
	//! Screen SceneNode.
	Ogre::SceneNode * mSceneNode;
	//! Screen Monitor Info.
	NxMonitor * mMonitorInfo;
	//! Screen Opacity.
	float mOpacity;
	//! Screen Patch Mesh.
	Nx3DSceneObjectPatch * mPatch;

};

}

#endif