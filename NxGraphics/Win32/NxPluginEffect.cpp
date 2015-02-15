#include "NxGraphicsPreCompiled.h"

namespace Nx {

class NxPluginStruct {
public :
	NxPluginStruct() : m_hDLL(0) {


	}

	~NxPluginStruct() {


	}

	HMODULE m_hDLL;
};
	
	


NxPlugin::NxPlugin() :  m_pfnGetPluginVersion(0), m_pfnRegisterPlugin(0), m_pDLLRefCount(0)
{
	Initialized = false;
	Muted = false;  
	PluginName = "NoNamePlugin";
	PluginPath = "NoPath";

	mHelper = new NxPluginStruct();

}

NxPlugin::~NxPlugin()
{
	delete mHelper;

}

NxPlugin::NxPlugin(const NxPlugin &Other) : mHelper(Other.mHelper), m_pfnGetPluginVersion(Other.m_pfnGetPluginVersion), m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin),
m_pDLLRefCount(Other.m_pDLLRefCount) 
{
	++*m_pDLLRefCount;// Increase DLL reference counter
}

bool NxPlugin::Load( const std::string & Filename )
{
	mHelper->m_hDLL = ::LoadLibrary(Filename.c_str());// Try to load the plugin as a dynamic library
	if(!mHelper->m_hDLL) return false;

	try // Locate the plugin's exported functions  
	{
		m_pfnGetPluginName = reinterpret_cast<fnGetPluginName *>( ::GetProcAddress( mHelper->m_hDLL, "GetPluginName" ) );
		if(!m_pfnGetPluginName ) throw runtime_error(string("'") + Filename + "' GetPluginName Failed");

		m_pfnGetPluginVersion = reinterpret_cast<fnGetPluginVersion *>( ::GetProcAddress( mHelper->m_hDLL, "GetPluginVersion" ) );
		if(!m_pfnGetPluginVersion ) throw runtime_error(string("'") + Filename + "' GetPluginVersion Failed");

		m_pfnGetPluginType = reinterpret_cast<fnGetPluginType *>( ::GetProcAddress( mHelper->m_hDLL, "GetPluginType" ) );
		if(!m_pfnGetPluginType ) throw runtime_error(string("'") + Filename + "' GetPluginType Failed");

		m_pfnGetPluginSubType = reinterpret_cast<fnGetPluginSubType *>( ::GetProcAddress( mHelper->m_hDLL, "GetPluginSubType" ) );
		if(!m_pfnGetPluginSubType ) throw runtime_error(string("'") + Filename + "' GetPluginSubType Failed");

		m_pfnRegisterPlugin = reinterpret_cast<fnRegisterPlugin *>( ::GetProcAddress(mHelper->m_hDLL, "registerPlugin" ) );
		if(!m_pfnRegisterPlugin ) throw runtime_error(string("'") + Filename + "' registerPlugin Failed");

		PluginName = std::string( m_pfnGetPluginName() ); 
		PluginPath = Filename;

		// Initialize a new DLL reference counter
		m_pDLLRefCount = new size_t(1);
	}
	catch(...)
	{
		::FreeLibrary(mHelper->m_hDLL);
		return false;
	}

	return true;
}

void NxPlugin::Unload()
{
	if( mHelper->m_hDLL )
	{
		if( !FreeLibrary( mHelper->m_hDLL )) LogMsg( "Cant free Library." );
	}
}

NxGraphics_Plugin_Effect_Types NxPlugin::GetPluginType()
{
	return (NxGraphics_Plugin_Effect_Types)m_pfnGetPluginType() ;
}

NxGraphics_Plugin_Effect_SubTypes NxPlugin::GetPluginSubType()
{
	return (NxGraphics_Plugin_Effect_SubTypes)m_pfnGetPluginSubType() ;
}

std::string NxPlugin::GetPluginName() 
{
	return PluginName  ; 
}

std::string NxPlugin::GetPluginPath()
{
	return PluginPath ;
}

bool NxPlugin::Initialize()
{
	return true ;
}

bool NxPlugin::DeInitialize()
{
	return true ;
}

bool NxPlugin::SetMute( bool mute )
{
	Muted = mute ;
	return true ;
}

bool NxPlugin::IsMuted()
{
	return Muted ;
}
	 
void NxPlugin::SetAssociatedLayerNumber( int layer )
{
	AssociatedLayerNumber = layer ;
}

int NxPlugin::GetAssociatedLayerNumber()
{
	return AssociatedLayerNumber;
}

size_t NxPlugin::GetNumParams()
{
	return ParamList.size();
}

void NxPlugin::GetParameterList( std::vector< NxPluginParam * > & ParamListInst )
{
	ParamListInst.resize( ParamList.size() );
	std::copy(ParamList.begin(),ParamList.end(),ParamListInst.begin());
}

const NxPluginParam & NxPlugin::GetParam( int ParamIndex ) const
{
	return *ParamList.at( ParamIndex );
}

int NxPlugin::GetIndexParam( NxPluginParam * parameter )
{
	int Index = -1 ;
	for( int i = 0; i < ParamList.size(); i++  ) 
	{
		NxPluginParam  * ptr = ParamList[i];
		if( ptr == parameter )
		{
			return Index;
		}
		Index++;
	}
}

bool NxPlugin::IsInitialized()
{
	return Initialized ;
}

void NxPlugin::ProcessComboBox( NxPluginParam * Parameter, std::string ParamName )
{

}

void NxPlugin::ProcessButton( NxPluginParam * Parameter, bool Toggled , void * Data )
{

}

void NxPlugin::ProcessSlider( NxPluginParam * Parameter, double Value )
{

}

float NxPlugin::GetPluginVersion() const
{
	return m_pfnGetPluginVersion();
}

}//namespace