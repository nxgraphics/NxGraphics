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
#ifndef __NXSOUNDPLUGINEFFECT_H__
#define __NXSOUNDPLUGINEFFECT_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

//!  A class providing basic description for available plugins.
class NxSound_Export NxSoundPluginDescription
{
public : 
	std::string Name ;
	std::string Path ;
	NxSound_Effects_Library_Types Type;
};

//!  A class to manage a sound effects.
/*!
	this is the base class for all media effects and shouldnt be used directly.
*/
class NxSound_Export NxSoundEffect
{
protected :
	//! NxSoundEffect class constructor.
	NxSoundEffect();
public :
	//! NxSoundEffect class deconstructor.
	virtual ~NxSoundEffect();
	//! Set Number of Channels, for initialization.
	void SetNumChannels( unsigned int Channels );
	//! Get Effect Type.
	NxSound_Effects_Library_Types GetType() ;
	//! Set Effect Muted.
	void SetMuted( bool Muted );
	//! Is Effect Muted.
	bool IsMuted();
	//! Set Effect Mix Value.
	virtual void SetMix( float MixVal );
	//! Get Effect Mix Value.
	virtual float GetMix();
	//! Process sample chunk to effect.
	virtual void Process( float ** BufIn , float ** BufOut , int BlockSize );
	//! Is Effect an Instrument.
	//bool IsInstrument();
protected:
	//! Get Num Channels.
	unsigned int GetNumChannels();
	//! Effect Type.
	NxSound_Effects_Library_Types mType;
	//! Muted.
	bool mMuted;
	//bool Instrument ;
private :
	unsigned int PluginChannelNumber ;
};


}


#endif