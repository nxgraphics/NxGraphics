#include "NxVideoPreCompiled.h"

/* theora */
#include <theoradec.h>

/* Vorbis */
#include <vorbis/codec.h>

/* windows */
#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
#include <io.h>
#include <fcntl.h> 
#endif

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32

 #define GLEW_STATIC
 #include <Gl/glew.h>


/*#pragma comment( lib, "opengl32" )*/
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_APPLE
#include <OPENGl/gl.h>
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// Name : EXT_unpack_subimage
// Name Strings : GL_EXT_unpack_subimage

#ifndef GL_UNPACK_ROW_LENGTH_EXT  
# define GL_UNPACK_ROW_LENGTH_EXT 0x0cf2
#endif
 
bool mUseExtensionUnpackRow = false;
const GLubyte *ext;

using namespace NxVideo_Namespace;
void CheckOpenGLExtensions(){

	// check opengl extensions
	ext = glGetString(GL_EXTENSIONS);
	if (ext){

		// Set version string
		const GLubyte* pcVer = glGetString(GL_VERSION);

		assert(pcVer && "Problems getting GL version string using glGetString");

		std::string tmpStr = (const char*)pcVer;
		Log("GL_VERSION = " + tmpStr);
		std::string mVersion = tmpStr.substr(0, tmpStr.find(" "));

		// Get vendor
		const GLubyte* pcVendor = glGetString(GL_VENDOR);
		tmpStr = (const char*)pcVendor;
		Log("GL_VENDOR = " + tmpStr);
		std::string mVendor = tmpStr.substr(0, tmpStr.find(" "));

		// Get renderer
		const GLubyte* pcRenderer = glGetString(GL_RENDERER);
		tmpStr = (const char*)pcRenderer;
		Log("GL_RENDERER = " + tmpStr);
		std::string mRenderer = tmpStr;//tmpStr.substr(0, tmpStr.find(" "));

		Log("NxVideo GL_EXTENSIONS = " + std::string((const char*)ext));
		if (!strstr((char *)ext, "_unpack_subimage")){
			Log( "GL_UNPACK_ROW_LENGTH not available for renderer :" + mRenderer );
			// some devices drivers dont report some extensions but support it.
			if( mRenderer == "Adreno (TM) 320" ) {
				Log("adding support for GL_UNPACK_ROW_LENGTH on device Adreno (TM) 320");
				mUseExtensionUnpackRow = true;
			}

		}else{
			Log( "GL_UNPACK_ROW_LENGTH available !" );
			mUseExtensionUnpackRow = true;
		}
	}

}


 
#endif

bool usepostprocess = false;

// good player https://svn.xiph.org/trunk/theora-tools/vp32theora/vp32theora.c

namespace NxVideo_Namespace {

	FILE *infile = stdin;
	/* Ogg and codec state for demux/decode */
	ogg_sync_state   oy;
	ogg_page         og;
	ogg_stream_state vo;
	ogg_stream_state to;
	th_info      ti;
	th_comment   tc;
	th_dec_ctx       *td;
	th_setup_info    *ts;
	vorbis_info      vi;
	vorbis_dsp_state vd;
	vorbis_block     vb;
	vorbis_comment   vc;
	th_pixel_fmt     px_fmt;

	int              theora_p=0;
	int              vorbis_p=0;
	int              stateflag=0;

	ogg_packet op;

	/* single frame video buffering */
	int          videobuf_ready=0;
	ogg_int64_t  videobuf_granulepos=-1;
	double       videobuf_time=0;

	/* single audio fragment audio buffering */
	int          audiobuf_fill=0;
	int          audiobuf_ready=0;
	ogg_int16_t *audiobuf;
	ogg_int64_t  audiobuf_granulepos=0; /* time position of last sample */


	int pp_level_max;
	int pp_level;
	int pp_inc;

