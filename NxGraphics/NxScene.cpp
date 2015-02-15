#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "../nxsound/NxSound_Headers.h"
 
 
namespace Nx {

class ColouredRectangle2D : public Ogre::Rectangle2D
{
public:
	 ColouredRectangle2D(bool includeTextureCoordinates /*= false*/):Ogre::Rectangle2D(includeTextureCoordinates)
	{
		int COLOUR_BINDING = 3;
		Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;

		decl->addElement( COLOUR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

		Ogre::HardwareVertexBufferSharedPtr vbuf =  Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(COLOUR_BINDING), mRenderOp.vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		// Bind buffer
		bind->setBinding(COLOUR_BINDING, vbuf);
	}

	 ~ColouredRectangle2D()
	{
	}

	void  setColours(const Ogre::ColourValue &topLeft, const Ogre::ColourValue &bottomLeft, const Ogre::ColourValue &topRight, const Ogre::ColourValue &bottomRight)
	{
		Ogre::HardwareVertexBufferSharedPtr vbuf =  mRenderOp.vertexData->vertexBufferBinding->getBuffer( 3 );
		unsigned int* pUint32 = static_cast<unsigned int*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		const Ogre::VertexElementType srcType = Ogre::VertexElement::getBestColourVertexElementType();
		*pUint32++ = Ogre::VertexElement::convertColourValue( topLeft, srcType );
		*pUint32++ = Ogre::VertexElement::convertColourValue( bottomLeft, srcType );
		*pUint32++ = Ogre::VertexElement::convertColourValue( topRight, srcType );
		*pUint32++ = Ogre::VertexElement::convertColourValue( bottomRight, srcType );
		vbuf->unlock();
	}

};

Nx3D_Scene::Nx3D_Scene( const Nx3DSceneDesc & SceneDesc ) : MouseActive(true), KeyboardActive(true)
{
	//pathRoot = NxGraphicsUtilities::GetCurrentWorkingDirectory();
	
	//NxInputManager::getSingleton().AddListener( this );

	mRtt = 0;
	mWindow = 0;
}

Nx3D_Scene::~Nx3D_Scene()
{
	//NxInputManager::getSingleton().RemoveListener( this );
}

void Nx3D_Scene::SetResourceGroupName( const std::string & ResourceName )
{
	ResourceGroupName = ResourceName ;
}
const std::string & Nx3D_Scene::GetResourceGroupName() const  
{
	return ResourceGroupName;
}

void Nx3D_Scene::AddResourceLocation( const std::string & ResourceLocation, const std::string & ResourceGroup, bool Recursive )
{
	if( !Ogre::ResourceGroupManager::getSingleton().resourceGroupExists( ResourceGroup ) ) {
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup( ResourceGroup );
	}
	else {
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup( ResourceGroup );
	}
	Ogre::Root::getSingleton().addResourceLocation( ResourceLocation, "FileSystem", ResourceGroup, Recursive );
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup( ResourceGroup );
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup( ResourceGroup, true, true );
}

void Nx3D_Scene::SetAmbientLight( const NxColourValue Colour )
{
	mSceneManager->setAmbientLight( NxColourtoOgre( Colour ) );
}

const NxColourValue Nx3D_Scene::GetAmbientLight()
{
	return OgretoNxColour( mSceneManager->getAmbientLight());
}

Ogre::SceneNode * Nx3D_Scene::GetRootSceneNode()
{
	return mSceneManager->getRootSceneNode();
}

NxNode * Nx3D_Scene::CreateNxNode( const std::string & NodeName )
{
	NxNode * c = new NxNode( this, NodeName );
	mNxNodes.insert( NxNodeList::value_type( NodeName, c ) );
	return c; 
}

void Nx3D_Scene::DeleteNxNode( const std::string & NodeName )
{
	NxNodeList::iterator i = mNxNodes.find(NodeName);
	if (i == mNxNodes.end()) {
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "NxNode '" + NodeName + "' not found.", "Nx3D_Scene::DeleteNxNode");
	}

