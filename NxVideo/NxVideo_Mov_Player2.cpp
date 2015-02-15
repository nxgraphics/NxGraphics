#include "NxVideoPreCompiled.h"

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
#define GLEW_STATIC
#include <Gl/glew.h>
 #pragma comment( lib, "opengl32" )
#else if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_APPLE
#include <OPENGl/gl.h>
#endif

#define forwardNormalSpeed 0x00010000 //normal speed (1) in fixed 
 
namespace NxVideo_Namespace {
 
struct NxVideoQuickTime
{
public:
    Movie MovieQT;
	Media mVideoMedia;
	Media mAudioMedia;
	Track mVideoTrack;
	Track mAudioTrack;
	short movieResFile;
	bool FrameReady;
	OSErr err_;
	//unsigned codec_;
	float gamma_;
	OSType manufacturer_;
	int codecFlags_;
	ICMDecompressionSessionRef session_;
	CVPixelBufferRef pixelBuffer_;
	long mFramePosition;
	TimeValue64 sampleTableStartDecodeTime  ;
	QTMutableSampleTableRef sampleTable  ;
	SInt64 lastDecompressedSampleNumber ;
	TimeValue64 mCurrentDecodeTime;
	TimeValue64 mCurrentDecodeTimeTarget; 
	unsigned long LastFrameNum;
	long mFormatDst;

	unsigned char * mBuffer;
	unsigned int mBpp;

	unsigned int mGLID;

	// audio
	MovieAudioExtractionRef mAudioSession;
	AudioBufferList * abl;
	AudioStreamBasicDescription inputStreamDesc;
	unsigned long mAudioLastSampleNum;


	NxVideoQuickTime() :  MovieQT( 0 ), session_(0), pixelBuffer_(0) {  

		mAudioTrack = NULL;
		FrameReady = false;
		mFramePosition = 0;
		sampleTableStartDecodeTime = 0;
		sampleTable = NULL;
		lastDecompressedSampleNumber = 0;
		mCurrentDecodeTime = 0;
		mCurrentDecodeTimeTarget = 0; 
		LastFrameNum = 0 ;
		mGLID = 0;

		mAudioLastSampleNum = 0;

		mBuffer = NULL;
		mBpp = -1;
	}

