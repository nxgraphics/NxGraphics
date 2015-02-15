#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
//#include "VST/public.sdk/source/vst2.x/audioeffectx.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaFile::NxSoundMediaFile() : mVolume(1.0f), mStopped(true),mPaused(false), mMuted(false), mLoop(false), mPlaying(false), mDstResolution(NX_SINT8)
{
	Log( "Creating Buffers for Media File...");
	//CreateEffectBuffers(); // only for vst ??
}

NxSoundMediaFile::~NxSoundMediaFile()
{

}

bool NxSoundMediaFile::OpenFile( std::string Path, bool Normalize )
{
	return true ;
}

void NxSoundMediaFile::Play()
{
	mStopped = false ;
	mPaused = false ;
	mPlaying = true;
}

void NxSoundMediaFile::Stop()
{
	mStopped = true ;
	mPaused = false ;
	mPlaying = false ;
}

void NxSoundMediaFile::Pause()
{
	mStopped = true ;
	mPaused = true ;
}

bool NxSoundMediaFile::isStopped()
{
	return mStopped ;
}

bool NxSoundMediaFile::isPaused()
{
	return mPaused ;
}

bool NxSoundMediaFile::IsPlaying()
{
	return mPlaying;
}

bool NxSoundMediaFile::IsStereo()
{
	if( mNumChannels == 2 ){ return true ;}
	return false;
}
bool NxSoundMediaFile::IsMono()
{
	if( mNumChannels == 1 ){ return true ;}
	return false;
}
	 
void NxSoundMediaFile::SetLooping( bool Loop )
{
	mLoop = Loop;
}

bool NxSoundMediaFile::GetLooping()
{
	return mLoop;
}

const std::string& NxSoundMediaFile::GetFilePath()
{
	return mFilePath ;
}

const std::string& NxSoundMediaFile::GetFileName()
{
	return mFileName ;
}

const std::string& NxSoundMediaFile::GetFileExtension()
{
	return mFileExtension ;
}

unsigned long NxSoundMediaFile::GetNumSamples()
{
	return 0;
}

float NxSoundMediaFile::GetRate()
{
	return rate_; 
}

void NxSoundMediaFile::SetVolume( float Volume )
{
	if( Volume < 0.0f ) Volume = 0.0f;
	if( Volume > 1.0f ) Volume = 1.0f;
	mVolume = Volume ;
}

float NxSoundMediaFile::GetVolume()
{
	return mVolume ;
}

void NxSoundMediaFile::SetPan( float Pan ){
	mPan = Pan ;
}

float NxSoundMediaFile::GetPan(){
	return mPan ;
}

void NxSoundMediaFile::SetMute( bool Mute ){
	mMuted = Mute ;
}

bool NxSoundMediaFile::isMuted(){
	return mMuted ;
}

NxAudioFormat NxSoundMediaFile::GetResolution()
{
	return mDstResolution;
}

void NxSoundMediaFile::SetPosition( unsigned long position, NXSOUNDTIMEUNIT postype )
{

}

double NxSoundMediaFile::GetPosition( NXSOUNDTIMEUNIT postype )
{
	return 0;
}

unsigned long NxSoundMediaFile::GetTotalDuration( NXSOUNDTIMEUNIT postype )
{

	return 0;
}

unsigned long NxSoundMediaFile::GetLengthSeconds()
{
	return 0;
}

unsigned long  NxSoundMediaFile::GetLengthMilliSeconds()
{
	return 0;
}

NxSoundMediaType NxSoundMediaFile::GetMediaType()
{
	return mType;
}

unsigned int NxSoundMediaFile::GetNumChannels()
{
	return mNumChannels ;
}

unsigned int NxSoundMediaFile::GetFirstSamplePosition()
{
	return 0 ;
}

std::vector< NxSoundFrames *> NxSoundMediaFile::GetSampleDictionary(){
	std::vector< NxSoundFrames *> Frames ;//to do
	return Frames ;
}

bool NxSoundMediaFile::AddAudioPlugin( NxSoundEffect * Effect ){
	if(!Effect) return false ;
	Effect->SetNumChannels( GetNumChannels() );
	mAudioPluginList.push_back(Effect);
	return true ;
}

NxSoundEffect * NxSoundMediaFile::GetAudioPlugin( int index ){
	if( !mAudioPluginList.size() ) return 0 ;
	std::list<NxSoundEffect *>::iterator i = mAudioPluginList.begin();
	std::advance(i, index );
	// now index points to the nth member of the list (counting from zero)
	return *i ;
}

bool NxSoundMediaFile::RemoveAudioPlugin( NxSoundEffect * Effect ){
	if(!Effect) return false ;
	mAudioPluginList.remove(Effect);
	return true ;
}

