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
#ifndef STK_FILEREADMP3_H
#define STK_FILEREADMP3_H

#include "FileRead.h"

#define OPT_ACCURACY //#define FPM_64BIT
#include "../nxdeps/include/libmad/mad.h"  

namespace NxSound_Namespace { // stk

class FileReadMp3 : public FileReadBase
{
public:
	FileReadMp3( void );
	FileReadMp3( std::string fileName );
	~FileReadMp3( void );
	void Open( std::string fileName );
	void Close( void );
	bool IsOpen( void );
	void Read( NxSoundFrames& buffer, unsigned long startFrame = 0, bool doNormalize = true );
	bool getMp3Info( const char *fileName );
	NxAudioFormat GetNativeResolution();
	//void SetOutputResolution( NxAudioFormat Res );
	//bool byteswap_;
 

	//==  MAD ==// 
	struct xing
	{
		long flags;
		unsigned long frames;
		unsigned long bytes;
		unsigned char toc[100];
		long scale;
	};

	struct xing _xing; //Xing VBR tag data 

	mad_stream  _madStream; // MAD stream structure 
	mad_frame  _madFrame; // MAD frame structure 
	mad_synth  _madSynth; // MAD synth structure 

	int _samplesLeftInBuffer;
		
	signed long long  _streamSize;

	signed int * _leftChannel;
	signed int * _rightChannel;

	mad_timer_t _madCurTime; // current playing time position  
	mad_timer_t _madTimeLength; //total playing time of current stream  

	unsigned int MADsampleRate ;

	int bitsPerSample  ;
	int MADnumChannels  ;
	unsigned char _buffer[40000]; // input stream buffer  


	signed int mBufferLeft[40000]; // input stream buffer  
	signed int mBufferRight[40000]; // input stream buffer  

	bool seek(signed long long sample);
	int parseXing(struct xing *xing, mad_bitptr ptr, unsigned int bitlen);
	int scanHeader(struct mad_header * header, struct xing *xing);

	//char *output_buf;
	//unsigned long output_bytes, output_at, output_size;
	unsigned long bitrate;
};

} // stk namespace

#endif
