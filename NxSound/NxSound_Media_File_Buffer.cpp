#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"


#include <speex/speex_resampler.h>


 #include <fstream>
 

using namespace stk;

namespace NxSound_Namespace {
 
	struct NxSoundMediaFileBufferStruct {

		NxSoundMediaFileBufferStruct(): st(0) {
		
		}

		~NxSoundMediaFileBufferStruct() {
		
		}
 
		SpeexResamplerState *st;
		ofstream * myFile;
	};



	NxSoundMediaFileBuffer::NxSoundMediaFileBuffer( unsigned int numChannels, double sourceSampleRate, double destSampleRate ) : NxSoundMediaFile()
	{
		this->mNumChannels = numChannels;
		mType = TYPE_BUFF ;
		OpenFile( "", false );
		mMuted = false;
		mStopped = false;
		mPaused = false;
		mNeedResample = false;
 
		mSourceSampleRate = sourceSampleRate;
		mDestSampleRate = destSampleRate;

		if( (mDestSampleRate/mSourceSampleRate) != 1.0 ) mNeedResample = true;


		/*
		myFile = new ofstream("data2.bin", ios::out | ios::binary);
		static int index2 = 0;
		if( index2 > 300 ) {
			myFile->close(); exit(-1);
		}
		else{
			myFile->write( (const char *)&data_out[0], outcount*4 ); index2++;
		}*/


		mInfo = new NxSoundMediaFileBufferStruct();



		
		// where nb_channels is the number of channels that will be used (either interleaved or non-interleaved),
		// input_rate is the sampling rate of the input stream, output_rate is the sampling rate of the output 
		// stream and quality is the requested quality setting (0 to 10). The quality parameter is useful for 
		// controlling the quality/complexity/latency tradeoff. Using a higher quality setting means less noise/aliasing,
		// a higher complexity and a higher latency. Usually, a quality of 3 is acceptable for most desktop uses and quality 10
		// is mostly recommended for pro audio work. Quality 0 usually has a decent sound (certainly better than using linear interpolation resampling),
		// but artifacts may be heard. 
		int quality = 3; // 0 - 10 | 3 recommended 
		mInfo->st = speex_resampler_init(numChannels, sourceSampleRate, destSampleRate, quality, NULL);
		speex_resampler_skip_zeros(mInfo->st);

	}

	NxSoundMediaFileBuffer::~NxSoundMediaFileBuffer()
	{
		speex_resampler_destroy(mInfo->st);
		delete mInfo;
		 
	}

	void NxSoundMediaFileBuffer::Flush() {
		while( mBufferList.size() ){
			NxSoundFrames * frame = mBufferList.front();
			delete frame;
			mBufferList.pop_front();
		}
		mBufferList.clear();
	}
 
	void NxSoundMediaFileBuffer::Feed( char * datafloat, unsigned long BufferSize, double samplerate, unsigned short bps ) {
 

		NxSoundFrames * frame = new NxSoundFrames( 0, 2048, mNumChannels );

		double ratio = mDestSampleRate/double(samplerate);

		if( mNeedResample ) { 

			
			unsigned int uincount = BufferSize ;
			// We want to consume all the input data, so we slightly oversize the
			// resampled data buffer so we can fit the output data in. We cannot really
			// predict the output frame count at each call.
			unsigned int outcount, outIterator = BufferSize * ratio + 1 ;
		
			float *data_in = (float*)datafloat;
			float* inchan = (float*)calloc( BufferSize*mNumChannels, sizeof(float));
			float* outchan = (float*)calloc( outIterator*mNumChannels, sizeof(float));

			speex_resampler_set_rate( mInfo->st, samplerate, mDestSampleRate );
		 
			for( int i = 0 ; i < mNumChannels; i++ ) {

				for(int y = 0 ; y < BufferSize; y++ ) { // uninterleave
					inchan[ i*BufferSize + y  ] = data_in[ y * mNumChannels + i  ]; 
				}
 
				int err = speex_resampler_process_float(mInfo->st, i,
					&inchan[ BufferSize * i],
					&uincount,
					&outchan[outIterator * i],
					&outcount); 

			}//for
		
			for( int i = 0 ; i < mNumChannels; i++ ) { //interleave
				for( int y = 0; y < outcount ; y++ ) {
					(*frame)(y,i) =  outchan[ i*outIterator + y ]; 
				} 
			}

			free(inchan);
			free(outchan);

		}else{
 
			const float * data = (const float *)datafloat;
			for( int i = 0 ; i < mNumChannels; i++ ) { // copy data
				for( int y = 0; y < BufferSize ; y++ ) {
					(*frame)(y,i) =  data[ y * mNumChannels + i ]; 
				} 
			} 

		}

		mBufferList.push_back(frame);  
	}

	unsigned long NxSoundMediaFileBuffer::GetNumSamples()
	{
		return 1;
	}

	void NxSoundMediaFileBuffer::Rewind()
	{
		time_ = (StkFloat) 0.0;
	}