	~NxVideoQuickTime(){
		FrameReady = false;
		if ( pixelBuffer_ ) {
			CVPixelBufferUnlockBaseAddress( pixelBuffer_, 0 );
			CVPixelBufferRelease( pixelBuffer_ );
		}
		if ( session_ )
			ICMDecompressionSessionRelease(session_);
		//ExitMovies();
	}

} ; 

//codec info : https://developer.apple.com/library/mac/technotes/tn2273/_index.html
long ConvertVideoFormat( NxVideoPixelFormat format ) {
	return format;
}

long ConvertAudioFormat( NxVideoAudioFormat format ) {
	if( format == NxVideoAudioFormat16BpS  ) return kAudioFormatFlagIsSignedInteger;
	else if ( format == NxVideoPixelFormat32BpS  ) return kAudioFormatFlagIsFloat;
	else{ Log("ConvertAudioFormat Error : No Audio Format defined"); }
	return 0;
}



void addIntToDictionary(CFMutableDictionaryRef dictionary, CFStringRef key, SInt32 number)
{
	CFNumberRef cfnumber = CFNumberCreate(NULL, kCFNumberSInt32Type, &number);
	if ( !number ){  Log("CFNumberRef Error"); return;  }
		
	CFDictionaryAddValue(dictionary, key, cfnumber);
	CFRelease(cfnumber);
}
 
static void emitFrame( void* reader, OSStatus result,
						  ICMDecompressionTrackingFlags flags,
						  CVPixelBufferRef pixelBuffer,
						  TimeValue64 displayTime,
						  TimeValue64 displayDuration,
						  ICMValidTimeFlags validFlags,
						  void* reserved, void* frameData)
{
 
	NxVideoQuickTime* self = (NxVideoQuickTime*)reader;
 
	if ( result != noErr ) {
		cerr << "Decode Error: " << result << endl;
		return;
	}

	if ( kICMDecompressionTracking_ReleaseSourceData & flags ){
		free(frameData);
		//Log("free DATA");
	}

	if ( kICMValidTime_DisplayTimeStampIsValid & flags )
		//Log("FRAME Display Time: " + NxVideoUtils::ToString(displayTime ));
	if ( kICMValidTime_DisplayDurationIsValid  & flags )
		//Log("FRAME Duration : " + NxVideoUtils::ToString(displayDuration ));
	if ( kICMDecompressionTracking_FrameDecoded & flags){
		 //   Log ("FRAME DECODED");
	}
		
	if ( kICMDecompressionTracking_FrameDropped & flags )
		Log ("FRAME DROPPED");
	if ( kICMDecompressionTracking_FrameNeedsRequeueing & flags )
		Log ("FRAME NEEDS REQUEUING");
  
	
	if ( (kICMDecompressionTracking_EmittingFrame & flags) && pixelBuffer ) {

		self->pixelBuffer_ = NULL;
		// Log("--- kICMDecompressionTracking_EmittingFrame !");

		if( self->mGLID == 0 ) { // pixel buffer blitting
			CVPixelBufferLockBaseAddress( pixelBuffer, 0 );
			unsigned char * baseAddress = ( unsigned char* )(CVPixelBufferGetBaseAddress( pixelBuffer ));

			const size_t bytesPerRow = CVPixelBufferGetBytesPerRow( pixelBuffer );
			const size_t width = CVPixelBufferGetWidth( pixelBuffer );
			const size_t height = CVPixelBufferGetHeight( pixelBuffer );
			const size_t datasize = CVPixelBufferGetDataSize( pixelBuffer );

			size_t bufferSize = bytesPerRow * height;
			if( bytesPerRow != width* self->mBpp  )
				for( size_t y = 0; y < height; ++y ) {
					const unsigned char* row = baseAddress + y*bytesPerRow;
					for (size_t x = 0; x < width; ++x) {
						self->mBuffer[0] = row[ 0 ];
						self->mBuffer[1] = row[ 1 ];
						if( self->mBpp == 3 ) self->mBuffer[2] = row[ self->mBpp == 4 ? 1 : 0];
						if( self->mBpp == 4 ) self->mBuffer[3] = 255  ; //is alpha
						self->mBuffer += self->mBpp;
						row += self->mBpp;

					}
				} 
			else{ memcpy( &self->mBuffer[0], &baseAddress[0], bufferSize ); }
			CVPixelBufferUnlockBaseAddress( pixelBuffer, 0 );

		}else{ // opengl upload

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
			if(wglMakeCurrent( wglGetCurrentDC() , wglGetCurrentContext())){
#else if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_APPLE
			CGLError error;
			error = CGLSetCurrentContext(  CGLGetCurrentContext()  );
			if( kCGLNoError != error ){
#endif


				CVPixelBufferLockBaseAddress( pixelBuffer, 0 ) ;
				unsigned char * imgdata = ( unsigned char* )(CVPixelBufferGetBaseAddress( pixelBuffer ));
				//
				const size_t bytesPerRow = CVPixelBufferGetBytesPerRow( pixelBuffer );
				const size_t width = CVPixelBufferGetWidth( pixelBuffer );
				const size_t height = CVPixelBufferGetHeight( pixelBuffer );
				const size_t datasize = CVPixelBufferGetDataSize( pixelBuffer );


				//Log("VIDEO WIDTH : " + NxVideoUtils::ToString( width ));
				//Log("DATA SIZE : " + NxVideoUtils::ToString( datasize ));
				//Log("Bytes per row : " + NxVideoUtils::ToString( bytesPerRow ));
				//Log("Bytes per row optimal : " + NxVideoUtils::ToString( width* self->mBpp ));
				//Log("Bpp : " + NxVideoUtils::ToString(  self->mBpp ));
				

				 /*
				if( bytesPerRow != width* self->mBpp  ){
					memset( imgdata, 255, datasize );

					Log("VIDEO WIDTH : " + NxVideoUtils::ToString( width ));
					Log("DATA SIZE : " + NxVideoUtils::ToString( datasize ));
					Log("Bytes per row : " + NxVideoUtils::ToString( bytesPerRow ));
					
 
					//faire une video dans after effects avec le meme format

					//	[8:36:17]: NxVideo : Video Width : 720
					//	[8:36:17]: NxVideo : Video Height : 480
					//	[8:36:17]: NxVideo : Video Total num frames : 6079
					//	[8:36:17]: NxVideo : Video Framerate : 29.97
					//	[8:36:17]: NxVideo : Audio Information : 
					//[8:36:17]: NxVideo : Audio SampleRate : 32000
					//	[8:36:17]: NxVideo : Audio Bits Per Sample : 32
					//	[8:36:17]: NxVideo : Audio Num Channels : 1
					 


				}*/
				 


				glBindTexture( GL_TEXTURE_2D, self->mGLID );
				int w, h;
				int miplevel = 0;
				glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

				//Log("VIDEO WIDTH 2: " + NxVideoUtils::ToString( w ));

				glTexSubImage2D( GL_TEXTURE_2D, 0 , 
					0, 0, w, h,
					GL_BGRA , GL_UNSIGNED_BYTE,
					imgdata );

				 
				CVPixelBufferUnlockBaseAddress( pixelBuffer, 0 ) ;
			}
		}

		self->FrameReady = true; 
 
	}
}
 

enum {
	kPlatformAny,
};

// Architectures we might be running on
enum Architecture {
	kArchitectureAny,
	kArchitectureIntel,
	kArchitecturePPC
};

// Flags indicating special cases for problem codecs
enum {
	kNo64ARGBPixelFormat = (1 << 0), // Don't use 64bit ARGB - it's broken
	kNo422YpCbCr8PixelFormat = (1 << 1), // Don't use 2vuy - it's broken
	kNo4444YpCbCrA8RPixelFormat = (1 << 2), // Don't use r408 - it's broken
	k64ARGBPixelFormatNativeByteOrder = (1 << 4), // Codec uses native byte order for 64bit ARGB (should be big-endian)
	kNo4444YpCbCrAFPixelFormat = (1 << 8), // Don't use r4fl - it's broken
	kDontTagGamma = (1 << 5), // Don't tag the movie with the gamma level
	kOnly64ARGBPixelFormat = (1 << 9), // Only support 64bit ARGB - everything else is broken
};

typedef struct
{
	OSType componentType;
	OSType componentSubType;
	OSType componentManufacturer;
	int platform;
	Architecture architecture;
	int flags;
} ComponentFlags;

// The list of dodgy codecs. We might want to make this a text file that users can edit when they acquire a new codec down the market.
static ComponentFlags gComponentFlags[] = {
	{ decompressorComponentType, 'r210', '2GMB', kPlatformAny, kArchitectureAny, kNo64ARGBPixelFormat },
	{ decompressorComponentType, 0,      'BMAG', kPlatformMacintosh, kArchitectureIntel, k64ARGBPixelFormatNativeByteOrder },
	{ decompressorComponentType, 0,      'Ajav', kPlatformMacintosh, kArchitectureIntel, k64ARGBPixelFormatNativeByteOrder },
	{ decompressorComponentType, 'mx3n',  'appl', kPlatformAny, kArchitectureAny, kNo4444YpCbCrA8RPixelFormat },
	{ decompressorComponentType, 'mx4n',  'appl', kPlatformAny, kArchitectureAny, kNo4444YpCbCrA8RPixelFormat },
	{ decompressorComponentType, 'mx5n',  'appl', kPlatformAny, kArchitectureAny, kNo4444YpCbCrA8RPixelFormat },

	{ decompressorComponentType, '2vuy',  'Ajav', kPlatformAny, kArchitectureAny, kNo422YpCbCr8PixelFormat | kNo64ARGBPixelFormat },
	{ decompressorComponentType, '2Vuy',  'Ajav', kPlatformAny, kArchitectureAny, kNo422YpCbCr8PixelFormat | kNo64ARGBPixelFormat },

	{ compressorComponentType, 'CFHD',  'cine', kPlatformAny, kArchitectureAny, kOnly64ARGBPixelFormat },
	{ decompressorComponentType, 'CFHD',  'cine', kPlatformAny, kArchitectureAny, kOnly64ARGBPixelFormat },
	{ compressorComponentType, 'AVdn',  'AVID', kPlatformAny, kArchitectureAny, kOnly64ARGBPixelFormat },
	{ decompressorComponentType, 'AVdn',  'AVID', kPlatformAny, kArchitectureAny, kOnly64ARGBPixelFormat },

	{ 0 }
};

enum {
	// Apple doesn't provide an enum for the r4fl pixel format, so we make one up....
	k4444YpCbCrAFPixelFormat = 'r4fl'
};

static void getCodecInfo( Component codec, OSType* pixelFormat, int* flags ) 
{
	*flags = 0;
	ComponentDescription cd;
	if ( GetComponentInfo( codec, &cd, NULL, NULL, NULL ) == noErr ) {
		for ( int i = 0; gComponentFlags[i].componentType; i++ ) {
			ComponentFlags* f = &gComponentFlags[i];
			if (
				(f->componentType == 0 || f->componentType == cd.componentType) &&
				(f->componentSubType == 0 || f->componentSubType == cd.componentSubType) &&
				(f->componentManufacturer == 0 || f->componentManufacturer == cd.componentManufacturer )
#if __APPLE__
				&& (f->platform == kPlatformAny || f->platform == kPlatformMacintosh)
#else 
				&& (f->platform == kPlatformAny || f->platform == kPlatformWindows)
#endif
				/*
#ifdef FN_PROCESSOR_PPC
				&& (f->architecture == kArchitectureAny || f->architecture == kArchitecturePPC)
#endif
#ifdef FN_PROCESSOR_INTEL
				&& (f->architecture == kArchitectureAny || f->architecture == kArchitectureIntel)
#endif*/
				) {
					*flags = f->flags;
					break;
			}
		}
	}

	*pixelFormat = 0;
	OSTypePtr* hResource = NULL;
	OSErr err = GetComponentPublicResource( codec, 'cpix', 1, (Handle*)&hResource );
	if ( err == noErr && hResource ) {
		int count = GetHandleSize( (Handle)hResource ) / 4;

		static OSType preferredFormats[] = {
			k4444YpCbCrAFPixelFormat,
			k4444YpCbCrA8RPixelFormat,
			k422YpCbCr8PixelFormat, 
			k64ARGBPixelFormat,
			k32ARGBPixelFormat
		};

		unsigned int bestFormat = INT_MAX;
		for ( int i = 0; i < count; i++ ) {
			OSType format = (*hResource)[i];

			for ( unsigned int j = 0; j < sizeof(preferredFormats) / sizeof(OSType); j++ ) {
				// Some codecs claim to support particular formats, but get it wrong.
				if ( format == k64ARGBPixelFormat && (*flags & kNo64ARGBPixelFormat) )
					continue;
				if ( format == k64ARGBPixelFormat && (*flags & kNo64ARGBPixelFormat) )
					continue;
				if ( format == k4444YpCbCrA8RPixelFormat && (*flags & kNo4444YpCbCrA8RPixelFormat) )
					continue;
				if ( format == k422YpCbCr8PixelFormat && (*flags & kNo422YpCbCr8PixelFormat) ) 
					continue;
				if ( format == k4444YpCbCrAFPixelFormat && (*flags & kNo4444YpCbCrAFPixelFormat) )
					continue;
				if ( format != k64ARGBPixelFormat && (*flags & kOnly64ARGBPixelFormat))
					continue;
				if ( format == preferredFormats[j] && j < bestFormat ) {
					*pixelFormat = format;
					bestFormat = j;
				}
			}
		}

		DisposeHandle( (Handle)hResource );
	}
	if ( *pixelFormat == 0 )
		*pixelFormat = k32ARGBPixelFormat;

	if ( *pixelFormat == k4444YpCbCrAFPixelFormat ) {
		// Now a magical special case. It turns out, after long investigation, that although the r4fl pixel format is
		// registered with Core Video, it's not registered with ICM. If we find this to be the case, register it ourselves.
		// If we don't register it, attempts to use it with codecs result in cDepthErr.
		ICMPixelFormatInfo pixelFormatInfo;
		pixelFormatInfo.size = sizeof(ICMPixelFormatInfo);
		OSErr err = ICMGetPixelFormatInfo( *pixelFormat, &pixelFormatInfo );
		if ( err != noErr ) {
			memset( &pixelFormatInfo, 0, sizeof(pixelFormatInfo) );
			pixelFormatInfo.size  = sizeof(ICMPixelFormatInfo);
			pixelFormatInfo.formatFlags = 0;
			pixelFormatInfo.bitsPerPixel[0] = 128;

			err = ICMSetPixelFormatInfo( *pixelFormat, &pixelFormatInfo );
		}
	}
}

void GetCodecName( CodecType type, std::string & ret ) {
 
	switch( type ) {
	case kJPEG2000CodecType : ret = "kJPEG2000CodecType"; break;
	case kH264CodecType : ret = "kH264CodecType"; break; 
	case kAnimationCodecType : ret = "kAnimationCodecType"; break; 
	case kPNGCodecType : ret = "kPNGCodecType"; break; 
	case kSorenson3CodecType : ret = "kSorenson3CodecType"; break; 
	case kJPEGCodecType : ret = "kJPEGCodecType"; break; 
	case kMotionJPEGACodecType : ret = "kMotionJPEGACodecType"; break; 
	case kMotionJPEGBCodecType : ret = "kMotionJPEGBCodecType"; break; 
	default : ret = "GetCodecName Not defined !"; break;
	}

	 
}

unsigned short GetVideoFrameDepth( ImageDescriptionHandle imgDesc )
{
	return (*imgDesc)->depth; 
}

ImageDescriptionHandle GetImageDescription( Media media ) {

	MediaSampleFlags sampleFlags = 0;
	TimeValue64 decodeTime;

	//We use the first sample for the session regardless of the current frame
	SampleNumToMediaDecodeTime(media, 1, &decodeTime, NULL);

	if ( GetMoviesError() != noErr ) {
		Log("Error finding decode time for start of movie");
	}

	ImageDescriptionHandle imageDesc = (ImageDescriptionHandle)NewHandle(0);

	//We have to run this first to get the sample description
	OSStatus err_ = GetMediaSample2(media,
		NULL, //data out
		0,   //max data size
		NULL, //bytes
		decodeTime, //decode time
		NULL, //returned decode time
		NULL, //duration per sample
		NULL, //offset
		(SampleDescriptionHandle) imageDesc,
		NULL, //sample description index
		1, //max number of samples
		NULL, //number of samples
		&sampleFlags //flags
		);

	if ( err_ != noErr ) {
		DisposeHandle( (Handle)imageDesc );
		Log("Failed to size up media sample");
	}

	return imageDesc;

}

unsigned short GetVideoColorDepth( Media media ) {
	ImageDescriptionHandle imageDesc = GetImageDescription( media );
	unsigned short depth = GetVideoFrameDepth( imageDesc );
	DisposeHandle( (Handle)imageDesc );
	return depth;
}

 

void createDecompressionSession( Media media, int width, int height, int mBpp, NxVideoQuickTime * ptr )
{
	CFMutableDictionaryRef pixelAttributes = NULL;
	ImageDescriptionHandle imageDesc = GetImageDescription( media );

	Log("Source Image description : ");
	std::string codecname; GetCodecName( (**imageDesc).cType, codecname );
	Log("Source Codec : " + codecname );
	char name[255]; CopyPascalStringToC((**imageDesc).name , name );
	Log("Source Name : " + std::string( name ) );
 	Log("Source Width : " + NxVideoUtils::ToString( (**imageDesc).width ));
 	Log("Source Height : " + NxVideoUtils::ToString( (**imageDesc).height ));
	Log("Source Depth : " + NxVideoUtils::ToString( (**imageDesc).depth ));
	Log("Source Horizontal resolution : " + NxVideoUtils::ToString( (**imageDesc).hRes ));
	Log("Source Vertical resolution : " + NxVideoUtils::ToString( (**imageDesc).vRes ));
	Log( "Source Datasize : " + NxVideoUtils::ToString(  (**imageDesc).dataSize  )  );
	Log("Source Image description : Done");

	ComponentDescription cd = { decompressorComponentType, 0, 0, 0, cmpIsMissing };
	Component decompressor = 0;
	cd.componentSubType = (*imageDesc)->cType;
	decompressor = FindNextComponent( 0, &cd );
 
	OSType manufacturer_ = 0;
	if ( GetComponentInfo( decompressor, &cd, NULL, NULL, NULL ) == noErr ) {
		manufacturer_ = cd.componentManufacturer; 
		Log( "Video Decompressor : " );
		Log( "Decompressor Type : " + NxVideoUtils::FOURCC2Str( cd.componentType ));
		Log( "Decompressor SubType : " + NxVideoUtils::FOURCC2Str( cd.componentSubType ));
		Log( "Decompressor Manufacturer : " + NxVideoUtils::FOURCC2Str( cd.componentManufacturer ));
		Log( "Video Decompressor : Done" );	
	}else {
		Log( "Unable to find decompressor !" );
	}

	//Setup output pixel format
	pixelAttributes = CFDictionaryCreateMutable( kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks );

	if ( !pixelAttributes ) {
		DisposeHandle( (Handle)imageDesc );
		Log("Couldn't create dictionary");
	}


	//width = width >> 16;
	//height = height >> 16;
 
	addIntToDictionary( pixelAttributes, kCVPixelBufferWidthKey, width );
	addIntToDictionary( pixelAttributes, kCVPixelBufferHeightKey, height );
 
 
	SInt32 pixel_format = ptr->mFormatDst;// k24RGBPixelFormat : k32ARGBPixelFormat;
	CFNumberRef number = CFNumberCreate( NULL, kCFNumberSInt32Type, &pixel_format );
	CFDictionaryAddValue( pixelAttributes, kCVPixelBufferPixelFormatTypeKey, number );
	CFRelease( number );




	//addIntToDictionary( pixelAttributes, kCVPixelBufferBytesPerRowAlignmentKey, 16 );
	//720 / 32 = 16
	//720 360 180 90 45 22.5

	//550 * 4 is not a multiple of 16.   | 16 32 64 128 256 512
	 addIntToDictionary( pixelAttributes, kCVPixelBufferBytesPerRowAlignmentKey, 16 ); // 2 byte

	 


	 /*
	OSType pixelFormat;
	int codecFlags_;
	getCodecInfo( decompressor, &pixelFormat, &codecFlags_ );
	// The BlackMagic r210 decoder ciaims to support k64ARGBPixelFormat, but actually returns 8-bit data
	addIntToDictionary( pixelAttributes, kCVPixelBufferPixelFormatTypeKey, pixelFormat );
	*/

	// Create Decompression Session
	ICMDecompressionTrackingCallbackRecord callbackRecord;
	callbackRecord.decompressionTrackingCallback = emitFrame;
	callbackRecord.decompressionTrackingRefCon = ptr;

	ICMDecompressionSessionOptionsRef sessionOptions = NULL;
	OSStatus err_ = ICMDecompressionSessionOptionsCreate( kCFAllocatorDefault, &sessionOptions );
	if ( err_ == noErr ) {
 
		// Turn on high quality for decompression
		CodecQ codecAccuracy = codecHighQuality;
		err_ = ICMDecompressionSessionOptionsSetProperty( sessionOptions,
		kQTPropertyClass_ICMDecompressionSessionOptions,
		kICMDecompressionSessionOptionsPropertyID_Accuracy,
		sizeof(CodecQ), &codecAccuracy );
		if ( err_ != noErr ) { Log( "error : kICMDecompressionSessionOptionsPropertyID_Accuracy"); } 
	 
		//set B - FRAMES used in h264
		Boolean DisplayOrderRequired = TRUE;// true per default
		err_ = ICMDecompressionSessionOptionsSetProperty( sessionOptions , 
		kQTPropertyClass_ICMDecompressionSessionOptions, 
		kICMDecompressionSessionOptionsPropertyID_DisplayOrderRequired, 
		sizeof(DisplayOrderRequired), &DisplayOrderRequired);
		if ( err_ != noErr ) { Log( "error : kICMDecompressionSessionOptionsPropertyID_DisplayOrderRequired"); } 

		//set max buffers count
		UInt32 maxbuffer = 1;
		err_ = ICMDecompressionSessionOptionsSetProperty( sessionOptions , 
		kQTPropertyClass_ICMDecompressionSessionOptions, 
		kICMDecompressionSessionOptionsPropertyID_MaxBufferCount,
		sizeof(maxbuffer), &maxbuffer);
		if ( err_ != noErr ) { Log( "error : kICMDecompressionSessionOptionsPropertyID_MaxBufferCount"); } 
 
	}else{
		Log( "error : ICMDecompressionSessionOptionsCreate");

	}
 
	err_ = ICMDecompressionSessionCreate(kCFAllocatorDefault, //allocator
		imageDesc, //image description
		sessionOptions, //decompression options
		pixelAttributes, //buffer attributes
		&callbackRecord,
		&ptr->session_
		);

	if ( sessionOptions )
		ICMDecompressionSessionOptionsRelease( sessionOptions );



	if (err_ == noCodecErr)
		Log("No suitable codec found for this movie");
	else if (err_ != noErr)
		Log("Failed to create decompression session");

	CFRelease(pixelAttributes);
	DisposeHandle( (Handle)imageDesc );
}

Track find_audio_track( NxVideoQuickTime * ptr )
{
	long trackcount = GetMovieTrackCount(ptr->MovieQT );
	Track pt = NULL;
	for (long i = 1; i <= trackcount; i++) {
		Track track = GetMovieIndTrack(ptr->MovieQT, i);
		if ( track != NULL ) {
			OSType type;
			GetMediaHandlerDescription(GetTrackMedia(track), &type, NULL, NULL); 
			if ( type == SoundMediaType || type == MusicMediaType ) {
				pt = track;
				i = trackcount + 1; //we only need one
			}
		}
	}

	return pt;
}


Track find_video_track( NxVideoQuickTime * ptr )
{
	long trackcount = GetMovieTrackCount( ptr->MovieQT );
	Track pt = NULL;
	for (long i = 1; i <= trackcount; i++) {
		Track track = GetMovieIndTrack( ptr->MovieQT, i);
		if ( track != NULL ) {
			OSType type;
			GetMediaHandlerDescription(GetTrackMedia(track), &type, NULL, NULL); 
			if ( type == VideoMediaType || type == MPEGMediaType || type == MovieMediaType || type == URLDataHandlerSubType ) {
					pt = track;
					i = trackcount + 1; //we only need one
			}
		}
	}

	return pt;
}

 
unsigned int open2( int posframe, int mBpp, NxVideoQuickTime * ptr ){

	OSStatus err = noErr;
 
	if ( ptr->pixelBuffer_ ) {
		//CVPixelBufferUnlockBaseAddress( ptr->pixelBuffer_, 0 );
		//CVPixelBufferRelease( ptr->pixelBuffer_ );
		ptr->pixelBuffer_ = NULL;
	}

	Media videoMedia = GetTrackMedia( ptr->mVideoTrack );
	if ( !ptr->session_ ) {
		Log("Creating createDecompressionSession .." );
		Fixed mheight, mwidth;
		GetTrackDimensions(ptr->mVideoTrack, &mwidth, &mheight);
		int width =  FixedToInt(mwidth);
		int height =  FixedToInt(mheight);
		createDecompressionSession( videoMedia, width, height, mBpp, ptr);
	}

	SInt64 syncSampleNumber, nextSampleNumber;
	TimeValue64 targetDecodeTime, syncDecodeTime;

	// Find the last key frame (sync sample) at or before the target sample number.
	SampleNumToMediaDecodeTime( videoMedia, posframe, &targetDecodeTime, NULL );
 
	GetMediaNextInterestingDecodeTime( videoMedia,
		nextTimeSyncSample | nextTimeEdgeOK,
		targetDecodeTime,
		-fixed1,
		&syncDecodeTime,
		NULL );

	err = GetMoviesError();
 
	MediaDecodeTimeToSampleNum( videoMedia, syncDecodeTime, &syncSampleNumber, NULL, NULL );

	// Pick the starting point.
	if( ( ptr->lastDecompressedSampleNumber + 1 <= posframe ) && ( syncSampleNumber < ptr->lastDecompressedSampleNumber + 1 ) )
		nextSampleNumber = ptr->lastDecompressedSampleNumber + 1;
	else
		nextSampleNumber = syncSampleNumber;

	// Walk forward in decode order.
	 for( ; nextSampleNumber <= posframe; nextSampleNumber++ ) {

		TimeValue64 sampleDecodeTime;
		ByteCount sampleDataSize = 0;
		MediaSampleFlags sampleFlags = 0;
		UInt8 *sampleData = NULL;
		ICMFrameTimeRecord frameTime = {0};

		// Get the frame's data size and sample flags.  

		SampleNumToMediaDecodeTime( videoMedia, nextSampleNumber, &sampleDecodeTime, NULL );
		err = GetMediaSample2( videoMedia, NULL, 0, &sampleDataSize, sampleDecodeTime,
			NULL, NULL, NULL, NULL, NULL, 1, NULL, &sampleFlags );

		// We can skip droppable frames before the target.
		if( ( nextSampleNumber != posframe ) && ( mediaSampleDroppable & sampleFlags ) )
		 	continue;

		// Load the frame.
		sampleData = (UInt8*)malloc( sampleDataSize );

		err = GetMediaSample2( videoMedia, sampleData, sampleDataSize, NULL, sampleDecodeTime,
			NULL, NULL, NULL, NULL, NULL, 1, NULL, NULL );

		// Set up an immediate decode request -- we don't care about frame times, we just need to pass a flag.
		frameTime.recordSize = sizeof(ICMFrameTimeRecord);
		*(TimeValue64 *)&frameTime.value = sampleDecodeTime;
		frameTime.scale = GetMediaTimeScale( videoMedia );
		frameTime.rate = fixed1;
		frameTime.frameNumber = nextSampleNumber;

		// If we haven't reached the target sample, tell the session not to emit the frame.
		 if( nextSampleNumber != posframe )
		 	frameTime.flags = icmFrameTimeDoNotDisplay;

		// Decode the frame. 
		err = ICMDecompressionSessionDecodeFrame( ptr->session_, sampleData, sampleDataSize, NULL, &frameTime, sampleData );

		// Note: trackFrame will release the sampleData buffer.
		ptr->lastDecompressedSampleNumber = nextSampleNumber;

	 }	 

	return 0;

}

/*
//to call each frame
unsigned int open( int posframe, int mBpp, NxVideoQuickTime * ptr )
{
 
	//Each frame should only be decoded once.
	if ( posframe == ptr->mFramePosition && ptr->pixelBuffer_ ) {
		Log("open returning : sample nun : " + NxVideoUtils::ToString( posframe ));
		return 0;
	}
	ptr->mFramePosition = posframe;
	 

	// As there's no close() call, we release the previous pixel buffer here.
	if ( ptr->pixelBuffer_ ) {
		//Log("CVPixelBufferUnlockBaseAddress");
		CVPixelBufferUnlockBaseAddress( ptr->pixelBuffer_, 0 );
		CVPixelBufferRelease( ptr->pixelBuffer_ );
		ptr->pixelBuffer_ = NULL;
	
		
	}
  
	/////////////////////////////////////////////////

	Media videoMedia = GetTrackMedia( ptr->mVideoTrack );

	if ( !ptr->session_ ) {
		Log("Creating createDecompressionSession .." );
		Fixed mheight, mwidth;
		GetTrackDimensions(ptr->mVideoTrack, &mwidth, &mheight);
		int width =  FixedToInt(mwidth);
		int height =  FixedToInt(mheight);
		createDecompressionSession( videoMedia, width, height, mBpp, ptr);
	}


	if ( !ptr->session_ ) {
		Log("Creating createDecompressionSession is null " );
		return 0;
	}

	// Walk forward in decode order.
	OSStatus err = noErr;
 
	SInt64 syncSampleNumber, nextSampleNumber;

	TimeValue64 targetDecodeTime, syncDecodeTime;


	SInt64 targetSampleNumber = posframe;
	 SampleNumToMediaDisplayTime  ( videoMedia, targetSampleNumber, &targetDecodeTime, NULL );

	err = GetMoviesError();
	if ( err != noErr ) {		 
		Log("Failed SampleNumToMediaDecodeTime");
		return 0;
	} 


	GetMediaNextInterestingDisplayTime(videoMedia,
		nextTimeSyncSample | nextTimeEdgeOK,
		targetDecodeTime,
		-fixed1,
		&syncDecodeTime,
		NULL );

	err = GetMoviesError();
	if ( err != noErr ) {		 
		Log("Failed GetMediaNextInterestingDecodeTime");
		return 0;
	}

	MediaDisplayTimeToSampleNum( videoMedia, syncDecodeTime, &syncSampleNumber, NULL, NULL );

	err = GetMoviesError();
	if ( err != noErr ) {		 
		Log("Failed MediaDecodeTimeToSampleNum");
		return 0;
	}   

	// Pick the starting point.

	if( ( ptr->lastDecompressedSampleNumber + 1 <= targetSampleNumber ) && ( syncSampleNumber < ptr->lastDecompressedSampleNumber + 1 ) )
		nextSampleNumber = ptr->lastDecompressedSampleNumber + 1;
	else
		nextSampleNumber = syncSampleNumber;

	  for( ; nextSampleNumber <= targetSampleNumber; nextSampleNumber++ ) {

		TimeValue64 sampleDecodeTime;
		TimeValue64 sampleDecodeDurationPerSamples;
		ByteCount sampleDataSize = 0;
		MediaSampleFlags sampleFlags = 0;
		UInt8 *sampleData = NULL;
		ICMFrameTimeRecord frameTime = {0};

		// Get the frame's data size and sample flags.  
		 SampleNumToMediaDecodeTime( videoMedia, nextSampleNumber, &sampleDecodeTime, NULL );
		// SampleNumToMediaDisplayTime  ( videoMedia, nextSampleNumber, &sampleDecodeTime, NULL );
 
		TimeValue64 sampledecodetime;
		TimeValue64 sampledecodetimeOffset;
		err = GetMediaSample2( videoMedia, NULL, 0, &sampleDataSize, sampleDecodeTime, &sampledecodetime, &sampleDecodeDurationPerSamples, &sampledecodetimeOffset, NULL, NULL, 1, NULL, &sampleFlags );
		if ( err != noErr ) {		 
			Log("Failed GetMediaSample2 1 ");
			return 0;
		} 


		// We can skip droppable frames before the target.
 
		if( ( nextSampleNumber != targetSampleNumber ) && ( mediaSampleDroppable & sampleFlags ) )
			continue;

		// Load the frame.
		sampleData = (UInt8*)malloc( sampleDataSize );

		err = GetMediaSample2( videoMedia, sampleData, sampleDataSize, NULL, sampleDecodeTime, NULL, NULL, NULL, NULL, NULL, 1, NULL, NULL );
		if ( err != noErr ) {		 
			Log("Failed GetMediaSample2 2 ");
			return 0;
		} 

		// Set up an immediate decode request -- we don't care about frame times, we just need to pass a flag.
		
		frameTime.recordSize = sizeof(ICMFrameTimeRecord);
		*(TimeValue64 *)&frameTime.value = sampleDecodeTime;
		frameTime.scale = GetMediaTimeScale( videoMedia );
		frameTime.rate = fixed1;
		frameTime.frameNumber = 0;//nextSampleNumber;
		//If we haven't reached the target sample, tell the session not to emit the frame.

		Log("Decoding sample TIME : " + NxVideoUtils::ToString( sampledecodetime )   );
		Log("Decoding sample NUM : " + NxVideoUtils::ToString( nextSampleNumber )   );

		if( nextSampleNumber != targetSampleNumber )
			frameTime.flags = icmFrameTimeDoNotDisplay;

		ptr->mCurrentDecodeTime = sampledecodetime + sampledecodetimeOffset; //nextSampleNumber;
		ptr->mCurrentDecodeTimeTarget = sampleDecodeTime;

		  // frameTime.duration = sampleDecodeDurationPerSamples;//sampleDecodeDurationPerSamples;
		  //frameTime.decodeTime =  sampledecodetime;//ptr->mCurrentDecodeTime;
		   // frameTime.flags = icmFrameTimeIsNonScheduledDisplayTime;

 
 
		//Log("mCurrentDecodeTime : " + NxVideoUtils::ToString( nextSampleNumber ) + " VS " + "mCurrentDecodeTimeTarget : " + NxVideoUtils::ToString( targetSampleNumber ));

		// Decode the frame. 
		err = ICMDecompressionSessionDecodeFrame( ptr->session_, sampleData, sampleDataSize, NULL, &frameTime, sampleData );
		if ( err != noErr ) {		 
			Log("Failed ICMDecompressionSessionDecodeFrame");
			return 0;
		} 

		 

		//Log("Setting time to decode : " + NxVideoUtils::ToString( sampledecodetime )  );


		//  ICMDecompressionSessionSetNonScheduledDisplayTime(ptr->session_, sampledecodetime , GetMediaTimeScale( videoMedia ),0  );

		//ICMDecompressionSessionFlush(ptr->session_);

		// Note: trackFrame will release the sampleData buffer.
		ptr->lastDecompressedSampleNumber = nextSampleNumber;
 
	  }    
 
	//ICMDecompressionSessionFlush(ptr->session_);
	 
 
	return 0;
 
}
*/



///////////////////
enum NxVideoLoopOptions
{
	NxLoopOneWay,
	NxLoopBounce,
	NxLoopNone
};

void setLoopState( NxVideo_Mov_Player * Ptr, NxVideoLoopOptions state )
{
	TimeBase myTimeBase;
	long myFlags = 0L;
	myTimeBase = GetMovieTimeBase( Ptr->m_Video->MovieQT );
	myFlags = GetTimeBaseFlags(myTimeBase);
	switch (state) {
case NxLoopOneWay:
	myFlags |= loopTimeBase;
	myFlags &= ~palindromeLoopTimeBase;
	SetMoviePlayHints(Ptr->m_Video->MovieQT, hintsLoop, hintsLoop);
	SetMoviePlayHints(Ptr->m_Video->MovieQT, 0L, hintsPalindrome);
	break;
case NxLoopBounce:
	myFlags |= loopTimeBase;
	myFlags |= palindromeLoopTimeBase;
	SetMoviePlayHints(Ptr->m_Video->MovieQT, hintsLoop, hintsLoop);
	SetMoviePlayHints(Ptr->m_Video->MovieQT, hintsPalindrome, hintsPalindrome);
	break;
case NxLoopNone:
default:
	myFlags &= ~loopTimeBase;
	myFlags &= ~palindromeLoopTimeBase;
	SetMoviePlayHints(Ptr->m_Video->MovieQT, 0L, hintsLoop | hintsPalindrome);
	break;
	}

	SetTimeBaseFlags(myTimeBase, myFlags);
	SetMoviePlayHints( Ptr->m_Video->MovieQT, hintsOffscreen | hintsDontUseVideoOverlaySurface, hintsOffscreen | hintsDontUseVideoOverlaySurface ); 

}

//////////////////



NxVideo_Mov_Player::NxVideo_Mov_Player() : NxVideo_Player()
{
	mVideoType = NxVideoMovFile;
}

NxVideo_Mov_Player::~NxVideo_Mov_Player()  
{
	if(m_Video) CloseVideoFile();
 
}

bool CreateAudioRessource( NxVideoQuickTime * ptr, NxVideoPixelFormatInfo info, unsigned short & channels, unsigned long & samplerate  ){

	//begin audio extraction
	OSStatus err = MovieAudioExtractionBegin( ptr->MovieQT , 0, &ptr->mAudioSession);
	if (err != noErr) return false;

	//conversion quality if needed ( different sample rates )
	UInt32 quality = kQTAudioRenderQuality_High;
	err = MovieAudioExtractionSetProperty( ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_RenderQuality,
		sizeof(quality),
		&quality);
	if (err != noErr) return false;

	  
	//get channel layout
	unsigned long output_layout_size;
	err = MovieAudioExtractionGetPropertyInfo( ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_AudioChannelLayout,
		0, &output_layout_size, 0);
	if (err != noErr) return false;
	//allocate the layout data
	AudioChannelLayout * qt_audio_channel_layout = (AudioChannelLayout *) calloc(1, output_layout_size);
	//set layout data from channel layout
	MovieAudioExtractionGetProperty (ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_AudioChannelLayout,
		output_layout_size, qt_audio_channel_layout, 0);

	/*
	//assign the channel layout tag.
	qt_audio_channel_layout[0].mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
	//set the channel layout tag.
	MovieAudioExtractionSetProperty (ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_AudioChannelLayout,
		output_layout_size, qt_audio_channel_layout);*/
	//get audio stream description
	err = MovieAudioExtractionGetProperty (ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_AudioStreamBasicDescription,
		sizeof (ptr->inputStreamDesc), &ptr->inputStreamDesc, 0);
	if (err != noErr) return false;
 

	//assign the channel layout tag.
	qt_audio_channel_layout[0].mChannelLayoutTag = 
		ptr->inputStreamDesc.mChannelsPerFrame == 1 ? kAudioChannelLayoutTag_Mono : kAudioChannelLayoutTag_Stereo;
 
	//set the channel layout tag.
	MovieAudioExtractionSetProperty (ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_AudioChannelLayout,
		output_layout_size, qt_audio_channel_layout);


	// get audio bits per samples format.
	long audioformat = ConvertAudioFormat( info.mDstAudioFormat );
	std::size_t audioPrecision = (audioformat == kAudioFormatFlagIsFloat) ? sizeof(float) : sizeof(SInt16);
 
	ptr->inputStreamDesc.mFormatFlags = audioformat | kAudioFormatFlagIsPacked | kAudioFormatFlagsNativeEndian; //interleaved per default;  // | kAudioFormatFlagsNativeEndian
	ptr->inputStreamDesc.mBitsPerChannel = audioPrecision * 8; 
	ptr->inputStreamDesc.mBytesPerFrame = audioPrecision * ptr->inputStreamDesc.mChannelsPerFrame;
	ptr->inputStreamDesc.mBytesPerPacket = ptr->inputStreamDesc.mBytesPerFrame;
	// destination sample rate doesnt work correctly ! do linear interpolation ourselves.. thx quicktime.
	// ptr->inputStreamDesc.mSampleRate = info.mDstAudioSampleRate;//44100;
 
	// set the desired audio output format
	err = MovieAudioExtractionSetProperty (ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Audio,
		kQTMovieAudioExtractionAudioPropertyID_AudioStreamBasicDescription,
		sizeof (ptr->inputStreamDesc), &ptr->inputStreamDesc);
	if (err != noErr) return false;

	//disable channels discrete. ( multi channel audio ) // disable mixing of audio channels
	Boolean allChannelsDiscrete = false;
	err = MovieAudioExtractionSetProperty (ptr->mAudioSession,
		kQTPropertyClass_MovieAudioExtraction_Movie,
		kQTMovieAudioExtractionMoviePropertyID_AllChannelsDiscrete,
		sizeof (allChannelsDiscrete), &allChannelsDiscrete);
	if (err != noErr) return false;

	// allocate an audio buffer to hold the audio data
	ptr->abl = (AudioBufferList*)calloc(1, sizeof(AudioBufferList) + (ptr->inputStreamDesc.mChannelsPerFrame - 1) * sizeof(AudioBuffer));
	ptr->abl->mNumberBuffers = 1;
	ptr->abl->mBuffers[0].mNumberChannels = ptr->inputStreamDesc.mChannelsPerFrame ;
	unsigned long DstAudioBufferSize = info.mDstAudioBufferSize;
	ptr->abl->mBuffers[0].mDataByteSize = DstAudioBufferSize*audioPrecision*ptr->inputStreamDesc.mChannelsPerFrame;
	ptr->abl->mBuffers[0].mData = malloc( ptr->abl->mBuffers[0].mDataByteSize );//DstAudioBufferSize*audioPrecision*ptr->inputStreamDesc.mChannelsPerFrame);
	ptr->abl->mBuffers[0].mNumberChannels = ptr->inputStreamDesc.mChannelsPerFrame;	
 
	// when you're done extracting, remember to free each mData in the bufferlist, then free the abl itself!


	samplerate = ptr->inputStreamDesc.mSampleRate;
	channels   = ptr->inputStreamDesc.mChannelsPerFrame;
 
	return true;
}

bool NxVideo_Mov_Player::OpenVideoFile( const std::string & szFileName, NxVideoPixelFormatInfo & info )
{
	if( !NxVideoManager::getSingleton().IsQuicktimeEnabled()){
		Log("NxVideo_Mov_Player::OpenVideoFile : failed, not quicktime support");
		return false;
	}
	 

	m_Video = new NxVideoQuickTime();

	mVideoFullPath = std::string( szFileName );//save the video full path
	std::string Tmp = mVideoFullPath ;

	NxVideoUtils::SplitFileName( Tmp );
	mVideoFullName = Tmp;

	NxVideoUtils::SplitFileExtension( Tmp );
	mVideoExtension = Tmp;

	Log("Opening file...");
	////////

	FSSpec sfFile;
#ifdef __APPLE__
	FSRef ref; // intermediate struct
	FSPathMakeRef( (const UInt8*)szFileName.c_str(), &ref, NULL );
	if ( FSGetCatalogInfo( &ref, kFSCatInfoNone , NULL, NULL, &sfFile, NULL) != noErr ){
		Log("FSGetCatalogInfo Failed!" );
		return false;
	}
#else
	if ( NativePathNameToFSSpec ((char*) (const char*) szFileName.c_str(), &sfFile, 0) != noErr ){
		Log("NativePathNameToFSSpec Failed!" );
		return false;
	}
#endif
	Log( "OpenMovieFile ..." );
	short movieResFile;
	OSErr err = OpenMovieFile (&sfFile, &movieResFile, fsRdPerm);
	if (err == noErr) {
		short movieResID = 0;
		Log( "NewMovieFromFile ..." );
		err = NewMovieFromFile(&m_Video->MovieQT, movieResFile, &movieResID, (unsigned char *) 0, newMovieActive, (Boolean *) 0);
		if (err == noErr){
					Log( "CloseMovieFile ..." );
			CloseMovieFile (movieResFile);
		} else {
			Log( "NewMovieFromFile failed" );
			return false;
		}
	} else {
		Log("OpenMovieFile failed " );
		return false;
	}
 
	m_Video->mVideoTrack = find_video_track( m_Video );
	if( m_Video->mVideoTrack != NULL ) {

		// set video context to null - we will be using decompress session.
		::SetMovieVisualContext( m_Video->MovieQT, NULL );

		Rect r;
		GetMovieBox( m_Video->MovieQT, &r);
		mVideoWidth[0] = r.right;
		mVideoHeight[0] = r.bottom;
		m_Video->mVideoMedia = GetTrackMedia( m_Video->mVideoTrack );
 
		if(mVideoWidth[0] % 32)
			mVideoWidth[0] += 32 - (mVideoWidth[0] % 32);

	 


		//720 / 32 = 16
		//720 360 180 90 45 22.5
		// 736
		//550 * 4 is not a multiple of 16.   | 16 32 64 128 256 512


		mFrameInbound = 0;
		mFrameOutbound = GetVideoDurationframes();

		mVideoBpp = GetVideoBppFromPixelFormat( info.mDstVideoFormat );
		m_Video->mBpp = mVideoBpp;

		mNumPlanes = 1; // TODO verify
		info.mWidth = mVideoWidth[0];
		info.mHeight = mVideoHeight[0];
		info.mBpp = mVideoBpp;
		m_Video->mFormatDst = ConvertVideoFormat( info.mDstVideoFormat );

		m_Video->mBuffer = new unsigned char [ mVideoWidth[0] *  mVideoHeight[0] *  mVideoBpp ];
 

		// SetMoviePlayHints( m_Video->MovieQT, hintsAllowInterlace + hintsHighQuality, 0xFFFFFFFF );

		Log("Video Width : " + NxVideoUtils::ToString( mVideoWidth  ));
		Log("Video Height : " + NxVideoUtils::ToString( mVideoHeight  ));
		Log("Video Total num frames : " + NxVideoUtils::ToString( mFrameOutbound ) );
		Log("Video Framerate : " + NxVideoUtils::ToString( GetFramerate() ) );

		mHasVideo = true;
 
	}

	m_Video->mAudioTrack = find_audio_track( m_Video );
	if( m_Video->mAudioTrack != NULL ) {
		/* no need to create a context, we process the samples elsewhere.
		QTAudioContextRef audioContext;
		QTAudioContextCreateForAudioDevice(NULL, NULL, NULL, &audioContext);
		SetMovieAudioContext(m_Video->MovieQT, audioContext);*/
		CreateAudioRessource( m_Video, info, mAudioNumChannels, mAudioSampleRate  );
		mAudioBufferSize = info.mDstAudioBufferSize;
 
		std::size_t BpS = (info.mDstAudioFormat == NxVideoPixelFormat32BpS) ? sizeof(float) : sizeof(SInt16);
		mAudioBitsperSamples = BpS * 8;
 
		Log("Audio Information : " );
		Log("Audio SampleRate : " + NxVideoUtils::ToString( mAudioSampleRate ) );
		Log("Audio Bits Per Sample : " + NxVideoUtils::ToString( mAudioBitsperSamples ) );
		Log("Audio Num Channels : " + NxVideoUtils::ToString( mAudioNumChannels ) );
 
		mHasAudio = true;
	}  

	if( !mHasVideo /*&& !mHasAudio */ ) return false;

	GoToBeginningOfMovie( m_Video->MovieQT );
	setLoopState( this,  NxLoopOneWay );
	StartMovie(m_Video->MovieQT);
	SetPlayBackState(VIDEO_STATUS_PLAY);
 
	return true;
}

bool NxVideo_Mov_Player::CloseVideoFile()
{
	Log( "NxVideo_Mov_Player::CloseVideoFile() ... " );
	ClearMoviesStickyError();
	mVideoPlayState = VIDEO_STATUS_STOP;
	 
	DisposeMovie( m_Video->MovieQT );
	StopMovie( m_Video->MovieQT );
	 Log("Closing Quicktime video File" ) ;
	CloseMovieFile( m_Video->movieResFile );
	//m_Video->Buffer ? delete m_Video->Buffer : Log("No release m_Video->Buffer" ) ;
	if(m_Video)delete m_Video;
	m_Video = NULL;
	Log( "NxVideo_Mov_Player::CloseVideoFile() : DONE " );

 
	return true;
}

void NxVideo_Mov_Player::Play()
{
	GoToBeginningOfMovie( m_Video->MovieQT );
	StartMovie( m_Video->MovieQT );
	MoviesTask( m_Video->MovieQT, 0 );
	SetPlayBackState( VIDEO_STATUS_PLAY );
	for( std::vector< NxVideoPlayerListener * >::iterator Iter = mListeners.begin(); Iter != mListeners.end(); Iter++ ) {
		(*Iter)->OnPlay();
	}
	 
}

void NxVideo_Mov_Player::Pause()
{
	 //SetPlayBackState( VIDEO_STATUS_PAUSE ); 
}

void NxVideo_Mov_Player::Stop()
{
	StopMovie( m_Video->MovieQT );
	SetMovieActive( m_Video->MovieQT, false );
	SetPlayBackState( VIDEO_STATUS_STOP ); 
	for( std::vector< NxVideoPlayerListener * >::iterator Iter = mListeners.begin(); Iter != mListeners.end(); Iter++ ) {
		(*Iter)->OnStop();
	}
}

void NxVideo_Mov_Player::Rewind()
{
	GoToBeginningOfMovie( m_Video->MovieQT );
}

void NxVideo_Mov_Player::SetInboundFrame( unsigned long Frame )
{
	TimeRecord tr;
	tr.value.hi = 0;
	tr.value.lo = Frame ;//* GetVideoDurationframes();
	tr.base = ::GetMovieTimeBase ( m_Video->MovieQT);
	tr.scale = ::GetMovieTimeScale ( m_Video->MovieQT);
	::SetTimeBaseStartTime (::GetMovieTimeBase ( m_Video->MovieQT), &tr);
	mFrameInbound = Frame;
	Log("Called Quicktime SetInboundFrame :" + NxVideoUtils::ToString( Frame ));
}

void NxVideo_Mov_Player::SetOutboundFrame( unsigned long Frame )
{
	TimeRecord tr;
	tr.value.hi = 0;
	tr.value.lo = Frame ;//* GetVideoDurationframes();
	tr.base = ::GetMovieTimeBase ( m_Video->MovieQT);
	tr.scale = ::GetMovieTimeScale ( m_Video->MovieQT);
	::SetTimeBaseStopTime (::GetMovieTimeBase ( m_Video->MovieQT ), &tr);
	mFrameOutbound = Frame;
	Log("Called Quicktime SetOutboundFrame :" + NxVideoUtils::ToString( Frame ));
}

void NxVideo_Mov_Player::SetVolume( float Volume )
{
	if( Volume > 1.0f ) Volume = 1.0f;
	if( Volume < 0.0f ) Volume = 0.0f;

	float val = 255.0f * Volume;
	short VolumeOut = val;
	SetMovieVolume( m_Video->MovieQT, VolumeOut );
}

unsigned long NxVideo_Mov_Player::GetPositionFrame()
{
 
	 /*
	TimeValue inTime = GetMovieTime( m_Video->MovieQT, nil );
	//OSType whichMediaType =  1;//VIDEO_TYPE;
	TimeValue newTimeValue = 0;
	GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeEdgeOK, inTime, 1, &newTimeValue, nil);
	if(newTimeValue == -1) {
		// Search backwards for the next available time
		GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeMediaSample,  inTime, -1, &newTimeValue, NULL);
		if(newTimeValue == -1) {
			//[NSException raise:@"CantFindTime" format:@"Cannot find a video frame time close to %ld", time.timeValue];
			Log("Cannot find a video frame time close");
		}
	}
	
	TimeValue aMediaTime, aDuration1;
	long aSampleIndex;
	MediaTimeToSampleNum( m_Video->mVideoMedia, TrackTimeToMediaTime( newTimeValue, GetMovieTrack( m_Video->MovieQT,1 ) ), &aSampleIndex, &aMediaTime, &aDuration1);
	return aSampleIndex; */
	 
 
	  
	TimeValue inTime = GetMovieTime( m_Video->MovieQT, nil );
 
	//OSType whichMediaType =  1;//VIDEO_TYPE;
	TimeValue newTimeValue = 0;
	GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeSyncSample | nextTimeEdgeOK, inTime, 1, &newTimeValue, nil);
 
	if(newTimeValue == -1) {
		// Search backwards for the next available time
		GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeMediaSample,  inTime, -1, &newTimeValue, NULL);
		if(newTimeValue == -1) {;
			Log("Cannot find a video frame time close");
		}
	}
	 
