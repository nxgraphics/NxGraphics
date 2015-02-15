#ifndef __NXGUIRECTANGLE_H__
#define __NXGUIRECTANGLE_H__

#include "NxGuiElement.h"

namespace Nx {

class NxGraphics_Export NxRectangle : public NxElement
{
public :
	NxRectangle( float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxRectangle();
	void Draw();
	void Repaint() ;
	void SetColour( const NxColourValue & Colour );
	const NxColourValue & GetColour() const;
	void OnViewportResized( float ScreenWidth, float ScreenHeight ) ;
	void SetVisible( bool Visible );
	void SetTexture( const std::string & TextureName );
	const std::string & getBackgroundTextureName() const;
	//! has texture texture coordinates
	bool hasTextureCoordinates();
	//! set texture coordinates
	void setTextureCoordinates( const  Nx::Vector2 &topLeft, const  Nx::Vector2 &bottomLeft, const  Nx::Vector2 &topRight, const  Nx::Vector2 &bottomRight);
	//! get texture coordiantes
	void getTextureCoordinates( Vector2 & topLeft, Vector2 & bottomLeft, Vector2 & topRight, Vector2 & bottomRight ); 
	//! set visibility flags
	void SetVisibilityFlags( int Flags );
	void SetRenderQueueGroup( unsigned int Group );
	void SetZOrder( unsigned short ZOrder );
	unsigned short GetZOrder();
private :
	NxGuiRectangle2D * mRectangle;
};

}

#endif