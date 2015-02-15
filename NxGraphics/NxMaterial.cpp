#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

MaterialNx::MaterialNx( const std::string & MaterialName )
{  
	mMaterial = Ogre::MaterialManager::getSingleton().create( MaterialName, "NxMaterialVideo"/*"NxMaterials" */ ).get();
	mMaterial->getTechnique(0)->removePass( 0 );
	mMaterial->removeTechnique(0); 
}
 
MaterialNx::~MaterialNx()
{
	Ogre::MaterialManager::getSingleton().destroyResourcePool(mMaterial->getName());

}

void MaterialNx::SetAmbient( const NxColourValue& ambient )
{
	mMaterial->setAmbient( NxColourtoOgre ( ambient ) );
}

void MaterialNx::SetDiffuse( const NxColourValue& diffuse )
{
	mMaterial->setDiffuse( NxColourtoOgre ( diffuse ) );
}

void MaterialNx::SetSpecular( const NxColourValue& specular )
{
	mMaterial->setSpecular( NxColourtoOgre ( specular ) );
}

void MaterialNx::SetLightingEnabled( bool Enabled )
{
	mMaterial->setLightingEnabled( Enabled );
}

void MaterialNx::SetSelfIllumination( float red, float green, float blue )
{
	mMaterial->setSelfIllumination( red, green, blue );
}

void MaterialNx::SetReceiveShadows( bool ReceiveShadows )
{
	mMaterial->setReceiveShadows( ReceiveShadows );
}

void MaterialNx::SetCullingMode( NxCullingMode Mode )
{
	mMaterial->setCullingMode( ( CullingMode) Mode );
}

NxTechnique * MaterialNx::CreateTechnique()
{
	NxTechnique * c = new NxTechnique( this, "DefaultTechnique");
	mTechniques.push_back(c);
	return c; 
}

NxTechnique * MaterialNx::CreateTechnique( const std::string & TechniqueName )
{
	NxTechnique * c = new NxTechnique( this, TechniqueName );
	mTechniques.push_back(c);
	return c; 
}

NxTechnique * MaterialNx::GetTechnique(unsigned short index)
{
    assert (index < mTechniques.size() && "Index out of bounds.");
    return mTechniques[index];
}

NxTechnique * MaterialNx::GetTechnique( const std::string & name )
{
    Techniques::iterator i    = mTechniques.begin();
    Techniques::iterator iend = mTechniques.end();
    NxTechnique * foundTechnique = 0;

    // iterate through techniques to find a match
    while (i != iend) {
        if ( (*i)->GetName() == name ) {
            foundTechnique = (*i);
            break;
        }
        ++i;
    }

    return foundTechnique;
}
	
unsigned short MaterialNx::GetNumTechniques(void) const
{
	return static_cast<unsigned short>(mTechniques.size());
}

void MaterialNx::Load( bool backgroundThread )
{
	mMaterial->load( backgroundThread );
}

void MaterialNx::Reload(){
	mMaterial->reload();
}

void MaterialNx::Compile( bool autoManageTextureUnits ) {
	mMaterial->compile( autoManageTextureUnits );
}

void MaterialNx::Prepare( bool backgroundThread ) {
	mMaterial->prepare( backgroundThread );
}

const std::string & MaterialNx::GetName() const
{
	return mMaterial->getName();
}

Ogre::Material * MaterialNx::GetMaterial()
{
	return mMaterial;
}


}