	delete i->second;
	mNxNodes.erase(i);
}

NxNodeMapIterator Nx3D_Scene::GetNxNodeIterator()
{
	return NxNodeMapIterator( mNxNodes.begin(), mNxNodes.end() );
}

NxConstNodeMapIterator Nx3D_Scene::GetNxNodeConstIterator()
{
	return NxConstNodeMapIterator( mNxNodes.begin(), mNxNodes.end() );
}

NxNode * Nx3D_Scene::GetNxNode( const std::string & NodeName ) const
{
    NxNodeList::const_iterator i = mNxNodes.find( NodeName );
    if(i == mNxNodes.end()){
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND,  "Cannot find NxNode with name " + NodeName, "Nx3D_Scene::GetNxNode");
    }
    else {
        return i->second;
    }
}

bool Nx3D_Scene::HasNxNode(const std::string & NodeName ) const
{
	NxNodeList::const_iterator i = mNxNodes.find( NodeName );
	if(i == mNxNodes.end()){
		return false;
	}
	return true;
}

void Nx3D_Scene::DeleteNxNodes()
{
	NxNodeList::iterator i = mNxNodes.begin();
	while(i != mNxNodes.end()) {
		delete i->second;
		i++;
	}
	mNxNodes.clear();
}

NxScenePhysics * Nx3D_Scene::GetPhysicScene()
{
	return mScenePhysics;
}

bool Nx3D_Scene::UpdatePhysics( float Time )
{
	return mScenePhysics->UpdatePhysics(Time) ;
}

NxPhysicsActor * Nx3D_Scene::GetNxActor( const std::string & ActorName )
{
	NxPhysicsActor * Res = NULL;
	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ) {
		NxNode * It = Iter.getNext();
		//Log("----- FOUND SCENENODE : " + It->GetName()  );
		NxActorListIterator ItActor = It->GetNxActorIterator();
		while( ItActor.hasMoreElements() ) {
			NxPhysicsActor * Act = ItActor.getNext();
			if( !Act->GetName().empty() ) {
				if( Act->GetName().compare( ActorName ) == 0 ) {
					//Log("FOUND ACTOR");
					Res = Act;
					break;
				}
			}
		}
	}

	return Res;
}


const NxGraphics_3D_Scene_Types Nx3D_Scene::GetSceneType()
{
	return mLayerType;
}

void Nx3D_Scene::SetFog( NxFogMode Mode, NxColourValue Color, float Intensity )
{	
	mSceneManager->setFog( (Ogre::FogMode)Mode , NxColourtoOgre( Color ), Intensity );
}

SceneManager * Nx3D_Scene::GetNxSceneManager()
{
	return mSceneManager;
}

Viewport * Nx3D_Scene::GetNxViewport()
{
	return mViewport  ;
}

RenderTexture * Nx3D_Scene::GetNxRenderTexture()
{
	return mRtt;
}

void Nx3D_Scene::SetViewportColour( const NxColourValue nxColor )
{
	mViewport->setBackgroundColour( Ogre::ColourValue( nxColor.r, nxColor.g, nxColor.b, nxColor.a ) );
}

void Nx3D_Scene::CreateViewportBackground( const std::string & materialname ) {

	LogMsg("Nx3D_Scene::CreateViewportBackground ...");
	// Create background rectangle covering the whole screen
	Ogre::Rectangle2D * rect = new Ogre::Rectangle2D(true);//new ColouredRectangle2D( false );
	rect->setCorners(-1.0, 1.0, 1.0, -1.0);

	LogMsg("Nx3D_Scene::CreateViewportBackground setmaterial");
	rect->setMaterial(materialname);
	// Render the background before everything else
	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);


	 

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	//rect->setBoundingBox(Ogre::AxisAlignedBox(-100000.0f * Ogre::Vector3::UNIT_SCALE, 100000.0f * Ogre::Vector3::UNIT_SCALE));

	// Attach background to the scene
			LogMsg("Nx3D_Scene::CreateViewportBackground create scene node");
	SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode("Background"+Ogre::StringConverter::toString( rand() % 255  ));
			LogMsg("Nx3D_Scene::CreateViewportBackground attach object");
	node->attachObject(rect);
			LogMsg("Nx3D_Scene::CreateViewportBackground DONE");


}

