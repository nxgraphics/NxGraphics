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
#ifndef __NXOVERLAYTEXT_H__
#define __NXOVERLAYTEXT_H__

#include "NxPrerequisites.h"
#include "NxOverlayElement.h"

namespace Nx {

class NxGraphics_Export NxOverlayText : public NxOverlayElement
{
public :
	//! NxOverlayContainer class constructor.
	NxOverlayText( const std::string & TextElementName );
	//! NxOverlayContainer class destructor.
	~NxOverlayText();
	//! set metric mode.
	void setMetricsMode( NxMetricsMode gmm );
	//! Set Parameter Value.
	void SetParameter( const std::string & ParameterName, const std::string & ParameterValue );
	//! Set Text Caption.
	void SetCaption( const std::string & Caption );
	//! Get Caption.
	const std::string & GetCaption() const;
	//! Set Char Height.
	void SetCharHeight( float Size );
	//! Set Text Space Width.
	void SetSpaceWidth( float Width );
	//! Set Font Name.
	void SetFontName( const std::string & FontName );
	//! Set Font Bottom Colour.
	void SetColourBottom( const NxColourValue & Val );
	//! Set Font Top Colour.
	void SetColourTop( const NxColourValue & Val );
};

}

#endif