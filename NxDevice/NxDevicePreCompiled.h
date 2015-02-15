/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       ____               _           
			   / | / /_  __ / __ \ ___  _   __ (_)_____ ___ 
			  /  |/ /| |/_// / / // _ \| | / // // ___// _ \
			 / /|  /_>  < / /_/ //  __/| |/ // // /__ /  __/
			/_/ |_//_/|_|/_____/ \___/ |___//_/ \___/ \___/ 
			                                                
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
/*
#include <string>
#include <iostream>
#include <sstream>
#include <string>
*/
//devices enabled
#define NXDEVICE_USE_DMX 1
#define NXDEVICE_USE_MIDI 1
#define NXDEVICE_USE_OSC 1
#define NXDEVICE_USE_ILDA 1
#define NXDEVICE_USE_TRACKIR 1
#define NXDEVICE_USE_WIIMOTE 1
//#define NXDEVICE_USE_WAVERIDER 

#include "NxDevice_Plateform.h"
#include "NxDevice.h"
#include "NxDevice_Singleton.h"
#include "NxDevice_Utilities.h"
#include "NxDevice_Log.h"
#include "NxDevice_Hardware.h"
#include "NxDevice_Midi.h"

#define Log( Message ) NxDeviceLog::getSingleton().LogMessage( Message )

using namespace std;