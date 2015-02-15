#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsBoxShape::NxPhysicsBoxShape( NxPhysicsActor * Actor, const NxBoxDesc & Desc ) : NxPhysicsShape( Actor, Desc )
{
	NxBoxShapeDesc ShapeDesc;
	ShapeDesc.setToDefault();
	ShapeDesc.dimensions = NxVec3ToPhysx( Desc.mDimensions / 2.0f );
	ShapeDesc.localPose.t = NxVec3ToPhysx( Desc.mlocalPosition );

	NxMat33 Rot;
	Rot.fromQuat( NxQuatToPhysx( Desc.mlocalOrientation ) );
	ShapeDesc.localPose.M = Rot;
	ShapeDesc.name = std::string( "Box" + Ogre::StringConverter::toString( instance_count() )).c_str();
	ShapeDesc.density = 1000.0f;

	mShape = Actor->GetNxActor()->createShape( ShapeDesc );
	SetSkinWidth( 0.001f );
	mShape->setFlag( NX_SF_DYNAMIC_DYNAMIC_CCD, true ); 
	//mShape->setFlag( NX_TRIGGER_ENABLE, IsTrigger );

	Actor->SetBodyProperties( Actor->GetBodyProperty() );
}

NxPhysicsBoxShape::~NxPhysicsBoxShape()
{

}

void NxPhysicsBoxShape::SetDimensions( const Nx::Vector3 & Dimension )
{
	NxBoxShape * BoxShape = static_cast<NxBoxShape *>( mShape );
	BoxShape->setDimensions( NxVec3ToPhysx( Dimension ) );
}

const Nx::Vector3 & NxPhysicsBoxShape::GetDimensions() const
{
	NxBoxShape * BoxShape = static_cast<NxBoxShape *>( mShape );
	return PhysxToNxVec3( BoxShape->getDimensions() );
}

}