void Nx3D_Scene::SetViewportColours( const NxColourValue &topLeft, const NxColourValue &bottomLeft, const NxColourValue &topRight, const NxColourValue &bottomRight)
{
	// Create background material
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "General");
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	ColouredRectangle2D* rect = new ColouredRectangle2D( false );
	rect->setCorners(-1.0, 1.0, 1.0, -1.0);
	rect->setMaterial("Background");

	// Set the colours

	Ogre::ColourValue tleft;
	NxColourtoOgre(tleft,topLeft);

	Ogre::ColourValue bleft;
	NxColourtoOgre(bleft,bottomLeft);

	Ogre::ColourValue tright;
	NxColourtoOgre(tright,topRight);

	Ogre::ColourValue bright;
	NxColourtoOgre(bright,bottomRight);


	rect->setColours( tleft, bleft , tright, bright ); 

	// Render the background before everything else
	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	// Attach background to the scene
	SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode("Background"+Ogre::StringConverter::toString( rand() % 255) );
	node->attachObject(rect);

}

const NxColourValue Nx3D_Scene::GetViewportColour() const
{
	return OgretoNxColour( mViewport->getBackgroundColour() ); 
}

void Nx3D_Scene::SetAccumulationBuffer( bool Accumulation )
{
	Accumulation ? mViewport->setClearEveryFrame( true, FBT_DEPTH ) :  mViewport->setClearEveryFrame( true );  	 
}

void Nx3D_Scene::SetSkyBox( bool Enable, const std::string & SkyBoxMaterialName )
{
	mSceneManager->setSkyBox( Enable, SkyBoxMaterialName, 500 );

	/*
	Ogre::Image img;
	img.load( TextureName , ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	TexturePtr Video_Texture = TextureManager::getSingleton().createManual( "myname"   ,
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME  ,
	TEX_TYPE_CUBE_MAP , img.getWidth(), img.getHeight(), img.getDepth(), Ogre::PixelFormat::PF_A8R8G8B8);
	Video_Texture->loadImage( img );
	MaterialPtr Mat = MaterialManager::getSingleton().create( SkyBoxMaterialName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	Ogre::Pass * Pass =  Mat->getTechnique(0)->getPass(0) ;
	Pass->setLightingEnabled(false);
	Ogre::TextureUnitState * State =  Pass->createTextureUnitState();
	State->setCubicTextureName( Video_Texture->getName() , true );
	State->setTextureAddressingMode ( Ogre::TextureUnitState::TextureAddressingMode::TAM_CLAMP );
	State->setEnvironmentMap(true,Ogre::TextureUnitState::EnvMapType::ENV_REFLECTION );
	External_Scene_Manager->setSkyDome(true,SkyBoxMaterialName);
	*/
}

 

void Nx3D_Scene::UpdateNodeAnimations( float Time )
{
	for( int i = 0 ; i < NodeAnimationList.size(); i++ ) {
		if( NodeAnimationList[i]->getEnabled() )
		NodeAnimationList[i]->addTime( Time );
	}
}

void Nx3D_Scene::SetMouseActive( bool Active )
{
	MouseActive = Active ;
}

bool Nx3D_Scene::GetMouseActive()
{
	return MouseActive ;
}

void Nx3D_Scene::SetKeyboardActive( bool Active )
{
	KeyboardActive = Active ;
}

bool Nx3D_Scene::GetKeyboardActive()
{
	return KeyboardActive;
}

void Nx3D_Scene::GetNxCharacterList( std::vector< std::string > & List )
{
	FileInfoListPtr Ptr = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo( "NxCharacters" , false );

	for( Ogre::FileInfoList::iterator it = Ptr->begin();it != Ptr->end(); ++it)  {
		Ogre::FileInfo list = *it;
		Ogre::String Base ;
		Ogre::String Ext ;
		Ogre::StringUtil::splitBaseFilename( list.basename, Base, Ext );
		Ogre::StringUtil::toLowerCase( Ext );
		
		if( Ext.compare("mesh") == 0 ) {
			List.push_back(  list.filename.c_str() );
			LogMsg(" Adding : " +  list.filename  );
		}
		 
		LogMsg("Found Resource in NxCharacters : " + Base + "  Ext : " + Ext );
	}
}

NxScene3DObjectCharacter * Nx3D_Scene::CreateNxCharacter( const std::string & CharacterName )
{
	/*
	static int index = 0;
	NxScene3DObject * node = new NxScene3DObject( this, CharacterName +  "_SceneNode_" + Ogre::StringConverter::toString(index++) );
	NxScene3DObjectCharacter * Character = new NxScene3DObjectCharacter( node );
	Character->SwitchNxCharacter( CharacterName );
	Character->SetRagdollVisible( false );
	node->AttachObject( Character );
	return Character;
	*/

	return 0;
}

