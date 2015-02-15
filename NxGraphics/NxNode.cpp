#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "NxMeshManager.h"

namespace Nx {

NxNode::NxNode( Nx3D_Scene * NxManager, const std::string & Name ) : mScene( NxManager ) 
{
	mActor = NULL;
	mValid = true;
	mLifetime = -1.0f;
	mCurrentTime = 0.0f;
	mSceneNode = NxManager->GetNxSceneManager()->createSceneNode( Name ); 
	NxManager->GetNxSceneManager()->getRootSceneNode()->addChild( mSceneNode );
}

NxNode::~NxNode()
{
	LogMsg("Erasing NxNode : " + GetName() );

	LogMsg("Erasing NxActors ... " );
	NxActorListIterator NxActorsIter = GetNxActorIterator();
	while( NxActorsIter.hasMoreElements() ){
		delete NxActorsIter.getNext();
	}
	mNxActors.clear();
	LogMsg("Erasing NxActors : DONE" );

	LogMsg("Erasing NxSounds ... " );
	NxSoundMapIterator SoundIter = GetNxSoundIterator();
	while( SoundIter.hasMoreElements() ){
		delete SoundIter.getNext();
	}
	LogMsg("Erasing NxSounds : DONE" );

	LogMsg("Erasing NxEntities... " );
	NxEntityListIterator EntityIter = GetNxEntityIterator();
	while( EntityIter.hasMoreElements() ){
		delete EntityIter.getNext();
	}
	LogMsg("Erasing NxEntities : DONE" );

	Ogre::SceneManager * mManager = mSceneNode->getCreator(); // detach child first ?
	mManager->destroySceneNode( mSceneNode );

	LogMsg("Erasing NxNode : Done" );	
	LogMsg("-----------------------");
}

Nx3D_Scene & NxNode::GetNxScene()
{
	return *mScene;
}
 
NxCharacterController * NxNode::CreateNxController( const std::string & ControllerName )
{
	if( mNxControllers.find( ControllerName ) != mNxControllers.end()) {
		OGRE_EXCEPT( Exception::ERR_DUPLICATE_ITEM, "A camera with the name " + ControllerName + " already exists", "NxNode::CreateNxController" );
	}
	NxCharacterController * c = new NxCharacterController( this, ControllerName );
	c->CreateCharacterController( 0.5, 2);
	mNxControllers.insert( NxControllerMap::value_type( ControllerName, c ) );
	// for update
	mNxEntities.insert( NxEntityList::value_type( ControllerName, c ) );
	return c;
}
 
NxCharacterController * NxNode::GetNxController( const std::string & ControllerName ) const
{
    NxControllerMap::const_iterator i = mNxControllers.find(ControllerName);
    if (i == mNxControllers.end()) {
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND,  "Cannot find Camera with name " + ControllerName, "NxNode::GetNxController");
    }
    else {
        return i->second;
    }
}

NxControllerMapIterator NxNode::GetNxControllerIterator()
{
	return NxControllerMapIterator( mNxControllers.begin(), mNxControllers.end() );
}

NxEntity * NxNode::CreateNxEntity( const std::string & MeshName )
{
	NxEntity * c = new NxEntity( this, MeshName );
	mNxEntities.insert( NxEntityList::value_type( MeshName, c ) );
	return c;
}

NxEntity * NxNode::CreateNxBox( const std::string & BoxName, const Nx::Vector3 & Dimensions, const Nx::Vector3 & NumSegments  )
{
	Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateBox( BoxName, Dimensions, NumSegments );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	NxEntity * c = new NxEntity( this, BoxName );
	mNxEntities.insert( NxEntityList::value_type( BoxName, c ) );
	return c;
}

