#include "NxDevicePreCompiled.h"

namespace NxDevice_Namespace {

template<> NxDeviceLog * Singleton<NxDeviceLog>::msSingleton = 0;

NxDeviceLog::NxDeviceLog()
{
	std::string NxMediaPath = "NxDevice.log";
	this->text = "";
	logger.open ( NxMediaPath.c_str() , ios::out | ios::ate);
}

NxDeviceLog::~NxDeviceLog()
{
	logger.close();
}
 
void NxDeviceLog::LogMessage(string x)
{
	struct tm *pTime;
	time_t ctTime; time(&ctTime);
	pTime = localtime( &ctTime );

	this->text += "[";
	this->text +=  NxDeviceUtils::ToString( pTime->tm_hour );
	this->text += ":";
	this->text +=  NxDeviceUtils::ToString( pTime->tm_min );
	this->text += ":";
	this->text +=  NxDeviceUtils::ToString( pTime->tm_sec );
	this->text += "]: NxDevice : " + x + "\n";
	logger << this->text ; 
	logger.flush();
	this->text = "";
}

NxDeviceLog& NxDeviceLog::getSingleton(void)
{
	assert( msSingleton );  return ( *msSingleton );
}


}//namespace