#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

NxPhysicsFluidEmitter::NxPhysicsFluidEmitter( NxPhysicsFluid * Fluid, NxPhysicsShape * Shape )
{

	 
	LogMsg("Creating Emitter ....");
	NxFluidEmitterDesc emitterDesc;
	emitterDesc.setToDefault();
	emitterDesc.maxParticles = 0;  // If set to 0, the number of emitted particles is unrestricted.
	emitterDesc.shape = NX_FE_RECTANGULAR ; // NX_FE_ELLIPSE or NX_FE_RECTANGULAR	 
	emitterDesc.dimensionX = 0.2;
	emitterDesc.dimensionY = 0.2;
	//float MaxOpeningAngle = 90; // 0 degres / max random
	//emitterDesc.randomAngle = MaxOpeningAngle * Ogre::Math::PI / 180.0f  ;// radians
	//Either the simulation enforces constant pressure or constant flow rate at the emission site, 
	//given the velocity of emitted particles.
	emitterDesc.type = NX_FE_CONSTANT_FLOW_RATE; //The rate has no effect with type NX_FE_CONSTANT_PRESSURE

	LogMsg( "Max particles ::: " + Ogre::StringConverter::toString(  Fluid->GetMaxParticles() )  );
	// Particles per second
	emitterDesc.rate = Fluid->GetMaxParticles() / 2.0f;
	// This specifies the time in seconds an emitted particle lives. 
	emitterDesc.particleLifetime = 2.0f;  
	emitterDesc.fluidVelocityMagnitude = 1.0f;

	emitterDesc.shape = NX_FE_RECTANGULAR;//NX_FE_RECTANGULAR;
	//emitterDesc.flags |= NX_FEF_ADD_BODY_VELOCITY;
	emitterDesc.flags |= NX_FEF_FORCE_ON_BODY;

	//Defines a factor for the impulse transfer from attached emitter to body.Only has an effect if NX_FEF_FORCE_ON_BODY is set.Default:1.0 , Range: [0,inf)
	emitterDesc.repulsionCoefficient = 0.1f;

	NxQuat q;
	q.fromAngleAxis( -90.0f , NxVec3( 1.0f, 0.0f, 0.0f ) );
	NxMat34 mat;
	mat.M.fromQuat(q);
	mat.t = NxVec3( 0.0f, 0.0f, 0.0f );
	emitterDesc.relPose = mat;
	
	LogMsg("Shape to Emitter...");
	emitterDesc.frameShape = Shape ? Shape->GetNxShape() : 0;
	if( !emitterDesc.isValid() ){ LogMsg("emitter isnt valid ! more than 65535 particles");  }

	mEmitter = Fluid->GetNxFluid()->createEmitter( emitterDesc );
	LogMsg("Creating Emitter : Done");
 
}

NxPhysicsFluidEmitter::~NxPhysicsFluidEmitter()
{
	LogMsg("~NxPhysicsFluidEmitter ... ");
	NxFluid  * Ref = &mEmitter->getFluid(); 
	Ref->releaseEmitter( *mEmitter );
	LogMsg("NxPhysicsFluidEmitter deleted ");
}

void NxPhysicsFluidEmitter::SetLocalPosition( const Nx::Vector3 & Position )
{
	mEmitter->setLocalPosition( NxVec3ToPhysx( Position ) );
}

const Nx::Vector3 & NxPhysicsFluidEmitter::GetLocalPosition() const
{
	return PhysxToNxVec3( mEmitter->getLocalPosition() );
}

void NxPhysicsFluidEmitter::SetLocalOrientation( const Nx::Quaternion & Orientation )
{
	mEmitter->setLocalOrientation ( NxQuatToPhysx( Orientation ) );
}

const Nx::Quaternion & NxPhysicsFluidEmitter::GetLocalOrientation() const
{
	return PhysxToQuatNx( mEmitter->getLocalOrientation() );
}

void NxPhysicsFluidEmitter::SetGlobalPosition( const Nx::Vector3 & Position )
{
	mEmitter->setGlobalPosition( NxVec3ToPhysx( Position ) );
}

const Nx::Vector3 & NxPhysicsFluidEmitter::GetGlobalPosition() const
{
	return PhysxToNxVec3( mEmitter->getGlobalPosition() );
}

void NxPhysicsFluidEmitter::SetGlobalOrientation( const Nx::Quaternion & Orientation )
{
	mEmitter->setGlobalOrientation ( NxQuatToPhysx( Orientation ) );
}

const Nx::Quaternion & NxPhysicsFluidEmitter::GetGlobalOrientation() const
{
	return PhysxToQuatNx( mEmitter->getGlobalOrientation() );
}

void NxPhysicsFluidEmitter::SetMaxAngle( float Angle )
{
	mEmitter->setRandomAngle( Angle * Ogre::Math::PI / 180.0f ); // in radians
}

void NxPhysicsFluidEmitter::SetRepulsionCoefficient( float coefficient )
{
	mEmitter->setRepulsionCoefficient( coefficient );
}

float NxPhysicsFluidEmitter::GetRepulsionCoefficient() const 
{
	return mEmitter->getRepulsionCoefficient();
}

void NxPhysicsFluidEmitter::SetParticlesLifeTime( float lifeTime )
{
	mEmitter->setParticleLifetime( lifeTime );
}

float NxPhysicsFluidEmitter::GetParticleLifeTime()
{
	return mEmitter->getParticleLifetime();
}

void NxPhysicsFluidEmitter::SetVelocityMagnitude( float Velocity )
{
	mEmitter->setFluidVelocityMagnitude( Velocity );
}

void NxPhysicsFluidEmitter::SetParticlesRate( float Rate )
{
	mEmitter->setRate( Rate );
}

void NxPhysicsFluidEmitter::SetEnabled( bool Enabled )
{
	// Resets the particle reservoir. 
	//NxEmitter->resetEmission( mEmitter->getMaxParticles() );
	// Flag to start and stop the emission. On default the emission is enabled.
	mEmitter->setFlag( NxFluidEmitterFlag::NX_FEF_ENABLED, Enabled );
}

}//namespace
