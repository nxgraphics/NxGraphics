#include "NxGraphicsPreCompiled.h"

namespace Nx {

Nx3DSceneObject::Nx3DSceneObject()
{
	
}

Nx3DSceneObject::~Nx3DSceneObject()
{

}

/*
Ogre::Mesh * Nx3DSceneObject::GetObject()
{
	return Ogre::MeshPtr();
}*/

Nx3DSceneObjectTypes Nx3DSceneObject::GetType()
{
	return Type ;

}

}//namespace
