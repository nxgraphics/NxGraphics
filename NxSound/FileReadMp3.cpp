#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
#include "FileReadmp3.h"
using namespace stk;

const int XING_FRAMES = 0x00000001L;
const int XING_BYTES = 0x00000002L;
const int XING_TOC = 0x00000004L;
const int XING_SCALE = 0x00000008L;

namespace NxSound_Namespace { //stk

template <class Type>
inline Type jlimit (const Type lowerLimit, const Type upperLimit, const Type valueToConstrain) throw() {
	return (valueToConstrain < lowerLimit) ? lowerLimit : ((valueToConstrain > upperLimit) ? upperLimit : valueToConstrain);
}

#define NormalizeToIEEEfloat(mad) float(jlimit(double(-1.f), double(1.f), (mad * double(1.0 / (1 << MAD_F_FRACBITS))))) 

FileReadMp3::FileReadMp3() : FileReadBase()
{

}

FileReadMp3::FileReadMp3( std::string fileName ) : FileReadBase()
{
	Open( fileName );
}

FileReadMp3::~FileReadMp3()
{
	if ( mFile ) fclose( mFile );
}

void FileReadMp3::Close( void )
{
	if ( mFile ) fclose( mFile );
	mFile = 0;
 
}

bool FileReadMp3::IsOpen( void )
{
  if ( mFile ) return true;
  else return false;
}

NxAudioFormat FileReadMp3::GetNativeResolution() 
{
	return mDataType ;
}

void FileReadMp3::Open( std::string fileName )
{
	Close(); // If another file is open, close it.
	mFile = fopen( fileName.c_str(), "rb" ); // Try to open the file.

	if ( !mFile ) {
		 Log("FileRead::open: could not open or find file (" + fileName + ")!");
	}
 
	if(!getMp3Info( fileName.c_str() )){  Log( "getMp3Info Error !" ); }
}

bool FileReadMp3::getMp3Info( const char *fileName )
{
	/* from JUCED class MADAudioFormatReader copyright @kunitoki */
	int  _samplesLeftInBuffer = 0;
	// Skip over ID3v2 tag (if there is one)
	{
		unsigned char tag_[10];
		memset(&tag_, 0, sizeof(tag_));
		int bytesRead = fread(&tag_, 1, sizeof(tag_), mFile );

		if(bytesRead == sizeof(tag_) && tag_[0] == 'I' && tag_[1] == 'D' && tag_[2] == '3') {
			SampleFirstPosition = sizeof(tag_);
			SampleFirstPosition += (int(tag_[6]) << 21)| (int(tag_[7]) << 14)| (int(tag_[8]) << 7)| int(tag_[9]);
		}
	}

	fseek( mFile, SampleFirstPosition , SEEK_SET ) ;	
	mad_stream_init(&_madStream);
	mad_frame_init(&_madFrame);
	mad_synth_init(&_madSynth);

	if(scanHeader(&_madFrame.header, &_xing) != -1)
	{
		fseek( mFile, SampleFirstPosition , SEEK_SET ) ;
		fseek (mFile , 0 , SEEK_END); 

	     _streamSize = ftell (mFile); 
		mFileSize = ftell (mFile);

		rewind (mFile);
		fseek( mFile, SampleFirstPosition , SEEK_SET ) ;

		_madSynth.pcm.length = 0;
        _madCurTime = mad_timer_zero;
		
		if(_xing.flags & XING_FRAMES)
		{
			_madTimeLength = _madFrame.header.duration;
            mad_timer_multiply(&_madTimeLength, _xing.frames);
        }
		else
		{
			// Estimate playing time from file size
            mad_timer_set(&_madTimeLength, 0, 1, _madFrame.header.bitrate / 8);
			mad_timer_multiply(&_madTimeLength, (long)_streamSize  );// original
        }
 
		MADsampleRate = _madFrame.header.samplerate;
        mSampleNum = int64(double(mad_timer_count(_madTimeLength, MAD_UNITS_MILLISECONDS)) / 1000.f * double(MADsampleRate) + .5f);
        MADnumChannels = MAD_NCHANNELS(&_madFrame.header);

		float len = ((float)_streamSize * 8000.0f ) / ((float) _madFrame.header.bitrate );// len millisecnds
		float lengthInSamples2 =  len   / 1000.f * float(MADsampleRate) + .5f    ;

		mFileSize = mSampleNum ; 
		mDataOffset = SampleFirstPosition ;

		long m_iFileLength_Secs = mad_timer_count( _madTimeLength , MAD_UNITS_SECONDS);
		double m_iBitRate_Kbs =  (double ) _madFrame.header.bitrate / 1000.0;
		unsigned int m_iFreq_Hz = _madFrame.header.samplerate;
		bool m_bStereo = _madFrame.header.mode == MAD_MODE_SINGLE_CHANNEL ? false : true ;

	}//scan header

	mDataType =  NX_SINT24;  //Stk::STK_FLOAT32;
	// Get file sample rate from the header.
	mFileRate =  (StkFloat) MADsampleRate;
	// Get number of channels from the header.
	mChannels = MADnumChannels ;//chans;
//	byteswap_ = false;
	return true;
}


bool FileReadMp3 ::seek(signed long long sample)
{
	// libmad decodes the mp3s as 32-bit signed integer samples
	if(!mFile) { return false; }
    int64 milliseconds_ = mad_timer_count(_madTimeLength, MAD_UNITS_MILLISECONDS);
    int64 currentMillisecond_ = int64(double(sample) / double(MADsampleRate) * 1000);
    double fraction_ = double(currentMillisecond_) / double(milliseconds_);
    mad_timer_set(&_madCurTime, currentMillisecond_, currentMillisecond_, MAD_UNITS_MILLISECONDS);

	if(_xing.flags & XING_TOC) {

		int percent_, p1_, p2_;
        percent_ = int(fraction_ * 100);
        p1_ = (percent_ < 100) ? _xing.toc[percent_    ] : 0x100;
        p2_ = (percent_ <  99) ? _xing.toc[percent_ + 1] : 0x100;

        fraction_ = (p1_ + (p2_ - p1_) * (fraction_ * 100 - percent_)) / 0x100;
    }

    int64 pos_ = int64(double(_streamSize) * fraction_);
	fseek( mFile, pos_ ? pos_ : GetFirstSamplePosition() , SEEK_SET ) ;
	int size = fread(_buffer, 1 , sizeof(_buffer)  ,  mFile  );
				 
	if(!size) {
		_madSynth.pcm.length = 0;
        _samplesLeftInBuffer = 0;
		return false;
    }

    mad_stream_buffer(&_madStream, _buffer, size);
    mad_frame_mute(&_madFrame);
    mad_synth_mute(&_madSynth);

    int skip_ = 2;

    do
    {
		if(!mad_frame_decode(&_madFrame, &_madStream))
        {
			mad_timer_add(&_madCurTime, _madFrame.header.duration);

			if(!(--skip_)) 
			mad_synth_frame(&_madSynth, &_madFrame);
		}
		else
		if(!MAD_RECOVERABLE(_madStream.error))
		break;
	}
	while(skip_);

	_madSynth.pcm.length = 0;
    _samplesLeftInBuffer = 0;
	return true;
}

int FileReadMp3::parseXing(struct xing *xing,  mad_bitptr ptr, unsigned int bitlen)
{
	if (bitlen < 64 || mad_bit_read(&ptr, 32) != (('X' << 24) | ('i' << 16) | ('n' << 8) | 'g'))
    { xing->flags = 0; return -1; }
    xing->flags = mad_bit_read(&ptr, 32);
    bitlen -= 64;
    if (xing->flags & XING_FRAMES)
	{
		if (bitlen < 32){ xing->flags = 0; return -1; }
		xing->frames = mad_bit_read(&ptr, 32);
		bitlen -= 32;
	}
	if (xing->flags & XING_BYTES) 
	{
		if (bitlen < 32){ xing->flags = 0; return -1; }
		xing->bytes = mad_bit_read(&ptr, 32);
		bitlen -= 32;
	}
	if (xing->flags & XING_TOC) 
	{
		int i;
		if (bitlen < 800){ xing->flags = 0; return -1; }
		for (i = 0; i < 100; ++i)
		xing->toc[i] = (unsigned char) mad_bit_read(&ptr, 8);
		bitlen -= 800;
	}
	if (xing->flags & XING_SCALE) 
	{
		if (bitlen < 32)
		{ xing->flags = 0; return -1; }
		xing->scale = mad_bit_read(&ptr, 32);
		bitlen -= 32;
	}			
	return 0;
}

int FileReadMp3::scanHeader( struct mad_header * header, struct xing * xing )
{
	struct mad_stream stream;
	struct mad_frame frame;
	unsigned char buffer[8192];
	unsigned int buflen = 0;
	int count = 0, result = 0;

	mad_stream_init(&stream);
	mad_frame_init(&frame);

	if (xing){ xing->flags = 0; }

	while(1) {
		if(buflen < sizeof(buffer)) {
			int64 bytes =  fread( buffer + buflen , 1, sizeof(buffer) - buflen,   mFile); 
            if(!bytes) { result = -1; break; }
			buflen += int(bytes);
		}

		mad_stream_buffer(&stream, buffer, buflen);
		while(1) {
			if(mad_frame_decode(&frame, &stream) == -1) {
				if(!MAD_RECOVERABLE(stream.error)){ break; }
				continue;
			}

			if(count++ || (xing && parseXing(xing, stream.anc_ptr, stream.anc_bitlen) == 0)){ break; }
		}

		if (count || stream.error != MAD_ERROR_BUFLEN){ break; }
		memmove(buffer, stream.next_frame, (buflen = &buffer[buflen] - stream.next_frame));
	}

	if(count) {
		if(header)
		*header = frame.header;
    }
	else
    result = -1;

    mad_frame_finish(&frame);
	mad_stream_finish(&stream);
    return result;
}

void FileReadMp3::Read( NxSoundFrames& buffer, unsigned long startFrame, bool doNormalize )
{
	// Check the buffer size.
	unsigned int nFrames = buffer.frames();

	// Check for file end.
	if ( startFrame + nFrames >= mFileSize ){ nFrames = mFileSize - startFrame;}

	long i, nSamples = (long) ( nFrames * mChannels );
	unsigned long offset = startFrame * mChannels;

	if(startFrame >= mSampleNum){ Log("mp3 returned"); return  ;}

	if(startFrame + nSamples > mSampleNum){ nSamples = mSampleNum - startFrame; }

    if(startFrame != mLastSampleNum) {
		if(seek( startFrame  )) 
        mLastSampleNum = startFrame;
        else {
			mSampleNum = mLastSampleNum;
			return  ;
        }
	}

	nSamples = 1152; //libmad provides up to 1152 samples.
    int numToRead_ = 0;

	while(mad_frame_decode(&_madFrame, &_madStream) == -1)
    {
		if(MAD_RECOVERABLE(_madStream.error))
        continue;

        int shift = 0;

        if(_madStream.next_frame)
        {
			shift = _madStream.bufend - _madStream.next_frame;
            memmove(_buffer, _madStream.next_frame, (size_t)shift);
        }
                 
		int64 bytes = fread(_buffer + shift,1, sizeof(_buffer) - shift, mFile);
        if(!bytes)
        {
			mSampleNum = mLastSampleNum;
			_madSynth.pcm.length = 0;
			_samplesLeftInBuffer = 0;
			return  ;
		}

		mad_stream_buffer( &_madStream, _buffer, (long)(shift + bytes));

	}// while frame decode

	mad_synth_frame(&_madSynth, &_madFrame);

	unsigned short length = _madSynth.pcm.length ;

    _samplesLeftInBuffer = _madSynth.pcm.length;
    _leftChannel = _madSynth.pcm.samples[0];
    _rightChannel = _madSynth.pcm.samples[1];

    mad_timer_add(&_madCurTime, _madFrame.header.duration);

    numToRead_ = (_samplesLeftInBuffer <= nSamples) ? _samplesLeftInBuffer : nSamples;

	nSamples -= numToRead_;
    _samplesLeftInBuffer -= numToRead_;
    mLastSampleNum += numToRead_;

	unsigned int j, counter = 0;
	for ( unsigned int i=0; i< numToRead_  ; i++)
	{
		 buffer( i, 0 ) = NormalizeToIEEEfloat( _leftChannel[i] );
		 buffer( i, 1 ) = NormalizeToIEEEfloat( _rightChannel[i] );
	}

	return ;
}

} // stk namespace