	SInt64 syncSample;
	//MediaDecodeTimeToSampleNum(m_Video->mVideoMedia, TrackTimeToMediaTime( newTimeValue, GetMovieTrack( m_Video->MovieQT,1 ) ) , &syncSample, NULL, NULL);
	TimeValue64 val = TrackTimeToMediaTime( newTimeValue, GetMovieTrack( m_Video->MovieQT,1 ) );
	if( val == -1 ){ Log("Error TrackTimeToMediaTime invalid value"); return 0;  }
	MediaDisplayTimeToSampleNum(m_Video->mVideoMedia, val , &syncSample, NULL, NULL);
 
	return syncSample;   

 

}

void NxVideo_Mov_Player::SeekPosition( unsigned long Frame )
{
	// get duration and sample count  
	TimeValue duration = GetMovieDuration( m_Video->MovieQT );
	long samples = GetMediaSampleCount( m_Video->mVideoMedia);  
	// calculate time values per frame  
	double timeValuesPerFrame = (double)duration / (double)samples;  
	double t = (double)Frame * (double)timeValuesPerFrame;  
	// jump to specific time
	SetMovieTimeValue( m_Video->MovieQT, t);
	MoviesTask( m_Video->MovieQT, 0);
}

float NxVideo_Mov_Player::GetFramerate()
{
	long sampleCount = GetMediaSampleCount( m_Video->mVideoMedia );
	TimeValue64 duration = GetMediaDisplayDuration( m_Video->mVideoMedia );
	TimeValue64 timeScale = GetMediaTimeScale( m_Video->mVideoMedia );
	float outFPS = (float)sampleCount * (float)timeScale / (float)duration;
	return outFPS;
}