NxEntity * NxNode::CreateNxSphere( const std::string & SphereName, float Radius, unsigned int numRings, unsigned int numSegments , float uTile, float vTile )
{
	Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateSphere( SphereName, Radius, numRings, numSegments, uTile, vTile );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	/*
	Ogre::Mesh::LodValueList lodDList;
	lodDList.push_back(4.0);
	lodDList.push_back(8.0);
	//lodDList.push_back(128.0);
	Ogre::ProgressiveMesh::generateLodLevels( NxMesh.getPointer(),lodDList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, 0.5);
	*/



	NxEntity * c = new NxEntity( this, SphereName );
	mNxEntities.insert( NxEntityList::value_type( SphereName, c ) );
	return c;
}

NxEntity * NxNode::CreateNxCapsule( const std::string & CapsuleName, const Nx::Vector2 & Dimensions )
{
	Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateCapsule( CapsuleName, Dimensions.x, Dimensions.y-(Dimensions.x*2) );
	//Ogre::MeshPtr NxMesh = NxMeshFactory::getSingleton().CreateCapsule( CapsuleName, Dimensions.x, Dimensions.y, mScene->GetNxSceneManager() );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	NxEntity * c = new NxEntity( this, CapsuleName );
	mNxEntities.insert( NxEntityList::value_type( CapsuleName, c ) );
	return c;
}

NxEntity * NxNode::CreateNxPlane( const std::string & PlaneName, const Nx::Vector2 & Dimensions, const Nx::Vector2 & NumSegments )
{
	Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreatePlane( PlaneName, Dimensions, NumSegments );	

	/*AxisAlignedBox boxer;
	boxer.setInfinite();
	NxMesh->_setBounds(boxer);*/

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	NxEntity * c = new NxEntity( this, PlaneName );
	mNxEntities.insert( NxEntityList::value_type( PlaneName, c ) );
	return c;
}

NxEntity * NxNode::CreateNxCylinder( const std::string & CylinderName, float Radius, float Height )
{
	Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateCylinder( CylinderName, Radius, Height );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	NxEntity * c = new NxEntity( this, CylinderName );
	mNxEntities.insert( NxEntityList::value_type( CylinderName, c ) );
	return c;
}

NxEntity * NxNode::CreateNxCone( const std::string & ConeName, float Radius, float Height )
{
	Ogre::MeshPtr NxMesh = NxMeshManager::getSingleton().CreateCone( ConeName, Radius, Height  );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	NxEntity * c = new NxEntity( this, ConeName );
	mNxEntities.insert( NxEntityList::value_type( ConeName, c ) );
	return c;
}


NxRectangle2D * NxNode::CreateNxRectangle2D( const std::string & RectangleName, bool includeTextureCoordinates ) {

	NxRectangle2D * c = new NxRectangle2D( this, includeTextureCoordinates );
	mNxEntities.insert( NxEntityList::value_type( RectangleName, c ) );
	return c;
}

Nx3DObjectPhysicFluid * NxNode::CreateNxParticles( const std::string & ParticlesName, unsigned long NumParticles, bool UseVertexColor )
{
	Nx3DObjectPhysicFluid * c = new Nx3DObjectPhysicFluid( this, NumParticles, UseVertexColor );
	mNxEntities.insert( NxEntityList::value_type( ParticlesName, c ) );
	return c;
}

NxParticleSystem * NxNode::CreateParticleSystem( const std::string & ParticleSystemName, unsigned long NumParticles  )
{
	NxParticleSystem * c = new NxParticleSystem( this, NumParticles );
	mNxEntities.insert( NxEntityList::value_type( ParticleSystemName, c ) );
	return c;
}
 
Nx3DObjectPhysicClothe * NxNode::CreateNxCloth( const std::string & ClotheName, const NxClotheDesc & ClotheDesc )
{
	Nx3DObjectPhysicClothe * c = new Nx3DObjectPhysicClothe( this, ClotheDesc );
	mNxEntities.insert( NxEntityList::value_type( ClotheName, c ) );
	return c;
}

Nx3DObjectPhysicLine * NxNode::CreateNxLine( const std::string & LineName, bool UseVertexColor  ) {
	Nx3DObjectPhysicLine * c = new Nx3DObjectPhysicLine( this, LineName, UseVertexColor );
	mNxEntities.insert( NxEntityList::value_type( LineName, c ) );
	return c;
}

