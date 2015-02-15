#include "NxGraphicsPreCompiled.h"

namespace Nx {

class NxPluginStruct {
public :
	NxPluginStruct()   {


	}

	~NxPluginStruct() {


	}

	 
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
	 

	return true;
}

void NxPlugin::Unload()
{
	 
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