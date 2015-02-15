/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __NXGRAPHICSPHYSICS_H__
#define __NXGRAPHICSPHYSICS_H__

#include <NxPrerequisites.h>


class NxForceFieldShape;
class NxForceField;
class NxShape;
class NxFluidEmitter;
class NxFluid;
class NxD6Joint;
class NxRevoluteJoint;
class NxActor;
class NxController;
class NxScene;

namespace Nx {

enum NxShapeTypes
{  
	ShapeBox,
	ShapeSphere,
	ShapeCapsule,
	ShapePlane,
	ShapeMesh
};

class NxGraphics_Export NxBaseDesc
{
	NxShapeTypes mType;
protected:
	NxBaseDesc( NxShapeTypes Type ) : mType( Type ), mlocalPosition( Nx::Vector3(0,0,0) ), mlocalOrientation( Nx::Quaternion::IDENTITY ){}
public :
	const NxShapeTypes GetType() const { return mType; }
	Nx::Vector3 mlocalPosition;
	Nx::Quaternion mlocalOrientation;
};

class NxGraphics_Export NxBoxDesc : public NxBaseDesc
{
public:
	NxBoxDesc() : NxBaseDesc( ShapeBox ){}
	Nx::Vector3 mDimensions; 
};

class NxGraphics_Export NxSphereDesc : public NxBaseDesc
{
public:
	NxSphereDesc() : NxBaseDesc( ShapeSphere ){}
	float mRadius; 
};

class NxGraphics_Export NxPlaneDesc : public NxBaseDesc
{
public:
	NxPlaneDesc() : NxBaseDesc( ShapePlane ){}
	Nx::Vector2 mDimensions; 
};

class NxGraphics_Export NxCapsuleDesc : public NxBaseDesc
{
public:
	NxCapsuleDesc() : NxBaseDesc( ShapeCapsule ){}
	float Radius;
	float Height;
};

class NxGraphics_Export NxEntityDesc : public NxBaseDesc
{
public:
	NxEntityDesc() : NxBaseDesc( ShapeMesh ), mUseConvex(false){}
	NxEntity * mNxEntity;
	bool mUseConvex;
};

class NxGraphics_Export NxParticleDesc
{
public:
	NxParticleDesc() : NumParticles(256){}
	unsigned int NumParticles;
};

class NxGraphics_Export NxClotheDesc
{
public:
	NxClotheDesc(){}
	Ogre::Entity * mEntity;
};

class NxGraphics_Export NxPhysicsForceFieldShape
{
public :
	NxPhysicsForceFieldShape( NxForceFieldShape * FieldShape );
	~NxPhysicsForceFieldShape();
	NxForceFieldShape * GetShape();
private :
	NxForceFieldShape * mForceField;
};

class NxPhysicsShape;
class NxPhysicsMeshShape;
class NxGraphics_Export NxPhysicsForceField
{
public :
	//! NxPhysicsForceField Class Constructor.
	NxPhysicsForceField( NxPhysicsActor * Actor );
	//! NxPhysicsForceField Class Destructor.
	~NxPhysicsForceField();
	//! Set Position Multiplier.
	void SetPositionMultiplier( const Nx::Vector3 & Force );
	//! Set Velocity Multiplier.
	void SetVelocityMultiplier( const Nx::Vector3 & Force );
	//! set position target.
	void SetPositionTarget( const Nx::Vector3 & PosTarget );
	//! set Velocity target.
	void SetVelocityTarget( const Nx::Vector3 & VeloTarget );
	//! Set Noise
	void SetNoise( const Nx::Vector3 & noise );
	//! Set Magnet coordinate type.
	void SetCoordinatesType( const NxEntityMagnetCoordinates Type );
	//! Set Magnet Constant.
	void SetConstant( const Nx::Vector3 & Constant );
	//! Create a box include Shape.
	NxPhysicsForceFieldShape * CreateIncludeShape( const NxBoxDesc & BoxDesc );
	//! Create a sphere include Shape.
	NxPhysicsForceFieldShape * CreateIncludeShape( const NxSphereDesc & SphereDesc );
	//! create a forcefiled shape from include shapes.
	NxPhysicsForceFieldShape * CreateIncludeShape( NxPhysicsMeshShape * MeshShape );
	//! remove forcefiled shape from include shapes.
	void ReleaseIncludeShape( NxPhysicsForceFieldShape * FieldShape );
	//! release include shapes.
	void ReleaseIncludeShapes();
private:
	NxForceField * mForceField;
	std::vector< NxPhysicsForceFieldShape * > mIncludeShape;
};
 
class NxGraphics_Export NxPhysicsShape : public CountedObj<NxPhysicsShape>
{
protected :
	//! NxPhysicsShape Class Constructor.
	NxPhysicsShape( NxPhysicsActor * Actor, const NxBaseDesc & Desc );
public :
	//! NxPhysicsShape Class Destructor.
	virtual ~NxPhysicsShape();
	//! Set Skin Width.
	void SetSkinWidth( float SkinWidth );
	//! Get Skin Width.
	float GetSkinWidth() const;
	//! Set Shape Local Position.
	void SetLocalPosition( const Ogre::Vector3 & Position );
	//! Get Shape Local Position.
	const Ogre::Vector3 & GetLocalPosition() const;
	//! Set Shape Global Position.
	void SetGlobalPosition( const Ogre::Vector3 & Position );
	//! Get Shape Global Position.
	const Ogre::Vector3 & GetGlobalPosition() const;
	//! Set Shape Local Orientation.
	void SetLocalOrientation( const Ogre::Quaternion & Orientation );
	//! Set Shape Local Orientation.
	const Ogre::Quaternion & GetLocalOrientation() const;
	//! Set Shape Global Orientation.
	void SetGlobalOrientation( const Ogre::Quaternion & Orientation );
	//! Get Shape Global Orientation.
	const Ogre::Quaternion & GetGlobalOrientation() const;
	//! Set Contact Pair With Shape
	void SetShapePairFlags( NxPhysicsShape * shape, int flags );
	//! set group
	void SetGroup( int group );
	//! Create a Force Field for the Current Shape.
	//NxPhysicsForceField * CreateForceField();
	//! Get Shape Type.
	const NxShapeTypes GetType();
	//! Get Physx Shape.
	NxShape * GetNxShape();
protected :
	//! 
	NxShapeTypes mType;
	//! Physx Shape.
	NxShape * mShape;
};

class NxPhysicsBoxShape : public NxPhysicsShape
{
public :
	//! NxPhysicsBoxShape Class Constructor.
	NxPhysicsBoxShape( NxPhysicsActor * Actor, const NxBoxDesc & Desc );
	//! NxPhysicsBoxShape Class Destructor.
	~NxPhysicsBoxShape();
	//! Set Box Dimensions.
	void SetDimensions( const Nx::Vector3 & Dimension );
	//! Get Box Dimensions.
	const Nx::Vector3 & GetDimensions() const;
};

class NxPhysicsPlaneShape : public NxPhysicsShape
{
public :
	//! NxPhysicsPlaneShape Class Constructor.
	NxPhysicsPlaneShape( NxPhysicsActor * Actor, const NxPlaneDesc & Desc );
	//! NxPhysicsPlaneShape Class Destructor.
	~NxPhysicsPlaneShape();
};

class NxPhysicsCapsuleShape : public NxPhysicsShape
{
public :
	//! NxPhysicsBoxShape Class Constructor.
	NxPhysicsCapsuleShape( NxPhysicsActor * Actor, const NxCapsuleDesc & Desc );
	//! NxPhysicsBoxShape Class Destructor.
	~NxPhysicsCapsuleShape();
	//! Get Radius.
	float GetRadius();
	//! Get Height.
	float GetHeight();
};

class NxPhysicsMeshShape : public NxPhysicsShape
{
public:
	//! NxPhysicsMeshShape Class Constructor.
	NxPhysicsMeshShape( NxPhysicsActor * Actor, const NxEntityDesc & Desc );
	//! NxPhysicsMeshShape Class Destructor.
	~NxPhysicsMeshShape();
};

class NxGraphics_Export NxPhysicsFluidEmitter
{
public :
	//! NxPhysicsFluidEmitter Class Constructor.
	NxPhysicsFluidEmitter( NxPhysicsFluid * Fluid, NxPhysicsShape * Shape );
	//! NxPhysicsFluidEmitter Class Destructor.
	~NxPhysicsFluidEmitter();
	//! Set Emitter Local Position.
	void SetLocalPosition( const Nx::Vector3 & Position );
	//! Get Emitter Local Position.
	const Nx::Vector3 & GetLocalPosition() const;
	//! Set Local Orientation.
	void SetLocalOrientation( const Nx::Quaternion & Orientation );
	//! Get Local Orientation.
	const Nx::Quaternion & GetLocalOrientation() const;
	//! Set Emitter Global Position.
	void SetGlobalPosition( const Nx::Vector3 & Position );
	//! Get Emitter Global Position.
	const Nx::Vector3 & GetGlobalPosition() const;
	//! Set Emitter Global Orientation.
	void SetGlobalOrientation( const Nx::Quaternion & Orientation );
	//! Get Emitter Global Orientation.
	const Nx::Quaternion & GetGlobalOrientation() const;
	//! Set Emitter Max Angle.
	void SetMaxAngle( float Angle );
	//! coefficient The repulsion coefficient in the range from 0 to inf.
	void SetRepulsionCoefficient( float coefficient );
	//! get The repulsion coefficient.
	float GetRepulsionCoefficient() const ;
	//! Set Particle Life Time.
	void SetParticlesLifeTime( float lifeTime );
	//! Get Particles Life time.
	float GetParticleLifeTime();
	//! Set Velocity magnitude.
	void SetVelocityMagnitude( float Velocity );
	//! Set Particles rate : Amount / Second.
	void SetParticlesRate( float Rate );
	//! Enable / Disable the Emitter.
	void SetEnabled( bool Enabled );
private :
	//! NxFluidEmitter.
	NxFluidEmitter * mEmitter;
};

class NxGraphics_Export NxPhysicsFluid 
{
	typedef vector<NxPhysicsFluidEmitter*>::type NxEmitterVector;
	typedef NxVectorIterator<NxEmitterVector> NxEmitterVectorIterator;
public :
	//! NxPhysicsFluidShape Class Constructor.
	NxPhysicsFluid( NxPhysicsActor * Actor, const NxParticleDesc & Desc );
	//! NxPhysicsFluidShape Class Destructor.
	~NxPhysicsFluid();
	//! Create Emitter;
	NxPhysicsFluidEmitter * CreateEmitter( NxPhysicsShape * Shape );
	//! Get NxEmitter iterator;
	NxEmitterVectorIterator GetNxEmitterIterator();
	//! Get NxFluid
	NxFluid * GetNxFluid();
	//! Get NxFluid Max Particles.
	unsigned int GetMaxParticles();
	//! Get Particle Positions.
	const float * GetParticlePositions() const;
	//! Get Current number of emitted particles.
	unsigned int GetNumParticles();
	//! Set particles external force / direction
	void SetExternalAcceleration( const Nx::Vector3& acceleration );
	//! Particles affected to gravity ?
	void SetGravity( bool GravityAffected );
	//! Set Viscosiy.
	void SetViscosity( float Viscosity );
	//! Sets the restitution used for collision with static shapes. Must be between 0 and 1.
	void SetRestitutionForStaticShapes( float rest );
	//! Sets the dynamic friction used for collision with static actors. Must be between 0 and 1.
	void SetDynamicFrictionForStaticShapes( float Friction );
	//! Sets the static friction used for collision with static actors. Range: [0,inf] default : 0
	void SetStaticFrictionForStaticShapes( float Friction );
	//! Sets the restitution used for collision with dynamic actors. Must be between 0 and 1.	
	void SetRestitutionForDynamicShapes( float rest );
	//! Sets the dynamic friction used for collision with dynamic shapes. Must be between 0 and 1.
	void SetDynamicFrictionForDynamicShapes( float friction );
	//! Sets the static friction used for collision with dynamic shapes. range 0 - inf , default : 0
	void SetStaticFrictionForDynamicShapes( float friction );
	//! coefficient The collision response coefficient (0 or greater).
	void SetCollisionResponseCoefficient( float coefficient );
private:
	//! Physx Fluid
	MyFluid * mFluid;
	//! Physx Fluid.
	NxParticles * mFluidObject;
	//! NxEmitters List.
	NxEmitterVector mNxEmitters;
};

/////////// Joints ////////////

enum NxJointParams
{
	// Raised if collision detection should be enabled between the bodies attached to the joint.
	// By default collision constraints are not generated between pairs of bodies which are connected by joints.
    NxJointCollisionEnabled	= (1<<0),

