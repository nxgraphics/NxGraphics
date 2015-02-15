#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "NxMeshManager.h"
#include "../nxvideo/NxVideo_Headers.h"
 
//#include "..\nxdeps\include\OGRE\OgreProgressiveMesh.h"
 
namespace Nx {

NxEntity::NxEntity() : mEntity(0)
{
	mSound = 0;
	mActor = 0;
	mType = NxTypeEntity;
	mIsSelected = false;	
}

NxEntity::NxEntity( NxNode * parentNode, const std::string & MeshName ) : mScene( &parentNode->GetNxScene() ), mEntity(0)
{

	/*

	LOD TEST
	Ogre::MeshPtr MyMesh =  Ogre::MeshManager::getSingleton().load( MeshName, "General" );
	MyMesh->setLodStrategy( Ogre::LodStrategyManager::getSingleton().getDefaultStrategy() );
	if( !MyMesh.get() ) Log( "MESH INVALID" );
	Log( "00" );
	Ogre::ProgressiveMesh::LodValueList lodValues;
	lodValues.push_back(50);
	lodValues.push_back(100);
	lodValues.push_back(150);
	lodValues.push_back(200);
	bool OK = Ogre::ProgressiveMesh::generateLodLevels( MyMesh.get(), lodValues, Ogre::ProgressiveMesh::VertexReductionQuota::VRQ_PROPORTIONAL, 2 );

	if( !OK ) Log("COULD NOT BAKE THE MESH");
	Log( "01" );

	*/
 
	mSound = 0;
	mActor = 0;
	mType = NxTypeEntity;
	mIsSelected = false;
	mEntity = parentNode->GetNxScene().GetNxSceneManager()->createEntity( MeshName );
	mEntity->setQueryFlags( NXENTITY );   


	//mEntity->setRenderQueueGroup(  Ogre::RENDER_QUEUE_8 );



	mNode = parentNode->GetNxScene().GetNxSceneManager()->createSceneNode( parentNode->GetName() + MeshName );
	mNode->attachObject( mEntity );
	mNxNodeParent = parentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );
 
	////Check Animations
	//AnimationStateSet * Set = mEntity->getAllAnimationStates();
	//if( Set ) {
	//	AnimationStateIterator iter =  Set->getAnimationStateIterator();
	//	while( iter.hasMoreElements() ) {
	//		mAnimationStates.push_back(iter.getNext());
	//	}

	//	if( mAnimationStates.size() > 0 ) {
	//		mAnimationStates.begin()->setEnabled(true);
	//		mAnimationStates.begin()->setLoop(true); 
	//	}
	//}

	


}

NxEntity::~NxEntity()
{

	if( mEntity ) {
		//Log("~NxScene3DObjectEntity() : Detach and Destroy Entity");
		Ogre::Entity * ent = ( Ogre::Entity * ) mNode->detachObject( mEntity->getName() );
		Ogre::MeshManager::getSingleton().unload(ent->getMesh()->getHandle());
		Ogre::MeshManager::getSingleton().remove(ent->getMesh()->getHandle());
		//Ogre::MeshManager::getSingleton().unloadUnreferencedResources();
		mScene->GetNxSceneManager()->destroyEntity( ent->getName() );	
	}

	if( mType == NxTypeEntity ) {
		Ogre::SceneNode * Parent =  mNode->getParentSceneNode();
		Parent->removeChild( mNode );
		mScene->GetNxSceneManager()->destroySceneNode( mNode );
	} 
}

bool NxEntity::SetLodDetails( std::vector<float> mLodValues, float reductionvalue )
{

	return false;
	/*
	Ogre::Mesh::LodValueList lodDList;
	for( int i = 0 ; i < mLodValues.size(); i++ ){
		lodDList.push_back(mLodValues[i]);
	}

	return Ogre::ProgressiveMesh::generateLodLevels( mEntity->getMesh().getPointer(),lodDList, Ogre::ProgressiveMesh::VRQ_PROPORTIONAL, reductionvalue );
	*/
	
}

