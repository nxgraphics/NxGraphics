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
#ifndef __NXGRAPHICS3DSOUND_H__
#define __NXGRAPHICS3DSOUND_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxScene3DObjectSound : public CountedObj<NxScene3DObjectSound>
{
public :
	//! NxScene3DObjectSound Class Constructor.
	NxScene3DObjectSound( const std::string & SoundName, const std::string & SoundFile );
	//! NxScene3DObjectSound Class Destructor.
	~NxScene3DObjectSound();
	//! Set Global Sound Position.
	void SetPosition( const Nx::Vector3 & Position );
	//! Get Global Sound Position 
	const Nx::Vector3 & GetPosition() const;
	//! Set Sounds Direction XYZ.
	void SetDirection( const Nx::Vector3 & Direction );
	//! Set Sounds Range in Meters.
	void SetRange( const Nx::Vector2 & MinMaxRange );
private :
	NxSoundMediaFile * mSound;
};

}

#endif