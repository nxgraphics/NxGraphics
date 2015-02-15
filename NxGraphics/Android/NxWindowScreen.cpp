#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

NxScreen::NxScreen( int MonitorID, bool Fullscreen, unsigned int Width, unsigned int Height ) : mRenderWindow(0), mOpacity(1.0f)
{
	LogMsg("NxScreen initialization...");
	unsigned int  i = rand() % 60000 ; 
	Ogre::Camera * Parent_Camera = NxEngine::getSingleton().GetNxCamera()->GetCamera();

	mMonitorInfo = NxScreenManager::getSingleton().GetMonitor( MonitorID );
	LogMsg( "Creating NxOutput for " +  std::string( mMonitorInfo->GetDeviceName() )  );

	if( !mMonitorInfo->IsDisabled() )
	{ 
		mWindowHandle = NxScreenManager::getSingleton().CreateWindowHolder( mMonitorInfo, Fullscreen );
		ScreenPosX = mMonitorInfo->GetPositionX() ;

		//if( 0 == ::SetWindowPos( (HWND)mWindowHandle, 0, mMonitorInfo->GetPositionX(), mMonitorInfo->GetPositionY(), Fullscreen ? mMonitorInfo->GetWidth() : Width, Fullscreen ? mMonitorInfo->GetHeight() : Height, SWP_FRAMECHANGED | SWP_NOACTIVATE ) )
		//{
		//	Log("SetWindowPos Failed");
		//}

		//ShowWindow( (HWND)mWindowHandle , SW_SHOW );

		Ogre::NameValuePairList  options ;
		options["title"] = "Ogre_Window_" + Ogre::StringConverter::toString(i);
		options["externalWindowHandle"] = Ogre::StringConverter::toString( reinterpret_cast<unsigned int>( mWindowHandle ) );
		options["vsync"] = Ogre::StringConverter::toString( true );      
		options["FSAA"] = "16";
		options["FSAAQuality"] = "16";
		options["displayFrequency"] = Ogre::StringConverter::toString( mMonitorInfo->GetRefreshRate() );
		options["colourDepth"] = Ogre::StringConverter::toString( mMonitorInfo->GetBpp() );
		options["monitorIndex"] = StringConverter::toString( MonitorID );
		options["externalGLControl"] = Ogre::StringConverter::toString( (false) );

		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		Fullscreen = true ;

		mRenderWindow = Root::getSingleton().createRenderWindow("Window_"+Ogre::StringConverter::toString(i), Fullscreen ? mMonitorInfo->GetWidth() : Width, Fullscreen ? mMonitorInfo->GetHeight() : Height, Fullscreen, &options );
		
		mRenderWindow->setDeactivateOnFocusChange( false ); 
		mSceneManager = Root::getSingleton().createSceneManager( ST_GENERIC, "Scene_Managers_" + Ogre::StringConverter::toString(i) ); 

		/*  render to texture output */
		//Ogre::Compositor * FullCompositor =  0 ;
		//Ogre::LogManager::getSingleton().logMessage(" Preparing quad material");
		//// create our material with compositor texture
		//VideoOutputOgreMaterial  = MaterialManager::getSingleton().create( "RTT_Material_"+Ogre::StringConverter::toString( i  ), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		// 
		//Ogre::CompositorChain * chain =   Ogre::CompositorManager::getSingleton().getCompositorChain(  MixerManager::getSingleton().Get_Preview_RenderWindow()->getViewport(0)  );
		//size_t numcomp = chain->getNumCompositors() ;
		//CompositorInstance * inst ;
		//for(  int  g = 0 ; g < numcomp ; g++ )
		//{
		//	inst =   chain->getCompositor( g ) ;
		//	//Ogre::LogManager::getSingleton().logMessage( inst->getCompositor()->getName() );
		//	if(  inst->getCompositor()->getName() == "NxCompositorOutput" )
		//	{
		//		FullCompositor = inst->getCompositor() ;
		//		Ogre::LogManager::getSingleton().logMessage("--->Found Output Compositor");
		//		break ;
		//	}
		//}

		//if(!FullCompositor){ Log("Didnt find compositor !"); }

		//Ogre::LogManager::getSingleton().logMessage( "Selected compositor : "  + inst->getCompositor()->getName() );
		//CompositionTechnique * tec = inst->getTechnique() ; 
		//CompositionTechnique::TextureDefinition * def =  NULL;
		//TexturePtr pointer ;
		//def = tec->getTextureDefinition("scene"); 

		//if( def )
		//{
		//	pointer =  inst->getTextureInstance( def->name , 0 );
		//	if( pointer.isNull() )
		//	{
		//		Ogre::LogManager::getSingleton().logMessage("---> Could not find Compositor texture instance, did you enable the compositor ? for texture definition : " + def->name  );
		//		 
		//		return ;
		//	}
		//	Ogre::LogManager::getSingleton().logMessage("---> Found Output Compositor texture instance "  );
		//}else
		//{
		//	Ogre::LogManager::getSingleton().logMessage("ERROR Output Compositor texture instance");
		//}

		//Ogre::LogManager::getSingleton().logMessage("-----> Found Output Compositor texture name :  " + pointer->getName()  );

		//VideoOutputOgreTextureUnitState =  NULL ;
		// VideoOutputOgreTextureUnitState = VideoOutputOgreMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();// pointer->getName() ); 
		// VideoOutputOgreTextureUnitState->setContentType(Ogre::TextureUnitState::CONTENT_NAMED );//CONTENT_COMPOSITOR);
		// VideoOutputOgreTextureUnitState->setTextureName( pointer->getName() , TextureType::TEX_TYPE_2D );


		mMaterial = MaterialManager::getSingleton().create( "RTT_Material_"+Ogre::StringConverter::toString( i ), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ).get() ; 
		mMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled( false );
		mMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled( false );
		mMaterial->getTechnique(0)->getPass(0)->setLightingEnabled( false );

		mTextureUnitState = NULL ;
		mTextureUnitState = mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(); 
		mTextureUnitState->setContentType( Ogre::TextureUnitState::CONTENT_NAMED );
		mTextureUnitState->setTextureName( "RTT_Texture_100",  TEX_TYPE_2D );

		mPatch = new Nx3DSceneObjectPatch();
		mPatch->Create("Screen_Mesh_Layer_"+ StringConverter::toString(i),ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mMonitorInfo->GetWidth(), mMonitorInfo->GetHeight() );

		mEntity = mSceneManager->createEntity("Screen_Entity_Name_"+ StringConverter::toString(i), "Screen_Mesh_Layer_"+ StringConverter::toString(i) );
		mEntity->getSubEntity(0)->setUseIdentityView( false ); // true make it move with camera
		mEntity->setMaterialName("RTT_Material_"+Ogre::StringConverter::toString( i )); 

		mSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Screen_SceneNode_QUAD_Name_"+ StringConverter::toString(i));
		mSceneNode->attachObject( mEntity );
		mSceneNode->setPosition( -(mMonitorInfo->GetWidth() /2.0f), (mMonitorInfo->GetHeight() / 2.0f ), -2000.0f );


		mCamera = mSceneManager->createCamera("Screen_Camera_" + Ogre::StringConverter::toString( i ) );
		mCamera->setPosition( Parent_Camera->getPosition() );
		mCamera->setDirection( Parent_Camera->getDirection() );
		mCamera->setAspectRatio( mMonitorInfo->GetWidth() / mMonitorInfo->GetHeight() ); 
		mCamera->setNearClipDistance( Parent_Camera->getNearClipDistance() );
		mCamera->setFarClipDistance( Parent_Camera->getFarClipDistance() );

		// set the orthographic projection
		Real Width_Trans   = mMonitorInfo->GetWidth() ;
		Real  Height_Trans = mMonitorInfo->GetHeight() ;
		Real Zoom = 1.0f;

		Nx::Matrix4 p =  NxScreenManager::getSingleton().BuildScaledOrthoMatrix( Width_Trans / Zoom / -2.0f  ,
			Width_Trans  / Zoom /  2.0f  ,
			Height_Trans / Zoom / -2.0f  ,
			Height_Trans / Zoom /  2.0f  ,
			mCamera->getNearClipDistance() ,
			mCamera->getFarClipDistance() );

		mCamera->setCustomProjectionMatrix( true, NxMat4toOgre( p ) );

		mViewport = mRenderWindow->addViewport( mCamera );
		// mViewport->getTarget()->setAutoUpdated( false );// will be updated manually in the mixer
		// mRenderWindow->setActive(false);

		 mViewport->setAutoUpdated( true );
		 mRenderWindow->setAutoUpdated(true);
		mRenderWindow->setActive(true);


		mViewport->setClearEveryFrame( true );
		mViewport->setBackgroundColour( ColourValue::Black );
		mViewport->setOverlaysEnabled( false );

	}
}


