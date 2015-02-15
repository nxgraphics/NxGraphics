#include "NxGraphicsPreCompiled.h"

namespace NxGraphics_Namespace {

template<> NxScreenManager* Singleton<NxScreenManager>::ms_Singleton = 0;


//=======================================
	static const int Total_Max_Screens = 20 ;// the engine can support maximum up to 20 output screen.	
	NxMonitorInfo * Monitor_Info[Total_Max_Screens];
 //=======================================
 


NxScreenManager::NxScreenManager() : mRenderTexture( 0 )
{
	mMultiDisplayMode = WINDOW_STYLE_DEFAULT;
	 

	BuildMonitorsInfo();

}

NxScreenManager::~NxScreenManager()
{

}

NxScreenManager& NxScreenManager::getSingleton()
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

NxScreenManager * NxScreenManager::getSingletonPtr()
{
	return ms_Singleton;
}

void NxScreenManager::Update()
{
	 
	if( mRenderTexture )
	{
		mRenderTexture->update( false );
		mRenderTexture->swapBuffers(  ) ;
	}

	for( std::list< NxScreen *>::iterator iter = MonitorListActive.begin(); iter != MonitorListActive.end(); iter++)
	{
		NxScreen * Window = *iter; 
		Window->Update();
	}
	 
}
 
void * NxScreenManager::CreateWindowHolder( NxMonitor * Monitor, bool Fullscreen )
{
 
	return 0 ;
}

void NxScreenManager::GetDeviceNameFromIndex( std::string & DevName, unsigned int Index )
{	 
	for( int i =0; i < MonitorList.size() ; i++ ) {
		NxMonitor * WindowInfo =  MonitorList[i];
		if( WindowInfo->GetId() == Index+1 ) {
			DevName = WindowInfo->GetDeviceName();
			break;
		}
	}
}


NxScreen * NxScreenManager::CreateExternalWindow( int MonitorID, bool FullScreen, unsigned int Width, unsigned int Height )
{
	//Viewport * mainviewport = NxEngine::getSingleton().GetNxViewport(); 
	//Log("Enabling Output Compositor...");
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled( mainviewport , "NxCompositorOutput" , true );
	//Log("Enabling Output Compositor:Done");

	static bool Initialized = false;

	if( !Initialized )
	{
		TexturePtr tester = TextureManager::getSingleton().createManual( "RTT_Texture_100",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
		32, 32, 0, Ogre::PF_BYTE_BGR, TU_RENDERTARGET  );

		mRenderTexture = tester->getBuffer(0,0)->getRenderTarget();
		mRenderTexture->setAutoUpdated( false );
		//Ogre::Viewport * NxViewport = mRenderTexture->addViewport( NxEngine::getSingleton().GetNxCamera() ); // view from main scene

		Ogre::Viewport * NxViewport = mRenderTexture->addViewport(  NxEngine::getSingleton().GetNxWindow()->GetViewport(0)->GetViewport()->getCamera( ) );
		Initialized = true;
	}
 
	NxScreen * Output = new NxScreen( MonitorID, FullScreen, Width, Height );
	MonitorListActive.push_back( Output );

	return Output ;
}

void NxScreenManager::DeleteExternalWindow( NxScreen * Output )
{
	MonitorListActive.remove( Output );
	delete Output ;
	Output = 0;
}

void NxScreenManager::DeleteWindows()
{
	for( std::list< NxScreen *>::iterator iter = MonitorListActive.begin(); iter != MonitorListActive.end(); iter++)
	{
		NxScreen * Window =  *iter; 
		MonitorListActive.remove( Window );
		delete Window ;
		Window = 0;
	}
}

void NxScreenManager::SetAnimationScroll( float U, float V )//o to 1
{
	for( std::list< NxScreen *>::iterator iter = MonitorListActive.begin(); iter != MonitorListActive.end(); iter++ )
	{
		NxScreen * Window =  *iter; 
		Window->GetTextureUnit()->setScrollAnimation( (Real)U , (Real)  V );
	}
}

void NxScreenManager::SetAnimationRotate( float Speed )//o to 1
{
	for( std::list< NxScreen *>::iterator iter = MonitorListActive.begin(); iter != MonitorListActive.end(); iter++)
	{
		NxScreen * Window =  *iter; 
		Window->GetTextureUnit()->setRotateAnimation( (Real) Speed );
	}
}

NxScreen & NxScreenManager::GetScreen( int Index )
{
	std::list< NxScreen * >::iterator i = MonitorListActive.begin();
	std::advance( i, Index );// now index points to the nth member of the list (counting from zero)
	return **i ;
}

unsigned int NxScreenManager::GetNumScreens()
{
	return MonitorListActive.size();
}

Nx::Matrix4 NxScreenManager::BuildScaledOrthoMatrix( float left, float right, float bottom, float top, float near_plane, float far_plane )
{

	Real divide = right - left ;
	Real invw = 1.0f / divide ;

	divide = top - bottom ;
	Real invh = 1.0f / divide;

	divide = far_plane  - near_plane ;
	Real invd =  1.0f / divide ;

	Nx::Matrix4 proj = Nx::Matrix4::ZERO;

	proj[0][0] = 2.0f * invw;
	proj[0][3] = -(right + left) * invw;
	proj[1][1] = 2.0f * invh;
	proj[1][3] = -(top + bottom) * invh;
	proj[2][2] = -2.0f * invd;
	proj[2][3] = -(far_plane + near_plane) * invd;
	proj[3][3] = 1.0f;

	return proj;
}


void Set_Default_Projection_Matrix( NxScreenManager * ptr  )
{
	ptr->RebuiltScreenOrder();
	std::list< NxScreen * > ActiveWindows;
	ptr->GetActiveWindowsList( ActiveWindows ); 
	for( std::list< NxScreen *>::iterator iter = ActiveWindows.begin(); iter != ActiveWindows.end(); iter++)
	{
		NxScreen * Window =  *iter; 
		Matrix4 matrix(1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1); 
		Vector3 scale( 1.0f ,1.0f,1.0f);
		matrix.setScale(scale);
		Vector3 trans(0.0f,0.0f,0.0f);
		matrix.setTrans(trans);
		Window->GetTextureUnit()->setTextureTransform( matrix ); 
	}
}

void Set_VPanoramic_Projection_Matrix( NxScreenManager * ptr  )//vertical panoramic 
{
	Real Screen_X_Offset = 1.0f /  (Real)( ptr->GetNumScreens() );
	Real Screen_X = 0.0f ;
	ptr->RebuiltScreenOrder();

	std::list< NxScreen * > ActiveWindows;
	ptr->GetActiveWindowsList( ActiveWindows ); 
	for( std::list< NxScreen *>::iterator iter = ActiveWindows.begin(); iter != ActiveWindows.end(); iter++)
	{
		NxScreen * Window =  *iter; 
		Matrix4 matrix(1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1); 
		Vector3 scale( 1.0f  , Screen_X_Offset , 1.0f );  
		matrix.setScale(scale);
		//Vector3 trans( -Screen_X ,0.0f,  0.0f );
		Vector3 trans(  0.0f, -Screen_X ,  0.0f );
		matrix.setTrans(trans);
		Window->GetTextureUnit()->setTextureTransform( matrix ); 
		Screen_X = Screen_X + Screen_X_Offset ;
	}
	return ;
}


void Set_Panoramic_Projection_Matrix( NxScreenManager * ptr )
{
	Real Screen_X_Offset = 1.0f / (Real)( ptr->GetNumScreens() ) ;
	Real Screen_X = 0.0f ;
	ptr->RebuiltScreenOrder();

	std::list< NxScreen * > ActiveWindows;
	ptr->GetActiveWindowsList( ActiveWindows );  
	for( std::list< NxScreen *>::iterator iter = ActiveWindows.begin(); iter != ActiveWindows.end(); iter++)
	{
		NxScreen * Window =  *iter; 
		Matrix4 matrix(1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1); 
		Vector3 scale( Screen_X_Offset ,1.0f , 1.0f );  

		matrix.setScale(scale);
		Vector3 trans( -Screen_X, 0.0f, 0.0f);
		matrix.setTrans(trans);
		Window->GetTextureUnit()->setTextureTransform( matrix ); 
		Screen_X = Screen_X + Screen_X_Offset ;
	}
	return ;
}

void NxScreenManager::SetMultiDisplayMode( NxGraphics_Display_Types DisplayType )
{
	switch( DisplayType  )
	{
		case WINDOW_STYLE_DEFAULT :
			mMultiDisplayMode = WINDOW_STYLE_DEFAULT;
			Set_Default_Projection_Matrix( this );
		break ;
		case WINDOW_STYLE_PANORAMIC :
			mMultiDisplayMode = WINDOW_STYLE_PANORAMIC;
			Set_Panoramic_Projection_Matrix( this  );
		break ;
		case WINDOW_STYLE_VPANORAMIC :
			mMultiDisplayMode = WINDOW_STYLE_VPANORAMIC;
			Set_VPanoramic_Projection_Matrix( this  );
		break ;
		case WINDOW_STYLE_STRETCHED :
			// to implement
			break ;
		default :
		break;
	}
}

NxGraphics_Display_Types NxScreenManager::GetMultiDisplayMode()
{
	return mMultiDisplayMode;
}

static bool Greater( const NxScreen * a, const NxScreen * b)
{
	if( a->ScreenPosX == b->ScreenPosX )
	{
		return a->ScreenPosX > b->ScreenPosX;
	}
	return a->ScreenPosX < b->ScreenPosX;
}

void NxScreenManager::RebuiltScreenOrder()
{
	MonitorListActive.sort( Greater );
}

NxMonitor * NxScreenManager::GetMonitor( int Index )
{
	NxMonitor * ret = NULL;
	if( Index < MonitorList.size()  ) ret = MonitorList[ Index ];
	return ret;
}

void NxScreenManager::GetMonitorList( std::vector< NxMonitor * > & MonList )
{
	MonList.resize(MonitorList.size());
	std::copy(MonitorList.begin(),MonitorList.end(),MonList.begin());
}

void NxScreenManager::GetActiveWindowsList( std::list< NxScreen *> & ScreenList )
{
	ScreenList.resize(MonitorListActive.size());
	std::copy(MonitorListActive.begin(),MonitorListActive.end(),ScreenList.begin());
}

void NxScreenManager::BuildMonitorsInfo()
{
 

}



} // namespace