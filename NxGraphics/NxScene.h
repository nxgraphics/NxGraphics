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
#ifndef __NXSCENE_H__
#define __NXSCENE_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export Nx3DSceneListener
{
public :
	Nx3DSceneListener(){}
	virtual ~Nx3DSceneListener(){}
	virtual void OnCollide( NxEntity * Ent1, NxEntity * Ent2, NxEntityEvents Event ){}
};
 
typedef map<std::string, NxNode* >::type NxNodeList;
typedef NxMapIterator<NxNodeList> NxNodeMapIterator;
typedef NxConstMapIterator<NxNodeList> NxConstNodeMapIterator;

//!  A class to manage a 3D scene
/*!
	this is the base class for all 3d scenes and shouldn't be used directly.
*/
class NxGraphics_Export Nx3D_Scene : public CountedObj<Nx3D_Scene>, public NxFrameListener, public NxInputListener
{
protected:

	//! Nx3D_Scene class Constructor
	Nx3D_Scene( const Nx3DSceneDesc & SceneDesc );
public:
	//!  Nx3D_Scene class Destructor
	virtual ~Nx3D_Scene();
	//! Creates the 3D scene.
	virtual void CreateScene() = 0;
	//! Deletes the 3D scene.
	virtual void DeleteScene() = 0;
	//! Get the 3D scene type.
	const NxGraphics_3D_Scene_Types GetSceneType();
	//! Create a Scene Node
	NxNode * CreateNxNode( const std::string & Name );
	//! Get Nodes Iterator.
	NxNodeMapIterator GetNxNodeIterator();
	//! Cont Node Iterator.
	NxConstNodeMapIterator GetNxNodeConstIterator();
	//! get NxNode By Name;
	NxNode * GetNxNode( const std::string & NodeName ) const;
	//! graph has this nxnode ?
	bool HasNxNode(const std::string & NodeName ) const;
	//! Get NxActor by Name.
	NxPhysicsActor * GetNxActor( const std::string & ActorName );
	//! Get Root Scene Node.
	Ogre::SceneNode * GetRootSceneNode();
	//! remove all SceneNodes and Attached objects.
	void DeleteNxNodes();
	//! Delete node by name.
	void DeleteNxNode( const std::string & NodeName );
	//! Set resource group name.
	void SetResourceGroupName( const std::string & ResourceName );
	//! Get resource group name.
	const std::string & GetResourceGroupName() const ;
	//! Add a resource location
	void AddResourceLocation( const std::string & ResourceLocation, const std::string & ResourceGroup = "General", bool Recursive = false );
	//! Get the current Scene Manager.
	Ogre::SceneManager * GetNxSceneManager();
	//! Get the current Scene Viewport.
	Ogre::Viewport * GetNxViewport();
 
	//! Get the current Scene RT.
	Ogre::RenderTexture * GetNxRenderTexture();

	//! Set Ambient Light.
	void SetAmbientLight( const NxColourValue Colour );
	//! Get Ambient Light.
	const NxColourValue GetAmbientLight();
	//! Set Fog intensity.
	void SetFog( NxFogMode Mode, NxColourValue Color, float Intensity );
	//! Sets scene background color
    /*!
	  \param BackgroundColor the background color to set
      \sa GetNxSceneBackgroundColor
    */
	void SetViewportColour( const NxColourValue BackgroundColor );

	void CreateViewportBackground( const std::string & materialname );
	//! Gets the scene background color.
	const NxColourValue GetViewportColour() const;

	//! Sets scene background color.
	void SetViewportColours( const NxColourValue &topLeft, const NxColourValue &bottomLeft, const NxColourValue &topRight, const NxColourValue &bottomRight);
	//! Set Viewport Background to clear Off / On
	void SetAccumulationBuffer( bool Accumulation );
	//! Creates a skybok for the current scene manager
    /*!
	  \param SkyBoxMaterialName the skybox material name to set
	  \param TextureName the texture name to set
    */
	void SetSkyBox( bool Enable, const std::string & SkyBoxMaterialName );
	//! updates scene Node animations.
    /*!
	  \param Time the FrameStarted time.
    */
	void UpdateNodeAnimations( float Time );
	//! Get Physics System
	NxScenePhysics * GetPhysicScene();
	//! Update Physic System
	bool UpdatePhysics( float Time );
	//! Get list of available characters
	void GetNxCharacterList( std::vector< std::string > & List );
	//! add a nxcharacter to the scene
	NxScene3DObjectCharacter * CreateNxCharacter( const std::string & CharacterName );
	//! Delete Character
	void DeleteNxCharacter( NxScene3DObjectCharacter * Character );
	//! Set Current View To character
	void SetCharacterView( NxScene3DObjectCharacter * Character, NavigationMode ViewType = ModeWalk );
	//! Set Mouse active / inactive for 3D scene
	void SetMouseActive( bool Active );
	//! Get mouse status for 3D scene.
	bool GetMouseActive();
	//! Set Keyboard active / inactive for 3D scene
	void SetKeyboardActive( bool Active );
	//! Get keyboard status for 3D scene.
	bool GetKeyboardActive();
 
