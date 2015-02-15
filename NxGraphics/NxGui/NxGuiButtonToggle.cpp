#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxButtonToggle::NxButtonToggle( const std::string & ButtonText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager )
: NxButton( ButtonText ,PosX ,PosY, Width, Height, Manager )
{
	setClickingTogglesState (true);
}

NxButtonToggle::~NxButtonToggle()
{

}

void NxButtonToggle::ColourChanged()
{
	NxButton::ColourChanged();
}

}