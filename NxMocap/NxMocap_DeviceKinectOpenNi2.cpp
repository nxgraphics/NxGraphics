#include "NxMocapPreCompiled.h"
#include <OpenNI.h> // you need to install openni2 sdk

using namespace NxMocap_Namespace;

class KinectPlayerOpenNi
{
private :
	openni::Device device;
	openni::VideoStream depth, color, infra;
	openni::VideoFrameRef m_depthFrame;
	openni::VideoFrameRef m_colorFrame;
	openni::VideoFrameRef m_infraFrame;
	openni::VideoStream** m_streams;

	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;
	openni::VideoMode infraVideoMode;

	int m_width;
	int m_height;
	//long* m_colorCoordinates;

	mutable bool mHasNewDepth;
	mutable bool mHasNewColor;
	mutable bool mHasNewInfraRed;

	bool mUseColor;
	bool mUseDepth;
	bool mUseInfrared;
	std::vector< openni::VideoStream * > mStreams;
 
public :
	KinectPlayerOpenNi( bool UseColor, bool UseDepth, bool UseIr ) : 
	mUseColor(UseColor), mUseDepth(UseDepth), mUseInfrared(UseIr), mHasNewColor(false), mHasNewDepth(false), mHasNewInfraRed(false)
	{
	
	}

