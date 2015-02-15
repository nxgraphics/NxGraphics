#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsForceFieldShape::NxPhysicsForceFieldShape( NxForceFieldShape * FieldShape ) : mForceField( FieldShape )
{

}

NxPhysicsForceFieldShape::~NxPhysicsForceFieldShape()
{
	//Log("Calling ~NxPhysicsForceFieldShape()");
}

NxForceFieldShape * NxPhysicsForceFieldShape::GetShape()
{
	return mForceField;
}


//===========================

NxPhysicsForceField::NxPhysicsForceField( NxPhysicsActor * Actor )//   NxPhysicsShape * Shape )
{

	//NxShape * mShape = Shape->GetNxShape();

	NxForceFieldLinearKernelDesc lKernelDesc;
	//constant force of 10 forward, which is then animated in the update() method
	lKernelDesc.constant = NxVec3(0, 0, 0);

	//lKernelDesc.constant = NxVec3(20, 0, 0);

	//The forces do not depend on where the objects are positioned
	NxMat33 m; m.zero();

	m(0,0) = -100; //radial force 
	m(0,1) = -100; //radial force 
	m(0,2) = -100; //radial force 

	lKernelDesc.positionMultiplier = m;
	lKernelDesc.noise = NxVec3(0.0,0.5,0.0); //adds a random noise on the forces to make the objects a little more chaotic

	//Set target velocity along the main axis to 20
	lKernelDesc.velocityTarget = NxVec3(10,10,10);  //20

	//Acts with a force relative to the current velocity to reach the target velocities. 0 means that those components won't be affected
	m.diagonal(NxVec3(0,0,0)); 
	lKernelDesc.velocityMultiplier = m;

	NxForceFieldLinearKernel * linearKernel = Actor->GetNxActor()->getScene().createForceFieldLinearKernel(lKernelDesc);

	//Create the forcefield with the Custom kernel
	NxForceFieldDesc fieldDesc1;
	fieldDesc1.kernel = linearKernel;
	fieldDesc1.coordinates = NX_FFC_SPHERICAL;//  NX_FFC_CARTESIAN;
	fieldDesc1.fluidType = NX_FF_TYPE_OTHER;//NX_FF_TYPE_GRAVITATIONAL;
	fieldDesc1.clothType = NX_FF_TYPE_OTHER;//NX_FF_TYPE_GRAVITATIONAL;
	fieldDesc1.softBodyType = NX_FF_TYPE_OTHER;//NX_FF_TYPE_GRAVITATIONAL;
	fieldDesc1.rigidBodyType = NX_FF_TYPE_OTHER;//NX_FF_TYPE_GRAVITATIONAL;

	// The field's pose is relative to the actor's pose and relative to the world frame if field is null.
	fieldDesc1.actor = Actor->GetNxActor();

	fieldDesc1.flags = NX_FFF_VOLUMETRIC_SCALING_FLUID | NX_FFF_VOLUMETRIC_SCALING_CLOTH | NX_FFF_VOLUMETRIC_SCALING_SOFTBODY | NX_FFF_VOLUMETRIC_SCALING_RIGIDBODY;

	mForceField = Actor->GetNxActor()->getScene().createForceField(fieldDesc1);

}

NxPhysicsForceField::~NxPhysicsForceField()
{
	ReleaseIncludeShapes();
}

// Sets the coefficient of force field function position term.
void NxPhysicsForceField::SetPositionMultiplier( const Nx::Vector3 & Force )
{
	if( mForceField->getForceFieldKernel()->getType() == NX_FFK_LINEAR_KERNEL ) {
		NxForceFieldLinearKernel * CurrentKernel = ( NxForceFieldLinearKernel * ) mForceField->getForceFieldKernel() ;
		NxMat33 m2 = CurrentKernel->getPositionMultiplier();
		m2(0,0) = Force.x; m2(0,1) = Force.y; m2(0,2) = Force.z;
		CurrentKernel->setPositionMultiplier( m2 );
	}
	else // NX_FFK_CUSTOM_KERNEL
	{
	
	}
}

// Sets the coefficient of force field function velocity term.
void NxPhysicsForceField::SetVelocityMultiplier( const Nx::Vector3 & Force )
{
	if( mForceField->getForceFieldKernel()->getType() == NX_FFK_LINEAR_KERNEL ) {
		NxForceFieldLinearKernel * CurrentKernel = ( NxForceFieldLinearKernel * ) mForceField->getForceFieldKernel() ;
		NxMat33 m2 = CurrentKernel->getVelocityMultiplier();
		m2(0,0) = Force.x; m2(0,1) = Force.y; m2(0,2) = Force.z;
		CurrentKernel->setVelocityMultiplier( m2 );
	}
	else // NX_FFK_CUSTOM_KERNEL
	{

	}
}

