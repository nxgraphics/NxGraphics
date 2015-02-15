#include "NxGraphicsPreCompiled.h"
#include "NxMeshManager.h"

#include "../nxdeps/include/OgreProcedurals/include/Procedural.h"

namespace Nx {

template<> NxMeshManager* NxSingleton<NxMeshManager>::ms_Singleton = 0;

NxMeshManager::NxMeshManager()
{
	
}

NxMeshManager::~NxMeshManager()
{

}

NxMeshManager& NxMeshManager::getSingleton(void)
{
	assert( ms_Singleton ); return ( *ms_Singleton );
}

Ogre::MeshPtr NxMeshManager::CreateCapsule( const std::string& name, float Radius, float Height)
{
	return Procedural::CapsuleGenerator().setRadius( Radius ).setNumRings( 8 ).setNumSegments( 12 ).setNumSegHeight( 1 ).setHeight( Height ).realizeMesh(name);
}

Ogre::MeshPtr NxMeshManager::CreateBox( const std::string& name, Nx::Vector3 Size, Nx::Vector3 Segments)
{
	return Procedural::BoxGenerator().setSizeX(Size.x).setSizeY(Size.y).setSizeZ(Size.z).setNumSegX( Segments.x ).setNumSegY( Segments.y ).setNumSegZ( Segments.z ).realizeMesh(name);
}

Ogre::MeshPtr NxMeshManager::CreateSphere( const std::string& name, float Radius, unsigned int numRings, unsigned int numSegments, float uTile, float vTile )
{
	return Procedural::SphereGenerator().setRadius(Radius).setNumRings(numRings).setNumSegments(numSegments).setUTile(uTile).setVTile(vTile).realizeMesh(name);
	//return Procedural::SphereGenerator().setRadius(Radius).setNumRings(15).setNumSegments(16).setUTile(5.).setVTile(5.).realizeMesh(name);

}

Ogre::MeshPtr NxMeshManager::CreatePlane( const std::string& name, Nx::Vector2 Size, Nx::Vector2 Segments)
{
	return Procedural::PlaneGenerator().setSizeX( Size.x ).setSizeY( Size.y ).setNumSegX( Segments.x ).setNumSegY( Segments.y ).setUTile(1.0).setVTile(1.0).realizeMesh(name); 
}

Ogre::MeshPtr NxMeshManager::CreateCylinder( const std::string& name , float Radius, float Height )
{
	return Procedural::CylinderGenerator().setRadius( Radius ).setNumSegBase(32).setNumSegHeight(8).setHeight( Height ).realizeMesh(name);
}

Ogre::MeshPtr NxMeshManager::CreateCone( const std::string& name , float Radius, float Height )
{
	return Procedural::ConeGenerator().setRadius( Radius ).setNumSegBase(32).setNumSegHeight( 8 ).setHeight( Height ).realizeMesh(name);
}


}//namespace

