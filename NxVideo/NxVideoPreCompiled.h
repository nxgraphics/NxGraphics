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
#pragma once
#pragma warning(disable:4006)

#if !defined _CRT_SECURE_NO_DEPRECATE && _MSC_VER > 1300
#define _CRT_SECURE_NO_DEPRECATE /* to avoid multiple Visual Studio 2005 warnings */
#endif
 
#include "NxVideo_Plateform.h"
#include "NxVideo_Singleton.h"
#include "NxVideo.h"
#include "NxVideo_Log.h"
#include "NxVideo_Utilities.h"
#include "NxVideo_Video.h"
#include "nxvideo_grabber.h"
#include "NxVideo_Player.h"

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
#include <windows.h>
#include <vfw.h>

#if _MSC_VER >= 1400
//  Following 8 lines: workaround for a bug in some older SDKs
#   pragma push_macro("GetProcessInformation")
#   define GetProcessInformation _local_GetProcessInformation
#   include <QTML.h> // to force the header not to be included elsewhere
#   pragma pop_macro("GetProcessInformation")
#endif


#include <QTML.h>
#include <FixMath.h>
#include <QuickTimeComponents.h>
#include <TextUtils.h>
#include <MediaHandlers.h>
#include <GXMath.h>
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_APPLE
#include <QuickTime/QuickTime.h>
#endif














#define Log( Message ) NxVideoLog::getSingleton().LogMessage( Message )


