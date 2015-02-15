#include "NxGraphicsPreCompiled.h"
#include "NxMaterialCubeReflection.h"

namespace Nx {

template<> NxMaterialManager* NxSingleton<NxMaterialManager>::ms_Singleton = 0;

NxMaterialManager::NxMaterialManager()
{

}

NxMaterialManager::~NxMaterialManager()
{


}

NxMaterialManager& NxMaterialManager::getSingleton()
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxMaterialManager* NxMaterialManager::getSingletonPtr()
{
	return ms_Singleton;
}

MaterialNx * NxMaterialManager::CreateMaterial( const std::string & MaterialName )
{
	MaterialNx * c = new MaterialNx( MaterialName );
	mMaterials.insert( NxMaterialList::value_type( MaterialName, c ) );
	return c; 
}

//! Destroy a Material
void NxMaterialManager::DestroyMaterial( const std::string & MaterialName )
{
	NxMaterialList::iterator i = mMaterials.find(MaterialName);
	if (i == mMaterials.end()) {
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "NxNode '" + MaterialName + "' not found.", "NxMaterialManager::DestroyMaterial");
	}

	delete i->second;
	mMaterials.erase(i);
}

bool NxMaterialManager::MaterialExists( const std::string & materialname )
{
	return MaterialManager::getSingleton().resourceExists( materialname );
}

MaterialNx * NxMaterialManager::GetMaterial( const std::string & MaterialName )
{
	NxMaterialList::iterator i = mMaterials.find(MaterialName);
	if (i == mMaterials.end()) {
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "NxMaterial '" + MaterialName + "' not found.", "NxMaterialManager::GetMaterial");
	}

	return i->second;

    //Materials::iterator i    = mMaterials.begin();
    //Materials::iterator iend = mMaterials.end();
    //MaterialNx * foundMaterial = 0;

    //// iterate through materials to find a match
    //while (i != iend) {
    //    if ( (*i)->GetName() == MaterialName ){
    //        foundMaterial = (*i);
    //        break;
    //    }
    //    ++i;
    //}

    //return foundMaterial;
}


void NxMaterialManager::DestroyProgram( const std::string & ProgramName ) {
 
	NxShaderList::iterator i = mShaders.find( ProgramName );
	if (i == mShaders.end()) {
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "NxShader '" + ProgramName + "' not found.", "NxMaterialManager::DestroyProgram");
	}

	delete i->second;
	mShaders.erase(i); 
}
 

NxVertexShader * NxMaterialManager::CreateVertexProgram( const std::string & VertexProgramName, const std::string & ShaderType   )
{
	NxVertexShader * c = new NxVertexShader( VertexProgramName, ShaderType);
	mShaders.insert( NxShaderList::value_type( VertexProgramName, c ) );
	return c;
}

NxPixelShader * NxMaterialManager::CreateFragmentProgram( const std::string & FragmentProgramName, const std::string & ShaderType  )
{
	NxPixelShader * c = new NxPixelShader( FragmentProgramName, ShaderType );
	mShaders.insert( NxShaderList::value_type( FragmentProgramName, c ) );
	return c;
}

NxGeometryShader * NxMaterialManager::CreateGeometryProgram( const std::string & GeometryProgramName, const std::string & ShaderType  )
{
	NxGeometryShader * c = new NxGeometryShader( GeometryProgramName, ShaderType );
	mShaders.insert( NxShaderList::value_type( GeometryProgramName, c ) );
	return c;
}

void NxMaterialManager::CreateCubemapMaterial( const std::string & MaterialName, Ogre::Entity * Ent )
{
	new NxMaterialCubeMap( MaterialName, Ent );
}

}