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
#ifndef STK_FILEREADFLAC_H
#define STK_FILEREADFLAC_H

#include "FileRead.h"

namespace NxSound_Namespace { // stk

class NxSound_Export FileReadFlac : public FileReadBase
{
public:
	FileReadFlac();
	FileReadFlac( std::string fileName );
	~FileReadFlac( void );
	void Open( std::string fileName );
	void Close( void );
	bool IsOpen( void );
	void Read( NxSoundFrames& buffer, unsigned long startFrame = 0, bool doNormalize = true );
	bool getFlacInfo( const char *fileName );
	bool seek( uint64 sampleNum );
	NxAudioFormat GetNativeResolution();

	void OnMetadataCallback( const void * metadata );
	void OnWriteCallback( const void *frame, const int * const buffer[] );

	int BufferFlacL[4096*SND_BUFFER_SIZE*10];
	int BufferFlacR[4096*SND_BUFFER_SIZE*10];

};

} // stk namespace

#endif