void NxScreen::Update()
{
	if( mRenderWindow )
	{
		// mRenderWindow->getViewport(0)->getTarget()->update(false );
		// mRenderWindow->getViewport(0)->getTarget()->swapBuffers( true ) ;
	}
}

NxScreen::~NxScreen()
{
	if( mWindowHandle )
	{
		delete mPatch;
		Log("Destroying External Window : " + mRenderWindow->getName() );
		mRenderWindow->setActive( false );
		mRenderWindow->destroy();
		//::DestroyWindow( (HWND)mWindowHandle );
		mWindowHandle = 0 ;
	}
}

void NxScreen::SetTextureName( const std::string & TextureName )
{
	mTextureUnitState->setTextureName( TextureName );
}

void NxScreen::SetCamera( Ogre::Camera * cam )
{
	mViewport->setCamera(cam);

}

void * NxScreen::GetWindowHandle()
{
	return mWindowHandle;
}



std::string NxScreen::GetScreenTitleName()
{
	char WindowTitleName[300];
	//::GetWindowTextA( (HWND)mWindowHandle, WindowTitleName, sizeof(WindowTitleName) );
	return std::string( WindowTitleName );
}

std::string NxScreen::GetScreenIdentifierName()
{
	return mMonitorInfo->GetAdapterName() ;
}

