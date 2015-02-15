#include "NxGraphicsPreCompiled.h"

namespace Nx {

class NxPluginDescriptionStruct {
public :
	NxPluginDescriptionStruct(){

	}
	~NxPluginDescriptionStruct()
	{


	}

HMODULE m_hDLL;


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
	// Try to load the plugin as a dynamic library
	mHelper->m_hDLL = ::LoadLibrary( Path.c_str() );
	if(!mHelper->m_hDLL) throw runtime_error(string("Could not load '") + Path + "'");

	// Locate the plugin's exported functions  
	try 
	{
		m_pfnGetPluginName = reinterpret_cast< GetPluginName * >( ::GetProcAddress( mHelper->m_hDLL, "GetPluginName" ) );
		if(!m_pfnGetPluginName ) throw runtime_error(string("'") + Path + "' GetPluginName Failed");

		m_pfnGetPluginVersion = reinterpret_cast< GetPluginVersion * >( ::GetProcAddress( mHelper->m_hDLL, "GetPluginVersion" ) );
		if(!m_pfnGetPluginVersion ) throw runtime_error(string("'") + Path + "' GetPluginVersion Failed");

		m_pfnGetPluginType = reinterpret_cast< GetPluginType * >( ::GetProcAddress( mHelper->m_hDLL, "GetPluginType" ) );
		if(!m_pfnGetPluginType ) throw runtime_error(string("'") + Path + "' GetPluginType Failed");

		m_pfnGetPluginSubType = reinterpret_cast< GetPluginSubType * >( ::GetProcAddress( mHelper->m_hDLL, "GetPluginSubType" ) );
		if(!m_pfnGetPluginSubType ) throw runtime_error(string("'") + Path + "' GetPluginSubType Failed");

		m_pfnRegisterPlugin = reinterpret_cast< RegisterPlugin * >( ::GetProcAddress( mHelper->m_hDLL, "registerPlugin" ) );
		if(!m_pfnRegisterPlugin ) throw runtime_error(string("'") + Path + "' registerPlugin Failed");

		// Initialize a new DLL reference counter
		m_pDLLRefCount = new size_t(1);

		mPluginName = std::string( m_pfnGetPluginName() ); 
		mPluginPath = Path;
		mPluginType = (NxGraphics_Plugin_Effect_Types) m_pfnGetPluginType();
		mPluginSubType = (NxGraphics_Plugin_Effect_SubTypes) m_pfnGetPluginSubType();
		mVersion = m_pfnGetPluginVersion();

	}
	catch(...)
	{
		throw runtime_error(string("'") + Path + "' cant load plugin !");
		::FreeLibrary(mHelper->m_hDLL);
		return ;
	}
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