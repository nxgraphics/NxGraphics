#include "NxVideoPreCompiled.h"

namespace NxVideo_Namespace {

struct NxVideoVFW
{
	PAVISTREAM pStream;
	PGETFRAME pFrame;
	HIC hic; // handler to our decompressor
	BYTE* pDIB ;
	BYTE * Final_pDIB ;
	LPBITMAPINFOHEADER lpbi;

	// audio

	WAVEFORMATEX waveformat  ;
	PAVISTREAM as  ;
	WORD channels  ;
	DWORD samplerate  ;
}; 

NxVideo_Avi_Player::NxVideo_Avi_Player() : NxVideo_Player()
{
	m_Video = new NxVideoVFW();
	mVideoType =  NxVideoAviFile;
}

NxVideo_Avi_Player::~NxVideo_Avi_Player()
{

}

bool NxVideo_Avi_Player::CloseVideoFile()
{
	StopRendering();

	//if( mVideoData )
	//{ 
	//	delete mVideoData;
	//	mVideoData = NULL;
	//}else
	//{ 
	//	//Log("NxVideo_Avi_Player : CloseVideoFile() no data to delete ");
	//}

	AVIStreamGetFrameClose(m_Video->pFrame);
	if (m_Video->pStream!=NULL){ AVIStreamRelease(m_Video->pStream); }
	m_Video->pFrame = NULL ;
	AVIFileExit();
	return true;
}

bool NxVideo_Avi_Player::OpenVideoFile( const std::string & szFileName, NxVideoPixelFormatInfo & info )
{
	//CoInitialize(0);
	AVIFileInit();
	PAVIFILE avi;
	HRESULT res = AVIFileOpen(&avi, szFileName.c_str() , OF_READ, NULL); // crashes here in debug mode

	if( res != AVIERR_OK )
	{
		if( avi != NULL )
		{
			Log("avi!=NULL started");
			AVIFileRelease(avi);
		} 
	  
		Log( "NxVideo: Error opening file" );   
		return false;
   }

	mVideoFullPath = std::string( szFileName );//save the video full path
	std::string Tmp = mVideoFullPath ;

	NxVideoUtils::SplitFileName( Tmp );
	mVideoFullName = Tmp;

	NxVideoUtils::SplitFileExtension( Tmp );
	mVideoExtension = Tmp;
 
	//Get_Video_Codec_Info( szFileName ); // get all information concerning the file

	AVIFILEINFO avi_info;
	AVIFileInfo(avi, &avi_info, sizeof(AVIFILEINFO));

	//mVideoCodecName = VIDEO_GLOBAL_INFO->video_global_information->CODEC_INFO->CODEC_NAME ;
	mNumPlanes = 1;
	mVideoWidth[0]  = avi_info.dwWidth ;
	mVideoHeight[0] = avi_info.dwHeight ;
	SetFramerate( ((float) avi_info.dwRate / avi_info.dwScale ) );


	///////////

	//for sound stream decoding lokkat :
	//http://tothemax.googlecode.com/svn/trunk/verge/Source/win_movie.cpp
 

	////////// VIDEO

	res = AVIFileGetStream(avi, &m_Video->pStream, streamtypeVIDEO , 0 /*first stream*/);
 
	if (res!=AVIERR_OK) {
		if (m_Video->pStream!=NULL) {
			AVIStreamRelease(m_Video->pStream);
		}
		AVIFileExit();
		return FALSE;
	}

   // find the start frame number
   mFrameOffset = AVIStreamStart( m_Video->pStream );
   mFrameInbound = mFrameOffset ;
   if ( mFrameOffset == -1 ) { // error
	   if (m_Video->pStream!=NULL) {
		   AVIStreamRelease(m_Video->pStream);
	   } 
	   AVIFileExit();
	   return FALSE;
   }

   mFrame = mFrameOffset ;

   mFramesLength = AVIStreamLength( m_Video->pStream );// Frames_Length_Original / Fps = lenght seconds , * 1000 = milliseconds
   mFrameOutbound = mFramesLength ;
	if (mFramesLength == -1)//Error getteing the number of frames inside the stream
	{
		if (m_Video->pStream!=NULL)
		{
			AVIStreamRelease(m_Video->pStream);
		} 
		AVIFileExit();
		return FALSE;
   }

	BITMAPINFOHEADER	bmih;
	memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof (BITMAPINFOHEADER);	 // Size Of The BitmapInfoHeader
	bmih.biPlanes = 1;							 // Bitplanes	
	bmih.biBitCount = 3;				 // Bits Format We Want (24 Bit, 3 Bytes)
	bmih.biWidth = mVideoWidth[0];		 // Width We Want (rX Pixels)
	bmih.biHeight = -mVideoHeight[0];    // minus Flip image
	bmih.biCompression = BI_RGB;      // Requested Mode = RGB

	if (!(m_Video->pFrame = AVIStreamGetFrameOpen(m_Video->pStream, &bmih))) {
		Log("ERR: AVIStreamGetFrameOpen failed to load in %dbpp for %s\n" );
		if (!(m_Video->pFrame = AVIStreamGetFrameOpen(m_Video->pStream, NULL))) {
			Log("ERR: AVIStreamLength failed for %s\n" );
			return false;
		}
	}

	/*
   //getting bitmap from frame
   BITMAPINFOHEADER bih;
   ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
	m_Video->pFrame = AVIStreamGetFrameOpen( m_Video->pStream ,  NULL );  
	if( m_Video->pFrame == NULL )
	{
		Log( "type-1 DV AVI files are not compatible with VfW.\r\nCould not find a appropriate video decompressor !\r\n" );
		return false;
	}
	*/
 
	 
	// extract stream info
	AVISTREAMINFO vidsinfo ;
	res = AVIStreamInfo(m_Video->pStream, &vidsinfo, sizeof(vidsinfo) ) ;
	if (res!=AVIERR_OK) {
		Log( "Nxvideo info AVIStreamInfo failed" );
		return false ;
	}
 
	// [VIDEO]   if (streaminfo.fccType == streamtypeVIDEO) {
	// get information about the codec ////
	DWORD  CodecfccHandler = vidsinfo.fccHandler  ;
	std::string CodecfccHandlerStr =  NxVideoUtils::FOURCC2Str( CodecfccHandler ) ;

	NxVideoUtils::ToUpper( CodecfccHandlerStr );
	//std::transform(CodecfccHandlerStr.begin(), CodecfccHandlerStr.end(), CodecfccHandlerStr.begin(),  std::toupper);
	Log( "Nxvideo file decompressed Four : " + CodecfccHandlerStr );
	Log( "Nxvideo file decompressed Name : " + std::string( vidsinfo.szName ) );
	
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_Video->pFrame, 0 ); 
	if(!lpbi) {
		Log( "LPBITMAPINFOHEADER failed --> Frame_Start_Original  \r\n" );
		return 0;
	}
 
