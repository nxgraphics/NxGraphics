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
#ifndef __NXVIDEOGRABBER_H__
#define __NXVIDEOGRABBER_H__
 
#include "NxVideo_Prerequisites.h"

namespace NxVideo_Namespace {

//!  A Class To Manage Video Cameras / DirectShow / Video For Windows / Quicktime / Point Grey Research.
/*!
	NxVideo_Grabber Class Implementation.
*/
class NxVideo_Export NxVideo_Grabber
{
public:
	//! NxVideo_Grabber Class Constructor.
	NxVideo_Grabber();
	//! NxVideo_Grabber Class Deconstructor.
	~NxVideo_Grabber();
	//! Open Camera with parameters.
	virtual bool OpenVideoDevice( unsigned long Index, int Width, int Height, int Bpp ) = 0;
	//! Close Camera.
	virtual bool CloseVideoDevice() = 0;
	//! Get Camera Type, see NxVideoCaptureDeviceType.
	NxVideoCaptureDeviceType GetCaptureDeviceType();
	//! Get Camera Width.
	unsigned int GetWidth();
	//! Get Camera Height.
	unsigned int GetHeight();
	//! Get Camera Bits per Pixel.
	unsigned int GetBpp();
	//is frame new
	virtual bool NewFrame() = 0;
	//! Get Camera Pixel Buffer.
	virtual unsigned char * GetBuffer() = 0;

	void SetPaused( bool Paused ){ mPaused = Paused; }

	unsigned char * mVideoData;
	bool mNewFrame;

protected:
	unsigned long mVideoWidth;    
	unsigned long mVideoHeight;
	int mVideoBpp;
	bool mPaused;
	unsigned long mDeviceIndex;
	std::string mDeviceName;
	NxVideoCaptureDeviceType mType ;

};

class NxVideo_Export NxVideoGrabberDirectshow : public NxVideo_Grabber
{
public :
	NxVideoGrabberDirectshow();
	~NxVideoGrabberDirectshow();
	bool OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP );
	bool CloseVideoDevice();
	unsigned char * GetBuffer();
	bool NewFrame();
	NxVideoDeviceDS * mVideo;
};

class NxVideo_Export NxVideoGrabberVFW : public NxVideo_Grabber
{
public :
	NxVideoGrabberVFW();
	~NxVideoGrabberVFW();
	bool OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP );
	bool CloseVideoDevice();
	unsigned char * GetBuffer();
	bool NewFrame();
	NxVideoDeviceVFW * mVideo;
};

class NxVideo_Export NxVideoGrabberQuicktime : public NxVideo_Grabber
{
public :
	NxVideoGrabberQuicktime();
	~NxVideoGrabberQuicktime();
	bool OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP );
	bool CloseVideoDevice();
	unsigned char * GetBuffer();
	bool NewFrame();
	NxVideoDeviceQTM * mVideo;
};

class NxVideo_Export NxVideoGrabberPointGrey : public NxVideo_Grabber
{
public :
	NxVideoGrabberPointGrey();
	~NxVideoGrabberPointGrey();
	bool OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP );
	bool CloseVideoDevice();
	unsigned char * GetBuffer();
	bool NewFrame();
	NxVideoDevicePGR * mVideo;
};


}

#endif


