#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
#include "FileReadFlac.h"
#include "libFlac/stream_decoder.h"

  
using namespace stk;

namespace NxSound_Namespace { // stk

FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);

FLAC__StreamDecoder * decoder ;


FileReadFlac::FileReadFlac() : FileReadBase()
{

}

FileReadFlac::FileReadFlac( std::string fileName ) : FileReadBase()
{
	Open( fileName );
}

FileReadFlac::~FileReadFlac()
{
	if ( mFile ) fclose( mFile );
}

void FileReadFlac::Close( void )
{
	if ( mFile ) fclose( mFile );
	mFile = 0;
	//wavFile_ = false;
}

bool FileReadFlac::IsOpen( void )
{
	if ( mFile ) return true;
	else return false;
}

void FileReadFlac::Open( std::string fileName )
{
	// If another file is open, close it.
	Close();

	mFile = fopen( fileName.c_str(), "rb" );

	if ( !mFile ) Log( "FileRead::open: could not open or find file" );

	if(!getFlacInfo( fileName.c_str() )){ Log("getFlacInfo Error !"); }
	return;
}

NxAudioFormat FileReadFlac::GetNativeResolution() 
{
	return mDataType ;
}
 
bool FileReadFlac::getFlacInfo( const char *fileName )
{
	decoder = 0;
	FLAC__StreamDecoderInitStatus init_status;

	unsigned char header[35];
	bool isOgg = false;
	fread( header, 1, 35, mFile );
	if ( memcmp(header, "OggS", 4) == 0 && memcmp(&header[29], "FLAC", 4) == 0 ) { 
		Log( "NxSound : FLAC is an ogg file" );
		isOgg = true;
	}

	fseek( mFile, 0, SEEK_END );
	mFileSize = ftell(mFile);
	rewind( mFile );

	if((decoder = FLAC__stream_decoder_new() ) == NULL)  {
		Log( "NxSound : FLAC ERROR: allocating decoder." );
		fclose( mFile );
		return false;
	}

	FLAC__stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_STREAMINFO);
	FLAC__stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_VORBIS_COMMENT);

	FLAC__stream_decoder_set_md5_checking( decoder, true);

	// is it Ogg Flac ?
	if ( FLAC_API_SUPPORTS_OGG_FLAC && isOgg ) { 
		init_status = FLAC__stream_decoder_init_ogg_FILE( decoder, mFile, ( FLAC__StreamDecoderWriteCallback ) write_callback, (FLAC__StreamDecoderMetadataCallback) metadata_callback, ( FLAC__StreamDecoderErrorCallback )error_callback, this );
	}
	else { 
		init_status = FLAC__stream_decoder_init_FILE( decoder, mFile, ( FLAC__StreamDecoderWriteCallback ) write_callback, (FLAC__StreamDecoderMetadataCallback) metadata_callback, ( FLAC__StreamDecoderErrorCallback ) error_callback, this );
	}
 
	if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK)  {
		fprintf(stderr, "ERROR: initializing decoder: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
		return false;
	}

	//fire flac information
	FLAC__stream_decoder_process_until_end_of_metadata( decoder );

	if ( mSampleNum == 0 && mFileRate > 0 )
	{
		// the length hasn't been stored in the metadata, so we'll need to
		// work it out the length the hard way, by scanning the whole file..
		// scanningForLength = true;
		NxSoundLog::getSingleton().LogMessage( "NxSound : FLAC could not find file sample size" );
		FLAC__stream_decoder_process_until_end_of_stream( decoder );
		//scanningForLength = false;
		FLAC__int64 tempLength = mSampleNum;

		FLAC__stream_decoder_reset( decoder);
		FLAC__stream_decoder_process_until_end_of_metadata( decoder);
		mSampleNum = tempLength;
	}

	FLAC__ChannelAssignment ChanAssign = FLAC__stream_decoder_get_channel_assignment( decoder );

	//dataType_ = Stk::STK_SINT16;
	//fileSize_ = mSampleNum; 
//	byteswap_ = false;
	SampleFirstPosition = 0;

	return true;
}

