#include "NxGraphicsPreCompiled.h"
//#include "..\nxdeps\include\OGRE\OgreFontManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreFontManager.h"
using namespace Ogre;

namespace Nx {

template<> NxFontManager* NxSingleton<NxFontManager>::ms_Singleton = 0;

NxFontManager::NxFontManager()
{

}

NxFontManager::~NxFontManager()
{

}

NxFontManager& NxFontManager::getSingleton()
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxFontManager * NxFontManager::getSingletonPtr()
{
	return ms_Singleton;
}

NxFont * NxFontManager::CreateFont( const std::string & FontResourceName, const std::string & FontFileName )
{
	Font * f = (Font*)Ogre::FontManager::getSingleton().create( FontResourceName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ).getPointer();
	f->setType(  FT_TRUETYPE );
	f->setSource( FontFileName  );
	f->setTrueTypeSize( 16 );
	f->setTrueTypeResolution( 96 );
	return new NxFont( FontResourceName );

}

//// FONT

NxFont::NxFont( const std::string & FontResourceName )
{
	mFont = (Ogre::Font*)Ogre::FontManager::getSingleton().create( FontResourceName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ).getPointer();
}

NxFont::~NxFont()
{

}

void NxFont::SetTrueTypeSize( float Size )
{
	mFont->setTrueTypeSize( Size );
}

void NxFont::SetTrueTypeResolution( int resolution )
{
	mFont->setTrueTypeResolution( resolution );
}

const std::string & NxFont::GetName() const
{
	return mFont->getName().c_str();
}

}//namespace