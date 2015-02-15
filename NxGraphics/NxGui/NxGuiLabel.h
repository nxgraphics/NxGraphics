#ifndef __NXGUILABEL_H__
#define __NXGUILABEL_H__

#include "NxGuiWidget.h"

namespace Nx {

class NxGraphics_Export NxLabel : public NxWidget
{
public :
	NxLabel( const std::string & Text, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxLabel();
	void SetTextPixelsHeight( float Height );
	void SetTextPixelsSpacing( float Spacing );
	//void SetTextColour( const Ogre::ColourValue & Colour );
	void ColourChanged();
	void Update( float Time ) ;
	void RePaint();
	void SetVisible( bool Visible, bool AffectChildren = true  );
	void OnViewportResized( float ScreenWidth, float ScreenHeight );
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button) ;
	bool mousePressed(int x, int y, int button) ;
	void mouseReleased(int x, int y, int button) ;
	void keyPressed(int key) ;
	void keyReleased(int key);

	enum NxColourIds 
	{
		backgroundColourId			= 0x1000200,
		labeltextColourId			= 0x1000201
	};

	void SetVerticalAlignment( NxTextVerticalAlignment VAlign );
	void SetHorizontalAlignment( NxTextHorizontalAlignment HAlign );
private :
	void stateChange();
	NxText * mText;
};

}

#endif