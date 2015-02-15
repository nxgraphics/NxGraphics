#include "NxDevicePreCompiled.h"

#include "../nxdeps/include/PortMidi/pm_common/portmidi.h"
#include "../nxdeps/include/PortMidi/porttime/porttime.h"

namespace NxDevice_Namespace {

NxMidiManager::NxMidiManager()
{

	if( Pm_Initialize() != pmNoError ) {
		Log("Could not initialize PortMidi.");
	}

	ListMidiInputs();
	ListMidiOutputs();

}

NxMidiManager::~NxMidiManager()
{
	Pm_Terminate( );

}

void NxMidiManager::GetMidiInputList( std::vector<std::string> & MidiInputList )
{
	 for (int i = 0; i < Pm_CountDevices(); i++)  {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
		if (info->input) {
			MidiInputList.push_back( string(  info->name )  );
		}
	 }
}

void NxMidiManager::GetMidiOutputList( std::vector<std::string> & MidiOutputList )
{
	 for (int i = 0; i < Pm_CountDevices(); i++)  {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
		if( info->output ) {
			MidiOutputList.push_back( string( info->name ) );
		}
	 }
}

void NxMidiManager::ListMidiInputs()
{
	Midi_Inputs.clear();
	for (int i = 0; i < Pm_CountDevices(); i++)  {
		const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
		if ( info->input )  {
			Midi_Inputs.insert( make_pair( i, string(  info->name ) ) );
		}
	}
}

void NxMidiManager::ListMidiOutputs()
{
	Midi_Outputs.clear();
	for (int i = 0; i < Pm_CountDevices(); i++) {
		const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
		if( info->output ) {
			Midi_Outputs.insert( make_pair( i , string(  info->name ) ) );
		}
	}
}

 

}