	int frames = 0;
	int dropped = 0;

long         audiofd_totalsize=-1;
int          audiofd_fragsize;      /* read and write only complete fragments
                                       so that SNDCTL_DSP_GETOSPACE is
                                       accurate immediately after a bank
                                       switch */
int          audiofd=-1;
ogg_int64_t  audiofd_timer_calibrate=-1;

static ogg_int64_t last=0;
static ogg_int64_t up=0;


static ogg_int64_t Frames = 0;
static float currentTimeOffset = 0.0f;

static float TimeSinceLastFrame = 0.0f;


////////////

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
#define ALIGN( x ) __declspec( align( 16 ) ) x
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_LINUX
#define ALIGN( x ) x __attribute__( ( aligned( 16 ) ) )
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_ANDROID
#define ALIGN( x ) x __attribute__( ( aligned( 16 ) ) )
#else
#define ALIGN( x ) x
#endif

ALIGN( static int sRGBY[ 256 ][ 4 ] );
ALIGN( static int sRGBCb[ 256 ][ 4 ] );
ALIGN( static int sRGBCr[ 256 ][ 4 ] );

static unsigned char  sClampBuff[ 1024 ];
static unsigned char* sClamp = sClampBuff + 384;

int getMin(int a, int b)
{
	return a>b ? b : a;
}

int getMax(int a, int b)
{
	return a>b ? a : b;
}

int mClamp(int val, int low, int high)
{
	return getMax(getMin(val, high), low);
}

static void initLookupTables()
{
	static bool sGenerated = false;
	if( !sGenerated )
	{
		for( int i = 0; i < 256; ++ i )
		{
			// Y.

			sRGBY[ i ][ 0 ]   = ( 298 * ( i - 16 ) ) >> 8; // B
			sRGBY[ i ][ 1 ]   = ( 298 * ( i - 16 ) ) >> 8; // G
			sRGBY[ i ][ 2 ]   = ( 298 * ( i - 16 ) ) >> 8; // R
			sRGBY[ i ][ 3 ]   = 0xff;                      // A

			// Cb.

			sRGBCb[ i ][ 0 ]  = ( 516 * ( i - 128 ) + 128 ) >> 8;       // B
			sRGBCb[ i ][ 1 ]  = - ( ( 100 * ( i - 128 ) + 128 ) >> 8 ); // G 

			// Cr.

			sRGBCr[ i ][ 1 ]  = - ( ( 208 * ( i - 128 ) + 128 ) >> 8 ); // B
			sRGBCr[ i ][ 2 ]  = ( 409 * ( i - 128 ) + 128 ) >> 8;       // R
		}

		// Setup clamping table for generic transcoder.

		for( int i = -384; i < 640; ++ i )
			sClamp[ i ] = mClamp( i, 0, 0xFF );

		sGenerated = true;
	}
}

static int sampleG( unsigned char* pCb, unsigned char* pCr )
{
	return sRGBCr[ *pCr ][ 1 ] + sRGBCr[ *pCb ][ 1 ];
}







////////////

#ifdef _WINDOWS_
int gettimeofday(struct timeval* p, void* tz) {
	ULARGE_INTEGER ul; // As specified on MSDN.
	FILETIME ft;

	// Returns a 64-bit value representing the number of
	// 100-nanosecond intervals since January 1, 1601 (UTC).
	GetSystemTimeAsFileTime(&ft);

	// Fill ULARGE_INTEGER low and high parts.
	ul.LowPart = ft.dwLowDateTime;
	ul.HighPart = ft.dwHighDateTime;
	// Convert to microseconds.
	ul.QuadPart /= 10ULL;
	// Remove Windows to UNIX Epoch delta.
	ul.QuadPart -= 11644473600000000ULL;
	// Modulo to retrieve the microseconds.
	p->tv_usec = (long) (ul.QuadPart % 1000000LL);
	// Divide to retrieve the seconds.
	p->tv_sec = (long) (ul.QuadPart / 1000000LL);

	return 0;
}
#endif


/* Helper; just grab some more compressed bitstream and sync it for page extraction */
int buffer_data(FILE *in,ogg_sync_state *oy){
  char *buffer=ogg_sync_buffer(oy,4096);
  int bytes=fread(buffer,1,4096,in);
  ogg_sync_wrote(oy,bytes);
  return(bytes);
}

/* helper: push a page into the appropriate steam */
/* this can be done blindly; a stream won't accept a page
                that doesn't belong to it */
int queue_page(ogg_page *page){
  if(theora_p)ogg_stream_pagein(&to,page);
  if(vorbis_p)ogg_stream_pagein(&vo,page);
  return 0;
}

/* Report the encoder-specified colorspace for the video, if any.
   We don't actually make use of the information in this example;
   a real player should attempt to perform color correction for
   whatever display device it supports. */
static void report_colorspace(th_info *ti)
{
    switch(ti->colorspace){
      case TH_CS_UNSPECIFIED:
        /* nothing to report */
        break;;
      case TH_CS_ITU_REC_470M:
		Log("encoder specified ITU Rec 470M (NTSC) color" );
        break;;
      case TH_CS_ITU_REC_470BG:
		Log("encoder specified ITU Rec 470BG (PAL) color." );
        break;;
      default:
		Log("warning: encoder specified unknown colorspace :" + NxVideoUtils::ToString( ti->colorspace ));
        break;;
    }
}

/* dump the theora (or vorbis) comment header */
static int dump_comments(th_comment *tc){
	int i, len;
	char *value;
	FILE *out=stdout;
	//Log("Encoded by :" + std::string tc->vendor ));
	if(tc->comments){
		Log("theora comment header...");
		for(i=0;i<tc->comments;i++){
			if(tc->user_comments[i]){
				len=tc->comment_lengths[i];
				value=(char*)malloc(len+1);
				memcpy(value,tc->user_comments[i],len);
				value[len]='\0';
				//fprintf(out, "\t%s\n", value);
				//Log("Encoded by :" + NxVideoUtils::ToString( tc->vendor ));
				 
				free(value);
			}
		}
	}
	Log("theora comment header:done");
	return(0);
}


/* get relative time since beginning playback, compensating for A/V
   drift */

void resetTime(){

audiofd_timer_calibrate=-1;
audiofd = -1;
last=0;
up=0;
 

}

double get_time(){

  ogg_int64_t now;
  struct timeval tv;

  gettimeofday(&tv,0);
  now=tv.tv_sec*1000+tv.tv_usec/1000;

  if(audiofd_timer_calibrate==-1)audiofd_timer_calibrate=last=now;

  if(audiofd<0){
    /* no audio timer to worry about, we can just use the system clock */
    /* only one complication: If the process is suspended, we should
       reset timing to account for the gap in play time.  Do it the
       easy/hack way */
    if(now-last>1000)audiofd_timer_calibrate+=(now-last);
    last=now;
  }

  if(now-up>200){
    double timebase=(now-audiofd_timer_calibrate)*.001;
    int hundredths=timebase*100-(long)timebase*100;
    int seconds=(long)timebase%60;
    int minutes=((long)timebase/60)%60;
    int hours=(long)timebase/3600;

    fprintf(stderr,"   Playing: %d:%02d:%02d.%02d\r", hours,minutes,seconds,hundredths);
    up=now;
  }

  return (now-audiofd_timer_calibrate)*.001;

}

/* write a fragment to the OSS kernel audio API, but only if we can
   stuff in a whole fragment without blocking */
void audio_write_nonblocking(void){

	/*
  if(audiobuf_ready){
    audio_buf_info info;
    long bytes;

    ioctl(audiofd,SNDCTL_DSP_GETOSPACE,&info);
    bytes=info.bytes;
    if(bytes>=audiofd_fragsize){
      if(bytes==audiofd_totalsize)audio_calibrate_timer(1);

      while(1){
        bytes=write(audiofd,audiobuf+(audiofd_fragsize-audiobuf_fill),
                    audiofd_fragsize);

        if(bytes>0){

          if(bytes!=audiobuf_fill){
            // shouldn't actually be possible... but eh 
            audiobuf_fill-=bytes;
          }else
            break;
        }
      }

      audiobuf_fill=0;
      audiobuf_ready=0;

    }
  }*/
}


unsigned int _getPixelOffset( th_ycbcr_buffer buffer, unsigned int plane, unsigned int x, unsigned int y )
{
	/*switch( getDecoderPixelFormat() )
	{
	case PIXEL_FORMAT_444: break;
	case PIXEL_FORMAT_422: if( plane != 0 ) x >>= 1; break;
	case PIXEL_FORMAT_420: if( plane != 0 ) { x >>= 1; y >>= 1; } break;

	default:
		AssertFatal( false, "OggTheoraDecoder::_getPixelOffset() - invalid pixel format" );
	}*/

	if( plane != 0 ) { x >>= 1; y >>= 1; }

	return ( y * buffer[ plane ].stride + x );
}

///
unsigned char * _getPixelPtr( th_ycbcr_buffer buffer, unsigned int plane, unsigned int offset, unsigned int x, unsigned int y )
{
	return ( buffer[ plane ].data + offset + _getPixelOffset( buffer, plane, x, y ) );
}

///
unsigned int _getPictureOffset( th_ycbcr_buffer buffer, unsigned int plane )
{
	return _getPixelOffset( buffer, plane, ti.pic_x, ti.pic_y );
}

void GFXPackPixel( /*GFXFormat format,*/ unsigned char*& ptr, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, bool leastSignficantFirst = true )
{

	if( leastSignficantFirst )
	{
		ptr[ 0 ] = blue;
		ptr[ 1 ] = green;
		ptr[ 2 ] = red;
	}
	else
	{
		ptr[ 0 ] = red;
		ptr[ 1 ] = green;
		ptr[ 2 ] = blue;
	}
	ptr += 3;

	/*
	switch( format )
	{
	case GFXFormatR8G8B8A8:
		if( leastSignficantFirst )
		{
			ptr[ 0 ] = blue;
			ptr[ 1 ] = green;
			ptr[ 2 ] = red;
			ptr[ 3 ] = alpha;
		}
		else
		{
			ptr[ 0 ] = red;
			ptr[ 1 ] = green;
			ptr[ 2 ] = blue;
			ptr[ 3 ] = alpha;
		}
		ptr += 4;
		break;

	case GFXFormatR8G8B8:
		if( leastSignficantFirst )
		{
			ptr[ 0 ] = blue;
			ptr[ 1 ] = green;
			ptr[ 2 ] = red;
		}
		else
		{
			ptr[ 0 ] = red;
			ptr[ 1 ] = green;
			ptr[ 2 ] = blue;
		}
		ptr += 3;
		break;

	default:
		AssertISV( false, "GFXPackPixel() - pixel format not implemented." );
	}*/
}



void _transcode( th_ycbcr_buffer ycbcr, unsigned char * buffer, const unsigned int width, const unsigned int height )
{
#define ycbcrToRGB( rgb, pY, pCb, pCr, G ) \
	{ \
	GFXPackPixel( \
	/*mPacketFormat.mFormat,*/ \
	rgb, \
	sClamp[ sRGBY[ *pY ][ 2 ] + sRGBCr[ *pCr ][ 2 ] ], \
	sClamp[ sRGBY[ *pY ][ 1 ] + G ], \
	sClamp[ sRGBY[ *pY ][ 0 ] + sRGBCb[ *pCb ][ 0 ] ], \
	255 \
	); \
}

	// Determine number of chroma samples per 4-pixel luma block.

	unsigned int numChromaSamples = 4;
	 
	//if( pixelFormat == PIXEL_FORMAT_422 )
	//	numChromaSamples = 2;
	//else if( pixelFormat == OggTheoraDecoder::PIXEL_FORMAT_420 )
		numChromaSamples = 1;

	// Convert and copy the pixels. Deal with all three
	// possible plane configurations.

		/*
		//for( int i = 0 ; i < mVideoWidth *  mVideoHeight *  mVideoBpp;i++ ){
		//m_video->mBuffer[i] = rand() % 255;
		//}*/

	const unsigned int pictOffsetY = _getPictureOffset( ycbcr, 0 );
	const unsigned int pictOffsetU = _getPictureOffset( ycbcr, 1 );
	const unsigned int pictOffsetV = _getPictureOffset( ycbcr, 2 );

	int rgbpitch = 3*width;
	for( unsigned int y = 0; y < height; y += 2 )
	{
		unsigned char* dst0 = buffer + y * rgbpitch;//mPacketFormat.mPitch;
		unsigned char* dst1 = dst0 + rgbpitch;//mPacketFormat.mPitch;

		unsigned char* pY0 = _getPixelPtr( ycbcr, 0, pictOffsetY, 0, y );
		unsigned char* pY1 = _getPixelPtr( ycbcr, 0, pictOffsetY, 0, y + 1 );
		unsigned char* pU0 = _getPixelPtr( ycbcr, 1, pictOffsetU, 0, y );
		unsigned char* pU1 = _getPixelPtr( ycbcr, 1, pictOffsetU, 0, y + 1 );
		unsigned char* pV0 = _getPixelPtr( ycbcr, 2, pictOffsetV, 0, y );
		unsigned char* pV1 = _getPixelPtr( ycbcr, 2, pictOffsetV, 0, y + 1 );

		for( unsigned int x = 0; x < width; x += 2 )
		{
			// Pixel 0x0.

			int G = sampleG( pU0, pV0 );

			ycbcrToRGB( dst0, pY0, pU0, pV0, G );

			++ pY0;

			if( numChromaSamples == 4 )
			{
				++ pU0;
				++ pV0;
			}

			// Pixel 0x1.

			if( numChromaSamples == 4 )
				G = sampleG( pU0, pV0 );

			ycbcrToRGB( dst0, pY0, pU0, pV0, G );

			++ pY0;
			++ pU0;
			++ pV0;

			// Pixel 1x0.

			if( numChromaSamples != 1 )
				G = sampleG( pU1, pV1 );

			ycbcrToRGB( dst1, pY1, pU1, pV1, G );

			++ pY1;

			if( numChromaSamples == 4 )
			{
				++ pU1;
				++ pV1;
			}

			// Pixel 1x1.

			if( numChromaSamples == 4 )
				G = sampleG( pU1, pV1 );

			ycbcrToRGB( dst1, pY1, pU1, pV1, G );

			++ pY1;
			++ pU1;
			++ pV1;
		}
	}
 
}


