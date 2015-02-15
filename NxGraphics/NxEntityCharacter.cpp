#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "../nxmocap/NxMocap_Headers.h"

namespace Nx {

NxScene3DObjectCharacter::NxScene3DObjectCharacter( NxNode * node ) : IsDead( false )
{
	mType = NxTypeCharacter;
	mCharacterScene = NxScene3DManager::getSingleton().GetNx3DSceneFromOgreSceneManager( node->GetNxSceneNode()->getCreator() );

	mCharacterCamera = new NxCharacterController( node, "CHANGENAME" );
	mCharacterCamera->CreateCharacterController( 0.5, 1.85f );

	mNode = node->GetNxScene().GetNxSceneManager()->createSceneNode( "Character" + Ogre::StringConverter::toString( instance_count() ) );
	mNxNodeParent = node;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );
}

NxScene3DObjectCharacter::~NxScene3DObjectCharacter()
{

}

NxCharacterController * NxScene3DObjectCharacter::GetNxCamera()
{
	return mCharacterCamera;
}

void NxScene3DObjectCharacter::SetNavigationMode( NavigationMode Mode )
{
	mCharacterCamera->SetNavigationMode( Mode );
}

void NxScene3DObjectCharacter::SetCharacterSpeed( float Speed )
{
	mCharacterCamera->SetNxCameraSpeed( Speed );
}

void NxScene3DObjectCharacter::OnMouseMoved( const OIS::MouseEvent &e )
{
	mCharacterCamera->OnMouseMoved( e );
}

void NxScene3DObjectCharacter::OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	mCharacterCamera->OnMousePressed( e, id );
}

void NxScene3DObjectCharacter::OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	mCharacterCamera->OnMouseReleased( e, id );
}

void NxScene3DObjectCharacter::OnKeyPressed( const OIS::KeyEvent &e )
{
	mCharacterCamera->OnKeyPressed( e );
}

void NxScene3DObjectCharacter::OnKeyReleased( const OIS::KeyEvent &e )
{
	mCharacterCamera->OnKeyReleased( e );
}

void NxScene3DObjectCharacter::AddProp( NxEntity * prop, std::string BoneName, Ogre::Vector3 Offset )
{
	//Locker->lock();
	/*

	for( BindingBoneActors::iterator iter = BonesActorsMap.begin(); iter != BonesActorsMap.end(); iter++)
	{
		Ogre::Bone * CurrentBone = iter->first ;
		NxScene3DObjectEntity  * CurrentEntity = iter->second ;

		//CurrentEntity->GetEntity()->attachObjectToBone()
		///tp = objectEntity->attachObjectToBone( ParamName, BillboardPic );

		if( CurrentBone->getName().compare( BoneName ) == 0  )
		{
			if( prop->GetNxType() == NxTypeCloth )
			{
				NxCloth * clother = (( Nx3DObjectPhysicCloth *  )prop)->GetNxCloth();
				NxActor *  Actor = CurrentEntity->GetPhysxActor();
				clother->freeVertex( 0 ) ;
				NxVec3 Rotation( Offset.z ,Offset.x ,Offset.y );// doesnt work correctly
				clother->attachVertexToShape( 0 , *Actor->getShapes(), Rotation ,0); 
				NxPropsCollection.insert(  make_pair( CurrentBone, prop ));  
			}
			else if( prop->GetNxType() == NxTypeFluid )
			{
				//NxFluid * Fluider = (( Nx3DObjectPhysicFluid *  )prop)->GetNxFluid();
				//NxFluidEmitter * NxEmit = (( Nx3DObjectPhysicFluid *  )prop)->GetNxEmitter();
				NxActor *  Actor = CurrentEntity->GetPhysxActor();
				NxPropsCollection.insert(  make_pair( CurrentBone, prop ));  
			}
			else if( prop->GetNxType() == NxTypeMagnet )
			{
				NxPropsCollection.insert(  make_pair( CurrentBone, prop ));
			}
			else
			{
				NxPropsCollection.insert(  make_pair( CurrentBone, prop ));
			}
		}
	}

	//Locker->unlock();
	*/
}

Ogre::Skeleton * NxScene3DObjectCharacter::GetSkeleton()
{
	if( mCharacterEntity->hasSkeleton() )
	{
		return mCharacterEntity->getSkeleton();
	}
	LogMsg("NxScene3DObjectCharacter::GetSkeleton() : No Skeleton to return");
	return 0 ;
}

Ogre::Entity * NxScene3DObjectCharacter::GetEntity()
{
	return mCharacterEntity;
}

void NxScene3DObjectCharacter::InitializeBoneState( const std::string& Name, const Nx::Radian& Angle, const Nx::Vector3 Axis )
{
    Nx::Quaternion q;
    q.FromAngleAxis( Angle, Axis );     
    InitializeBoneState( Name, q );
}

void NxScene3DObjectCharacter::InitializeBoneState( const std::string& Name, const Nx::Degree& Yaw, const Nx::Degree& Pitch, const Nx::Degree& Roll )
{
	if( mCharacterSkeleton->hasBone( Name ) )
	{
        Ogre::Bone* bone = mCharacterSkeleton->getBone(Name);
        bone->setManuallyControlled(true);
        bone->setInheritOrientation(false);
        bone->resetOrientation();
        bone->yaw( NxRadiantoOgre( Yaw ));
        bone->pitch( NxRadiantoOgre( Pitch ));
        bone->roll( NxRadiantoOgre( Roll ));
        //Matrix3 mat = bone->getLocalAxes();
        bone->setInitialState();
	}
	else
	{
		LogMsg("NxScene3DObjectCharacter::InitializeBoneState : didnt find bone name");
	}
}

