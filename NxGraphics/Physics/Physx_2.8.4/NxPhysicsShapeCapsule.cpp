#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsCapsuleShape::NxPhysicsCapsuleShape( NxPhysicsActor * Actor, const NxCapsuleDesc & Desc ) : NxPhysicsShape( Actor, Desc )
{
	NxCapsuleShapeDesc ShapeDesc;
	ShapeDesc.setToDefault();
	ShapeDesc.radius = Desc.Radius;
	ShapeDesc.height =  Desc.Height-(Desc.Radius*2) ;//Desc.Height;
	ShapeDesc.name = std::string( "Capsule" + Ogre::StringConverter::toString( instance_count() )).c_str();
	ShapeDesc.density = 1000.0f;

	mShape = Actor->GetNxActor()->createShape( ShapeDesc );

	mShape->setFlag( NX_SF_DYNAMIC_DYNAMIC_CCD, true ); 

	//mShape->setFlag( NX_TRIGGER_ENABLE, IsTrigger );

	Actor->SetBodyProperties( Actor->GetBodyProperty() );
}

NxPhysicsCapsuleShape::~NxPhysicsCapsuleShape()
{

}

float NxPhysicsCapsuleShape::GetRadius()
{
	NxCapsuleShape * CapsuleShape = static_cast<NxCapsuleShape *>( mShape );
	return CapsuleShape->getRadius();
}

float NxPhysicsCapsuleShape::GetHeight()
{
	NxCapsuleShape * CapsuleShape = static_cast<NxCapsuleShape *>( mShape );
	return CapsuleShape->getHeight();
}

}