	// Enable debug renderer for this joint
    NxJointVisualization = (1<<1)    
};

enum NxJointTypes
{
	NxJointPrismatic, // Permits a single translational degree of freedom.
	NxJointRevolute, // Also known as a hinge joint, permits one rotational degree of freedom.
	NxJointCylindrical, // Formerly known as a sliding joint, permits one translational and one rotational degree of freedom.
	NxJointSpherical, // Also known as a ball or ball and socket joint.
	NxJointPointOnLine, // A point on one actor is constrained to stay on a line on another.
	NxJointPointInPlane, // A point on one actor is constrained to stay on a plane on another.
	NxJointDistance, // A point on one actor maintains a certain distance range to another point on another actor.
	NxJointPulley,  // A pulley joint.
	NxJointFixed,   // A "fixed" connection.
	NxJointD6       // A 6 degree of freedom joint
};

#define MAX_REAL 3.402823466e+38F
class NxGraphics_Export NxJointBaseDesc
{
	NxJointTypes mType;
protected:
	NxJointBaseDesc( NxJointTypes Type ) : mType( Type ){
		SetToDefault();
	}
public :
	//! Get Type
	const NxJointTypes GetType() const { return mType; }

	inline virtual void SetToDefault() {
		for (int i=0; i<2; i++) {
			mActor[i] = 0;
			localAxis[i] = Nx::Vector3(0,0,1);
			localNormal[i] = Nx::Vector3(1,0,0);
			localAnchor[i] = Nx::Vector3::ZERO;
		}

		maxForce	= MAX_REAL;
		maxTorque	= MAX_REAL;
		solverExtrapolationFactor = 1.0f;
		useAccelerationSpring = 0;
		mParams	= NxJointVisualization;
	}

