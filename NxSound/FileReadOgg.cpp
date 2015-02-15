#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
#include "FileReadOgg.h"


#define OV_EXCLUDE_STATIC_CALLBACKS 1
#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisenc.h"
#include "vorbis/vorbisfile.h"



using namespace stk;

namespace NxSound_Namespace { //stk

OggVorbis_File  oggStream;     // stream handle
vorbis_info*    vorbisInfo;    // some formatting data
vorbis_comment* vorbisComment; // user comments

size_t read_istream(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	FILE *hfile=(FILE *)datasource;
	return fread(ptr, 1, size*nmemb, hfile);
}

int seek_istream(void *datasource, ogg_int64_t offset, int whence)
{
	FILE *hfile=(FILE *)datasource;
	long integer_offert=(long)offset;
	int returnVal;
	switch(whence)
	{
	case SEEK_SET:
	case SEEK_CUR:
	case SEEK_END:
		returnVal=fseek(hfile, integer_offert, whence);
		break;
	default:		//Bad value
		return -1;
	}

	if(returnVal==0)
		return 0;
	else
		return -1;		//Could not do a seek. Device not capable of seeking. (Should never encounter this case)
}

int close_istream(void *datasource)
{
	FILE *hfile=(FILE *)datasource;
	fclose(hfile);
	return 0;
}

long tell_istream(void *datasource)
{
	FILE *hfile=(FILE *)datasource;
	return ftell(hfile);
}


//Collect the function pointers into an array
//***************************************************
//This will be sent to ov_open_callbacks()
const ov_callbacks istream_ogg_callbacks =
{
	read_istream,
	seek_istream,
	close_istream,
	tell_istream
};


FileReadOgg::FileReadOgg() : FileReadBase()
{
}

FileReadOgg::FileReadOgg( std::string fileName ) : FileReadBase()
{
	Open( fileName );
}

FileReadOgg::~FileReadOgg()
{
  if ( mFile ) fclose( mFile );
}

void FileReadOgg::Close( void )
{
  if ( mFile ) fclose( mFile );
  mFile = 0;
}

bool FileReadOgg::IsOpen( void )
{
  if ( mFile ) return true;
  else return false;
}

void FileReadOgg::Open( std::string fileName )
{
	// If another file is open, close it.
	Close();

    if(!(mFile = fopen( fileName.c_str(), "rb" )))
        throw string("Could not open Ogg file.");

	int mOggResult = ov_open_callbacks(mFile, &oggStream, NULL, 0, istream_ogg_callbacks);
	if(mOggResult<0)
	{
		fclose(mFile);
		mFile=0;
		return ;
	}

	if(!getOggInfo( fileName.c_str() )) Log("getOggInfo Failed !");
		 
	return;
	
}

NxAudioFormat FileReadOgg::GetNativeResolution() 
{
	return mDataType ;
}

bool FileReadOgg::getOggInfo( const char *fileName )
{
    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);
	mDataType = NX_FLOAT32 ;//Stk::STK_FLOAT32;// converted to 32 bits
	mFileRate = vorbisInfo->rate;
	mChannels = vorbisInfo->channels ; 
	mFileSize = ov_pcm_total(&oggStream,-1);
	mSampleNum = ov_pcm_total(&oggStream,-1);
    double duration = ov_time_total(&oggStream,-1);
	//byteswap_ = false;
	SampleFirstPosition = 0;
	return true;
}

bool FileReadOgg ::seek(signed long long sample)
{
	if(!mFile) { return false; }
	//seeking is provided in the read method.
	return true;
}

void FileReadOgg::Read( NxSoundFrames& buffer, unsigned long startFrame, bool doNormalize )
{
	float **pcm_channels = 0;
	int nbytes = buffer.frames();
	int total_bytes_read = 0;

 	while( nbytes > 0 ) 
 	{
		int SeekRes = ov_pcm_seek_lap( &oggStream, startFrame + total_bytes_read );
		if( SeekRes != 0 )
		{
			switch(SeekRes )
			{
				case OV_ENOSEEK :
					Log("Bitstream is not seekable.");
				break;
				case OV_EINVAL :
					Log("Invalid argument value; possibly called with an OggVorbis_File structure that isn't open.");
				break;
				case OV_EREAD :
					Log("A read from media returned an error.");
				break;
				case OV_EFAULT :
					Log("Internal logic fault; indicates a bug or heap/stack corruption.");
				break;
				case OV_EBADLINK :
					Log("Invalid stream section supplied to libvorbisfile, or the requested link is corrupt.");
				break;
			}
		}

		int bitstream;
		long bytes = ov_read_float( &oggStream, &pcm_channels, nbytes, &bitstream );
		if( bytes == OV_HOLE || bytes == OV_EBADLINK || bytes == EOF ) {
			Log("NxSound : Ogg ov_read_float Error !"); return;
		}

/*		OV_HOLE indicates there was an interruption in the data. (one of: garbage between pages, loss of sync followed by recapture, or a corrupt page)
		OV_EBADLINK indicates that an invalid stream section was supplied to libvorbisfile, or the requested link is corrupt.
		EOF indicates end of file
		else indicates actual number of bytes read. ov_read() will decode at most one vorbis packet per invocation, so the value returned will generally be less than length.*/ 

		total_bytes_read += bytes;
		nbytes -= bytes;

		for(int i = 0 ; i < bytes; i++ ) {
			buffer(i+total_bytes_read-bytes,0) = pcm_channels[0][i] ;
			buffer(i+total_bytes_read-bytes,1) = pcm_channels[1][i] ;
		}
 	}

	return ;
}

} // stk namespace