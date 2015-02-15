#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxButtonRadio::NxButtonRadio( const std::string & ButtonText, int groupId, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager )
: NxButton( ButtonText ,PosX ,PosY, Width, Height, Manager )
{
	setClickingTogglesState (true);
	setRadioGroupId( groupId );
}

NxButtonRadio::~NxButtonRadio()
{

}

void NxButtonRadio::ColourChanged()
{
	NxButton::ColourChanged();
}



}