/*
void FileReadFlac::normalize( StkFloat peak , NxSound_Namespace::NxSoundFrames& buffer )
{
	size_t i;
	StkFloat max = 0.0;

	for ( i=0; i<buffer.size(); i++ ) { // check for the highest peak in buffer sample
		if ( fabs( buffer[i] ) > max ) max = (StkFloat) fabs((double) buffer[i]);
	}

	if ( max > 0.0 )  { // if we found a highpeak
		max = 1.0 / max;
		max *= peak;
		for ( i=0; i<buffer.size(); i++ )
		buffer[i] *= max;
	}
}
*/

void FileReadFlac::Read( NxSoundFrames& buffer, unsigned long startFrame, bool doNormalize )
{

	seek( startFrame );//triggers flac callback.

	for( int y = 0; y < buffer.frames(); y++ ) {

		buffer(y,0) = BufferFlacL[y]* mResolutionCoef ;
		buffer(y,1) = BufferFlacR[y]* mResolutionCoef ;
	}
	return ;
}

bool FileReadFlac::seek( uint64 sampleNum )
{
	if(!mFile) { return false; }

	FLAC__uint64 FlacSampleNum = sampleNum;
 
	if(!FLAC__stream_decoder_seek_absolute( decoder, FlacSampleNum ))
		Log( "NxSound : FLAC__stream_decoder_seek_absolute failed for Sample Num : " + NxSoundUtils::ToString( sampleNum ) );

	FLAC__StreamDecoderState State = FLAC__stream_decoder_get_state( decoder );

	if( State ==  FLAC__STREAM_DECODER_END_OF_STREAM ) { // end of file
		Log("Flac File Ended");
	}
	else if( State == FLAC__STREAM_DECODER_SEEK_ERROR ) {
		FLAC__stream_decoder_flush( decoder );
		FLAC__stream_decoder_reset( decoder );
		Log( "NxSound : FLAC__STREAM_DECODER_SEEK_ERROR " );
	} 

	return true;
}

FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	assert(client_data != NULL);
	FileReadFlac *instance = reinterpret_cast<FileReadFlac *>(client_data);
	assert(instance != NULL);
	instance->OnWriteCallback( frame, buffer );
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void FileReadFlac::OnWriteCallback( const void *frame, const int * const buffer[] )
{

	for( int y = 0; y < ( (FLAC__Frame*) frame)->header.blocksize; y++ )
	{
		BufferFlacL[y] = buffer[0][y];
		BufferFlacR[y] = buffer[1][y];
	}


}


void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	assert(client_data != NULL);
	FileReadFlac *instance = reinterpret_cast<FileReadFlac *>(client_data);
	assert(instance != NULL);
	instance->OnMetadataCallback(metadata);
}

