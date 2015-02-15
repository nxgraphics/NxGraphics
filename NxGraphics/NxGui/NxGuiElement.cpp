#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxElement::NxElement( int PosX, int PosY, int Width, int Height, NxGuiManager * Manager ) : mManager(Manager), mNeedsUpate(true)
{
	mX = PosX;
	mY = PosY;
	mWidth = Width;
	mHalfwidth = (float)mWidth*.5f;
	mHeight = Height;
	mHalfheight = (float)mHeight*.5f;
	mScreenWidth = Manager->GetViewportWidth();
	mScreenHeight = Manager->GetViewportHeight();
	mParent = 0;
}

NxElement::~NxElement()
{

}

void NxElement::ConvertToPixelCoordinates( float sx, float sy, float & dx, float & dy )
{
	dx = (sx / (mScreenWidth / 2.0f) - 1.0f);
	dy = 1 - (sy / ( mScreenHeight / 2.0f ) );
}

void NxElement::SetParent( NxElement *_parent)
{
	mParent = _parent;
}

NxElement * NxElement::GetParent()
{
	return mParent;
}

NxGuiManager * NxElement::GetManager()
{
	return mManager;
}

bool NxElement::IsInside( float screenX, float screenY )
{
	int srcX = getPositionDerivedX();
	int srcY = getPositionDerivedY();
 
	if ( screenX >= srcX && screenY >= srcY && ((unsigned int) screenX) < (unsigned int) srcX+getWidth() && ((unsigned int) screenY) < (unsigned int) srcY+getHeight() ) {
		return true;
	}
	return false; 
 


	 /*
	if(mParent != 0) {
		return IsInsideParent(px,py);
	}
	else {
		return IsInsideChild(px,py);
	}  */
}

bool NxElement::IsInsideChild(float px, float py)
{
	if( px >= mX && py >= mY && px < mX + mWidth && py < mY + mHeight ){
		return true;
	}
	return false;

}

bool NxElement::IsInsideParent(float px, float py)
{
	if( px >= (mX+mParent->getX()) && py >= (mY+mParent->getY()) && px < (mX+mParent->getX()) + mWidth && py < (mY+mParent->getY()) + mHeight )
	{
		return true;
	}
	return false;
}

void NxElement::SetTopLeftPosition( float PosX, float PosY )
{
	mX = PosX; mY = PosY;
	//mNeedsUpate = true;
}

void NxElement::derivedPositionToLocal( int& x, int& y ) {
	/*
	x +=  getX(); 
	y +=  getY(); */

	if (mParent != 0)
		mParent->derivedPositionToLocal( x, y );

	x -= getX();
	y -= getY();
}

void NxElement::localPositionToDerived( int& x, int& y ) {

	/*
	x -=  getX(); 
	y -=  getY(); */
 
	NxElement * c = this;
 
	do {
		x +=  c->getX(); 
		y +=  c->getY(); 
		c = c->mParent;
	}
	while (c != 0);
}

int NxElement::getPositionDerivedX() {
	int x = 0;
	int y = 0;
	localPositionToDerived( x,  y );
	return x;
}

int NxElement::getPositionDerivedY() {
	int x = 0;
	int y = 0;
	localPositionToDerived( x,  y );
	return y;
}

int NxElement::getX()
{
	return mX;

	/*
	if(mParent != 0) {
 
		 if( mParent->GetParent() != 0 ) {
			return (mX+mParent->GetParent()->getX());
		}
		else{
			return (mX+mParent->getX());
		}	
	}
	else {
		return mX;
	}	*/
}

int NxElement::getY()
{
	return mY;
	
	/*
	if(mParent != 0) {
		if( mParent->GetParent() != 0 ) {
			return (mY+mParent->GetParent()->getY());
		}
		else{
			return (mY+mParent->getY());
		}	
	}
	else {
		return mY;
	}*/
}

void NxElement::setHeight( int height )
{
	mHeight = height;
	mHalfheight = (float)mHeight*.5f;
	mNeedsUpate = true;
}

void NxElement::setWidth( int width )
{
	mWidth = width;
	mHalfwidth = (float)mWidth*.5f;
	mNeedsUpate = true;
}

int NxElement::getWidth()
{
	return mWidth;
}

int NxElement::getHeight()
{
	return mHeight;
}

float NxElement::GetHalfWidth()
{
	return mHalfwidth;
}

float NxElement::GetHalfHeight()
{
	return mHalfheight;
}

}