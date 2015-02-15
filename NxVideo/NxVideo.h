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
#ifndef __NXVIDEOMANAGER_H__
#define __NXVIDEOMANAGER_H__

#include "NxVideo_Singleton.h"

namespace NxVideo_Namespace {

//!  Main Interface for NxVideo
/*!
	NxVideoManager Class Implementation.
*/
class NxVideo_Export NxVideoManager : public Singleton<NxVideoManager>  
{
public :
	//! NxVideo manager Class Constructor.
	NxVideoManager();
	//! NxVideo manager Class Deconstructor.
	~NxVideoManager();
	//! NxVideo manager Class Singleton.
	static NxVideoManager& getSingleton();
	//! NxVideo manager Class Singleton.
	static NxVideoManager* getSingletonPtr();
	//! Open A video File.
	NxVideo_Player * OpenVideoFile( const std::string& Filename, NxVideoPixelFormatInfo & info );
	//! Close a File.
	bool CloseVideoFile( NxVideo_Player * File );
	//! open video device
	NxVideo_Grabber * OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP, NxVideoCaptureDeviceType CameraType );
	//! Get Available camera list.
	void GetAvailableCameraNames( std::vector<std::string> & CameraList, NxVideoCaptureDeviceType CameraType );
	//! Get Camera Index From Name.
	unsigned long GetCameraIndexFromName( const std::string & CameraName, NxVideoCaptureDeviceType CameraType );
	//! Quicktime Installed ?
	bool IsQuicktimeEnabled();
private  :
	bool mQuicktimeEnabled;

};



}

#endif