Nx3DObjectPhysicLine * NxNode::GetNxLine( const std::string & Name ) const
{
	NxEntityList::const_iterator i = mNxEntities.find( Name );
	if( i == mNxEntities.end() ){
		OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find NxLine with name " + Name, "NxNode::GetNxLine");
	}
	else{
		return (Nx3DObjectPhysicLine *)i->second;
	}
}


void NxNode::DeleteObject( const std::string & Name ) {
	NxEntityList::iterator i = mNxEntities.find(Name );
	if (i == mNxEntities.end()) {


		//for(NxEntityList::iterator p = mNxEntities.begin(); p != mNxEntities.end(); )
		//{
		//	Log( p->first);
		//	p++;
		//}
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "DeleteObject '" + Name  + "' not found.", "NxNode::DeleteObject");
	}

	delete i->second;
	mNxEntities.erase(i);
}

void NxNode::DeleteNxLine( const std::string & LineName ) {
	NxEntityList::iterator i = mNxEntities.find(LineName);
	if (i == mNxEntities.end()) {

	 
		//for(NxEntityList::iterator p = mNxEntities.begin(); p != mNxEntities.end(); )
		//{
		//	Log( p->first);
		//	p++;
		//}
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "NxLine '" + LineName + "' not found.", "NxNode::DeleteNxLine");
	}

	delete i->second;
	mNxEntities.erase(i);
}

Nx3DObjectText * NxNode::CreateNxText( const std::string & TextName, const std::string &caption, const std::string & FontName , float charHeight , const NxColourValue &color  ) {

	Nx3DObjectText * c = new Nx3DObjectText( this, TextName, caption,  FontName ,  charHeight ,    color   );
	mNxEntities.insert( NxEntityList::value_type( TextName, c ) );
	return c;
}

NxScene3DObjectCharacter * NxNode::CreateNxCharacter( const std::string & CharacterMesh )
{
	NxScene3DObjectCharacter * c = new NxScene3DObjectCharacter( this );
	c->CreateCharacter( CharacterMesh );
	c->SetRagdollVisible( false );
	//! Set character node
	c->GetNxCamera()->SetNxCharacter( c );
	//! Set camera view
	c->GetNxCamera()->SetNavigationMode( ModeCharacter );
	//! set bundle active
	c->GetNxCamera()->SetActive( true );
	mNxEntities.insert( NxEntityList::value_type( CharacterMesh, c ) );
	return c;
}

NxEntityListIterator NxNode::GetNxEntityIterator()
{
	return NxEntityListIterator( mNxEntities.begin(), mNxEntities.end());
}

NxEntity * NxNode::GetNxEntity( const std::string& Name ) const
{
    NxEntityList::const_iterator i = mNxEntities.find( Name );
    if (i == mNxEntities.end()){
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find Node with name " + Name, "NxNode::GetAttachedObject");
    }
    else {
        return i->second;
    }
}

NxPhysicsActor * NxNode::CreateNxActor( const std::string & Name, const PhysicGroup Property )
{
	NxPhysicsActor * c = new NxPhysicsActor( this, Name, Property );
	mNxActors.insert( NxActorList::value_type( Name, c ) );
	return c;
}

NxActorListIterator NxNode::GetNxActorIterator()
{
	return NxActorListIterator( mNxActors.begin(), mNxActors.end());
}

NxPhysicsActor * NxNode::GetNxActor( const std::string & Name ) const
{
    NxActorList::const_iterator i = mNxActors.find( Name );
    if( i == mNxActors.end() ){
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find NxPhysicsActor with name " + Name, "NxNode::GetNxActor");
    }
    else{
        return i->second;
    }
}

