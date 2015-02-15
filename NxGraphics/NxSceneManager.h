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
#ifndef __NXSCENEMANAGER_H__
#define __NXSCENEMANAGER_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export Nx3DSceneDesc
{
public:
	//! class Constructor.
	inline Nx3DSceneDesc() : mType(Nx3D_Scene_Default),mTarget(0), mScenePath(""){ }
	//! Render window
	NxRenderWindow * mRenderWindow;
	//! Type
	NxGraphics_3D_Scene_Types mType;
	//! Render target if any
	Ogre::RenderTexture * mTarget;
	//! Scene path if any
	std::string mScenePath;
};
 
class NxGraphics_Export NxScene3DManager : public NxSingleton<NxScene3DManager>  
{
	
public:
    //! 3D Scene Manager Constructor, Initializes the Singleton
	NxScene3DManager();
	//! Creates 3D scene from provided project file.
    /*!
      \param FilePath the path of scene to load.
      \param RTarget the Ogre::Rendertexture pointer to render the scene.
	  \param Type the type of 3D projet scene to load.
      \return Shared Pointer
      \sa DeleteNx3DScene
    */

	~NxScene3DManager();

	//Nx3DScenePtr CreateNx3DScene( const Nx3DSceneDesc & SceneDesc );
	Nx3D_Scene * CreateNx3DScene( const Nx3DSceneDesc & SceneDesc );
	//! Deletes the provided 3D scene.
    /*!
      \param Scene the scene pointer to delete.
      \sa CreateNx3DScene
    */
	bool DeleteNx3DScene( Nx3D_Scene * Scene );
	//! Get raw 3D scene pointer from index.
    /*!
      \param index the 3D scene index to get.
    */	
	Nx3D_Scene * GetNx3DScene( int index ); 
	//! Get number of 3D scenes owned by the manager.
	size_t GetNumNxScenes();
	//! Get 3DSceneManager Singleton
	static NxScene3DManager& getSingleton(void);
	//! Updates all Scene Physics.
	//bool UpdatePhysics( unsigned long elapsedMS );
	//! Update Camera Controllers
	//bool UpdateControllers();
	//! Get Nx3DScene From Scene Manager.
	Nx3D_Scene * GetNx3DSceneFromOgreSceneManager( Ogre::SceneManager * SceneMgr );
	//! Get Nx3DScene From Physx Scene.
	//Nx3D_Scene * GetNx3DSceneFromPhysxScene( NxScene * Scene );

private :
	//! 3D Scene Container.
	std::list< Nx3D_Scene * > Scene3DList;
};

}//namespace

#endif