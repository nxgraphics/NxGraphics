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
/*
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#include <gl/gl.h>
#include <gl/glu.h>*/
 #define GLEW_STATIC
 #include <Gl/glew.h>


/*#pragma comment( lib, "opengl32" )*/
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_APPLE
#include <OPENGl/gl.h>
#elif NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif



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
  char *buffer=ogg_sync_buffer(oy,4096*2);
  int bytes=fread(buffer,1,4096*2,in);
  ogg_sync_wrote(oy,bytes);
  return(bytes);
}

/* helper: push a page into the appropriate steam */
/* this can be done blindly; a stream won't accept a page
                that doesn't belong to it */
static int queue_page(ogg_page *page){
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



static void video_write(  unsigned char * buffer  ){

	 


	/*
	int i;
	th_ycbcr_buffer yuv;
	int y_offset, uv_offset;
	th_decode_ycbcr_out(td,yuv);
	 // Lock SDL_yuv_overlay  
	if ( SDL_MUSTLOCK(screen) ) {
		if ( SDL_LockSurface(screen) < 0 ) return;
	}
	if (px_fmt!=TH_PF_444 && SDL_LockYUVOverlay(yuv_overlay) < 0) return;

	  //let's draw the data on a SDL screen (*screen)  
	  //deal with border stride 
	  //reverse u and v for SDL  
	  //and crop input properly, respecting the encoded frame rect  
	  //problems may exist for odd frame rect for some encodings  

	y_offset=(ti.pic_x&~1)+yuv[0].stride*(ti.pic_y&~1);

	if (px_fmt==TH_PF_422) {
		uv_offset=(ti.pic_x/2)+(yuv[1].stride)*(ti.pic_y);
		 // SDL doesn't have a planar 4:2:2  
		for(i=0;i<yuv_overlay->h;i++) {
			int j;
			char *in_y  = (char *)yuv[0].data+y_offset+yuv[0].stride*i;
			char *out = (char *)(yuv_overlay->pixels[0]+yuv_overlay->pitches[0]*i);
			for (j=0;j<yuv_overlay->w;j++)
				out[j*2] = in_y[j];
			char *in_u  = (char *)yuv[1].data+uv_offset+yuv[1].stride*i;
			char *in_v  = (char *)yuv[2].data+uv_offset+yuv[2].stride*i;
			for (j=0;j<yuv_overlay->w>>1;j++) {
				out[j*4+1] = in_u[j];
				out[j*4+3] = in_v[j];
			}
		}
	} else if (px_fmt==TH_PF_444){
		SDL_Surface *output;
		for(i=0;i<screen->h;i++) {
			int j;
			unsigned char *in_y  = (unsigned char *)yuv[0].data+y_offset+yuv[0].stride*i;
			unsigned char *in_u  = (unsigned char *)yuv[1].data+y_offset+yuv[1].stride*i;
			unsigned char *in_v  = (unsigned char *)yuv[2].data+y_offset+yuv[2].stride*i;
			unsigned char *out = RGBbuffer+(screen->w*i*4);
			for (j=0;j<screen->w;j++) {
				int r, g, b;
				r=(1904000*in_y[j]+2609823*in_v[j]-363703744)/1635200;
				g=(3827562*in_y[j]-1287801*in_u[j]
				-2672387*in_v[j]+447306710)/3287200;
				b=(952000*in_y[j]+1649289*in_u[j]-225932192)/817600;
				out[4*j+0]=OC_CLAMP255(b);
				out[4*j+1]=OC_CLAMP255(g);
				out[4*j+2]=OC_CLAMP255(r);
			}  
			output=SDL_CreateRGBSurfaceFrom(RGBbuffer,screen->w,screen->h,32,4*screen->w,0,0,0,0);
			SDL_BlitSurface(output,NULL,screen,NULL);
		}
	} else {
		uv_offset=(ti.pic_x/2)+(yuv[1].stride)*(ti.pic_y/2);
		for(i=0;i<yuv_overlay->h;i++)
			memcpy(yuv_overlay->pixels[0]+yuv_overlay->pitches[0]*i,
			yuv[0].data+y_offset+yuv[0].stride*i,
			yuv_overlay->w);
		for(i=0;i<yuv_overlay->h/2;i++){
			memcpy(yuv_overlay->pixels[1]+yuv_overlay->pitches[1]*i,
				yuv[2].data+uv_offset+yuv[2].stride*i,
				yuv_overlay->w/2);
			memcpy(yuv_overlay->pixels[2]+yuv_overlay->pitches[2]*i,
				yuv[1].data+uv_offset+yuv[1].stride*i,
				yuv_overlay->w/2);
		}
	}

	 // Unlock SDL_yuv_overlay  
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
	if (px_fmt!=TH_PF_444) {
		SDL_UnlockYUVOverlay(yuv_overlay);
		 // Show, baby, show!  
		SDL_DisplayYUVOverlay(yuv_overlay, &rect);
	} else { 
		SDL_Flip(screen);
	}*/
}




	struct NxVideoTheora
	{
	public:
		NxVideoTheora()   {  

			//mBuffer = 0;





		}

		~NxVideoTheora(){

		}

		//unsigned char * mBuffer;

	}; 

	NxVideo_Theora_Player::NxVideo_Theora_Player() : NxVideo_Player()
	{
		mVideoType = NxVideoTheoraFile;

		

		m_video = new NxVideoTheora();

		initLookupTables();
	}

	NxVideo_Theora_Player::~NxVideo_Theora_Player()  
	{
		delete m_video;
		m_video = 0;

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
			while(ogg_sync_pageout(&oy,&og)>0){
				ogg_stream_state test;

				// is this a mandated initial header? If not, stop parsing  
				if(!ogg_page_bos(&og)){
					// don't leak the page; get it into the appropriate stream  
					queue_page(&og);
					stateflag=1;
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
  if(theora_p){

	mVideoWidth[0]=(ti.pic_x+ti.pic_width+1&~1)-(ti.pic_x&~1);
	mVideoHeight[0]=(ti.pic_y+ti.pic_height+1&~1)-(ti.pic_y&~1);

    //td=th_decode_alloc(&ti,ts);
    printf("Ogg logical stream %lx is Theora %dx%d %.02f fps", to.serialno,ti.pic_width,ti.pic_height, (double)ti.fps_numerator/ti.fps_denominator);
    px_fmt=ti.pixel_fmt;
    switch(ti.pixel_fmt) {
	case TH_PF_420: 
	{ 
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
	mVideoBpp = 3;//GetVideoBppFromPixelFormat( format );
	mVideoFrameRate = (float)ti.fps_numerator/ti.fps_denominator;

	if (px_fmt==TH_PF_444){ // SDL_YUY2
	 
	}else if (px_fmt==TH_PF_422) {
		 
	} else if (px_fmt==TH_PF_420) {

	} 


	//m_video->mBuffer = new unsigned char [ mVideoWidth *  mVideoHeight *  mVideoBpp ];
	mHasVideo = true;
	mHasAudio = false;

	// for( int i = 0 ; i < mVideoWidth *  mVideoHeight *  mVideoBpp;i++ ){
		 //m_video->mBuffer[i] = 255; 
	 //} 


	Log("Video Width : " + NxVideoUtils::ToString( mVideoWidth  ));
	Log("Video Height : " + NxVideoUtils::ToString( mVideoHeight  ));
	Log("Video Total num frames : " + NxVideoUtils::ToString( mFrameOutbound ) );
	Log("Video Framerate : " + NxVideoUtils::ToString( GetFramerate() ) );

 

    //report_colorspace(&ti);
    //dump_comments(&tc);
    th_decode_ctl(td,TH_DECCTL_GET_PPLEVEL_MAX,&pp_level_max, sizeof(pp_level_max));
    pp_level=pp_level_max;
    th_decode_ctl(td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
    pp_inc=0;

    /*{
      int arg = 0xffff;
      th_decode_ctl(td,TH_DECCTL_SET_TELEMETRY_MBMODE,&arg,sizeof(arg));
      th_decode_ctl(td,TH_DECCTL_SET_TELEMETRY_MV,&arg,sizeof(arg));
      th_decode_ctl(td,TH_DECCTL_SET_TELEMETRY_QI,&arg,sizeof(arg));
      arg=10;
      th_decode_ctl(td,TH_DECCTL_SET_TELEMETRY_BITS,&arg,sizeof(arg));
    }*/
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

		return true;
	}

static ogg_int64_t Frames = 0;
bool CheckNewFrame(){
			 
	ogg_int64_t FramesTmp = th_granule_frame( td, videobuf_granulepos );
	//Log( "Got frame : " + NxVideoUtils::ToString(  FramesTmp  ) );
	if( FramesTmp != Frames ){ Frames = FramesTmp; return true; }
	return false;
}


	bool HanldeLoop(){

		if( !videobuf_ready && feof(infile)) {  // If end of file => loop

			// MessageBox(0,"FIN","",0);
 
			last=0;
			up=0;
			audiofd = -1;
			audiofd_timer_calibrate=-1;
			ogg_stream_reset(&vo);

			ogg_int64_t granpos = 0;
			int  rc = th_decode_ctl(td, TH_DECCTL_SET_GRANPOS, &granpos,sizeof(granpos));
			assert(rc == 0);
			//tstream->prev_framenum = -1;

			rc = ogg_sync_reset(&oy);
			assert(rc == 0);
 
			int seeked = fseek(infile, 0, SEEK_SET);
			assert(seeked); 

			th_decode_ctl(td,TH_DECCTL_GET_PPLEVEL_MAX,&pp_level_max, sizeof(pp_level_max));
			pp_level=pp_level_max;
			th_decode_ctl(td,TH_DECCTL_SET_PPLEVEL,&pp_level,sizeof(pp_level));
			pp_inc=0;

			Log( "SEEK beggining..." );
			return false;  

		}  

		return true;

	}

 


	bool NxVideo_Theora_Player::NewFrame() {

		//videobuf_ready = 0;
 
		 buffer_data(infile,&oy);
		  while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }

		//if( ogg_sync_pageout(&oy, &og) > 0 ) { queue_page(&og);  return false;   } 

		if(ogg_stream_packetout(&to,&op)>0){  
 
			//if(op.granulepos>=0){ th_decode_ctl(td,TH_DECCTL_SET_GRANPOS,&op.granulepos, sizeof(op.granulepos)); }
 
			int ret = th_decode_packetin(td,&op,&videobuf_granulepos);
			if(ret==0){
				videobuf_time=th_granule_time(td,videobuf_granulepos);

				 //Log( "grenule time : " + NxVideoUtils::ToString(  videobuf_time ) + " ==> time : " + NxVideoUtils::ToString(  get_time() ) );

				if(videobuf_time>=get_time()){ 
					//Log( "Got frame at : " + NxVideoUtils::ToString(  videobuf_time  ) );
					videobuf_ready=0;
					if( CheckNewFrame() ){ videobuf_ready=1; }	 
				}
			} 
			else if (ret == TH_DUPFRAME)  // same as previous frame, don't bother decoding it
			{  Log( "TH_DUPFRAME" ); return false;  }
			else{ Log( "TH_UNKNOWN" ); return false; }

		}else{
			//Log( "ogg_stream_packetout is zero or minus" );
			HanldeLoop(); return false;
		}



			 

 
	//	if( !HanldeLoop()){ Log( "loop false" ); return false; }
	 
		/*
		if( !videobuf_ready ){  // || !audiobuf_ready
			// no data yet for somebody.  Grab another page  
			buffer_data(infile,&oy);
			while(ogg_sync_pageout(&oy,&og)>0){ queue_page(&og); }

			
		}*/

	 
		if(videobuf_ready) { /*Log( "return true" );*/ return true;}//RenderFrame(); 
		Log( "return false" );
		return false;

 
	}

 

	//! Render a Frame.
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
				//glPixelStorei(GL_UNPACK_ROW_LENGTH, yuv[i].stride); // doesnt compile on android
				glTexSubImage2D( GL_TEXTURE_2D, 0 , 
					0, 0, mVideoWidth[i], mVideoHeight[i],
					GL_LUMINANCE , GL_UNSIGNED_BYTE, yuv[i].data );
			}

			videobuf_ready = 0 ;
		}



		//faire boulcer le tout


	//	#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_ANDROID

		//Log("CALLED.. 1 ");

		//if(mOpengGlID){

			// Log("CALLED.. 2 ");

			//wglMakeCurrent( wglGetCurrentDC() , wglGetCurrentContext());


			//EGLBoolean result = eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_);
			 //Log("binding ID : " + NxVideoUtils::ToString( mOpengGlID )  );

			//glBindTexture( GL_TEXTURE_2D, mOpengGlID );
			//int w, h;
			//int miplevel = 0;
			//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
			//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

			//Log("VIDEO WIDTH 2: " + NxVideoUtils::ToString( w ));

			//th_ycbcr_buffer yuv;
			//th_decode_ycbcr_out(td,yuv);

			//th_img_plane plane1 = yuv[0];
			//th_img_plane plane2 = yuv[1];
			//th_img_plane plane3 = yuv[2];


			/*
			Log("PLANE 1 WIDTH : " + NxVideoUtils::ToString( plane1.width ));
			Log("PLANE 1 HEIGHT : " + NxVideoUtils::ToString( plane1.height ));
			Log("PLANE 1 STRIDE : " + NxVideoUtils::ToString( plane1.stride ));
			 
			Log("PLANE 2 WIDTH : " + NxVideoUtils::ToString( plane2.width ));
			Log("PLANE 2 HEIGHT : " + NxVideoUtils::ToString( plane2.height ));
			Log("PLANE 2 STRIDE : " + NxVideoUtils::ToString( plane2.stride ));

			Log("PLANE 3 WIDTH : " + NxVideoUtils::ToString( plane3.width ));
			Log("PLANE 3 HEIGHT : " + NxVideoUtils::ToString( plane3.height ));
			Log("PLANE 3 STRIDE : " + NxVideoUtils::ToString( plane3.stride ));*/

			//glTexSubImage2D( GL_TEXTURE_2D, 0 , 
			//	0, 0, 512, 512,
			//	GL_LUMINANCE , GL_UNSIGNED_BYTE,
			//	plane1.data );

		//}


//#endif
		/*
		th_ycbcr_buffer yuv;
		int y_offset, uv_offset;
		th_decode_ycbcr_out(td,yuv);
		_transcode( yuv, data, mVideoWidth , mVideoHeight );

		videobuf_ready = 0;
		*/


		//memcpy( &data[0], m_video->mBuffer, mVideoWidth *  mVideoHeight *  mVideoBpp );
	}

	void NxVideo_Theora_Player::SetFramerate( float framerate ) {
	
	}





}