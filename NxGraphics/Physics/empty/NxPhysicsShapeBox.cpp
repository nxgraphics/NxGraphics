#include <NxGraphicsPreCompiled.h>

namespace Nx {

NxPhysicsBoxShape::NxPhysicsBoxShape( NxPhysicsActor * Actor, const NxBoxDesc & Desc ) : NxPhysicsShape( Actor, Desc )
{

}

NxPhysicsBoxShape::~NxPhysicsBoxShape()
{

}

void NxPhysicsBoxShape::SetDimensions( const Nx::Vector3 & Dimension )
{

}

const Nx::Vector3 & NxPhysicsBoxShape::GetDimensions() const
{
	
	return Nx::Vector3(0,0,0);
}

}
