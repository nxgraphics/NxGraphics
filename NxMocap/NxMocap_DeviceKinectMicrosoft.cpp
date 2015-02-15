#include "NxMocapPreCompiled.h"
#include <Shlobj.h>
#include "NuiApi.h"

using namespace NxMocap_Namespace;

class KinectPlayerMicrosoft
{
private :
 

	mutable bool mHasNewDepth;
	mutable bool mHasNewColor;
	mutable bool mHasNewInfraRed;

	bool mUseColor;
	bool mUseDepth;
	bool mUseInfrared;


	// Current Kinect
	INuiSensor*             m_pNuiSensor;
	HANDLE                  m_pSkeletonStreamHandle;
	HANDLE                  m_hNextSkeletonEvent;

	static const int        cBytesPerPixel    = 3; // 4

	static const NUI_IMAGE_RESOLUTION cDepthResolution = NUI_IMAGE_RESOLUTION_640x480;//NUI_IMAGE_RESOLUTION_320x240;

	// green screen background will also be scaled to this resolution
	static const NUI_IMAGE_RESOLUTION cColorResolution = NUI_IMAGE_RESOLUTION_640x480;

	//static const int        cStatusMessageMaxLen = MAX_PATH*2;

	HANDLE                  m_pDepthStreamHandle;
	HANDLE                  m_hNextDepthFrameEvent;

	HANDLE                  m_pColorStreamHandle;
	HANDLE                  m_hNextColorFrameEvent;

	LONG                    m_depthWidth;
	LONG                    m_depthHeight;

	LONG                    m_colorWidth;
	LONG                    m_colorHeight;

	LONG                    m_colorToDepthDivisor;

	USHORT*                 m_depthD16;
	USHORT*                 m_depthD16_NoPlayer;
	BYTE*                   m_colorRGBX;
	BYTE*                   m_backgroundRGBX;
	BYTE*                   m_outputRGBX;
	LONG*                   m_colorCoordinates;

	LARGE_INTEGER           m_depthTimeStamp;
	LARGE_INTEGER           m_colorTimeStamp;

	NUI_DEPTH_IMAGE_POINT * depthPoints ;//= new NUI_DEPTH_IMAGE_POINT[640 * 480];
	 INuiCoordinateMapper* pMapper;
 

public :
	KinectPlayerMicrosoft( bool UseColor, bool UseDepth, bool UseIr ) : 
		mUseColor(UseColor), mUseDepth(UseDepth), mUseInfrared(UseIr), mHasNewColor(false), mHasNewDepth(false), mHasNewInfraRed(false)
	{





	}

	bool CreateDepth()
	{
		 
		return true;

	}

	bool CreateInfraRed()
	{
	 
		return true;
	}

	bool CreateColor()
	{
 
		return true;
	}


