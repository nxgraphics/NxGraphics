#include "NxGraphicsPreCompiled.h"
//#include "NxTextureManager.h"
//#include "NxGraphics_3D_Scene.h"
#include <Overlay/OgreOverlaySystem.h>

//#include "NxGraphics_3D_MeshFactory.h"


#include "../nxdeps/include/OIS/OIS.h"

//#include <OgreDistanceLodStrategy.h>
 
#ifdef NDEBUG
#define DEBUG_D_POSTFIX ""
#else
#define DEBUG_D_POSTFIX "_d"
#endif
 

#ifdef __ANDROID__

#include <jni.h>
#include <EGL/egl.h>
#include <android/api-level.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Android/OgreAndroidEGLWindow.h"
#include "Android/OgreAPKFileSystemArchive.h"
#include "Android/OgreAPKZipArchive.h"
#include "OgreOctreePlugin.h"
#include "OgreParticleFXPlugin.h"

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
#	include "OgreGLES2Plugin.h"
#	define GLESRS GLES2Plugin
#else
#	include "OgreGLESPlugin.h"
#	define GLESRS GLESPlugin
#endif

#include "OgreRTShaderSystem.h"

//#define _RTSS_WRITE_SHADERS_TO_DISK

using namespace Ogre;

class ShaderGeneratorTechniqueResolverListener : public MaterialManager::Listener
{
public:

	ShaderGeneratorTechniqueResolverListener(RTShader::ShaderGenerator* pShaderGenerator)
	{
		mShaderGenerator = pShaderGenerator;
	}

	virtual Technique* handleSchemeNotFound(unsigned short schemeIndex, 
		const String& schemeName, Material* originalMaterial, unsigned short lodIndex, 
		const Renderable* rend)
	{		
		// Case this is the default shader generator scheme.
		if (schemeName == RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
		{
			MaterialRegisterIterator itFind = mRegisteredMaterials.find(originalMaterial);
			bool techniqueCreated = false;

			// This material was not registered before.
			if (itFind == mRegisteredMaterials.end())
			{
				techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
					originalMaterial->getName(), 
					MaterialManager::DEFAULT_SCHEME_NAME, 
					schemeName);				
			}
			mRegisteredMaterials[originalMaterial] = techniqueCreated;
		}

		return NULL;
	}

protected:
	typedef std::map<Material*, bool>		MaterialRegisterMap;
	typedef MaterialRegisterMap::iterator	MaterialRegisterIterator;


protected:
	MaterialRegisterMap				mRegisteredMaterials;		// Registered material map.
	RTShader::ShaderGenerator*		mShaderGenerator;			// The shader generator instance.
};
//static ShaderGeneratorTechniqueResolverListener* gMatListener = NULL;

RTShader::ShaderGenerator*					mShaderGenerator;			// The Shader generator instance.
ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;		// Material manager listener.