void NxScene3DObjectCharacter::InitializeBoneState( const std::string& Name, const Nx::Quaternion& q )
{
	if( mCharacterSkeleton->hasBone(Name))
	{
		Ogre::Bone * bone = mCharacterSkeleton->getBone(Name);
        bone->setManuallyControlled(true);
		bone->setInheritOrientation(false);
        bone->resetOrientation();
        bone->setOrientation( NxQuatToOgre( q ) );
        bone->setInitialState();	
	}
	else
	{
		LogMsg("NxScene3DObjectCharacter::InitializeBoneState : didnt find bone name");
	}

}

void NxScene3DObjectCharacter::TransformBone( const std::string& BoneName, Nx::Matrix3 RotInfo, bool flip )
{

	Ogre::Bone* bone = mCharacterSkeleton->getBone( BoneName );
	Ogre::Quaternion qI = bone->getInitialOrientation();
	Ogre::Quaternion newQ = Ogre::Quaternion::IDENTITY;

	// Get the openNI bone info
	//xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap();            
	// XnSkeletonJointOrientation jointOri;
	// pUserSkel.GetSkeletonJointOrientation(m_candidateID, skelJoint, jointOri);

	// static float deg = 0;
	// if(jointOri.fConfidence > 0 )
	//{
	// XnVector3D col1 = xnCreatePoint3D(jointOri.orientation.elements[0], jointOri.orientation.elements[3], jointOri.orientation.elements[6]);
	//  XnVector3D col2 = xnCreatePoint3D(jointOri.orientation.elements[1], jointOri.orientation.elements[4], jointOri.orientation.elements[7]);
	// XnVector3D col3 = xnCreatePoint3D(jointOri.orientation.elements[2], jointOri.orientation.elements[5], jointOri.orientation.elements[8]);

	// Ogre::Matrix3 matOri(jointOri.orientation.elements[0],-jointOri.orientation.elements[1],jointOri.orientation.elements[2],
	//         -jointOri.orientation.elements[3],jointOri.orientation.elements[4],-jointOri.orientation.elements[5],
	//         jointOri.orientation.elements[6],-jointOri.orientation.elements[7],jointOri.orientation.elements[8]);
	//

			


	// Quaternion q;

	newQ.FromRotationMatrix( NxMat3toOgre( RotInfo ));

	bone->resetOrientation(); //in order for the conversion from world to local to work.
	newQ = bone->convertWorldToLocalOrientation(newQ);

	bone->setOrientation(newQ*qI);                  
	// } 
}

// see https://bitbucket.org/MindCalamity/ogrephysx/src/066807cc828e97803d4020a74202b021c49b3bab/source/OgrePhysXRagdoll.cpp?at=default