/*
NxCamera * NxNode::CreateNxCamera( const std::string & CameraName )
{
	NxCamera * c = new NxCamera( this, CameraName );
	mNxCameras.insert( NxCameraMap::value_type( CameraName, c ) );
	return c;
}

NxCamera * NxNode::GetNxCamera( const std::string & CameraName ) const
{
	NxCameraMap::const_iterator i = mNxCameras.find( CameraName );
	if( i == mNxCameras.end() ) {
		OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find NxCamera with name " + CameraName, "NxNode::GetNxCamera");
	}
	else{
		return i->second;
	}
}

NxCameraMapIterator NxNode::GetNxCameraIterator()
{
	return NxCameraMapIterator( mNxCameras.begin(), mNxCameras.end());
}*/

NxScene3DObjectLight * NxNode::CreateNxLight( const std::string & LightName, const NxLightType Type )
{
	NxScene3DObjectLight * c = new NxScene3DObjectLight( this, LightName, Type );
	mNxLights.insert( NxLightList::value_type( LightName, c ) );
	return c;
}

NxLightListIterator NxNode::GetNxLightIterator()
{
	return NxLightListIterator( mNxLights.begin(), mNxLights.end());
}

NxScene3DObjectLight * NxNode::GetNxLight( const std::string & LightName ) const
{
    NxLightList::const_iterator i = mNxLights.find( LightName );
    if( i == mNxLights.end() ){
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find NxLight with name " + LightName, "NxNode::GetNxLight");
    }
    else{
        return i->second;
    }
}

NxScene3DObjectSound * NxNode::CreateNxSound(  const std::string & SoundName, const std::string & SoundFile )
{
	NxScene3DObjectSound * c = new NxScene3DObjectSound( SoundName, SoundFile );
	mNxSounds.insert( NxSoundList::value_type( SoundName, c ) );
	return c;
}

NxScene3DObjectSound * NxNode::GetNxSound( const std::string & SoundName ) const
{
    NxSoundList::const_iterator i = mNxSounds.find( SoundName );
    if( i == mNxSounds.end() ){
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find NxSound with name " + SoundName, "NxNode::GetNxSound");
    }
    else{
        return i->second;
    }
}

NxSoundMapIterator NxNode::GetNxSoundIterator()
{
	return NxSoundMapIterator( mNxSounds.begin(), mNxSounds.end());
}

std::string NxNode::GetName()
{
	return std::string( mSceneNode->getName().c_str() );
}

Ogre::SceneNode * NxNode::GetNxSceneNode()
{ 
	return mSceneNode;
}

bool NxNode::SetNxActor( const std::string & ActorName )
{
	NxPhysicsActor * Actor = GetNxScene().GetNxActor( ActorName );
	if( Actor ) {

		mSceneNode->setPosition( NxVec3ToOgre( Actor->GetPosition() ) );
		mSceneNode->setOrientation( NxQuatToOgre( Actor->GetOrientation() ) );

		mActor = Actor;
	}
	else{ LogMsg("Didnt Find at NxNode::SetNxActor name : " + ActorName ); }
	return true;


}

void NxNode::DetachFromParent()
{
	Ogre::SceneNode * parent = mSceneNode->getParentSceneNode();
	if(parent) parent->removeChild( mSceneNode->getName() );
}

void NxNode::AddChild( NxNode * obj )
{
	mSceneNode->addChild( obj->GetNxSceneNode() );
}

void NxNode::SetFixedYawAxis( bool useFixed, const Nx::Vector3& fixedAxis ) {

	Ogre::Vector3 axis;
	NxVec3ToOgre( axis, fixedAxis);

	mSceneNode->setFixedYawAxis( useFixed, axis );
}

void NxNode::Yaw(const Nx::Radian& angle, NxTransformSpace relativeTo ) {

	Ogre::Radian rad;
	NxRadiantoOgre( rad, angle );
	mSceneNode->yaw( rad, ( Ogre::Node::TransformSpace ) relativeTo );
}
 
