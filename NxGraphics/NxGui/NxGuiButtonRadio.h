#ifndef __NXGUIBUTTONRADIO_H__
#define __NXGUIBUTTONRADIO_H__

#include "NxGuiButton.h"

namespace Nx {

class NxGraphics_Export NxButtonRadio : public NxButton
{
public :
	NxButtonRadio( const std::string & ButtonText, int groupId, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxButtonRadio();
	void ColourChanged();



};

}


#endif