unsigned long NxVideo_Mov_Player::GetVideoDurationframes()
{
	 /* too long
	TimeValue64 sampleTableStartDecodeTime = 0;
	QTMutableSampleTableRef sampleTable = NULL;
	OSErr err = CopyMediaMutableSampleTable( m_Video->mVideoMedia,
		0,
		&sampleTableStartDecodeTime,
		0,
		0,
		&sampleTable);

	SInt64  sampleCount2;
	sampleCount2 = QTSampleTableGetNumberOfSamples(sampleTable);
 
	return sampleCount2 ; 
	*/
 
	/* Some videos from mac | apple pro res 422 | report wrong frame size played on windows */
	 return GetMediaSampleCount( m_Video->mVideoMedia );
}

void NxVideo_Mov_Player::StepTime( double TimePos )
{
	TimeValue tv = GetMovieDuration( m_Video->MovieQT );
	TimeValue res = tv * TimePos;
	SetMovieTimeValue( m_Video->MovieQT,  res );
}

void NxVideo_Mov_Player::SetFramerate( float FrameRate )
{
	float rate = FrameRate / GetFramerate();
	Log("Setting mov movie rate : " + NxVideoUtils::ToString( rate ));
	mVideoFrameRate = FrameRate;
	SetMovieRate( m_Video->MovieQT, FloatToFixed( rate  ) );
	SetMoviePreferredRate( m_Video->MovieQT, FloatToFixed( rate  ) );
}