void NxScene3DObjectCharacter::CreateCharacter( const std::string & CharacterMesh )
{
	Ogre::Vector3 Scale =  Ogre::Vector3(1,1,1);  
	Ogre::Vector3 Pos = Ogre::Vector3(0,0,0);
	static int IdName = 0;

	/*
	if( mSceneNode ) //Delete old Character if Any.
	{
		Scale = mSceneNode->getScale();
		Pos = mSceneNode->getPosition();
		mocapdef.clear();
		BoneFinal.clear();

		if(  mSceneNode->numAttachedObjects() > 0 )
		{
			mSceneNode->detachObject( mCharacterEntity );
			mCharacterScene->GetNxSceneManager()->destroyEntity( mCharacterEntity->getName()); 
		}

		if ( mCharacterScene->GetNxSceneManager()->hasSceneNode( mSceneNode->getName() ) ) 
		mCharacterScene->GetNxSceneManager()->destroySceneNode( mSceneNode->getName() );
		//Ogre::MeshManager::getSingleton().unload( Character ); unload the mesh
	}
	*/

	//Load the characters mesh
	Ogre::MeshPtr Character = Ogre::MeshManager::getSingleton().load( CharacterMesh, "NxCharacters" );

	LogMsg("Creating Entity");
	//Create Entity
	mCharacterEntity = mCharacterScene->GetNxSceneManager()->createEntity( Character->getName() +  "_Entity_" + Ogre::StringConverter::toString( IdName ), Character->getName() );
	mCharacterEntity->setCastShadows( true );
	mNode->attachObject( mCharacterEntity );

	//mCharacterCamera->SetCharacterControllerHeight( Character->getBounds().getMaximum().y );

	// Set Character viewable even if scenenode if out of camera.
	AxisAlignedBox aabBounds;
	aabBounds.setInfinite();	
	Character->_setBounds( aabBounds  );

 

	

	// Get Characters Skeleton
	mCharacterSkeleton = mCharacterEntity->getSkeleton();
	mCharacterSkeleton->setBlendMode( ANIMBLEND_CUMULATIVE );

 



	//Ogre::AnimationState * sts = mCharacterEntity->getAllAnimationStates()->getAnimationState("Run");
	//sts->setEnabled(true);
	//sts->setLoop(true); 

	//mNode->setDebugDisplayEnabled(true);
	 

	 
	 /*
	//Set Global POse Kinect
	Ogre::Quaternion q = Ogre::Quaternion::IDENTITY;
  
	Quaternion OffsetX;
	Quaternion OffsetY;
	Quaternion OffsetZ;
	Quaternion Final;

	InitializeBoneState("Bip01 L UpperArm",q);//haut bras
	Final.FromAngleAxis( Ogre::Degree(180),Vector3(0,0,1));
	InitializeBoneState("Bip01 R UpperArm",Final);

	InitializeBoneState("Bip01 L Forearm",q);//bas bras 
	Final.FromAngleAxis( Ogre::Degree(180),Vector3(0,0,1));
	InitializeBoneState("Bip01 R Forearm",Final);

	OffsetY.FromAngleAxis( Ogre::Degree(180),Vector3(0,1,0));
	OffsetZ.FromAngleAxis( Ogre::Degree(-90),Vector3(0,0,1));
	InitializeBoneState("Bip01 L Thigh",OffsetY * OffsetZ);
	InitializeBoneState("Bip01 R Thigh",OffsetY * OffsetZ);//haut leg

    InitializeBoneState("Bip01 L Calf",OffsetY * OffsetZ);//tibia
    InitializeBoneState("Bip01 R Calf",OffsetY * OffsetZ);

	OffsetX.FromAngleAxis( Ogre::Degree(180),Vector3(1,0,0));
	OffsetY.FromAngleAxis( Ogre::Degree(0),Vector3(0,1,0));
	OffsetZ.FromAngleAxis( Ogre::Degree(-90),Vector3(0,0,1));
	Final = OffsetX*OffsetY*OffsetZ;

	InitializeBoneState("Bip01 Spine2",Final);
	InitializeBoneState("Bip01 Spine1",Final);//stomach
	InitializeBoneState("Bip01 Head",Final);//head

	//InitializeBoneState("Bip01 pelvis",Final);//head

	InitializeBoneState("Bip01",Degree(-90),Degree(0),Degree(0));

	 

 
 
	Ogre::Skeleton::BoneIterator bi = mCharacterSkeleton->getBoneIterator();
	while( bi.hasMoreElements() )
	{
		Bone * CurrentBone = bi.getNext();
		CurrentBone->setManuallyControlled(false);
	}
 
*/


 

 /*
	Ogre::Skeleton::BoneIterator bi = mCharacterSkeleton->getBoneIterator();
	while( bi.hasMoreElements() )
	{
		Bone * CurrentBone = bi.getNext();
		Log("Found Character Bone : " + CurrentBone->getName()  );

		CurrentBone->setInitialState();
		CurrentBone->setManuallyControlled(true);

		NxScene3DObjectCharacter::MocapBoneData data ;
		if( CurrentBone->getParent() != NULL )
		{
			memcpy ( &data.QuatParent, &CurrentBone->getParent()->_getDerivedOrientation() , sizeof( Quaternion ) ); 
		}
		if( CurrentBone->getName() == "Bip01" )
		{
			data.IsRoot = true ; 
			data.CurrentPosition = CurrentBone->_getDerivedPosition();
		}
		else
		{
			data.IsRoot = false;
		}
		 
		memcpy ( &data.QuatOffset, &CurrentBone->getOrientation() , sizeof( Quaternion ) ); 
		memcpy( &data.CurrentBone, &CurrentBone , sizeof( CurrentBone ) );
		BoneFinal.insert( make_pair( string( CurrentBone->getName().c_str() ) , data ) ); 
		


		
	}//while bones

 */


	/*
	mocapdef.insert( make_pair(  string( "Bip01" ) , 1 ) );
	mocapdef.insert( make_pair( string(   "Bip01 Head" ) ,  2) );
	mocapdef.insert( make_pair(  string(  "Bip01 Spine1" ) , 12  ) );
	mocapdef.insert( make_pair(  string(  "Bip01 L UpperArm" ) , 6 ) );
	mocapdef.insert( make_pair(  string(  "Bip01 L Forearm" ) , 7) );
	mocapdef.insert( make_pair(  string(  "Bip01 L Thigh" ) , 3 ) );
	mocapdef.insert( make_pair(  string(  "Bip01 L Calf" ) , 11  ) );
	mocapdef.insert( make_pair(  string(  "Bip01 R UpperArm" ) , 4 ) );
	mocapdef.insert( make_pair(  string(  "Bip01 R Forearm" ), 5 ) );
	mocapdef.insert( make_pair(  string(  "Bip01 R Thigh" ) , 8) );
	mocapdef.insert( make_pair(  string(  "Bip01 R Calf" ) , 9) );
	*/
 

	mCharacterPosOffsetX = 0.0f ;
	mCharacterPosOffsetY = 0.0f ;
	mCharacterPosOffsetZ = 0.0f ;
	mCharacterPosOffsetScale = 50.0f; // root node pos factor

	if( BonesActorsMap.size() > 0 ) DeletePhysXRagdoll();

return;	

CreatePhysXRagdoll();

	IdName++;


	CreatePhysxJoints();
	
	//joints here

}

void NxScene3DObjectCharacter::DeletePhysXRagdoll()
{
	//Locker->lock();
	for( BindingBoneActors::iterator iter = BonesActorsMap.begin(); iter != BonesActorsMap.end(); iter++)
	{
		Ogre::Bone * CurrentBone = iter->first ;
		delete iter->second;
	}

	BonesActorsMap.clear();
	BoneListNames.clear();
	//Locker->unlock();
}