	//! Actors
	NxPhysicsActor * mActor[2];

	// X axis of joint space, in actor[i]'s space, orthogonal to localAxis[i]
	// localAxis and localNormal should be unit length and at right angles to each other, i.e. 
	// dot(localNormal[0],localAxis[0])==0 and dot(localNormal[1],localAxis[1])==0.
	// Range: direction vector
	// Default:  [0] 1.0f,0.0f,0.0f 
	Nx::Vector3 localNormal[2];

	// Z axis of joint space, in actor[i]'s space. This is the primary axis of the joint.
	// localAxis and #localNormal should be unit length and at right angles to each other, i.e. 
	// dot(localNormal[0],localAxis[0])==0 and dot(localNormal[1],localAxis[1])==0.
	// direction vector<br>
	// Default:</b> [0] 0.0f,0.0f,1.0f<br>
	Nx::Vector3 localAxis[2];

	// Attachment point of joint in actor[i]'s space
	//Range: position vector
	// Default: [0] Zero
	Nx::Vector3 localAnchor[2];

	// Maximum linear force that the joint can withstand before breaking, must be positive.
	float maxForce;

	// Maximum angular force (torque) that the joint can withstand before breaking, must be positive. 
	float maxTorque;

	// Extrapolation factor for solving joint constraints.
	// This parameter can be used to build stronger joints and increase the solver convergence. Higher values
	// lead to stronger joints.
	// Setting the value too high can decrease the joint stability.
	// Currently, this feature is supported for D6, Revolute and Spherical Joints only.
	float solverExtrapolationFactor;

