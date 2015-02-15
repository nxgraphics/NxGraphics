#ifndef __NXGUILINE_H__
#define __NXGUILINE_H__

#include "NxGuiElement.h"

namespace Nx {

class NxGraphics_Export NxGuiLine : public NxElement
{
public :
	NxGuiLine( float PosX, float PosY, float Width, float Height, NxGuiManager * Manager );
	~NxGuiLine();
	void Draw();
	void Repaint() ;
	void SetColour( const NxColourValue & Colour );
	void OnViewportResized( float ScreenWidth, float ScreenHeight ) ;
	void SetVisible( bool Visible );
	void SetVisibilityFlags( int Flags );
	void SetRenderQueueGroup( unsigned int Group );
	void SetZOrder( unsigned short ZOrder );
	unsigned short GetZOrder();
	void addPoint(const Vector3 &p);
	void addPoint(Real x, Real y, Real z);
	const Vector3& getPoint(unsigned short index) const;
	unsigned short getNumPoints(void) const;
	void setPoint(unsigned short index, const Vector3 &value);
private :
	NxLine2D * mLine;
};

}

#endif