bool HasMediaTypeInTrack( Movie theMovie, OSType theMediaType)
{
	Track aTrack = NULL;
	long aTrackCount = 0;
	long index;
	OSType aMediaType;
	bool haveMediaType = false;

	aTrackCount = GetMovieTrackCount(theMovie);
	if(aTrackCount == 0)
		return false;				// no tracks in movie

	for(index = 1; index <= aTrackCount; index++) {
		aTrack = GetMovieIndTrack(theMovie, index);
		GetMediaHandlerDescription( GetTrackMedia(aTrack), &aMediaType, NULL, NULL);
		haveMediaType = ( aMediaType == theMediaType);
		if(haveMediaType == true)
			return true;
	}
	return false;
}
/*
OSStatus AudioCall(void *inUserData, AudioUnitRenderActionFlags *ioRenderFlags,
				   const AudioTimeStamp *inTimeStamp,
				   UInt32 inNumberFrames,
				   AudioBufferList *inInputData,
				   AudioBufferList *outOutputData)
{

Log( "Processing audio...." );
return 0;
}

OSErr movieDrawingCompleteCallDelegate( Movie theMovie, long refCon)
{
	NxVideo_Mov_Player * ptr = ( NxVideo_Mov_Player * )refCon;
	ptr->m_Video->FrameReady = true;
	return noErr;
}*/