void NxScene3DObjectCharacter::CreatePhysXRagdoll()
{
	
	BoneListNames.push_back("Bip01 Head");
	
	BoneListNames.push_back("Bip01 L UpperArm");
	BoneListNames.push_back("Bip01 L Forearm");
	BoneListNames.push_back("Bip01 L Thigh");
	BoneListNames.push_back("Bip01 L Calf");
	BoneListNames.push_back("Bip01 R UpperArm");
	BoneListNames.push_back("Bip01 R Forearm");
	BoneListNames.push_back("Bip01 R Thigh");
	BoneListNames.push_back("Bip01 R Calf");

	BoneListNames.push_back("Bip01 Pelvis");
 
	BoneListNames.push_back("Bip01 Spine");
	BoneListNames.push_back("Bip01 Spine1");
	BoneListNames.push_back("Bip01 Spine2");
	//BoneListNames.push_back("Bip01 Spine3");

	BoneListNames.push_back("Bip01 L Clavicle");
	BoneListNames.push_back("Bip01 R Clavicle");

	BoneListNames.push_back("Bip01 L Hand");
	BoneListNames.push_back("Bip01 R Hand");
	BoneListNames.push_back("Bip01 L Foot");
	BoneListNames.push_back("Bip01 R Foot");


	for ( int i = 0 ; i < BoneListNames.size() ; i++)
	{
		Bone * bone = mCharacterSkeleton->getBone( BoneListNames[i] );
		Ogre::Vector3 point1 = bone->_getDerivedPosition();
		Ogre::Vector3 point2 = bone->getChild( 0 )->_getDerivedPosition();

		float boneLength = point1.distance( point2 );
		Ogre::Vector3 midpoint = point1.midPoint( point2 );

		Ogre::Vector3 position = bone->_getDerivedPosition();
		Ogre::Quaternion rotation = bone->_getDerivedOrientation() * Ogre::Quaternion( Ogre::Degree( -90 ), Ogre::Vector3::UNIT_Z );
		
		float boneRadius = 0.06f;
		//boneLength -= boneRadius*2;
		if( boneLength <= 0.1f){ boneLength = 0.1f;}

		if( bone->getName().compare("Bip01 R Foot") == 0  || bone->getName().compare("Bip01 L Foot") == 0 )
		{
			rotation = bone->_getDerivedOrientation() * Ogre::Quaternion( Ogre::Degree( -90 ), Ogre::Vector3::UNIT_X );
			boneLength += boneRadius*2;
		}

		/*

		NxCapsuleObjectDesc ObjDesc;
		ObjDesc.Name = bone->getName().c_str();
		ObjDesc.Scene = mCharacterScene;
		ObjDesc.Size.x = boneRadius;
		ObjDesc.Size.y = boneLength;
		ObjDesc.Physic = PhysicGroup::GROUP_KINEMATIC;


		NxScene3DObjectEntity * PhysxEnt = NxScene3DManager::getSingleton().CreateCapsule( ObjDesc );
		PhysxEnt->SetVisible(false);

		//PhysxEnt->GetNxSceneNode()->setPosition( position  );
		//PhysxEnt->GetNxSceneNode()->setOrientation( rotation );
		//PhysxEnt->GetNxSceneNode()->setInitialState();
 
		PhysxEnt->SetPosition(   position  );
		//PhysxEnt->SetOrientation(  rotation  );
		PhysxEnt->Update( 0.0f ); 
		BonesActorsMap.insert( make_pair( bone, PhysxEnt ) );
		*/
	}
}

void NxScene3DObjectCharacter::CreatePhysxJoints()
{

	/*
	NxScene3DObjectEntity * LFoot  = mCharacterScene->GetNxEntityByName( "Bip01 L Foot_SceneNode" );
	NxScene3DObjectEntity * LThigh  = mCharacterScene->GetNxEntityByName( "Bip01 L Thigh_SceneNode" );
	NxScene3DObjectEntity * LCalf  = mCharacterScene->GetNxEntityByName( "Bip01 L Calf_SceneNode" );

	NxScene3DObjectEntity * RFoot  = mCharacterScene->GetNxEntityByName( "Bip01 R Foot_SceneNode" );
	NxScene3DObjectEntity * RThigh  = mCharacterScene->GetNxEntityByName( "Bip01 R Thigh_SceneNode" );
	NxScene3DObjectEntity * RCalf  = mCharacterScene->GetNxEntityByName( "Bip01 R Calf_SceneNode" );

	NxScene3DObjectEntity * Pelvis  = mCharacterScene->GetNxEntityByName( "Bip01 Pelvis_SceneNode" );

	NxScene3DObjectEntity * Spine  = mCharacterScene->GetNxEntityByName( "Bip01 Spine_SceneNode" );
	NxScene3DObjectEntity * Spine1 = mCharacterScene->GetNxEntityByName( "Bip01 Spine1_SceneNode" );	
	NxScene3DObjectEntity * Spine2 = mCharacterScene->GetNxEntityByName( "Bip01 Spine2_SceneNode" );
	NxScene3DObjectEntity * Spine3 = mCharacterScene->GetNxEntityByName( "Bip01 Spine3_SceneNode" );
	
	NxScene3DObjectEntity * LClavicle = mCharacterScene->GetNxEntityByName( "Bip01 L Clavicle_SceneNode" );
	NxScene3DObjectEntity * RClavicle = mCharacterScene->GetNxEntityByName( "Bip01 R Clavicle_SceneNode" );

	NxScene3DObjectEntity * LUpperArm = mCharacterScene->GetNxEntityByName( "Bip01 L UpperArm_SceneNode" );
	NxScene3DObjectEntity * LForearm = mCharacterScene->GetNxEntityByName( "Bip01 L Forearm_SceneNode" );

	NxScene3DObjectEntity * RUpperArm = mCharacterScene->GetNxEntityByName( "Bip01 R UpperArm_SceneNode" );
	NxScene3DObjectEntity * RForearm = mCharacterScene->GetNxEntityByName( "Bip01 R Forearm_SceneNode" );

	NxScene3DObjectEntity * LHand = mCharacterScene->GetNxEntityByName( "Bip01 L Hand_SceneNode" );
	NxScene3DObjectEntity * RHand = mCharacterScene->GetNxEntityByName( "Bip01 R Hand_SceneNode" );

	NxScene3DObjectEntity * Head = mCharacterScene->GetNxEntityByName( "Bip01 Head_SceneNode" );


	Vector3 globalAnchorUpper;
	Vector3 globalAxis = Vector3(0,1,0);

	//LEFT LEG//
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Foot")->_getDerivedPosition();
	NxD6Joint * Joint = NxScene3DManager::getSingleton().CreateD6Joint( LFoot->GetPhysxActor() , LCalf->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Calf")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( LCalf->GetPhysxActor() , LThigh->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Thigh")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( LThigh->GetPhysxActor() , Pelvis->GetPhysxActor(), globalAnchorUpper,  globalAxis );


	//RIGHT LEG//
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 R Foot")->_getDerivedPosition();  
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( RFoot->GetPhysxActor() , RCalf->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 R Calf")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( RCalf->GetPhysxActor() , RThigh->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Thigh")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( RThigh->GetPhysxActor() , Pelvis->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	//SPINE//
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 Pelvis")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Pelvis->GetPhysxActor() , Spine->GetPhysxActor(), globalAnchorUpper,  globalAxis );
	
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 Spine1")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Spine->GetPhysxActor() , Spine1->GetPhysxActor(), globalAnchorUpper,  globalAxis );
	
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 Spine2")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Spine1->GetPhysxActor() , Spine2->GetPhysxActor(), globalAnchorUpper,  globalAxis );
	
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 Spine3")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Spine2->GetPhysxActor() , Spine3->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Clavicle")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Spine3->GetPhysxActor() , LClavicle->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 R Clavicle")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Spine3->GetPhysxActor() , RClavicle->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 Head")->_getDerivedPosition();  
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( Spine3->GetPhysxActor() , Head->GetPhysxActor(),  globalAnchorUpper,  globalAxis );


	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L UpperArm")->_getDerivedPosition();  
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( LClavicle->GetPhysxActor() , LUpperArm->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 R UpperArm")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( RClavicle->GetPhysxActor() , RUpperArm->GetPhysxActor(), globalAnchorUpper,  globalAxis );


	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Forearm")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( LUpperArm->GetPhysxActor() , LForearm->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 R Forearm")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( RUpperArm->GetPhysxActor() , RForearm->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 L Hand")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( LForearm->GetPhysxActor() , LHand->GetPhysxActor(), globalAnchorUpper,  globalAxis );
	
	globalAnchorUpper = mCharacterSkeleton->getBone("Bip01 R Hand")->_getDerivedPosition();
	Joint = NxScene3DManager::getSingleton().CreateD6Joint( RForearm->GetPhysxActor() , RHand->GetPhysxActor(), globalAnchorUpper,  globalAxis );

	*/

}

