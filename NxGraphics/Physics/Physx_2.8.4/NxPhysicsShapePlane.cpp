#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsPlaneShape::NxPhysicsPlaneShape( NxPhysicsActor * Actor, const NxPlaneDesc & Desc ) : NxPhysicsShape( Actor, Desc )
{
	NxPlaneShapeDesc ShapeDesc;
	ShapeDesc.setToDefault();
	ShapeDesc.name = std::string( "Plane" + Ogre::StringConverter::toString( instance_count() )).c_str();
	ShapeDesc.density = 1000.0f;

	mShape = Actor->GetNxActor()->createShape( ShapeDesc );

	mShape->setFlag( NX_SF_DYNAMIC_DYNAMIC_CCD, true ); 

	//mShape->setFlag( NX_TRIGGER_ENABLE, IsTrigger );

	Actor->SetBodyProperties( Actor->GetBodyProperty() );

}

NxPhysicsPlaneShape::~NxPhysicsPlaneShape()
{

}

}//namespace