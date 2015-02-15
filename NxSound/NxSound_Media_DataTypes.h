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
#ifndef __NXSOUNDMEDIADATATYPES_H__
#define __NXSOUNDMEDIADATATYPES_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSound_Export NxSoundFrames  
{
public :
	NxSoundFrames( unsigned int nFrames = 0, unsigned int nChannels = 0 );
	NxSoundFrames( const NxDouble& value, unsigned int nFrames, unsigned int nChannels );
	~NxSoundFrames();
	NxSoundFrames( const NxSoundFrames& f );
	NxSoundFrames& operator= ( const NxSoundFrames& f );
	NxDouble& operator[] ( size_t n );
	NxDouble operator[] ( size_t n ) const;
	void operator+= ( NxSoundFrames& f );
	void operator*= ( NxSoundFrames& f );
	NxDouble& operator() ( size_t frame, unsigned int channel );
	NxDouble operator() ( size_t frame, unsigned int channel ) const;
	NxDouble interpolate( NxDouble frame, unsigned int channel = 0 ) const;
	size_t size() const { return size_; }; 
	bool empty() const;
	void resize( size_t nFrames, unsigned int nChannels = 1 );
	void resize( size_t nFrames, unsigned int nChannels, NxDouble value );
	unsigned int channels( void ) const { return nChannels_; };
	unsigned int frames( void ) const { return nFrames_; };
	void setDataRate( NxDouble rate ) { dataRate_ = rate; };
	NxDouble dataRate( void ) const { return dataRate_; };
 
private:
	NxDouble * NxData_;
	NxDouble dataRate_;
	size_t nFrames_;
	unsigned int nChannels_;
	size_t size_;
	size_t bufferSize_;
 
};

inline bool NxSoundFrames::empty() const
{
	if ( size_ > 0 ) return false;
	else return true;
}

inline NxDouble& NxSoundFrames::operator[] ( size_t n )
{
	return NxData_[n];
}

inline NxDouble NxSoundFrames :: operator[] ( size_t n ) const
{
	return NxData_[n];
}

inline NxDouble& NxSoundFrames :: operator() ( size_t frame, unsigned int channel )
{
	return NxData_[ frame * nChannels_ + channel ];
}

inline NxDouble NxSoundFrames :: operator() ( size_t frame, unsigned int channel ) const
{
	return NxData_[ frame * nChannels_ + channel ];
}

inline void NxSoundFrames :: operator+= ( NxSoundFrames& f )
{
	NxDouble *fptr = &f[0];
	NxDouble *dptr = NxData_;
	for ( unsigned int i=0; i<size_; i++ )
		*dptr++ += *fptr++;
}

inline void NxSoundFrames :: operator*= ( NxSoundFrames& f )
{
	NxDouble *fptr = &f[0];
	NxDouble *dptr = NxData_;
	for ( unsigned int i=0; i<size_; i++ )
		*dptr++ *= *fptr++;
}

}

#endif