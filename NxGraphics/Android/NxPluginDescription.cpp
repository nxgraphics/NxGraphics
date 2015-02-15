#include "NxGraphicsPreCompiled.h"

namespace Nx {

class NxPluginDescriptionStruct {
public :
	NxPluginDescriptionStruct(){

	}
	~NxPluginDescriptionStruct()
	{


	}

 


};

	

	//! Dll Instance.
	//

NxPluginDescription::NxPluginDescription()
{

	mHelper = new NxPluginDescriptionStruct(); 

}

NxPluginDescription::NxPluginDescription( std::string PluginName, std::string PluginPath, NxGraphics_Plugin_Effect_Types Type, NxGraphics_Plugin_Effect_SubTypes SubType ):
mPluginName(PluginName), mPluginPath(PluginPath), mPluginType(Type), mPluginSubType(SubType)
{

}

NxPluginDescription::~NxPluginDescription()
{
	delete mHelper;

}

void NxPluginDescription::LoadInformation( std::string Path )
{
	 
 
}

void NxPluginDescription::Register( NxPluginManager & PlugManager )
{
	m_pfnRegisterPlugin( PlugManager );	
}

const std::string NxPluginDescription::GetPath() const
{
	return mPluginPath;
}

const std::string NxPluginDescription::GetName() const
{
	return mPluginName;
}

const NxGraphics_Plugin_Effect_Types NxPluginDescription::GetType() const
{
	return mPluginType;
}

const NxGraphics_Plugin_Effect_SubTypes NxPluginDescription::GetSubType() const
{
	return mPluginSubType;
}


}