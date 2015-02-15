#include "NxGraphicsPreCompiled.h"


#ifdef __ANDROID__
#include <android/log.h>

#include <EGL/egl.h>
#include "Android/OgreAndroidEGLWindow.h"

#define  LOG_TAG    "NxGraphicsCoreWindow"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

namespace Nx {

	NxRenderWindowDesc::NxRenderWindowDesc() 
	{
		Width = 1024;
		Height = 768;
		Window = 0;
		Fullscreen = false;
		ScreenIndex = 0;
		Vsync = true;
		Fsaa = 0;
		VsyncRate = 60;
		mWindowName = "";
#ifdef __ANDROID__

		 GlContext = 0;
		 mConfig = 0;
#endif
	}

	class NxRenderWindowEventListener : public Ogre::WindowEventListener
	{
	public :

		NxRenderWindowEventListener( NxRenderWindow * ptr ) : mRenderWindow( ptr ), mFocus(true)
		{
		
		}

		void windowMoved(RenderWindow* rw) {

			unsigned int width;
			unsigned int height;
			unsigned int colourDepth;
			int left;
			int top;
			rw->getMetrics( width, height, colourDepth, left, top );

			for( std::list< NxEngineListener * >::iterator iter = mListener.begin(); iter !=  mListener.end(); iter++) {
				(*iter)->OnWindowMoved( left, top );
			}


		}

		void windowResized(RenderWindow* rw) {

			//! here error should be one windw
			NxInputManager::getSingleton().UpdateBounds( rw->getWidth(), rw->getHeight() );

			unsigned int width;
			unsigned int height;
			unsigned int colourDepth;
			int left;
			int top;
			rw->getMetrics( width, height, colourDepth, left, top );

			for( std::list< NxEngineListener * >::iterator iter = mListener.begin(); iter !=  mListener.end(); iter++) {
				(*iter)->OnWindowResized( width, height );
			}
		
		}

		bool windowClosing(RenderWindow* rw) {
			LogMsg("OnWindowClosing");
			for( std::list< NxEngineListener * >::iterator iter = mListener.begin(); iter !=  mListener.end(); iter++) {
				(*iter)->OnWindowClosing();
			}
			
			
			return true;
		}

		void windowClosed(RenderWindow* rw) {
			LogMsg("OnwindowClosed");
			for( std::list< NxEngineListener * >::iterator iter = mListener.begin(); iter !=  mListener.end(); iter++) {
				(*iter)->OnWindowClosed();
			}
		}

		void windowFocusChange(RenderWindow* rw) {
			LogMsg("OnwindowFocusChange");
			for( std::list< NxEngineListener * >::iterator iter = mListener.begin(); iter !=  mListener.end(); iter++) {
				(*iter)->OnWindowFocusChange();
			}

			mFocus = !mFocus;
			//Log("Window : " + rw->getName() +  " Focus : " + Ogre::StringConverter::toString( mFocus ) );
		}

		bool HasFocus()
		{
			return mFocus;
		}

		void AddListener( NxEngineListener * Listener )
		{
			mListener.push_back( Listener );
		}

		void RemoveListener( NxEngineListener * Listener )
		{
			mListener.remove( Listener );
		}

	private :

		std::list< NxEngineListener * > mListener;

		NxRenderWindow* mRenderWindow;
		bool mFocus;

	
	};

	//#pragma comment(lib, "opengl32.lib") 

