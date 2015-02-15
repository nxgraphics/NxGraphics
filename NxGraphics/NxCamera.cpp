#include "NxGraphicsPreCompiled.h"

namespace Nx {


	 
NxCamera::NxCamera( NxContentManager * SceneMgr, const std::string & CameraName )
//NxCamera::NxCamera( NxScene3DObject * ParentNode, const std::string & CameraName ) :  NxScene3DObjectEntity()
{

	mCamera = SceneMgr->GetContentManager()->createCamera( CameraName );
	mCamera->setPosition( 0, 0, 0 );
	mCamera->setNearClipDistance( 0.1f );
	mCamera->setFarClipDistance ( 4096.0f ); 

	/*
	mCamera = ParentNode->GetNxScene().GetNxSceneManager()->createCamera( CameraName );
	mCamera->setPosition( 0, 0, 0 );
	mCamera->setNearClipDistance( 0.1f );
	mCamera->setFarClipDistance ( 4096.0f ); 

	mNode = ParentNode->GetNxScene().GetNxSceneManager()->createSceneNode( CameraName );
	mNode->attachObject( mCamera );

	mNxNodeParent = ParentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );*/

}

NxCamera::~NxCamera()
{

}

void NxCamera::LookAt( const Ogre::Vector3 & Position )
{
	mCamera->lookAt( Position );
}

void NxCamera::Roll( float AngleDegree )
{
	mCamera->roll( Ogre::Degree( AngleDegree ) );
}

void NxCamera::SetAspectRatio( float Ratio )
{
	mCamera->setAspectRatio( Ratio );
}

void NxCamera::SetFOVy( float degrees )
{
	mCamera->setFOVy( Ogre::Degree( degrees )  );
}

void NxCamera::SetPolygonMode( NxPolygonMode Mode )
{
	mCamera->setPolygonMode( (Ogre::PolygonMode)Mode );
}

Ogre::Vector3 NxCamera::GetDirection() const
{
	return mCamera->getDirection();
}

Ogre::Vector3 NxCamera::GetRight() const
{
	return mCamera->getRight();
}

void NxCamera::Yaw( float AngleDegrees )
{
	mCamera->yaw( Ogre::Degree( AngleDegrees ) );
}  

void NxCamera::Pitch( float AngleDegrees )
{
	mCamera->pitch( Ogre::Degree( AngleDegrees ) );
} 

void NxCamera::SetNearClipDistance( float neardist ){
	mCamera->setNearClipDistance(neardist);
}

void NxCamera::SetFarClipDistance( float fardist ){
	mCamera->setFarClipDistance(fardist);
}

void NxCamera::SetPosition( float x, float y, float z ){
	mCamera->setPosition(x,y,z);
}

Ogre::Camera * NxCamera::GetCamera()
{
	return mCamera;
}

 
}