unsigned int NxScreen::GetScreenIndex()
{
	return mMonitorInfo->GetId()-1;
}

Ogre::Viewport * NxScreen::GetViewport()
{
	return mViewport;
}

void NxScreen::SetPointPosition( unsigned int Point, Ogre::Vector3 Position )
{
	GetPatchMesh().SetVertexPosition( (NxGraphicsQuadPositions)Point , Position );
}

void NxScreen::SetFullscreen( bool FullScreen )
{
	mRenderWindow->setFullscreen( FullScreen, GetWidth(), GetHeight() );
}

bool NxScreen::IsFullscreen()
{
	return mRenderWindow->isFullScreen();
}

long NxScreen::GetX()
{ 
	return mMonitorInfo->GetPositionX() ;
}

long NxScreen::GetY()
{
	return mMonitorInfo->GetPositionY() ;
}

long NxScreen::GetWidth()
{ 
	return mMonitorInfo->GetWidth() ;
}

long NxScreen::GetHeight()
{
	return mMonitorInfo->GetHeight() ;
}

const std::string NxScreen::GetDeviceName()
{
	return mMonitorInfo->GetDeviceName() ;
}

TextureUnitState * NxScreen::GetTextureUnit()
{
	return mTextureUnitState;
} 

void NxScreen::SetPolygonView( NxPolygonMode Mode )
{
	mCamera->setPolygonMode( (Ogre::PolygonMode)  Mode ); 
}

NxPolygonMode NxScreen::GetPolygonView()
{
	return (NxPolygonMode)mCamera->getPolygonMode(); 
}

void NxScreen::SetOpacity( float Opacity )
{
	mMaterial->getTechnique(0)->getPass(0)->setSceneBlending( SBT_TRANSPARENT_ALPHA );
	mMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled( false );
	mTextureUnitState->setAlphaOperation( LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, Opacity );
	mOpacity = Opacity;
}

float NxScreen::GetOpacity()
{
	return mOpacity;
}

Nx3DSceneObjectPatch & NxScreen::GetPatchMesh()
{
	return *mPatch;
}


}