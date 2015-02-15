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
#ifndef __NXCOREENGINE_H__
#define __NXCOREENGINE_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxEngineListener
{
public :
	NxEngineListener(){}
	virtual ~NxEngineListener(){}
	virtual void OnWindowMoved( unsigned int PosX, unsigned int PosY ) = 0;
	virtual void OnWindowResized( unsigned int Width , unsigned int Height ) = 0;
	virtual bool OnWindowClosing() = 0;
	virtual void OnWindowClosed() = 0;
	virtual void OnWindowFocusChange() = 0;
};

class NxGraphics_Export NxEngine : public NxSingleton<NxEngine>//, public Ogre::RenderSystem::Listener// public Ogre::WindowEventListener,
{
public :
	//! NxEngine class Constructor.
	NxEngine();
	//! NxEngine class Destructor.
	~NxEngine();
	//! NxEngine Singleton.
	static NxEngine& getSingleton();
	//! NxEngine Singleton.
	static NxEngine* getSingletonPtr();
	//! Update One Frame.
	bool Update();
	//! Is Device Lost
	bool IsDeviceLost();
	//! Initializes the Engine with specific parameters.
	void InitializeEngine( const NxGraphicsDesc& engineDesc );
	//! Post initialize callback
	void PostInitialized();
	//! ShutDown the Engine.
	void ShutDownEngine();
	//! Start main loop.
	void StartRendering();
	//! Stop main loop.
	void StopRendering();
	//! Is Running.
	bool IsRendering();
	//! Pause the Engine or resume.
	void PauseEngine( bool Pause );
	//! Add game state.
	void AddState( NxState * State );
	//! Set game state.
	void SetState( NxState * State );
	//! Add remove game state.
	void RemoveState( NxState * State );
	//! Create a render window
	NxRenderWindow * CreateRenderWindow( const NxRenderWindowDesc & WindowDesc );
	//! Create a scene Manager.
	NxContentManager * CreateContentManager( NxSceneType Type, const std::string & name );
	//! Get Media Path.
	const std::string & GetNxMediaPath();
	//! Set Polygon View Mode.
	//void SetPolygonViewMode( NxPolygonMode Mode );
	//! Get Ogre RenderWindow.
	NxRenderWindow * GetNxWindow();
	//! Get render window
	//NxRenderWindow * GetWindow();
	//! Get Ogre Main Camera.
	NxCamera * GetNxCamera();
	//! Get Ogre Main SceneManager.
	NxContentManager * GetContentManager();
	//! Get Ogre Main Viewport.
	NxViewport * GetNxViewport(); 
	//! Get Ogre Main Root.
	//Ogre::Root * GetNxRoot();

	void AddFrameListener( NxFrameListener * newListener ) ;
	void RemoveFrameListener( NxFrameListener * oldListener );

	unsigned int GetFrameListenerSize();

	void LogMessage( const std::string & msg );


	//bool _fireFrameStarted(Ogre::FrameEvent& evt);
	//bool _fireFrameRenderingQueued(Ogre::FrameEvent& evt);
	//bool _fireFrameEnded(Ogre::FrameEvent& evt);

	bool IsInitialized();


private:
	bool RenderFrame( unsigned long elapsedMS );
	//void eventOccurred( const Ogre::String& eventName, const Ogre::NameValuePairList * parameters);
	void SetupRenderer( unsigned long width, unsigned long height, NxRendererType Render_Type, NxRendererGlOptions OpenGL_Options );
	void CreateResources();
	void CheckCapabilities();
	ProgressBar * mProgressBar;
	bool m_RenderStop;
	bool m_DeviceLost;
	//bool IsResized();
	//bool m_Resized;
	bool mShutdown;
	//bool m_Orthogonal ;
	//bool m_Fullscreen;
	bool m_HasPreviewWindow;
	bool m_PauseRendering;

	bool mIsInitialized;

	NxContentManager * mSceneManager;
	NxCamera * mCamera;
	NxViewport * mViewport;

	NxRenderWindow * mNxWindow;

	NxRendererType mRenderSystem;
	NxRendererGlOptions mRenderOptions;
	//Ogre::SceneManager * mSceneManager;
	 Ogre::Root * mRoot;
	//Ogre::Viewport * mViewport;
	//Ogre::Camera * mCamera;
	//NxRenderWindow * mNxWindow;
	Ogre::RenderSystem * NxRenderSystem;
	std::string NxMediaPath;
	NxStateManager * mStateManager;

	/** Set of registered frame listeners */
	//std::set<NxFrameListener*> mFrameListeners;

	/** Set of frame listeners marked for removal*/
	//std::set<NxFrameListener*> mRemovedFrameListeners;

	std::vector<NxFrameListener*> mFrameListeners;

};

} // NxGraphics_Namespace  
#endif