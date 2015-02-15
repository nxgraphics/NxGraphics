#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxPluginParam::NxPluginParam( std::string PluginParamName, PluginParameterTypes ParameterType = PluginParameterTypeNone  )
: ValueIndex(0.5f), mValueDefault(0.0f), mValueMin(0.0f), mValueMax(1.0f), mParamName( PluginParamName ), mParamType( ParameterType )
{
	mParamIndex = -1;// invalid
 
}

NxPluginParam::~NxPluginParam()
{

}

const std::string NxPluginParam::GetParamName() const 
{
	return mParamName ;
}

PluginParameterTypes NxPluginParam::GetParameterType()
{
	return mParamType ;
}

float NxPluginParam::GetMin()
{
	return mValueMin ;
}

float NxPluginParam::GetMax()
{
	return mValueMax;
}

float NxPluginParam::GetValue()
{
	return ValueIndex ;
}

void NxPluginParam::SetValue( float val )
{
	ValueIndex = val ;
}

void NxPluginParam::SetDefaultValue( float val )
{
	mValueDefault = val ;
}

float NxPluginParam::GetDefaultValue()
{
	return mValueDefault ;
}

void NxPluginParam::SetHost( NxPlugin * Host )
{
	mPluginHost = Host ;
	mPluginType = mPluginHost->GetPluginType();
	mPluginSubType = mPluginHost->GetPluginSubType();
}

NxPlugin * NxPluginParam::GetHost()
{
	return mPluginHost;
}

void NxPluginParam::SetParamIndex( int Index )
{
	mParamIndex = Index;
}

int NxPluginParam::GetParamIndex()
{
	return mParamIndex;
}
	
void NxPluginParam::SetParamDescription( std::string DisplayText )
{
	mParamDescription = DisplayText;
}

std::string NxPluginParam::GetParamDescription()
{
	return mParamDescription;
}


 
}//namespace