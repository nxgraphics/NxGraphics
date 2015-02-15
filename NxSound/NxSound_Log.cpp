#include "NxSoundPreCompiled.h"
 
namespace NxSound_Namespace {

template<> NxSoundLog* Singleton<NxSoundLog>::msSingleton = 0;

NxSoundLog::NxSoundLog()
{
	std::string logPath = "";
	#ifdef __ANDROID__
		logPath = "/sdcard/NxSound.log";
	#else
		logPath = "NxSound.log";
	#endif

	this->text = "";
	logger.open ( logPath.c_str() , ios::out | ios::ate);
}

NxSoundLog::~NxSoundLog()
{
	logger.close();
}
 
void NxSoundLog::LogMessage(string x)
{
	struct tm *pTime;
	time_t ctTime; time(&ctTime);
	pTime = localtime( &ctTime );

	this->text += "[";
	this->text +=  NxSoundUtils::ToString( pTime->tm_hour );
	this->text += ":";
	this->text +=  NxSoundUtils::ToString( pTime->tm_min );
	this->text += ":";
	this->text +=  NxSoundUtils::ToString( pTime->tm_sec );
	this->text += "]: NxSound : " + x + "\n";
	logger << this->text ; 
	logger.flush();
	this->text = "";
}

NxSoundLog& NxSoundLog::getSingleton(void)
{
	assert( msSingleton );  return ( *msSingleton );
}


}//namespace
