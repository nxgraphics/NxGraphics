/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Copyright (c) 2011 Stephane Kyles, http://www.subnetworks.org
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
#include "NxVideoPreCompiled.h"
#include "../NxDeps/include/VideoInput/videoInput.h"

namespace NxVideo_Namespace {

struct NxVideoDeviceDS
{
public:
	unsigned char * Buffer;
	videoInput * mVideoGrabber;
	NxVideoDeviceDS() : Buffer( 0 )
	{

	}

	~NxVideoDeviceDS()
	{

	}

} ; 

NxVideoGrabberDirectshow::NxVideoGrabberDirectshow() : NxVideo_Grabber()
{
	mType = NxVideoCaptureDS ;
	mVideo = new NxVideoDeviceDS();
}
 
NxVideoGrabberDirectshow::~NxVideoGrabberDirectshow()
{

}

bool NxVideoGrabberDirectshow::OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP )
{
	mVideo->mVideoGrabber = new videoInput();
	mDeviceIndex = Index;


	mVideo->mVideoGrabber->setUseCallback( false );

	//if(!mVideo->mVideoGrabber->setupDevice( mDeviceIndex, Width, Height ) ) {
	if(!mVideo->mVideoGrabber->setupDevice( mDeviceIndex, Width, Height  ) ) {  //VI.setupDevice(dev,   640, 480, VI_COMPOSITE); 
		Log("Failed to setup Device");
		return false;
	}

	Log("NxVideo frame Buffer Size " + NxVideoUtils::ToString( mVideo->mVideoGrabber->getSize( mDeviceIndex )  ));
 

	//mVideo->mVideoGrabber->showSettingsWindow( mDeviceIndex );

	mVideo->mVideoGrabber->setVerbose(true);
	mVideoWidth = mVideo->mVideoGrabber->getWidth( mDeviceIndex );
	mVideoHeight = mVideo->mVideoGrabber->getHeight( mDeviceIndex );
	mVideoBpp = 3;
	return true ;
}

bool NxVideoGrabberDirectshow::CloseVideoDevice()
{
	if( mVideo->mVideoGrabber )
	{
		mVideo->mVideoGrabber->stopDevice( mDeviceIndex );
		delete mVideo->mVideoGrabber;
	}
	return true;
}

bool NxVideoGrabberDirectshow::NewFrame()
{
	if(mPaused) return false;
	return mVideo->mVideoGrabber->isFrameNew( mDeviceIndex );
}

unsigned char * NxVideoGrabberDirectshow::GetBuffer()
{
	return mVideo->mVideoGrabber->getPixels( mDeviceIndex ,false, false); 
}

}//namespace