	// Switch to acceleration based spring.
	// This parameter can be used to switch between acceleration and force based spring. Acceleration
	// based springs do not take the mass of the attached objects into account, i.e., the spring/damping
	// behaviour will be independent of the load.
	// Currently, this feature is supported for D6, Revolute and Spherical Joints only.
	// Range:</b> {0: use force spring, 1: use acceleration spring}<br>
	// Default:</b> 0
	unsigned int useAccelerationSpring;

	//Global anchor.
	Nx::Vector3 mGlobalAnchor;

	//Global Axis.
	Nx::Vector3 mGlobalAxis;

	//! Joint Params
	NxJointParams mParams;
};

//
//\brief Joint projection modes.
//
//Joint projection is a method for correcting large joint errors.
//
//It is also necessary to set the distance above which projection occurs.
//
//@see NxRevoluteJointDesc.projectionMode NxRevoluteJointDesc.projectionDistance NxRevoluteJointDesc.projectionAngle
//@see NxSphericalJointDesc.projectionMode
//@see NxD6JointDesc.projectionMode
 
enum NxJointProjectionDesc
{
	NxJointProjectionDescNone,				//!< don't project this joint
	NxJointProjectionDescPointMinDist,		//!< linear and angular minimum distance projection
	NxJointProjectionDescLinearMinDist 		//!< linear only minimum distance projection
	//there may be more modes later
};


enum NxJointD6Motion
{
	NxJointD6MotionMotionLocked,	//!< The DOF is locked, it does not allow relative motion.
	NxJointD6MotionMotionLimited,  //!< The DOF is limited, it only allows motion within a specific range.
	NxJointD6MotionMotionFree      //!< The DOF is free and has its full range of motions.
};

class NxJointLimitedDesc
{
public :
	 //Default:</b> 0.0
	float value;
	// limit bounce //Range: [0,1]  //Default: 0.0
	float restitution;
	//[not yet implemented!] limit can be made softer by setting this to less than 1.
	// Range:</b> [0,1]<br>
	// Default:</b> 1.0
	float hardness;

