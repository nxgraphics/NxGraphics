#ifndef __NXGUIMANAGER_H__
#define __NXGUIMANAGER_H__

#include "NxGuiColours.h"

namespace Nx {

class NxGraphics_Export NxGuiManager 
{
public :
	//! NxGuiManager Constructor.
	NxGuiManager( NxContentManager * pSceneManager, NxViewport * pViewport );
	//! NxGuiManager Destructor.
	~NxGuiManager();
	//! Update Gui.
	void Update( float Time );
	//! Get Viewport Original Width.
	unsigned int GetViewportOriginalWidth();
	//! Get Viewport Original Height.
	unsigned int GetViewportOriginalHeight();
	//! Get Viewport  Width.
	unsigned int GetViewportWidth();
	//! Get Viewport  Height.
	unsigned int GetViewportHeight();
	//! Create a Panel.
	NxPanel * CreatePanel( const std::string & HeaderText, float PosX, float PosY, float Width, float Height );

	void AddPanel( NxPanel * panel );

	//! Set font for Gui.
	void SetFontName( const std::string & FontName );
	//! get current font associated with the Gui.
	const std::string & GetFontName() const;
	//! get default font size.
	float getDefaultFontSize(); 
	//! set default font size.
	void setDefaultFontSize( float size );
	//! Get Associated SceneManager.
	NxContentManager * GetContentManager();
	//! get viewport
	NxViewport * GetViewport();
	//! Viewport resize Callback.
	void OnViewportResized( unsigned int ScreenWidth, unsigned int ScreenHeight );
	//! Get Colour Manager.
	NxGuiColours * GetColours();
	//! Mouse Moved Callback.
	void mouseMoved( int x, int y );
	//! Mouse Dragged Callback.
	void mouseDragged( int x, int y, int button );
	//! Mouse Pressed Callback. return if a widegt was hit
	bool mousePressed( int x, int y, int button );
	//! Mouse Released Callback.
	void mouseReleased( int x, int y, int button );


	void touchPressed( int x, int y, int touchId );
	void touchReleased( int x, int y, int touchId );
	void touchMoved( int x, int y, int touchId );
	void touchCancelled( int x, int y, int touchId );


	void getMouseLastCoordinates( int &x, int &y );



	//! Show panels
	void Show();
	//! Hide panels
	void Hide();
private :
	//! Font Name
	std::string mFontName;
	//! Default font size.
	float mFontSize;
	//! panel List
	std::vector <NxPanel *> mPanels;
	//! Viewport Original Size
	unsigned int mScreenOriWidth, mScreenOriHeight;
	//! Viewport  Size
	unsigned int mScreenWidth, mScreenHeight;
	//! SceneManager.
	NxContentManager * mSceneManager;
	//! Viewport
	NxViewport * mViewport;
	//! Colour Manager.
	NxGuiColours * mColours;
	int mX;
	int mY;
};

}

#endif