	// calculate the bits per pixel
	mVideoBpp = lpbi->biBitCount / 8 ;
	// create the video pixel buffer
	//mVideoData  = new BYTE[  mVideoWidth * mVideoHeight * mVideoBpp  ];
	/*
	//fill the first frame
	long good_pos =	AVIStreamFindSample(  m_Video->pStream,  mFrame , FIND_NEXT | FIND_ANY ); 
 	if(good_pos != -1)
 	{
		LPBITMAPINFOHEADER lpbi2;
		lpbi2 = (LPBITMAPINFOHEADER)AVIStreamGetFrame( m_Video->pFrame, good_pos );
		if(lpbi2 == NULL)
		{
			Log( "Could not decompress frame ! \r\n" );
		}
		mVideoData =  (BYTE*) lpbi2 + lpbi2->biSize;
	}
	*/

	StartRendering();

	return true ;
}

void NxVideo_Avi_Player::SetFramerate( float FrameRate )
{
	mVideoFrameRate = FrameRate ;
}
 

bool NxVideo_Avi_Player::RenderFrame()
{
	if( GetPlayBackState() == VIDEO_STATUS_STOP ){ return false; }
 
	if( mFramePrevious != mFrame )
	{ 
		if(!m_Video->pFrame){ return false ;}
		long good_pos =	AVIStreamFindSample(  m_Video->pStream, mFrame, FIND_NEXT|FIND_ANY );

	 	if(good_pos != -1) 
		{
			/*
			LPBITMAPINFOHEADER lpbi;
			lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame( m_Video->pFrame, good_pos );
			if(lpbi == NULL) { Log( "Could not decompress frame ! \r\n" ); return false; }

			mVideoData =  (BYTE*) lpbi + lpbi->biSize;
			*/

			mFramePrevious =  good_pos  ; 
			mNewFrame = true;
			return true ;
		 }
		else
		{
			return false ;
		}
	}
	else
	{ 
		return false;
	}
}

bool NxVideo_Avi_Player::NewFrame()
{
	return RenderFrame();
}

void NxVideo_Avi_Player::ResetFrame()
{
	mNewFrame = false;
}

void NxVideo_Avi_Player::GetPixelData( unsigned char * data  )
{

	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame( m_Video->pFrame, mFramePrevious );
	if(lpbi == NULL) { Log( "Could not decompress frame ! \r\n" );  }

	//mVideoData =  (BYTE*) lpbi + lpbi->biSize;

	memcpy( &data[0], (BYTE*) lpbi + lpbi->biSize, mVideoWidth[0]*mVideoHeight[0]*mVideoBpp );

	//memcpy( &data[0], &mVideoData[0], mVideoWidth*mVideoHeight*mVideoBpp );


}


}//namespace


