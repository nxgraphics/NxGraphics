#include "NxVideoPreCompiled.h"

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
#include "../NxDeps/include/VideoInput/videoInput.h"
#include "../NxDeps/include/FlyCapture/include/PGRFlyCapture.h"
#endif
 
namespace NxVideo_Namespace {

template<> NxVideoManager * Singleton<NxVideoManager>::msSingleton = 0;


#if USE_QUICKTIME
static const long minimumQuickTimeVersion = 0x07300000; // 7.3
static SInt32 quickTimeVersion = 0;
//static bool QuickTimeEnabled = false;

void MyMoviesErrorProc( OSErr theErr, long theRefcon ) {
	Log("Quicktime Error : " + NxVideoUtils::ToString( theErr ) ); 
}
#endif
bool initializeQuickTime() 
{
	#if USE_QUICKTIME
    //static bool initialized = false;
    static bool initializationSucceeded = false;
    //if (!initialized) 
	//{
		//initialized = true;
		// Initialize and check QuickTime version // Generates an error if QTCF.dll isnt found. ( to fix )
		OSErr result;
		#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
		result = InitializeQTML( kInitializeQTMLEnableDoubleBufferedSurface );
		if( result != noErr ) { Log("No QuickTime available. Disabling QT Video and Audio support."); TerminateQTML(); return false; }
		#endif

		result = Gestalt(gestaltQuickTime, &quickTimeVersion);
		if( result != noErr ) { Log("Can't find QuickTime version."); }

        if (quickTimeVersion < minimumQuickTimeVersion) {
			Log("QuickTime version " + NxVideoUtils::ToString( (unsigned long)quickTimeVersion ) + " detected, at least " + NxVideoUtils::ToString( (unsigned long)minimumQuickTimeVersion ) + " required. Disabling <video> and <audio> support."  );
            return false;
        }

		int Low, Mid, High;
		High = ((quickTimeVersion & 0xff000000) >> 24);
		Mid = ((quickTimeVersion & 0x00ff0000) >> 16);
		Low = ((quickTimeVersion & 0x0000ff00) >> 8);

		Log("QuickTime version " + NxVideoUtils::ToString( High ) + "." + NxVideoUtils::ToString( Mid ) + "." + NxVideoUtils::ToString( Low ) );
 
   
		if (EnterMovies() != noErr)  //Initializes the Movie Toolbox and creates a private storage area for your application.
		{
            Log("No QuickTime available. EnterMovies Failed.");
            return false;
        }

        initializationSucceeded = true;
		SetMoviesErrorProc( MyMoviesErrorProc, ( long ) 0 ); // this
		 

    //}
    return initializationSucceeded;
#endif
	return false;
}

NxVideoManager::NxVideoManager()
{
	new NxVideoLog();
	mQuicktimeEnabled = initializeQuickTime();

	Log("Started NxVideo.");
	
}

NxVideoManager::~NxVideoManager(void)
{
#if USE_QUICKTIME
	ExitMovies();
	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
	TerminateQTML();
	#endif
#endif
	
}

bool NxVideoManager::IsQuicktimeEnabled()
{
	return mQuicktimeEnabled;
}

NxVideo_Player * NxVideoManager::OpenVideoFile( const std::string& FilePath, NxVideoPixelFormatInfo & info )
{
	std::string Tmp = FilePath;
	NxVideoUtils::ToLower( Tmp );

	std::string Extension = Tmp.substr(Tmp.find_last_of(".") + 1);

	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32 || NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_APPLE
	if( Extension == "mov" | Extension == "sdp" | Extension == "mp4" ) // avi
	{
		NxVideo_Mov_Player * File = new NxVideo_Mov_Player();  
		if(!File->OpenVideoFile( Tmp, info )) { 
			delete File;
			return 0;
		}
		return File;
	} 
	#endif
	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
	if( Extension == "avi" )// mov
	{
		NxVideo_Avi_Player * File = new NxVideo_Avi_Player() ; 
		if(!File->OpenVideoFile( Tmp, info )) {
			delete File;		
			return 0;
		}
		return File; 
	}
	#endif
	if( Extension == "ogv" )// mov
	{
		NxVideo_Theora_Player * File = new NxVideo_Theora_Player() ; 
		if(!File->OpenVideoFile( Tmp, info )) {
			delete File;		
			return 0;
		}
		return File; 
	}

	 
	Log("NxVideoManager::OpenVideoFile : File Format not supported.");
	return 0;
	 
}

bool NxVideoManager::CloseVideoFile( NxVideo_Player * File )
{

	Log("NxVideoManager::CloseVideoFile...");
	File->CloseVideoFile();
	delete File;
	File = 0;
	return true;
}

void NxVideoManager::GetAvailableCameraNames( std::vector<std::string> & CameraList, NxVideoCaptureDeviceType CameraType )
{

	if( CameraType == NxVideoCaptureQuicktime )
	{
	
	}
	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
	else if( CameraType == NxVideoCaptureDS )
	{
		videoInput * Input_Parser = new videoInput();
		std::vector<std::string> CamList;
		for( int i = 0 ; i < Input_Parser->listDevices( true ); i++ ) {
			char * Name = Input_Parser->getDeviceName( i );
			CamList.push_back(Name);
		}
		delete Input_Parser;
		Input_Parser = 0;
		for( int i = 0 ; i < CamList.size() ; i++  ){
			CameraList.push_back( CamList[i] ); Log( "NxVideo: Listing DirectShow cameras :" +  CamList[i] );
		}
	
	}
	else if( CameraType == NxVideoCapturePGR )
	{
		unsigned int uiPGRCameraBusCount = 0 ;
		FlyCaptureError HR = flycaptureBusCameraCount(&uiPGRCameraBusCount);
		FlyCaptureInfoEx aInfo[255];
		unsigned int uiPGRCameraCount = uiPGRCameraBusCount;//Num;
		HR = flycaptureBusEnumerateCamerasEx(aInfo, &uiPGRCameraCount);
		if ( HR != FLYCAPTURE_OK){ Log("NxVideo: No PGR Cameras Found"); return; }
		else{ Log("NxVideo: Found PGR Cameras...");  }
		for(int i = 0 ; i < uiPGRCameraBusCount ; i++){ CameraList.push_back( NxVideoUtils::ToString( aInfo[i].SerialNumber )); }
	}
	else if( CameraType == NxVideoCaptureVFW )
	{
		char devicename[80];
		char deviceversion[80];
		for (int index = 0; index < 10; index++ ) { // MAX allowed in VFW is 10
			bool ok = 0!= capGetDriverDescription( index, devicename, sizeof(devicename), deviceversion, sizeof(deviceversion));
			if (ok){ CameraList.push_back(std::string( devicename )); }
			else
				break; // no more devices
		}
	}
	#endif
} 


NxVideo_Grabber * NxVideoManager::OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP, NxVideoCaptureDeviceType CameraType )
{
	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
	if( CameraType == NxVideoCaptureDS ) {
		NxVideoGrabberDirectshow * Ptr = new NxVideoGrabberDirectshow();
		if(!Ptr->OpenVideoDevice( Index, Width, Height, BPP )) {
			Log("Could not opend directshow device capture");
		}
		return Ptr;
	}
	#endif

	return 0;
}


