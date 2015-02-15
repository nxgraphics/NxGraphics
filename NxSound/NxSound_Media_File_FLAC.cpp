#include "NxSoundPreCompiled.h"
#include "FileReadFlac.h"
#include "STK/include/Stk.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaFileFlac::NxSoundMediaFileFlac() 
{
	mType = TYPE_FLAC ;
	fileflac_ = new FileReadFlac();
}

NxSoundMediaFileFlac::~NxSoundMediaFileFlac()
{
	if( fileflac_->IsOpen()){  this->closeFile();  }
}


//NxAudioFormat NxSoundMediaFileFlac::GetResolution()
//{
//	return NX_FLOAT32;//mResolution;
//}

unsigned long NxSoundMediaFileFlac::GetNumSamples()
{
	return fileflac_->GetSampleNum();
}

void NxSoundMediaFileFlac::Rewind()
{
	time_ = (StkFloat) 0.0;
}

void NxSoundMediaFileFlac::SetResolution( NxAudioFormat Res )
{
	mDstResolution = Res;
	fileflac_->SetOutputResolution( mDstResolution );
}  

bool NxSoundMediaFileFlac::OpenFile( std::string Filename, bool Normalize )
{
	this->closeFile();

	try{
		fileflac_->Open( Filename );
	}
	catch(  StkError &e ){
		Log( "NxSound : NxSoundMediaFileFlac::openFile Failed" );
		return false;
	}

	mFilePath = Filename ;

	NxSoundUtils::SplitFileName( Filename );
	mFileName = Filename ;
	NxSoundUtils::SplitFileExtension( Filename ); 
	mFileExtension = Filename ;

	mNumChannels = fileflac_->GetChannels() ;

	//doNormalize = false ;
	chunkSize_ = fileflac_->GetBlockSize();
	chunking_ = true;
	chunkPointer_ = fileflac_->GetFirstSamplePosition() ;
	dataframe.resize( chunkSize_ + 1 , fileflac_->GetChannels() );
	normalizing_ = Normalize; 

	// Load all or part of the data.
	fileflac_->Read( dataframe , fileflac_->GetFirstSamplePosition(), Normalize );

	if ( chunking_ ) // If chunking, save the first sample frame for later.
	{ 
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
	lastFrame_.resize( 1, fileflac_->GetChannels() );

	//Set default rate based on file sampling rate.
	this->SetRate( dataframe.dataRate() / Stk::sampleRate() );

	if ( Normalize & !chunking_ ) this->normalize();

	this->reset();

	return true;
}

void NxSoundMediaFileFlac::Play()
{
	mStopped = false ;
	mPaused = false ;
}

void NxSoundMediaFileFlac::Stop()
{
	mStopped = true ;
	mPaused = false ;
	this->reset();
}

void NxSoundMediaFileFlac::Pause()
{
	mStopped = true ;
	mPaused = true ;
}

bool NxSoundMediaFileFlac::isStopped()
{
	return mStopped ;
}

bool NxSoundMediaFileFlac::isPaused()
{
	return mPaused ;
}

unsigned int NxSoundMediaFileFlac::GetFirstSamplePosition()
{
	return fileflac_->GetFirstSamplePosition() ;
}

std::vector< NxSoundFrames *> NxSoundMediaFileFlac::GetSampleDictionary()  
{
	std::vector< NxSoundFrames *> Frames;
	unsigned long Position = GetFirstSamplePosition() ;

	for( int i = 0 ; i <  GetNumSamples() ; i++ ){
		NxSoundFrames Frame ;
		Frame.resize( SND_BUFFER_SIZE , mNumChannels  );
		fileflac_->Read( Frame ,  Position , true  );
		Frames.push_back( &Frame );
		Position += SND_BUFFER_SIZE ;
	}
	return Frames ;
}



void NxSoundMediaFileFlac::closeFile( void )
{
	fileflac_->Close();
}

void NxSoundMediaFileFlac::reset( void )
{
	time_ = (StkFloat) 0.0;
	for ( unsigned int i=0; i<lastFrame_.size(); i++ ) lastFrame_[i] = 0.0;
	finished_ = false;
}

void NxSoundMediaFileFlac::normalize( void )
{
	this->normalize( 1.0 );
}

void NxSoundMediaFileFlac::normalize( double peak )
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

void NxSoundMediaFileFlac::SetRate( double rate )//STK
{
	rate_ = rate;
	if ( fmod( rate_, 1.0 ) != 0.0 ) interpolate_ = true;
	else interpolate_ = false;
}

void NxSoundMediaFileFlac::addTime( double time )
{
  // Add an absolute time in samples.
  time_ += time;

  StkFloat fileSize = fileflac_->GetFileSize();
  while ( time_ < 0.0 )
    time_ += fileSize;
  while ( time_ >= fileSize )
    time_ -= fileSize;
}


double NxSoundMediaFileFlac::tick( unsigned int channel = 0  )
{
  // Check limits of time address ... if necessary, recalculate modulo fileSize.

 

  StkFloat fileSize = fileflac_->GetFileSize();
 
  while ( time_ < 0.0 )
    time_ += fileSize;
  while ( time_ >= fileSize )
    time_ -= fileSize;

  StkFloat tyme = time_;

  if ( phaseOffset_ ) {
    tyme += phaseOffset_;
    while ( tyme < 0.0 )
      tyme += fileSize;
    while ( tyme >= fileSize )
      tyme -= fileSize;
  }
 
  if ( chunking_ )  {

    // Check the time address vs. our current buffer limits.
    if ( ( time_ < (StkFloat) chunkPointer_ ) ||( time_ > (StkFloat) ( chunkPointer_ + chunkSize_ - 1 ) ) )  {

      while ( time_ < (StkFloat) chunkPointer_ ) {// negative rate
        chunkPointer_ -= chunkSize_ - 1; // overlap chunks by one frame
        if ( chunkPointer_ < 0 ) chunkPointer_ = 0;
      }

      while ( time_ > (StkFloat) ( chunkPointer_ + chunkSize_ - 1 ) ) { // positive rate
        chunkPointer_ += chunkSize_ ;//- 1; // overlap chunks by one frame

        if ( chunkPointer_ + chunkSize_ > fileSize ) { // at end of file

		 

          chunkPointer_ = fileSize - chunkSize_ + 1; // leave extra frame at end of buffer

		   for ( unsigned int j=0; j<firstFrame_.channels(); j++ ){ dataframe( dataframe.frames() - 1, j ) = firstFrame_[j]; } // Now fill extra frame with first frame data.
        }
      }

      // Load more data.	  
      fileflac_->Read( dataframe , chunkPointer_, normalizing_ );
	  //NxSoundLog::getSingleton().LogMessage( "NxSound :  current frame " + Ogre::StringConverter::toString(  filemp3_.CurreFrame ) );
	  
    }

    // Adjust index for the current buffer.
    tyme -= chunkPointer_;
  }

 
  

  if ( interpolate_ ) {
    for ( unsigned int i=0; i<lastFrame_.size(); i++ )
      lastFrame_[i] = dataframe.interpolate( tyme, i );
  }
  else {
    for ( unsigned int i=0; i<lastFrame_.size(); i++ )
      lastFrame_[i] = dataframe( (size_t) tyme, i );
  }

	//Increment time, which can be negative.
	time_ += rate_  ;

	return lastFrame_[channel];
}
 
StkFrames& NxSoundMediaFileFlac::Update( StkFrames& Frame  )
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

StkFrames& NxSoundMediaFileFlac::tick( StkFrames& frames )
{
	unsigned int nChannels = lastFrame_.channels();

	if( mStopped  )
	{
		chunkPointer_ =  GetFirstSamplePosition() ;
		return frames ;
	}

	for ( unsigned int i=0; i< frames.frames() ; i++ )
	{
		this->tick();
		frames( i , 0 ) = lastFrame_[0];
		frames( i , 1 ) = lastFrame_[1];
	} 

  return frames;
}



}//namespace