bool NxEntity::SetNxActor( const std::string & ActorName )
{
	NxPhysicsActor * Actor = mNxNodeParent->GetNxScene().GetNxActor( ActorName );
	if( Actor ) {

		//Actor->SetUserData( this );
 
		 mNode->setPosition(  NxVec3ToOgre( Actor->GetPosition() ) );
		 mNode->setOrientation( NxQuatToOgre( Actor->GetOrientation() ) );
 
		//Ogre::Quaternion qParent = mNode->getParentSceneNode()->_getDerivedOrientation().Inverse();
		//Ogre::Vector3 vParent = mNode->getParentSceneNode()->_getDerivedPosition();
		//Ogre::Vector3 newPos = ( Actor->GetPosition() - vParent);
		//newPos /= mNode->getParentSceneNode()->_getDerivedScale();
		//Ogre::Vector3 pos = qParent * newPos;
		//mNode->setPosition( pos );

		//mNode->setOrientation( Actor->GetOrientation() );


		mActor = Actor;
	}
	else{ LogMsg("Didnt Find NxActor : " + ActorName ); }
	return true;
}

bool NxEntity::SetNxSound( const std::string & SoundName )
{
	mSound = mNxNodeParent->GetNxSound( SoundName );
	return true;
}

const NxType NxEntity::GetNxType()
{
	return mType;
}

void NxEntity::BuildTangentVectors()
{
	unsigned short src, dest;
	if (!mEntity->getMesh()->suggestTangentVectorBuildParams( VES_TANGENT, src, dest)) {
		mEntity->getMesh()->buildTangentVectors(VES_TANGENT, src, dest);		
	}
}

unsigned int NxEntity::GetTriangleCount()
{
	int iTriangleCount = 0;
	if (mEntity) {
		Ogre::MeshPtr lpMesh = mEntity->getMesh();
		int iSubMeshes = lpMesh->getNumSubMeshes();
		for (int iCurrentSubMesh = 0; iCurrentSubMesh < iSubMeshes; iCurrentSubMesh++) {
			Ogre::SubMesh* lpSubMesh = lpMesh->getSubMesh(iCurrentSubMesh);
			if (lpSubMesh) {
				Ogre::RenderOperation rend;
				lpSubMesh->_getRenderOperation(rend);

				int iIndexCount = lpSubMesh->indexData->indexCount;
				if (iIndexCount) {
					switch (rend.operationType) {
						case Ogre::RenderOperation::OT_TRIANGLE_LIST:
						{
							iTriangleCount += iIndexCount / 3;
						} break;

						case Ogre::RenderOperation::OT_TRIANGLE_STRIP:
						{
							iTriangleCount += iIndexCount - 2;
						} break;

						case Ogre::RenderOperation::OT_TRIANGLE_FAN:
						{
							iTriangleCount += iIndexCount - 2;
						} break;
					}

				}
			}
		}
	}

	return iTriangleCount;
}


const Ogre::Vector3 & NxEntity::GetBoundsSize() const
{
	return mEntity->getBoundingBox().getSize(); 
}

void NxEntity::SetBoundsInfinite()
{
	AxisAlignedBox aabBounds;
	aabBounds.setInfinite();	
	mEntity->getMesh()->_setBounds( aabBounds );
}

const std::string & NxEntity::GetName()
{
	return mNode->getName();
}

void NxEntity::SetCastShadows( bool CastShadows )
{
	mEntity->setCastShadows( CastShadows );
}

const std::string & NxEntity::GetMaterialName()
{
	return mEntity->getSubEntity(0)->getMaterialName().c_str();
}

void NxEntity::SetMaterialName( const std::string & MaterialName )
{
	mEntity->setMaterialName( MaterialName );
}

void NxEntity::SetVisible( bool Visible )
{
	mEntity->setVisible( Visible );
}

void NxEntity::SetDisplayDebug( bool Debug )
{
	mEntity->setDebugDisplayEnabled( Debug );
}

void NxEntity::SetSelected( bool Selected )
{
	mIsSelected = Selected;

	MeshPtr EntityMesh = mEntity->getMesh();
	if( EntityMesh.get() )
	{
		MaterialPtr Mat = MaterialManager::getSingleton().getByName( EntityMesh->getSubMesh(0)->getMaterialName() );
		if( mIsSelected )
		{
			Mat->setReceiveShadows(false);
			Mat->getTechnique(0)->setLightingEnabled(true);
			Mat->getTechnique(0)->getPass(0)->setDiffuse(1,0,0,0);
			Mat->getTechnique(0)->getPass(0)->setAmbient(1,0,0);
			Mat->getTechnique(0)->getPass(0)->setSelfIllumination(1,0,0); 	

			if( Mat->getTechnique(0)->getPass(0)->getNumTextureUnitStates() > 0 )
			{
				TextureUnitState * Texture_state_temp = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
				Texture_state_temp->setAlphaOperation( LBX_SOURCE1, LBS_MANUAL, LBS_CURRENT, 0.5f );
			}
		}
		else
		{
			Mat->setReceiveShadows(true);
			Mat->getTechnique(0)->setLightingEnabled(false);
			//Mat->getTechnique(0)->getPass(0)->setDiffuse(1,0,0,0);
			//Mat->getTechnique(0)->getPass(0)->setAmbient(1,0,0);
			//Mat->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,0); 

			if( Mat->getTechnique(0)->getPass(0)->getNumTextureUnitStates() > 0 )
			{
				TextureUnitState * Texture_state_temp = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
				Texture_state_temp->setAlphaOperation( LBX_SOURCE1, LBS_MANUAL, LBS_CURRENT, 1.0f );
			}
		}
	}
}