unsigned long NxVideoManager::GetCameraIndexFromName( const std::string & CameraName, NxVideoCaptureDeviceType CameraType )
{
	unsigned long DevIndex = -1;

	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
	if( CameraType == NxVideoCaptureDS )
	{
		videoInput * Input_Parser = new videoInput();
		for( int i = 0 ; i < Input_Parser->listDevices( true ); i++ ) {
			std::string DevName( Input_Parser->getDeviceName( i ));
			if( DevName.compare(CameraName)==0) return i;
		}
		delete Input_Parser ;
	}
	else if( CameraType == NxVideoCaptureVFW )
	{
		char devicename[80];
		char deviceversion[80];
		for (int index = 0; index < 10; index++ ) // MAX allowed in VFW is 10
		{
			bool ok = 0!= capGetDriverDescription( index, devicename, sizeof(devicename), deviceversion, sizeof(deviceversion));
			if (ok)
			{	std::string dev( devicename ) ;
				if( dev.compare( CameraName ) == 0 )
				{
					DevIndex = index ;
				}	
			}
			else
				break; // no more devices
		}
	}
	#endif

	return DevIndex ;
}

NxVideoManager& NxVideoManager::getSingleton(void)
{
	assert( msSingleton );  return ( *msSingleton);
}

NxVideoManager* NxVideoManager::getSingletonPtr()
{
	return msSingleton;
}

}//namespace







 




 



