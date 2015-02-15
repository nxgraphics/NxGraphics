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
#pragma once
#pragma warning(disable:4006)

#if !defined _CRT_SECURE_NO_DEPRECATE && _MSC_VER > 1300
#define _CRT_SECURE_NO_DEPRECATE /* to avoid multiple Visual Studio 2005 warnings */
#endif

//== VST ==//
#define VST_2_4_EXTENSIONS 1
#define VST_FORCE_DEPRECATED 0

#include "NxSound_Plateform.h"
#include "NxSound.h"
#include "NxSound_Singleton.h"
#include "NxSound_Utilities.h"
#include "NxSound_Log.h"
#include "NxSound_Device.h"
#include "NxSound_Device_Input.h"
#include "NxSound_Device_Output.h"
#include "NxSound_Media_DataTypes.h"
#include "NxSound_Media_File.h"
#include "NxSound_Media_File_MP3.h"
#include "NxSound_Media_File_OGG.h"
#include "NxSound_Media_File_FLAC.h"
#include "NxSound_Media_File_VSTi.h"
#include "NxSound_Media_File_WAV.h"
#include "NxSound_Media_File_Buffer.h"
#include "NxSound_Media_Inst_MODAL.h"
#include "NxSound_Effect_Base.h"
#include "NxSound_Effect_VSTHost.h"
#include "NxSound_Effect_VSTPlugin.h"
#include "NxSound_Effect_VSTPluginPreset.h"
#include "NxSound_Effect_Shift.h"
#include "NxSound_Effect_Chorus.h"
#include "NxSound_Effect_Echo.h"
#include "NxSound_Effect_Granulate.h"
#include "NxSound_Effect_BeatDetector.h"
#include "NxSound_Effect_OggBroadCaster.h"
#include "NxSound_Effect_3D.h"
#include "NxSound_FFT.h"

#define Log( Message ) NxSoundLog::getSingleton().LogMessage( Message )