	bool initPrimeSensor()
	{

		INuiSensor * pNuiSensor;
 
		int iSensorCount = 0;
		HRESULT hr = NuiGetSensorCount(&iSensorCount);
		if (FAILED(hr))
		{
			return hr;
		}

		// Look at each Kinect sensor
		for (int i = 0; i < iSensorCount; ++i) {
			// Create the sensor so we can check status, if we can't create it, move on to the next
			hr = NuiCreateSensorByIndex(i, &pNuiSensor);
			if (FAILED(hr)) {
				continue;
			}

			// Get the status of the sensor, and if connected, then we can initialize it
			hr = pNuiSensor->NuiStatus();
			if (S_OK == hr) {
				m_pNuiSensor = pNuiSensor;
				break;
			}

			// This sensor wasn't OK, so release it since we're not using it
			pNuiSensor->Release();
		}

		 

		if (NULL != m_pNuiSensor) {
			// Initialize the Kinect and specify that we'll be using skeleton
			hr = m_pNuiSensor->NuiInitialize( NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR); 			
			if (SUCCEEDED(hr)) {
				
				// Create an event that will be signaled when skeleton data is available
				m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

				// Open a skeleton stream to receive skeleton data
				hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0); 

				// get resolution as DWORDS, but store as LONGs to avoid casts later
				DWORD width = 0;
				DWORD height = 0;

				NuiImageResolutionToSize(cDepthResolution, width, height);
				m_depthWidth  = static_cast<LONG>(width);
				m_depthHeight = static_cast<LONG>(height);

				NuiImageResolutionToSize(cColorResolution, width, height);
				m_colorWidth  = static_cast<LONG>(width);
				m_colorHeight = static_cast<LONG>(height);

				m_colorToDepthDivisor = m_colorWidth/m_depthWidth;

				m_depthTimeStamp.QuadPart = 0;
				m_colorTimeStamp.QuadPart = 0;

				// create heap storage for depth pixel data in RGBX format
				m_depthD16 = new USHORT[m_depthWidth*m_depthHeight];
				m_depthD16_NoPlayer = new USHORT[m_depthWidth*m_depthHeight];
				m_colorCoordinates = new LONG[m_depthWidth*m_depthHeight*2]; 

				m_colorRGBX = new BYTE[m_colorWidth*m_colorHeight*cBytesPerPixel];
				m_backgroundRGBX = new BYTE[m_colorWidth*m_colorHeight*cBytesPerPixel];
				m_outputRGBX = new BYTE[m_colorWidth*m_colorHeight*cBytesPerPixel];

				// Create an event that will be signaled when depth data is available
				m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

				// Create an event that will be signaled when color data is available
				m_hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


				// Open a depth image stream to receive depth frames
				hr = m_pNuiSensor->NuiImageStreamOpen(
					NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX ,
					cDepthResolution,
					0,
					2,
					m_hNextDepthFrameEvent,
					&m_pDepthStreamHandle);

				// Open a color image stream to receive depth frames
				hr = m_pNuiSensor->NuiImageStreamOpen(
					NUI_IMAGE_TYPE_COLOR,
					cColorResolution,
					0,
					2,
					m_hNextColorFrameEvent,
					&m_pColorStreamHandle);

				//NuiImageStreamSetImageFrameFlags(m_pDepthStreamHandle, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE);


						 depthPoints = new NUI_DEPTH_IMAGE_POINT[640 * 480];
						

						 m_pNuiSensor->NuiGetCoordinateMapper(&pMapper);

				Log("Microsoft Kinect Connected !");



			}
		}

		if (NULL == m_pNuiSensor || FAILED(hr)) {
			return false;
		}


		 

		/////




		if( mUseColor ) CreateColor();
		if( mUseDepth ) CreateDepth();
		if( mUseInfrared ) CreateInfraRed();

		if( mUseColor && mUseDepth )  {
			 
		} 

		 

		return true;
	}

	void Disconnect()
	{
		

	}

	HRESULT ProcessColor()
	{
		HRESULT hr = S_OK;
		NUI_IMAGE_FRAME imageFrame;

		// Attempt to get the depth frame
		hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0, &imageFrame);
		if (FAILED(hr)) {
			return hr;
		}

		m_colorTimeStamp = imageFrame.liTimeStamp;

		INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
		NUI_LOCKED_RECT LockedRect;

		// Lock the frame data so the Kinect knows not to modify it while we're reading it
		pTexture->LockRect(0, &LockedRect, NULL, 0);

		BYTE * pDest=static_cast<BYTE*>(m_colorRGBX);
		unsigned int widthStep = (4 * m_colorWidth) ;
		int LayerBpp = 4;
		// Make sure we've received valid data
		if (LockedRect.Pitch != 0) {
			for(size_t i = 0; i < m_colorHeight; i++) {
				for (size_t j = 0; j < m_colorWidth; j++) {
					unsigned char blue,green,red;
					*pDest++ = ((unsigned char*)(LockedRect.pBits + i * widthStep))[j*LayerBpp+2];
					*pDest++ = ((unsigned char*)(LockedRect.pBits + i * widthStep))[j*LayerBpp+1];
					*pDest++ = ((unsigned char*)(LockedRect.pBits + i * widthStep))[j*LayerBpp+0];
				}
			}
		}

 
		// We're done with the texture so unlock it
		pTexture->UnlockRect(0);

		// Release the frame
		m_pNuiSensor->NuiImageStreamReleaseFrame(m_pColorStreamHandle, &imageFrame);

		return hr;
	}


	//public int GetPlayerAt(int x, int y)
	//{
	//	return _img.Bits[y * _img.Width * _img.BytesPerPixel + x * _img.BytesPerPixel] & 0x07;
	//}


	HRESULT ProcessDepth()
	{
		HRESULT hr = S_OK;
		NUI_IMAGE_FRAME imageFrame;

		// Attempt to get the depth frame
		hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0, &imageFrame);
		if (FAILED(hr))
		{
			return hr;
		}

		m_depthTimeStamp = imageFrame.liTimeStamp;

		INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
		NUI_LOCKED_RECT LockedRect;

		// Lock the frame data so the Kinect knows not to modify it while we're reading it
		pTexture->LockRect(0, &LockedRect, NULL, 0);

 


