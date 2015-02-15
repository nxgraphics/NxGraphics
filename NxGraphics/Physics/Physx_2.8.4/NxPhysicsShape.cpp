#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsShape::NxPhysicsShape( NxPhysicsActor * Actor, const NxBaseDesc & Desc )
{

}

NxPhysicsShape::~NxPhysicsShape()
{
 
}

void NxPhysicsShape::SetGroup( int group )
{
	mShape->setGroup( group );
}

void NxPhysicsShape::SetShapePairFlags( NxPhysicsShape * shape, int flags )
{
	mShape->getActor().getScene().setShapePairFlags(*mShape, *shape->mShape, (NxContactPairFlag)flags );//NX_IGNORE_PAIR
}

void NxPhysicsShape::SetSkinWidth( float SkinWidth )
{
	mShape->setSkinWidth( SkinWidth );
}

float NxPhysicsShape::GetSkinWidth() const
{
	return mShape->getSkinWidth();
}

void NxPhysicsShape::SetLocalPosition( const Ogre::Vector3 & Position )
{
	mShape->setLocalPosition( VecToPhysx( Position ) );
}

const Ogre::Vector3 & NxPhysicsShape::GetLocalPosition() const
{
	return VecToOgre( mShape->getLocalPosition() );
}

void NxPhysicsShape::SetGlobalPosition( const Ogre::Vector3 & Position )
{
	mShape->setGlobalPosition( VecToPhysx( Position ) );
}

const Ogre::Vector3 & NxPhysicsShape::GetGlobalPosition() const
{
	return VecToOgre( mShape->getGlobalPosition() );
}

void NxPhysicsShape::SetLocalOrientation( const Ogre::Quaternion & Orientation )
{
	mShape->setLocalOrientation( nxquat( Orientation ) );
}

const Ogre::Quaternion & NxPhysicsShape::GetLocalOrientation() const
{
	return ogrequat( mShape->getLocalOrientation() );
}

void NxPhysicsShape::SetGlobalOrientation( const Ogre::Quaternion & Orientation )
{
	mShape->setGlobalOrientation( nxquat( Orientation ) );
}

const Ogre::Quaternion & NxPhysicsShape::GetGlobalOrientation() const
{
	return ogrequat( mShape->getGlobalOrientation() );
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
	return mShape;
}


}