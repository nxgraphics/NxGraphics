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
#ifndef __NXDEVICEPrerequisites_H__
#define __NXDEVICEPrerequisites_H__


#include <NxDeviceConfig.h>
#include "NxDevice_Plateform.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
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
#include <iostream>
#include <fstream>


enum NxMidiMessage
{
	MIDI_CLOCK = 0xf8,
	MIDI_ACTIVE = 0xfe,
	MIDI_STATUS_MASK = 0x80,
	MIDI_SYSEX = 0xf0,
	MIDI_EOX = 0xf7,
	MIDI_START = 0xFA,
	MIDI_STOP = 0xFC,
	MIDI_CONTINUE = 0xFB,
	MIDI_F9 = 0xF9,
	MIDI_FD = 0xFD,
	MIDI_RESET = 0xFF,
	MIDI_NOTE_ON = 0x90,
	MIDI_NOTE_OFF = 0x80,
	MIDI_CHANNEL_AT = 0xD0,
	MIDI_POLY_AT = 0xA0,
	MIDI_PROGRAM = 0xC0,
	MIDI_CONTROL = 0xB0,
	MIDI_PITCHBEND = 0xE0,
	MIDI_MTC = 0xF1,
	MIDI_SONGPOS = 0xF2,
	MIDI_SONGSEL = 0xF3,
	MIDI_TUNE = 0xF6
};



typedef void NxMidiStream;
typedef enum {
    NxMidiNoError = 0,
    NxMidiNoData = 0, /* A "no error" return that also indicates no data avail. */
    NxMidiGotData = 1, /* A "no error" return that also indicates data available */
    NxMidiHostError = -10000,
    NxMidiInvalidDeviceId, /* out of range or 
                        * output device when input is requested or 
                        * input device when output is requested or
                        * device is already opened 
                        */
    NxMidiInsufficientMemory,
    NxMidiBufferTooSmall,
    NxMidiBufferOverflow,
    NxMidiBadPtr,
    NxMidiBadData, /* illegal midi data, e.g. missing EOX */
    NxMidiInternalError,
    NxMidiBufferMaxSize /* buffer is already as large as it can be */
    /* NOTE: If you add a new error type, be sure to update Pm_GetErrorText() */
} NxMidiError;

typedef long NxMidiTimestamp;
//typedef long NxMidiMessage;



typedef struct 
{
    NxMidiMessage message;
    NxMidiTimestamp timestamp;
} NxMidiEvent;


class MTRand;

 

namespace NxDevice_Namespace {

class NxDevice_Hardware ;
class NxDeviceMidi ;
class NxMidiManager ;


struct NxDevice3DConnexionInst;

struct NxDeviceDmxUtil;
class NxDeviceDmx;

struct NxDeviceTrackIrInst;
class NxDeviceTrackIr;

struct NxDeviceWiimoteInst;
class NxDeviceWiimote;

class NxDeviceOscListener;
class NxDeviceOsc;
class NxDeviceOscMessages;

class NxDeviceManager;








/*
template <typename T> class NxDevice_Export Singleton
{
	public:
	  static T* ms_Singleton;
        Singleton( void )
        {
			assert( !ms_Singleton );
			#if defined( _MSC_VER ) && _MSC_VER < 1200	 
            int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
			#else
			ms_Singleton = static_cast< T* >( this );
			#endif
        }

        ~Singleton( void )
		{  
			assert( ms_Singleton );
			ms_Singleton = 0;  
		}

        static T& getSingleton( void )
		{	
			assert( ms_Singleton );
			return ( *ms_Singleton );
		}

        static T* getSingletonPtr( void )
		{ 
			return ms_Singleton;
		}
    };*/


}


#endif