	struct NxVideoTheora
	{
	public:
		NxVideoTheora()   {  
 
		}

		~NxVideoTheora(){

		}
 
	}; 

	NxVideo_Theora_Player::NxVideo_Theora_Player() : NxVideo_Player()
	{
		mVideoType = NxVideoTheoraFile;

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_ANDROID

		CheckOpenGLExtensions();
 
#endif
 
		m_video = new NxVideoTheora();

		
	}

	NxVideo_Theora_Player::~NxVideo_Theora_Player()  
	{

		Log("----> Destructor ! Theora video...");
		 last=0;
	  up=0;


		  Frames = 0;
	 currentTimeOffset = 0.0f;

		  TimeSinceLastFrame = 0.0f;

		  resetTime();


		

		fclose( infile );
		infile = 0;


	
		 theora_p = 0;
		  
	
		ogg_sync_destroy( &oy  );

		vorbis_info_clear(&vi);
		vorbis_comment_clear(&vc);


		// Libération des objets propres à la libogg et à la libtheora.
		ogg_stream_clear(&to);
		//theora_clear(&td);
		//theora_comment_clear(&tc);
		//theora_info_clear(&ti);
		ogg_sync_clear(&oy);


	 stateflag = 0;

		th_info_clear(&ti);

		delete m_video;
		m_video = 0;

		Log("----> Destructor ! Theora video : DONE");


	}


 


