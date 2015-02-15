#include "NxVideoPreCompiled.h"

#include "../NxDeps/include/FlyCapture/include/PGRFlyCapture.h"
#include "../NxDeps/include/FlyCapture/include/PGRFlyCapturePlus.h"

namespace NxVideo_Namespace {

struct NxVideoDevicePGR
{
public:

	FlyCaptureContext * Context;
	FlyCaptureImagePlus ImagePlus;
	FlyCaptureVideoMode VideoMode   ;
	FlyCaptureFrameRate FrameRate   ;
	FlyCaptureError HR ;
	FlyCaptureImage image_01;
	FlyCaptureImage image_01_BGR;  
	unsigned long _ulSleepOnTimeout;
    unsigned int  _uiLastSeqNum;
    unsigned char *_pLoadBuffer0;
    unsigned char *_pLoadBuffer1;
	bool BrightnessAuto ;

	NxVideoDevicePGR() : Context( 0 )
	{

	}

	~NxVideoDevicePGR()
	{

	}

}; 
 
NxVideoGrabberPointGrey::NxVideoGrabberPointGrey() : NxVideo_Grabber() //, BrightnessAuto(false)
{
	mType = NxVideoCapturePGR ;
	mVideo = new NxVideoDevicePGR();
}

NxVideoGrabberPointGrey::~NxVideoGrabberPointGrey()
{
	FlyCaptureError HR = flycaptureStop( *mVideo->Context );
	if ( HR != FLYCAPTURE_OK)
	{  
		return;
	}
	delete mVideo->Context ;
	mVideo->Context = NULL;
	free( &mVideo->image_01 ) ;
	free( &mVideo->image_01_BGR ) ; 
}


bool NxVideoGrabberPointGrey::OpenVideoDevice( unsigned long SerialNumber, int Width, int Height, int BPP )
{
	unsigned int uiPGRCameraCount;
	FlyCaptureError HR = flycaptureBusCameraCount(&uiPGRCameraCount);
	if ( HR != FLYCAPTURE_OK)
	{   //Ogre::LogManager::getSingleton().logMessage("NxVideo: flycaptureBusCameraCount Failed");                 
		return  false;
	}
	//Ogre::LogManager::getSingleton().logMessage("NxVideo : Found "+ Ogre::StringConverter::toString(uiPGRCameraCount) + " PGR cameras");

	mVideo->Context = new FlyCaptureContext();
	HR = flycaptureCreateContext( mVideo->Context );
	if ( HR != FLYCAPTURE_OK)
	{   //Log("NxVideo: flycaptureCreateContext Failed");                 
		return  false;
	}

	//initialize from serial number
	HR = flycaptureInitializeFromSerialNumber( *mVideo->Context , SerialNumber );
	if ( HR != FLYCAPTURE_OK)
	{   //Log("NxVideo: flycaptureInitializeFromSerialNumber Failed");                 
		return  false;
	}
	
	 /*
	//initialize from bus index
	HR = flycaptureInitialize(*Context, 0);
	if ( HR != FLYCAPTURE_OK)
	{   Log("NxVideo: flycaptureInitialize Failed");                 
		return  false;
	}
	*/
	 
	  // Flip the image horizontally
	bool bFlipHorizontal = false ;
	unsigned long ulRegister = 0x1048;
	unsigned long ulValue, ulValueNew;
	if (bFlipHorizontal)
	{
		flycaptureGetCameraRegister( *mVideo->Context , 0x1048, &ulValue);
		ulValueNew = (ulValue | 0x00000100);
		FlyCaptureError fce = flycaptureSetCameraRegister( *mVideo->Context , 0x1048,  ulValueNew);
		flycaptureGetCameraRegister( *mVideo->Context , 0x1048, &ulValueNew);
	}
	else
	{
		flycaptureGetCameraRegister( *mVideo->Context , 0x1048, &ulValue);
		ulValueNew = (ulValue & 0xFFFFFEFF);
		FlyCaptureError fce = flycaptureSetCameraRegister( *mVideo->Context , 0x1048,  ulValueNew);
		flycaptureGetCameraRegister( *mVideo->Context, 0x1048, &ulValueNew);
	}
	// Set the timeouts
	// HR = flycaptureSetGrabTimeoutEx(* Context , ulTimeout);
	//640x480 8-bit greyscale or bayer tiled color image.
	//FlyCaptureVideoMode Mode;
	//FlyCaptureFrameRate Rate;
	//	Mode = ( FlyCaptureVideoMode ) FLYCAPTURE_VIDEOMODE_640x480Y8;    //FLYCAPTURE_VIDEOMODE_CUSTOM ;
	//	Rate = ( FlyCaptureFrameRate ) FLYCAPTURE_FRAMERATE_60 ;
	 
	mVideo->VideoMode = FLYCAPTURE_VIDEOMODE_640x480Y8;
	mVideo->FrameRate = FLYCAPTURE_FRAMERATE_60;
	// Start capturing images
	flycaptureStop( *mVideo->Context );

	memset( &mVideo->image_01, 0x0, sizeof( FlyCaptureImage ) );
	memset( &mVideo->image_01_BGR, 0x0, sizeof( FlyCaptureImage ) );

    mVideo->image_01_BGR.pixelFormat = FLYCAPTURE_BGR ;
	mVideo->image_01_BGR.pData=new unsigned char[Width*Height*3];

	HR = flycaptureStart( *mVideo->Context , ( FlyCaptureVideoMode ) mVideo->VideoMode , ( FlyCaptureFrameRate ) mVideo->FrameRate );
	if ( HR != FLYCAPTURE_OK)
	{   
		//Log("nxvideo: could not start firefly camera");
		return false;
	}

	FlyCaptureError error = flycaptureGrabImage2( *mVideo->Context  , ( FlyCaptureImage *) &mVideo->image_01 );
	error = flycaptureConvertImage( *mVideo->Context   ,( const FlyCaptureImage *) &mVideo->image_01 ,  ( FlyCaptureImage *) &mVideo->image_01_BGR);  
	//Log("nxvideo: firefly camera started successfully"); 
	//Initialized = true ;
	return true;

}

bool NxVideoGrabberPointGrey::CloseVideoDevice()
{
	return true;
}


unsigned char * NxVideoGrabberPointGrey::GetBuffer()
{
	if( mVideo->Context )
	{
		FlyCaptureError HR = flycaptureGrabImage2( *mVideo->Context, ( FlyCaptureImage *)&mVideo->image_01 );
		if ( HR != FLYCAPTURE_OK)
		{   //Log("NxVideo: flycaptureGrabImage2 Failed");                 
			return  0;
		}
		HR = flycaptureConvertImage( *mVideo->Context ,( const FlyCaptureImage *)&mVideo->image_01 , ( FlyCaptureImage *) &mVideo->image_01_BGR);
		if ( HR != FLYCAPTURE_OK)
		{  // Log("NxVideo: flycaptureGrabImage2 Failed");                 
			return  0;
		}

		return (unsigned char * )mVideo->image_01_BGR.pData ;
	}
	else
	{
		//Log("NxVideo: Context Failed"); 
		return 0;
	}

	return 0;
}

bool NxVideoGrabberPointGrey::NewFrame()
{
	return true;
}

/*
bool NxVideoGrabberFireFly::GetLatestImage(unsigned char **ppBuffer, unsigned int *pnImageIncrement)
{
	// Release previous image buffer (if applicable)
	flycaptureUnlock( *Context ,  ImagePlus.uiBufferIndex);

	// Grab the latest image
	HR = (  NxVideoGrabberFireFly::NxFlyCaptureError )  flycaptureLockLatest(* Context, ( FlyCaptureImagePlus * ) &ImagePlus );
	if (HR != FLYCAPTURE_OK)
	{
		if (HR == FLYCAPTURE_TIMEOUT)
		{
		   // _pLogger->LogEx(LL_DEBUG, "Timeout when grabbing latest image");
			if (_ulSleepOnTimeout > 0) Sleep(_ulSleepOnTimeout);
			*pnImageIncrement = 0;
			return false;
		}
		else
		{
			//_pLogger->LogEx(LL_ERROR, "Failed to grab latest image (%s)", flycaptureErrorToString(_fce));
			return false;
		}
	}

	// Image pointer
	*ppBuffer =  ImagePlus.image.pData;

	// Number of skipped images
	*pnImageIncrement = ( ImagePlus.uiSeqNum) - _uiLastSeqNum;
	if (*pnImageIncrement < 0) *pnImageIncrement += 2^32;
	_uiLastSeqNum =  ImagePlus.uiSeqNum;

	// Done
	return true;
}


unsigned long GetImageSize( NxFlyCaptureVideoMode fcvm )
{
	unsigned long nBytes = 0;
	switch (fcvm)
	{
		case FLYCAPTURE_VIDEOMODE_640x480Y8:
		nBytes = 640*480*1;
		break;
		case FLYCAPTURE_VIDEOMODE_640x480Y16:
		nBytes = 640*480*2;
		break;
		default:
		nBytes = 0;
		break;
	}

	return nBytes;
}


void NxVideoGrabberFireFly::SetCameraBrightness( float Value )
{
	if(flycaptureSetCameraProperty(*Context, FLYCAPTURE_BRIGHTNESS, Value  , Value, false ) != FLYCAPTURE_OK)
	{
		//Ogre::LogManager::getSingleton().logMessage("NxVideo: PGR capture FLYCAPTURE_BRIGHTNESS failed"); 
	}
}

void NxVideoGrabberFireFly::SetCameraGain( float Value )
{
	if(flycaptureSetCameraProperty(*Context, FLYCAPTURE_GAIN , Value  , Value, false ) != FLYCAPTURE_OK)
	{
		//Ogre::LogManager::getSingleton().logMessage("NxVideo: PGR capture FLYCAPTURE_GAIN failed"); 
	}
}

void NxVideoGrabberFireFly::SetCameraShutterSpeed( float Value )
{
	if(flycaptureSetCameraProperty(*Context, FLYCAPTURE_SHUTTER , Value  , Value, false ) != FLYCAPTURE_OK)
	{
		//Ogre::LogManager::getSingleton().logMessage("NxVideo: PGR capture FLYCAPTURE_SHUTTER failed"); 
	}
}

void NxVideoGrabberFireFly::SetCameraExposure( float Value )
{
	if(flycaptureSetCameraProperty(*Context, FLYCAPTURE_AUTO_EXPOSURE , Value  , Value, false ) != FLYCAPTURE_OK)
	{
		//Ogre::LogManager::getSingleton().logMessage("NxVideo: PGR capture FLYCAPTURE_AUTO_EXPOSURE failed"); 
	}
}
*/


}//namespace