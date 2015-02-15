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
#ifndef __NXVIEWPORT_H__
#define __NXVIEWPORT_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxViewport
{
public :
	//! NxViewport Class Constructor.
	NxViewport( NxRenderWindow * ParentWindow, NxCamera * Cam, int Zorder );
	//! NxViewport Class Destructor.
	~NxViewport();
	//! Set viewport Dimensions.
	void SetDimensions( float Left, float Top, float Width, float Height );
	//! clear
	void Clear(unsigned int buffers = NXFBT_COLOUR | NXFBT_DEPTH, const NxColourValue& colour = NxColourValue::Black, float depth = 1.0f, unsigned short stencil = 0);
	//! set orientation mode
	void SetOrientationMode( NxOrientationMode orientationMode, bool setDefault = true ); 	
	//! Set viewport background colour.
	void SetBackgroundColour( const NxColourValue & BackGroundColour );
	//! set shadows enabled
	void SetShadowsEnabled( bool enabled );
	//! set skies enabled
	void SetSkiesEnabled( bool enabled );
	//! set visibility mask
	void SetVisibilityMask( unsigned int mask);
	//! set overlays enabled
	void SetOverlaysEnabled( bool enabled );
	//! Get Actual Left.
	int GetActualLeft() const;
	//! Get Actual Top.
	int GetActualTop() const;
	//! Get Actual Width.
	int GetActualWidth() const;
	//! Get Actual Height.
	int GetActualHeight() const;
	//! Set clear every frame.
	void SetClearEveryFrame( bool clear, unsigned int buffers = NXFBT_COLOUR | NXFBT_DEPTH ) ;
	//! Set auto Update.
	void SetAutoUpdated( bool AutoUpdate );
	//! Update Viewport;
	void Update();
	//! Get Viewport.
	Ogre::Viewport * GetViewport();
private :
	Ogre::Viewport * mViewport;

};

}

#endif