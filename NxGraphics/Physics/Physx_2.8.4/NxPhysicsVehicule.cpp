#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>
#include <NxMeshManager.h>
 
#include "../../nxdeps/include/OIS/OIS.h"

namespace Nx {

	enum NxWheelFlags {
        NX_WF_STEERABLE_INPUT           = (1 << 0),
        NX_WF_STEERABLE_AUTO            = (1 << 1),
        NX_WF_AFFECTED_BY_HANDBRAKE     = (1 << 2),
        NX_WF_ACCELERATED                       = (1 << 3),
        
        NX_WF_BUILD_LOWER_HALF          = (1 << 8),
        NX_WF_USE_WHEELSHAPE            = (1 << 9),

        NX_WF_ALL_WHEEL_FLAGS           = NX_WF_STEERABLE_INPUT
                                                                | NX_WF_STEERABLE_AUTO
                                                                | NX_WF_AFFECTED_BY_HANDBRAKE
                                                                | NX_WF_ACCELERATED,

};

class NxWheelDesc {
public:
        NxVec3                                  position;
       // NxVec3                                        wheelAxis;
       // NxVec3                                        downAxis;

        NxReal                                  wheelRadius;
        NxReal                                  wheelWidth;

        NxReal                                  wheelSuspension;

        NxReal                                  springRestitution;
        NxReal                                  springDamping;
        NxReal                                  springBias;

        NxReal                                  maxBrakeForce;
        NxReal                                  frictionToSide;
        NxReal                                  frictionToFront;

        NxU32                                   wheelApproximation;

        NxU32                                   wheelFlags;

        

        void*                                   userData;

