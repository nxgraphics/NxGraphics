#include "NxGraphicsPreCompiled.h"

namespace Nx {

template<> NxLog* NxSingleton<NxLog>::ms_Singleton = 0;

NxLog::NxLog()
{
 
	 
}

NxLog::~NxLog()
{

}

NxLog * NxLog::getSingletonPtr(void)
{
	return ms_Singleton;
}

NxLog& NxLog::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

void NxLog::LogMessage( const std::string& message )
{
	Ogre::LogManager::getSingleton().getDefaultLog()->logMessage( message );

}

 


}//namespace