	inline NxJointLimitedDesc() {
		value = 0;
		restitution = 0;
		hardness = 1;
	}
};

class NxJointSoftLimitDesc
{
public:
	float value;
	float restitution;
	float spring;
	float damping;
	inline NxJointSoftLimitDesc() {
		value = 0;
		restitution = 0;
		spring = 0;
		damping = 0;
	}
};

class NxJointLimitPairSoftDesc
{
public:
	NxJointSoftLimitDesc low;
	NxJointSoftLimitDesc high;		
	inline NxJointLimitPairSoftDesc()
	{
	
	}
};

class NxJointPairLimitDesc
{
public :
	NxJointLimitedDesc low;
	NxJointLimitedDesc high;

	inline NxJointPairLimitDesc()
	{

	}
};

class NxJointDescDrive
{
public:
	//NxBitField32 driveType;
	float spring;
	float damping;
	float forceLimit;
	NxJointDescDrive() {
		spring = 0;
		damping = 0;
		forceLimit = FLT_MAX;
		//driveType = 0;
	}
};

enum NxJointProjection
{
	NxJointProjectionNone  = 0,				//!< don't project this joint
	NxJointProjectionPointMinDist = 1,		//!< linear and angular minimum distance projection
	NxJointProjectionLinearMinDist = 2		//!< linear only minimum distance projection
};

class NxGraphics_Export NxJointD6Desc : public NxJointBaseDesc
{
public :
	NxJointD6Desc() : NxJointBaseDesc( NxJointD6 ) {
		xMotion = NxJointD6MotionMotionLocked;
		yMotion  = NxJointD6MotionMotionLocked;
		zMotion = NxJointD6MotionMotionLocked; 
		swing1Motion = NxJointD6MotionMotionLocked;
		swing2Motion  = NxJointD6MotionMotionLocked;
		twistMotion = NxJointD6MotionMotionLocked;
	}

	NxJointD6Motion xMotion, yMotion, zMotion; 
	NxJointD6Motion swing1Motion, swing2Motion, twistMotion;
	NxJointSoftLimitDesc linearLimit;
	NxJointSoftLimitDesc swing1Limit; 
	NxJointSoftLimitDesc swing2Limit;
	NxJointLimitPairSoftDesc twistLimit;
	NxJointDescDrive xDrive, yDrive, zDrive;
	NxJointDescDrive swingDrive, twistDrive;
	NxJointDescDrive slerpDrive;
	Nx::Vector3 drivePosition;
	Nx::Quaternion driveOrientation;
	Nx::Vector3 driveLinearVelocity;
	Nx::Vector3 driveAngularVelocity;
	NxJointProjection projectionMode;
	float projectionDistance;	
	float projectionAngle;	
	float gearRatio;

};

class NxGraphics_Export NxMotorDescription
{
	public:

	// The relative velocity the motor is trying to achieve.
	//The motor will only be able to reach this velocity if the maxForce is sufficiently large.
	//If the joint is spinning faster than this velocity, the motor will actually try to brake(see #freeSpin). 
	//If you set this to infinity then the motor will keep speeding up, unless there is some sort 
	//of resistance on the attached bodies. The sign of this variable determines the rotation direction,
	//with positive values going the same way as positive joint angles.

	//<b>Range:</b> [0,inf]<br>
	//<b>Default:</b> NX_MAX_REAL
	float velTarget;	//target velocity of motor

 
	// The maximum force (or torque) the motor can exert.
	
