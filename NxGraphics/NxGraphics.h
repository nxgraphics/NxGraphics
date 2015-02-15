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
#ifndef __NXGRAPHICS_H__
#define __NXGRAPHICS_H__

#include "NxSingleton.h"

class AAssetManager;
class ANativeWindow;
class AConfiguration;

namespace Nx {

class NxGraphics_Export NxGraphicsDesc
{
public:
	//! class Constructor.
	NxGraphicsDesc();
	//! Width.
	int Width;
	//! Height.
	int Height;
	//! Renderer.
	NxRendererType Renderer;
	//! OpenGl Options.
	NxRendererGlOptions GlOptions;

	//! OpenGl Context.
	//HGLRC OpenGlContext;
	//! Hwnd to attach.
	void * Window;
	//! Fullscreen.
	bool Fullscreen;
	//! ScreenIndex
	unsigned int ScreenIndex;
	//! Logs.
	NxLog * Log;
	//! Media Path
	std::string MediaPath;
	//! VSync
	bool Vsync;
	//! Fsaa
	int Fsaa;
	//! Vsync Rate
	int VsyncRate;
	//! Use NVidia Perf Hud For Debugging.
	bool UsePerfHud;

	float mSplashSize[4];
	std::string mSplashTexture;

	#ifdef __ANDROID__
	AAssetManager* mAssetMgr;
	ANativeWindow* mNativeWnd;
	AConfiguration* mConfig;
	void * GlContext;
#endif

};
 
class NxGraphics_Export NxGraphics : public NxSingleton<NxGraphics>
{
public:
	//! NxGraphics class Constructor.
	NxGraphics();
	//! NxGraphics class Destructor.
	~NxGraphics();
	//! Initialize and Create the Engine.
	bool CreateNxEngine( const NxGraphicsDesc& engineDesc );
	//! Release the Engine.	
	void ReleaseEngine();
	//! Set Main Window Fullscreen.
	void SetFullScreen( int Width, int Height, bool Fulscreen );

	void AddState( NxState * State );
	void SetState( NxState * State );
	void RemoveState( NxState * State );


	//! open a NxScript File
	void OpenScript( const std::string& ScriptName );
	//! Get Engine Manager.
	NxEngine * GetEngineManager();
	//! Get Mixer Manager.
	//MixerManager * GetMixerManager();
	//! Get Device Manager.
	NxDeviceManager * GetDeviceManager();
	//! Get Sound Manager.
	NxSoundManager * GetSoundManager(); 
	//! Render one frame, used in embeded mode.
	void Update();
	//! Start Rendering.
	void StartRendering();
	//! is engine running ?
	bool IsRunning();
	//! Access class Singleton.	
	static NxGraphics& getSingleton();

};

} 
#endif



