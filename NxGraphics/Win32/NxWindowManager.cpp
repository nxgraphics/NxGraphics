#include "NxGraphicsPreCompiled.h"
#include <tchar.h>

namespace Nx {

template<> NxScreenManager* NxSingleton<NxScreenManager>::ms_Singleton = 0;


//=======================================
	static const int Total_Max_Screens = 20 ;// the engine can support maximum up to 20 output screen.	
	NxMonitorInfo * Monitor_Info[Total_Max_Screens];
 //=======================================

Ogre::String szClassName;
LPCTSTR szWindowText  ;
HINSTANCE hThisInstance;


NxScreenManager::NxScreenManager() : mRenderTexture( 0 )
{
	mMultiDisplayMode = WINDOW_STYLE_DEFAULT;
	HMODULE hThisInstance = GetModuleHandle(0);
	szClassName = "NxScreenClass" ;

	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_OWNDC; // CS_HREDRAW | CS_VREDRAW | 

	wcx.lpfnWndProc =  (WNDPROC)  Ogre::WindowEventUtilities::_WndProc;
	wcx.cbClsExtra  = 0;
	wcx.cbWndExtra  = 0;
	wcx.hInstance  = hThisInstance;
	wcx.hIcon   = NULL;
	wcx.hCursor  = NULL;
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "NxScreenClass";
	wcx.hIconSm  = NULL;
 
	if ( !RegisterClassEx (&wcx) )
	{
		MessageBox(0,"NxGraphics : Window Class Failed to Register !","",0);
		return  ;
	}

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
	HMODULE mInstance = GetModuleHandle(0);
	if(!mInstance) LogMsg("Cant get CreateWindowHolder Instance" );

	HWND mWindow = CreateWindowEx( WS_EX_TOPMOST, (LPCSTR) szClassName.c_str(), Monitor->GetMonitorName().c_str() , WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, 
		0, 0, Monitor->GetWidth(), Monitor->GetHeight(), NULL, NULL, mInstance, NULL );
	if( !mWindow ) return 0;

	//set an ICON
	//LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_NXGRAPHICS) );
	//SetClassLong( Tmp_Hwnd , GCL_HICON, iconID );
	//WNDPROC mOldProc = (WNDPROC)SetWindowLong( Tmp_Hwnd , GWL_WNDPROC, (LONG)VideoWndProc); 

	if( Fullscreen )// remove styles
	{
 		DWORD style;
		RECT  rect;
		style = GetWindowLong( mWindow, GWL_STYLE);
		style&=~WS_CAPTION   ; 
		style&=~WS_THICKFRAME ; 
		AdjustWindowRect (&rect, style, FALSE);
		SetWindowLong( mWindow, GWL_STYLE, style);
	}

	return mWindow ;
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
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TextureType::TEX_TYPE_2D,
		32, 32, 0, Ogre::PF_BYTE_BGR, TU_RENDERTARGET  );

		mRenderTexture = tester->getBuffer(0,0)->getRenderTarget();
		mRenderTexture->setAutoUpdated( false );
		//Ogre::Viewport * NxViewport = mRenderTexture->addViewport( NxEngine::getSingleton().GetNxCamera() ); // view from main scene

		Ogre::Viewport * NxViewport = mRenderTexture->addViewport( NxEngine::getSingleton().GetNxWindow()->GetViewport(0)->GetViewport()->getCamera( ) );
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
		Ogre::Matrix4 matrix(1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1); 
		Ogre::Vector3 scale( 1.0f ,1.0f,1.0f);
		matrix.setScale(scale);
		Ogre::Vector3 trans(0.0f,0.0f,0.0f);
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
		Ogre::Matrix4 matrix(1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1); 
		Ogre::Vector3 scale( 1.0f  , Screen_X_Offset , 1.0f );  
		matrix.setScale(scale);
		//Vector3 trans( -Screen_X ,0.0f,  0.0f );
		Ogre::Vector3 trans(  0.0f, -Screen_X ,  0.0f );
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
		Ogre::Matrix4 matrix(1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1); 
		Ogre::Vector3 scale( Screen_X_Offset ,1.0f , 1.0f );  

		matrix.setScale(scale);
		Ogre::Vector3 trans( -Screen_X, 0.0f, 0.0f);
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
	TCHAR msg[10000] = _T("");

	DISPLAY_DEVICE dd;
	dd.cb = sizeof(dd);
	DWORD dev = 0; // device index
	int id = 1; // monitor number, as used by Display Properties > Settings

