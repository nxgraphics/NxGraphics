#include "NxSoundPreCompiled.h"
#include "FileReadWav.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaFileWav::NxSoundMediaFileWav() : NxSoundMediaFile()
{
	mType = TYPE_WAV ;
	mSoundFile = new FileReadWav();
}

NxSoundMediaFileWav::~NxSoundMediaFileWav()
{
	if( mSoundFile ) { 
		mSoundFile->closeFile();
		delete mSoundFile;
		mSoundFile = NULL; 
	}
}

bool NxSoundMediaFileWav::OpenFile( std::string Path, bool Normalize )
{
	Log("Opening Wav File: " + Path );
	mSoundFile->openFile( Path, false, Normalize );
	mFilePath = Path ;
	NxSoundUtils::SplitFileName( Path );
	mFileName = Path;
	NxSoundUtils::SplitFileExtension( Path ); 
	mFileExtension = Path;
	mNumChannels = mSoundFile->channelsOut();
	Log( "NxSound : opening audio file : " + mFileName + ", Extension : " + mFileExtension );
	//mResolution = mSoundFile->GetResolution();

	return true ;
}

void NxSoundMediaFileWav::SetResolution( NxAudioFormat Res )
{
	mDstResolution = Res;
	mSoundFile->SetOutputResolution( mDstResolution );
}  

void NxSoundMediaFileWav::SetLooping( bool Loop )
{
	mLoop = Loop;
	mSoundFile->SetLooping( Loop );
}

stk::StkFrames& NxSoundMediaFileWav::Update( StkFrames& Frames )
{
	if(!Frames.size()) Frames.resize( SND_BUFFER_SIZE, GetNumChannels() );


	if( isMuted() || isStopped() || mSoundFile->IsFinished() ){
		for ( unsigned int i=0; i < Frames.size(); i++ ){ 
			Frames[i] = 0.0f;
		} 
		return Frames;
	}

	//if( mSoundFile->IsFinished())
	//{
	//	Log("IS FINISHED !!!!");
	//}

	return mSoundFile->Tick( Frames );
}

void NxSoundMediaFileWav::Stop()
{
	mStopped = true;
	mPaused = false;
	mPlaying = false;
}

void NxSoundMediaFileWav::Play()
{
	mStopped = false;
	mPaused = false;
	mPlaying = true;
	if( mSoundFile ){ mSoundFile->reset(); }
}

void NxSoundMediaFileWav::Rewind()
{
	mSoundFile->reset();
}

bool NxSoundMediaFileWav::IsPlaying()
{
	return mPlaying;
}

unsigned long NxSoundMediaFileWav::GetNumSamples()
{
	return mSoundFile->getSize();
}

unsigned int NxSoundMediaFileWav::GetNumChannels()
{
	return mNumChannels;
}

void NxSoundMediaFileWav::SetRate( double Rate )
{
	rate_ = Rate ;
	mSoundFile->setRate( Rate );
}

void NxSoundMediaFileWav::SetPosition( unsigned long position, NXSOUNDTIMEUNIT postype )
{
	if( postype == NX_TIMEUNIT_MS ) {/* Milliseconds. */
		mSoundFile->SetPosition( ( (double) ( position * mSoundFile->GetFrequency() ) / 1000.0 ));
	}
	else if( postype == NX_TIMEUNIT_S ) {
		mSoundFile->SetPosition( ( (double) ( position * mSoundFile->GetFrequency() ) ));
	}
}

double NxSoundMediaFileWav::GetPosition( NXSOUNDTIMEUNIT postype )
{
	if( postype == NX_TIMEUNIT_MS ) { /* Milliseconds. */
		return ( mSoundFile->GetPosition() / mSoundFile->GetFrequency() ) * 1000.0f; // samples Stk::sampleRate()  
	}
	else if( postype == NX_TIMEUNIT_S ) {
		return ( mSoundFile->GetPosition() / mSoundFile->GetFrequency() ); // samples Stk::sampleRate()  
	}
	else if( postype == NX_TIMEUNIT_PCM ) { /* PCM samples, related to milliseconds * samplerate / 1000. */
		return mSoundFile->GetPosition();
	}
	else if( postype == NX_TIMEUNIT_PCMBYTES ) { /* Bytes, related to PCM samples * channels * datawidth (ie 16bit = 2 bytes). */
		return mSoundFile->GetPosition() * SND_BUFFER_SIZE ; // get channels
	}
}

unsigned long NxSoundMediaFileWav::GetTotalDuration( NXSOUNDTIMEUNIT postype )
{
	if( postype == NX_TIMEUNIT_MS ) { /* Milliseconds. */ 
		return ( unsigned long ) mSoundFile->GetFileSize()  / mSoundFile->GetFrequency() * 1000 ; // samples Stk::sampleRate()  //89077
	}
	else if( postype == NX_TIMEUNIT_S ){
		return ( mSoundFile->GetFileSize() / mSoundFile->GetFrequency() ); // samples Stk::sampleRate()  
	}
	else if( postype == NX_TIMEUNIT_PCM ){/* PCM samples, related to milliseconds * samplerate / 1000. */
		return mSoundFile->GetFileSize();
	}
	else if( postype == NX_TIMEUNIT_PCMBYTES ){/* Bytes, related to PCM samples * channels * datawidth (ie 16bit = 2 bytes). */
		return mSoundFile->GetFileSize() * SND_BUFFER_SIZE ; // get channels
	}
}


}//namespace