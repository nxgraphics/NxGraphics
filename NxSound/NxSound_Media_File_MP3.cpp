#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
#include "FileReadMp3.h"
 
using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaFileMp3::NxSoundMediaFileMp3() : NxSoundMediaFile()
{
	mType = TYPE_MP3 ;
	filemp3_ = new FileReadMp3();
}

NxSoundMediaFileMp3::~NxSoundMediaFileMp3()
{
	if( filemp3_->IsOpen()) {  
		this->closeFile();
	}
}

void NxSoundMediaFileMp3::SetResolution( NxAudioFormat Res )
{
	mDstResolution = Res;
	filemp3_->SetOutputResolution( mDstResolution );
}  

unsigned long NxSoundMediaFileMp3::GetNumSamples()
{
	return filemp3_->GetSampleNum();
}

bool NxSoundMediaFileMp3::OpenFile( std::string Filename, bool Normalize )
{
	try {
		this->openFile( Filename , false,  true ) ;
	}
	catch( StkError &e  ) {
		return false ;
	}
	return true ;
}

void NxSoundMediaFileMp3::openFile( std::string Path, bool raw, bool doNormalize )
{
	this->closeFile();
	Log( "NxSound : openFile mp3 on path : " +  Path );

	try {
		filemp3_->Open( Path );
	}catch(  StkError &e )
	{
		Log( "NxSound : NxSoundMediaFileMp3::openFile Failed" );
		return ;
	}

	mFilePath = Path ;

	NxSoundUtils::SplitFileName( Path );
	mFileName = Path ;

	NxSoundUtils::SplitFileExtension( Path ); 
	mFileExtension = Path ;

	mNumChannels = filemp3_->GetChannels() ;

	chunkSize_ = 1152;
	chunking_ = true;
	chunkPointer_ = filemp3_->GetFirstSamplePosition() ;
	dataframe.resize( 1152, filemp3_->GetChannels() );
	normalizing_ = doNormalize;

	// Load all or part of the data.
	filemp3_->Read( dataframe , filemp3_->GetFirstSamplePosition(), doNormalize );

	if ( chunking_ ) // If chunking, save the first sample frame for later.
	{ 
		firstFrame_.resize( 1, dataframe.channels() );
		for ( unsigned int i=0; i<dataframe.channels(); i++ ){ firstFrame_[i] = dataframe[i]; }
	}
	else // If not chunking, copy the first sample frame to the last.
	{  
		for ( unsigned int i=0; i<dataframe.channels(); i++ ){dataframe( dataframe.frames() - 1, i ) = dataframe[i];}
	}

	//Resize our lastOutputs container.
	lastFrame_.resize( 1, filemp3_->GetChannels() );

	//Set default rate based on file sampling rate.
	this->SetRate( dataframe.dataRate() / Stk::sampleRate() );

	if ( doNormalize & !chunking_ ) this->normalize();

	this->reset();

}


void NxSoundMediaFileMp3::Play()
{
	mStopped = false ;
	mPaused = false ;
}

void NxSoundMediaFileMp3::Stop()
{
	mStopped = true ;
	mPaused = false ;
	this->reset();
}

void NxSoundMediaFileMp3::Pause()
{
	mStopped = true ;
	mPaused = true ;
}

void NxSoundMediaFileMp3::Rewind()
{

}

bool NxSoundMediaFileMp3::isStopped()
{
	return mStopped ;
}

bool NxSoundMediaFileMp3::isPaused()
{
	return mPaused ;
}

unsigned int NxSoundMediaFileMp3::GetFirstSamplePosition()
{
	return filemp3_->GetFirstSamplePosition() ;
}

std::vector< NxSoundFrames *> NxSoundMediaFileMp3::GetSampleDictionary()  
{
	std::vector< NxSoundFrames *> Frames ;
	unsigned long Position = GetFirstSamplePosition() ;

	for( int i = 0 ; i <  GetNumSamples() ; i++   )
	{
		NxSoundFrames Frame ;
		Frame.resize( SND_BUFFER_SIZE, mNumChannels  );

		filemp3_->Read( Frame, Position , true  );
		Frames.push_back( &Frame );
		Position += SND_BUFFER_SIZE ;
	}

	return Frames ;

}



