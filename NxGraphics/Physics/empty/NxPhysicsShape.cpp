#include <NxGraphicsPreCompiled.h>

namespace Nx {

NxPhysicsShape::NxPhysicsShape( NxPhysicsActor * Actor, const NxBaseDesc & Desc )
{

}

NxPhysicsShape::~NxPhysicsShape()
{
 
}

void NxPhysicsShape::SetGroup( int group )
{
	 
}

void NxPhysicsShape::SetShapePairFlags( NxPhysicsShape * shape, int flags )
{
	 
}

void NxPhysicsShape::SetSkinWidth( float SkinWidth )
{
	 
}

float NxPhysicsShape::GetSkinWidth() const
{
	return 0.0f;
}

void NxPhysicsShape::SetLocalPosition( const Ogre::Vector3 & Position )
{
	 
}

const Ogre::Vector3 & NxPhysicsShape::GetLocalPosition() const
{
	return Ogre::Vector3(0,0,0);
}

void NxPhysicsShape::SetGlobalPosition( const Ogre::Vector3 & Position )
{
	 
}
const Ogre::Vector3 & NxPhysicsShape::GetGlobalPosition() const
{
	return Ogre::Vector3(0,0,0);
}

void NxPhysicsShape::SetLocalOrientation( const Ogre::Quaternion & Orientation )
{
	 
}
const Ogre::Quaternion & NxPhysicsShape::GetLocalOrientation() const
{
	return Ogre::Quaternion();
}

void NxPhysicsShape::SetGlobalOrientation( const Ogre::Quaternion & Orientation )
{
	 
}

const Ogre::Quaternion & NxPhysicsShape::GetGlobalOrientation() const
{
	return Ogre::Quaternion();
}

const NxShapeTypes NxPhysicsShape::GetType()
{
	return mType;
}

//// to do forcefielddesc
//NxPhysicsForceField * NxPhysicsShape::CreateForceField()
//{
//	return new NxPhysicsForceField( this );
//}

NxShape * NxPhysicsShape::GetNxShape()
{
	return 0;
}


}