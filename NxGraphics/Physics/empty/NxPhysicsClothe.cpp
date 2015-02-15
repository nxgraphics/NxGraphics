#include <NxGraphicsPreCompiled.h>

namespace Nx {

Nx3DObjectPhysicClothe::Nx3DObjectPhysicClothe( NxNode * parentNode, const NxClotheDesc & ClotheDesc ) : NxEntity()
{
	mType = NxTypeCloth ;

}

Nx3DObjectPhysicClothe::~Nx3DObjectPhysicClothe()
{

}

void Nx3DObjectPhysicClothe::AttachVertexToGlobalPosition( unsigned int Vertice, const Ogre::Vector3 & Position )
{
 
}

void Nx3DObjectPhysicClothe::allocateReceiveBuffers( int numVertices, int numTriangles )
{
	
}

void Nx3DObjectPhysicClothe::Update( float time )
{
	 
}


}