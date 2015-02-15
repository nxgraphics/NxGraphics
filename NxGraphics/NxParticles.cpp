#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"

namespace Nx {

Nx3DObjectPhysicFluid::Nx3DObjectPhysicFluid( NxNode * parentNode, unsigned int NumParticles, bool UseVertexColor ) : NxEntity(), mNumParticles( NumParticles), mFluids(0) 
{
	mType = NxTypeFluid;
	Nx3D_Scene * CurrentScene = &parentNode->GetNxScene(); 
	mFluidObject = new NxParticles( Ogre::RenderOperation::OT_POINT_LIST, UseVertexColor );
 
	mNode = CurrentScene->GetNxSceneManager()->createSceneNode(  );//"NxParticlesNode" + Ogre::StringConverter::toString( instance_count() )
 
	mNode->attachObject( mFluidObject );

	mNxNodeParent = parentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );

	mFluidObject->setBoundingBox( Ogre::AxisAlignedBox( -1000, -1000, -1000, 1000, 1000, 1000 ) );
	mFluidObject->setMaterial( "PointTexture" );	

}

Nx3DObjectPhysicFluid::~Nx3DObjectPhysicFluid()
{

	//if( mFluids )
	//{
	
	//}


	mFluids = 0;
	LogMsg("Erasing Nx3DObjectPhysicFluid");

	mNxNodeParent->GetNxSceneNode()->removeChild( mNode );

	NxParticles * Particles = ( NxParticles * ) mNode->detachObject( mFluidObject->getName() );
	delete Particles;


	LogMsg("Deleting Node : "+ mNode->getName());
 
	mNxNodeParent->GetNxScene().GetNxSceneManager()->destroySceneNode( mNode );
	LogMsg("~Nx3DObjectPhysicFluid() DONE ");
 

}

//auto update
void Nx3DObjectPhysicFluid::Update( float time )
{
	if( mFluids ) { // physx object linked
		mFluidObject->Draw( mFluids->GetParticlePositions(), 0, mNumParticles );
	}
}

// manual update //todo setpositions puis mFluidObject->update
void Nx3DObjectPhysicFluid::Update( float time, const float * Positions, const unsigned int * Colors, unsigned long NumParticles )
{
	mFluidObject->Draw( Positions, Colors, NumParticles );
}

 
void Nx3DObjectPhysicFluid::SetVisible( bool Visible )
{
	mFluidObject->setVisible( Visible );
}

void Nx3DObjectPhysicFluid::SetParticlesSize( float SizeX, float SizeY )
{
	Ogre::MaterialPtr Mat =  mFluidObject->getMaterial(); 
	//Mat->getTechnique(0)->getPass(0)->setPointSize( SizeX );
}

void Nx3DObjectPhysicFluid::SetMaterialName( std::string MaterialName )
{
	mFluidObject->setMaterial( MaterialName );	 
}

void Nx3DObjectPhysicFluid::SetNxFluids( NxPhysicsFluid * Fluid )
{
	mFluids = Fluid;
}

	



/*







void Nx3DObjectPhysicFluid::SetOffsetPosition( float X, float Y, float Z ){
	mEmitter->setLocalPosition( NxVec3( X, Y, Z) );	
}

void Nx3DObjectPhysicFluid::SetManual( bool Manual ){
	mManual = Manual;
}

void Nx3DObjectPhysicFluid::Update( float time ){

	
	//if( mFluid && !mManual ){
	//	mFluidObject->DrawFluids( mFluid->GetPositions(), 0 , mFluid->getParticlesNum() );
	//}

	//if( mEmitter ){
	//	if( GetPhysxActor()->readBodyFlag( NxBodyFlag::NX_BF_KINEMATIC ) ){
	//		//GetPhysxActor()->setGlobalPosition( NxVec3( NxSceneNode->getPosition().x, NxSceneNode->getPosition().y, NxSceneNode->getPosition().z ) );
	//		mEmitter->setGlobalPosition( NxVec3( mSceneNode->getPosition().x, mSceneNode->getPosition().y, mSceneNode->getPosition().z ));
	//	}
	//	else{
	//		//NxSceneNode->setPosition( VecToOgre( GetPhysxActor()->getGlobalPosition() ) );
	//		mSceneNode->setPosition( VecToOgre( mEmitter->getGlobalPosition() ) );
	//	}
	//}
	
}

void Nx3DObjectPhysicFluid::SetPosition( Ogre::Vector3 Position )
{
	 
	//NxVec3 FinalPosNx = NxVec3( Position.x, Position.y, Position.z );
	//mEmitter->setGlobalPosition( FinalPosNx );
	//mSceneNode->setPosition( VecToOgre( GetPhysxActor()->getGlobalPosition() ));
 
}

Ogre::Vector3 Nx3DObjectPhysicFluid::GetPosition()
{
	return Ogre::Vector3(0,0,0);
}

void Nx3DObjectPhysicFluid::SetOrientation( Ogre::Quaternion Orientation ){

	Matrix3 m;
	m.FromEulerAnglesXYZ( (Radian) Ogre::Math::DegreesToRadians( -90.0f ),(Radian) Ogre::Math::DegreesToRadians( 0.0f ),(Radian) Ogre::Math::DegreesToRadians( 0.0f )  );
	Quaternion tmp( m ) ;

	Ogre::Quaternion q = Orientation * tmp ;
	NxQuat v;
	v.setWXYZ(q.w, q.x, q.y, q.z);
	mEmitter->setGlobalOrientation( v );
}

Ogre::Quaternion Nx3DObjectPhysicFluid::GetOrientation(){
	return Ogre::Quaternion();
}

unsigned int Nx3DObjectPhysicFluid::GetNumParticles(){ 
	return mEmitter->getMaxParticles();
}
*/

}//namespace