	void NxSoundMediaFileBuffer::SetResolution( NxAudioFormat Res )
	{
		mDstResolution = Res;
		 
	}  


	 
 

	bool NxSoundMediaFileBuffer::OpenFile( std::string Filename, bool Normalize )
	{
 
		//doNormalize = false ;
		chunkSize_ = SND_BUFFER_SIZE;//fileflac_->GetBlockSize();
		chunking_ = true;
		chunkPointer_ = 0;//fileflac_->GetFirstSamplePosition() ;
		dataframe.resize( chunkSize_ + 1 , mNumChannels );
		normalizing_ = Normalize; 
 
		//Resize our lastOutputs container.
		lastFrame_.resize( 1, mNumChannels );

		//Set default rate based on file sampling rate.
		this->SetRate( dataframe.dataRate() / Stk::sampleRate() );

		if ( Normalize & !chunking_ ) this->normalize();

		this->reset();

		return true;
	}

	void NxSoundMediaFileBuffer::Play()
	{
		mStopped = false ;
		mPaused = false ;
	}

	void NxSoundMediaFileBuffer::Stop()
	{
		mStopped = true ;
		mPaused = false ;
		this->reset();
	}

	void NxSoundMediaFileBuffer::Pause()
	{
		mStopped = true ;
		mPaused = true ;
	}

	bool NxSoundMediaFileBuffer::isStopped()
	{
		return mStopped ;
	}

	bool NxSoundMediaFileBuffer::isPaused()
	{
		return mPaused ;
	}

	unsigned int NxSoundMediaFileBuffer::GetFirstSamplePosition()
	{
		return 0;
	}

	std::vector< NxSoundFrames *> NxSoundMediaFileBuffer::GetSampleDictionary()  
	{
		std::vector< NxSoundFrames *> Frames;
		unsigned long Position = GetFirstSamplePosition() ;

		for( int i = 0 ; i <  GetNumSamples() ; i++ ){
			NxSoundFrames Frame ;
			Frame.resize( SND_BUFFER_SIZE , mNumChannels  );
			//fileflac_->Read( Frame ,  Position , true  );
			Frames.push_back( &Frame );
			Position += SND_BUFFER_SIZE ;
		}
		return Frames ;
	}



	void NxSoundMediaFileBuffer::closeFile( void )
	{
		 
	}

	void NxSoundMediaFileBuffer::reset( void )
	{
		time_ = (StkFloat) 0.0;
		for ( unsigned int i=0; i<lastFrame_.size(); i++ ) lastFrame_[i] = 0.0;
		finished_ = false;
	}

	void NxSoundMediaFileBuffer::normalize( void )
	{
		this->normalize( 1.0 );
	}

	void NxSoundMediaFileBuffer::normalize( double peak )
	{
		// When chunking, the "normalization" scaling is performed by FileRead.
		if ( chunking_ ) return;

		size_t i;
		StkFloat max = 0.0;

		for ( i=0; i<data_.size(); i++ ){
			if ( fabs( dataframe[i] ) > max )
				max = (StkFloat) fabs((double) data_[i]);
		}

		if ( max > 0.0 ){
			max = 1.0 / max;
			max *= peak;
			for ( i=0; i<data_.size(); i++ )
				dataframe[i] *= max;
		}
	}

	void NxSoundMediaFileBuffer::SetRate( double rate )//STK
	{
		rate_ = rate;
		if ( fmod( rate_, 1.0 ) != 0.0 ) interpolate_ = true;
		else interpolate_ = false;
	}

	void NxSoundMediaFileBuffer::addTime( double time )
	{

	}


	double NxSoundMediaFileBuffer::tick( unsigned int channel = 0  )
	{
	
		return lastFrame_[channel];
	}

	StkFrames& NxSoundMediaFileBuffer::Update( StkFrames& Frame )
	{
		if(!Frame.size()) Frame.resize( SND_BUFFER_SIZE, GetNumChannels() );
		if( isMuted() || isStopped() ){
			for ( unsigned int i=0; i < Frame.size(); i++ ){ 
				Frame[i] = 0.0f;
			} 
			return Frame;
		}
		return this->tick( Frame );
	}

	StkFrames& NxSoundMediaFileBuffer::tick( StkFrames& frames )
	{
		 if( mBufferList.size() == 0 ) {
			for( unsigned int i=0; i< frames.frames() ; i++ ) {
				for( int c = 0 ; c < frames.channels()  ; c++ ){
					frames( i , c ) = 0.0f;
				} 
			}
			return frames;
		 }

		NxSoundFrames * popedframe = mBufferList.front();

		for( unsigned int i=0; i< frames.frames() ; i++ ) {
			for( int c = 0 ; c < frames.channels() ; c++ ) {
				frames( i , c ) = (*popedframe)( i , popedframe->channels() == 1 ? 0 : c );
			}
		}

	 
		delete popedframe;
		mBufferList.pop_front();
		return frames;
	}



}//namespace