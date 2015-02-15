#include "NxGraphicsPreCompiled.h"

#define numElementsInArray(a)   ((int) (sizeof (a) / sizeof ((a)[0])))

namespace Nx {

NxGuiColours::NxGuiColours()
{
	static const int standardColours[] =
	{
		NxButton::buttonColourId,                 0xffbbbbff,
		NxButton::buttonOnColourId,               0xff4444ff,
		NxButton::buttonClickedColourId,		  0xff00007f,
		NxButton::buttontextColourId,             0xffffffff,

		NxLabel::backgroundColourId,			  0x00ffffff,
		NxLabel::labeltextColourId,				  0xffffffff,

		NxPanel::panelColourId,					  0xff00007f,
		NxPanel::panelHeaderColourId,			0xff00007f,
		NxPanel::panelHeaderTextColourId,		0xff00007f,

		NxSlider::sliderBackgroundColourId,		0xffff007f,   
		NxSlider::sliderColourId,				0xff00007f,
		NxSlider::sliderTextColourId,			0x80ffffff


	};

	for (int i = 0; i < numElementsInArray (standardColours); i += 2)
		SetColour (standardColours [i],  (standardColours [i + 1]) );

}

NxGuiColours::~NxGuiColours()
{

}

NxColourValue NxGuiColours::Unpack( unsigned int ColourVal )
{
	Ogre::ColourValue Colour;
	Ogre::PixelUtil::unpackColour( &Colour ,  PF_BYTE_RGBA,  &ColourVal );

	NxColourValue ret( Colour.r, Colour.g, Colour.b, Colour.a );

	return ret;
}

unsigned int NxGuiColours::Pack( NxColourValue Val )
{
	unsigned int ColourVal;
	Ogre::PixelUtil::packColour( Ogre::ColourValue( Val.r, Val.g, Val.b, Val.a ),  PF_BYTE_RGBA,  &ColourVal );
	return ColourVal;
}

const NxColourValue NxGuiColours::FindColour( const int colourId )
{
	std::vector<int>::iterator it = std::find( colourIds.begin(), colourIds.end(), colourId );
	if( it != colourIds.end() ) { // found.
		const int index = std::distance( colourIds.begin(), it );
		return colours [index];
	}
	return NxColourValue::Black;
}

void NxGuiColours::SetColour(const int colourId, unsigned int colour) 
{
	std::vector<int>::iterator it = std::find( colourIds.begin(), colourIds.end(), colourId );
	NxColourValue Val = Unpack( colour );
	if( it != colourIds.end() ) { //found
		const int index = std::distance( colourIds.begin(), it );
		colours[index] = Val;	 
	}
		
	colourIds.push_back(colourId);
	colours.push_back( Val );
}

}