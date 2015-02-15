#include "NxGraphicsPreCompiled.h"
#include "NxMeshManager.h"
#include "NxConversions.h"

#ifdef __ANDROID__
#include <OgreGLES2Texture.h>
#include <android/log.h>
#define  LOG_TAG    "TEXTURE BASE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

namespace Nx {

NxTexture::NxTexture() : mHasAudio(false)
{

}

void NxTexture::create( const std::string & TexName, const std::string & textureGroupName, int Width, int Height, int numMips, NxTextureType type, NxPixelFormat format, NxTextureUsage usage ) {
 
	TexturePtr tex = TextureManager::getSingleton().createManual(
		TexName,
		textureGroupName, //ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		(Ogre::TextureType)type,
		Width,
		Height,
		numMips,
		(Ogre::PixelFormat)format,
		(Ogre::TextureUsage)usage ); 

	 mTexture.push_back( tex.get() );

	Ogre::HardwarePixelBufferSharedPtr pb = mTexture[0]->getBuffer();
	pb->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	const Ogre::PixelBox& pixelBox = pb->getCurrentLock();
	size_t dstBpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
	size_t dstPitch = pixelBox.rowPitch * dstBpp;
	Ogre::uint8* dstData = static_cast<Ogre::uint8*>(pixelBox.data);
	memset(dstData, 255, dstPitch * Height);
	pb->unlock();

}

NxTexture::~NxTexture() {
	LogMsg("~NxTexture() ");
}


void NxTexture::updateTexture( float time ) {
	mFrameBuffer->UpdateTexture( time );
}



unsigned int NxTexture::GetTextureID( int plane ) {

	unsigned int TextureID = -1;
	#ifdef __ANDROID__
		TextureID = ((GLES2Texture*)mTexture[plane])->getGLID();
		LOGD("---> ANDROID mTexture->getCustomAttribute ID : %d" ,  TextureID );
	#else
		mTexture[plane]->getCustomAttribute("GLID", &TextureID );
		LogMsg( "NxGraphics :  NxTexture::GetTextureID()  : " + Ogre::StringConverter::toString( TextureID ));
	#endif
	return TextureID;
}

unsigned int NxTexture::GetWidth( int plane ) {
	return mTexture[plane]->getWidth();
}

unsigned int NxTexture::GetHeight( int plane ) {
	return mTexture[plane]->getHeight();
}

unsigned long NxTexture::GetPixelSize( int plane ) {
	return Ogre::PixelUtil::getMemorySize(
		mTexture[plane]->getWidth(),
		mTexture[plane]->getHeight(),
		mTexture[plane]->getDepth() ,
		mTexture[plane]->getDesiredFormat()
		);
}

unsigned int NxTexture::GetBpp( int plane ) {
	return Ogre::PixelUtil::getNumElemBytes( mTexture[plane]->getDesiredFormat() );
}

 
size_t NxTexture::getNumPlanes() {
	return mTexture.size();
}


Ogre::HardwarePixelBuffer * NxTexture::GetPixelBuffer( int plane ) const {
	return mTexture[plane]->getBuffer(0,0).get();
}
 
const std::string & NxTexture::GetTextureName( int plane ) const {
	return mTexture[plane]->getName();
}

void NxTexture::SetMuted( bool muted ) {
	 mMuted = muted;
}

bool NxTexture::isLoaded( int plane ) const {
	return mTexture[plane]->isLoaded();
}
 
bool NxTexture::isLoading( int plane ) const {
	return mTexture[plane]->isLoading();
}
 
bool NxTexture::isManuallyLoaded( int plane ) const {
	return mTexture[plane]->isManuallyLoaded();
}
 
bool NxTexture::isPrepared( int plane ) const{
	return mTexture[plane]->isPrepared();
}
 
bool NxTexture::isReloadable( int plane ) const {
	return mTexture[plane]->isReloadable();
} 

void NxTexture::Prepare( int plane, bool BackgroundThread ) {
	mTexture[plane]->prepare( BackgroundThread );
}

void NxTexture::Load( int plane, bool BackgroundThread ) {
	mTexture[plane]->load( BackgroundThread );
	if (mTexture[plane]->isLoaded()) {
		LogMsg("Resource was loaded: " + mTexture[plane]->getName() );
	}else {
		LogMsg("Resource was Not loaded: " + mTexture[plane]->getName() );
	}

}

void NxTexture::Reload( int plane ) {
	if ( !mTexture[plane]->isReloadable() ){
		LogMsg("Texture is not reloadable." );
		return;
	}
	mTexture[plane]->reload();
	if (mTexture[plane]->isLoaded()) {
		LogMsg("Resource was Reloaded: " + mTexture[plane]->getName() );
	}else {
		LogMsg("Resource was Not Reloaded: " + mTexture[plane]->getName() );
	}
}

void NxTexture::Unload( int plane )
{
	mTexture[plane]->unload();
	if (mTexture[plane]->isLoaded()) {
		LogMsg("Resource was Not unloaded: " + mTexture[plane]->getName() );
	}else {
		LogMsg("Resource was unloaded: " + mTexture[plane]->getName() );
	}
}

void NxTexture::FreeInternalResources( int plane )
{
	mTexture[plane]->freeInternalResources();
}

void NxTexture::CreateInternalResources( int plane )
{
	mTexture[plane]->createInternalResources();
}


//------------------------------------------

NxLayer::NxLayer()
{
	NxLayer( NxEngine::getSingleton().GetNxWindow()->GetWidth(), NxEngine::getSingleton().GetNxWindow()->GetHeight() );

}

NxLayer::~NxLayer()
{

	LogMsg("Delete ~NxLayer()");
}

NxLayer::NxLayer( int Width, int Height ) : mWidth(Width), mHeight(Height)
{
 
	LogMsg("Creating NxLayer : NxLayer::NxLayer");

	Position = Nx::Vector3(0,0,0);
	Scale = Nx::Vector3(1,1,1);
	
	static int UniqueLayerID = 0 ;
	Ogre::MeshPtr mMesh = NxMeshManager::getSingleton().CreatePlane("3D_Mesh_Layer_"+ StringConverter::toString(UniqueLayerID),Nx::Vector2(Width,Height),Nx::Vector2(100,100) );
 
	NxEntity = NxEngine::getSingleton().GetContentManager()->GetContentManager()->createEntity("3D_Entity_Name_"+ StringConverter::toString(UniqueLayerID), "3D_Mesh_Layer_"+ StringConverter::toString(UniqueLayerID) );
	NxEntity->getSubEntity(0)->setUseIdentityView( false ); // true make it move with camera
	NxEntity->setRenderQueueGroup( Ogre::RENDER_QUEUE_MAIN + UniqueLayerID );//trick for compositor
	NxEntity->setVisible(true);

	NxPivot = NxEngine::getSingleton().GetContentManager()->GetContentManager()->getRootSceneNode()->createChildSceneNode("3D_SceneNode_QUAD_Name_"+ StringConverter::toString(UniqueLayerID));
	NxPivot->translate( Ogre::Vector3( 0, 0, 0 ), Ogre::Node::TS_WORLD );
	Ogre::Quaternion RotationX;
	RotationX.FromAngleAxis( Ogre::Degree( 90 ), Ogre::Vector3( 1, 0, 0 ) );
	Ogre::Quaternion RotationZ;
	RotationZ.FromAngleAxis( Ogre::Degree( 90 ), Ogre::Vector3( 0, 1, 0 ) );
	NxPivot->setOrientation( RotationX * RotationZ );
	NxPivot->setDebugDisplayEnabled(true);
	NxPivot->attachObject( NxEntity );


	NxSceneNode = NxEngine::getSingleton().GetContentManager()->GetContentManager()->createSceneNode( "3D_SceneNodeCHILD_Name_"+ StringConverter::toString(UniqueLayerID) );
	NxSceneNode->setDebugDisplayEnabled(true);
	NxSceneNode->translate( Ogre::Vector3( 0.0f, 0.0f, 0.0f ), Ogre::Node::TS_WORLD );

	NxMeshManager::getSingleton().CreateSphere( "PIVOT_Mesh_Layer_"+ StringConverter::toString(UniqueLayerID),10.0f );
	mPivotSphere = NxEngine::getSingleton().GetContentManager()->GetContentManager()->createEntity("PIVOT_Entity_Name_"+ StringConverter::toString(UniqueLayerID), "PIVOT_Mesh_Layer_"+ StringConverter::toString(UniqueLayerID) );
	mPivotSphere->setVisible( false );

	NxSceneNode->attachObject( mPivotSphere );


	if( NxPivot->getParent()){  NxPivot->getParent()->removeChild( NxPivot ); }
	NxSceneNode->addChild(  NxPivot );

	NxEngine::getSingleton().GetContentManager()->GetContentManager()->getRootSceneNode()->addChild( NxSceneNode );
	UniqueLayerID++;


	LogMsg("NxLayer Creating : Done");

}

void NxLayer::SetMaterialName( const std::string & MaterialName )
{
	NxEntity->setMaterialName( MaterialName ); 
}

void NxLayer::Set3DObjectType( Nx3DSceneObjectTypes Type )
{

	
	static int UniqueSphereLayerID = 0 ;

	float radius = 220.0f;
	Ogre::MeshPtr mMesh = NxMeshManager::getSingleton().CreateSphere("3D_Sphere_Layer_"+ StringConverter::toString(UniqueSphereLayerID), radius );

	Ogre::Entity * ent = NxEngine::getSingleton().GetContentManager()->GetContentManager()->createEntity("3D_Entity_SphereName"+ StringConverter::toString(UniqueSphereLayerID), mMesh->getName() );
	ent->getSubEntity(0)->setUseIdentityView( false ); // true make it move with camera
	ent->setMaterialName( NxEntity->getSubEntity(0)->getMaterialName() ); 

	ent->setRenderQueueGroup( NxEntity->getRenderQueueGroup() );//trick for compositor

	//NxEntity->setRenderQueueGroup( Ogre::RENDER_QUEUE_MAIN );//trick for compositor

	ent->setVisible(true);

	NxPivot->detachAllObjects();
 
	NxPivot->attachObject( ent );

	UniqueSphereLayerID++;




}

void NxLayer::SetPosition( Nx::Vector3 Position )
{

	Real engine_render_width =  NxEngine::getSingleton().GetNxWindow()->GetWidth() ;
	Real engine_render_height = NxEngine::getSingleton().GetNxWindow()->GetHeight() ;
	Real Ortho_Position_X =  engine_render_width * Position.x ;
	Real Ortho_Position_Y = engine_render_height * -Position.y ;
	NxSceneNode->setPosition( Ortho_Position_X, Ortho_Position_Y , Position.z );



 
	//Real engine_render_width =  NxGraphics::getSingleton().GetEngineManager()->GetWidth() ;
	//Real engine_render_height = NxGraphics::getSingleton().GetEngineManager()->GetHeight() ;

	//Real Ortho_Position_X = ( mWidth * ( engine_render_width / mWidth )) * Position.x ;
	//Real Ortho_Position_Y = ( mHeight* ( engine_render_height / mHeight )) * Position.y ;

	//Ortho_Position_X  = Ortho_Position_X +  (mWidth / 2.0f ) *   NxSceneNode->getScale().x  ; 
	//Ortho_Position_Y  = Ortho_Position_Y +  (mHeight / 2.0f ) *   NxSceneNode->getScale().y ; 

	// NxSceneNode->setPosition( Ortho_Position_X - (engine_render_width /2.0f) , -Ortho_Position_Y + (engine_render_height / 2.0f ) , 0.0f);//-2000.0f + (Real)layer ); // set the quad mesh position

	 //Position.x  = Position.x ;
	 //Position.y = Position.y ; 


}

Nx::Vector2 NxLayer::GetPosition()
{
	return Nx::Vector2(  Position.x,  Position.y );
}

void NxLayer::SetScale( Nx::Vector2 Scale )
{

	NxPivot->setScale( Scale.x , Scale.y  ,1.0f); 


	return ;

	//Real engine_render_width =  NxGraphics::getSingleton().GetEngineManager()->GetWidth() ;
	//Real engine_render_height = NxGraphics::getSingleton().GetEngineManager()->GetHeight() ;
	//Real Base_Scale_X = ( engine_render_width / mWidth ) * Scale.x ;
	//Real Base_Scale_Y = ( engine_render_height/ mHeight) * Scale.y ;
	//NxSceneNode->setScale( Base_Scale_X , Base_Scale_Y  ,1.0f);  
	//SetPosition( Ogre::Vector2(  Position.x ,  Position.y ) );
	//Scale.x = Scale.x;
	//Scale.y = Scale.y; 
}

Nx::Vector2 NxLayer::GetScale()
{
	return Nx::Vector2(  Scale.x,  Scale.y );
}

void NxLayer::SetRotation( Nx::Quaternion Rotation, NxTransformSpace TrSpace )
{
	NxSceneNode->rotate( NxQuatToOgre( Rotation ), (Ogre::Node::TransformSpace)TrSpace );

	//MixerManager::getSingleton().Set_Rotation_3D_X( Rotation.x, mLayer.LayerNumber );
	//MixerManager::getSingleton().Set_Rotation_3D_Y( Rotation.y, mLayer.LayerNumber );
	//MixerManager::getSingleton().Set_Rotation_3D_Z( Rotation.z, mLayer.LayerNumber );
}

Nx::Vector3 NxLayer::GetRotation()
{
	return Rotation;
}

void NxLayer::SetRenderZorder( int RenderingOrder )
{
	 NxEntity->setRenderQueueGroup( Ogre::RENDER_QUEUE_MAIN + RenderingOrder );// pour que le compositor marche
	  
}

void NxLayer::SetVisible( bool Visible )
{
	//mPivotSphere->setVisible( Visible );
	NxEntity->setVisible( Visible );
	//MixerManager::getSingleton().SetLayerMuted( mLayer.LayerNumber, Mute );
}

//bool NxLayer::GetMute()
//{
//	return  mMuted;
//}
//
//int NxLayer::GetIndex() const 
//{
//	return  LayerNumber;
//}

}