	bool CreateDepth()
	{
		openni::Status rc = depth.create(device, openni::SENSOR_DEPTH);
		if (rc == openni::STATUS_OK) {
			rc = depth.start();
			if (rc != openni::STATUS_OK) {
				printf("SimpleViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
				depth.destroy();
				return false;
			}
		}
		else {
			printf("SimpleViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			return false;
		}

		if (!depth.isValid()) {
			printf("Depth Invalid.\r");
			openni::OpenNI::shutdown();
			return false;
		}

		depthVideoMode = depth.getVideoMode();
 
		Log("Depth map width : " + NxMocapUtils::ToString( depthVideoMode.getResolutionX() ));
		Log("Depth map height : " + NxMocapUtils::ToString( depthVideoMode.getResolutionY()));
		Log("Depth map pixel format : " + NxMocapUtils::ToString( (int) depthVideoMode.getPixelFormat() ));

		//m_colorCoordinates = new long[depthVideoMode.getResolutionX()*depthVideoMode.getResolutionY()*2]; 

		

		printf("DepthMap Stream Created\n");

		mStreams.push_back( &depth );
		return true;
	
	}

	bool CreateInfraRed()
	{
		openni::Status rc = infra.create(device, openni::SENSOR_IR);
		if (rc == openni::STATUS_OK) {
			rc = infra.start();
			if (rc != openni::STATUS_OK) {
				printf("SimpleViewer: Couldn't start infra stream:\n%s\n", openni::OpenNI::getExtendedError());
				infra.destroy();\
				return false;
			}
		}
		else {
			printf("SimpleViewer: Couldn't find infra stream:\n%s\n", openni::OpenNI::getExtendedError());
			return false;
		}

		if (!infra.isValid()) {
			printf("Infrared Invalid.\r");
			openni::OpenNI::shutdown();
			return false;
		}

		infraVideoMode = infra.getVideoMode();
		Log("Infrared map width : " + NxMocapUtils::ToString( infraVideoMode.getResolutionX() ));
		Log("Infrared map height : " + NxMocapUtils::ToString( infraVideoMode.getResolutionY() ));
		Log("Infrared map pixel format : " + NxMocapUtils::ToString( (int) infraVideoMode.getPixelFormat() ));

		printf("InfraRed Stream Created\n");

		mStreams.push_back( &infra );
		return true;
	}

	bool CreateColor()
	{
		openni::Status rc = color.create(device, openni::SENSOR_COLOR);
		if (rc == openni::STATUS_OK) {
			rc = color.start();
			if (rc != openni::STATUS_OK) {
				printf("SimpleViewer: Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
				color.destroy();
				return false;
			}
		}
		else {
			printf("SimpleViewer: Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
			return false;
		}

		if ( !color.isValid() ) {
			printf("Color Stream Invalid\n");
			openni::OpenNI::shutdown();
			return false;
		}

		colorVideoMode = color.getVideoMode();
		Log("Color map width : " + NxMocapUtils::ToString( colorVideoMode.getResolutionX() ));
		Log("Color map height : " + NxMocapUtils::ToString( colorVideoMode.getResolutionY() ));
		Log("Color map pixel format : " + NxMocapUtils::ToString( (int) colorVideoMode.getPixelFormat() ));

		printf("Color Stream Created\n");

		mStreams.push_back( &color );
		return true;
	}


	bool initPrimeSensor()
	{
		openni::Status rc = openni::STATUS_OK;
		const char* deviceURI = openni::ANY_DEVICE;
 
		rc = openni::OpenNI::initialize();
		printf("After initialization:\n%s\n", openni::OpenNI::getExtendedError());

		rc = device.open(deviceURI);
		if (rc != openni::STATUS_OK) {
			printf("SimpleViewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
			openni::OpenNI::shutdown();
			return false;
		}

		if( mUseColor ) CreateColor();
		if( mUseDepth ) CreateDepth();
		if( mUseInfrared ) CreateInfraRed();

		if( mUseColor && mUseDepth )  {
			if(device.isImageRegistrationModeSupported( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR) ) {
				device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );
			 	Log("IMAGE_REGISTRATION_DEPTH_TO_COLOR supported !");
			 }else
			 {
			 	Log("IMAGE_REGISTRATION_DEPTH_TO_COLOR isnt supported !");
			
			 }
			 device.setDepthColorSyncEnabled(true);
		} 

		m_streams = new openni::VideoStream*[mStreams.size()];
		for(int i = 0 ; i < mStreams.size(); i++ ) {
			m_streams[i] = mStreams[i];
		}

		return true;
	}

	void Disconnect()
	{
		depth.destroy();
		color.destroy();
		infra.destroy();
		device.close();
	
	}
 
	bool Update() 
	{
		int changedIndex;
		openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, mStreams.size(), &changedIndex);
		if (rc != openni::STATUS_OK) {
			Log("Wait failed");
			return false;
		}

		openni::SensorType type =  mStreams[changedIndex]->getSensorInfo().getSensorType();

		switch ( type ) {
			case openni::SensorType::SENSOR_DEPTH  :
				depth.readFrame(&m_depthFrame); mHasNewDepth = true; break;
			case openni::SensorType::SENSOR_COLOR:
				color.readFrame(&m_colorFrame); mHasNewColor = true; break;
			case openni::SensorType::SENSOR_IR  : 
				infra.readFrame(&m_infraFrame); mHasNewInfraRed = true; break;	
			default:
				Log("Error in wait");
		}	
 
		return true;
 
	}

	bool UseInfraRed()
	{
		return mUseInfrared;
	}

	bool HasNewDepth() {
		return mHasNewDepth;
	}
 
	bool HasNewColor() {
		return mHasNewColor;
	}

	bool HasNewInfraRed() {
		return mHasNewInfraRed;
	}

	const NxMocapImageDepth * GetDepthMap() const
	{
		mHasNewDepth = false;
		return (NxMocapImageDepth*) m_depthFrame.getData();
	}

	const NxMocapImageRGB * GetColorMap() const
	{
		mHasNewColor = false;
		return (NxMocapImageRGB *)m_colorFrame.getData();
	}

	void ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY )
	{
		openni::CoordinateConverter::convertDepthToColor(depth,color,depthX,depthY, (openni::DepthPixel) DepthZ, (int*) ColorX, (int*) ColorY);
	}

	bool GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const
	{
		if( !depth.isValid() || !color.isValid() ) return false; 
		for(int y = 0; y < 480; y++) {
			for(int x = 0; x < 640; x++) {
				int BaseByte = x + y * 640;
				openni::CoordinateConverter::convertDepthToColor(depth,color,x,y, src[BaseByte], (int *) &dst[BaseByte * 2 ], (int *) &dst[BaseByte * 2 + 1] );
			}
		}
		return true;
	}
	
	const NxMocapImageIr * GetInfraRedMap() const
	{
		mHasNewInfraRed = false;
		return  (NxMocapImageIr*) m_infraFrame.getData();
	}

	// nite2 FUNCTIONS
	bool IsTracking( int UserID )
	{

		

		return true;

	}
	unsigned int GetNumUsers() {

		return 0;
	}

	NxMocapUser * GetUser( int Index )
	{
		return new NxMocapUser(0);
	}

};

namespace NxMocap_Namespace {

NxMocapDeviceKinectOpenNi::NxMocapDeviceKinectOpenNi() : NxMocapDeviceKinect()  {

}

NxMocapDeviceKinectOpenNi::~NxMocapDeviceKinectOpenNi(){

}

bool NxMocapDeviceKinectOpenNi::ConnectDevice()
{
	return ConnectDevice( true,true,true );
}

bool NxMocapDeviceKinectOpenNi::ConnectDevice( bool useColor, bool useDepth, bool useInfrared ) {

	Log("Connecting Kinect..");
	mKinect = new KinectPlayerOpenNi( useColor, useDepth, useInfrared );
	if( !mKinect->initPrimeSensor()   ){ Log(" Connecting Kinect Failed !"); return false; }
	Log("Connecting Kinect Success");
	return true;
}

bool NxMocapDeviceKinectOpenNi::DisconnectDevice(){
	mKinect->Disconnect();
	return true;
}

bool NxMocapDeviceKinectOpenNi::IsUsingInfraRedMap()
{
	return mKinect->UseInfraRed();
}

bool NxMocapDeviceKinectOpenNi::HasNewDepth() {
	return mKinect->HasNewDepth();
}

bool NxMocapDeviceKinectOpenNi::HasNewColor() {
	return mKinect->HasNewColor();
}

bool NxMocapDeviceKinectOpenNi::HasNewInfraRed() {
	return mKinect->HasNewInfraRed();
}

const NxMocapImageDepth * NxMocapDeviceKinectOpenNi::GetDepthMap() const
{
	return mKinect->GetDepthMap();
}

const NxMocapImageRGB * NxMocapDeviceKinectOpenNi::GetImageMap() const
{
	return mKinect->GetColorMap();
}

const NxMocapImageIr * NxMocapDeviceKinectOpenNi::GetInfraRedMap() const
{
	return mKinect->GetInfraRedMap();
}

void NxMocapDeviceKinectOpenNi::ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY )
{
	mKinect->ConvertDepthToColor( depthX, depthY, DepthZ, ColorX, ColorY );
}

bool NxMocapDeviceKinectOpenNi::GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const
{
	return mKinect->GetDepthToColorMap( src, dst );
}

bool NxMocapDeviceKinectOpenNi::Update(){

	return mKinect->Update();
}

}