bool initializeShaderGenerator(SceneManager* sceneMgr)
{	

	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

		mShaderGenerator->addSceneManager(sceneMgr);

#if NXGRAPHICS_PLATFORM != NXGRAPHICS_PLATFORM_ANDROID && NXGRAPHICS_PLATFORM != NXGRAPHICS_PLATFORM_NACL //&& NXGRAPHICS_PLATFORM != NXGRAPHICS_PLATFORM_WINRT
		// Setup core libraries and shader cache path.
		Ogre::StringVector groupVector = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
		Ogre::StringVector::iterator itGroup = groupVector.begin();
		Ogre::StringVector::iterator itGroupEnd = groupVector.end();
		Ogre::String shaderCoreLibsPath;
		Ogre::String shaderCachePath;

		for (; itGroup != itGroupEnd; ++itGroup)
		{
			Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
			Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
			Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
			bool coreLibsFound = false;

			// Try to find the location of the core shader lib functions and use it
			// as shader cache path as well - this will reduce the number of generated files
			// when running from different directories.
			for (; it != itEnd; ++it)
			{
				if ((*it)->archive->getName().find("RTShaderLib") != Ogre::String::npos)
				{
					shaderCoreLibsPath = (*it)->archive->getName() + "/cache/";
					shaderCachePath = shaderCoreLibsPath;
					coreLibsFound = true;
					break;
				}
			}
			// Core libs path found in the current group.
			if (coreLibsFound) 
				break; 
		}

		// Core shader libs not found -> shader generating will fail.
		if (shaderCoreLibsPath.empty())			
			return false;			

#ifdef _RTSS_WRITE_SHADERS_TO_DISK
		// Set shader cache path.
#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_APPLE_IOS
		shaderCachePath = Ogre::macCachePath();
#elif NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_APPLE
		shaderCachePath = Ogre::macCachePath() + "/org.ogre3d.RTShaderCache";
#endif
		mShaderGenerator->setShaderCachePath(shaderCachePath);		
#endif
#endif
		// Create and register the material manager listener if it doesn't exist yet.
		if (mMaterialMgrListener == NULL) {
			mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);
			Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
		}
	}

	return true;


	/*
	if (RTShader::ShaderGenerator::initialize())
	{
		mShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();

		// Set the scene manager.
		mShaderGenerator->addSceneManager(sceneMgr);

#if NXGRAPHICS_PLATFORM != NXGRAPHICS_PLATFORM_ANDROID && NXGRAPHICS_PLATFORM != NXGRAPHICS_PLATFORM_NACL //&& NXGRAPHICS_PLATFORM != NXGRAPHICS_PLATFORM_WINRT

		// Setup core libraries and shader cache path.
		ResourceGroupManager::LocationList resLocationsList = ResourceGroupManager::getSingleton().getResourceLocationList("Popular");
		ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
		ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
		String shaderCoreLibsPath;
		String shaderCachePath;

		// Default cache path is current directory;
		shaderCachePath = "./";

		// Try to find the location of the core shader lib functions and use it
		// as shader cache path as well - this will reduce the number of generated files
		// when running from different directories.
		for (; it != itEnd; ++it)
		{

			if ((*it)->archive->getName().find("RTShaderLib") != String::npos)
			{
				shaderCoreLibsPath = (*it)->archive->getName() + "/";
				shaderCachePath    = shaderCoreLibsPath;
				break;
			}
		}

		// Core shader libs not found -> shader generating will fail.
		if (shaderCoreLibsPath.empty())			
			return false;			

		// Add resource location for the core shader libs. 
		ResourceGroupManager::getSingleton().addResourceLocation(shaderCoreLibsPath , "FileSystem");

		// Set shader cache path.
		//mShaderGenerator->setShaderCachePath(shaderCachePath);	// error on android

#ifdef _RTSS_WRITE_SHADERS_TO_DISK
		// Set shader cache path.
#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_APPLE_IOS
		shaderCachePath = Ogre::macCachePath();
#elif NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_APPLE
		shaderCachePath = Ogre::macCachePath() + "/org.ogre3d.RTShaderCache";
#endif
		mShaderGenerator->setShaderCachePath(shaderCachePath);		
#endif
#endif
		// Create and register the material manager listener if it doesn't exist yet.
		if (mMaterialMgrListener == NULL) {
			mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);
			Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
		}


		// Create and register the material manager listener.
		mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);				
		MaterialManager::getSingleton().addListener(mMaterialMgrListener);
	}

	return true;*/
}

AAssetManager* mAssetMgr;       // Android asset manager to access files inside apk


#endif

 
namespace Nx {

template<> NxEngine* NxSingleton<NxEngine>::ms_Singleton = 0;

NxEngine::NxEngine() : m_RenderStop( false ), m_PauseRendering( false ), m_HasPreviewWindow( false )
{
	mIsInitialized = false;
	mStateManager = new NxStateManager();
	mShutdown = false ;
	m_DeviceLost = false ;
	mNxWindow = 0;
	std::string logPath = "";

#ifdef __ANDROID__
	NxMediaPath = "";///sdcard/NxMedia/Video/OGRE/";
	mRoot = new Ogre::Root( "", "", "/sdcard/NxGraphics.log" );  
#else
	NxMediaPath = "../../NxMedia/Video/OGRE/";
	mRoot = new Ogre::Root( "", "", NxMediaPath + "Media/Settings/Logs/NxGraphics.log" ); 
#endif
	if( !mRoot ) throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR, "Could not create NxGraphics Root node!", "NxGraphics Core Engine" );
}