////
		/*
		INuiCoordinateMapper* pMapper;
		m_pNuiSensor->NuiGetCoordinateMapper(&pMapper);
		pMapper->MapColorFrameToDepthFrame(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_640x480,
			NUI_IMAGE_RESOLUTION_640x480,
			640 * 480, 
			(NUI_DEPTH_IMAGE_PIXEL*)LockedRect.pBits,
			640 * 480, 
			depthPoints);
			*/
////


		  
		USHORT* pDest = (USHORT*) m_depthD16;
		USHORT* pDest2 = (USHORT*) m_depthD16_NoPlayer;
		unsigned int widthStep = (2 * m_depthWidth) ;
		int LayerBpp = 2;
		// Make sure we've received valid data
		if (LockedRect.Pitch != 0) {
			for(size_t i = 0; i < m_depthHeight; i++) {
				for (size_t j = 0; j < m_depthWidth; j++) {
					int BaseByte = i * m_depthWidth * 2 + j * 2;


				//RealDepth = NuiDepthPixelToDepth(*pBufferRun);
				//Player    = NuiDepthPixelToPlayerIndex(*pBufferRun);


					bool hasPlayerData = true;
					if (hasPlayerData) {
						*pDest++ = (LockedRect.pBits[BaseByte + 1] << 5) | (LockedRect.pBits[BaseByte] >> 3);

						*pDest2++ = (LockedRect.pBits[BaseByte + 1] << 8) | (LockedRect.pBits[BaseByte]);
					}
					else {
						*pDest++ = (LockedRect.pBits[BaseByte + 1] << 8) | (LockedRect.pBits[BaseByte]);
					}
				}
			}
		}
		 
	 

		/*
		 
		USHORT* pDest = (USHORT*) m_depthD16;
		unsigned int widthStep = (2 * m_depthWidth) ;
		int LayerBpp = 2;
		// Make sure we've received valid data
		if (LockedRect.Pitch != 0) {

			USHORT* packedPixel = (USHORT*) LockedRect.pBits;
			for(size_t i = 0; i < m_depthHeight; i++) {
				for (size_t j = 0; j < m_depthWidth; j++) {

					int BaseByte = i * m_depthWidth * 2 + j * 2;


					LONG colorX = 0;
					LONG colorY = 0;
					m_pNuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
						NUI_IMAGE_RESOLUTION_640x480,
						NUI_IMAGE_RESOLUTION_640x480,
						NULL,
						j,
						i,
						*packedPixel,
						&colorX,
						&colorY
						);

					if (colorX >= 0 && colorX <= m_depthWidth && colorY >= 0 && colorY <= m_depthHeight) {
					pDest[(m_depthWidth*colorY)+colorX] = NuiDepthPixelToDepth(*packedPixel);//(packedPixel[BaseByte + 1] << 5) | (packedPixel[BaseByte] >> 3);//*packedPixel & NUI_IMAGE_PLAYER_INDEX_MASK ? 255 : 0;
				}

				++packedPixel;
				}
			}
		}
		*/
		 

	 

	 



		 
		// Make sure we've received valid data
		//if (LockedRect.Pitch != 0)
		//{
		// 	memcpy(m_depthD16, LockedRect.pBits, LockedRect.size);
		//}

		// We're done with the texture so unlock it
		pTexture->UnlockRect(0);

		// Release the frame
		m_pNuiSensor->NuiImageStreamReleaseFrame(m_pDepthStreamHandle, &imageFrame);

		 /*
		// Get of x, y coordinates for color in depth space
		// This will allow us to later compensate for the differences in location, angle, etc between the depth and color cameras
		m_pNuiSensor->NuiImageGetColorPixelCoordinateFrameFromDepthPixelFrameAtResolution(
			cColorResolution,
			cDepthResolution,
			m_depthWidth*m_depthHeight,
			m_depthD16,
			m_depthWidth*m_depthHeight*2,
			m_colorCoordinates
			);
			 */

		return hr;
	}

	void UpdateBuffers()
	{

		bool needToDraw = false;

		if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextDepthFrameEvent, 0) )
		{
			// if we have received any valid new depth data we may need to draw
			if ( SUCCEEDED(ProcessDepth()) ) {
				needToDraw = true;
				mHasNewDepth = true;
			}
		}

		if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextColorFrameEvent, 0) )
		{
			// if we have received any valid new color data we may need to draw
			if ( SUCCEEDED(ProcessColor()) ) {
				needToDraw = true;
				mHasNewColor = true;
			}
		}


		/*
		// Depth is 30 fps.  For any given combination of FPS, we should ensure we are within half a frame of the more frequent of the two.  
		// But depth is always the greater (or equal) of the two, so just use depth FPS.
		const int depthFps = 30;
		const int halfADepthFrameMs = (1000 / depthFps) / 2;

		// If we have not yet received any data for either color or depth since we started up, we shouldn't draw
		if (m_colorTimeStamp.QuadPart == 0 || m_depthTimeStamp.QuadPart == 0)
		{
			needToDraw = false;
		}

		// If the color frame is more than half a depth frame ahead of the depth frame we have,
		// then we should wait for another depth frame.  Otherwise, just go with what we have.
		if (m_colorTimeStamp.QuadPart - m_depthTimeStamp.QuadPart > halfADepthFrameMs)
		{
			needToDraw = false;
		}

		if (needToDraw)
		{
			int outputIndex = 0;
			LONG* pDest;
			LONG* pSrc;

			// loop over each row and column of the color
			for (LONG y = 0; y < m_colorHeight; ++y)
			{
				for (LONG x = 0; x < m_colorWidth; ++x)
				{
					// calculate index into depth array
					int depthIndex = x/m_colorToDepthDivisor + y/m_colorToDepthDivisor * m_depthWidth;

					USHORT depth  = m_depthD16[depthIndex];
					//USHORT player = NuiDepthPixelToPlayerIndex(depth);

					// default setting source to copy from the background pixel
					pSrc  = (LONG *)m_backgroundRGBX + outputIndex;

					// if we're tracking a player for the current pixel, draw from the color camera
					// if ( player > 0 )
					// {
						// retrieve the depth to color mapping for the current depth pixel
						LONG colorInDepthX = m_colorCoordinates[depthIndex * 2];
						LONG colorInDepthY = m_colorCoordinates[depthIndex * 2 + 1];

						// make sure the depth pixel maps to a valid point in color space
						if ( colorInDepthX >= 0 && colorInDepthX < m_colorWidth && colorInDepthY >= 0 && colorInDepthY < m_colorHeight )
						{
							// calculate index into color array
							LONG colorIndex = colorInDepthX + colorInDepthY * m_colorWidth;

							// set source for copy to the color pixel
							pSrc  = (LONG *)m_colorRGBX + colorIndex;
						}
					//}

					// calculate output pixel location
					pDest = (LONG *)m_outputRGBX + outputIndex++;

					// write output
					*pDest = *pSrc;
				}
			}

			mHasNewColor = true;

			// Draw the data with Direct2D
			//m_pDrawGreenScreen->Draw(m_outputRGBX, m_colorWidth * m_colorHeight * cBytesPerPixel);

			
		}   
		*/

	
	}

	bool Update() 
	{


		const int eventCount = 2;
		HANDLE hEvents[eventCount];
 
		hEvents[0] = m_hNextDepthFrameEvent;
		hEvents[1] = m_hNextColorFrameEvent;

		// Check to see if we have either a message (by passing in QS_ALLINPUT)
		// Or a Kinect event (hEvents)
		// Update() will check for Kinect events individually, in case more than one are signalled
		DWORD dwEvent = MsgWaitForMultipleObjects(eventCount, hEvents, FALSE, INFINITE, QS_ALLINPUT);

		// Check if this is an event we're waiting on and not a timeout or message
		if (WAIT_OBJECT_0 == dwEvent || WAIT_OBJECT_0 + 1 == dwEvent)
		{
			UpdateBuffers();
		}

		MSG       msg = {0};
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
 

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
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
		return (NxMocapImageDepth*) m_depthD16;
	}

	const NxMocapImageRGB * GetColorMap() const
	{
		mHasNewColor = false;
		return (NxMocapImageRGB *) m_colorRGBX;
	}

	void ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY )
	{
		m_pNuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
			NUI_IMAGE_RESOLUTION_640x480,
			NUI_IMAGE_RESOLUTION_640x480,
			NULL,
			depthX,
			depthY,
			(USHORT) DepthZ << 3,
			ColorX,
			ColorY
			);
	}

	bool GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const
	{
		m_pNuiSensor->NuiImageGetColorPixelCoordinateFrameFromDepthPixelFrameAtResolution(
			cColorResolution,
			cDepthResolution,
			m_depthWidth*m_depthHeight,
			m_depthD16_NoPlayer,
			m_depthWidth*m_depthHeight*2,
			dst
			);

		return true;
	}

	const NxMocapImageIr * GetInfraRedMap() const
	{
		mHasNewInfraRed = false;
		return  0;//(NxMocapImageIr*) m_infraFrame.getData();
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

	NxMocapDeviceKinectMicrosoft::NxMocapDeviceKinectMicrosoft() : NxMocapDeviceKinect()  {

	}

	NxMocapDeviceKinectMicrosoft::~NxMocapDeviceKinectMicrosoft(){

	}

	bool NxMocapDeviceKinectMicrosoft::ConnectDevice()
	{
		return ConnectDevice( true,true,true );
	}

	bool NxMocapDeviceKinectMicrosoft::ConnectDevice( bool useColor, bool useDepth, bool useInfrared ) {

		Log("Connecting Kinect..");
		mKinect = new KinectPlayerMicrosoft( useColor, useDepth, useInfrared );
		if( !mKinect->initPrimeSensor()   ){ Log(" Connecting Kinect Failed !"); return false; }
		Log("Connecting Kinect Success");

		return true;
	}

	bool NxMocapDeviceKinectMicrosoft::DisconnectDevice(){
		mKinect->Disconnect();

		return true;
	}

 

	bool NxMocapDeviceKinectMicrosoft::IsUsingInfraRedMap()
	{
		return mKinect->UseInfraRed();
	}

	bool NxMocapDeviceKinectMicrosoft::HasNewDepth() {
		return mKinect->HasNewDepth();
	}

	bool NxMocapDeviceKinectMicrosoft::HasNewColor() {
		return mKinect->HasNewColor();
	}

	bool NxMocapDeviceKinectMicrosoft::HasNewInfraRed() {
		return mKinect->HasNewInfraRed();
	}

	 

	const NxMocapImageDepth * NxMocapDeviceKinectMicrosoft::GetDepthMap() const
	{
		return mKinect->GetDepthMap();
	}

	const NxMocapImageRGB * NxMocapDeviceKinectMicrosoft::GetImageMap() const
	{
		return mKinect->GetColorMap();
	}

	const NxMocapImageIr * NxMocapDeviceKinectMicrosoft::GetInfraRedMap() const
	{
		return mKinect->GetInfraRedMap();
	}

	void NxMocapDeviceKinectMicrosoft::ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY )
	{
		mKinect->ConvertDepthToColor( depthX, depthY, DepthZ, ColorX, ColorY );
	}

	bool NxMocapDeviceKinectMicrosoft::GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const
	{
		return mKinect->GetDepthToColorMap( src, dst );
	}

	bool NxMocapDeviceKinectMicrosoft::Update(){

		return mKinect->Update();
	}



}