	//! Animation
	std::vector< Ogre::AnimationState * > NodeAnimationList;// node animation

	//listeners
	void AddListener( Nx3DSceneListener * listener );
	void RemoveListener( Nx3DSceneListener * listener );
	std::list< Nx3DSceneListener * > ListenersCallbackList;

	NxScene3DObjectTerrain * CreateTerrain( const NxTerrainDesc & TerrainDesc );
	NxSceneTools * GetEditorTools();

	//! NxInput Virtuals
	void OnMouseMoved( const OIS::MouseEvent &e );
	void OnMouseButtonPressed( const OIS::MouseEvent &e, NxMouseButtonID id );
	void OnMouseButtonReleased( const OIS::MouseEvent &e, NxMouseButtonID id );
	void OnKeyPressed( const NxKeyCode &e );
	void OnKeyReleased( const NxKeyCode &e );
	void OnJoystickButtonPressed( const OIS::JoyStickEvent &arg, int button );
	void OnJoystickButtonReleased( const OIS::JoyStickEvent &arg, int button );
	void OnJoystickAxisMoved( const OIS::JoyStickEvent &arg, int axis );

protected :
	//! Scene Physics manager.
	NxScenePhysics * mScenePhysics;
	//! Scene Tools
	NxSceneTools * mSceneTools;
	bool frameStarted( const NxFrameEvent& evt );
	bool frameRenderingQueued( const NxFrameEvent& evt );
	bool frameEnded( const NxFrameEvent& evt );
	//! Current 3D scene RT.
	Ogre::RenderTexture * mRtt;
	//! attached render window
	NxRenderWindow * mWindow;
	//! Current scene manager.
	Ogre::SceneManager * mSceneManager;
	//! Current Viewport.
	Ogre::Viewport * mViewport;
	//! 3D scene type.
	NxGraphics_3D_Scene_Types mLayerType;
	//! scene resource group name.
	std::string ResourceGroupName;
	//! Set Mouse active 
	bool MouseActive;
	//! Set Keyboard active 
	bool KeyboardActive ; 
	//! Terrain List.
	std::vector< NxScene3DObjectTerrain * > mTerrains;
	//! update sound Listener orientation and position.
	void UpdateSoundListener();
private :
	//! NxNode List
	NxNodeList mNxNodes;
};

class Nx3DSceneDefault : public Nx3D_Scene
{
public :
	Nx3DSceneDefault( const Nx3DSceneDesc & SceneDesc );
	~Nx3DSceneDefault();
	void CreateScene();
	void DeleteScene();
};

class OgreMaxSceneCallback;
//!  A class to Manage OgreMax .scene format exporter
class Nx3DSceneOgreMax: public Nx3D_Scene
{
public :
	Nx3DSceneOgreMax( const Nx3DSceneDesc & SceneDesc );
	~Nx3DSceneOgreMax() ;
	void CreateScene();
	void DeleteScene();
	//! Gets the scene project file path
	std::string GetNxSceneFileFullPath();
private:
	void CreateActors();
	void ParseUserData();
	std::string mSceneFile;
	OgreMax::OgreMaxScene * mSceneOgreMax ;
	OgreMaxSceneCallback * mSceneOgreMaxCallback;
};

class NxGraphics_Export Nx3DScenePtr : public shared_ptr<Nx3D_Scene> 
{
public:
	Nx3DScenePtr() :  shared_ptr<Nx3D_Scene>(){ }
    explicit Nx3DScenePtr( Nx3D_Scene * rep ) :  shared_ptr<Nx3D_Scene>(rep){ }
    Nx3DScenePtr( const Nx3DScenePtr& r ) :  shared_ptr<Nx3D_Scene>(r){ } 
};

} //NxGraphics_Namespace 
#endif