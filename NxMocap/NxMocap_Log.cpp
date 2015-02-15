#include "NxMocapPreCompiled.h"

namespace NxMocap_Namespace {

template<> NxMocapLog * Singleton<NxMocapLog>::ms_Singleton = 0;

NxMocapLog::NxMocapLog()
{
	std::string NxMediaPath = "NxMocap.log";
	this->text = "";
	logger.open ( NxMediaPath.c_str() , ios::out | ios::ate);
}

NxMocapLog::~NxMocapLog()
{
	logger.close();
}
 
void NxMocapLog::LogMessage(string x)
{
	struct tm *pTime;
	time_t ctTime; time(&ctTime);
	pTime = localtime( &ctTime );

	this->text += "[";
	this->text +=  NxMocapUtils::ToString( pTime->tm_hour );
	this->text += ":";
	this->text +=  NxMocapUtils::ToString( pTime->tm_min );
	this->text += ":";
	this->text +=  NxMocapUtils::ToString( pTime->tm_sec );
	this->text += "]: NxMocap : " + x + "\n";
	logger << this->text ; 
	logger.flush();
	this->text = "";
}

NxMocapLog& NxMocapLog::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}


}//namespace
