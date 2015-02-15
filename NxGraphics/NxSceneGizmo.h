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
#ifndef __NXGRAPHICS3DGIZMO_H__
#define __NXGRAPHICS3DGIZMO_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxSceneTools
{
public:
	NxSceneTools( Nx3D_Scene * Scene, Ogre::Camera * Camera );
	~NxSceneTools();
	void CreateGizmo();
	void UpdateGizmo();
	void DeleteGizmo();
	void SetGizmoMode( NXTOOLS_MODE Mode );
	const NXTOOLS_MODE GetGizmoMode() const;
	void SetActiveAxis( const std::string & AxisMeshName );
	const NXTOOL_AXIS GetActiveAxis() const;
	bool PickGizmos( Ogre::Ray &ray, int &Axis );
	Ogre::Vector3 GetGizmoIntersect( Ogre::Ray &pickRay, Ogre::Plane &planetouse, int TranslationAxis, Ogre::Vector3 &vLastPosition );
	Ogre::Plane FindGizmoTranslationPlane( Ogre::Ray &pickRay, int TranslationAxis );
	void SetEditionActive( bool EditActive );
	bool GetEditionActive();
	void OnKeyPressed( const OIS::KeyEvent &e );
	void OnKeyReleased( const OIS::KeyEvent &e );
	void OnKeyMouseMoved( const OIS::MouseEvent &e );
	void OnKeyMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id );
	void OnKeyMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id );
private :
	bool mEditionActive;
	NXTOOLS_MODE mCurrentGizmoMode;
	NXTOOL_AXIS mCurrentAxis;
	Ogre::Entity * mEntitySelectionOld;
	Ogre::Entity * mEntitySelection;
	float mGizmoScale;                             
	Ogre::SceneNode *mGizmoNode;                                  
	Ogre::SceneNode *mGizmoX;                                     
	Ogre::SceneNode *mGizmoY;                                     
	Ogre::SceneNode *mGizmoZ;                               
	Ogre::Entity *mGizmoEntities[6];                                  
	NXTOOLS_MODE mOldGizmoMode;                              
	NXTOOLS_MODE mOldGizmoAxis;                              
	Nx3D_Scene * mScene;
	Ogre::Camera * mCamera;
};

} // namespace

#endif