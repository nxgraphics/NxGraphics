#include "NxVideoPreCompiled.h"
 
//check :
// code.google.com/p/wowmdelviewer/source/browse/trunk/src/avigenerator.cpp

// needed headers
#include <comdef.h>
#include <memory.h>
#include <tchar.h>

namespace NxVideo_Namespace {

struct NxVideoRecorderVFW
{
	long m_lFrame;
	//! file interface pointer
	PAVIFILE m_pAVIFile;
	//! Address of the stream interface
	PAVISTREAM m_pStream;		
	//! Address of the compressed video stream
	PAVISTREAM m_pStreamCompressed;
	BITMAPINFOHEADER m_bih;
 
}; 

NxVideo_Avi_Recorder::NxVideo_Avi_Recorder() : NxVideo_Recorder()
{
	mVideo = new NxVideoRecorderVFW();
}

NxVideo_Avi_Recorder::~NxVideo_Avi_Recorder()
{

}

bool NxVideo_Avi_Recorder::CreateVideoFile( const std::string & szFileName, int width, int height, int Bpp, int Fps )
{
	// initialize m_bih
	memset(&mVideo->m_bih,0, sizeof(BITMAPINFOHEADER));
	// filling bitmap info structure.
	mVideo->m_bih.biSize=sizeof(BITMAPINFOHEADER);
	mVideo->m_bih.biWidth= width ;
	mVideo->m_bih.biHeight= height ;
	mVideo->m_bih.biPlanes=1;
	mVideo->m_bih.biBitCount= Bpp * 8;
	mVideo->m_bih.biSizeImage=((mVideo->m_bih.biWidth*mVideo->m_bih.biBitCount+31)/32 * 4)*mVideo->m_bih.biHeight; 
	mVideo->m_bih.biCompression=BI_RGB;//BI_RGB means BRG in reality

	_bstr_t m_sError;
	_bstr_t m_sFile = szFileName.c_str();	
	//! Frame rate 
	DWORD m_dwRate = Fps;
	AVISTREAMINFO strHdr; // information for a single stream 
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};

	TCHAR szBuffer[1024];
	HRESULT hr;
	m_sError=_T("Ok");