NxEngine::~NxEngine()
{

}

NxEngine * NxEngine::getSingletonPtr()
{
	return ms_Singleton;
}

NxEngine& NxEngine::getSingleton()
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}
 
void NxEngine::ShutDownEngine()
{
	mRoot->shutdown();
}

const std::string & NxEngine::GetNxMediaPath()
{
	return NxMediaPath;
}

bool NxEngine::IsDeviceLost()
{
	return m_DeviceLost ;
}

bool NxEngine::IsRendering()
{
	return mShutdown;
}

bool NxEngine::IsInitialized() {
	return mIsInitialized;

}

void NxEngine::AddFrameListener( NxFrameListener * newListener ) {

	mFrameListeners.push_back(newListener); // Insert, unique only (set)

	/*
	// Check if the specified listener is scheduled for removal
	std::set<NxFrameListener *>::iterator i = mRemovedFrameListeners.find(newListener);

	// If yes, cancel the removal. Otherwise add it to other listeners.
	if (i != mRemovedFrameListeners.end())
		mRemovedFrameListeners.erase(*i);
	else
		mFrameListeners.insert(newListener); // Insert, unique only (set)

		*/
}

//-----------------------------------------------------------------------
void NxEngine::RemoveFrameListener( NxFrameListener * oldListener ) {
	std::vector<NxFrameListener*>::iterator it;
	for ( it = mFrameListeners.begin(); it != mFrameListeners.end(); )
	{
		if( (*it) == oldListener )
		{
			delete * it;  
			it = mFrameListeners.erase(it);
		}	
		else {
			++it;
		}
	}
 

	 
	// Remove, 1 only (set), and only when this listener was added before.
	//if( mFrameListeners.find( oldListener ) != mFrameListeners.end() )
	//	mRemovedFrameListeners.insert(oldListener);
}

void NxEngine::LogMessage( const std::string & msg ) {

	Ogre::LogManager::getSingleton().logMessage( msg );
}

unsigned int NxEngine::GetFrameListenerSize() {

	return mFrameListeners.size();
}

bool NxEngine::Update() {

	if( mShutdown ) return false;
	static bool FirstTime = false;
	static unsigned long lastMS ;

	if(!FirstTime) {
		assert(Root::getSingleton().getRenderSystem() != 0); 
		lastMS = Root::getSingleton().getTimer()->getMilliseconds();
		Root::getSingleton().clearEventTimes();
		FirstTime = true;
	}

	unsigned long nowMS = Root::getSingleton().getTimer()->getMilliseconds();
	unsigned long elapsedMS = nowMS - lastMS;
	lastMS = nowMS;

	static float divideBy1000 = 0.001f;
	float elapsedS = static_cast<float>(elapsedMS) * divideBy1000;

	//FrameEvent frameEvent;
	//frameEvent.timeSinceLastEvent = frameEvent.timeSinceLastFrame = elapsedS;
	 Ogre::WindowEventUtilities::messagePump();
	 
#ifdef __ANDROID__ 
	mNxWindow->GetWindow()->windowMovedOrResized();
#endif
	 
	
	if(!mRoot->_fireFrameStarted()){
		LogMsg( "NxGraphics _fireFrameStarted Shutting down" );
		mShutdown = true ;
	}

	RenderFrame( elapsedMS );

	mRoot->_updateAllRenderTargets(); 

	mRoot->_fireFrameRenderingQueued();

	

	NxFrameEvent frameEvent;
	frameEvent.timeSinceLastEvent = frameEvent.timeSinceLastFrame = elapsedS;

	std::vector<NxFrameListener*>::iterator i;
	for (i= mFrameListeners.begin(); i != mFrameListeners.end(); ++i) {

		NxFrameListener* iter = (*i);
		if (!iter->frameRenderingQueued( frameEvent )){}
			//return false;
	}

	if(!mRoot->_fireFrameEnded()){ 
		LogMsg( "NxGraphics _fireFrameEnded Shutting down" );
		mShutdown = true; 
	}   

	

	return true;
}

