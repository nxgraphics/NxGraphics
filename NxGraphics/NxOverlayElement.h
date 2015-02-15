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
#ifndef __NXOVERLAYELEMENT_H__
#define __NXOVERLAYELEMENT_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxOverlayElement
{
public :
	//! NxOverlayElement class constructor
	NxOverlayElement();
	//! NxOverlayElement class destructor
	~NxOverlayElement();
	//! Set metric mode
	virtual void SetMetricsMode( NxMetricsMode Mode );
	//! Set Position.
	void SetPosition( float X, float Y);
	//! Set Dimensions.
	void SetDimensions( float Width , float Height );
	//! Set Material name
	virtual void SetMaterialName( const std::string & matName );
	//! Get Material Name
	virtual const std::string & GetMaterialName() const;
	//! Get Width.
	float GetWidth() const;
	//! Set Width.
	void SetWidth( float Width );
	//! Get Height.
	float GetHeight() const;
	//! Set Height.
	void SetHeight( float Height );
	//! Set left
	void SetLeft( float left);
	//! get Left
	float GetLeft() const;
	//! Set Top
	void SetTop( float Top );
	//! Show
	void Show();
	//! Hide
	void Hide();
	//! get base element
	Ogre::OverlayElement * GetElement();
protected :
	Ogre::OverlayElement * mOverlayElement;
};

}

#endif