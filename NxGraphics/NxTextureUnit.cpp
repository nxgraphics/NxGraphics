#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

NxTextureUnit::NxTextureUnit( NxPass * parent, const std::string & TextureUnitName )
{
	mTextureUnit = parent->GetPass()->createTextureUnitState();
	mTextureUnit->setName( TextureUnitName );
 
}

NxTextureUnit::~NxTextureUnit()
{

}

void NxTextureUnit::SetTextureFile( const std::string & TextureFile )
{
	Ogre::TexturePtr Tex = TextureManager::getSingleton().load( TextureFile, "General" );
	mTextureUnit->setTexture( Tex );
}

void NxTextureUnit::SetTextureName( const std::string & TextureName, NxTextureType Type )
{
	mTextureUnit->setTextureName( TextureName , (Ogre::TextureType)Type);
}

void NxTextureUnit::SetTextureName( const std::string & TextureName )
{
	Ogre::TexturePtr Tex = TextureManager::getSingleton().getByName( TextureName );
	mTextureUnit->setTexture( Tex );
}

const std::string & NxTextureUnit::GetName() const
{
	return mTextureUnit->getName().c_str();
}

void NxTextureUnit::SetAlphaOperation( NxLayerBlendOperationEx op, NxLayerBlendSource source1, NxLayerBlendSource source2, float arg1, float arg2, float manualBlend )
{
	mTextureUnit->setAlphaOperation( (Ogre::LayerBlendOperationEx)op, (Ogre::LayerBlendSource)source1, (Ogre::LayerBlendSource)source2, arg1, arg2, manualBlend );
}

void NxTextureUnit::SetOpacity( float Value )
{
	mTextureUnit->setAlphaOperation( LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, Value );
}

void NxTextureUnit::SetColourOperation( const NxLayerBlendOperation op )
{
	mTextureUnit->setColourOperation( ( Ogre::LayerBlendOperation )op );
}

void NxTextureUnit::SetTextureRotate( float degree )
{
	mTextureUnit->setTextureRotate( Ogre::Degree( degree ) );
}

void NxTextureUnit::SetTextureScale( float ScaleX, float ScaleY )
{
	mTextureUnit->setTextureScale( ScaleX, ScaleY );
}

/*
void NxTextureUnit::SetTextureTransform( const Ogre::Matrix4 & Mat )
{
	mTextureUnit->setTextureTransform( Mat );
}*/

void NxTextureUnit::SetTextureTransform( const Nx::Matrix4 & mat2 )
{
	Ogre::Matrix4 mat1;
	NxMat4toOgre( mat1, mat2 );
	mTextureUnit->setTextureTransform( mat1 );
}

void NxTextureUnit::SetTextureAnisotropy( unsigned int MaxAnisotropy )
{
	mTextureUnit->setTextureAnisotropy( MaxAnisotropy );
}

unsigned int NxTextureUnit::GetTextureWidth()
{
	return mTextureUnit->_getTexturePtr(  mTextureUnit->getCurrentFrame() )->getWidth();
}

unsigned int NxTextureUnit::GetTextureHeight()
{
	return mTextureUnit->_getTexturePtr(  mTextureUnit->getCurrentFrame() )->getHeight();
}

void NxTextureUnit::SetTextureBorderColour( const NxColourValue& BorderColour )
{
	Ogre::ColourValue val;
	NxColourtoOgre( val, BorderColour );
	mTextureUnit->setTextureBorderColour( val );
}

void NxTextureUnit::SetCubicTextureName( const std::string & name, bool forUVW )
{
	mTextureUnit->setCubicTextureName( name, forUVW );
}

void NxTextureUnit::SetEnvironmentMap(bool enable, NxEnvMapType envMapType )
{
	mTextureUnit->setEnvironmentMap( enable, (Ogre::TextureUnitState::EnvMapType)envMapType );
}

void NxTextureUnit::SetTextureAddressingMode( NxTextureAddressingMode tam )
{
	mTextureUnit->setTextureAddressingMode( (TextureUnitState::TextureAddressingMode) tam );
}

void NxTextureUnit::SetTextureFiltering( NxFilterType ftype, NxFilterOptions opts ) {
	mTextureUnit->setTextureFiltering( (FilterType) ftype, (FilterOptions) opts );
}

void NxTextureUnit::SetTextureFiltering( NxFilterOptions minFilter, NxFilterOptions magFilter, NxFilterOptions mipFilter) {
	mTextureUnit->setTextureFiltering( (FilterOptions) minFilter, (FilterOptions) magFilter, (FilterOptions) mipFilter );
}

void NxTextureUnit::SetTextureFiltering( NxTextureFilterOptions filterType ) {
	mTextureUnit->setTextureFiltering( (TextureFilterOptions) filterType );
}

void NxTextureUnit::SetTextureCoordSet( unsigned int setCoord )
{
	mTextureUnit->setTextureCoordSet( setCoord );
}

void NxTextureUnit::SetNumMipmaps( unsigned int numMips )
{
	mTextureUnit->setNumMipmaps( numMips );
}

void NxTextureUnit::SetTextureMipmapBias( float bias )
{
	mTextureUnit->setTextureMipmapBias( bias );
}

Ogre::TextureUnitState * NxTextureUnit::GetTextureUnit()
{
	return mTextureUnit;
}






 

}//namespace