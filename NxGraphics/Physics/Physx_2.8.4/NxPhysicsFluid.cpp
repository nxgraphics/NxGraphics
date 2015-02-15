#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

struct ParticleSDK
{
	Ogre::Vector3 position;
	float mPositions[3];
	float mColours[3];
	Ogre::Vector3 colour;
	Ogre::Vector3 velocity;
	float density;
	float  lifetime;
	unsigned int id;
	Ogre::Vector3 collisionNormal;
};

class MyFluid 
{
public:
	MyFluid( NxScene * scene, NxFluidDesc &desc, bool trackUserData, bool provideCollisionNormals, const NxVec3& color, float particleSize);
	~MyFluid();
	NxFluid * getNxFluid() { return mFluid; }
	const ParticleSDK * getParticles() { return mParticleBuffer; }
	unsigned int getParticlesNum() { return mParticleBufferNum; }
	const unsigned int * getCreatedIds() { return mCreatedParticleIds; }
	unsigned int getCreatedIdsNum() { return mCreatedParticleIdsNum; }
	const unsigned int * getDeletedIds() { return mDeletedParticleIds; }
	unsigned int getDeletedIdsNum() { return mDeletedParticleIdsNum; }
	void setParticleSize( float  size) { mParticleSize = size; }
	float getParticleSize() { return mParticleSize; }
	float * GetPositions(){ return &mParticleBuffer->mPositions[0]; }
	unsigned int GetMaxParticles(){ return mMaxParticles; }
private:
	unsigned int mParticleBufferNum;
	ParticleSDK * mParticleBuffer;
	NxFluid * mFluid;
	float mParticleSize;
	unsigned int mMaxParticles;
	bool mTrackUserData;
	unsigned int mCreatedParticleIdsNum;
	unsigned int * mCreatedParticleIds;
	unsigned int mDeletedParticleIdsNum;
	unsigned int * mDeletedParticleIds;
	float * mRenderBuffer;
	float * mRenderBufferUserData;
};

MyFluid::MyFluid( NxScene * scene, NxFluidDesc &desc, bool trackUserData, bool provideCollisionNormals, const NxVec3& particleColor, NxReal particleSize ):
	mParticleBufferNum(0),
	mParticleBuffer(NULL),
	mFluid(NULL),
	mTrackUserData(trackUserData),
	mCreatedParticleIdsNum(0),
	mCreatedParticleIds(NULL),
	mDeletedParticleIdsNum(0),
	mDeletedParticleIds(NULL),
	mParticleSize(particleSize),
	mRenderBuffer(NULL),
	mRenderBufferUserData(NULL)
{
	mMaxParticles = desc.maxParticles;
	mParticleBuffer = new ParticleSDK[mMaxParticles];
	desc.userData = this;

	NxParticleData particleData;
	particleData.numParticlesPtr = &mParticleBufferNum;

	particleData.bufferPos = &mParticleBuffer[0].mPositions[0];
	particleData.bufferPosByteStride = sizeof(float) * 3;

	particleData.bufferVel = &mParticleBuffer[0].velocity.x;
	particleData.bufferVelByteStride = sizeof(ParticleSDK);
	particleData.bufferDensity = &mParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(ParticleSDK);
	particleData.bufferLife = &mParticleBuffer[0].lifetime;
	particleData.bufferLifeByteStride = sizeof(ParticleSDK);
	particleData.bufferId = &mParticleBuffer[0].id;
	particleData.bufferIdByteStride = sizeof(ParticleSDK);
	particleData.bufferDensity = &mParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(ParticleSDK);

	if (provideCollisionNormals)
	{
		particleData.bufferCollisionNormal = &mParticleBuffer[0].collisionNormal.x;
		particleData.bufferCollisionNormalByteStride = sizeof(ParticleSDK);
	}
	
	desc.particlesWriteData = particleData;

	//User data buffers
	if (mTrackUserData)
	{
        mCreatedParticleIds = new NxU32[mMaxParticles];
        mDeletedParticleIds = new NxU32[mMaxParticles];

		//Setup id write data.
		NxParticleIdData idData;
		
		//Creation
		idData.numIdsPtr = &mCreatedParticleIdsNum;
		idData.bufferId = mCreatedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		desc.particleCreationIdWriteData = idData;
		
		//Deletion
		idData.numIdsPtr = &mDeletedParticleIdsNum;
		idData.bufferId = mDeletedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		desc.particleDeletionIdWriteData = idData;
	}

	assert(scene);
	mFluid = scene->createFluid(desc);
	assert(mFluid);
}

