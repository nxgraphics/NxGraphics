#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
#include "FileReadOgg.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaFileOgg::NxSoundMediaFileOgg() : NxSoundMediaFile()
{
	mType = TYPE_OGG ;
	mFileOgg = new FileReadOgg();
}

NxSoundMediaFileOgg::~NxSoundMediaFileOgg()
{
	if( mFileOgg->IsOpen()){  this->closeFile();  }
}

void NxSoundMediaFileOgg::SetResolution( NxAudioFormat Res )
{
	mDstResolution = Res;
	mFileOgg->SetOutputResolution( mDstResolution );
}  

unsigned long NxSoundMediaFileOgg::GetNumSamples()
{
	return mFileOgg->GetSampleNum();
}

bool NxSoundMediaFileOgg::OpenFile( std::string Filename, bool Normalize )
{
	try {
		this->openFile( Filename, false, true ) ;
	}
	catch( StkError &e ) {
		return false ;
	}

	mFilePath = Filename ;
	return true ;
}


void NxSoundMediaFileOgg::Play()
{
	mStopped = false ;
	mPaused = false ;
}

void NxSoundMediaFileOgg::Stop()
{
	mStopped = true ;
	mPaused = false ;
	this->reset();
}

void NxSoundMediaFileOgg::Pause()
{
	mStopped = true ;
	mPaused = true ;
}

void NxSoundMediaFileOgg::Rewind()
{

}

bool NxSoundMediaFileOgg::isStopped()
{
	return mStopped ;
}

bool NxSoundMediaFileOgg::isPaused()
{
	return mPaused ;
}

unsigned int NxSoundMediaFileOgg::GetFirstSamplePosition()
{
	return mFileOgg->GetFirstSamplePosition() ;
}

std::vector< NxSoundFrames *> NxSoundMediaFileOgg::GetSampleDictionary()  
{
	std::vector< NxSoundFrames *> Frames ;
	unsigned long Position = GetFirstSamplePosition() ;

	for( int i = 0 ; i <  GetNumSamples() ; i++ ) {
		NxSoundFrames Frame ;
		Frame.resize( SND_BUFFER_SIZE , mNumChannels  );
		mFileOgg->Read( Frame ,  Position , true );
		Frames.push_back( &Frame );
		Position += SND_BUFFER_SIZE ;
	}

	return Frames ;

}

void NxSoundMediaFileOgg::openFile( std::string Path, bool raw = false, bool doNormalize = true )
{
	this->closeFile();

	try
	{
		mFileOgg->Open( Path /* , raw */ );
	}
	catch(  StkError &e )
	{
		Log( "NxSound :  NxSoundMediaFileMp3::openFile Failed" );
		return ;
	}

	mFilePath = Path ;

	NxSoundUtils::SplitFileName( Path );
	mFileName = Path ;

	NxSoundUtils::SplitFileExtension( Path ); 
	mFileExtension = Path ;

	mNumChannels = mFileOgg->GetChannels() ;

	doNormalize = false ;
	chunkSize_ = 4096;//SND_BUFFER_SIZE; // + 1 ?
    chunking_ = true;

	//mResolution = 

    chunkPointer_ = mFileOgg->GetFirstSamplePosition() ;
    dataframe.resize( chunkSize_ , mFileOgg->GetChannels() );


	if ( doNormalize ){ normalizing_ = true;}
	else { normalizing_ = false;}

	// Load part of the data.
	mFileOgg->Read( dataframe, 0, doNormalize );

	if ( chunking_ ) { // If chunking, save the first sample frame for later.
	
		firstFrame_.resize( 1, dataframe.channels() );

		for ( unsigned int i=0; i<dataframe.channels(); i++ ) { 
			firstFrame_[i] = dataframe[i];
		}
	}
	else { // If not chunking, copy the first sample frame to the last.
	 
		for ( unsigned int i=0; i<dataframe.channels(); i++ ) {
			dataframe( dataframe.frames() - 1, i ) = dataframe[i];
		}
	}

	//Resize our lastOutputs container.
	lastFrame_.resize( 1, mFileOgg->GetChannels() );
  
	//Set default rate based on file sampling rate.
	this->SetRate( dataframe.dataRate() / Stk::sampleRate() );

	if ( doNormalize & !chunking_ ) this->normalize();

	this->reset();
}

void NxSoundMediaFileOgg::closeFile( void )
{
	mFileOgg->Close();
}

