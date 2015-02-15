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
#ifndef STK_FILEREADBASE_H
#define STK_FILEREADBASE_H

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace { // stk
 
class FileReadBase
{
public:
	FileReadBase();
	~FileReadBase();
	unsigned long GetFileSize() const;
	unsigned int GetChannels() const;
	NxDouble GetFileRate() const;
	int64 GetFirstSamplePosition();
	//int GetBps();
	unsigned int GetBlockSize() const;
	uint64 GetSampleNum();


	//virtual unsigned int GetResolution() = 0;
	virtual void Open( std::string fileName ) = 0;
	virtual void Close() = 0 ;
	virtual bool IsOpen() = 0;
	virtual void Read( NxSoundFrames & buffer, unsigned long startFrame = 0, bool doNormalize = true ) = 0;
	 void SetOutputResolution( NxAudioFormat Res ) ;
	virtual NxAudioFormat GetNativeResolution() = 0;

	void SetLooping( bool Loop );
	bool IsFinished(); 

protected:

	double mResolutionCoef;
	uint64 mSampleNum;
	NxDouble mFileRate;
	unsigned long mDataOffset;
	NxAudioFormat mDataType;
	unsigned int mBlockSize;
	signed long long mLastSampleNum;
	FILE * mFile;
	unsigned long mFileSize;
	unsigned int mChannels;
	int64 SampleFirstPosition ;
	bool mLoop;
	bool mFinished;
};

} // stk namespace

#endif