	//Zero disables the motor.
	//Default is 0, should be >= 0. Setting this to a very large value if velTarget is also 
	//very large may cause unexpected results.
	// Range: [0,inf) 
	// Default: 0.0
	float maxForce;	//maximum motor force/torque

	//If true, motor will not brake when it spins faster than velTarget
	//Default:</b> false
	int freeSpin;

	inline NxMotorDescription() {
		velTarget = sqrtf( 3.402823466e+38F );
		maxForce = 0;
		freeSpin = 0;
	}
};


class NxGraphics_Export NxSpringDescription
{
	public:

	// spring coefficient
	// Range:</b> [0,inf)<br>
	// Default:</b> 0
	float spring;

	//damper coefficient
	 //Range:</b> [0,inf)<br>
	 //Default:</b> 0
	float damper;

	 //target value (angle/position) of spring where the spring force is zero.
	 //Range:</b> Angular: (-PI,PI]<br>
	 //Range:</b> Positional: (-inf,inf)<br>
	 //Default:</b> 0
	float targetValue;

	inline NxSpringDescription()
	{
		spring = 0;
		damper = 0;
		targetValue = 0;
	}

};

class NxGraphics_Export NxJointRevoluteDesc : public NxJointBaseDesc
{
public :
	NxJointRevoluteDesc() : NxJointBaseDesc( NxJointRevolute ) { 
		SetToDefault(true);
	}

	inline void SetToDefault(bool fromCtor)
	{
		NxJointBaseDesc::SetToDefault();
		projectionDistance = 1.0f;
		projectionAngle = 0.0872f;	//about 5 degrees in radians.

		if (!fromCtor)
		{
			//limit.SetToDefault();
			//motor.SetToDefault();
			//spring.SetToDefault();
		}

		flags = 0;
		projectionMode = NxJointProjectionDescNone;
	}


	NxJointPairLimitDesc limit;
	NxMotorDescription motor;
	NxSpringDescription spring; // Optional spring.

	 
	//brief The distance beyond which the joint is projected.
	
	//projectionMode is NX_JPM_POINT_MINDIST, the joint gets artificially projected together when it drifts more than this distance. Sometimes it is not possible to project (for example when the joints form a cycle)
	//Should be nonnegative. However, it may be a bad idea to always project to a very small or zero distance because the solver *needs* some error in order to produce correct motion.

	//<b>Range:</b> [0,inf)<br>
	//<b>Default:</b> 1.0

	//@see projectionMode projectionAngle
    
	float projectionDistance;	

	 
	//brief The angle beyond which the joint is projected.
	
	//This similar to #projectionDistance, except this is an angle (in radians) to which angular drift is projected.

	//<b>Unit:</b> Radians
	//<b>Range:</b> (0.2,PI)<br>
	//<b>Default:</b> 0.0872 (about 5 degrees in radians)

	//@see projectionDistance projectionMode
	float projectionAngle;

	 
	//brief This is a combination of the bits defined by ::NxRevoluteJointFlag.

	//<b>Default:</b> 0

	//@see NxRevoluteJointFlag
	 
	unsigned int flags;

	//use this to enable joint projection
	//Default:</b> NX_JPM_NONE
	//NxJointProjectionDesc projectionDistance projectionAngle NxRevoluteJoint.setProjectionMode()
 