        NX_INLINE NxWheelDesc();
        NX_INLINE void setToDefault();
        NX_INLINE bool isValid() const;

};

NX_INLINE NxWheelDesc::NxWheelDesc()    //constructor sets to default
{
        setToDefault();
}

NX_INLINE void NxWheelDesc::setToDefault()
{
        position.set(0,0,0);
        //wheelAxis.set(0,0,1);
        //downAxis.set(0,-1,0);
        userData = NULL;
        wheelFlags = 0;
        wheelRadius = 1;
        wheelWidth = 0.1f;

        springBias = 0;
        springRestitution = 1.f;
        springDamping = 0.f;

        wheelSuspension = 1.f;
        maxBrakeForce = 0.f;
        frictionToSide = 1.0f;
        frictionToFront = 1.0f;
}

NX_INLINE bool NxWheelDesc::isValid() const
{
        //if (NxMath::abs(1-wheelAxis.magnitudeSquared()) > 0.001f)
        //      return false;
        if (wheelApproximation > 0 && wheelApproximation < 4)
		{
                fprintf(stderr, "wheelApproximation must be either 0 or bigger than 3\n");
                return false;
        }
        if ((wheelFlags & NX_WF_STEERABLE_AUTO) && (wheelFlags & NX_WF_STEERABLE_INPUT)) 
		{
                fprintf(stderr, "NX_WF_STEERABLE_AUTO and NX_WF_STEERABLE_INPUT not permitted at the same time\n");
                return false;
        }
        return true;
}

NxWheelShape * wheelLeftFront;  
NxWheelShape * wheelRightFront;
NxWheelShape * wheelLeftRear;
NxWheelShape * wheelRightRear;

NxMaterial * mMat;

NxReal mSteerAngle;

std::vector< Ogre::SceneNode * > mWheels;

NxActor * CarBody;

 

NxWheelShape * CreateWheel( NxActor * actor, NxWheelDesc* wheelDesc ) 
{
	NxScene * scene = &actor->getScene();
	//create a shared car wheel material to be used by all wheels
	static NxMaterial * wsm = 0;
	if(!wsm)
	{
		NxMaterialDesc m;
		m.flags |= NX_MF_DISABLE_FRICTION;
		wsm = scene->createMaterial(m);
	}

	NxWheelShapeDesc wheelShapeDesc;

	wheelShapeDesc.localPose.t = wheelDesc->position;
	NxQuat q;
	q.fromAngleAxis(90.0f, NxVec3(0,1,0));
	//wheelShapeDesc.localPose.M.fromQuat(q);
	wheelShapeDesc.materialIndex = wsm->getMaterialIndex();
	wheelShapeDesc.wheelFlags = wheelDesc->wheelFlags;

	NxReal heightModifier = (wheelDesc->wheelSuspension + wheelDesc->wheelRadius) / wheelDesc->wheelSuspension;

	wheelShapeDesc.suspension.spring = wheelDesc->springRestitution*heightModifier;
	wheelShapeDesc.suspension.damper = 0;//wheelDesc->springDamping*heightModifier;
	wheelShapeDesc.suspension.targetValue = wheelDesc->springBias*heightModifier;

	wheelShapeDesc.radius = wheelDesc->wheelRadius;
	wheelShapeDesc.suspensionTravel = wheelDesc->wheelSuspension; 
	wheelShapeDesc.inverseWheelMass = 0.1f;	//not given!? TODO

	wheelShapeDesc.lateralTireForceFunction.stiffnessFactor *= wheelDesc->frictionToSide;	
	wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor *= wheelDesc->frictionToFront;	

	NxWheelShape * wheelShape = static_cast<NxWheelShape *>(actor->createShape(wheelShapeDesc));
	return wheelShape;
}



NxWheelShape * attachCarWheel( NxActor * Carframe , NxWheelDesc & wheelDesc, bool left  )
{
	LogMsg("attachCarWheel ... ");
	NxWheelShapeDesc wheelShapeDesc;
    if (!mMat)
    {
        NxMaterialDesc m;
        m.flags |= NX_MF_DISABLE_FRICTION;
		mMat = Carframe->getScene().createMaterial(m);
    }

	//wheelShapeDesc.shapeFlags

    wheelShapeDesc.materialIndex = mMat->getMaterialIndex();

    wheelShapeDesc.localPose.t = wheelDesc.position;
    NxQuat q;
    q.fromAngleAxis(90,NxVec3(0,1,0));
    //wheelShapeDesc.localPose.M.fromQuat(q);
    NxReal heightModifier = (wheelDesc.wheelSuspension + wheelDesc.wheelRadius) / wheelDesc.wheelSuspension;
    wheelShapeDesc.suspension.spring = wheelDesc.springRestitution * heightModifier;
    wheelShapeDesc.suspension.damper = 0;
    wheelShapeDesc.suspension.targetValue = wheelDesc.springBias * heightModifier;
    wheelShapeDesc.radius = wheelDesc.wheelRadius;
    wheelShapeDesc.suspensionTravel = wheelDesc.wheelSuspension;
    wheelShapeDesc.inverseWheelMass = 0.1f;
    wheelShapeDesc.lateralTireForceFunction.stiffnessFactor *= wheelDesc.frictionToSide;
    wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor *= wheelDesc.frictionToFront;

    NxWheelShape * wheelShape = NULL;
    Ogre::SceneNode * node;

	Nx3D_Scene * CurrentScene = NxScene3DPhysicsManager::getSingleton().GetNx3DSceneFromPhysxScene( &Carframe->getScene() );

	float Height = 0.6;
	float Radius = 0.45;
	
    if (left)
    {


			std::string Name = "leftweel"+ Ogre::StringConverter::toString(rand()%10000);
			Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateCylinder( Name, Radius, Height );
			Entity * TmpEntity = CurrentScene->GetNxSceneManager()->createEntity( Name + "Entity", Name);
			Ogre::SceneNode * TmpSceneNode = CurrentScene->GetNxSceneManager()->getRootSceneNode()->createChildSceneNode( Name+"SceneNode"+ Ogre::StringConverter::toString(rand()%10000) );  
			TmpSceneNode->translate( 0, 0 , 0, Ogre::Node::TS_WORLD );
			TmpSceneNode->attachObject( TmpEntity );
			TmpSceneNode->setPosition( Ogre::Vector3( 0,0,0 ) );
			TmpEntity->setMaterialName("Examples/OgreLogo");



            //wheelShapeDesc.name = "left";
            //node = mRenderSystem->createSceneNodeEntityPair(Ogre::String("wheelLeft.mesh"),NxOgre::Vec3(0,0,0),NxOgre::Quat());
			//Ogre::SceneNode * WheelNode = CurrentScene->GetNxSceneManager()->createSceneNode( "leftweel" + Ogre::StringConverter::toString(rand()%10000) );


			//Wheel = NxScene3DManager::getSingleton().CreateCylinder( CurrentScene, "leftweel" + Ogre::StringConverter::toString(rand()%10000), 1.5 ,0.3);
		 
			mWheels.push_back( TmpSceneNode );

		//	node = Wheel->GetNxSceneNode();
            //node->setOrientation(Ogre::Quaternion(Ogre::Radian(55),Ogre::Vector3::UNIT_Y));
            //node->yaw(Ogre::Radian(55));
	}
    else
    {


			std::string Name = "rightweel"+ Ogre::StringConverter::toString(rand()%10000);
			Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateCylinder( Name, Radius, Height );
			Entity * TmpEntity = CurrentScene->GetNxSceneManager()->createEntity( Name + "Entity", Name);
			Ogre::SceneNode * TmpSceneNode = CurrentScene->GetNxSceneManager()->getRootSceneNode()->createChildSceneNode( Name+"SceneNode"+ Ogre::StringConverter::toString(rand()%10000) );  
			TmpSceneNode->translate( 0, 0 , 0, Ogre::Node::TS_WORLD );
			TmpSceneNode->attachObject( TmpEntity );
			TmpSceneNode->setPosition( Ogre::Vector3( 0,0,0 ) );
			TmpEntity->setMaterialName("Examples/OgreLogo");

			mWheels.push_back( TmpSceneNode );

            //wheelShapeDesc.name = "right";       
			//Wheel = NxScene3DManager::getSingleton().CreateCylinder( CurrentScene, "righttweel"+ Ogre::StringConverter::toString(rand()%10000), 1.5, 0.3);
		 
		//	mWheels.push_back( Wheel );
        //    node = Wheel->GetNxSceneNode();//mRenderSystem->createSceneNodeEntityPair(Ogre::String("wheelRight.mesh"),NxOgre::Vec3(0,0,0),NxOgre::Quat());
            //node->setOrientation(Ogre::Quaternion(Ogre::Radian(-55),Ogre::Vector3::UNIT_Y));
            //node->yaw(Ogre::Radian(-55));
    }

    //node->setScale(0.55,0.55,0.55);

	wheelShape = CreateWheel( Carframe, &wheelDesc ) ;
	return wheelShape;

}

NxScene3DObjectVehicule::NxScene3DObjectVehicule( NxNode * VehiculeSceneNode, const std::string & ChassiName ) : NxEntity()
{

	mType = NxTypeVehicule;
	Nx3D_Scene * CurrentScene = NxScene3DManager::getSingleton().GetNx3DSceneFromOgreSceneManager( VehiculeSceneNode->GetNxSceneNode()->getCreator() );
	NxScene *  mScene = &CurrentScene->GetPhysicScene()->GetPhysicsScene3D() ;
	 
	mEntity = VehiculeSceneNode->GetNxScene().GetNxSceneManager()->createEntity( ChassiName );
	mNode = VehiculeSceneNode->GetNxScene().GetNxSceneManager()->createSceneNode( ChassiName + Ogre::StringConverter::toString( instance_count() ) );
	mNode->attachObject( mEntity );

	mNxNodeParent = VehiculeSceneNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );
 
 
	
 
	Ogre::Vector3 ChassisSize( 2.5,1,5 );
	NxBodyDesc desc;
	desc.mass = 1200; // 1 tonne
	desc.flags |= NX_BF_FROZEN_ROT_Z | NX_BF_FROZEN_ROT_X;
	NxBoxShapeDesc bd;