void NxScene3DObjectCharacter::SetVisible( bool Visible )
{
	mNode->setVisible( Visible);
}

void NxScene3DObjectCharacter::SetMagnetic( bool Magnetic )
{
	//for( int i = 0 ; i < NxEntityCollection.size(); i++ )
	//{
	//	NxEntityCollection[i]->SetMagnetic( true );
	//	NxEntityCollection[i]->SetMagnetForce( 0.1 );
	//	//NxEntityCollection[i]->SetMagnetConstant( 0, 0, 0 );	 
	//}
}

 

void NxScene3DObjectCharacter::Kill( bool Kill )
{
	//mCharacterScene->Lock();
	/*

	IsDead = Kill;
	if( Kill )//dead
	{
		Log("Killing NxCharacter");
		for( int i = 0; i < NxEntityCollection.size(); i++ )
		{
			NxEntityCollection[i]->GetPhysxActor()->clearBodyFlag( NxBodyFlag::NX_BF_KINEMATIC );
			NxEntityCollection[i]->SetDynamic( true );

			//set ogrebones to this pos + ori
		}
	}
	else //alive
	{
		Log("Re live NxCharacter");
		for( int i = 0; i < NxEntityCollection.size(); i++ )
		{
			NxEntityCollection[i]->GetPhysxActor()->raiseBodyFlag( NxBodyFlag::NX_BF_KINEMATIC );
			NxEntityCollection[i]->SetDynamic( true );
		}


	}
	*/
	//mCharacterScene->Unlock();

}



void NxScene3DObjectCharacter::UpdatePhysx()
{
	/*
	Locker->lock();

	for( BindingBoneActors::iterator iter = BonesActorsMap.begin(); iter != BonesActorsMap.end(); iter++)
	{
		Ogre::Bone * CurrentBone = iter->first ;


		//Log( " Updating Bone Name :  " +  CurrentBone->getName()   );

		NxScene3DObjectEntity  * CurrentEntity = iter->second ;
		CurrentEntity->SetPosition( CurrentBone->_getDerivedPosition() + mSceneNode->getPosition() );

		if( CurrentBone->getName().compare("Bip01 R Foot") == 0  || CurrentBone->getName().compare("Bip01 L Foot") == 0 )
		{
			//X
			CurrentEntity->SetOrientation(  CurrentBone->_getDerivedOrientation() * Ogre::Quaternion( Ogre::Degree( -90 ), Ogre::Vector3::UNIT_X ) );
		}
		else
		{
			//Z
			CurrentEntity->SetOrientation(  CurrentBone->_getDerivedOrientation() * Ogre::Quaternion( Ogre::Degree( -90 ), Ogre::Vector3::UNIT_Z ) ); 
		}


		CurrentEntity->Update( 0.0f ); 

	}
	 
	for( BindingBoneActors::iterator iter = NxPropsCollection.begin(); iter != NxPropsCollection.end(); iter++)
	{
		Ogre::Bone * CurrentBone = iter->first ;
		NxScene3DObjectEntity  * CurrentEntity = iter->second ;

		if( CurrentEntity->GetNxType() == NxTypeCloth )
		{
			//CurrentEntity->MoveToPosition( CurrentBone->_getDerivedPosition()  );
			CurrentEntity->Update(0.0f);
		}
		else if( CurrentEntity->GetNxType() == NxTypeFluid )
		{
			CurrentEntity->MoveToPosition( CurrentBone->_getDerivedPosition()  );
			CurrentEntity->Update(0.0f);
		}
		else if( CurrentEntity->GetNxType() == NxTypeMagnet )
		{
			CurrentEntity->MoveToPosition( CurrentBone->_getDerivedPosition()  );
			CurrentEntity->Update(0.0f);
			CurrentEntity->GetPhysxActor()->setGlobalPosition( VecToPhysx( CurrentBone->_getDerivedPosition() ) );
			//((Nx3DObjectPhysicMagnet * ) CurrentEntity)->SetMagnetOffsetPosition( CurrentBone->_getDerivedPosition().x , CurrentBone->_getDerivedPosition().y, CurrentBone->_getDerivedPosition().z);
		}
		else
		{
			CurrentEntity->SetPosition( CurrentBone->_getDerivedPosition()  );
			CurrentEntity->SetOrientation ( CurrentBone->_getDerivedOrientation()  );
			CurrentEntity->Update(0.0f);
		}

	}

	Locker->unlock();

	*/
}

