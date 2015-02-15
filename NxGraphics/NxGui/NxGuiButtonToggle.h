#ifndef __NXGUIBUTTONTOGGLE_H__
#define __NXGUIBUTTONTOGGLE_H__

#include "NxGuiButton.h"

namespace Nx {

class NxGraphics_Export NxButtonToggle : public NxButton
{
public :
	NxButtonToggle( const std::string & ButtonText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxButtonToggle();
	void ColourChanged();

};

}


#endif