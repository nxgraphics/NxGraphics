#include <NxGraphicsPreCompiled.h>

namespace Nx {

NxPhysicsCapsuleShape::NxPhysicsCapsuleShape( NxPhysicsActor * Actor, const NxCapsuleDesc & Desc ) : NxPhysicsShape( Actor, Desc )
{
 
}

NxPhysicsCapsuleShape::~NxPhysicsCapsuleShape()
{

}

float NxPhysicsCapsuleShape::GetRadius()
{
 return 0.0f;
}

float NxPhysicsCapsuleShape::GetHeight()
{
 return 0.0f;
}

}