void NxEngine::StartRendering(){

	assert(Root::getSingleton().getRenderSystem() != 0);
	Root::getSingleton().getRenderSystem()->_initRenderTargets();
	// Clear event times
	Root::getSingleton().clearEventTimes();
 
	// Infinite loop, until broken out of by frame listeners or break out by calling queueEndRendering()
	unsigned long lastMS = Ogre::Root::getSingleton().getTimer()->getMilliseconds();

	while( !mShutdown ) { 
		// Update Character Controllers
		NxScene3DPhysicsManager::getSingleton().UpdateControllers();
		//Pump messages in all registered RenderWindow windows.
		Ogre::WindowEventUtilities::messagePump(); 
		unsigned long nowMS = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		unsigned long elapsedMS = nowMS - lastMS;
		lastMS = nowMS;
		static float divider = 0.001f;
		float elapsedS = static_cast<float>(elapsedMS) * divider;


		if (!Root::getSingleton().renderOneFrame(elapsedS)) break;

		NxFrameEvent frameEvent;
		frameEvent.timeSinceLastEvent = frameEvent.timeSinceLastFrame = elapsedS;
		std::vector<NxFrameListener*>::iterator i;
		for (i= mFrameListeners.begin(); i != mFrameListeners.end(); ++i) {
			NxFrameListener* iter = (*i);
			if (!iter->frameRenderingQueued( frameEvent )){}
			//return false;
		}
 
		mStateManager->Update( elapsedS );

		RenderFrame( elapsedMS );

		//Sleep(10);

	}

	LogMsg("StartRendering() exited ! ");
}

void NxEngine::StopRendering()
{
	mShutdown = true ;
}

bool NxEngine::RenderFrame( unsigned long elapsedMS ){
	
	//update input manager
	float Time = ( float ) elapsedMS / 1000.0f ;
	NxInputManager::getSingleton().Update( Time );
	NxAnimationManager::getSingleton().Update( elapsedMS );
	NxTextureManager::getSingleton().Update( Time );

	 // update external screens ?
	GetScreenManager()->Update();
	return true ;
}
 
void NxEngine::PauseEngine( bool Pause )
{
	m_PauseRendering = Pause ;
}

void NxEngine::AddState( NxState * State )
{
	// to do implement http://www.ogre3d.org/tikiwiki/Game+State+Manager
	mStateManager->PushState( State );
}

void NxEngine::SetState( NxState * State )
{
	mStateManager->ChangeState( State );
}
 
void NxEngine::RemoveState( NxState * State )
{	 
	mStateManager->RemoveState( State );
}


