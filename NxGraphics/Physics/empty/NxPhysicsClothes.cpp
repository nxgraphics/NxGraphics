#include <NxGraphicsPreCompiled.h>

namespace Nx {

Nx3DObjectPhysicCloth::Nx3DObjectPhysicCloth( SceneNode * Node  ) : NxEntity( ) 
{
 

}

Nx3DObjectPhysicCloth::~Nx3DObjectPhysicCloth()
{

}

bool Nx3DObjectPhysicCloth::Create( NxClothDesc &desc, char *objFileName, float scale, NxVec3* offset, char *texFilename   )
{

	return true;
}

void Nx3DObjectPhysicCloth::Update( float time )
{
	 
}

void Nx3DObjectPhysicCloth::SetMaterialName( const std::string & MaterialName )
{
	 
}

void Nx3DObjectPhysicCloth::init( NxScene * scene , NxClothDesc &desc, NxClothMeshDesc &meshDesc)
{
	 

}

bool Nx3DObjectPhysicCloth::generateObjMeshDesc(NxClothMeshDesc &desc, char *filename, float scale, NxVec3* offset, bool textured)
{
 

	return true;
}

void Nx3DObjectPhysicCloth::generateTearLines(NxClothMeshDesc& desc, unsigned int w, unsigned int h)
{
 
}

void Nx3DObjectPhysicCloth::releaseMeshDescBuffers(const NxClothMeshDesc& desc)
{
 
}

bool Nx3DObjectPhysicCloth::cookMesh(NxClothMeshDesc& desc)
{
 
	return true;
}

void Nx3DObjectPhysicCloth::allocateReceiveBuffers( int numVertices, int numTriangles )
{
 
}

void Nx3DObjectPhysicCloth::releaseReceiveBuffers()
{
 
}

void Nx3DObjectPhysicCloth::render()
{
 
}

void Nx3DObjectPhysicCloth::DetachCloth()
{
	 
}




}//namespace