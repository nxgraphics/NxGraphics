#include "NxDevicePreCompiled.h"

namespace NxDevice_Namespace {

NxDevice_Hardware::NxDevice_Hardware() : DeviceID(-1) 
{
	Thread_Should_Run = false ;
}  

NxDevice_Hardware::~NxDevice_Hardware()
{

}

bool NxDevice_Hardware::loadPropertySheet()
{
	return true ;
}

bool NxDevice_Hardware::SavePropertySheet()
{
	return true ;
}

bool NxDevice_Hardware::ConnectDevice()
{
	return true ;
}

bool NxDevice_Hardware::DisconnectDevice()
{
	return true ;
}

void NxDevice_Hardware::SetDeviceIndex( int Index )
{
	mDeviceIndex = Index;
}

int NxDevice_Hardware::GetDeviceIndex()
{
	return mDeviceIndex;
}
 
void NxDevice_Hardware::GenerateID()
{
	DeviceID =  NxDeviceManager::getSingleton().GenerateId();
}

unsigned long NxDevice_Hardware::GetID()
{
	return DeviceID ;
}

void NxDevice_Hardware::SetID( unsigned long Id )
{
	DeviceID = Id ;
}
 
bool NxDevice_Hardware::StartListening( int port )
{
	return false; 
}

bool NxDevice_Hardware::StopListening()
{
	return false;
}

bool NxDevice_Hardware::IsListening()
{
	return Listening ;
}
 
NxDevice_Hardware_Types NxDevice_Hardware::GetHardwareDeviceType()
{
	return mDeviceType ;  
}

void NxDevice_Hardware::SetHardwareDeviceType( NxDevice_Hardware_Types Type)
{
	mDeviceType = Type ;
}

NxDevice_Hardware_SubTypes NxDevice_Hardware::GetHardwareDeviceSubType()
{
	return mDeviceSubType;
}

void NxDevice_Hardware::SetHardwareDeviceSubType( NxDevice_Hardware_SubTypes SubType )
{
	mDeviceSubType = SubType ;
}

std::string NxDevice_Hardware::GetHardwareDeviceName()
{
	return mDeviceName ;
}

void NxDevice_Hardware::SetHardwareDeviceName( std::string Name )
{
	mDeviceName = Name ;
}

void NxDevice_Hardware::BuildParamDictionary()
{

}

NxDevice_Hardware::DeviceHardwareParamType NxDevice_Hardware::GetParameterDictionary()
{
	return DeviceHardwareParamList ;
}

std::list<string>  NxDevice_Hardware::GetParameterDictionaryPageList()
{
	std::list< string> PageList ;
	for( DeviceHardwareParamType::iterator iter =  DeviceHardwareParamList.begin(); iter !=  DeviceHardwareParamList.end(); iter++)
	{
		string tmp = iter->first ;
		PageList.push_back( tmp  );
	}
	return PageList ;
}

std::list< DeviceHardwareParam *> NxDevice_Hardware::GetParameterDictionaryPage( string Page  )
{
	std::list< DeviceHardwareParam *> ParamList ;
	for( DeviceHardwareParamType::iterator iter =  DeviceHardwareParamList.begin(); iter !=  DeviceHardwareParamList.end(); iter++)
	{
		if(Page == iter->first )
		{
			ParamList = iter->second ;
		}
	}
	return ParamList ;
}

DeviceHardwareParam * NxDevice_Hardware::GetParameterDictionaryParameter( string DictionaryName ,  long Index )
{
	std::list< DeviceHardwareParam *> param =  GetParameterDictionaryPage( DictionaryName ) ;
	list< DeviceHardwareParam * >::iterator i = param.begin();
	std::advance(i, Index );
	// now index points to the nth member of the list (counting from zero)
	return   *i ;
}

}//namespace