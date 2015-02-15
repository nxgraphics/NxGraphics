/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __      _____                           __
			   / | / /_  __/ ___/ ____   __  __ ____   ____/ /
			  /  |/ /| |/_/\__ \ / __ \ / / / // __ \ / __  / 
			 / /|  /_>  < ___/ // /_/ // /_/ // / / // /_/ /  
			/_/ |_//_/|_|/____/ \____/ \__,_//_/ /_/ \__,_/   
			                                                  
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
#ifndef __NXSOUNDEFFECT3D_H__
#define __NXSOUNDEFFECT3D_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSound_Export NxSoundEffect3D : public NxSoundEffect
{
public:
	//! NxSoundEffect3D Class Constructor. 
	NxSoundEffect3D();
	//! NxSoundEffect3D Class Deconstructor.
    ~NxSoundEffect3D();
	//! Process Sound Buffer Chunk.
	void Process( float ** BufIn , float ** BufOut, int BlockSize );
	//! Set Sound Relative To Listener.
	void SetRelative( bool Relative );
	//! Sound Relative to User ?.
	bool GetRelative();
	//! Set Sound Position in meters.
	void SetPosition( float X, float Y, float Z );
	//! Get Sound Position.
	void GetPosition( float &X, float &Y, float &Z );
	//! Set Sound Direction.
	void SetDirection( float X, float Y, float Z );
	//! Get Sound Direction.
	void GetDirection( float &X, float &Y, float &Z );
	//! Set Sound Velocity.
	void SetVelocity( float X, float Y, float Z) ;
	//! Get Sound Velocity.
	void GetVelocity( float &X, float &Y, float &Z );
	//! Set Sound linear Range in meters.
	void SetRange( float Min, float Max );
	//! Get sound linear Range
	void GetRange( float &Min, float &Max );
	//! Set Sound Cones.
	void SetSoundCone( float Inner, float Outer, float OuterGain );
private :
	bool playing();
	void empty();
	#define NUMBUFFERS 2
	unsigned int mBuffers[NUMBUFFERS]; 
    unsigned int mSource;
	int mStreamFormat;
};

}

#endif