	while( EnumDisplayDevices(0, dev, &dd, 0) )
	{
		if (!(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))// ignore virtual mirror displays
		{
			// get information about the monitor attached to this display adapter. dualhead cards
			// and laptop video cards can have multiple monitors attached

			DISPLAY_DEVICE ddMon;
			ZeroMemory(&ddMon, sizeof(ddMon));
			ddMon.cb = sizeof(ddMon);
			DWORD devMon = 0;

			// please note that this enumeration may not return the correct monitor if multiple monitors
			// are attached. this is because not all display drivers return the ACTIVE flag for the monitor
			// that is actually active
			while (EnumDisplayDevices(dd.DeviceName, devMon, &ddMon, 0))
			{
				if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE){break;}

				devMon++;
			}

			if (!*ddMon.DeviceString)
			{
				EnumDisplayDevices(dd.DeviceName, 0, &ddMon, 0);
				if (!*ddMon.DeviceString)
				{lstrcpy(ddMon.DeviceString, _T("Default Monitor"));}
			}

			// get information about the display's position and the current display mode
			DEVMODE dm;
			ZeroMemory(&dm, sizeof(dm));
			dm.dmSize = sizeof(dm);

			if (EnumDisplaySettingsEx(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm, 0) == FALSE)
			{EnumDisplaySettingsEx(dd.DeviceName, ENUM_REGISTRY_SETTINGS, &dm, 0);}

			// get the monitor handle and workspace
			HMONITOR hm = 0;
			MONITORINFO mi;
			ZeroMemory(&mi, sizeof(mi));
			mi.cbSize = sizeof(mi);

			if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
			{
				// display is enabled. only enabled displays have a monitor handle
				POINT pt = { dm.dmPosition.x, dm.dmPosition.y };
				hm = MonitorFromPoint(pt, MONITOR_DEFAULTTONULL);
				if (hm){::GetMonitorInfo(hm, &mi);}
			}

			// format information about this monitor
			TCHAR buf[1000];

			Monitor_Info[id-1] = new NxMonitorInfo;
			//Monitor_Info[id-1]->Is_Monitor_OGRE_Use = false ;
			//Monitor_Info[id-1]->User_Input_Is_Fullscreen = false ;
			Monitor_Info[id-1]->Is_Disabled = false ;
			Monitor_Info[id-1]->Is_Primary  = false ;
			Monitor_Info[id-1]->Is_Removable = false ;
			
			Monitor_Info[id-1]->id = id ;
			strcpy(  Monitor_Info[id-1]->Monitor_Name , ddMon.DeviceString );
			strcpy(  Monitor_Info[id-1]->Adapter_Name,  dd.DeviceString );
 
			// 1. MyMonitor on MyVideoCard
			wsprintf(buf, _T("%d. %s on %s\r\n"), id, ddMon.DeviceString, dd.DeviceString);
			lstrcat(msg, buf);

			// status flags: primary, disabled, removable
			buf[0] = _T('\0');

			if (!(dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
			{
				lstrcat(buf, _T("disabled, "));
				Monitor_Info[id-1]->Is_Disabled = true ;
			}
			else if ( dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE )
			{
				lstrcat(buf, _T("primary, "));
				Monitor_Info[id-1]->Is_Primary = true ;
			}
			if ( dd.StateFlags & DISPLAY_DEVICE_REMOVABLE )
			{
				lstrcat(buf, _T("removable, "));
				Monitor_Info[id-1]->Is_Removable = true ;
			}

			if (*buf)
			{
				buf[lstrlen(buf) - 2] = _T('\0');
				lstrcat(buf, _T("\r\n"));
				lstrcat(msg, buf);
			}

			Monitor_Info[id-1]->Width = dm.dmPelsWidth ;
			Monitor_Info[id-1]->Height = dm.dmPelsHeight ;
			Monitor_Info[id-1]->Pos_X = dm.dmPosition.x ;
			Monitor_Info[id-1]->Pos_Y = dm.dmPosition.y ;
			Monitor_Info[id-1]->Bits_Per_Pixel = dm.dmBitsPerPel;
			Monitor_Info[id-1]->Refresh_Rate = dm.dmDisplayFrequency;

			// width x height @ x,y - bpp - refresh rate
			// note that refresh rate information is not available on Win9x
			wsprintf(buf, _T("%d x %d @ %d,%d - %d-bit - %d Hz\r\n"),
				dm.dmPelsWidth, dm.dmPelsHeight,
				dm.dmPosition.x, dm.dmPosition.y, 
				dm.dmBitsPerPel, dm.dmDisplayFrequency);
			lstrcat(msg, buf);

			if (hm)
			{
				// workspace and monitor handle
				// first point
				Monitor_Info[id-1]->Workspace_X[0] = mi.rcWork.left ;
				Monitor_Info[id-1]->Workspace_X[1] = mi.rcWork.top ;
				// second point
				Monitor_Info[id-1]->Workspace_Y[0] =  mi.rcWork.right ;
				Monitor_Info[id-1]->Workspace_Y[1] =  mi.rcWork.bottom ;
				// HMonitor
				Monitor_Info[id-1]->Monitor_ID = hm ;

	
				// workspace: x,y - x,y HMONITOR: handle
				wsprintf(buf, _T("workspace: %d,%d - %d,%d HMONITOR: 0x%X\r\n"),
					mi.rcWork.left,mi.rcWork.top,
					mi.rcWork.right, mi.rcWork.bottom, hm);
				lstrcat(msg, buf);
			}

			strcpy( Monitor_Info[id-1]->Device_Name ,  *ddMon.DeviceName ? ddMon.DeviceName : dd.DeviceName);
			wsprintf(buf, _T("Device: %s\r\n\r\n"), *ddMon.DeviceName ? ddMon.DeviceName : dd.DeviceName);
			lstrcat(msg, buf);
			MonitorList.push_back( new NxMonitor( Monitor_Info[id-1] ) );
			id++;

		}//DISPLAY_DEVICE_MIRRORING_DRIVER

		dev++;
	}//while (EnumDisplayDevices

}



} // namespace