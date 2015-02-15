#ifndef __NXGUITEXT_H__
#define __NXGUITEXT_H__

#include "NxGuiElement.h"

namespace Nx {

class NxGraphics_Export NxText : public NxElement
{
public :
	//! NxText Constructor.
	NxText( const std::string & CaptionText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager, const std::string & FontName );
	//! NxText Destructor.
	~NxText();
	//! Draw Element.
	void Draw();
	//! repaint Element.
	void Repaint();
	//! Viewport size change callback.
	void OnViewportResized( float ScreenWidth, float ScreenHeight );
	// Set Text Caption.
	void SetCaption( const std::string & CaptionText );
	//! Get Text Caption.
	const std::string & GetCaption() const;
	//! Set Text Colour.
	void SetColour( const NxColourValue & Colour );
	//! set text pixels height.
	void SetTextPixelHeight( int PixelHeight );
	//! set text pixels Spacing.
	void SetTextPixelSpacing( int PixelSpace );
	//! Set Text Wrapping.
	bool SetTextWidthWrap( bool Wrap );
	//! Is Text Wrapping.
	bool IsTextWrapping();
	//! Set Element Visible.
	void SetVisible( bool Visible );
	//! Set Vertical Alignment.
	void SetVerticalAlignment( NxTextVerticalAlignment VAlign );
	//! Set Horizontal Alignment.
	void SetHorizontalAlignment( NxTextHorizontalAlignment HAlign );

	void SetVisibilityFlags( int Flags );

	void SetRenderQueueGroup( unsigned int Group );
	void SetZOrder( unsigned short ZOrder );
	unsigned short GetZOrder();
private :
	NxText2D * mTextStruct;
	std::string mText;
	bool mTextWrap;
	Ogre::SceneNode * mRootNode;
};

}

#endif