	NxRenderWindow::NxRenderWindow( const NxRenderWindowDesc & WindowDesc ) : mIsSelfWindow(true)
	{

		#ifdef __ANDROID__
		Ogre::NameValuePairList opt;
 

		if( WindowDesc.GlContext != 0 ){
			LOGD("---> Creating window from external GLContext and Window.");


			//opt["currentGLContext"] = Ogre::String("true");

			//opt["currentGLContext"]     = "true";

			LOGD("---> Window : %d" , (int)WindowDesc.Window );
			LOGD("---> GlContext : %d" ,  (int)WindowDesc.GlContext );

			opt["externalWindowHandle"] = Ogre::StringConverter::toString((int)WindowDesc.Window);
			opt["externalGLContext"]    = Ogre::StringConverter::toString( (int)WindowDesc.GlContext );
		}else{
			LOGD("---> Creating window from external window.");
			opt["externalWindowHandle"] = Ogre::StringConverter::toString((int)WindowDesc.Window);

			if( WindowDesc.mConfig != 0 )
				opt["androidConfig"] = Ogre::StringConverter::toString((int)WindowDesc.mConfig);

		}

		opt["vsync"] = Ogre::StringConverter::toString( WindowDesc.Vsync ); 
 
		bool fullscreen = true;
		mWindow = Ogre::Root::getSingleton().createRenderWindow("OgreWindow", 0, 0, fullscreen, &opt);
		mWindowListener = new NxRenderWindowEventListener( this );
		Ogre::WindowEventUtilities::addWindowEventListener( mWindow, mWindowListener );
		return;



		#endif




		static int id = 0;

		Ogre::NameValuePairList options;
		if( WindowDesc.Window != 0 ){ // self created window

			//options["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<unsigned long>( WindowDesc.Window ));
			//options["externalGLControl"] = Ogre::StringConverter::toString( (true) );
			//options["externalGLContext"] = Ogre::StringConverter::toString( wglGetCurrentContext() );
			
			mIsSelfWindow = false;
			
			options["parentWindowHandle"] = Ogre::StringConverter::toString( reinterpret_cast<unsigned long>( WindowDesc.Window ) );

		
		}

		 

		options["vsync"] = Ogre::StringConverter::toString( WindowDesc.Vsync );  // without physx goes fast  

		options["displayFrequency"] = Ogre::StringConverter::toString( WindowDesc.VsyncRate );
		//options["vsyncInterval"] = Ogre::StringConverter::toString( 1 );
		options["FSAA"] = Ogre::StringConverter::toString(WindowDesc.Fsaa);
		options["FSAAQuality"] = "0";
		options["colourDepth"] = "32";
		options["gamma"] = "0";
		options["useNVPerfHUD"] = "0";
		options["monitorIndex"] = StringConverter::toString( WindowDesc.ScreenIndex );
		LogMsg("NxGraphics_Root->createRenderWindow...."  ); // Sometimes Blocks here from antiviruses

		//if( WindowDesc.mWindowName.empty() ) WindowDesc.mWindowName =  "NxWindow"+Ogre::StringConverter::toString(id++) ;
	 

		if( WindowDesc.Window == 0 ){ // self created window
			options["title"] = WindowDesc.mWindowName; 
			mWindow = Ogre::Root::getSingleton().createRenderWindow(WindowDesc.mWindowName, WindowDesc.Width, WindowDesc.Height , WindowDesc.Fullscreen,  &options );
		}
		else{
			options["externalGLControl"] = Ogre::StringConverter::toString( (false) );
			options["title"] = WindowDesc.mWindowName; 
			mWindow = Ogre::Root::getSingleton().createRenderWindow(WindowDesc.mWindowName, WindowDesc.Width, WindowDesc.Height , WindowDesc.Fullscreen , &options );
		}

		mWindow->setDeactivateOnFocusChange( false );

		// WindowEventUtilities::_addRenderWindow( mWindow );

		mWindowListener = new NxRenderWindowEventListener( this );
		Ogre::WindowEventUtilities::addWindowEventListener( mWindow, mWindowListener );

		 
		//NxInputManager::getSingleton().Create( mWindow );


		 

	
	
	}
	 
	NxRenderWindow::~NxRenderWindow()
	{

	}

	void NxRenderWindow::SwapBuffers() {
		mWindow->swapBuffers();
	}

	void NxRenderWindow::SetAutoUpdated( bool update ) {
		mWindow->setAutoUpdated( update );
	}

	void NxRenderWindow::Update( bool swapBuffer ) {
		mWindow->update( swapBuffer );
	}

	bool NxRenderWindow::IsSelfWindow() {
		return mIsSelfWindow;
	}

	void NxRenderWindow::AddInputListener( NxInputListener * Listener )
	{
		NxInputManager::getSingleton().AddListener( this, Listener );
	}

	void NxRenderWindow::AddListener( NxEngineListener * Listener )
	{
		mWindowListener->AddListener( Listener );
	}