void NxEngine::InitializeEngine( const NxGraphicsDesc& engineDesc )
{
	unsigned int Width  = engineDesc.Width ;
    unsigned int Height = engineDesc.Height ;

	mRenderSystem = engineDesc.Renderer;
	mRenderOptions = engineDesc.GlOptions;
	//m_Fullscreen = engineDesc.Fullscreen;
	NxMediaPath = engineDesc.MediaPath;

	/*
	if( wglGetCurrentDC() && ( NxGraphics_OpenGL == mRenderSystem ) ){ 
		Log("WARNING there is an opengl DC running !");
		if( wglGetCurrentContext() ){
			Log("WARNING there is an opengl Context running !");
		}
	}*/

	SetupRenderer( Width, Height, mRenderSystem, mRenderOptions );//Prepare the renderer


	mRoot->initialise( false );
	//unsigned int NumMonitors = Ogre::Root::getSingleton().getDisplayMonitorCount();

#ifdef __ANDROID__
	mAssetMgr = engineDesc.mAssetMgr;
	if ( mAssetMgr ) {
		// add files android support
		ArchiveManager::getSingleton().addArchiveFactory( new APKFileSystemArchiveFactory((AAssetManager*)mAssetMgr) );
		// add zip file support
		ArchiveManager::getSingleton().addArchiveFactory( new APKZipArchiveFactory((AAssetManager*)mAssetMgr) );
	}
#endif

	// Window Creation
	NxRenderWindowDesc WindowDesc;
#ifdef __ANDROID__
	WindowDesc.Window = engineDesc.mNativeWnd; 
	WindowDesc.GlContext = engineDesc.GlContext;
	WindowDesc.mConfig = engineDesc.mConfig;
#else
	WindowDesc.Window = engineDesc.Window;
#endif

	WindowDesc.Width = engineDesc.Width;
	WindowDesc.Height = engineDesc.Height;
	WindowDesc.Vsync = engineDesc.Vsync;
	WindowDesc.VsyncRate = engineDesc.VsyncRate;
 	WindowDesc.ScreenIndex = engineDesc.ScreenIndex;
 	WindowDesc.Fullscreen = engineDesc.Fullscreen;
 	WindowDesc.Fsaa = engineDesc.Fsaa;
	WindowDesc.mWindowName = "NxGraphics Standalone Application 1.0";

	LogMsg("Creating window .. ");
	mNxWindow = CreateRenderWindow( WindowDesc );

	LogMsg("Creating window DONE ");
	//static_cast<Ogre::AndroidEGLWindow*>(mNxWindow->GetWindow())->_createInternalResources( engineDesc.mNativeWnd, NULL);
 

	#ifdef __ANDROID__
	//NxInputManager::getSingleton().AddWindow( mNxWindow ); //crash
#else
	// if standalone ..
	if( !WindowDesc.Window ) NxInputManager::getSingleton().AddWindow( mNxWindow ); // because crash on embeded mode..
#endif
	//
	LogMsg("Creating scene manager .. ");
	mSceneManager = new NxContentManager( NXST_GENERIC, "NxMainSceneManager");
	mCamera = mSceneManager->CreateCamera( "NxMainCamera" );
	
	Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
	mSceneManager->AddRenderQueueListener(pOverlaySystem);

	mCamera->SetNearClipDistance(0.1f);
	mCamera->SetFarClipDistance(4096.0f);
	mCamera->SetPosition( 0.0f, 0.0f, 2000.0f );
	/*
	float Distance = 2000.0f;
	// Calculate FOV H in degrees from Camera Distance.
	double FOVRad = 2.0f *  atan(( (float)Height / 2.0f) / Distance );
	double FOVDeg = FOVRad * 180.0f / Ogre::Math::PI;
	mCamera->SetFOVy( FOVDeg );*/

	mViewport = mNxWindow->AddViewport(mCamera);

	LogMsg("==> Creating CreateResources.. ");
	CreateResources();
	LogMsg("==>initializing resource group Bootstrap .. ");
	ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");//for screen loader

#ifdef __ANDROID__
	LogMsg("==>initializing shader generator .. ");
	 if(!initializeShaderGenerator( mSceneManager->GetContentManager() )) LogMsg("-> initializeShaderGenerator Failed !"); 
#endif
	//mCamera->SetAspectRatio( float( mViewport->GetActualWidth()) / float(mViewport->GetActualHeight()) );
	mViewport->SetBackgroundColour( NxColourValue(0,0,0,1) );
	mViewport->SetClearEveryFrame( true );//true); // ori
	// mViewport->GetViewport()->setClearEveryFrame(true, Ogre::FBT_DEPTH|Ogre::FBT_STENCIL);


	mNxWindow->SetAutoUpdated(true); // window will be updated in client application
	mViewport->SetAutoUpdated(true);// rendertarget will be updated by the engine


	// Set texture and material default
	TextureManager::getSingleton().setDefaultNumMipmaps(0);
	MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	MaterialManager::getSingleton().setDefaultAnisotropy(8); 


	// lod strategy
	//Mesh::LodValueList lodlist;
	//lodlist.push_back(4.0);
	//lodlist.push_back(8.0);
	//lodlist.push_back(128.0);
	//Ogre::DistanceLodStrategy *start = new Ogre::DistanceLodStrategy() ;// lodlist);
	//start->assertSorted(  lodlist );
	//LodStrategyManager::getSingleton().setDefaultStrategy(start);
	
	
	// load splash screen texture name if it doesnt exists
	LogMsg("==> loading splash screen texture ");
//	if( !TextureManager::getSingleton().resourceExists ( engineDesc.mSplashTexture ) ) {
 	if( !Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup ( engineDesc.mSplashTexture ) ) {
	
		LogMsg("==> texture doesnt exist : creating " + engineDesc.mSplashTexture);
	
		NxTextureImage * splashTexture = NxTextureManager::getSingleton().CreateTextureImage( engineDesc.mSplashTexture, engineDesc.mSplashTexture );
	
 
	
	}
	
	
	
	//engineDesc.mSplashTexture
	
	
	
	LogMsg("==> progress bar .. ");
  
	mProgressBar = new ProgressBar();
	mProgressBar->Start( Nx::Vector4(engineDesc.mSplashSize[0], engineDesc.mSplashSize[1], engineDesc.mSplashSize[2], engineDesc.mSplashSize[3]), engineDesc.mSplashTexture ,  ResourceGroupManager::getSingleton().getResourceGroups().size(), ResourceGroupManager::getSingleton().getResourceGroups().size(), 2.0f );

	// Turn off rendering of everything except overlays
	mSceneManager->ClearSpecialCaseRenderQueues();
	mSceneManager->AddSpecialCaseRenderQueue( RENDER_QUEUE_OVERLAY );
	mSceneManager->SetSpecialCaseRenderQueueMode( NXSCRQM_INCLUDE ); 

		LogMsg("==> initialiseAllResourceGroups .. ");
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); //Load All. takes Some time... ;
	LogMsg("==> initialiseAllResourceGroups DONE");

	CheckCapabilities();
	mShutdown = false ;
}

