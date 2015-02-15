#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxViewport::NxViewport( NxRenderWindow * ParentWindow, NxCamera * Cam, int Zorder )
{
	mViewport = ParentWindow->GetWindow()->addViewport( Cam->GetCamera(), Zorder );
}

NxViewport::~NxViewport()
{

}

void NxViewport::SetOverlaysEnabled( bool enabled ) {

	mViewport->setOverlaysEnabled( enabled );
}

void NxViewport::SetOrientationMode( NxOrientationMode orientationMode, bool setDefault ) {

	mViewport->setOrientationMode( (Ogre::OrientationMode )orientationMode, setDefault );
}

void NxViewport::Clear( unsigned int buffers, const NxColourValue& col, float depth, unsigned short stencil ) {

	mViewport->clear( buffers, Ogre::ColourValue(  col.r, col.g, col.b, col.a  ) , depth, stencil );
}

void NxViewport::SetDimensions( float Left, float Top, float Width, float Height )
{
	mViewport->setDimensions( Left, Top, Width, Height );
}
 
void NxViewport::SetBackgroundColour( const NxColourValue & BackGroundColour )
{
	mViewport->setBackgroundColour( Ogre::ColourValue(  BackGroundColour.r, BackGroundColour.g, BackGroundColour.b, BackGroundColour.a  )  );
}

int NxViewport::GetActualLeft() const {
	return mViewport->getActualLeft();
}
   
int NxViewport::GetActualTop() const{
	return mViewport->getActualTop();
}

int NxViewport::GetActualWidth() const
{
	return mViewport->getActualWidth();
}

int NxViewport::GetActualHeight() const
{
	return mViewport->getActualHeight();
}

void NxViewport::SetClearEveryFrame( bool clear, unsigned int buffers ) {
	mViewport->setClearEveryFrame(clear, buffers);
}

void NxViewport::SetShadowsEnabled( bool enabled ) {
	mViewport->setShadowsEnabled( enabled );
}
 
void NxViewport::SetSkiesEnabled( bool enabled ) {
	mViewport->setSkiesEnabled( enabled );
}
 
void NxViewport::SetVisibilityMask( unsigned int mask ) {
	mViewport->setVisibilityMask( mask );
}

void NxViewport::SetAutoUpdated( bool AutoUpdate )
{
	mViewport->setAutoUpdated( AutoUpdate );
}

void NxViewport::Update()
{
	mViewport->update();
}

Ogre::Viewport * NxViewport::GetViewport()
{
	return mViewport;
}



}