	void NxRenderWindow::RemoveListener( NxEngineListener * Listener )
	{
		mWindowListener->RemoveListener( Listener );
	}

	bool NxRenderWindow::HasFocus()
	{
		return mWindowListener->HasFocus();
	}

	float NxRenderWindow::GetWidth()
	{
		unsigned int width;
		unsigned int height;
		unsigned int colourDepth;
		int left;
		int top;
		mWindow->getMetrics( width, height, colourDepth, left, top );
		return width  ;
	}

	float NxRenderWindow::GetHeight()
	{
		unsigned int width;
		unsigned int height;
		unsigned int colourDepth;
		int left;
		int top;
		mWindow->getMetrics( width, height, colourDepth, left, top );
		return height  ;
	}

	void NxRenderWindow::SetFullscreen( bool FullScreen, int Width, int Height )
	{
		mWindow->setFullscreen( FullScreen, Width, Height );
	}

	void NxRenderWindow::SetFullscreen( bool FullScreen )
	{
		mWindow->setFullscreen( FullScreen, GetWidth(), GetHeight() );
	}

	bool NxRenderWindow::IsFullscreen()
	{
		return mWindow->isFullScreen();
	}

	void NxRenderWindow::SetHidden( bool Hidden )
	{
		mWindow->setHidden( Hidden );
	}

	void * NxRenderWindow::GetWindowNativeHandle()
	{
		#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
			HWND windowHnd = 0;
			mWindow->getCustomAttribute("WINDOW", &windowHnd);
			return windowHnd;
		#elif NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_APPLE
			LogMsg("You need to define void * NxRenderWindow::GetWindowNativeHandle()");
			return 0;
		#elif NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_ANDROID	
			LogMsg("You need to define void * NxRenderWindow::GetWindowNativeHandle()");
			return 0;
		#elif NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_APPLE_IOS
			LogMsg("You need to define void * NxRenderWindow::GetWindowNativeHandle()");
			return 0;
		#endif
	}

	float NxRenderWindow::GetAverageFPS() const
	{
		return mWindow->getAverageFPS();
	}

	void NxRenderWindow::SetWindowPosition( unsigned int Left, unsigned int Top )
	{
		 LogMsg( "NxRenderWindow::SetWindowPosition" );
		mWindow->reposition( Left, Top );
	}

	void NxRenderWindow::SetWindowSize( unsigned int width, unsigned int height )
	{
		 LogMsg( "NxRenderWindow::SetWindowSize" );
		mWindow->resize( width, height );
	}

	void NxRenderWindow::UpdateWindowMetrics()
	{
		mWindow->windowMovedOrResized();
	}


#ifdef __ANDROID__

	void NxRenderWindow::CreateInternalResources( void * window, AConfiguration* config) {
 
		static_cast<AndroidEGLWindow*>(mWindow)->_createInternalResources( (ANativeWindow *) window, config );
	}
	 
	void NxRenderWindow::DestroyInternalResources() {

		static_cast<AndroidEGLWindow*>(mWindow)->_destroyInternalResources();
	}

#endif

	 

	/*
	void NxEngine::SetFocus()
	{
		HWND windowHnd = 0;
		GetNxWindow()->getCustomAttribute("WINDOW", &windowHnd);
		::SetFocus( windowHnd );
	}*/


	void NxRenderWindow::GetCustomAttribute( const std::string& name, void* pData ){

		mWindow->getCustomAttribute( name, pData );
	}


	NxViewport * NxRenderWindow::AddViewport( NxCamera * Cam, int ZOrder )
	{
		NxViewport * c = new NxViewport( this, Cam, ZOrder );
		mViewports.insert(NxViewportList::value_type(ZOrder, c));
		return c;
	}

	unsigned short NxRenderWindow::GetNumViewports() const
	{
		return mWindow->getNumViewports();
	}

	NxViewport* NxRenderWindow::GetViewport(unsigned short index)
	{
		assert (index < mViewports.size() && "Index out of bounds");

		NxViewportList::iterator i = mViewports.begin();
		while (index--)
			++i;
		return i->second;
	}
 

	Ogre::RenderWindow * NxRenderWindow::GetWindow()
	{
		return mWindow;
	}



}
