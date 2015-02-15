#include "NxGraphicsPreCompiled.h"
//#include "..\nxdeps\include\OGRE\OgreTextAreaOverlayElement.h"
//#include "..\nxdeps\include\OGRE\OgreFontManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreOverlayManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreOverlayContainer.h"
#include "../nxdeps/include/OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "../nxdeps/include/OGRE/Overlay/OgreFontManager.h"

#include "NxConversions.h"

using namespace Ogre;

namespace Nx {

template<> NxOverlayManager* NxSingleton<NxOverlayManager>::ms_Singleton = 0;

NxOverlayManager::NxOverlayManager()
{

}

NxOverlayManager::~NxOverlayManager()
{

}

NxOverlayManager& NxOverlayManager::getSingleton()
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxOverlayManager* NxOverlayManager::getSingletonPtr()
{
	return ms_Singleton;
}

NxOverlayContainer * NxOverlayManager::CreateContainer( const std::string & PanelName )
{
	Ogre::OverlayManager::getSingleton().createOverlayElement( "Panel", PanelName );
	return new NxOverlayContainer( PanelName );
}

NxOverlayText * NxOverlayManager::CreateTextElement( const std::string & TextElementName )
{
	Ogre::OverlayManager::getSingleton().createOverlayElement( "TextArea", TextElementName );
	return new NxOverlayText( TextElementName );
}
 
NxOverlay * NxOverlayManager::CreateOverlay( const std::string & Name )
{
	Ogre::OverlayManager::getSingleton().create( Name );
	return new NxOverlay( Name );
}



//////////////////// OVERLAY /////////////////


//////////// OVERLAYCONTAINER ////////////////

NxOverlayContainer::NxOverlayContainer( const std::string & PanelName ) : NxOverlayElement()
{

	 
	mOverlayElement = static_cast<Ogre::OverlayContainer*>( Ogre::OverlayManager::getSingleton().getOverlayElement( PanelName, false ));
	mOverlayElement->setMetricsMode(Ogre::GMM_RELATIVE);
	mOverlayElement->setPosition(0.0f, 0.0f);
	mOverlayElement->setDimensions(1.0f, 1.0f);
	mOverlayElement->show();
}

NxOverlayContainer::~NxOverlayContainer()
{

}

void NxOverlayContainer::AddTextElement( NxOverlayText * elem )
{
	((OverlayContainer*)mOverlayElement)->addChild( elem->GetElement() );
}
 
void NxOverlayContainer::AddChild( NxOverlayElement * elem )
{
	((OverlayContainer*)mOverlayElement)->addChild( elem->GetElement() );
}
     
void NxOverlayContainer::RemoveChild( const std::string & name )
{
	((OverlayContainer*)mOverlayElement)->removeChild( name );
}

 
 

/////////////////// OVERLAY TEXT ELEMENT

 
NxOverlayText::NxOverlayText( const std::string & TextElementName ) : NxOverlayElement()
{
	mOverlayElement = static_cast<TextAreaOverlayElement*>( Ogre::OverlayManager::getSingleton().getOverlayElement( TextElementName , false ));
	mOverlayElement->setMetricsMode(Ogre::GMM_RELATIVE);
	mOverlayElement->setPosition(0.0f, 0.0f);
	mOverlayElement->setDimensions(1.0f, 1.0f);
	mOverlayElement->show();
	//mOverlayElement->setPosition(0, 0);
	//mOverlayElement->setDimensions(100, 100);
	//mOverlayElement->setCaption("Hello, World!");
	//mOverlayElement->setCharHeight(16);
	//mOverlayElement->setFontName("StarWars");
	//mOverlayElement->setColourBottom(ColourValue(0.3, 0.5, 0.3));
	//mOverlayElement->setColourTop(ColourValue(0.5, 0.7, 0.5));
}

NxOverlayText::~NxOverlayText()
{

}

void NxOverlayText::SetParameter( const std::string & ParameterName, const std::string & ParameterValue )
{
	((TextAreaOverlayElement*)mOverlayElement)->setParameter( ParameterName, ParameterValue );
}

void NxOverlayText::setMetricsMode( NxMetricsMode gmm )
{
	((TextAreaOverlayElement*)mOverlayElement)->setMetricsMode( (Ogre::GuiMetricsMode)  gmm );
}

void NxOverlayText::SetCaption( const std::string & Caption )
{
	((TextAreaOverlayElement*)mOverlayElement)->setCaption( Caption );
}

const std::string & NxOverlayText::GetCaption() const
{
	return ((TextAreaOverlayElement*)mOverlayElement)->getCaption();//.toUTF8();
}

void NxOverlayText::SetCharHeight( float Size )
{
	((TextAreaOverlayElement*)mOverlayElement)->setCharHeight( Size );
}

void NxOverlayText::SetSpaceWidth( float Width )
{
	((TextAreaOverlayElement*)mOverlayElement)->setSpaceWidth( Width );
}

void NxOverlayText::SetFontName( const std::string & FontName )
{
	((TextAreaOverlayElement*)mOverlayElement)->setFontName( FontName );
}

void NxOverlayText::SetColourBottom( const NxColourValue & Val )
{
	((TextAreaOverlayElement*)mOverlayElement)->setColourBottom( NxColourtoOgre( Val ) );
}

void NxOverlayText::SetColourTop( const NxColourValue & Val )
{
	((TextAreaOverlayElement*)mOverlayElement)->setColourTop( NxColourtoOgre( Val ) );
}
 

//////////////////// OVERLAY ELEMENT /////////////////

NxOverlayElement::NxOverlayElement()
{

}

NxOverlayElement::~NxOverlayElement()
{

}

OverlayElement * NxOverlayElement::GetElement()
{
	return mOverlayElement;
}

const std::string & NxOverlayElement::GetMaterialName() const
{
	return mOverlayElement->getMaterialName().c_str();
}

void NxOverlayElement::SetMaterialName( const std::string & matName )
{
	mOverlayElement->setMaterialName( matName );
}

void NxOverlayElement::SetMetricsMode( NxMetricsMode Mode )
{
	mOverlayElement->setMetricsMode( (GuiMetricsMode) Mode );
}

void NxOverlayElement::SetPosition( float X, float Y)
{
	mOverlayElement->setPosition( X, Y );
}

void NxOverlayElement::SetDimensions( float Width, float Height )
{
	mOverlayElement->setDimensions(  Width , Height );
}

float NxOverlayElement::GetWidth() const
{
	return mOverlayElement->getWidth();
}

void NxOverlayElement::SetWidth( float Width )
{
	mOverlayElement->setWidth( Width );
}

float NxOverlayElement::GetHeight() const
{
	return mOverlayElement->getHeight();
}

void NxOverlayElement::SetHeight( float Height )
{
	mOverlayElement->setHeight( Height );
}

void NxOverlayElement::SetLeft( float left)
{
	mOverlayElement->setLeft( left );
}

float NxOverlayElement::GetLeft() const
{
	return mOverlayElement->getLeft();
}

void NxOverlayElement::SetTop( float Top )
{
	mOverlayElement->setTop( Top );
}

void NxOverlayElement::Show()
{
	mOverlayElement->show();
}

void NxOverlayElement::Hide()
{
	mOverlayElement->hide();
}

}