void NxScene3DObjectCharacter::SetRagdollVisible( bool Visible )
{
	/*
	for( BindingBoneActors::iterator iter = BonesActorsMap.begin(); iter != BonesActorsMap.end(); iter++)
	{
		Ogre::Bone * CurrentBone = iter->first ;
		NxScene3DObjectEntity  * CurrentEntity = iter->second ;
		CurrentEntity->GetNxSceneNode()->setVisible( Visible );
	}

	*/
}

void NxScene3DObjectCharacter::SetMocapData( std::vector< NxMocapJoint *> MocapData )
{


}


void NxScene3DObjectCharacter::SetBoneOrientation( NxMocapJoint Joint )
{

	if( mCharacterSkeleton->hasBone( Joint.mName ) )
	{
		Ogre::Bone* bone = mCharacterSkeleton->getBone( Joint.mName );
		Ogre::Quaternion qI = bone->getInitialOrientation();
		Ogre::Quaternion newQ( Joint.mOrientation[0], Joint.mOrientation[1],Joint.mOrientation[2],Joint.mOrientation[3]  );// = Quaternion::IDENTITY;

		//newQ.FromRotationMatrix( RotInfo );

		bone->resetOrientation(); //in order for the conversion from world to local to work.
		newQ = bone->convertWorldToLocalOrientation(newQ);

		bone->setOrientation(newQ*qI);  
	}
	
	/*
	if( mCharacterSkeleton->hasBone( Joint.mName ) )
	{
		Ogre::Bone * mBone = mCharacterSkeleton->getBone( Joint.mName );
		MasterMocapBones::iterator Iter = BoneFinal.find( Joint.mName );
		MocapBoneData data = Iter->second;

		Ogre::Quaternion Rot((Ogre::Real) Joint.mOrientation[0],(Ogre::Real) Joint.mOrientation[1],(Ogre::Real) Joint.mOrientation[2],(Ogre::Real) Joint.mOrientation[3]);
		Ogre::Quaternion RotFinal = data.QuatParent.Inverse() * Rot * data.QuatParent * data.QuatOffset;
		mBone->setOrientation( RotFinal );
		if( Joint.mIsRoot )
		{
			//Ogre::Vector3 Pos( Joint.mPosition[0], Joint.mPosition[1], Joint.mPosition[2] );
			//mBone->setPosition( Pos );
			float divider = mCharacterPosOffsetScale * NxSceneNode->getScale().y ;
			float offsetY = mBone->getInitialPosition().y ;//BoneInstance->getInitialPosition().y ;//- 0.5f;
			Vector3 DestPos( Joint.mPosition[0] / divider, ( Joint.mPosition[1] / divider ) + offsetY , Joint.mPosition[2]/divider);
			DestPos.x = DestPos.x + mCharacterPosOffsetX ;
			DestPos.y = DestPos.y + mCharacterPosOffsetY ;
			DestPos.z = DestPos.z + mCharacterPosOffsetZ ;
			mBone->setPosition( DestPos );
		}
	}
	*/

}

void NxScene3DObjectCharacter::SetBonePosition( NxMocapJoint Joint )
{
	if( mCharacterSkeleton->hasBone( Joint.mName ) )
	{
		Ogre::Bone* bone = mCharacterSkeleton->getBone( Joint.mName );
		bone->setPosition( Ogre::Vector3( Joint.mPosition ) );
	}

}

/* formocap
void NxScene3DObjectCharacter::TransformBone( NxMocapJoint Joint )
{
	
	if( mCharacterSkeleton->hasBone( Joint.mName ) )
	{
		Ogre::Bone * mBone = mCharacterSkeleton->getBone( Joint.mName );
		MasterMocapBones::iterator Iter = BoneFinal.find( Joint.mName );
		MocapBoneData data = Iter->second;

		Ogre::Quaternion Rot((Ogre::Real) Joint.mOrientation[0],(Ogre::Real) Joint.mOrientation[1],(Ogre::Real) Joint.mOrientation[2],(Ogre::Real) Joint.mOrientation[3]);
		Ogre::Quaternion RotFinal = data.QuatParent.Inverse() * Rot * data.QuatParent * data.QuatOffset;
		mBone->setOrientation( RotFinal );
		if( Joint.mIsRoot )
		{
			//Ogre::Vector3 Pos( Joint.mPosition[0], Joint.mPosition[1], Joint.mPosition[2] );
			//mBone->setPosition( Pos );
			float divider = mCharacterPosOffsetScale * NxSceneNode->getScale().y ;
			float offsetY = mBone->getInitialPosition().y ;//BoneInstance->getInitialPosition().y ;//- 0.5f;
			Vector3 DestPos( Joint.mPosition[0] / divider, ( Joint.mPosition[1] / divider ) + offsetY , Joint.mPosition[2]/divider);
			DestPos.x = DestPos.x + mCharacterPosOffsetX ;
			DestPos.y = DestPos.y + mCharacterPosOffsetY ;
			DestPos.z = DestPos.z + mCharacterPosOffsetZ ;
			mBone->setPosition( DestPos );
		}
	}
	
 
}*/

