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
#ifndef __NXCOREWINDOW_H__
#define __NXCOREWINDOW_H__

#include "NxPrerequisites.h"

class AConfiguration;

namespace Nx {

class NxGraphics_Export NxRenderWindowDesc
{
public:
	//! class Constructor.
	NxRenderWindowDesc();
	//! Width.
	int Width;
	//! Height.
	int Height;
	//! Hwnd to attach.
	void * Window;
	//! Fullscreen.
	bool Fullscreen;
	//! ScreenIndex
	unsigned int ScreenIndex;
	//! VSync
	bool Vsync;
	//! Fsaa
	int Fsaa;
	//! Vsync Rate
	int VsyncRate;
	std::string mWindowName;
#ifdef __ANDROID__
	void * GlContext;
	AConfiguration* mConfig;
#endif
};

class NxGraphics_Export NxRenderWindow
{
public :
	NxRenderWindow( const NxRenderWindowDesc & WindowDesc );
	~NxRenderWindow();
	float GetWidth();
	float GetHeight();
	bool HasFocus();
	void SetFullscreen( bool FullScreen );
	void SetFullscreen( bool FullScreen, int Width, int Height );
	void SetAutoUpdated( bool update );
	bool IsFullscreen();
	void Update( bool swapbuffer );
	void SwapBuffers();
	void SetHidden( bool Hidden );
	float GetAverageFPS() const;
	bool IsSelfWindow();
	void SetWindowPosition( unsigned int Left, unsigned int Top );
	void SetWindowSize( unsigned int width, unsigned int height );
	void UpdateWindowMetrics();
	//! Get Window Native Handle ( on windows it will be HWND ).
	void * GetWindowNativeHandle();
	void GetCustomAttribute( const std::string& name, void* pData );
	NxViewport * AddViewport( NxCamera * Cam, int ZOrder = 0 );
	unsigned short GetNumViewports() const;
	NxViewport* GetViewport(unsigned short index);
	void AddInputListener( NxInputListener * Listener );
	void AddListener( NxEngineListener * Listener );
	void RemoveListener( NxEngineListener * Listener );
	Ogre::RenderWindow * GetWindow();
#ifdef __ANDROID__
	void CreateInternalResources( void * window, AConfiguration* config );
	void DestroyInternalResources();
#endif

private :
	typedef map<int, NxViewport*>::type NxViewportList;
	/// List of viewports, map on Z-order
	NxViewportList mViewports;
	//is auto created window ?
	bool mIsSelfWindow;
	Ogre::RenderWindow * mWindow;
	NxRenderWindowEventListener * mWindowListener;
 
};

}

#endif