#include "NxSoundPreCompiled.h"
#include "../nxdeps/include/STK/include/Thread.h"

/* safe bounds! */
#define INBUF OUT_CHUNK*sizeof(float)*8
#define RSMPBUF INBUF*sizeof(float)*12

//should be requested as foloow : 
//http://127.0.0.1:8000/stream.ogg

//unity request http headers 
//GET /dede.ogg HTTP/1.1
//User-Agent: UnityPlayer/2.6.1f3 (http://unity3d.com)
//Host: 127.0.0.1:8000
//Accept: */*

#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisenc.h"
//#include "vorbis/vorbisfile.h" 
#include "STK/include/TcpServer.h"

using namespace stk;

namespace NxSound_Namespace {

	struct NxSoundEffectOggBroadCaster::ogg_page
	{
		unsigned char *header;
		long header_len;
		unsigned char *body;
		long body_len;
	};
 
	struct encoder_s
	{
		ogg_stream_state os;
		ogg_page og;
		ogg_packet op;
		vorbis_dsp_state vd;
		vorbis_block vb;
		vorbis_info vi;
		vorbis_comment vc;

		ogg_packet header_main;
		ogg_packet header_comments;
		ogg_packet header_codebooks;

		int channels;
		int rate;
		int samplesize;
		int bytespersec;
	};

NxSoundEffectOggBroadCaster::NxSoundEffectOggBroadCaster()
{
	mType = Effect_OggBroadCast ;
	Initialized = false ;
}

NxSoundEffectOggBroadCaster::~NxSoundEffectOggBroadCaster()
{
	
}

int NxSoundEffectOggBroadCaster::write_page( ogg_page * page, int sock, bool flush )
{
	//if( !Initialized ) return 0;
	int BytesSent = 0 ;
	int i = 0 ;
	BytesSent = sendto( sock, (const char *) page->header, page->header_len , 0 , 0 ,0 );

	/*
	if( BytesSent == SOCKET_ERROR )
	{ 
		for ( vector<int>::iterator Iter = ServerSocketList.begin( ) ; Iter != ServerSocketList.end( ) ; Iter++ )
		{
			if( *Iter == sock  )
			{

				
				NxSoundLog::getSingleton().LogMessage(" socket pre deleted ");
				closesocket( sock );
				//delete ServerSocketList[i];
				ServerSocketList.erase( Iter ) ;
				sock = NULL ;
				NxSoundLog::getSingleton().LogMessage(" socket deleted ");
				Initialized = false ;
				return 0;
			}
			i++;
		}
		return 0 ;
	}

	*/


	BytesSent += sendto( sock, (const char *) page->body, page->body_len, 0 , 0 ,0 );
	return BytesSent;
	

	return 0 ;
}

void NxSoundEffectOggBroadCaster::SendOggFileHeader( int Sock )
{
	vorbis_analysis_headerout (
	&OggEncoder->vd,
	&OggEncoder->vc,
	&OggEncoder->header_main,
	&OggEncoder->header_comments,
	&OggEncoder->header_codebooks);

	ogg_stream_packetin(&OggEncoder->os,&OggEncoder->header_main);
	ogg_stream_packetin(&OggEncoder->os,&OggEncoder->header_comments);
	ogg_stream_packetin(&OggEncoder->os,&OggEncoder->header_codebooks);

	int result ;
	int ret ;

	while ((result = ogg_stream_flush (&OggEncoder->os, &OggEncoder->og)))
	{
		if (result == 0)
		break;
		ret = write_page ( ( ogg_page * ) &OggEncoder->og, Sock , true );
	}
 
	NxSoundLog::getSingleton().LogMessage( "Created Vorbis Streaming Socket" );
	Initialized = true ;
}


THREAD_RETURN THREAD_TYPE thread_function(void *ptr)
{
	NxSoundEffectOggBroadCaster * mPtr = ( NxSoundEffectOggBroadCaster *) ptr;
	while( true ) {

		int Sock = mPtr->Server->accept();

		std::string Response = // send a stream header to spoof icecast ogg stream with parameters
			"HTTP/1.0 200 OK\r\n"\
			"Content-Type: application/ogg\r\n"\
			"icy-br:224\r\n"\
			"ice-audio-info: bitrate=224;samplerate=44100;channels=2\r\n"\
			"icy-description:News and current affairs station\r\n"\
			"icy-genre:news\r\n"\
			"icy-name:CRo 1 - Radiozurnal\r\n"\
			"icy-pub:1\r\n"\
			"Server: Icecast 2.3.2\r\n"\
			"Cache-Control: no-cache\r\n"\
			"\r\n";

		Log( "sending HTTP response ...." );
		sendto( Sock, Response.c_str(), Response.size(),0 ,0 ,0 );
		NxSoundLog::getSingleton().LogMessage( "HTTP response sent" );
		mPtr->SendOggFileHeader( Sock );
		NxSoundLog::getSingleton().LogMessage( "ogg header sent" );

		mPtr->ServerSocketList.push_back( Sock );

	}

return 0;
}

void NxSoundEffectOggBroadCaster::CreateServer( int Port )
{
	port = Port ;
	Server = new TcpServer(Port);
	mSender = new Thread();
	OggEncoder = new encoder_s();
 
	int channels = 2;
	int rate = 44100;
	int bitrate;
	int serialno = rand();
	int result, ret;

	bitrate = 224 ;
	vorbis_info_init (&OggEncoder->vi);
  
	if (vorbis_encode_setup_managed (&OggEncoder->vi, channels, rate, -1, bitrate * 1000, -1)) {
		NxSoundLog::getSingleton().LogMessage("--- Failed to setup encoder\n");
		vorbis_info_clear (&OggEncoder->vi);
		return ;
	}

	vorbis_encode_ctl (&OggEncoder->vi, OV_ECTL_RATEMANAGE_SET, NULL);
  
	vorbis_encode_setup_init (&OggEncoder->vi);
	vorbis_analysis_init (&OggEncoder->vd, &OggEncoder->vi);
	vorbis_block_init (&OggEncoder->vd, &OggEncoder->vb);

	ogg_stream_init (&OggEncoder->os, serialno);

	vorbis_comment_init (&OggEncoder->vc);
	vorbis_comment_add_tag (&OggEncoder->vc, "Title", "NxSound Plugin");

	char *tagstr = const_cast<char *>("Artist");
    char *valuestr = const_cast<char *>("Unity Ogg Audio Stream");
    vorbis_comment_add_tag(&OggEncoder->vc, tagstr, valuestr);

    char *str = const_cast<char *>("added a tag");
    vorbis_comment_add(&OggEncoder->vc, str);

	mSender->start( THREAD_FUNCTION( thread_function), this );
	Log( "Created Vorbis Streaming Socket" );
}

void NxSoundEffectOggBroadCaster::Process( float ** BufIn , float ** BufOut , int BlockSize )
{
	if( !Initialized ) return ;
	int size = BlockSize  ;
	int i, result, ret, eos = 0;
	static int samples_in_page = 0;
	static int prev_granulepos = 0;
	int rate = 44100;

	float **vorbbuf = vorbis_analysis_buffer (&OggEncoder->vd, size );
	samples_in_page += size / 4;
  
	for (i = 0; i < size ; i++) 
	{
		vorbbuf[0][i] = BufIn[0][i];
		vorbbuf[1][i] = BufIn[1][i];
	}
  
	vorbis_analysis_wrote (&OggEncoder->vd, size );

	while ((ret = vorbis_analysis_blockout (&OggEncoder->vd, &OggEncoder->vb)) == 1) 
	{
		vorbis_analysis (&OggEncoder->vb, NULL);
		vorbis_bitrate_addblock (&OggEncoder->vb);

		while (vorbis_bitrate_flushpacket (&OggEncoder->vd, &OggEncoder->op))
		{
			ogg_stream_packetin (&OggEncoder->os, &OggEncoder->op);

			while (!eos) 
			{
				if (samples_in_page > rate * 2)
				{
					result = ogg_stream_flush (&OggEncoder->os, &OggEncoder->og);
					samples_in_page = 0;
				} else
				result = ogg_stream_pageout (&OggEncoder->os, &OggEncoder->og);
				if (result == 0)
				{
					break;
				}
				samples_in_page -= ogg_page_granulepos (&OggEncoder->og) - prev_granulepos;
				prev_granulepos = ogg_page_granulepos (&OggEncoder->og);

				for( int o = 0 ; o < ServerSocketList.size(); o++ )
				{
					ret = write_page ( ( NxSoundEffectOggBroadCaster::ogg_page *) &OggEncoder->og, ServerSocketList[ o ] , true );
				}
				
				if (ogg_page_eos (&OggEncoder->og))
				{
					eos = 1;
				}
			}
		}
	}
}




}//namespace