void Nx3D_Scene::DeleteNxCharacter( NxScene3DObjectCharacter * Character )
{

}

void Nx3D_Scene::SetCharacterView( NxScene3DObjectCharacter * Character, NavigationMode ViewType )
{
	/*
	NxCameraMapIterator Iter = GetNxCameraIterator();
	while( Iter.hasMoreElements() )
	{
		Iter.getNext()->SetActive( false );
	}
	//! Set character node
	Character->GetNxCamera()->SetNxCharacter( Character );
	//! Set camera view
	Character->GetNxCamera()->SetNavigationMode( ViewType );
	//! set bundle active
	Character->GetNxCamera()->SetActive( true );
	*/

	 
}


void Nx3D_Scene::UpdateSoundListener()
{
	//Vector3 curpos = GetCameraActive()->GetCameraFps()->getDerivedPosition();
	//NxSoundManager::getSingleton().SetListenerPosition( curpos.x, curpos.y, curpos.z );
	//Vector3 vDirection = GetCameraActive()->GetCameraFps()->getDerivedOrientation() * -Vector3::UNIT_Z; 
	//NxSoundManager::getSingleton().SetListenerOrientation( vDirection.x, vDirection.y, vDirection.z );
}

NxSceneTools * Nx3D_Scene::GetEditorTools()
{
	return mSceneTools;
}

NxScene3DObjectTerrain * Nx3D_Scene::CreateTerrain( const NxTerrainDesc & TerrainDesc )
{
	NxScene3DObjectTerrain * c = new NxScene3DObjectTerrain( this, TerrainDesc );
	mTerrains.push_back( c );
	return c;
}

bool Nx3D_Scene::frameStarted( const NxFrameEvent & evt )
{
	return true;
}

bool Nx3D_Scene::frameEnded( const NxFrameEvent & evt )
{
	return true ;
}

bool Nx3D_Scene::frameRenderingQueued( const NxFrameEvent & evt )
{
	UpdateNodeAnimations( evt.timeSinceLastFrame );//update all Node animations.
	UpdatePhysics( evt.timeSinceLastFrame );

	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ) {

		NxNode * Node = Iter.getNext();
		Node->Update( evt.timeSinceLastFrame );

		 if( !Node->IsValid() ) {
		 	mNxNodes.erase( mNxNodes.find( Node->GetName() ) );
		 	delete Node;
		 }
	}

	for( int i = 0 ; i < mTerrains.size(); i++ ){
		mTerrains[i]->Update( evt.timeSinceLastFrame );
	}

	mScenePhysics->DrawDebug(); 

	return true;
}

void Nx3D_Scene::OnMouseMoved( const OIS::MouseEvent &e )
{
	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ){
		Iter.getNext()->OnMouseMoved( e );
	}

	mSceneTools->OnKeyMouseMoved( e );
}

void Nx3D_Scene::OnMouseButtonPressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ){
		Iter.getNext()->OnMousePressed( e, id ); 
	}

	mSceneTools->OnKeyMousePressed( e, id );
}

void Nx3D_Scene::OnMouseButtonReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ){
		Iter.getNext()->OnMouseReleased( e, id );
	}

	mSceneTools->OnKeyMouseReleased( e, id );
}

void Nx3D_Scene::OnKeyPressed( const NxKeyCode &e )
{
	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ){
		Iter.getNext()->OnKeyPressed( e ); 
	}
}

void Nx3D_Scene::OnKeyReleased( const NxKeyCode &e )
{
	NxNodeMapIterator Iter = GetNxNodeIterator();
	while( Iter.hasMoreElements() ){
		Iter.getNext()->OnKeyReleased( e );  
	}
}

void Nx3D_Scene::OnJoystickButtonPressed( const OIS::JoyStickEvent &arg, int button )
{

}

void Nx3D_Scene::OnJoystickButtonReleased( const OIS::JoyStickEvent &arg, int button )
{

}

void Nx3D_Scene::OnJoystickAxisMoved( const OIS::JoyStickEvent &arg, int axis )
{

}

void Nx3D_Scene::AddListener( Nx3DSceneListener * listener )
{
	ListenersCallbackList.push_back( listener );
}

void Nx3D_Scene::RemoveListener( Nx3DSceneListener * listener )
{
	ListenersCallbackList.remove( listener );
}

 

} //NxGraphics_Namespace 