NxRenderWindow * NxEngine::CreateRenderWindow( const NxRenderWindowDesc & WindowDesc )
{
	return new NxRenderWindow( WindowDesc );
}

NxContentManager * NxEngine::CreateContentManager( NxSceneType Type, const std::string & name )
{
	return new NxContentManager( Type, name );
}

 #ifdef __ANDROID__
Ogre::DataStreamPtr openAPKFile(const Ogre::String& fileName)
{
	Ogre::DataStreamPtr stream;
	AAsset* asset = AAssetManager_open(mAssetMgr, fileName.c_str(), AASSET_MODE_BUFFER);
	if(asset)
	{
		off_t length = AAsset_getLength(asset);
		void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
		memcpy(membuf, AAsset_getBuffer(asset), length);
		AAsset_close(asset);

		stream = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(membuf, length, true, true));
	}
	return stream;
}
 
#endif

void NxEngine::CreateResources()
{
	ConfigFile cf;
#ifdef __ANDROID__
	cf.load(openAPKFile("resources.cfg"));
#else
	cf.load( NxMediaPath + "media/resources.cfg");
#endif
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first; archName = i->second;
			bool recursive = false;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName, recursive);// Load Script Resource Locations
		}
	} 

	//Editor Resources ( Gizmo, icons )
	ResourceGroupManager::getSingleton().createResourceGroup("EditorResources");

	//Material Resources.
	ResourceGroupManager::getSingleton().createResourceGroup("NxMaterials");

	//Characters Resources
	ResourceGroupManager::getSingleton().createResourceGroup("NxCharacters");

	// Create location for Video Materials
	ResourceGroupManager::getSingleton().createResourceGroup("NxMaterialVideo");

#ifndef __ANDROID__

	ResourceGroupManager::getSingleton().addResourceLocation( NxMediaPath + "media/Models/Characters", "FileSystem", "NxCharacters", false );// recursive ?

	std::vector<std::string> refvecDirectory;
	NxUtils::ListDirectories( refvecDirectory, NxMediaPath + "media/Models/Characters", false );
	for( int i = 0 ; i < refvecDirectory.size() ; i++ ) {
		ResourceGroupManager::getSingleton().addResourceLocation( NxMediaPath + "media/Models/Characters/" + Ogre::String( refvecDirectory[i] ), "FileSystem", "NxCharacters", false );
	}

#endif

}

void NxEngine::PostInitialized()
{
	// Back to full rendering
	mSceneManager->ClearSpecialCaseRenderQueues();
	mSceneManager->SetSpecialCaseRenderQueueMode( NXSCRQM_EXCLUDE );
	mProgressBar->Finish();
	delete mProgressBar; 
	mIsInitialized = true;
}