void NxEntity::SetVertexAnimation( const std::string & VertexAnimationName, bool Looped )
{
	AnimationStateSet * Set = mEntity->getAllAnimationStates();
	if( Set ) {
		AnimationStateIterator iter =  Set->getAnimationStateIterator();
		while( iter.hasMoreElements() ) {
			AnimationState * anim = iter.getNext();
			if( anim->getAnimationName().compare( Ogre::String( VertexAnimationName.c_str()  ) ) == 0 ) {
				anim->setTimePosition( 0 );
				anim->setEnabled(true);
				anim->setLoop( Looped ); 
			}
		 }
	}
}

void NxEntity::GetVertexAnimationNames( std::vector< std::string > & VertexAnimationNames )
{
	AnimationStateSet * Set = mEntity->getAllAnimationStates();
	if( Set ) {
		AnimationStateIterator iter =  Set->getAnimationStateIterator();
		while( iter.hasMoreElements() ) {
			AnimationState * anim = iter.getNext();
			VertexAnimationNames.push_back(anim->getAnimationName());
		}
	}
}

Entity * NxEntity::GetEntity()
{
	return mEntity;
}

Ogre::SceneNode * NxEntity::GetSceneNode()
{
	return mNode;
}

Ogre::SceneNode * NxEntity::GetParentSceneNode()
{
	return mNxNodeParent->GetNxSceneNode();
}

void NxEntity::AddChild( NxEntity * obj )
{
	mNode->addChild( obj->mNode );
}

void NxEntity::SetRenderQueueGroup( unsigned char renderGroup )
{
	mEntity->setRenderQueueGroup( renderGroup );
}








 /*
void NxScene3DObjectEntity::Translate( const Ogre::Vector3 & Position,  NxTransformSpace Space )
{
	mNode->translate( Position, (Ogre::Node::TransformSpace) Space  );	
}

void NxScene3DObjectEntity::Rotate( const Ogre::Quaternion & Orientation, NxTransformSpace Space )
{
	mNode->rotate( Orientation, (Ogre::Node::TransformSpace)Space );
}
*/

//NX

void NxEntity::Rotate( const Nx::Quaternion & Orientation, NxTransformSpace Space )
{
	Ogre::Quaternion quat;
	NxQuatToOgre( quat, Orientation );
	mNode->rotate( quat, (Ogre::Node::TransformSpace)Space );
}


/*
void NxScene3DObjectEntity::SetPosition( const Nx::Vector3 & Position )
{
	Ogre::Vector3 pos;
	NxVec3ToOgre( pos, Position );
	mNode->setPosition( pos );
}

void NxScene3DObjectEntity::SetOrientation( const Nx::Quaternion & Orientation ) 
{
	Ogre::Quaternion quat;
	NxQuatToOgre( quat, Orientation );
	mNode->setOrientation( quat );
}
*/



void NxEntity::SetPosition( const Nx::Vector3 & Position )
{
	Ogre::Vector3 pos;
	NxVec3ToOgre( pos, Position );
	mNode->setPosition( pos );

	/*
	switch( mPhysicProperty )
	{
		case PhysicGroup::GROUP_KINEMATIC :
		{
			mNode->setPosition( Position );
			Ogre::Vector3 FinalPos = mNode->_getDerivedPosition() * mNode->getScale();
			MoveToPosition( FinalPos );
			Ogre::Quaternion FinalOri = mNode->_getDerivedOrientation();
			SetEntityOrientation( FinalOri );
		}
		break;
		case PhysicGroup::GROUP_DYNAMIC :
		{
			PhysxActorsList[0]->raiseBodyFlag( NX_BF_KINEMATIC );
			mNode->setPosition( Position );
			Ogre::Vector3 FinalPos = mNode->_getDerivedPosition() * mNode->getScale();
			MoveToPosition( FinalPos );
			Ogre::Quaternion FinalOri = mNode->_getDerivedOrientation();
			SetEntityOrientation( FinalOri );
			PhysxActorsList[0]->clearBodyFlag( NX_BF_KINEMATIC );
		}

		break;
		case PhysicGroup::GROUP_STATIC :
		{
			mNode->setPosition( Position  );
			Ogre::Vector3 FinalPos = mNode->_getDerivedPosition() * mNode->getScale();
			MoveToPosition( FinalPos );
			Ogre::Quaternion FinalOri = mNode->_getDerivedOrientation();
			SetEntityOrientation( FinalOri );
		}
		break;
		case PhysicGroup::GROUP_NONE :
		{
			mNode->setPosition( Position );
		}
		break;
		default :
		break;
	}

	*/
}