	NxJointProjectionDesc projectionMode;
};


class NxGraphics_Export NxPhysicsD6Joint
{
public :
	//! NxPhysicsD6Joint Class Constructor.
	NxPhysicsD6Joint( const NxJointD6Desc & JointDesc );
	//! NxPhysicsD6Joint Class Destructor.
	~NxPhysicsD6Joint();
private :
	NxD6Joint * mJoint;
};

class NxGraphics_Export NxPhysicsRevoluteJoint
{
public :
	//! NxPhysicsD6Joint Class Constructor.
	NxPhysicsRevoluteJoint( const NxJointRevoluteDesc & JointDesc );
	//! NxPhysicsD6Joint Class Destructor.
	~NxPhysicsRevoluteJoint();
private :
	NxRevoluteJoint * mJoint;
};

enum NxForceTypes
{
	NxForce,                //!< parameter has unit of mass * distance/ time^2, i.e. a force
	NxImpulse,              //!< parameter has unit of mass * distance /time
	NxVelocityChange,		//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
	NxSmoothImpulse,        //!< same as NX_IMPULSE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
	NxSmoothVelocityChange,	//!< same as NX_VELOCITY_CHANGE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
	NxAcceleration			//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
};

 

class NxGraphics_Export NxPhysicsActor : public CountedObj<NxPhysicsActor>
{
public :
	//! NxPhysicsActor Class Constructor.
	NxPhysicsActor( NxNode * Node, const std::string & Name, const PhysicGroup Property );
	//! NxPhysicsActor Class Destructor.
	~NxPhysicsActor();
	//! Create a box shape.
	NxPhysicsBoxShape * CreateBoxShape( const NxBoxDesc & BoxDesc );
	//! Create a Capsule shape.
	NxPhysicsCapsuleShape * CreateCapsuleShape( const NxCapsuleDesc & CapsuleDesc );
	//! Create a plane shape.
	NxPhysicsPlaneShape * CreatePlaneShape( const NxPlaneDesc & PlaneDesc );
	//! Create a mesh shape.
	NxPhysicsMeshShape * CreateMeshShape( const NxEntityDesc & MeshDesc );
	//! Create Fluids.
	NxPhysicsFluid * CreateFluids( const NxParticleDesc & FluidDesc );
	//! Create force field
	NxPhysicsForceField * CreateForceField();

	//! is Actor Dynamic ?
	bool IsDynamic();
	//! Get Actors Name.
	const std::string & GetName() const;
	//! Set User Data.
	void SetUserData( void * Data );
	//!Get User Data.
	void * GetUserData();
	//! Set Actor Global Position.
	void SetPosition( const Nx::Vector3 & Position );
	//! Get Actors Global Position.
	const Nx::Vector3 & GetPosition() const;
	//! Set Actors Global Orientation.
	void SetOrientation( const Nx::Quaternion & Orientation );
	//! Get Actors Global orientation.
	const Nx::Quaternion & GetOrientation() const;
	//! Add force at position
	void AddForceAtPos( const Nx::Vector3& force, const Nx::Vector3& pos, NxForceTypes mode = NxForce, bool wakeup = true );
	//! Add force at local position
	void AddForceAtLocalPos( const Nx::Vector3& force, const Nx::Vector3& pos, NxForceTypes mode = NxForce, bool wakeup = true);
	//! Add force to the Actor, the Actor must be Dynamic.
	void AddForce( const Nx::Vector3 & Force, NxForceTypes mode = NxForce, bool wakeup = true );
	// add torque.
	void AddTorque( const Nx::Vector3& torque, NxForceTypes mode = NxForce, bool wakeup = true);
	//! raise body flags.
	void RaiseBodyFlag( int flags );
	//! Set Entity Physic Properties.
	void SetBodyProperties( const PhysicGroup Property );
	//! Get Physic Properties.
	const PhysicGroup GetBodyProperty() const;
	//! set actor pair flags;
	void SetActorPairFlags( NxPhysicsActor * actor, int flags );

	//! Get Physx Actor.
	NxActor * GetNxActor();
	//! set mass
	void SetMass( float mass );

private:
	//! Physx Actor.
	NxActor * mActor;
	//! Physx Fluid Vector;
	std::vector< NxPhysicsFluid * > mFluids;
	//! Physx ForceField Vector;
	std::vector< NxPhysicsForceField * > mForceFields;
	//! Actor Physic Property.
	PhysicGroup mPhysicProperty;
	//! name
	std::string mName;
};

class NxPhysicsController 
{
public :
	 NxPhysicsController( NxNode * Node ) ;
	~NxPhysicsController();
	void CreateCharacterController( const Nx::Vector3 & position, float radius, float height);
	void DeleteCharacterController();


	void SetPosition( const Nx::Vector3 & position );
	const Nx::Vector3 & GetPosition() const;
	void SetExtends( const Nx::Vector3 & position );
	void SetCollision( bool Collision );
	void SetHeight( float Height );
	float GetHeight();
	void SetRadius( float Radius );
	void SetStepOffset( float Offset );
	void Move(const Nx::Vector3 & disp, unsigned int activeGroups, float minDist, unsigned int& collisionFlags, float sharpness=1.0f );

	NxPhysicsControllerCallback * mCallback;
private :
	bool ExtendCharacter ;
	NxController* mController;
	NxScene * CurrentNxScene;

};



} // namespace

#endif