void NxPhysicsForceField::SetPositionTarget( const Nx::Vector3 & PosTarget )
{
	if( mForceField->getForceFieldKernel()->getType() == NX_FFK_LINEAR_KERNEL ) {
		NxForceFieldLinearKernel * CurrentKernel = ( NxForceFieldLinearKernel * ) mForceField->getForceFieldKernel() ;
		CurrentKernel->setPositionTarget( NxVec3ToPhysx( PosTarget ) );
	}
	else // NX_FFK_CUSTOM_KERNEL
	{

	}
}

void NxPhysicsForceField::SetVelocityTarget( const Nx::Vector3 & VeloTarget )
{
	if( mForceField->getForceFieldKernel()->getType() == NX_FFK_LINEAR_KERNEL ) {
		NxForceFieldLinearKernel * CurrentKernel = ( NxForceFieldLinearKernel * ) mForceField->getForceFieldKernel() ;
		CurrentKernel->setVelocityTarget( NxVec3ToPhysx( VeloTarget ) );
	}
	else // NX_FFK_CUSTOM_KERNEL
	{

	}
}

void NxPhysicsForceField::SetNoise( const Nx::Vector3 & noise )
{
	if( mForceField->getForceFieldKernel()->getType() == NX_FFK_LINEAR_KERNEL ) {
		NxForceFieldLinearKernel * CurrentKernel = ( NxForceFieldLinearKernel * ) mForceField->getForceFieldKernel() ;
		CurrentKernel->setNoise( NxVec3ToPhysx( noise ) );
	}
	else // NX_FFK_CUSTOM_KERNEL
	{

	}
}
 
void NxPhysicsForceField::SetConstant( const Nx::Vector3 & Constant )
{
	if( mForceField->getForceFieldKernel()->getType() == NX_FFK_LINEAR_KERNEL ) {
		NxForceFieldLinearKernel * CurrentKernel = ( NxForceFieldLinearKernel * ) mForceField->getForceFieldKernel() ;
		CurrentKernel->setConstant ( NxVec3( Constant.x, Constant.y, Constant.z ) ) ;	
	}
}

void NxPhysicsForceField::SetCoordinatesType( const NxEntityMagnetCoordinates Type )
{
	mForceField->setCoordinates( ( NxForceFieldCoordinates ) Type ); 
}

NxPhysicsForceFieldShape * NxPhysicsForceField::CreateIncludeShape( const NxSphereDesc & SphereDesc )
{
	NxSphereForceFieldShapeDesc SphereShape;
	SphereShape.setToDefault();
	SphereShape.radius = SphereDesc.mRadius;
	SphereShape.pose.id();
	NxPhysicsForceFieldShape * c = new NxPhysicsForceFieldShape( mForceField->getIncludeShapeGroup().createShape( SphereShape ) );
	mIncludeShape.push_back(c);
	return c;
}

NxPhysicsForceFieldShape * NxPhysicsForceField::CreateIncludeShape( const NxBoxDesc & BoxDesc )
{
	NxBoxForceFieldShapeDesc BoxShape;
	BoxShape.setToDefault();
	BoxShape.dimensions = NxVec3ToPhysx( BoxDesc.mDimensions );
	BoxShape.pose.id();
	NxPhysicsForceFieldShape * c = new NxPhysicsForceFieldShape( mForceField->getIncludeShapeGroup().createShape( BoxShape ) );
	mIncludeShape.push_back(c);
	return c;
}

NxPhysicsForceFieldShape * NxPhysicsForceField::CreateIncludeShape( NxPhysicsMeshShape * MeshShape )
{
	NxForceFieldShape * Field = 0;
	if( MeshShape->GetNxShape()->isConvexMesh() ) {
		NxConvexForceFieldShapeDesc ConvexShape;
		ConvexShape.setToDefault();
		ConvexShape.meshData = &MeshShape->GetNxShape()->isConvexMesh()->getConvexMesh();
		Field = mForceField->getIncludeShapeGroup().createShape( ConvexShape );
	}

	NxPhysicsForceFieldShape * c = new NxPhysicsForceFieldShape( Field );
	mIncludeShape.push_back(c);
	return c;
}

void NxPhysicsForceField::ReleaseIncludeShape( NxPhysicsForceFieldShape * FieldShape )
{
	std::vector<NxPhysicsForceFieldShape*>::iterator i;
	i = std::find(mIncludeShape.begin(), mIncludeShape.end(), FieldShape);
	if( i != mIncludeShape.end()) { // found
		mForceField->getIncludeShapeGroup().releaseShape( *FieldShape->GetShape() );
		mIncludeShape.erase(i);
	}

}

void NxPhysicsForceField::ReleaseIncludeShapes()
{
	for( int i = 0; i < mIncludeShape.size(); i++ ){
		ReleaseIncludeShape( mIncludeShape[i] );
	}
	mIncludeShape.clear();
}

}//namespace