	void PrepareOgg() {

		rewind (infile);


		//ogg_sync_destroy(&oy);
		//th_info_clear(&ti);

		// start up Ogg stream synchronization layer 
		ogg_sync_init(&oy);

		// init supporting Vorbis structures needed in header parsing  
		vorbis_info_init(&vi);
		vorbis_comment_init(&vc);

		// init supporting Theora structures needed in header parsing  
		th_comment_init(&tc);
		th_info_init(&ti);

		// Ogg file open; parse the headers  
		//Only interested in Vorbis/Theora streams  
		while(!stateflag){
			int ret=buffer_data(infile,&oy);
			if(ret==0)break;


			Log("while(!stateflag){...");
			while(ogg_sync_pageout(&oy,&og)>0){


					Log("while(ogg_sync_pageout(&oy,&og)>0) IN ....");

				ogg_stream_state test;

				// is this a mandated initial header? If not, stop parsing  
				if(!ogg_page_bos(&og)){
					// don't leak the page; get it into the appropriate stream  
					queue_page(&og);
					stateflag=1;

					Log("stateflag=1;");
					break;
				}

				ogg_stream_init(&test,ogg_page_serialno(&og));
				ogg_stream_pagein(&test,&og);
				ogg_stream_packetout(&test,&op);


				//identify the codec: try theora  
				if(!theora_p && th_decode_headerin(&ti,&tc,&ts,&op)>=0){
					// it is theora  
					Log("Found theora video file");
					memcpy(&to,&test,sizeof(test));
					theora_p=1;
				}else if(!vorbis_p && vorbis_synthesis_headerin(&vi,&vc,&op)>=0){
					Log("Found Vorbis video file");
					//it is vorbis  
					memcpy(&vo,&test,sizeof(test));
					vorbis_p=1;
				}else{
					Log("Didnt Found a theora video file...");
					// whatever it is, we don't care about it  
					ogg_stream_clear(&test);
				}
			}
			//fall through to non-bos page parsing  
		}

		// we're expecting more header packets.  
		while((theora_p && theora_p<3) || (vorbis_p && vorbis_p<3)){
			int ret;

			// look for further theora headers  
			while(theora_p && (theora_p<3) && (ret=ogg_stream_packetout(&to,&op))){
				if(ret<0){
					Log("Error parsing Theora stream headers corrupt stream?");
					exit(1);
				}
				if(!th_decode_headerin(&ti,&tc,&ts,&op)){
					Log("Error parsing Theora stream headers corrupt stream?");
					exit(1);
				}
				theora_p++;
			}

			// look for more vorbis header packets  
			while(vorbis_p && (vorbis_p<3) && (ret=ogg_stream_packetout(&vo,&op))){
				if(ret<0){
					Log("Error parsing Vorbis stream headers; corrupt stream?");
					exit(1);
				}
				if(vorbis_synthesis_headerin(&vi,&vc,&op)){
					Log("Error parsing Vorbis stream headers; corrupt stream?");
					exit(1);
				}
				vorbis_p++;
				if(vorbis_p==3)break;
			}

			// The header pages/packets will arrive before anything else we
			//  care about, or the stream is not obeying spec  

			if(ogg_sync_pageout(&oy,&og)>0){
				queue_page(&og);  // demux into the appropriate stream  
			}else{
				int ret=buffer_data(infile,&oy);  // someone needs more data   
				if(ret==0){
					Log("End of file while searching for codec headers.");
					exit(1);
				}
			}
		}

		 

		 td=th_decode_alloc(&ti,ts);



	}
 
bool NxVideo_Theora_Player::OpenVideoFile( const std::string& FilePath, NxVideoPixelFormatInfo & info ) {


	mVideoFullPath = std::string( FilePath );//save the video full path
	std::string Tmp = mVideoFullPath ;

	NxVideoUtils::SplitFileName( Tmp );
	mVideoFullName = Tmp;

	NxVideoUtils::SplitFileExtension( Tmp );
	mVideoExtension = Tmp;


	//initLookupTables(); only for cpu pixel buffer
 
	#ifdef _WIN32 /* We need to set stdin/stdout to binary mode. Damn windows. */
	/* Beware the evil ifdef. We avoid these where we can, but this one we
	cannot. Don't add any more, you'll probably go to hell if you do. */
	_setmode( _fileno( stdin ), _O_BINARY );
	#endif

	infile=fopen(FilePath.c_str(),"rb");
	if(infile==NULL){
		Log("unable to open theora video file for playback : " + FilePath );
		exit(1);
	}
 

	PrepareOgg();

	/* and now we have it all.  initialize decoders */
	if(theora_p) {

		mVideoWidth[0]=(ti.pic_x+ti.pic_width+1&~1)-(ti.pic_x&~1);
		mVideoHeight[0]=(ti.pic_y+ti.pic_height+1&~1)-(ti.pic_y&~1);


			Log("---> check Video Width : " + NxVideoUtils::ToString( ti.pic_width ));

		printf("Ogg logical stream %lx is Theora %dx%d %.02f fps", to.serialno,ti.pic_width,ti.pic_height, (double)ti.fps_numerator/ti.fps_denominator);
		px_fmt=ti.pixel_fmt;
		switch(ti.pixel_fmt) {
		case TH_PF_420: { 
			Log(" 4:2:0 video");
			mVideoFormat = NxVideoPixelFormaty420;
			mVideoWidth[1] = mVideoWidth[2] = mVideoWidth[0] / 2;
			mVideoHeight[1] = mVideoHeight[2] = mVideoHeight[0] / 2;
			break;
		}
		  case TH_PF_422: Log(" 4:2:2 video"); mVideoFormat = NxVideoPixelFormaty422; break;
		  case TH_PF_444: Log(" 4:4:4 video"); mVideoFormat = NxVideoPixelFormaty444; break;
		  case TH_PF_RSVD:
		  default:
		   Log("video\n  (UNKNOWN Chroma sampling!)");
		   break;
		}
		if(ti.pic_width!=ti.frame_width || ti.pic_height!=ti.frame_height) {
		  printf("  Frame content is %dx%d with offset (%d,%d).\n", ti.frame_width, ti.frame_height, ti.pic_x, ti.pic_y);
		}
 
 
		mNumPlanes = 3;
		mFrameInbound = 0;
		mFrameOutbound = 100; // unknown see here : http://lists.xiph.org/pipermail/theora/2007-February/001402.html
		mVideoBpp = 3; 
		mVideoFrameRate = (float)ti.fps_numerator/ti.fps_denominator;
 
		mHasVideo = true;
		mHasAudio = false;
 
		Log("Video Width : " + NxVideoUtils::ToString( mVideoWidth[0]  ));
		Log("Video Height : " + NxVideoUtils::ToString( mVideoHeight[0]  ));
		Log("Video Total num frames : " + NxVideoUtils::ToString( mFrameOutbound ) );
		Log("Video Framerate : " + NxVideoUtils::ToString( GetFramerate() ) );

		//turn off post processing
		pp_level = 0;
		th_decode_ctl(td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));

		if( usepostprocess ){
			th_decode_ctl(td,TH_DECCTL_GET_PPLEVEL_MAX,&pp_level_max, sizeof(pp_level_max));
			pp_level=pp_level_max;
			th_decode_ctl(td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
			pp_inc=0;
		}

		buffer_data(infile,&oy);
		while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }
 
  }else{
    /* tear down the partial theora setup */
    th_info_clear(&ti);
    th_comment_clear(&tc);
  }
  
