#include "NxGraphicsPreCompiled.h"
#include "NxParticleSystemAffectorFactory.h"
#include "NxParticleSystemEmitterFactory.h"

#include "../nxdevice/NxDevice_Headers.h"
#include "../nxmocap/NxMocap_Headers.h"
#include "../nxsound/NxSound_Headers.h"
#include "../nxvideo/NxVideo_Headers.h"

namespace Nx {
 
NxGraphicsDesc::NxGraphicsDesc() //set the engine default values.
{
	Width = 1024;
	Height = 1024;
	Renderer =  NxGraphics_OpenGL;
	Window = 0;
	Fullscreen = false;
	ScreenIndex = 0;
	Log = 0;
	MediaPath = "../../NxMedia/Video/OGRE/";
	Vsync = true;
	Fsaa = 0;
	VsyncRate = 60;
	UsePerfHud = false;

	float RatioX = 1.0f/3.0f;
	float RatioY = 1.0f/2.0f;
	mSplashSize[0] = RatioX;
	mSplashSize[1] = 0.1f;
	mSplashSize[2] = RatioX;
	mSplashSize[3] = RatioY;

	mSplashTexture = "NxLogo.jpg";

	#ifdef __ANDROID__
	GlContext = 0;
	mConfig = 0;
	#endif
}
 


//--------------------------------------

 

template<> NxGraphics * NxSingleton<NxGraphics>::ms_Singleton = 0;

NxGraphics::NxGraphics()
{

	new NxLog();
	NxScreenManager * mManagerScreens = new NxScreenManager();
	NxDeviceManager * mManagerDevices = new NxDeviceManager();
	NxPluginManager * mManagerPlugins = new NxPluginManager();
	NxTextureManager * mTextureManager = new NxTextureManager();
}

NxGraphics::~NxGraphics()
{
	GetScreenManager()->DeleteWindows();
	ReleaseEngine();
}

NxGraphics& NxGraphics::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}



bool NxGraphics::CreateNxEngine( const NxGraphicsDesc& engineDesc )
{
	NxAnimationManager * mManagerAnimation = new NxAnimationManager();
	NxInputManager * InputManager = new NxInputManager() ;

	NxEngine * m_ManagerEngine = new NxEngine(); // create & initialize the engine
	//if( engineDesc.Log ) Ogre::LogManager::getSingleton().getDefaultLog()->addListener( engineDesc.Log );

	LogMsg("Creating 3D Scene Manager ...");
	NxScene3DManager * Scene3D = new NxScene3DManager();

	LogMsg("Creating Sound Manager ...");
	NxSoundManager * SoundManager = new NxSoundManager(  NX_WIN_DS );
	LogMsg("Creating Default Sound output Device ...");
	SoundManager->CreateDefaultDeviceOutput();
 
	m_ManagerEngine->InitializeEngine( engineDesc );// initialize the core engine
 
	LogMsg("Creating Motion Capture Manager ...");
	NxMocapManager * MocapManager = new NxMocapManager();
	LogMsg("Creating Video Manager ...");
	NxVideoManager * VideoManager = new NxVideoManager();
	LogMsg("Creating Script Manager ...");
	NxScriptManager * m_ManagerScripting = new NxScriptManager();
	LogMsg("Creating Post Effect Manager ...");
	NxPostEffectManager * m_ManagerPostEffects = new NxPostEffectManager();
	LogMsg("Creating Material Manager ...");
	new NxMaterialManager();

	//add particle affectors.
	Ogre::ParticleSystemManager::getSingleton().addAffectorFactory( new InterpolationAffectorFactory() );
	//add particle emitters.
	Ogre::ParticleSystemManager::getSingleton().addEmitterFactory( new JongEmitterFactory() );


//	bool Res = CheckUserRights();
	m_ManagerEngine->PostInitialized();

	new NxOverlayManager();


	return true;
}

void NxGraphics::Update()
{
	NxEngine::getSingleton().Update();
}

void NxGraphics::StartRendering()
{
	NxEngine::getSingleton().StartRendering();
}

void NxGraphics::ReleaseEngine()
{
	LogMsg("Engine Release .... ");

	NxRenderWindow * MainWindow = NxEngine::getSingleton().GetNxWindow();

	if( MainWindow->IsFullscreen() ) {
		MainWindow->SetFullscreen( false );
	}

	NxSoundManager * SoundManager = &NxSoundManager::getSingleton();
	delete SoundManager;

	// Delete all devices
	NxDeviceManager::getSingleton().DeleteDevices();
	// Delete all layers
//	MixerManager::getSingleton().ReleaseLayers();
	NxEngine::getSingleton().ShutDownEngine();

	LogMsg("Engine Release : Done");
}
 
void NxGraphics::SetFullScreen( int Width, int Height, bool Fulscreen )
{
	NxEngine::getSingleton().GetNxWindow()->SetFullscreen( Fulscreen, Width, Height );
}

void NxGraphics::AddState( NxState * State )
{
	NxEngine::getSingleton().AddState( State );
}

void NxGraphics::SetState( NxState * State )
{
	NxEngine::getSingleton().SetState( State );
}

void NxGraphics::RemoveState( NxState * State )
{
	NxEngine::getSingleton().RemoveState( State );
}

NxEngine * NxGraphics::GetEngineManager()
{
	return &NxEngine::getSingleton();
}

NxDeviceManager * NxGraphics::GetDeviceManager()
{
	return &NxDeviceManager::getSingleton();
}

NxSoundManager * NxGraphics::GetSoundManager()
{
	return &NxSoundManager::getSingleton();
}

void NxGraphics::OpenScript( const std::string& ScriptName )
{
	NxScriptManager::getSingleton().LoadFile( ScriptName );
}

bool NxGraphics::IsRunning()
{
	return !NxEngine::getSingleton().IsRendering();
}

} //NxGraphics_Namespace  