	NxVec3 size( ChassisSize.x, ChassisSize.y, ChassisSize.z );
	bd.dimensions = size / 2 ;
	bd.localPose.t = NxVec3(0, size.y/2.0f ,0) ; // physx pivot point
	NxActorDesc nxBoxActorDesc;
	nxBoxActorDesc.setToDefault();
	nxBoxActorDesc.shapes.pushBack(&bd);
	nxBoxActorDesc.body =  &desc;

	/*NxCompartmentDesc cd;
    cd.type = NX_SCT_RIGIDBODY;
    cd.deviceCode = NX_DC_PPU_0;
    NxCompartment * compartment = mScene->createCompartment(cd);
	nxBoxActorDesc.compartment = compartment;
	*/
	CarBody = mScene->createActor(nxBoxActorDesc);
	//AddPhysxActor(CarBody);
	//SetPhysicProperties( PhysicGroup::GROUP_KINEMATIC );
	
	CarBody->setGlobalPosition( NxVec3( VehiculeSceneNode->GetNxSceneNode()->_getDerivedPosition().x,VehiculeSceneNode->GetNxSceneNode()->_getDerivedPosition().y, VehiculeSceneNode->GetNxSceneNode()->_getDerivedPosition().z ));

 
    NxTireFunctionDesc lngTFD;
    lngTFD.extremumSlip = 1.0f;
    lngTFD.extremumValue = 0.02f;
    lngTFD.asymptoteSlip = 2.0f;
    lngTFD.asymptoteValue = 0.01f;
    lngTFD.stiffnessFactor = 1000000.0f;

