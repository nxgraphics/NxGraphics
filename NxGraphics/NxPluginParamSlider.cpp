#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxPluginParamSlider::NxPluginParamSlider( std::string PluginParamName ): NxPluginParam( PluginParamName, PluginParameterTypeSlider ), mValueInbound(0.0f), mValueOutbound(0.0f)
{

}

NxPluginParamSlider::~NxPluginParamSlider()
{

}

void NxPluginParamSlider::SetRange( double MinVal, double MaxVal, double Increment )
{
	mValueMin = MinVal;
	mValueMax = MaxVal;
	mValueIncrement = Increment;
}

double NxPluginParamSlider::GetIncrementValue()
{
	return mValueIncrement ;
}

double NxPluginParamSlider::GetInboundValue()
{
	return mValueInbound;
}

double NxPluginParamSlider::GetOutboundValue()
{
	return mValueOutbound;
}


}