void NxSoundMediaFileOgg::reset( void )
{
	time_ = (StkFloat) 0.0;
	for ( unsigned int i=0; i<lastFrame_.size(); i++ ) lastFrame_[i] = 0.0;
	finished_ = false;
}

void NxSoundMediaFileOgg::normalize( void )
{
	this->normalize( 1.0 );
}

void NxSoundMediaFileOgg::normalize( StkFloat peak )
{
  // When chunking, the "normalization" scaling is performed by FileRead.
  if ( chunking_ ) return;

  size_t i;
  StkFloat max = 0.0;

  for ( i=0; i<data_.size(); i++ )  {
    if ( fabs( dataframe[i] ) > max )
      max = (StkFloat) fabs((double) data_[i]);
  }

  if ( max > 0.0 ) {
    max = 1.0 / max;
    max *= peak;
    for ( i=0; i<data_.size(); i++ )
      dataframe[i] *= max;
  }
}
 
void NxSoundMediaFileOgg::SetRate( double rate )//STK
{
	rate_ = rate;
	if ( fmod( rate_, 1.0 ) != 0.0 ) interpolate_ = true;
	else interpolate_ = false;
}

void NxSoundMediaFileOgg::addTime( double time )
{
  // Add an absolute time in samples.
  time_ += time;

  StkFloat fileSize = mFileOgg->GetFileSize();
  while ( time_ < 0.0 )
    time_ += fileSize;
  while ( time_ >= fileSize )
    time_ -= fileSize;
}

double NxSoundMediaFileOgg::tick( unsigned int channel = 0  )
{

  StkFloat fileSize = mFileOgg->GetFileSize();
 
  while ( time_ < 0.0 )
    time_ += fileSize;
  while ( time_ >= fileSize )
    time_ -= fileSize;

  StkFloat tyme = time_;
  if ( phaseOffset_ )
  {
    tyme += phaseOffset_;
    while ( tyme < 0.0 )
      tyme += fileSize;
    while ( tyme >= fileSize )
      tyme -= fileSize;
  }

  if ( chunking_ ) 
  {

    // Check the time address vs. our current buffer limits.
    if ( ( time_ < (StkFloat) chunkPointer_ ) ||( time_ > (StkFloat) ( chunkPointer_ + chunkSize_ - 1 ) ) ) 
	{

      while ( time_ < (StkFloat) chunkPointer_ )// negative rate
	  { 
        chunkPointer_ -= chunkSize_ - 1; // overlap chunks by one frame
        if ( chunkPointer_ < 0 ) chunkPointer_ = 0;
      }

      while ( time_ > (StkFloat) ( chunkPointer_ + chunkSize_ - 1 ) )  // positive rate
	  {
        chunkPointer_ += chunkSize_ ;//- 1; // overlap chunks by one frame
        if ( chunkPointer_ + chunkSize_ > fileSize ) // at end of file
		{ 
          chunkPointer_ = fileSize - chunkSize_ + 1; // leave extra frame at end of buffer
          // Now fill extra frame with first frame data.
		  for ( unsigned int j=0; j<firstFrame_.channels(); j++ ) { 
			  dataframe( dataframe.frames() - 1, j ) = firstFrame_[j];
		  }
        }
      }

      // Load more data.	  
      mFileOgg->Read( dataframe , chunkPointer_, normalizing_ );
    }

    // Adjust index for the current buffer.
    tyme -= chunkPointer_;
  }

	if( interpolate_ ) {
		for ( unsigned int i=0; i<lastFrame_.size(); i++ )
			lastFrame_[i] = dataframe.interpolate( tyme, i );
	}
	else {
		for ( unsigned int i=0; i<lastFrame_.size(); i++ ) {
			lastFrame_[i] = dataframe( (size_t) tyme, i );
		}
	}

	//Increment time, which can be negative.
	time_ += rate_  ;

	return lastFrame_[channel];
}

StkFrames& NxSoundMediaFileOgg::Update( StkFrames& Frame  )
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

StkFrames& NxSoundMediaFileOgg::tick( StkFrames& frames )
{
	if( !mFileOgg->IsOpen() ) {
		return frames;
	}

	unsigned int nChannels = lastFrame_.channels();

	if( mStopped ) {
		chunkPointer_ = GetFirstSamplePosition() ;
		return frames ;
	}

	unsigned int j, counter = 0;
	for ( unsigned int i=0; i<frames.frames(); i++ ) {
		this->tick();
		for ( j=0; j<nChannels; j++ ) {
			frames(i,j)= lastFrame_[j];
		}
	}
 
	return frames;
}



}//namespace