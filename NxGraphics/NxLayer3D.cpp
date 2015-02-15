#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxTexture3D::NxTexture3D() : NxTexture()
{

}

NxTexture3D::~NxTexture3D()
{

}

bool NxTexture3D::CreateScene( const Nx3DSceneDesc & desc ) {

	switch( desc.mType ) {
		case Nx3D_Scene_Ogremax :
			//SendLoadProgress(  "3D layer" , "Creating 3d OgreMax Scene" , 25.0f );
			mNx3DScene = NxScene3DManager::getSingleton().CreateNx3DScene( desc );
			break ;
		case Nx3D_Scene_Default :
			//SendLoadProgress(  "3D layer" , "Creating 3d Empty Scene" , 25.0f );
			mNx3DScene = NxScene3DManager::getSingleton().CreateNx3DScene( desc );
			break ;
		default :
			LogMsg("NxGraphics : Could not find any 3D scene for this type !");
			return false;
			break ;
	}
	return true;
}

Nx3D_Scene * NxTexture3D::GetScene()
{
	return mNx3DScene;
}


 
NxLayer3D::NxLayer3D( int Width, int Height ): NxLayer( Width, Height )
{

}

NxLayer3D::~NxLayer3D()
{

}

NxTexture3D * NxLayer3D::GetTexture()
{
	return dynamic_cast<NxTexture3D*>( NxTextureManager::getSingleton().GetTexture( NxEntity->getSubEntity(0)->getMaterialName() ));
}

}