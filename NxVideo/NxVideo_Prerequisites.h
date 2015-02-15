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
#ifndef __NXVIDEOPrerequisites_H__
#define __NXVIDEOPrerequisites_H__

#include <NxVideoConfig.h>
#include "NxVideo_Plateform.h"
#include "NxVideo_Format.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

#include <time.h>
#include <fstream>
#include <locale>//toupper

using namespace std;
 
namespace NxVideo_Namespace {

enum NxVideoStreamFlags{
	NxVideoAudioExtractionComplete = (1L << 0),
	NxVideoAudioExtractionRunning  = (1L << 1)
};

enum NxVideoStreamType 
{
	NxVideoAviFile,
	NxVideoMovFile,
	NxVideoTheoraFile
}; 

enum NxVideo_PlayBack_Type 
{ 
	VIDEO_PLAYBACK_NORMAL, // play and stop
	VIDEO_PLAYBACK_REVERSE, // play reverse and stop
	VIDEO_PLAYBACK_BOUNCE, // play both ways without stop
	VIDEO_PLAYBACK_RANDOM,
	VIDEO_PLAYBACK_MANUAL
}; 

enum NxVideo_PlayBack_Status
{  
	VIDEO_STATUS_PLAY,
	VIDEO_STATUS_PAUSE,
	VIDEO_STATUS_STOP
};

enum NxVideoCaptureDeviceType
{ 
	NxVideoCaptureDS,
	NxVideoCaptureVFW,
	NxVideoCaptureQuicktime,
	NxVideoCapturePGR
};
 
class NxVideoUtils;
class NxVideoBuffer;
class NxVideoManager;
class NxVideo_Avi_Player ;
class NxVideo_Avi_Manager ;
class NxVideo_Grabber;
class NxVideo_Player;
 
class NxVideoGrabberVFW ;
class NxVideoGrabberFireFly ;

struct NxVideoVFW;
struct NxVideoQuickTime;
struct NxVideoTheora;
struct NxVideoRecorderVFW;

struct NxVideoDeviceDS;
struct NxVideoDeviceVFW;
struct NxVideoDeviceQTM;
struct NxVideoDevicePGR;
 
}// NxVideo_Namespace

#endif