	// Step 0 : Let's make sure we are running on 1.1 
	DWORD wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a)
	{
		 // oops, we are too old, blow out of here 
		m_sError=_T("Version of Video for Windows too old. Come on, join the 21th century!");
		return S_FALSE;
	}

	// Step 1 : initialize AVI engine
	AVIFileInit();
	// Step 2 : Open the movie file for writing....
	hr = AVIFileOpen(&mVideo->m_pAVIFile,	// Address to contain the new file interface pointer
		       (LPCSTR)m_sFile,				// Null-terminated string containing the name of the file to open
		       OF_WRITE | OF_CREATE,	    // Access mode to use when opening the file. 
		       NULL);						// use handler determined from file extension.
											// Name your file .avi -> very important

	if (hr != AVIERR_OK)
	{
		_tprintf(szBuffer,_T("AVI Engine failed to initialize. Check filename %s."),m_sFile);
		m_sError=szBuffer;
		// Check it succeded.
		switch(hr)
		{
		case AVIERR_BADFORMAT: 
			m_sError+=_T("The file couldn't be read, indicating a corrupt file or an unrecognized format.");
			break;
		case AVIERR_MEMORY:		
			m_sError+=_T("The file could not be opened because of insufficient memory."); 
			break;
		case AVIERR_FILEREAD:
			m_sError+=_T("A disk error occurred while reading the file."); 
			break;
		case AVIERR_FILEOPEN:		
			m_sError+=_T("A disk error occurred while opening the file.");
			break;
		case REGDB_E_CLASSNOTREG:		
			m_sError+=_T("According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it");
			break;
		}

		return false;
	}

	// Fill in the header for the video stream....
	memset(&strHdr, 0, sizeof(strHdr));
	strHdr.fccType                = streamtypeVIDEO;	// video stream type
	strHdr.fccHandler             = 0;
	strHdr.dwScale                = 1;					// should be one for video
	strHdr.dwRate                 = m_dwRate;		    // fps
	strHdr.dwSuggestedBufferSize  = mVideo->m_bih.biSizeImage;	// Recommended buffer size, in bytes, for the stream.
	SetRect(&strHdr.rcFrame, 0, 0,		    // rectangle for stream
	    (int) mVideo->m_bih.biWidth,
	    (int) mVideo->m_bih.biHeight);

	// Step 3 : Create the stream;
	hr = AVIFileCreateStream(mVideo->m_pAVIFile,	// file pointer
			         &mVideo->m_pStream,		    // returned stream pointer
			         &strHdr);						// stream header

	// Check it succeded.
	if (hr != AVIERR_OK)
	{
		m_sError=_T("AVI Stream creation failed. Check Bitmap info.");
		if (hr==AVIERR_READONLY)
		{
			m_sError+=_T(" Read only file.");
		}
		return hr;
	}


	// Step 4: Get codec and infos about codec
	Log("NxGraphics : Recording, Getting codec info");
	memset(&opts, 0, sizeof(opts));
	// Poping codec dialog
	if (!AVISaveOptions(NULL, 0, 1, &mVideo->m_pStream, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
	{
		Log("NxGraphics : Recording, Getting codec info Failed");
		AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
		return S_FALSE;
	}

	
	Log("NxGraphics : Recording, Creating Stream");
	// Step 5:  Create a compressed stream using codec options.
	hr = AVIMakeCompressedStream(&mVideo->m_pStreamCompressed, 
				mVideo->m_pStream, 
				&opts, 
				NULL);

	if (hr != AVIERR_OK)
	{
		m_sError=_T("AVI Compressed Stream creation failed.");
		
		switch(hr)
		{
		case AVIERR_NOCOMPRESSOR:
			m_sError+=_T(" A suitable compressor cannot be found.");
				break;
		case AVIERR_MEMORY:
			m_sError+=_T(" There is not enough memory to complete the operation.");
				break; 
		case AVIERR_UNSUPPORTED:
			m_sError+=_T("Compression is not supported for this type of data. This error might be returned if you try to compress data that is not audio or video.");
			break;
		}

		return hr;
	}

	// releasing memory allocated by AVISaveOptionFree
	hr=AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
	if (hr!=AVIERR_OK)
	{
		m_sError=_T("Error releasing memory");
		return hr;
	}

	Log("NxGraphics : Recording, Setting video format");
	// Step 6 : sets the format of a stream at the specified position
	hr = AVIStreamSetFormat(mVideo->m_pStreamCompressed, 
					0,			// position
					&mVideo->m_bih,	    // stream format
					mVideo->m_bih.biSize +   // format size
					mVideo->m_bih.biClrUsed * sizeof(RGBQUAD));

	if (hr != AVIERR_OK)
	{
		m_sError=_T("AVI Compressed Stream format setting failed.");
		return hr;
	}

	// Step 6 : Initialize step counter
	mVideo->m_lFrame=0;
 
	return true;
}

bool NxVideo_Avi_Recorder::CloseVideoFile()
{

	if (mVideo->m_pStream)
	{
		AVIStreamRelease(mVideo->m_pStream);
		mVideo->m_pStream=NULL;
	}

	if (mVideo->m_pStreamCompressed)
	{
		AVIStreamRelease(mVideo->m_pStreamCompressed);
		mVideo->m_pStreamCompressed=NULL;
	}

	if (mVideo->m_pAVIFile)
	{
		AVIFileRelease(mVideo->m_pAVIFile);
		mVideo->m_pAVIFile=NULL;
	}

	// Close engine
	AVIFileExit();
	return true;
}

void NxVideo_Avi_Recorder::AddVideoFrame( unsigned char * bmBits )
{
	HRESULT hr;

	// compress bitmap
	hr = AVIStreamWrite( mVideo->m_pStreamCompressed,	// stream pointer
		mVideo->m_lFrame,						// time of this frame
		1,						// number to write
		bmBits,					// image buffer
		mVideo->m_bih.biSizeImage,		// size of this frame
		AVIIF_KEYFRAME,			// flags....
		NULL,
		NULL);

	// updating frame counter
	mVideo->m_lFrame++;
}
 
}//namespace