unsigned int firstPO2From( unsigned int  n)
{
	--n;            
	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	++n;
	return n;
}
/*
 Helper function to find the smallest power of two greater than or equal
 to the given number.
 */
static unsigned int NextLargerPowerOfTwo(unsigned int n) {
    unsigned int candidate = n;
    
    while((candidate & (candidate - 1)) != 0) {
        ++candidate;
    }
    
    return candidate;
}

void NxVideo_Mov_Player::SetLooping( bool Loop ) 
{
	mLooping = Loop;
	setLoopState( this, mLooping ? NxLoopOneWay : NxLoopNone );
}

void NxVideo_Mov_Player::SetPlayBackType( NxVideo_PlayBack_Type type )
{
	mVideoPlayType = type;
}

bool NxVideo_Mov_Player::RenderFrame()
{
	 if( GetPlayBackState() == VIDEO_STATUS_STOP  ) { //|| mFramePrevious == mFrame
	 	return false;
	 }

	 m_Video->FrameReady = false;

	 

	 TimeValue64 inTime3 = GetMovieTime( m_Video->MovieQT, nil );
	 //inTime3 = TrackTimeToMediaTime( inTime3, m_Video->mVideoTrack );

	  inTime3 = TrackTimeToMediaDisplayTime( inTime3, m_Video->mVideoTrack );


	 Media media3 = GetTrackMedia( m_Video->mVideoTrack );
	  SInt64 newframenum3;
	 MediaDisplayTimeToSampleNum(media3,inTime3,&newframenum3,NULL,NULL);
	 
	// long newframenum3;
	// MediaTimeToSampleNum( media3,inTime3,&newframenum3,NULL,NULL);
	 // Log(" asking frame : " +  NxVideoUtils::ToString( newframenum3 ) );

	 //printf(" asking frame : %d \n", newframenum3 );

	 
	 if(  m_Video->lastDecompressedSampleNumber != newframenum3 ){

		 m_Video->mFramePosition = newframenum3;
		 return true;
		//open2( frame, mVideoBpp, m_Video );
		//if( m_Video->FrameReady ){ ::MoviesTask( m_Video->MovieQT, 0 ); frame++; return true; }
	 }

	 return false;

	 
	//

	/*

	TimeRecord trec;
	TimeValue64 inTime = GetMovieTime(m_Video->MovieQT, &trec );	
	//TimeValue64 inTime = GetMovieTime( m_Video->MovieQT, nil );
	Log(" value : " +  NxVideoUtils::ToString( (SInt64) &trec.value  ) );
	Log(" scale : " +  NxVideoUtils::ToString(  (long)trec.scale  ) );
	Log(" TIME : " +  NxVideoUtils::ToString(  inTime  ) );

	inTime = TrackTimeToMediaTime( inTime, m_Video->mVideoTrack );
	Log(" NEW TIME : " +  NxVideoUtils::ToString(  inTime  ) );
 

	Media media = GetTrackMedia( m_Video->mVideoTrack );


	 //static TimeValue64  start           = inTime;
	 //TimeValue64  sample_time     = 0;
	 //TimeValue64  sample_duration = -1;
	 //int frames = 0;
	 //while( sample_time != -1 ) {

		// GetMediaNextInterestingDisplayTime( media, nextTimeMediaSample | nextTimeEdgeOK, start, fixed1, &sample_time, &sample_duration );
		// if( sample_time != -1 ) {
		//	 ++frames;
		// }
		// 
		//	 start += sample_time;
	 //} 


	 TimeValue64 newdisplayTime;
	 GetMediaNextInterestingDisplayTime(media,
		  nextTimeMediaSample | nextTimeEdgeOK,
		 inTime,
		 fixed1,
		 &newdisplayTime,
		 NULL );

	 SInt64 newframenum;
	 MediaDisplayTimeToSampleNum(media,newdisplayTime,&newframenum,NULL,NULL);
 

	

	 if( m_Video->mAudioTrack != NULL ) { //&& GetPlayBackState() != VIDEO_STATUS_PAUSE) {
		 
	 }

 
	   Log(" asking frame : " +  NxVideoUtils::ToString(  newframenum  ) );
	  

	  int deff = open( newframenum, mVideoBpp, m_Video );

	  //if( m_Video->LastFrameNum != newframenum ) 
		  ::MoviesTask( m_Video->MovieQT, 0 );

	   m_Video->LastFrameNum = newframenum;
 
	return  m_Video->FrameReady;*/
}