  //th_setup_free(ts);

  ////////////////////////////////////////////////

   
Log("Theora video DONE !");

		return true;
	}


	//! Close Video File.
	bool NxVideo_Theora_Player::CloseVideoFile(){


		Log("NxVideo_Theora_Player::CloseVideoFile()");


		return true;
	}



bool CheckNewFrame(){
			 
	ogg_int64_t FramesTmp = th_granule_frame( td, videobuf_granulepos );
	//Log( "Got frame : " + NxVideoUtils::ToString(  FramesTmp  ) );
	if( FramesTmp == -1 ) return false;
	if( FramesTmp != Frames ){ Frames = FramesTmp; return true; }
	return false;
}


	bool HanldeLoop(){

		if( !videobuf_ready && feof(infile)) {  // If end of file => loop

			//Log( "SEEK Started..." );
 
			/*last=0;
			up=0;
			audiofd = -1;
			audiofd_timer_calibrate=-1;*/
			ogg_stream_reset(&vo);

			ogg_int64_t granpos = 0;
			int  rc = th_decode_ctl(td, TH_DECCTL_SET_GRANPOS, &granpos,sizeof(granpos));
			assert(rc == 0);
			//tstream->prev_framenum = -1;

			rc = ogg_sync_reset(&oy);
			assert(rc == 0);
 
			int seeked = fseek(infile, 0, SEEK_SET);
			assert(seeked); 


			resetTime();

			if( usepostprocess ){ 
				th_decode_ctl(td,TH_DECCTL_GET_PPLEVEL_MAX,&pp_level_max, sizeof(pp_level_max));
				pp_level=pp_level_max;
				th_decode_ctl(td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
			}

			//Log( "SEEK Ended..." );
			return false;  

		}  

		return true;

	}


	bool NxVideo_Theora_Player::NewFrame() {

	 
		float fps = (float)ti.fps_numerator/ti.fps_denominator;
		float waitbetween = 1.0f / fps; // 0.04

		float currentTime = get_time(); 
		float elapsedtime = currentTime - TimeSinceLastFrame;
		currentTimeOffset += elapsedtime;

		float rest = elapsedtime - waitbetween;
		TimeSinceLastFrame = currentTime+rest;
		 
//Log("----> currentTime : " + NxVideoUtils::ToString( currentTime ) + " TimeSinceLastFrame : " + NxVideoUtils::ToString( TimeSinceLastFrame  )   );
 //		 Log("----> currentTimeOffset : " + NxVideoUtils::ToString( currentTimeOffset ) + " waitbetween : " + NxVideoUtils::ToString( waitbetween  )   );
		if( currentTimeOffset >= waitbetween ){   /*Log("----> Emmit frame at Time: " + NxVideoUtils::ToString( currentTime ) + " Time offset: " + NxVideoUtils::ToString( currentTimeOffset )); */  currentTimeOffset = 0.0f;}
		else {   /*Log("----> not time : " + NxVideoUtils::ToString( currentTimeOffset ) ); */   return false; }

		videobuf_ready=0;

	 while( !videobuf_ready) {


		buffer_data(infile,&oy);
		while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }


      /* theora is one in, one out... */
      if(ogg_stream_packetout(&to,&op)>0){
 
        if(op.granulepos>=0) th_decode_ctl( td, TH_DECCTL_SET_GRANPOS,&op.granulepos, sizeof(op.granulepos));
        
        if(th_decode_packetin(td,&op,&videobuf_granulepos)==0){
          
          videobuf_time=th_granule_time(td,videobuf_granulepos);
          frames++;

		 if(CheckNewFrame()) videobuf_ready=1;

		  //Log("---->   Emmit frame ... "  );

          /* is it already too old to be useful?  This is only actually
             useful cosmetically after a SIGSTOP.  Note that we have to
             decode the frame even if we don't show it (for now) due to
             keyframing.  Soon enough libtheora will be able to deal
             with non-keyframe seeks.  */

          //if(videobuf_time>=get_time())
          //  videobuf_ready=1;
          //else{
          //  /*If we are too slow, reduce the pp level.*/
          //  pp_inc=pp_level>0?-1:0;
          //  dropped++;
          //}
        }

      }else{

		  if( !HanldeLoop()){ currentTimeOffset = 0.0f; TimeSinceLastFrame = 0.0f; /*Log( "loop false" );*/ break; }

		  //Log("----> didnt Emmit frame ... "  );
		  //break;

	  }


    }


	// TimeSinceLastFrame = get_time();



return videobuf_ready;




	 

		//videobuf_ready = 0;
 
		 //buffer_data(infile,&oy);
		// while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }

		//if( ogg_sync_pageout(&oy, &og) > 0 ) { queue_page(&og);  return false;   } 
		buffer_data(infile,&oy);
		while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }

		//th_decode_ctl(td,TH_DECCTL_SET_GRANPOS,&op.granulepos, sizeof(op.granulepos));

		int res = ogg_stream_packetout(&to,&op);

		if(res>0) {  

			if(pp_inc && usepostprocess){
				pp_level+=pp_inc;
				th_decode_ctl(td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
				pp_inc=0;
			}
 
			if(op.granulepos>=0){ th_decode_ctl(td,TH_DECCTL_SET_GRANPOS,&op.granulepos, sizeof(op.granulepos)); }
 
			int ret = th_decode_packetin(td,&op,&videobuf_granulepos);
			if(ret==0){

				videobuf_time=th_granule_time(td,videobuf_granulepos);

				// Log( "grenule time : " + NxVideoUtils::ToString(  videobuf_time ) + " ==> time : " + NxVideoUtils::ToString(  get_time() ) );


			 
				//if(videobuf_time>=get_time()){//( get_time() - 0.3 )){ 
					//Log( "Got frame at : " + NxVideoUtils::ToString(  videobuf_time  ) );
					videobuf_ready=0;
					if( CheckNewFrame() ){ videobuf_ready=1; }	 
				//}else{
				// 	  pp_inc=pp_level>0?-1:0;
				//}
			} 
			else if (ret == TH_DUPFRAME)  // same as previous frame, don't bother decoding it
			{  
				//Log( "TH_DUPFRAME" ); 
				//return false;
			}
			else
			{ 
				//Log( "TH_UNKNOWN" );
				//return false;
			}

		}else{ // res // need more to decode
			//Log( "ogg_stream_packetout is zero or minus" );

			//return false;

			//HanldeLoop(); return false;
		}

		 if(!videobuf_ready ){
			 //buffer_data(infile,&oy);
			 //while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }
			 return false;
		 }
			 

 
	 	 if( !HanldeLoop()){ /*Log( "loop false" );*/ return false; }



		 float tdiff(videobuf_time-get_time());
		 if (tdiff > ti.fps_denominator*0.25/ti.fps_numerator )
		 {
			 pp_inc = pp_level < pp_level_max ? 1 : 0;
		 }
		 else if (tdiff < ti.fps_denominator*0.05/ti.fps_numerator)
		 {
			 pp_inc = pp_level > 0 ? -1 : 0;
		 }

		if(videobuf_ready) { /*Log( "return true" );*/ return true;}//RenderFrame(); 
		//Log( "return false" );
		return false;

 
	}

	bool NxVideo_Theora_Player::RenderFrame(){
		return true;
	}

	bool NxVideo_Theora_Player::GetAudioData( int64 audioSampleindex, char * data, unsigned int * numSamples, NxVideoStreamFlags * OutFlags ) {
		return true;
	}

	void NxVideo_Theora_Player::SetOpenGLID( int plane, unsigned int ID  ) {
		mOpengGlID[plane] = ID;
	}

	void NxVideo_Theora_Player::GetPixelData( unsigned char * data ) {

		if( videobuf_ready ){
			 //ogg_int64_t FramesTmp = th_granule_frame( td, videobuf_granulepos ); 
			 //Log( "EMMITING frame : " + NxVideoUtils::ToString(  FramesTmp  ) );
			 
			th_ycbcr_buffer yuv;
			th_decode_ycbcr_out(td,yuv);

			for( int i = 0; i < mNumPlanes; i++ ) {

				glBindTexture( GL_TEXTURE_2D, mOpengGlID[i] );

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_ANDROID
				if (yuv[i].stride == mVideoWidth[i]) {
					// Yay! We can upload the entire plane in a single GL call.
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mVideoWidth[i], mVideoHeight[i], GL_LUMINANCE,
						GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(yuv[i].data));
				} else {
 
					if( mUseExtensionUnpackRow ) { // opengles 2.0 extension some devices support it.
						glPixelStorei( GL_UNPACK_ROW_LENGTH_EXT, yuv[i].stride );
						glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mVideoWidth[i], mVideoHeight[i], GL_LUMINANCE,
							GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(yuv[i].data)); 
						glPixelStorei(GL_UNPACK_ROW_LENGTH_EXT, 0);
					}else{ // not available
						// Boo! Since GLES2 doesn't have GL_UNPACK_ROW_LENGTH and Android doesn't
						// have GL_EXT_unpack_subimage we have to upload a row at a time. Ick.
						for (int row = 0; row < mVideoHeight[i]; ++row) {
							glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row, mVideoWidth[i], 1, GL_LUMINANCE,
								GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(yuv[i].data + (row * yuv[i].stride )));
						}
					} 
 
				}

#else // windows || mac etc 
				glPixelStorei(GL_UNPACK_ROW_LENGTH, yuv[i].stride);

				glTexSubImage2D( GL_TEXTURE_2D, 0 , 
					0, 0, mVideoWidth[i], mVideoHeight[i],
					GL_LUMINANCE , GL_UNSIGNED_BYTE, yuv[i].data );

#endif

			}

			videobuf_ready = 0 ;
		}



	}

	void NxVideo_Theora_Player::SetFramerate( float framerate ) {
	
	}





}