void NxScene3DObjectCharacter::Update( float time )
{

	mCharacterCamera->Update(time);


	Ogre::AnimationStateSet * Set = mCharacterEntity->getAllAnimationStates();
	Ogre::AnimationStateIterator iter = Set->getAnimationStateIterator();
	while( iter.hasMoreElements() ){
		Ogre::AnimationState * CurrentAnimation = iter.getNext();
		if( CurrentAnimation->getEnabled()){
			CurrentAnimation->addTime( time );
		}
	}




	return;
 

	for( MasterMocapBones::iterator iter = BoneFinal.begin(); iter != BoneFinal.end(); iter++)
	{
		MasterMocapList::const_iterator mit( mocapdef.find( iter->first )), mend( mocapdef.end());
		if( mit != mend ) // bone found in list
		{
			//Bone * BoneInstance = CharacterEntity->getSkeleton()->getBone( iter->first );
			//if(  BoneInstance )
			//{
				MocapBoneData data = iter->second ;
				data.CurrentBone->setOrientation( NxQuatToOgre( data.CurrentOrientation ) );
				if(data.IsRoot){ data.CurrentBone->setPosition(  NxVec3ToOgre( data.CurrentPosition ) );  }
			//}
		}
	}

	/*
	AnimationStateSet * set = CharacterEntity->getAllAnimationStates();
	if( set )
	{
		AnimationStateIterator iter = set->getAnimationStateIterator(); 
		while( iter.hasMoreElements() )
		{
			Ogre::AnimationState * state = iter.getNext();
			if( state->getEnabled() ){ state->addTime( time ); }
		}
	}
	*/

}

/*
void NxScene3DObjectCharacter::Update( const NxMocapDataList & List )
{

	for( MasterMocapBones::iterator iter = BoneFinal.begin(); iter != BoneFinal.end(); iter++)
	{
		string bonename = ( string )iter->first ;
		MocapBoneData & data = iter->second ;
		MasterMocapList::const_iterator mit( mocapdef.find( bonename )), mend( mocapdef.end());

		if( mit != mend ) // bone found in list
		{
			int SensorID = mit->second ;

			NxMocapDataList::const_iterator p;
			p = List.find( SensorID - 1 );
			NxMocapData Data = *p->second;

			Ogre::Quaternion rotation((Ogre::Real) Data[3],(Ogre::Real) Data[4],(Ogre::Real) Data[5],(Ogre::Real) Data[6]);

			
			//Radian angle; Vector3 axis;
			//rotation.ToAngleAxis(angle, axis);
			//Vector3 modifiedAxis = data.QuatParent * axis;
			//Quaternion modifiedRotation( angle, modifiedAxis );
			

			Quaternion newRotation = data.QuatParent.Inverse() * rotation * data.QuatParent * data.QuatOffset;
			data.CurrentOrientation = newRotation ;
			//BoneInstance->setOrientation(newRotation);

			if( data.IsRoot )
			{
				//NxMocapDataList::const_iterator p;
				//p = List.find( 1 );
				//NxMocapData Data = *p->second;
				//Vector3 OgreVect( Data[0], Data[1], Data[2] );
				float divider = mCharacterPosOffsetScale * NxSceneNode->getScale().y ;
				float offsetY = data.CurrentBone->getInitialPosition().y ;//BoneInstance->getInitialPosition().y ;//- 0.5f;
				Vector3 DestPos(Data[0] / divider, ( Data[1] / divider ) + offsetY , Data[2]/divider);
				DestPos.x = DestPos.x + mCharacterPosOffsetX ;
				DestPos.y = DestPos.y + mCharacterPosOffsetY ;
				DestPos.z = DestPos.z + mCharacterPosOffsetZ ;
				data.CurrentPosition = DestPos ;
				//BoneInstance->setPosition(  DestPos );
			} 
		}
	}
	

}*/

/*
void NxScene3DObjectCharacter::Update( NxMocapPlayer * PlayerCallback )
{
	
	if( IsDead )
	{
	

	}
	else
	{
		for( MasterMocapBones::iterator iter = BoneFinal.begin(); iter != BoneFinal.end(); iter++)
		{
			string bonename = ( string )iter->first ;
			MocapBoneData data = iter->second ;
			data.CurrentBone->setManuallyControlled(true);

			MasterMocapList::const_iterator mit( mocapdef.find( bonename )), mend( mocapdef.end());

			if( mit != mend ) // bone found in list
			{
				int SensorID = mit->second ;
				CQuat quater  ; 
				CVector3D Poser ;

				switch(SensorID)
				{
					case 1: // SACROILIAC
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::SACROILIAC ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::SACROILIAC ) ;
					break ;
					case 2: // head
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::VC7 ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::VC7 ) ;
					break ;
					case 12 : // spine 1
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::VL5 ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::VL5 ) ;
					break;
					case 6 : // left upper arm
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::L_SHOULDER ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::L_SHOULDER ) ;
					break;
					case 7 : // left down arm
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::L_ELBOW ) ;
					Poser = PlayerCallback->GetHAnimJointPosition( CHAnim::EJoints::L_ELBOW ) ;
					break;
					case 3 : // left upper leg
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::L_HIP ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::L_HIP ) ;
					break;
					case 11 : // left down leg
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::L_KNEE ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::L_KNEE ) ;
					break;
					case 4 : // right upper arm
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::R_SHOULDER ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::R_SHOULDER ) ;
					break;
					case 5 : // right down arm
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::R_ELBOW ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::R_ELBOW ) ;
					break;
					case 8 : // right upper leg
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::R_HIP ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::R_HIP ) ;
					break;
					case 9 : // right down leg
					quater = PlayerCallback->GetHAnimJointRotation( CHAnim::EJoints::R_KNEE ) ;
					Poser = PlayerCallback->GetHAnimJointPosition(CHAnim::EJoints::R_KNEE ) ;
					break;
					default :
						return ;
					break ;
				}

				Ogre::Quaternion rotation((Ogre::Real)quater.w,(Ogre::Real)quater.x,(Ogre::Real) quater.y,(Ogre::Real) quater.z );
				Vector3 axis;
				Radian angle;

				rotation.ToAngleAxis(angle, axis);
		          
				Vector3 modifiedAxis = data.QuatParent * axis;
				Quaternion modifiedRotation( angle, modifiedAxis );

				Bone * BoneInstance = CharacterEntity->getSkeleton()->getBone( bonename );
				if(  BoneInstance )
				{
					//BoneInstance->setOrientation(modifiedRotation*data.QuatOffset);
					Quaternion newRotation = data.QuatParent.Inverse() * rotation * data.QuatParent * data.QuatOffset;

					BoneInstance->setOrientation(newRotation);
 
					if( BoneInstance->getName() == "Bip01" )//root bone
					{
						CVector3D vec = PlayerCallback->GetHAnimJointPosition(  CHAnim::EJoints::SACROILIAC  );
						Vector3 OgreVect( vec.v[0], vec.v[1], vec.v[2] );
					 
						float divider = AvatarRootTranslateScaleFloat * NxSceneNode->getScale().y ;
						float offsetY =  BoneInstance->getInitialPosition().y ;//- 0.5f;
						Vector3 DestPos(vec.v[0] / divider, ( vec.v[1] / divider ) + offsetY , vec.v[2]/divider);
						DestPos.x = DestPos.x + AvatarRootTranslateXFloat ;
						DestPos.y = DestPos.y + AvatarRootTranslateYFloat ;
						DestPos.z = DestPos.z + AvatarRootTranslateZFloat ;
						BoneInstance->setPosition( DestPos );

					} // if Bip01

				}// if bone instance
			}	 
		}

		return ;
	}
	
}*/


