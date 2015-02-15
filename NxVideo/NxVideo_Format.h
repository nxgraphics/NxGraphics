/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __      _    __ _      __           
			   / | / /_  __| |  / /(_)____/ /___   ____ 
			  /  |/ /| |/_/| | / // // __  // _ \ / __ \
			 / /|  /_>  <  | |/ // // /_/ //  __// /_/ /
			/_/ |_//_/|_|  |___//_/ \__,_/ \___/ \____/ 

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
#ifndef __NXVIDEOFORMAT_H__
#define __NXVIDEOFORMAT_H__

#define FOUR_CHAR_CODE(x) (x) 

namespace NxVideo_Namespace {

// Video codecs info see : https://developer.apple.com/library/mac/documentation/QuartzCore/Reference/CVPixelFormatDescriptionRef/Reference/reference.html#//apple_ref/doc/constant_group/Pixel_Format_Types
enum NxVideoPixelFormat {
	NxVideoPixelFormat24BGR			= FOUR_CHAR_CODE('24BG'),	/* 24 bit bgr  */
	NxVideoPixelFormat24RGB			= 0x00000018,				/* 24 bit rgb  */
	NxVideoPixelFormat32ARGB		= 0x00000020,				/* 32 bit argb (Mac)*/
	NxVideoPixelFormat32BGRA		= FOUR_CHAR_CODE('BGRA'),	/* 32 bit bgra (Matrox)*/
	NxVideoPixelFormat32ABGR		= FOUR_CHAR_CODE('ABGR'),	/* 32 bit abgr */
	NxVideoPixelFormat32RGBA		= FOUR_CHAR_CODE('RGBA'),	/* 32 bit rgba */
	NxVideoPixelFormat2vuy			= FOUR_CHAR_CODE('2vuy'),	/* UYVY 4:2:2 byte ordering 16 */
	NxVideoPixelFormaty420			= FOUR_CHAR_CODE('y420'),	/* UYVY 4:2:0 byte ordering 16 */
	NxVideoPixelFormaty422			= FOUR_CHAR_CODE('y422'),	/* UYVY 4:2:2 byte ordering 16 */
	NxVideoPixelFormaty444			= FOUR_CHAR_CODE('y444')	/* UYVY 4:4:4 byte ordering 16 */
};


enum NxVideoAudioFormat {
	NxVideoAudioFormat16BpS,	/* 16 bit per samples, signed int */
	NxVideoPixelFormat32BpS		/* 32 bit per samples, signed float */
 
};


struct NxVideoPixelFormatInfo {

	NxVideoPixelFormatInfo() {
		mDstVideoFormat = NxVideoPixelFormat32RGBA;
		mDstAudioFormat = NxVideoPixelFormat32BpS;
		mDstAudioBufferSize = 2048;
		mDstAudioSampleRate = 44100.0;
		//mOpenglID = 0;
	}

	//Video settings
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mStride;
	unsigned int mBpp;
	//unsigned int mOpenglID;
	NxVideoPixelFormat mDstVideoFormat;
	
	//Audio settings
	NxVideoAudioFormat mDstAudioFormat;
	unsigned long mDstAudioBufferSize;
	double mDstAudioSampleRate;



};

}

#endif