    NxTireFunctionDesc latTFD;
    latTFD.extremumSlip = 1.0f;
    latTFD.extremumValue = 0.02f;
    latTFD.asymptoteSlip = 2.0f;
    latTFD.asymptoteValue = 0.01f;  
    latTFD.stiffnessFactor = 1000000.0f;

    NxTireFunctionDesc slipTFD;
    slipTFD.extremumSlip = 1.0f;
    slipTFD.extremumValue = 0.02f;
    slipTFD.asymptoteSlip = 2.0f;
    slipTFD.asymptoteValue = 0.01f; 
    slipTFD.stiffnessFactor = 100.0f;  

    NxTireFunctionDesc medTFD;
    medTFD.extremumSlip = 1.0f;
    medTFD.extremumValue = 0.02f;
    medTFD.asymptoteSlip = 2.0f;
    medTFD.asymptoteValue = 0.01f;  
    medTFD.stiffnessFactor = 10000.0f;  


	float HeightRelativePos = 0;//-1.2;
	float WidthRelativePos = 1;
	float DepthRelativePos =  3;

	float WheelRadius = 0.45;
	float WheelWidth = 0.6;
	float WheelSuspension = 1.0f;
	float WheelMaxBreakForce = 1.0f;
	float WheelSpringRestitution = 7000;//100.0f;
	float WheelSpringDamping = 800;//0.5f;
	

    NxWheelDesc wheelDesc;
    wheelDesc.wheelApproximation = 10;
    wheelDesc.wheelRadius = WheelRadius;
    wheelDesc.wheelWidth = WheelWidth;
    wheelDesc.wheelSuspension = WheelSuspension;
    wheelDesc.springRestitution = WheelSpringRestitution;
    wheelDesc.springDamping = WheelSpringDamping;
    wheelDesc.springBias = 0;
    wheelDesc.maxBrakeForce = WheelMaxBreakForce;
    wheelDesc.position = NxVec3(WidthRelativePos,HeightRelativePos,DepthRelativePos);
    wheelDesc.wheelFlags = NX_WF_USE_WHEELSHAPE | NX_WF_BUILD_LOWER_HALF | NX_WF_ACCELERATED | NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_STEERABLE_INPUT;
    wheelLeftFront = attachCarWheel(CarBody,wheelDesc,true);

	

    // Front right wheel
    NxWheelDesc wheelDesc2;
    wheelDesc2.wheelApproximation = 10;
    wheelDesc2.wheelRadius = WheelRadius;
    wheelDesc2.wheelWidth = WheelWidth;  // 0.1
    wheelDesc2.wheelSuspension = WheelSuspension;  
    wheelDesc2.springRestitution = WheelSpringRestitution;
    wheelDesc2.springDamping = WheelSpringDamping;
    wheelDesc2.springBias = 0;  
    wheelDesc2.maxBrakeForce = WheelMaxBreakForce;
    wheelDesc2.position = NxVec3(-WidthRelativePos,HeightRelativePos,DepthRelativePos);
    wheelDesc2.wheelFlags = NX_WF_USE_WHEELSHAPE | NX_WF_BUILD_LOWER_HALF | NX_WF_ACCELERATED |  NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_STEERABLE_INPUT; 
    wheelRightFront = attachCarWheel(CarBody,wheelDesc2,false);

    // Rear left wheel
    NxWheelDesc wheelDesc3;
    wheelDesc3.wheelApproximation = 10;
    wheelDesc3.wheelRadius = WheelRadius;
    wheelDesc3.wheelWidth = WheelWidth;  // 0.1
    wheelDesc3.wheelSuspension = WheelSuspension;  
    wheelDesc3.springRestitution = WheelSpringRestitution;
    wheelDesc3.springDamping = WheelSpringDamping;
    wheelDesc3.springBias = 0;  
    wheelDesc3.maxBrakeForce = WheelMaxBreakForce;
    wheelDesc3.position = NxVec3(WidthRelativePos,HeightRelativePos,-DepthRelativePos);
    wheelDesc3.wheelFlags = NX_WF_USE_WHEELSHAPE | NX_WF_BUILD_LOWER_HALF | NX_WF_ACCELERATED | NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_STEERABLE_INPUT; 
    wheelLeftRear = attachCarWheel(CarBody,wheelDesc3,true);