void NxScene3DObjectCharacter::Calibrate( bool Calibrate )
{

}

void NxScene3DObjectCharacter::SetPosition( const Nx::Vector3 & Position )
{
	Ogre::Vector3 pos;
	NxVec3ToOgre( pos, Position ); 
	mNode->setPosition( pos );
	mCharacterCamera->SetPosition(  Position  );
}

const Nx::Vector3 & NxScene3DObjectCharacter::GetPosition() const
{
	return Nx::Vector3(mNode->getPosition());
}
	
void NxScene3DObjectCharacter::SetOrientation( const Nx::Quaternion & Orientation )
{
	Ogre::Quaternion quat;
	NxQuatToOgre( quat, Orientation ); 
	mNode->setOrientation( quat );
}

const Nx::Quaternion & NxScene3DObjectCharacter::GetOrientation() const
{
	return Nx::Quaternion(mNode->getOrientation());
}

void NxScene3DObjectCharacter::SetScale( const Nx::Vector3 & Scale )
{
	Ogre::Vector3 sca;
	NxVec3ToOgre( sca, Scale ); 
	mNode->setScale( sca );
}

const Nx::Vector3 & NxScene3DObjectCharacter::GetScale() const
{
	return Nx::Vector3(mNode->getScale());
}


void NxScene3DObjectCharacter::SetAnimation( const std::string & AnimationName, bool Looped )
{
	Ogre::AnimationStateSet * Sets =  mCharacterEntity->getAllAnimationStates();
	if( Sets->hasAnimationState( AnimationName ) )
	{
		Ogre::AnimationState * state = mCharacterEntity->getAnimationState( AnimationName );
		state->setEnabled( true );
		state->setLoop( Looped );
		LogMsg(" Animation Enabled : " + AnimationName );
	}
	else
	{
		LogMsg("Could not find animation : " + AnimationName );
	}
}

void NxScene3DObjectCharacter::SetMaterialName( const std::string & MaterialName )
{
	if( mCharacterEntity )
	{
		mCharacterEntity->setMaterialName( MaterialName ); 
	}
}

void NxScene3DObjectCharacter::AjustClothVertexToBone( Nx3DObjectPhysicCloth * Cloth, int VertexIndex, const std::string& BoneName, Ogre::Vector3 Offset )
{
	/*
	if( NxClothVertexAttachmentStatus::NX_CLOTH_VERTEX_ATTACHMENT_SHAPE == Cloth->GetNxCloth()->getVertexAttachmentStatus( 0 ))
	{
		Cloth->GetNxCloth()->freeVertex( 0 ) ;
		NxVec3 Rotation( Offset.z ,Offset.x ,Offset.y );
		//Cloth->GetNxCloth()->attachVertexToShape( VertexIndex , *Cloth->GetPhysxActor()->getShapes(), Rotation, 0 );
		AttachClothVertexToBone( Cloth, VertexIndex, BoneName, Offset );
	}*/
}

void NxScene3DObjectCharacter::AttachClothVertexToBone( Nx3DObjectPhysicCloth * Cloth, int VertexIndex, const std::string& BoneName, Ogre::Vector3 Offset )
{
	/*
	for( BindingBoneActors::iterator iter = BonesActorsMap.begin(); iter != BonesActorsMap.end(); iter++)
	{
		Ogre::Bone * CurrentBone = iter->first ;
		NxScene3DObjectEntity  * CurrentEntity = iter->second ;

		if( CurrentBone->getName().compare( BoneName ) == 0  )
		{
			NxActor * DestActor = CurrentEntity->GetPhysxActor();
			//mCloth->detachFromShape( *Actor->getShapes()  ) ;
			//mCloth->freeVertex( 0 ) ;
			//mCloth->freeVertex( 30 ) ;
			NxVec3 Rotation( Offset.z ,Offset.x ,Offset.y );
			Cloth->GetNxCloth()->attachVertexToShape( VertexIndex , *DestActor->getShapes(), Rotation, 0 ); // with some offset
		}
	}
	*/
}



}