void NxEngine::CheckCapabilities()
{
	 /*
	LogMsg( "----- [ Hardware Capabilities ] -----" );
	RenderSystem * rend = Ogre::Root::getSingleton().getRenderSystem();
	const RenderSystemCapabilities  * caps = rend->getCapabilities();

	caps->hasCapability( RSC_FBO ) ? LogMsg( "-- Card has support for Opengl Fbo" ) : LogMsg( "-! Card Doesnt Have support for Opengl Fbo" );
	if(caps->hasCapability(RSC_FBO_ARB)) LogMsg( "-- CARD HAS RSC_FBO_ARB" );
	if(caps->hasCapability(RSC_FBO_ATI)) LogMsg( "-- CARD HAS RSC_FBO_ATI" );
	caps->hasCapability( RSC_PBUFFER ) ? LogMsg( "-- Card has support for Opengl PBuffers" ) : LogMsg( "-! Card Doesnt Have support for Opengl PBuffers" );
	caps->hasCapability( RSC_VBO ) ? LogMsg( "-- Card has support for hardware vertex and index buffers" ) : LogMsg( "-! Card Doesnt have support for hardware vertex and index buffers" );

	LogMsg( "-- Max Multirender Targets " + Ogre::StringConverter::toString( caps->getNumMultiRenderTargets() ) );

	caps->getNonPOW2TexturesLimited() ? 
	LogMsg( "-- Non Support of Non power of two textures, errors might occur" ) :
	LogMsg( "-- Supporting Non power of two textures" );
 
	DriverVersion driverversion = caps->getDriverVersion();
	LogMsg( "-- Driver Version : " + driverversion.toString() );

	LogMsg( "-----[ Hardware Capabilities ]-----" );
	*/
	 
}

