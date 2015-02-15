#include "NxDevicePreCompiled.h"

#include "../nxdeps/include/PortMidi/pm_common/portmidi.h"
#include "../nxdeps/include/PortMidi/porttime/porttime.h"

namespace NxDevice_Namespace {

#define TIME_PROC ((long (*)(void *)) Pt_Time)
#define TIME_INFO NULL
#define TIME_START Pt_Start(1, 0, 0) /* timer started w/millisecond accuracy */

NxDeviceMidi::NxDeviceMidi(void) : midi(0)
{
	BuildParamDictionary();
	GenerateID();
		#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
	ThreadMidi = NULL;
#endif
}

NxDeviceMidi::~NxDeviceMidi(void)
{
	if( midi )
	{
		if( ThreadShouldRun )
		{
		ThreadShouldRun = false ;
		
		
#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
		DWORD dwRet = WaitForSingleObject( ThreadMidi, INFINITE );  

		if (dwRet == WAIT_TIMEOUT)
		{
		   DWORD dwForcefulExitCode = 1;        
		   ::TerminateThread( ThreadMidi , dwForcefulExitCode);
		} 

		ThreadMidi = NULL ;
#endif
		// close the midi port
		PmError  err =  Pm_Close(  midi );

		int pterr = Pt_Stop();
		if (pterr) 
		{
			switch( pterr )
			{
			case ptHostError:
				//Ogre::LogManager::getSingleton().logMessage("NxDevice : Pt_Stop ptHostError " );
				break ;
			case ptAlreadyStarted:
				//Ogre::LogManager::getSingleton().logMessage("NxDevice : Pt_Stop ptAlreadyStarted " );
				break ;
			case ptAlreadyStopped:
				//Ogre::LogManager::getSingleton().logMessage("NxDevice : Pt_Stop ptAlreadyStopped " );
				break ;
			case ptInsufficientMemory:
				//Ogre::LogManager::getSingleton().logMessage("NxDevice : Pt_Stop ptInsufficientMemory " );
				break ;
			default :
				break ;
			}
		}

		midi = NULL;

		}//if threadrunning

	}//if midi

}


bool NxDeviceMidi::loadPropertySheet()
{
	return true;
}

bool NxDeviceMidi::SavePropertySheet()
{
	return true;
}

bool NxDeviceMidi::ConnectDevice()
{
	return true;
}

bool NxDeviceMidi::DisconnectDevice()
{
	if( midi )
	{
		ThreadShouldRun = false ;
#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
		if ( ThreadMidi ) WaitForSingleObject( ThreadMidi, 2000);
#endif
		//Sleep(100);
		Pm_Close( midi ) ;
		midi = NULL ;
		return true ;
	}
	return false;
}

//! Type: Note On/Off/Control
void NxDeviceMidi::SendMidiMessage( int Type, int Channel, int Note, int Velocity )
{
/*
#define MIDI_CLOCK      0xf8
#define MIDI_ACTIVE     0xfe
#define MIDI_STATUS_MASK 0x80
#define MIDI_SYSEX      0xf0
#define MIDI_EOX        0xf7
#define MIDI_START      0xFA
#define MIDI_STOP       0xFC
#define MIDI_CONTINUE   0xFB
#define MIDI_F9         0xF9
#define MIDI_FD         0xFD
#define MIDI_RESET      0xFF
#define MIDI_NOTE_ON    0x90
#define MIDI_NOTE_OFF   0x80
#define MIDI_CHANNEL_AT 0xD0
#define MIDI_POLY_AT    0xA0
#define MIDI_PROGRAM    0xC0
#define MIDI_CONTROL    0xB0
#define MIDI_PITCHBEND  0xE0
#define MIDI_MTC        0xF1
#define MIDI_SONGPOS    0xF2
#define MIDI_SONGSEL    0xF3
#define MIDI_TUNE       0xF6
*/
    PmEvent buffer;
    PmTimestamp timestamp;
    buffer.timestamp = TIME_PROC(TIME_INFO);
    buffer.message = Pm_Message( Type, Note, Velocity);
    Pm_Write(midi, &buffer, 1);
}

void NxDeviceMidi::BuildParamDictionary()
{
	std::list<string> MidiNotes ;
	string Octave = " -1";

	for( int i = 0 ; i < ( 127 / 11 ) ; i++)
	{
		if(i == 1 ){ Octave = " -1"; }
		else if(i == 2 ){ Octave = " 0"; }
		else if(i == 3 ){ Octave = " 1"; }
		else if(i == 4 ){ Octave = " 2"; }
		else if(i == 5 ){ Octave = " 3"; }
		else if(i == 6 ){ Octave = " 4"; }
		else if(i == 7 ){ Octave = " 5"; }
		else if(i == 8 ){ Octave = " 6"; }
		else if(i == 9 ){ Octave = " 7"; }
		else if(i == 10 ){ Octave = " 8"; }
		else if(i == 11 ){ Octave = " 9"; }

		MidiNotes.push_back("C"+ Octave);
		MidiNotes.push_back("C#"+ Octave);
		MidiNotes.push_back("D"+ Octave);
		MidiNotes.push_back("D#"+ Octave);
		MidiNotes.push_back("E"+ Octave);
		MidiNotes.push_back("F"+ Octave);
		MidiNotes.push_back("F#"+ Octave);
		MidiNotes.push_back("G"+ Octave);


		if( i != 11)
		{
			MidiNotes.push_back("G#"+ Octave);
			MidiNotes.push_back("A"+ Octave);
			MidiNotes.push_back("A#"+ Octave);
			MidiNotes.push_back("B"+ Octave);
		}

	}

	int y = 0;
	//midi note events
	std::list< DeviceHardwareParam * > FirstParameters ;
	for( list<string>::iterator iter = MidiNotes.begin(); iter != MidiNotes.end(); iter++)
	{
		string str =  *iter; 
		//Ogre::LogManager::getSingleton().logMessage(str +" "+ Ogre::StringConverter::toString( y )  );
		DeviceHardwareMidiParam  * Param = new DeviceHardwareMidiParam();
		Param->ParamName = str ;
		Param->Datatype = 0;
		Param->MinValue = 0 ;
		Param->MaxValue = 127 ;
	
		Param->Command = 144 ;// note on
		Param->Channel = 0 ; // all channels
		Param->Note = y ; //MidiNotes.size() -  y ;
		Param->Velocity = 0;

		FirstParameters.push_back( Param );
		y++;
	}
	DeviceHardwareParamList.insert( make_pair( "Midi Notes" , FirstParameters )  );


	//midi control change events
	std::list< DeviceHardwareParam * > SecondParameters ;
	for(int i = 0 ; i < 127 ; i++)
	{
		DeviceHardwareMidiParam  * ParamMidiControls = new DeviceHardwareMidiParam();

		std::ostringstream os;
		os << i ; 
		std::string buffer(os.str());

		ParamMidiControls->ParamName = "controller " + buffer ;
		ParamMidiControls->Datatype = 0;
		ParamMidiControls->MinValue = 0 ;
		ParamMidiControls->MaxValue = 127 ;
	
		ParamMidiControls->Command = 176 ;// control change
		ParamMidiControls->Channel = 0 ; // all channels
		ParamMidiControls->Note = i ;
		ParamMidiControls->Velocity = 0;
		SecondParameters.push_back( ParamMidiControls );

	}

	DeviceHardwareParamList.insert( make_pair( "Midi Controls" , SecondParameters  )  );
}

