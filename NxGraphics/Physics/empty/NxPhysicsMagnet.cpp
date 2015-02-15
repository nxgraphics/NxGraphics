#include <NxGraphicsPreCompiled.h>

namespace Nx {

NxPhysicsForceFieldShape::NxPhysicsForceFieldShape( NxForceFieldShape * FieldShape ) 
{

}

NxPhysicsForceFieldShape::~NxPhysicsForceFieldShape()
{
	//Log("Calling ~NxPhysicsForceFieldShape()");
}

NxForceFieldShape * NxPhysicsForceFieldShape::GetShape()
{
	return 0;
}

NxPhysicsForceField::NxPhysicsForceField( NxPhysicsActor * Actor )
{

}

NxPhysicsForceField::~NxPhysicsForceField()
{

}

// Sets the coefficient of force field function position term.
void NxPhysicsForceField::SetPositionMultiplier( const Nx::Vector3 & Force )
{
	
}

// Sets the coefficient of force field function velocity term.
void NxPhysicsForceField::SetVelocityMultiplier( const Nx::Vector3 & Force )
{
	
}

void NxPhysicsForceField::SetPositionTarget( const Nx::Vector3 & PosTarget )
{
	
}

void NxPhysicsForceField::SetVelocityTarget( const Nx::Vector3 & VeloTarget )
{
	
}

void NxPhysicsForceField::SetNoise( const Nx::Vector3 & noise )
{

}
 
void NxPhysicsForceField::SetConstant( const Nx::Vector3 & Constant )
{
	
}

void NxPhysicsForceField::SetCoordinatesType( const NxEntityMagnetCoordinates Type )
{
	
}

NxPhysicsForceFieldShape * NxPhysicsForceField::CreateIncludeShape( const NxSphereDesc & SphereDesc )
{

	return 0;
}

NxPhysicsForceFieldShape * NxPhysicsForceField::CreateIncludeShape( const NxBoxDesc & BoxDesc )
{

	return 0;
}

NxPhysicsForceFieldShape * NxPhysicsForceField::CreateIncludeShape( NxPhysicsMeshShape * MeshShape )
{

	return 0;
}

void NxPhysicsForceField::ReleaseIncludeShape( NxPhysicsForceFieldShape * FieldShape )
{

}

void NxPhysicsForceField::ReleaseIncludeShapes()
{

}

}//namespace