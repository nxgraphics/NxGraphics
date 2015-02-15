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
#ifndef __NXSOUNDMEDIAFILEFLAC_H__
#define __NXSOUNDMEDIAFILEFLAC_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSound_Export NxSoundMediaFileFlac : public NxSoundMediaFile
{
public :
	NxSoundMediaFileFlac();
	~NxSoundMediaFileFlac();
	bool OpenFile( std::string Filename, bool Normalize = true );
	void SetRate( double Rate );
	void SetResolution( NxAudioFormat Res );

	//NxSoundMediaFile virtuals
	unsigned long GetNumSamples(); 
	void Rewind();
	void Play();
	void Stop();
	void Pause();
	bool isStopped();
	bool isPaused();


	std::vector< NxSoundFrames *> GetSampleDictionary(); 
	//unsigned int GetNumChannels();
	unsigned int GetFirstSamplePosition();

protected :
	//void openFile( std::string fileName, bool raw  , bool doNormalize   );
	void closeFile( void );
	void reset( void );
	void normalize( void );
	void normalize( double peak );
	unsigned long getSize( void ) const { return dataframe.frames(); };
	double getFileRate( void ) const { return dataframe.dataRate(); };
	 
	void addTime( double time );
	double tick( unsigned int channel );
	stk::StkFrames& tick( stk::StkFrames& frames );
	stk::StkFrames& Update( stk::StkFrames& frames  );
	FileReadFlac * fileflac_;
	NxSoundFrames dataframe;
};

}//namespace

#endif