void NxSoundMediaFileMp3::closeFile( void )
{
	filemp3_->Close();
}

void NxSoundMediaFileMp3::reset( void )
{
	time_ = (StkFloat) 0.0;
	for ( unsigned int i=0; i<lastFrame_.size(); i++ ) lastFrame_[i] = 0.0;
	finished_ = false;
}

void NxSoundMediaFileMp3::normalize( void )
{
	this->normalize( 1.0 );
}

void NxSoundMediaFileMp3::normalize( double peak )
{
  // When chunking, the "normalization" scaling is performed by FileRead.
  if ( chunking_ ) return;

  size_t i;
  StkFloat max = 0.0;

  for ( i=0; i<data_.size(); i++ ) 
  {
    if ( fabs( dataframe[i] ) > max )
      max = (StkFloat) fabs((double) data_[i]);
  }

  if ( max > 0.0 )
  {
    max = 1.0 / max;
    max *= peak;
    for ( i=0; i<data_.size(); i++ )
      dataframe[i] *= max;
  }

}

void NxSoundMediaFileMp3::SetRate( double rate )//STK
{
	rate_ = rate;
	if ( fmod( rate_, 1.0 ) != 0.0 ) interpolate_ = true;
	else interpolate_ = false;
}

void NxSoundMediaFileMp3::addTime( double time )
{
  // Add an absolute time in samples.
  time_ += time;

  StkFloat fileSize = filemp3_->GetFileSize();
  while ( time_ < 0.0 )
    time_ += fileSize;
  while ( time_ >= fileSize )
    time_ -= fileSize;
}

StkFloat NxSoundMediaFileMp3::tick( unsigned int channel = 0  )
{
  StkFloat fileSize = filemp3_->GetFileSize();
 
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
    if ( ( time_ < (StkFloat) chunkPointer_ ) || ( time_ > (StkFloat) ( chunkPointer_ + chunkSize_ - 1 ) ) ) 
	{

      while ( time_ < (StkFloat) chunkPointer_ )// negative rate
	  { 
        chunkPointer_ -= chunkSize_ ;//- 1; // overlap chunks by one frame
        if ( chunkPointer_ < 0 ) chunkPointer_ = 0;
      }

      while ( time_ > (StkFloat) ( chunkPointer_ + chunkSize_ - 1 ) )  // positive rate
	  {
        chunkPointer_ += chunkSize_  ;

        if ( chunkPointer_ + chunkSize_ > fileSize ) // at end of file
		{ 
          chunkPointer_ = fileSize - chunkSize_ + 1; // leave extra frame at end of buffer
          // Now fill extra frame with first frame data.
		  for ( unsigned int j=0; j<firstFrame_.channels(); j++ ){ dataframe( dataframe.frames() - 1, j ) = firstFrame_[j]; }
        }
      }

      // Load more data.	  
      filemp3_->Read( dataframe , chunkPointer_ , normalizing_ );
	  
	} 

    // Adjust index for the current buffer.
    tyme -= chunkPointer_;
  }

  if ( interpolate_ )
  {
    for ( unsigned int i=0; i<lastFrame_.size(); i++ )
      lastFrame_[i] = dataframe.interpolate( tyme, i );
  }
  else
  {
    for ( unsigned int i=0; i<lastFrame_.size(); i++ )
      lastFrame_[i] = dataframe( (size_t) tyme, i );
  }

	//Increment time, which can be negative.
	time_ += rate_  ;

	return lastFrame_[channel];
}

StkFrames& NxSoundMediaFileMp3::Update( StkFrames& Frame  )
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

StkFrames& NxSoundMediaFileMp3::tick( StkFrames& frames )
{
	if ( !filemp3_->IsOpen() )
	{
		return frames;
	}

	if( mStopped  )
	{
		chunkPointer_ = GetFirstSamplePosition() ;
		return frames ;
	}

	unsigned int j, counter = 0;
	for( unsigned int i=0; i< frames.frames() ; i++ ) 
	{
		this->tick();
		for ( j=0; j<lastFrame_.channels(); j++ )
		{
			frames[counter++] = lastFrame_[j];
		}
	}

	return frames;
}



}//namespace