void NxNode::SetDirection(const Nx::Vector3& vec, NxTransformSpace relativeTo, const Nx::Vector3& localDirectionVector ) {

	Ogre::Vector3 dir;
	NxVec3ToOgre( dir, vec);

	Ogre::Vector3 DirectionVector;
	NxVec3ToOgre( DirectionVector, localDirectionVector);

	mSceneNode->setDirection( dir,  ( Ogre::Node::TransformSpace ) relativeTo, DirectionVector );
}

void NxNode::LookAt( const Nx::Vector3& targetPoint, NxTransformSpace relativeTo, const Nx::Vector3& localDirectionVector ) {

	Ogre::Vector3 target;
	NxVec3ToOgre( target, targetPoint);

	Ogre::Vector3 DirectionVector;
	NxVec3ToOgre( DirectionVector, localDirectionVector);

	mSceneNode->lookAt( target, ( Ogre::Node::TransformSpace ) relativeTo, DirectionVector );
}

void NxNode::SetAutoTracking(bool enabled, NxNode* const target, const Nx::Vector3& localDirectionVector, const Nx::Vector3& offset ) {

	Ogre::Vector3 DirectionVector;
	NxVec3ToOgre( DirectionVector,localDirectionVector);

	Ogre::Vector3 off;
	NxVec3ToOgre( off,offset);

	mSceneNode->setAutoTracking( enabled, target->GetNxSceneNode(), DirectionVector, off );
}

void NxNode::Translate( const Nx::Vector3 & Position,  NxTransformSpace Space )
{
	Ogre::Vector3 pos;
	NxVec3ToOgre(pos,Position);
	mSceneNode->translate( pos, (Ogre::Node::TransformSpace) Space  );
}

void NxNode::Rotate( const Nx::Quaternion & Orientation, NxTransformSpace Space )
{
	Ogre::Quaternion quat;
	NxQuatToOgre(quat, Orientation );
	mSceneNode->rotate( quat, (Ogre::Node::TransformSpace)Space );
}

void NxNode::SetPosition( const Nx::Vector3 & Position )
{
	Ogre::Vector3 pos;
	NxVec3ToOgre(pos,Position);
	mSceneNode->setPosition( pos );
}

const Nx::Vector3 & NxNode::GetDerivatedPosition() const
{
	//Ogre::Vector3 dede1(0,0,0);
	//Nx::Vector3 dede;
	//dede.operator = dede1;
	
	return Nx::Vector3( mSceneNode->_getDerivedPosition() );
}

const Nx::Quaternion & NxNode::GetDerivatedOrientation() const
{
	return Nx::Quaternion(mSceneNode->_getDerivedOrientation());
}

const Nx::Vector3 & NxNode::GetDerivatedScale() const
{
	return Nx::Vector3( mSceneNode->_getDerivedScale() );
}

const Nx::Vector3 & NxNode::GetPosition() const
{
	return Nx::Vector3(mSceneNode->getPosition());
}

void NxNode::SetOrientation( const Nx::Quaternion & Orientation )
{
	Ogre::Quaternion quat;
	NxQuatToOgre(quat, Orientation );
	mSceneNode->setOrientation( quat );
}

const Nx::Quaternion & NxNode::GetOrientation() const
{
	return Nx::Quaternion(mSceneNode->getOrientation());
}

void NxNode::SetScale( const Nx::Vector3 & Scale )
{
	Ogre::Vector3 sca;
	NxVec3ToOgre(sca,Scale);
	mSceneNode->setScale( sca );
}

const Nx::Vector3 & NxNode::GetScale() const
{
	return Nx::Vector3(mSceneNode->getScale());
}

void NxNode::SetInheritScale( bool inherit )
{
	mSceneNode->setInheritScale( inherit );
}

void NxNode::SetInheritOrientation( bool inherit )
{
	mSceneNode->setInheritOrientation( inherit );
}

