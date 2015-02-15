#ifndef __NXGUICOLOURS_H__
#define __NXGUICOLOURS_H__

#include <NxPrerequisites.h>

namespace Nx {

class NxGraphics_Export NxGuiColours
{
public :
	NxGuiColours();
	~NxGuiColours();
	const NxColourValue FindColour (const int colourId) ;
    void SetColour (const int colourId, unsigned int colour) ;
	NxColourValue Unpack( unsigned int ColourVal );
	unsigned int Pack( NxColourValue Val );
private :
	std::vector <int> colourIds;
	std::vector <NxColourValue > colours;
};

}


#endif