    // Rear right wheel
    NxWheelDesc wheelDesc4;
    wheelDesc4.wheelApproximation = 10;
    wheelDesc4.wheelRadius = WheelRadius;
    wheelDesc4.wheelWidth = WheelWidth;  // 0.1
    wheelDesc4.wheelSuspension = WheelSuspension;  
    wheelDesc4.springRestitution = WheelSpringRestitution;
    wheelDesc4.springDamping = WheelSpringDamping;
    wheelDesc4.springBias = 0;  
    wheelDesc4.maxBrakeForce = WheelMaxBreakForce;
    wheelDesc4.position = NxVec3(-WidthRelativePos,HeightRelativePos,-DepthRelativePos);
    wheelDesc4.wheelFlags = NX_WF_USE_WHEELSHAPE | NX_WF_BUILD_LOWER_HALF | NX_WF_ACCELERATED | NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_STEERABLE_INPUT;
    wheelRightRear = attachCarWheel(CarBody,wheelDesc4,false);

	NxVec3 massPos = CarBody->getCMassLocalPosition();
    massPos.y -= 0.5; // 1
	//set the pose of the center of mass relative to the actor.
	CarBody->setCMassOffsetLocalPosition( NxVec3( massPos.x,massPos.y,massPos.z ));
   

    wheelLeftRear->setBrakeTorque(20);
    wheelRightRear->setBrakeTorque(20);

    wheelLeftFront->setBrakeTorque(20);
    wheelRightFront->setBrakeTorque(20);

    wheelLeftRear->setLateralTireForceFunction(latTFD);
    wheelLeftRear->setLongitudalTireForceFunction(lngTFD);

    wheelRightRear->setLateralTireForceFunction(latTFD);
    wheelLeftRear->setLongitudalTireForceFunction(lngTFD);
 
}

NxScene3DObjectVehicule::~NxScene3DObjectVehicule()
{

}


static float gPressTime = 0;


bool TurnRight = false;
bool TurnLeft = false;

bool Break = false;
bool PushFront = false;
bool PushBack = false;