void FileReadFlac::OnMetadataCallback( const void * Flacmetadata )
{
	const FLAC__StreamMetadata * metadata = (FLAC__StreamMetadata *) Flacmetadata;
 
	if( metadata->type == FLAC__METADATA_TYPE_STREAMINFO )
	{
		mSampleNum = metadata->data.stream_info.total_samples;
		mFileRate = metadata->data.stream_info.sample_rate;
		mChannels = metadata->data.stream_info.channels;
		int Bps = metadata->data.stream_info.bits_per_sample;
		// (bits per sample)-1. FLAC supports from 4 to 32 bits per sample. Currently the reference encoder and decoders only support up to 24 bits per sample. 

		if( Bps == 24 ) {
			mDataType =  NX_SINT24;// Stk::STK_SINT24;
			Log("Flac File is 24 bits");
		}
		else if( Bps == 16 ){
			mDataType =  NX_SINT16;//Stk::STK_SINT16;
			Log("Flac File is 16 bits");
		}
		else if( Bps == 8 ) {
			mDataType = NX_SINT8;// Stk::STK_SINT8;
			Log("Flac File is 8 bits");
		}
		else {
			Log("Flac File : Could not find Bps information");
		}
  
		unsigned int m_maxFrameSize = metadata->data.stream_info.max_framesize;
		unsigned int m_minFrameSize = metadata->data.stream_info.min_framesize;
		unsigned int m_maxBlockSize = metadata->data.stream_info.max_blocksize;
		unsigned int m_minBlockSize = metadata->data.stream_info.min_blocksize;

		mBlockSize = m_maxBlockSize;
 
		Log( "NxSound : FLAC FILESIZE : " + NxSoundUtils::ToString(  GetFileSize()   ) ); 
		Log( "NxSound : FLAC total Samples : " + NxSoundUtils::ToString(  mSampleNum ) ); 
		Log( "NxSound : FLAC Sample Rate: " + NxSoundUtils::ToString(  mFileRate  ) );
		Log( "NxSound : FLAC Num Channels: " + NxSoundUtils::ToString(  mChannels  ) );
		Log( "NxSound : FLAC Bits Per Sample : " + NxSoundUtils::ToString(  Bps ) );
		Log( "NxSound : FLAC Max FrameSize : " + NxSoundUtils::ToString( m_maxFrameSize ) );
		Log( "NxSound : FLAC Min FrameSize : " + NxSoundUtils::ToString( m_minFrameSize ) );
		Log( "NxSound : FLAC Max BlockSize : " + NxSoundUtils::ToString( m_maxBlockSize ) );
		Log( "NxSound : FLAC Min BlockSize  : " + NxSoundUtils::ToString( m_minBlockSize ) );

	}
	else if (metadata->type == FLAC__METADATA_TYPE_VORBIS_COMMENT) 
	{
		/*
		for(unsigned int i = 0; i < metadata->data.vorbis_comment.num_comments; i++) {
			QString thiscomment = QString::fromLatin1((const char*)metadata->data.vorbis_comment.comments[i].entry, metadata->data.vorbis_comment.comments[i].length);
			if (thiscomment.left(6).upper() == "TITLE=") 
			{
				this->metadata.SetTrackName(thiscomment.right(thiscomment.length() - 6));
			} else if (thiscomment.left(6).upper() == "ALBUM=") 
			{
				this->metadata.SetAlbumName(thiscomment.right(thiscomment.length() - 6));
			} else if (thiscomment.left(7).upper() == "ARTIST=")
			{
				this->metadata.SetArtistName(thiscomment.right(thiscomment.length() - 7));
			} else if (thiscomment.left(12).upper() == "TRACKNUMBER=")
			{
				this->metadata.SetTrackNumber(thiscomment.right(thiscomment.length() - 12));
			} else if (thiscomment.left(20).upper() == "MUSICBRAINZ_TRACKID=")
			{
				this->metadata.SetTrackID(thiscomment.right(thiscomment.length() - 20));
			} else if (thiscomment.left(21).upper() == "MUSICBRAINZ_ARTISTID=")
			{
				this->metadata.SetArtistID(thiscomment.right(thiscomment.length() - 21));
			} else if (thiscomment.left(20).upper() == "MUSICBRAINZ_ALBUMID=")
			{
				this->metadata.SetAlbumID(thiscomment.right(thiscomment.length() - 20));
			} else if (thiscomment.left(18).upper() == "MUSICBRAINZ_TRMID=") 
			{
				this->metadata.SetTRMID(thiscomment.right(thiscomment.length() - 18));
			}
		}
		*/

	}

}

void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	//(void)decoder, (void)client_data;
	std::string ErrorStr = FLAC__StreamDecoderErrorStatusString[status];
	NxSoundLog::getSingleton().LogMessage("Flac callback Error : " + ErrorStr );
}

} // stk namespace