bool NxVideo_Mov_Player::NewFrame()
{
 
	  return RenderFrame();
 
	  //RenderFrame();
	  //return false;//RenderFrame();
}

void NxVideo_Mov_Player::ResetFrame()
{
	m_Video->FrameReady = false;
	mNewFrame = false;
}
 
bool NxVideo_Mov_Player::GetAudioData( int64 audioSampleindex, char * data, unsigned int * numSamples, NxVideoStreamFlags * OutFlags ) {

	if( !mHasAudio ) return false;
	
	//while( numSamples > 0 ) {
 
		TimeRecord timeRec;
		timeRec.scale	 = (TimeScale) m_Video->inputStreamDesc.mSampleRate;//GetMovieTimeScale( ptr->MovieQT );
		timeRec.base	 = 0;
		timeRec.value.hi = 0; 
		timeRec.value.lo = (UInt32) audioSampleindex;
 
		OSStatus err = MovieAudioExtractionSetProperty( m_Video->mAudioSession ,
			kQTPropertyClass_MovieAudioExtraction_Movie,
			kQTMovieAudioExtractionMoviePropertyID_CurrentTime,
			sizeof(TimeRecord), &timeRec);
		if( err != noErr ) printf("MovieAudioExtractionSetProperty error\n", 0 );


		m_Video->abl->mNumberBuffers = 1;
		m_Video->abl->mBuffers[0].mData = data;
		m_Video->abl->mBuffers[0].mDataByteSize= *numSamples * 4 * m_Video->abl->mBuffers[0].mNumberChannels;
 
		//int framesToDo = numSamples;//min (numSamples, (int) (m_Video->abl->mBuffers[0].mDataByteSize / m_Video->inputStreamDesc.mBytesPerFrame));
		//m_Video->abl->mBuffers[0].mDataByteSize = m_Video->inputStreamDesc.mBytesPerFrame * framesToDo;

		UInt32 numFrames = *numSamples;//framesToDo;
		UInt32 flags = 0;
		err = MovieAudioExtractionFillBuffer(m_Video->mAudioSession, &numFrames, m_Video->abl, &flags);
		if( err != noErr ) printf("MovieAudioExtractionFillBuffer error\n", 0 );

		*numSamples = numFrames;

		//Log("Num buffers : " + NxVideoUtils::ToString( m_Video->abl->mNumberBuffers  ));
		//Log("mDataByteSize : " + NxVideoUtils::ToString( m_Video->abl->mBuffers[0].mDataByteSize ));
		//Log("mDataCHANNELS : " + NxVideoUtils::ToString( m_Video->abl->mBuffers[0].mNumberChannels ));
		

		if (flags & kQTMovieAudioExtractionComplete)  *OutFlags = NxVideoAudioExtractionComplete;
		else *OutFlags = NxVideoAudioExtractionRunning;
		 
 
	return (*OutFlags == NxVideoAudioExtractionRunning);//true;
}

void NxVideo_Mov_Player::SetOpenGLID( int plane, unsigned int ID  ) {
	mOpengGlID[plane] = ID;
	m_Video->mGLID = mOpengGlID[plane];
	if( m_Video->mGLID ){
		if( m_Video->mBuffer ){ delete m_Video->mBuffer; m_Video->mBuffer=NULL; }
	}else{
		if( !m_Video->mBuffer ) m_Video->mBuffer = new unsigned char [ mVideoWidth[0] *  mVideoHeight[0] *  mVideoBpp ];
	}
}

void NxVideo_Mov_Player::GetPixelData( unsigned char * data )
{
	m_Video->FrameReady = false;
	open2( m_Video->mFramePosition, mVideoBpp, m_Video );
	if( m_Video->FrameReady ) {
		 if( m_Video->mGLID == 0 )
		 memcpy( &data[0], &m_Video->mBuffer[0], mVideoWidth[0]*mVideoHeight[0]*mVideoBpp );
	 }
}

 

}