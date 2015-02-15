#include "NxNetworkPreCompiled.h"

namespace NxNetwork {

	template<> NxNetworkLog * Singleton<NxNetworkLog>::msSingleton = 0;

	NxNetworkLog::NxNetworkLog()
	{
		std::string logPath = "";
#ifdef __ANDROID__
		logPath = "/sdcard/NxNetwork.log";
#else
		logPath = "NxNetwork.log";
#endif
		this->text = "";
		logger.open ( logPath.c_str(), std::ios::out | std::ios::ate);
	}

	NxNetworkLog::~NxNetworkLog()
	{
		logger.close();
	}

	void NxNetworkLog::LogMessage(std::string x)
	{
		struct tm *pTime;
		time_t ctTime; time(&ctTime);
		pTime = localtime( &ctTime );

		this->text += "[";
		this->text +=  NxNetworkUtils::ToString( pTime->tm_hour );
		this->text += ":";
		this->text +=  NxNetworkUtils::ToString( pTime->tm_min );
		this->text += ":";
		this->text +=  NxNetworkUtils::ToString( pTime->tm_sec );
		this->text += "]: NxNetwork : " + x + "\n";
		logger << this->text ; 
		logger.flush();
		this->text = "";
	}

	NxNetworkLog& NxNetworkLog::getSingleton(void)
	{
		assert( msSingleton );  return ( *msSingleton );
	}


}//namespace
