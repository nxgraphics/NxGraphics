#ifndef __NXGUIELEMENT_H__
#define __NXGUIELEMENT_H__

#include <NxPrerequisites.h>

namespace Nx {

class NxGraphics_Export NxElement
{
public :
	//! NxElement Constructor.
	NxElement( int PosX, int PosY, int Width, int Height, NxGuiManager * Manager );
	//! NxElement Destructor.
	virtual ~NxElement();
	//! get local x position
	int getX();
	//! get local y position
	int getY();
	//! get derived x position from all parents: screen position
	int getPositionDerivedX();
	//! get derived y position from all parents: screen position
	int getPositionDerivedY();
	//! convert local position to derived.
	void localPositionToDerived( int& x, int& y );
	//! convert derived position to local.
	void derivedPositionToLocal( int& x, int& y );


	void SetTopLeftPosition( float PosX, float PosY );
	int getWidth();
	int getHeight();
	void setHeight( int height );
	void setWidth( int width);
	float GetHalfWidth();
	float GetHalfHeight();
	//! Set Parent.
	void SetParent( NxElement * parent );
	//! Get Parent
	NxElement * GetParent();
	NxGuiManager * GetManager();
	virtual void SetVisible( bool Visible ) = 0;
	virtual void SetVisibilityFlags( int Flags ) = 0;
	virtual void SetRenderQueueGroup( unsigned int Group ) = 0;
	virtual void SetZOrder( unsigned short ZOrder ) = 0;
	virtual unsigned short GetZOrder() = 0;

	//! Convert point coordinates to pixels
	void ConvertToPixelCoordinates( float sx, float sy, float & dx, float & dy );
	bool IsInside(float px, float py);
	bool IsInsideChild(float px, float py);
	bool IsInsideParent(float px, float py);
	virtual void Draw() = 0;
	virtual void Repaint() = 0;
	virtual void OnViewportResized( float ScreenWidth, float ScreenHeight ) = 0;

protected :
	int mX, mY, mWidth, mHeight;
	float mHalfwidth;
	float mHalfheight;
	NxElement * mParent;
	float mViewportWidth;
	float mViewportHeight;
	bool mNeedsUpate;
	NxGuiManager * mManager;
	//! Current Viewport sizes.
	float mScreenWidth, mScreenHeight;
	Ogre::SceneNode * mNode;
};

}

#endif