MyFluid::~MyFluid()
{
	LogMsg("~MyFluid()");
	delete[] mParticleBuffer;
	mParticleBuffer = NULL;
	mFluid->getScene().releaseFluid(*mFluid);
	
	if (mTrackUserData)
	{
		delete[] mCreatedParticleIds;
		mCreatedParticleIds = NULL;
		delete[] mDeletedParticleIds;
		mDeletedParticleIds = NULL;
	}

	delete[] mRenderBuffer;
	mRenderBuffer = 0;
}

NxPhysicsFluid::NxPhysicsFluid( NxPhysicsActor * Actor, const NxParticleDesc & Desc )
{
	LogMsg("Creating fluid ....");
	//Create a set of initial particles
	ParticleSDK * initParticles = new ParticleSDK[ Desc.NumParticles ];
	unsigned initParticlesNum = 0;

	//Setup structure to pass initial particles.
	NxParticleData initParticleData;
	initParticlesNum = 0;
	initParticleData.numParticlesPtr		= &initParticlesNum;
	initParticleData.bufferPos				= &initParticles[0].position.x;
	initParticleData.bufferPosByteStride	= sizeof(ParticleSDK);
	initParticleData.bufferVel				= &initParticles[0].velocity.x;
	initParticleData.bufferVelByteStride	= sizeof(ParticleSDK);

	//Setup fluid descriptor
	NxFluidDesc fluidDesc;
	fluidDesc.setToDefault();

	fluidDesc.maxParticles                  = Desc.NumParticles;
	fluidDesc.kernelRadiusMultiplier		= 2.0f;
	fluidDesc.restParticlesPerMeter			= 10.0f;
	fluidDesc.motionLimitMultiplier			= 3.0f;
	fluidDesc.packetSizeMultiplier			= 8;
	fluidDesc.collisionDistanceMultiplier   = 0.12;
	fluidDesc.stiffness						= 50.0f;
	fluidDesc.viscosity						= 40.0f;
	fluidDesc.restDensity					= 1000.0f; // Target density for the fluid (water is about 1000).
	fluidDesc.damping						= 0.0f;

	//Must be between 0 and 1.
	//
	//A value of 0 causes the colliding particle to get a zero velocity component in the
	//direction of the surface normal of the static shape at the collision location; i.e.
	//it will not bounce.
	//
	//A value of 1 causes a particle's velocity component in the direction of the surface normal to invert;
	//i.e. the particle bounces off the surface with the same velocity magnitude as it had before collision. 
	//(Caution: values near 1 may have a negative impact on stability)
	fluidDesc.restitutionForStaticShapes	= 1.0f;

	//Must be between 0 and 1.
	//
	//A value of 1 will cause the particle to lose its velocity tangential to
	//the surface normal of the shape at the collision location; i.e. it will not slide
	//along the surface.
	//
	//A value of 0 will preserve the particle's velocity in the tangential surface
	//direction; i.e. it will slide without resistance on the surface.
	fluidDesc.dynamicFrictionForStaticShapes= 1.0f;

	//brief Defines the restitution coefficient used for collisions of the fluid particles with dynamic shapes.
	//Must be between 0 and 1.
	//(Caution: values near 1 may have a negative impact on stability)
	fluidDesc.restitutionForDynamicShapes = 0.2f;
	fluidDesc.restitutionForStaticShapes	= 0.5f;
	fluidDesc.dynamicFrictionForStaticShapes = 0.05f;
	fluidDesc.staticFrictionForStaticShapes = 0.05f;
	fluidDesc.attractionForStaticShapes	= 0.0f;
	fluidDesc.restitutionForDynamicShapes	= 0.5f;
	fluidDesc.dynamicFrictionForDynamicShapes = 0.5f;
	fluidDesc.staticFrictionForDynamicShapes = 0.5f;
	fluidDesc.attractionForDynamicShapes	= 0.0f;
	fluidDesc.collisionResponseCoefficient = 0.2f;
	fluidDesc.collisionMethod  = NX_F_STATIC|NX_F_DYNAMIC;
	fluidDesc.simulationMethod = NX_F_NO_PARTICLE_INTERACTION;//NX_F_NO_PARTICLE_INTERACTION;//NX_F_MIXED_MODE;// ;
	//NX_F_MIXED_MODE : has too much presure !

	//brief Acceleration (m/s^2) applied to all particles at all time steps. Useful to simulate smoke or fire.
	//This acceleration is additive to the scene gravity. The scene gravity can be turned off 
	//for the fluid, using the flag NX_FF_DISABLE_GRAVITY.
	//@see NxFluid.getExternalAcceleration() NxFluid.setExternalAcceleration()
	fluidDesc.externalAcceleration = NxVec3( 0,1,0 );

	//fluidDesc.flags &= ~NX_FF_VISUALIZATION; // visualize or not
	fluidDesc.flags |= NX_FF_HARDWARE;

	if (!NxScene3DPhysicsManager::getSingleton().HasPhysxHardwareAcceleration()){ 
		fluidDesc.flags &= ~NX_FF_HARDWARE; LogMsg("Fluids created in software mode.");	
	}else{ LogMsg("Fluids created in Hardware mode."); }	

	fluidDesc.initialParticleData = initParticleData; //Add initial particles to fluid creation.

	//AxisAlignedBox aabBounds;
	//aabBounds.setInfinite();
	//ParticleBase->setBounds( aabBounds );

	bool trackUserData = false;
	bool provideCollisionNormals = false;

	float particlesize = 0.001f; // 0.03 f
	NxVec3 particleColor = NxVec3(0.4f,0.5f,0.9f);

	Actor->GetNxActor()->raiseActorFlag( NX_AF_FLUID_DISABLE_COLLISION );

	mFluid = new MyFluid( &Actor->GetNxActor()->getScene(), fluidDesc, trackUserData, provideCollisionNormals, particleColor , particlesize);
	LogMsg("Creating fluid done");
	assert( Fluid );

	delete[] initParticles;
}