void NxNode::SetNodeAnimation( const std::string& AnimationName, bool Looped )
{
	std::list<AnimationState *> NodeAnimationNames ;
	SceneManager::AnimationIterator animationIt = mSceneNode->getCreator()->getAnimationIterator();

	while( animationIt.hasMoreElements() )  {
		Animation * animation = animationIt.getNext();
		const Animation::NodeTrackList& trackList = animation->_getNodeTrackList();
		Animation::NodeTrackList::const_iterator it = trackList.begin();
		Animation::NodeTrackList::const_iterator iend = trackList.end();
		Ogre::String AnimationName = animation->getName();

		for(; it != iend; ++it) {
			const Ogre::NodeAnimationTrack* track = it->second;
			if( track->getAssociatedNode() == mSceneNode ) {
				track->getAssociatedNode()->resetToInitialState(); 
				AnimationState * state = mSceneNode->getCreator()->getAnimationState( animation->getName() );
				if( state->getAnimationName().compare( Ogre::String( AnimationName.c_str() )  ) == 0 ) {
					state->setEnabled(true);
					state->setLoop( Looped );
				}
			}
		}
	}
}

void NxNode::SetDebugEnabled( bool Debug )
{
	mSceneNode->setDebugDisplayEnabled( Debug );
	mSceneNode->showBoundingBox( Debug );
}

void NxNode::Update( const float time )
{

	 
	if( mActor ) {
		// set node derived position and orientation from physic actor transform.
		Ogre::Quaternion qParent = mSceneNode->getParentSceneNode()->_getDerivedOrientation().Inverse();
		Ogre::Vector3 vParent = mSceneNode->getParentSceneNode()->_getDerivedPosition();
		Ogre::Vector3 newPos = (  NxVec3ToOgre( mActor->GetPosition() ) - vParent);
		newPos /= mSceneNode->getParentSceneNode()->_getDerivedScale();
		Ogre::Vector3 pos = qParent * newPos;
		mSceneNode->setPosition( pos );
		mSceneNode->setOrientation(  NxQuatToOgre(  mActor->GetOrientation() ) *  mSceneNode->getParentSceneNode()->_getDerivedOrientation().Inverse() );
	}  

	// Update NxEntities attached to this node.
	NxEntityListIterator Iter = GetNxEntityIterator();
	//LogMsg("num entities : " + Ogre::StringConverter::toString( mNxEntities.size() )  );   
	while( Iter.hasMoreElements() ) {
		NxEntity * entity = Iter.getNext();
		//LogMsg(" entities name : " + entity->GetName()  );
		entity->Update( time );
	}

	// increment time, check if node is still valid.
 	mCurrentTime += ( time ) ;
	if( mLifetime != -1.0f && mValid ) { 
		if( mCurrentTime >= (mLifetime - 1.0f)) { 
			mValid = false; mCurrentTime = 0.0f;
		}
	} 
}

void NxNode::SetLifeTime( const float milliseconds )
{
	mLifetime = milliseconds / 1000.0f;
}

float NxNode::GetLifetime()
{
	return mLifetime * 1000.0f;
}

bool NxNode::IsValid()
{
	return mValid;
}

void NxNode::OnMouseMoved( const OIS::MouseEvent &e )
{
	NxEntityListIterator Iter = GetNxEntityIterator();
	while( Iter.hasMoreElements() ){ Iter.getNext()->OnMouseMoved( e ); }
}

void NxNode::OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	NxEntityListIterator Iter = GetNxEntityIterator();
	while( Iter.hasMoreElements() ){ Iter.getNext()->OnMousePressed( e, id ); }
}

void NxNode::OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	NxEntityListIterator Iter = GetNxEntityIterator();
	while( Iter.hasMoreElements() ){ Iter.getNext()->OnMouseReleased( e, id ); }
}

void NxNode::OnKeyPressed( const NxKeyCode &e )
{
	NxEntityListIterator Iter = GetNxEntityIterator();
	while( Iter.hasMoreElements() ){ Iter.getNext()->OnKeyPressed( e ); }
}

void NxNode::OnKeyReleased( const NxKeyCode &e )
{
	NxEntityListIterator Iter = GetNxEntityIterator();
	while( Iter.hasMoreElements() ){ Iter.getNext()->OnKeyReleased( e ); }
}

}//namespace