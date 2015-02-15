#include "NxGraphicsPreCompiled.h"

namespace Nx {
 

NxPluginVideoFreeframe::NxPluginVideoFreeframe() : NxPluginVideo()
{
	 
}

NxPluginVideoFreeframe::~NxPluginVideoFreeframe()
{

}

bool NxPluginVideoFreeframe::Initialize()
{
	return true;
}

bool NxPluginVideoFreeframe::DeInitialize()
{
	return true;
}

void NxPluginVideoFreeframe::SetVideoWidth( unsigned int )
{

}

void NxPluginVideoFreeframe::SetVideoHeight( unsigned int )
{

}

void NxPluginVideoFreeframe::SetVideoBpp( unsigned int )
{

}

bool NxPluginVideoFreeframe::Load( const std::string & Filename )
{
 

	return false;
}

unsigned long NxPluginVideoFreeframe::ProcessFrame( void * Buffer )
{
	return 0;
}

}//namespace