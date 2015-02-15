#include "NxGraphicsPreCompiled.h"
//#include "..\nxdeps\include\OGRE\OgreTextAreaOverlayElement.h"
//#include "..\nxdeps\include\OGRE\OgreBorderPanelOverlayElement.h"
//#include "..\nxdeps\include\OGRE\OgreFontManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreOverlayManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "../nxdeps/include/OGRE/Overlay/OgreBorderPanelOverlayElement.h"
#include "../nxdeps/include/OGRE/Overlay/OgreFontManager.h"
using namespace Ogre;

namespace Nx {

void NxStateIntro::Init()
{

	Overlay * mLoadOverlay = Ogre::OverlayManager::getSingleton().create("OverlayIntro");


	BorderPanelOverlayElement * Border  = static_cast<BorderPanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "BorderPanel", "TestOverlay" ));
	Border->setMetricsMode( Ogre::GMM_PIXELS);
	Border->setVerticalAlignment( Ogre::GVA_CENTER);
	Border->setHorizontalAlignment( Ogre::GHA_CENTER);
	Border->setLeft(-200);
	Border->setTop(100);  
	Border->setWidth(400);
	Border->setHeight( 80 );
	//Border->setMaterialName( BackGroundColour->getName() );
	Border->setBorderSize(1,1,1,1);
	//Border->setBorderMaterialName(BackGroundColour->getName());

	mLoadOverlay->add2D( Border );

	mLoadOverlay->show();

	
}

void NxStateIntro::Cleanup()
{


}

void NxStateIntro::Pause()
{


}

void NxStateIntro::Resume()
{


}

void NxStateIntro::GetEvents()
{


}

void NxStateIntro::Update( float Seconds )
{


}

void NxStateIntro::Display()
{


}

}

 