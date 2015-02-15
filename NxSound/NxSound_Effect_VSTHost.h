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
#ifndef __NXSOUNDEFFECTVSTHOST_H__
#define __NXSOUNDEFFECTVSTHOST_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {
 
//!  A class to manage a Vst Instruments.
/*!
	this is the host class to load vst plugins from steinberg, supports instruments and effects.
*/
class NxSound_Export NxSoundEffectVSTHost : public Singleton<NxSoundEffectVSTHost> 
{
public :
	//! NxSoundEffectVSTHost Class Constructor.
	NxSoundEffectVSTHost();
	//! NxSoundEffectVSTHost Class Destructor.
	~NxSoundEffectVSTHost();
	//! NxSoundEffectVSTHost Class Singleton.
	static NxSoundEffectVSTHost& getSingleton();
	//! Load vst from path.
	NxSoundEffectVSTPlugin * LoadVstPlugin( const std::string & Path );
	//! unload vst.
	bool UnloadVstPlugin( NxSoundEffectVSTPlugin * );
	//! get vst from vsy effect.
	NxSoundEffectVSTPlugin * GetVSTPluginFromEffect( AEffect * effect );
	//! Set Master clock time.
	void SetTime( double Nanoseconds );
	//! Calculate time.
	void CalcTimeInfo( long lMask = -1 );


	static long   AudioMasterCallback( AEffect * effect, long opcode, long index, long value, void * ptr, float opt );
	// static long __cdecl AudioMasterCallback( AEffect * effect, long opcode, long index, long value, void * ptr, float opt );//for ndk-build

};
 
}//namespace

#endif