NxPhysicsFluid::~NxPhysicsFluid()
{
	LogMsg("~NxPhysicsFluid()");

	LogMsg("Erasing NxEmitters ... " );
	NxEmitterVectorIterator NxEmittersIter = GetNxEmitterIterator();
	while( NxEmittersIter.hasMoreElements() ){
		delete NxEmittersIter.getNext();
	}
	LogMsg("Erasing NxEmitters : Done " );

	delete mFluid;
}

unsigned int NxPhysicsFluid::GetMaxParticles()
{
	return mFluid->GetMaxParticles();
}

NxPhysicsFluidEmitter * NxPhysicsFluid::CreateEmitter( NxPhysicsShape * Shape )
{
	NxPhysicsFluidEmitter * c = new NxPhysicsFluidEmitter( this, Shape );
	mNxEmitters.push_back( c );
	return c;	
}

NxPhysicsFluid::NxEmitterVectorIterator NxPhysicsFluid::GetNxEmitterIterator()
{
	return NxPhysicsFluid::NxEmitterVectorIterator( mNxEmitters.begin(), mNxEmitters.end());
}

NxFluid * NxPhysicsFluid::GetNxFluid()
{
	return mFluid->getNxFluid();
}

void NxPhysicsFluid::SetExternalAcceleration( const Nx::Vector3& acceleration )
{
	mFluid->getNxFluid()->setExternalAcceleration( NxVec3ToPhysx( acceleration )  );
}

void NxPhysicsFluid::SetGravity( bool GravityAffected )
{
	mFluid->getNxFluid()->setFlag( NxFluidFlag::NX_FF_DISABLE_GRAVITY, !GravityAffected );
}

void NxPhysicsFluid::SetViscosity( float Viscosity )
{
	mFluid->getNxFluid()->setViscosity( Viscosity );
}

void NxPhysicsFluid::SetRestitutionForStaticShapes( float rest )
{
	mFluid->getNxFluid()->setRestitutionForStaticShapes( rest );
}

void NxPhysicsFluid::SetDynamicFrictionForStaticShapes( float Friction )
{
	mFluid->getNxFluid()->setDynamicFrictionForStaticShapes( Friction );
}

void NxPhysicsFluid::SetStaticFrictionForStaticShapes( float Friction )
{
	mFluid->getNxFluid()->setStaticFrictionForStaticShapes( Friction );
}

void NxPhysicsFluid::SetRestitutionForDynamicShapes( float rest )
{
	mFluid->getNxFluid()->setRestitutionForDynamicShapes( rest );
}

void NxPhysicsFluid::SetDynamicFrictionForDynamicShapes( float friction )
{
	mFluid->getNxFluid()->setDynamicFrictionForDynamicShapes( friction );
}

void NxPhysicsFluid::SetStaticFrictionForDynamicShapes( float friction )
{
	mFluid->getNxFluid()->setStaticFrictionForDynamicShapes( friction );
}

void NxPhysicsFluid::SetCollisionResponseCoefficient( float coefficient )
{
	mFluid->getNxFluid()->setCollisionResponseCoefficient( coefficient );
}

const float * NxPhysicsFluid::GetParticlePositions() const
{
	return mFluid->GetPositions();
}

unsigned int NxPhysicsFluid::GetNumParticles()
{
	return mFluid->getParticlesNum();
}

}//namepsace
