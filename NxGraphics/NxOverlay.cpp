#include "NxGraphicsPreCompiled.h"
#include "../nxdeps/include/OGRE/Overlay/OgreOverlayManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreOverlayContainer.h"
#include "../nxdeps/include/OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "../nxdeps/include/OGRE/Overlay/OgreFontManager.h"

namespace Nx {

NxOverlay::NxOverlay( const std::string & Name )
{
	mOverlay = Ogre::OverlayManager::getSingleton().getByName( Name ); 
}

NxOverlay::~NxOverlay()
{

}

void NxOverlay::Show()
{
	mOverlay->show();
}

void NxOverlay::Hide()
{
	mOverlay->hide();
}

void NxOverlay::Add2D( NxOverlayContainer * cont )
{
	mOverlay->add2D(  ( OverlayContainer * )cont->GetElement() );
}

void NxOverlay::Remove2D( NxOverlayContainer * cont )
{
	mOverlay->remove2D(  ( OverlayContainer * )cont->GetElement() );

}

}