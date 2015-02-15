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
#ifndef __NXDEVICEMIDI_H__
#define __NXDEVICEMIDI_H__

#include "NxDevice_Prerequisites.h"

namespace NxDevice_Namespace {

class NxDevice_Export NxMidiManager
{
public:
	NxMidiManager();
	~NxMidiManager();
	void ListMidiInputs();
	void ListMidiOutputs();
	void GetMidiInputList( std::vector<std::string> & MidiInputList );
	void GetMidiOutputList( std::vector<std::string> & MidiOutputList );
 
//private:
	typedef std::map< int, std::string > Midi_Inputs_List;
	typedef std::map< int, std::string > Midi_Ouputs_List;
	Midi_Inputs_List Midi_Inputs;
	Midi_Inputs_List Midi_Outputs;
};

class NxDevice_Export NxDeviceMidi : public NxDevice_Hardware  
{
public:
	NxDeviceMidi();
	~NxDeviceMidi();
	void BuildParamDictionary();
	bool loadPropertySheet();
	bool SavePropertySheet();
	bool ConnectDevice();
	bool DisconnectDevice();
	bool InitializeMidiOutput( const std::string & DevName ); 
	bool InitializeMidiInput( const std::string & DevName ); 
	//! Type: Note On/Off/Control
	void SendMidiMessage( int Type, int Channel, int Note, int Velocity );
	//void NxDevice_MIDI_Initialize_Engine();
	NxMidiStream * midi ; //main stream
	bool ThreadShouldRun  ;

private:
    NxMidiError status, length;
	bool found_midi_out_device  ;
	bool found_midi_in_device  ;
    NxMidiEvent buffer[1];
	#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
	HANDLE ThreadMidi ;
	#endif
	std::string Midi_Device_Name ;
	bool active ;
	int Midi_Index ;
};

} 

#endif