/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       __  ___                         
			   / | / /_  __ /  |/  /____   _____ ____ _ ____ 
			  /  |/ /| |/_// /|_/ // __ \ / ___// __ `// __ \
			 / /|  /_>  < / /  / // /_/ // /__ / /_/ // /_/ /
			/_/ |_//_/|_|/_/  /_/ \____/ \___/ \__,_// .___/ 
													/_/      
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
 
#include "NxMocap_Plateform.h"
//#include "NxMocap_Prerequisites.h"
#include "NxMocap.h"
#include "NxMocap_Singleton.h"
#include "NxMocap_Utilities.h"
#include "NxMocap_Log.h"
#include "NxMocap_Mysql.h"
#include "NxMocap_Stream.h"
#include "NxMocap_Player.h"
#include "NxMocap_Devices.h"
#include "NxMocap_Recorder.h"
#include "NxMocap_Skeleton.h"


#define Log( Message ) NxMocapLog::getSingleton().LogMessage( Message )