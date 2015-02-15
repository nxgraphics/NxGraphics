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
#ifndef __NXSOUNDMEDIAFILEWAV_H__
#define __NXSOUNDMEDIAFILEWAV_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSound_Export NxSoundMediaFileWav : public NxSoundMediaFile
{
public :
	//! NxSoundMediaFileWav Class Constructor.
	NxSoundMediaFileWav();
	//! NxSoundMediaFileWav Class Deconstructor.
	~NxSoundMediaFileWav();
	//! Open Wav File.
	bool OpenFile( std::string Path, bool Normalize = true );
	//! Set Stream Position in units.
	void SetPosition( unsigned long position, NXSOUNDTIMEUNIT postype );
	//! Get Stream Position in units.
	double GetPosition( NXSOUNDTIMEUNIT postype );
	//! Get Total stream duration.
	unsigned long GetTotalDuration( NXSOUNDTIMEUNIT postype );

	void SetResolution( NxAudioFormat Res );
	//! Update.
	stk::StkFrames& Update( stk::StkFrames& frames );
	void Play();
	void Stop();
	void Rewind();
	bool IsPlaying();
	unsigned long GetNumSamples(); 
	unsigned int GetNumChannels();
	void SetRate( double Rate );
	void SetLooping( bool Loop );
	//no virtuals
	//bool IsFinished();
private :
	stk::FileReadWav * mSoundFile;
	NxSoundFrames dataframe;
};

}

#endif