const Nx::Vector3 & NxEntity::GetDerivatedPosition() const
{
	return Nx::Vector3(mNode->_getDerivedPosition()); 
}
 
const Nx::Vector3 & NxEntity::GetPosition() const
{
	return Nx::Vector3(mNode->getPosition());	
}

void NxEntity::SetOrientation( const Nx::Quaternion & Orientation )
{
	Ogre::Quaternion quat;
	NxQuatToOgre( quat, Orientation );
	mNode->setOrientation( quat );
}

const Nx::Quaternion & NxEntity::GetDerivatedOrientation() const
{
	return Nx::Quaternion( mNode->_getDerivedOrientation() ); 
}

const Nx::Quaternion & NxEntity::GetOrientation() const
{
	return Nx::Quaternion(mNode->getOrientation());
}

void NxEntity::SetScale( const Nx::Vector3 & Scale )
{
	Ogre::Vector3 sca;
	NxVec3ToOgre( sca, Scale );
	mNode->setScale( sca );
}

const Nx::Vector3 & NxEntity::GetDerivatedScale() const
{
	return  Nx::Vector3( mNode->_getDerivedScale() ); 
}

const Nx::Vector3 & NxEntity::GetScale() const
{
	return Nx::Vector3(mNode->getScale());
}

void NxEntity::SetInheritOrientation( bool inherit )
{
	mNode->setInheritOrientation( inherit );
}

void NxEntity::SetInheritScale( bool inherit )
{
	mNode->setInheritScale( inherit );
}

void NxEntity::SetInitialState()
{
	mNode->setInitialState();
}

void NxEntity::SetPivotTransform( const Nx::Vector3 & Position, const Nx::Quaternion & Rotation, const Nx::Vector3 & Scale )
{
	NxMeshManager::getSingleton().SetPivotTransform( mEntity->getMesh(), Position, Rotation, Scale );
}

void NxEntity::SetAutoTracking(bool enabled, NxNode* const target, const Nx::Vector3& localDirectionVector, const Nx::Vector3& offset ) {

	Ogre::Vector3 DirectionVector;
	NxVec3ToOgre( DirectionVector, localDirectionVector);

	Ogre::Vector3 off;
	NxVec3ToOgre( off, offset);

	mNode->setAutoTracking( enabled, target->GetNxSceneNode(), DirectionVector, off );
}

 
void NxEntity::Update( float time )
{
 
	if( mActor ) {
		Ogre::Quaternion qParent = mNode->getParentSceneNode()->_getDerivedOrientation().Inverse();
		Ogre::Vector3 vParent = mNode->getParentSceneNode()->_getDerivedPosition();
		Ogre::Vector3 newPos = ( NxVec3ToOgre( mActor->GetPosition() ) - vParent);
		newPos /= mNode->getParentSceneNode()->_getDerivedScale();
		Ogre::Vector3 pos = qParent * newPos;
		mNode->setPosition( pos );
		mNode->setOrientation(  NxQuatToOgre( mActor->GetOrientation() ) *  mNode->getParentSceneNode()->_getDerivedOrientation().Inverse() );//, Node::TS_WORLD);
	}

	if( mSound ) {
		mSound->SetPosition( OgreToNxVec3( mNode->_getDerivedPosition() ) );
	}

	AnimationStateSet * Set = mEntity->getAllAnimationStates();
	if( Set ) {
		AnimationStateIterator iter =  Set->getAnimationStateIterator();
		while( iter.hasMoreElements() ) {
			AnimationState * anim = iter.getNext();
			if(anim->getEnabled()) anim->addTime( time );
		}
	}  
 
}

void NxEntity::OnMouseMoved( const OIS::MouseEvent &e )
{

}

void NxEntity::OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{

}

void NxEntity::OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{

}

void NxEntity::OnKeyPressed( const NxKeyCode &e )
{

}

void NxEntity::OnKeyReleased( const NxKeyCode &e )
{

}

}//namespace