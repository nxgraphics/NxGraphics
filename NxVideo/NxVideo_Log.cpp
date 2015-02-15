#include "NxVideoPreCompiled.h"

namespace NxVideo_Namespace {

	template<> NxVideoLog* Singleton<NxVideoLog>::msSingleton = 0;

	NxVideoLog::NxVideoLog()
	{
		std::string logPath = "";
#ifdef __ANDROID__
		logPath = "/sdcard/NxVideo.log";
#else
		logPath = "NxVideo.log";
#endif

		this->text = "";
		logger.open ( logPath.c_str() , ios::out | ios::ate);
	}

	NxVideoLog::~NxVideoLog()
	{
		logger.close();
	}

	void NxVideoLog::LogMessage(string x)
	{
		struct tm *pTime;
		time_t ctTime; time(&ctTime);
		pTime = localtime( &ctTime );

		this->text += "[";
		this->text +=  NxVideoUtils::ToString( pTime->tm_hour );
		this->text += ":";
		this->text +=  NxVideoUtils::ToString( pTime->tm_min );
		this->text += ":";
		this->text +=  NxVideoUtils::ToString( pTime->tm_sec );
		this->text += "]: NxVideo : " + x + "\n";
		logger << this->text ; 
		logger.flush();
		this->text = "";
	}

	NxVideoLog& NxVideoLog::getSingleton(void)
	{
		assert( msSingleton );  return ( *msSingleton );
	}


}//namespace
