#ifndef __NXGUIPANEL_H__
#define __NXGUIPANEL_H__

#include "NxGuiWidget.h"

namespace Nx {

class NxGraphics_Export NxPanel : public NxWidget
{
public :
	NxPanel( const std::string & HeaderText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxPanel();
	void SetHeaderHeight( float height );
	const float GetHeaderHeight();
	void SetHeaderTextHeight( float height );
	void SetHeaderText( const std::string & text );
	void Update( float Time ) ;
	void RePaint();
	void SetPosition( float PosX, float PosY );
	void SetSize( float Width, float Height );
	void SetVisible( bool Visible, bool AffectChildren = true  );
	void SetHeaderVisible( bool Visible );
	bool IsHeaderVisible();
	

	NxButton * CreateButton( const std::string & ButtonText, float PosX, float PosY, float Width, float Height );
	NxButtonToggle * CreateButtonToggle( const std::string & ButtonText, float PosX, float PosY, float Width, float Height );
	NxButtonRadio * CreateButtonRadio( const std::string & ButtonText, int groupId, float PosX, float PosY, float Width, float Height );
	NxSlider * CreateSlider( float PosX, float PosY, float Width, float Height );
	NxLabel * CreateLabel( const std::string & Text, float PosX, float PosY, float Width, float Height );

	//! Set Visibility flags for Viewport Rendering.
	void SetVisibilityFlags( int Flags );
	//! Set Z Render Order.
	void SetZOrder( unsigned short ZOrder );

	void OnViewportResized( float ScreenWidth, float ScreenHeight );

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button) ;
	bool mousePressed(int x, int y, int button) ;
	void mouseReleased(int x, int y, int button) ;
	void keyPressed(int key) ;
	void keyReleased(int key) ;


    enum NxColourIds 
    {
        panelColourId			= 0x2000100,
		panelHeaderColourId		= 0x2000101,
		panelHeaderTextColourId	= 0x2000102
 
    };

	 void ColourChanged();
private :
	NxRectangle * mHeader;
	float mHeaderHeight;
	NxText * mHeaderText;
	bool mHeaderVisible;
};

}

#endif