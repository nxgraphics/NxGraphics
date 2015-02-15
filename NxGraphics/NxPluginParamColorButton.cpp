#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxPluginParamColorButton::NxPluginParamColorButton( std::string PluginParamName ): NxPluginParam( PluginParamName, PluginParameterTypeColour ), mDefaultColourParam( NxColourValue::Black )
{
	
}

NxPluginParamColorButton::~NxPluginParamColorButton()
{

}

void NxPluginParamColorButton::SetDefaultColor( NxColourValue Col )
{
	mDefaultColourParam = Col ;
}

NxColourValue NxPluginParamColorButton::GetDefaultColor()
{
	return mDefaultColourParam ;
}


}