	#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
DWORD NxDevice_Midi_Receive(LPVOID lpParameter)
{
	NxDeviceMidi * ptr = ( NxDeviceMidi * ) lpParameter ;
	PmEvent buffer[1];
	
	while( Pm_Poll(ptr->midi) )// empty the buffer if any old data
	{
		Pm_Read(ptr->midi, buffer, 1);
	}

	ptr->ThreadShouldRun = true ;
	while( ptr->ThreadShouldRun ) {

		PmEvent event;
		int count;
		if (ptr->midi == NULL) {
			Log("Midi invalid pointer");
			return 0;
		}

		PmError status = Pm_Poll(ptr->midi);
        if (status == TRUE) // if has a message
		{
           int length = Pm_Read( ptr->midi, buffer, 1 );//read it

			if (length > 0)
			{
				//get message type and channel number
				int type = Pm_MessageStatus(buffer[0].message) & 0xf0;
				int channel = Pm_MessageStatus(buffer[0].message) & 0x0f;

				for( list<NxDeviceManagerListener  *>::iterator iter =  NxDeviceManager::getSingleton().Listeners_List.begin(); iter !=  NxDeviceManager::getSingleton().Listeners_List.end(); iter++)
				{
					NxDeviceManagerListener  * listen = *iter; 
					listen->NxDeviceReceiveMidiCallback( ptr->GetID(),
					buffer[0].timestamp, type, channel, Pm_MessageData1( buffer[0].message ),
					Pm_MessageData2(buffer[0].message) );
				}
			} 
			else 
			{
				Log(" wrong midi message !");
			}

		}// status
		else if( status == pmInvalidDeviceId )
		{Log(" Device already opened");}
		else if( status == PmError::pmBadData )
		{Log(" PmError::pmBadData");}
		else if( status == PmError::pmBadPtr )
		{Log(" PmError::pmBadPtr");}
		else if( status == PmError::pmBufferTooSmall )
		{Log(" PmError::pmBufferTooSmall");}
		else if( status == PmError::pmInsufficientMemory  )
		{Log(" PmError::pmInsufficientMemory");}
		else if( status == PmError::pmInternalError   )
		{Log(" PmError::pmInternalError");}

	Sleep(1);
	}// while true

}
#endif

/*
void NxDeviceMidi::NxDevice_MIDI_Initialize_Engine()
{

}
*/

bool NxDeviceMidi::InitializeMidiOutput( const std::string & device_name  )
{
	mDeviceType = NXDEVICE_MIDI_CONTROLLER;
	mDeviceSubType = NXDEVICE_SUB_MIDI_OUT;
	Midi_Index = -1 ;
	Log("Listing Midi Output Devices.");
	for( NxMidiManager::Midi_Ouputs_List::iterator iter = NxDeviceManager::getSingleton().GetMidiManager()->Midi_Outputs.begin(); iter != NxDeviceManager::getSingleton().GetMidiManager()->Midi_Outputs.end(); iter++)
	{
		int iKey = iter->first;
		string mMidiDeviceName = iter->second;
		Log("Found Midi Out Device: " + mMidiDeviceName );

		if(  mMidiDeviceName == device_name ) 
		{
			Log("--> Found requested Midi Device : " + mMidiDeviceName  );
			Midi_Device_Name = mMidiDeviceName ;
			Midi_Index = iKey ;
			TIME_START;
			PmError err = Pm_OpenOutput( &midi, iKey, NULL, 512, NULL, NULL, NULL );
			if( err ){ Log(Pm_GetErrorText(err)); Pt_Stop(); return 0; }
			if( midi == NULL ){ Log("Error opening Midi Input Device"); return 0; }

			long filter = 0;    
			err = Pm_SetFilter( midi, filter );
			if (err){ printf(Pm_GetErrorText(err)); Pt_Stop(); return 0; }

			return true ;

		}//compare function
 
	}//for

	// check if device wasnt found
	if( Midi_Index == -1 )
	{
		//Ogre::LogManager::getSingleton().logMessage("NxDevice : Could not lookup Midi Device Input : " + Midi_Device_Name);
		cout << "Available Midi Device Inputs :\r\n" ;
		for( NxMidiManager::Midi_Inputs_List::iterator iter = NxDeviceManager::getSingleton().GetMidiManager()->Midi_Inputs.begin(); iter != NxDeviceManager::getSingleton().GetMidiManager()->Midi_Inputs.end(); iter++)
		{
			int iKey = iter->first;
			string itemValue = iter->second;
			cout << "Midi Device Input : " <<  iKey << "  Name :"  << itemValue  <<"\r\n" ;
		}
	}

	return false;
 
}

bool NxDeviceMidi::InitializeMidiInput( const std::string & device_name )
{
	mDeviceType = NXDEVICE_MIDI_CONTROLLER;
	mDeviceSubType = NXDEVICE_SUB_MIDI_IN;
	Midi_Index = -1 ;
	for( NxMidiManager::Midi_Inputs_List::iterator iter = NxDeviceManager::getSingleton().GetMidiManager()->Midi_Inputs.begin(); iter != NxDeviceManager::getSingleton().GetMidiManager()->Midi_Inputs.end(); iter++)
	{
		int iKey = iter->first;
		string mMidiDeviceName = iter->second;
		cout << "Found Midi In Device: " << mMidiDeviceName.c_str()  <<"\r\n" ;

		if(  mMidiDeviceName == device_name ) 
		{
			Midi_Device_Name = mMidiDeviceName ;
			Midi_Index = iKey ;
			TIME_START;
			PmError err = Pm_OpenInput( &midi, iKey, NULL, 512, NULL, NULL );
			if( err ){ Log(Pm_GetErrorText(err)); Pt_Stop(); return 0; }
			if( midi == NULL ){ Log("Error opening Midi Input Device"); return 0; }

			long filter = 0;    
			err = Pm_SetFilter( midi, filter );
			if (err){ printf(Pm_GetErrorText(err)); Pt_Stop(); return 0; }

	#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
			DWORD DMX_thread_id;
			ThreadMidi = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) NxDevice_Midi_Receive  , this , 0, &DMX_thread_id); //works fine
#endif
			return true ;

		}//compare function
 
	}//for

	// check if device wasnt found
	if( Midi_Index == -1 )
	{
		//Ogre::LogManager::getSingleton().logMessage("NxDevice : Could not lookup Midi Device Input : " + Midi_Device_Name);
		cout << "Available Midi Device Inputs :\r\n" ;
		for( NxMidiManager::Midi_Inputs_List::iterator iter = NxDeviceManager::getSingleton().GetMidiManager()->Midi_Inputs.begin(); iter != NxDeviceManager::getSingleton().GetMidiManager()->Midi_Inputs.end(); iter++)
		{
			int iKey = iter->first;
			string itemValue = iter->second;
			cout << "Midi Device Input : " <<  iKey << "  Name :"  << itemValue  <<"\r\n" ;
		}
	}

	return false;
}

 
} // namespace