void NxScene3DObjectVehicule::Update( float time )
{

	//Update Chassis
	NxVec3 chassisPosition = CarBody->getGlobalPosition();
	NxQuat chassisOrient = CarBody->getGlobalOrientationQuat();

 
	mNxNodeParent->GetNxSceneNode()->setPosition( VecToOgre( chassisPosition ));
	mNxNodeParent->GetNxSceneNode()->setOrientation( ogrequat( chassisOrient ));

	if (wheelLeftFront->getAxleSpeed() && (mSteerAngle > 0 || mSteerAngle < 0))
	{
		if (mSteerAngle > 0)
		{
			mSteerAngle -= NxPi * 0.006;
			if (mSteerAngle < NxPi * 0.006)
			{
				mSteerAngle = 0;
			}				
		}
		else if(mSteerAngle < 0)
		{
			mSteerAngle += NxPi * 0.006;
			if (mSteerAngle > -NxPi * 0.006)
			{
				mSteerAngle = 0;
			}				
		}
		wheelLeftFront->setSteerAngle(mSteerAngle);
		wheelRightFront->setSteerAngle(mSteerAngle);
	}

	if( TurnRight )
	{
		if (mSteerAngle <= 0.75f)
		{
			mSteerAngle += NxPi * 0.015;
			wheelLeftFront->setSteerAngle(mSteerAngle);
			wheelRightFront->setSteerAngle(mSteerAngle);
		}
	}

	if( TurnLeft )
	{
		if (mSteerAngle >= -0.75f)
		{
			mSteerAngle -= NxPi * 0.015;
			wheelLeftFront->setSteerAngle(mSteerAngle);
			wheelRightFront->setSteerAngle(mSteerAngle);
		}		
	}


	//if(Break)
	//{
	//	wheelLeftRear->setBrakeTorque(5000);
	//	wheelRightRear->setBrakeTorque(5000);
	//}
	//if(PushFront)
	//{
	//	wheelLeftRear->setMotorTorque(2000 ) ;
	//	wheelRightRear->setMotorTorque(2000   );
	//}

	//if(PushBack)
	//{
	//	wheelLeftRear->setMotorTorque(-2000 );
	//	wheelRightRear->setMotorTorque(-2000  );
	//}

	mWheels[0]->setPosition( VecToOgre( wheelLeftFront->getGlobalPosition() )) ;
	mWheels[1]->setPosition( VecToOgre( wheelRightFront->getGlobalPosition() )) ;
	mWheels[2]->setPosition( VecToOgre( wheelLeftRear->getGlobalPosition() )) ;
	mWheels[3]->setPosition( VecToOgre( wheelRightRear->getGlobalPosition() )) ;


	float rotDist = wheelRightRear->getAxleSpeed() * time / NxTwoPi;
	static float rotAngleRAD = 0;
	rotAngleRAD += rotDist;
	if ( rotAngleRAD > Ogre::Math::PI * 2 )
	rotAngleRAD -= Ogre::Math::PI * 2;
	else if ( rotAngleRAD < -Ogre::Math::PI * 2 )
	rotAngleRAD += Ogre::Math::PI * 2;
	NxReal steerAngle = wheelRightRear->getSteerAngle();
	Ogre::Matrix3 x,z;
	x.FromEulerAnglesXYZ( Ogre::Radian( rotAngleRAD ), Ogre::Radian(),Ogre::Radian() );
	z.FromEulerAnglesXYZ( Ogre::Radian(),Ogre::Radian( steerAngle ), Ogre::Radian() );
	x = z * x;
	Ogre::Quaternion y;
	y.FromRotationMatrix( x );

	Ogre::Quaternion Offset;
	Offset.FromAngleAxis( (Ogre::Radian) Ogre::Math::DegreesToRadians( 90 ), Ogre::Vector3(0,0,1));
	mWheels[3]->setOrientation( y * Offset );

 

//ici
//http://89.151.96.106/addonforums/viewtopic.php?t=3918&view=previous
 
 

	/*
	NxQuat Ori;
	wheelLeftFront->getLocalOrientation().toQuat(Ori);
	mWheels[0]->setOrientation( ogrequat( Ori ));

	wheelRightFront->getLocalOrientation().toQuat(Ori);
	mWheels[1]->setOrientation( ogrequat( Ori  ));

	wheelLeftRear->getLocalOrientation().toQuat(Ori);
	mWheels[2]->setOrientation( ogrequat( Ori  ));

	wheelRightRear->getLocalOrientation().toQuat(Ori);
	mWheels[3]->setOrientation( ogrequat( Ori  ));
	*/
 

}

void NxScene3DObjectVehicule::OnKeyPressed( const OIS::KeyEvent &e )
{
	if( OIS::KeyCode::KC_P == e.key )
	{
		TurnRight=true;
	}
	else if( OIS::KeyCode::KC_O == e.key )
	{
		TurnLeft=true;
	}
	else if( OIS::KeyCode::KC_I == e.key )
	{
		PushFront=true;
		 wheelLeftRear->setMotorTorque(500 ) ;
		 wheelRightRear->setMotorTorque(500   );
	}
	else if( OIS::KeyCode::KC_U == e.key )
	{
		PushBack=true;
	}
	else if( OIS::KeyCode::KC_Y == e.key )
	{
		Break=true;
		wheelLeftRear->setBrakeTorque(2000);
		wheelRightRear->setBrakeTorque(2000);
			 wheelLeftRear->setMotorTorque(0 ) ;
	 wheelRightRear->setMotorTorque(0   );
	}

}
void NxScene3DObjectVehicule::OnKeyReleased( const OIS::KeyEvent &e )
{
	if( OIS::KeyCode::KC_P == e.key )
	{
		TurnRight=false;
	}
	else if( OIS::KeyCode::KC_O == e.key )
	{
		TurnLeft=false;
	}
	else if( OIS::KeyCode::KC_I == e.key )
	{
		PushFront=false;
		//wheelLeftRear->setMotorTorque(0);
		//wheelRightRear->setMotorTorque(0);
	}
	else if( OIS::KeyCode::KC_U == e.key )
	{
		PushBack=false;
		//wheelLeftRear->setMotorTorque(0);
		//wheelRightRear->setMotorTorque(0);
	}
	else if( OIS::KeyCode::KC_Y == e.key )
	{
		Break=false;
		wheelLeftRear->setBrakeTorque(0);
		wheelRightRear->setBrakeTorque(0);
	}
}
void NxScene3DObjectVehicule::OnMouseMoved( const OIS::MouseEvent &e )
{

}
void NxScene3DObjectVehicule::OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{

}
void NxScene3DObjectVehicule::OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{

}




 

}