void NxSoundMediaFile::CreateEffectBuffers() {

	int numInputs = 2 ;
	int numOutputs = 2 ;
	int kBlockSize = SND_BUFFER_SIZE ;
	
	inBufs = new float*[numInputs];
	for ( int f = 0; f < numInputs; f++){
		inBufs[f] = new float[kBlockSize];
		memset( inBufs[f] , 0 ,  kBlockSize * sizeof(float) );
	}

	outBufs = new float*[numOutputs];
	for ( int i = 0; i < numOutputs; i++){
		outBufs[i] = new float[kBlockSize];
		memset (outBufs[i], 0, kBlockSize * sizeof (float));
	}
}

void NxSoundMediaFile::CopyIn( StkFrames & Frame )
{
	//interleaved to non interleaved
	for (int f = 0; f < 2  ; f++){
		for ( unsigned int i=0; i< SND_BUFFER_SIZE  ; i++ )
		{  inBufs[f][i] = Frame(i , f); } 
	}
}

void NxSoundMediaFile::CopyOut( StkFrames & Frame )
{
	//non interleaved to interleaved
	for(int f = 0 ; f < GetNumChannels() ; f++){
		for ( unsigned int i=0; i< SND_BUFFER_SIZE ; i++ ){
			float  * ptr = outBufs[f] ;
			Frame(i,f) = ptr[i]  ;
		} 
	}
}

void NxSoundMediaFile::CopyOutUntouched( StkFrames & Frame )
{
	//non interleaved to interleaved
	for(int f = 0 ; f < 2 ; f++){
		for ( unsigned int i=0; i< SND_BUFFER_SIZE ; i++ ){
			float  * ptr = inBufs[f];
			Frame(i,f) = ptr[i];
		} 
	}
}

void NxSoundMediaFile::ProcessEffectPlugins( StkFrames & Frame  )
{
	for( std::list< NxSoundEffect * >::iterator iter = mAudioPluginList.begin(); iter != mAudioPluginList.end(); iter++){
		NxSoundEffect *  Plugin = *iter; 
		if( Plugin->GetType() == Effect_VST_Effect ){
			CopyIn( Frame );
			Plugin->Process( inBufs, outBufs, SND_BUFFER_SIZE);
			CopyOut( Frame );
		}
		else if( Plugin->GetType() == Effect_OggBroadCast ){ //ogg file streamer 
		
			NxSoundEffectOggBroadCaster * OggBroadCastPlug = ( NxSoundEffectOggBroadCaster * ) Plugin ;
			CopyIn( Frame ) ;
			OggBroadCastPlug->Process( inBufs , outBufs , SND_BUFFER_SIZE );
			CopyOut( Frame );
		}
		else if(  Plugin->GetType() ==  Effect_PitShift ){
			NxSoundEffectPitShift * Shiftplug = ( NxSoundEffectPitShift * ) Plugin ;
			CopyIn( Frame ) ;
			Shiftplug->Process( inBufs , outBufs , SND_BUFFER_SIZE  );
			CopyOut( Frame );
		}
		else if(  Plugin->GetType() == Effect_Granulate ){
			NxSoundEffectPitShift * Shiftplug = ( NxSoundEffectPitShift * ) Plugin ;
			CopyIn( Frame ) ;
			Shiftplug->Process( inBufs , outBufs , SND_BUFFER_SIZE  );
			CopyOut( Frame );
		}
		else if(  Plugin->GetType() == Effect_Chorus ){
			NxSoundEffectChorus * Shiftplug = ( NxSoundEffectChorus * ) Plugin ;
			CopyIn( Frame ) ;
			Shiftplug->Process( inBufs , outBufs , SND_BUFFER_SIZE  );
			CopyOut( Frame );
		}
		else if(  Plugin->GetType() == Effect_Echo ){
			NxSoundEffectChorus * Echoplug = ( NxSoundEffectChorus * ) Plugin ;
			CopyIn( Frame ) ;
			Echoplug->Process( inBufs , outBufs , SND_BUFFER_SIZE  );
			CopyOut( Frame );
		}
		else if( Plugin->GetType() == Effect_3D ){
			NxSoundEffect3D * Spatialplug = ( NxSoundEffect3D * ) Plugin ;
			CopyIn( Frame ) ;

			Spatialplug->Process( inBufs , outBufs , SND_BUFFER_SIZE  );
			//CopyOut( Frame );
			
			//fill blank out
			for(int f = 0 ; f < 2 ; f++ ){
				for ( unsigned int i=0; i< SND_BUFFER_SIZE ; i++ ){ 
					Frame(i,f) = 0.0f ;
				} 
			}
		}
		else if(  Plugin->GetType() == Effect_Beat ){
			NxSoundEffectBeatDetector * Beatplug = ( NxSoundEffectBeatDetector * ) Plugin ;
			CopyIn( Frame ) ;
			Beatplug->Process( inBufs, outBufs, SND_BUFFER_SIZE );
			CopyOutUntouched( Frame );
		}

}//for



}



}//namespace