#include "NxDevicePreCompiled.h"

namespace NxDevice_Namespace {

template<> NxDeviceManager* Singleton<NxDeviceManager>::msSingleton = 0;

NxDeviceManager::NxDeviceManager() 
{
	//IdGenerator  = new MTRand();

	NxDeviceLog * Logger = new NxDeviceLog();

	mMidiManager = new NxMidiManager();
	

	Log("Started NxDevice.");
} 

NxDeviceManager::~NxDeviceManager()
{

}

NxDeviceManager& NxDeviceManager::getSingleton(void)
{
	assert( msSingleton );  return ( *msSingleton );
}

NxDeviceManager* NxDeviceManager::getSingletonPtr(void)
{
	return msSingleton;
}

NxMidiManager * NxDeviceManager::GetMidiManager()
{
	return mMidiManager;
}

unsigned long NxDeviceManager::GenerateId()
{
	static int Index = 0;
	return Index++;//IdGenerator->randInt();
}

void NxDeviceManager::AddListener( NxDeviceManagerListener * listener )
{
	 Listeners_List.push_back(listener); 
}

void NxDeviceManager::RemoveListener( NxDeviceManagerListener * listener )
{
	Listeners_List.remove(listener); 
}

NxDevice_Hardware * NxDeviceManager::CreateDevice( const NxDeviceHardwareDesc& hardwareDesc )
{
	switch( hardwareDesc.DevType )
	{
		case NXDEVICE_OSC :
		{
			NxDeviceOsc * Device = new NxDeviceOsc();
			if(!Device->ConnectDevice()) return 0;
			Device_List.push_back(Device);
			return Device ;
		}
		break;

		case NXDEVICE_MIDI_CONTROLLER :
		{
			switch( hardwareDesc.DevSubType )
			{
				case NXDEVICE_SUB_MIDI_IN :
				{
					NxDeviceMidi * Device = new NxDeviceMidi();
					if(!Device->InitializeMidiInput( std::string( hardwareDesc.DevName )) )
					{ 
						Log("Midi Input Device Failed !");
						delete Device;
						return 0;
					}
					Device->SetHardwareDeviceName( hardwareDesc.DevName );
					Device_List.push_back(Device);
					return Device;
				}
				break ;
				case NXDEVICE_SUB_MIDI_OUT :
				{
					NxDeviceMidi * Device = new NxDeviceMidi();
					if(!Device->InitializeMidiOutput( hardwareDesc.DevName ))
					{
						Log("Midi Output Device Failed !");
						delete Device;
						return 0;
					}
					Device->SetHardwareDeviceName( hardwareDesc.DevName );
					Device_List.push_back(Device);
					return Device;
				}
				break;

				case NXDEVICE_SUB_MIDI_THRU:
				break;

				default : 

				break ;
			}
		}
		break;
		//case NXDEVICE_WIIMOTE :
		//{
		//	NxDeviceWiimote * Device = new NxDeviceWiimote();
		//	Device->ConnectDevice();
		//	Device->SetHardwareDeviceName( "Wiimote" );
		//	Device_List.push_back(Device);
		//	return Device;
		//}
		//break;
		//case NXDEVICE_LASER :
		//{
		//	NxDeviceLaserKvant * Device = new NxDeviceLaserKvant();
		//	Device->ConnectDevice();
		//	Device->SetHardwareDeviceName( "Laser" );
		//	Device_List.push_back(Device);
		//	return Device;
		//}
		//break;
		//case NXDEVICE_3DCONNEXION :
		//{
		//	Nx3DConnexion * Device = new Nx3DConnexion();
		//	if( !Device->ConnectDevice()){ return 0;  }
		//	Device_List.push_back(Device);
		//	return Device ;
		//}
		//break;

#ifdef USE_DMX 
		case NXDEVICE_DMX :
		{
			Log("Loading dmx module..."); // load delay dlls if in path
			NxDeviceDmx * Device = new NxDeviceDmx();

			if( !Device ) Log("Loading dmx module. Failed"); // load delay dlls if in path
			//DMX_tmp->SetDeviceIndex( device_index );
			if(!Device->ConnectDevice())
			{ 
				Log("Dmx Module Failed !");
				Device->DisconnectDevice();
				delete Device;
				return 0;
			}
			Log("Dmx Module Connnected !");
			Device_List.push_back(Device);
			return Device ;
		}
		break;
#endif

		default :
		break;
		
	}//switch
 
}

bool NxDeviceManager::DeleteDevices()
{
	for(std::vector<NxDevice_Hardware *>::reverse_iterator It = Device_List.rbegin();It != Device_List.rend(); ++It){
		NxDevice_Hardware * Device = (*It);
		Device->DisconnectDevice();
		delete *It;
	}

	Device_List.clear();
	return true ;
}

bool NxDeviceManager::DeleteDevice( NxDevice_Hardware * Device )
{
	std::vector<NxDevice_Hardware *>::iterator it;
	for(it=Device_List.end(); it!=Device_List.begin(); ){
		--it;
 
		if( (*it) == Device ){
			(*it)->DisconnectDevice();
			delete *it;
			it=Device_List.erase(it);
		}	
	}

	return true;
}

void NxDeviceManager::GetDeviceList( std::vector<NxDevice_Hardware *> & List )
{
	List.resize(Device_List.size());
	std::copy(Device_List.begin(),Device_List.end(),List.begin());
 
}
	
size_t NxDeviceManager::GetNumDevices()
{
	return Device_List.size();
}

NxDevice_Hardware * NxDeviceManager::GetDeviceFromID( unsigned long ID )
{
	for( std::vector<NxDevice_Hardware  *>::iterator iter = Device_List.begin(); iter !=  Device_List.end(); iter++)
	{
		NxDevice_Hardware  * Device = *iter; 
		if( Device->GetID() == ID )
		{
			return Device ;
		}
	}

	return NULL ;
}

//int NxDeviceManager::GetNumConnectedWiimotes()
//{
//	int NumWiimotes = 0;
//	for( std::vector<NxDevice_Hardware  *>::iterator iter = Device_List.begin(); iter !=  Device_List.end(); iter++)
//	{
//		NxDevice_Hardware * Device = *iter;
//		if( Device->GetHardwareDeviceType() == NxDevice_Hardware_Types::NXDEVICE_WIIMOTE ) NumWiimotes++;
//	}
//	return NumWiimotes ;
//}
//
//int NxDeviceManager::GetNumWiimotes()
//{
//	return NxDeviceWiimote::GetNumDevices();	 
//}

int NxDeviceManager::GetNumDmxDevices()
{
#ifdef USE_DMX
	return NxDeviceDmx::GetNumDevices();
#endif
	return 0;
}


} // namespace