void NxEngine::SetupRenderer( unsigned long width, unsigned long height, NxRendererType Render_Type, NxRendererGlOptions OpenGL_Options )
{
	std::map<Ogre::String,Ogre::String> configuration ;
 
	if( Render_Type == NxGraphics_DirectX ) {
		configuration["device"] = "Direct3D9 Rendering Subsystem";
		mRoot->loadPlugin("plugins/RenderSystem_Direct3D9"DEBUG_D_POSTFIX);
	}
	else if( Render_Type == NxGraphics_DirectX10 ) {
		configuration["device"] = "Direct3D10 Rendering Subsystem";
		mRoot->loadPlugin("plugins/RenderSystem_Direct3D10"DEBUG_D_POSTFIX);
	}
	else if( Render_Type == NxGraphics_DirectX11  ) {
		configuration["device"] = "Direct3D11 Rendering Subsystem";
		mRoot->loadPlugin("plugins/RenderSystem_Direct3D11"DEBUG_D_POSTFIX);
	}
	else if( Render_Type == NxGraphics_OpenGL ) {
		configuration["device"] = "OpenGL Rendering Subsystem";
		mRoot->loadPlugin("plugins/RenderSystem_GL"DEBUG_D_POSTFIX);
	}
	else if( Render_Type == NxGraphics_GLES2 ) {
#ifdef __ANDROID__
		configuration["device"] = "OpenGL ES 2.x Rendering Subsystem";
		static Ogre::GLESRS* gGLESPlugin = NULL;
		gGLESPlugin = OGRE_NEW GLESRS();
		mRoot->installPlugin(gGLESPlugin);
#endif
	}
	else  
	{
		LogMsg("NxEngine::SetupRenderer : No Renderer Set ! Error");
		return; 
	}

	// configuration
 
	configuration["fullscreen"] = "false";
	Ogre::StringUtil::toLowerCase( configuration["device"] );
 
	if( Render_Type != NxGraphics_GLES2 ) {
		mRoot->loadPlugin("plugins/Plugin_ParticleFX"DEBUG_D_POSTFIX);
		mRoot->loadPlugin("plugins/Plugin_CgProgramManager"DEBUG_D_POSTFIX);  
		mRoot->loadPlugin("plugins/Plugin_OctreeSceneManager"DEBUG_D_POSTFIX);
	}else{
#ifdef __ANDROID__
		static Ogre::ParticleFXPlugin* gParticleFXPlugin = NULL;
		gParticleFXPlugin = OGRE_NEW ParticleFXPlugin();
		mRoot->installPlugin(gParticleFXPlugin);

		static Ogre::OctreePlugin* gOctreePlugin = NULL;
		gOctreePlugin = OGRE_NEW OctreePlugin();
		mRoot->installPlugin(gOctreePlugin);
#endif

	}
 

	RenderSystemList RenderDeviceList = mRoot->getAvailableRenderers();
	LogMsg("Default Render Device : " + RenderDeviceList[0]->getName() );

	NxRenderSystem = RenderDeviceList[0];
	mRoot->setRenderSystem(NxRenderSystem);
	//NxRenderSystem->addListener( this );
 
	LogMsg("Checking render messages list");
	const StringVector& rendernames = NxRenderSystem->getRenderSystemEvents();
	for( int i = 0 ; i < rendernames.size() ; i++  )
	{
		LogMsg(" Render Messages : "+rendernames[ i ] );
	}
	LogMsg("render messages list Done");

	NxRenderSystem->setConfigOption("Full Screen", configuration["fullscreen"]);
	//NxRenderSystem->setConfigOption("VSync", "No");

	NxRenderSystem->getConfigOptions();

	if (configuration["device"] == "Direct3D9 Rendering Subsystem")
	{
		configuration["depth"] = "32" ;
		NxRenderSystem->setConfigOption("Video Mode", Ogre::StringConverter::toString( width )+" x "+Ogre::StringConverter::toString( height )+" @ "+  configuration["depth"] + "-bit colour"); // 4 ecran : 1600 x 300
		LogMsg("mRenderSystem->setConfigOption : " + Ogre::StringConverter::toString( width )+" x "+Ogre::StringConverter::toString( height )+" @ "+  configuration["depth"] + "-bit colour"  );
		NxRenderSystem->setConfigOption("Resource Creation Policy", "Create on active device" ); //no update for screen but no crash
		//  NxRenderSystem->setConfigOption("Resource Creation Policy", "Create on all devices" ); // crashes on switch window
 
	}
	else if (configuration["device"] == "Direct3D10 Rendering Subsystem")
	{
		NxRenderSystem->setConfigOption("Video Mode", Ogre::StringConverter::toString( width )+" x "+Ogre::StringConverter::toString( height )+" @ "+  configuration["depth"] + "-bit colour"); // 4 ecran : 1600 x 300
	}
	else if (configuration["device"] == "Direct3D11 Rendering Subsystem")
	{
		NxRenderSystem->setConfigOption("Video Mode", Ogre::StringConverter::toString( width )+" x "+Ogre::StringConverter::toString( height )+" @ "+  configuration["depth"] + "-bit colour"); // 4 ecran : 1600 x 300
	}
	else if(  configuration["device"] == "OpenGL Rendering Subsystem" ) {
	
		NxRenderSystem->setConfigOption( "Video Mode", Ogre::StringConverter::toString( width )+" x "+Ogre::StringConverter::toString( height )+" @ "+  configuration["depth"] + "-bit colour");
		NxRenderSystem->setConfigOption( "FSAA", "0" );

		Ogre::String OpenGl_opt ;
		switch( OpenGL_Options )
		{
		case NxGraphics_OpenGL_FBO :
			OpenGl_opt = "FBO"; // FBO is the fastest
			break;
		case NxGraphics_OpenGL_PBuffer :
			OpenGl_opt = "PBuffer";
			break;
		case NxGraphics_OpenGL_Copy :
			OpenGl_opt = "Copy";
			break;
		default :
			OpenGl_opt = "FBO"; 
			break;
		}

		NxRenderSystem->setConfigOption( "RTT Preferred Mode", OpenGl_opt ); // Set the options
		NxRenderSystem->setConfigOption( "VSync", "No" );	
	
	}else if( configuration["device"] == "OpenGL ES 2.x Rendering Subsystem" ) {


	}
	else  
	{    
		LogMsg("NxEngine::SetupRenderer : No configuration device found for: " + configuration["device"]);
		return;
	} 


	LogMsg("Graphic Device Configuration Done.");
}

NxRenderWindow * NxEngine::GetNxWindow()
{
	return mNxWindow;
}
 

NxCamera * NxEngine::GetNxCamera()
{
	return mCamera;
}

NxContentManager * NxEngine::GetContentManager()
{
	return mSceneManager;
}

NxViewport * NxEngine::GetNxViewport()
{
	return mViewport;
}

} //NxGraphics_Namespace 