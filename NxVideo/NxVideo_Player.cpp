#include "NxVideoPreCompiled.h"

namespace NxVideo_Namespace {

NxVideo_Player::NxVideo_Player() : mManualUpdate( false ), mVideoFrameRate( 25 ), mFramePrevious( 0 ) // mVideoLoop( true )
{
	mVideoPlayType = VIDEO_PLAYBACK_NORMAL;
	mVideoPlayState = VIDEO_STATUS_STOP;
	mFrame = 1;
	mVideoCodecName = "None";
	mNewFrame = true;
	mVideoSwitchBounce = true;
	mVideoData = NULL;

	mVideoWidth[0] = mVideoWidth[1] = mVideoWidth[2] = 0; 
	mVideoHeight[0] = mVideoHeight[1] = mVideoHeight[2] = 0; 
	mOpengGlID[0] = mOpengGlID[1] = mOpengGlID[2] = 0;

	mHasAudio = false;
	mHasVideo = false;
	mAudioBufferSize = 0;
	mAudioNumChannels = 0;
	mAudioSampleRate = 0;

}

bool NxVideo_Player::IsPlanar() {
	bool isPlanar = false;
	std::vector<NxVideoPixelFormat> mFormatList;
	mFormatList.push_back(NxVideoPixelFormaty420);
	mFormatList.push_back(NxVideoPixelFormaty422);
	mFormatList.push_back(NxVideoPixelFormaty444);
	for( int i = 0 ; i < mFormatList.size(); i++ ){
		if( mVideoFormat == mFormatList[i] ){
			isPlanar = true; break;
		}
	}
	return isPlanar;

}
NxVideoPixelFormat NxVideo_Player::GetVideoFormat(){
	return mVideoFormat;
}

unsigned short NxVideo_Player::GetVideoBppFromPixelFormat( NxVideoPixelFormat format ) {

	unsigned short bpp = 2;
	switch( format ) {

	case NxVideoPixelFormat32ARGB : 
	case NxVideoPixelFormat32ABGR :
	case NxVideoPixelFormat32BGRA :
	case NxVideoPixelFormat32RGBA :
		bpp = 4;
		break;
	case NxVideoPixelFormat24RGB :
	case NxVideoPixelFormat24BGR : 
		bpp = 3;
		break;
		//case NxVideoPixelFormatYUVS : 
		//case NxVideoPixelFormatYUVU :
		//case NxVideoPixelFormatYVYU422 : 
		//case NxVideoPixelFormatUYVY422 :
	case NxVideoPixelFormaty420 :
	case NxVideoPixelFormat2vuy :
		bpp = 2;
		break;
	default :
		Log("Error : GetVideoBppFromPixelFormat, bpp not defined !");
		break;

	}
	return bpp;
}

void NxVideo_Player::SetOpenGLID( int plane, unsigned int ID ) {
	mOpengGlID[plane] = ID;
}

unsigned int NxVideo_Player::GetOpenGLID( int plane ) {
	return mOpengGlID[plane];
}

bool NxVideo_Player::HasAudio() {
	return mHasAudio;
}
 
bool NxVideo_Player::HasVideo() {
	return mHasVideo;
}

unsigned long NxVideo_Player::GetAudioBufferSize() {
	return mAudioBufferSize;
}

unsigned short NxVideo_Player::GetAudioNumChannels() {
	return mAudioNumChannels;
}

unsigned long NxVideo_Player::GetAudioSampleRate(){
	return mAudioSampleRate;
}

unsigned short NxVideo_Player::GetAudioBitsperSamples(){
	return mAudioBitsperSamples;
}



bool NxVideo_Player::NewFrame() {
	return mNewFrame;
}

void NxVideo_Player::ResetFrame() {
	mNewFrame = false;
}

NxVideo_Player::~NxVideo_Player() {
	
}

NxVideoStreamType NxVideo_Player::GetType() {
	return mVideoType;
}

void NxVideo_Player::Play() {
	SetPlayBackState( VIDEO_STATUS_PLAY );
}

void NxVideo_Player::Pause() {
	SetPlayBackState( VIDEO_STATUS_PAUSE ); 
}

void NxVideo_Player::Stop()
{
	SeekPosition( mFrameInbound ) ;
	SetPlayBackState( VIDEO_STATUS_STOP );
}

void NxVideo_Player::SeekPosition( unsigned long pos )
{
	mFrame = pos ;
}

void NxVideo_Player::StepTime( double TimePos )// Seconds ex 1.2 = 1 second and 2 milliseconds
{
	mFrame = TimePos * GetFramerate() ;
}

unsigned long NxVideo_Player::GetPositionFrame()
{
	return mFrame;
}

//void NxVideo_Player::AddListener( NxVideoPlayerListener * listener )
//{
//	 mListenersPlayerList.push_back(listener); 
//}
//
//void NxVideo_Player::RemoveListener( NxVideoPlayerListener * listener )
//{
//	mListenersPlayerList.remove( listener );
//}

void NxVideo_Player::SetManualUpdate( bool Manual )
{
	mManualUpdate = Manual;
}

bool NxVideo_Player::IsManualUpdate()
{
	return mManualUpdate ;
}
 
void NxVideo_Player::StepFrame( unsigned long Frame )
{
	if( IsManualUpdate() || ( mVideoPlayState == VIDEO_STATUS_PAUSE ) )
	{
		return;//goto UpdateListeners ;
	}
 
	unsigned long Frames_Length = mFrameOutbound ;
	unsigned long Frame_Start = mFrameInbound ;

	NxVideo_PlayBack_Status  status  = GetPlayBackState() ;

	if( status == VIDEO_STATUS_STOP )
	{
		mFrame = mFrameInbound ;
		return;//goto UpdateListeners ;
	}
	else if( status == VIDEO_STATUS_PLAY )
	{

	}
	else if( status == VIDEO_STATUS_PAUSE )
	{
	
	}

	switch( mVideoPlayType )
	{
		case VIDEO_PLAYBACK_NORMAL :
			mFrame = mFrame + 1 ;
			if( mFrame >= Frames_Length )
			{ 
				mFrame = mFrameInbound; 
			}
			mVideoCheckBounce = true ;
		break;
		case  VIDEO_PLAYBACK_REVERSE :
			mFrame =  mFrame - 1 ;
			if( mFrame <= Frame_Start ){ mFrame = Frames_Length - 1;}
			mVideoCheckBounce = true ;
		break;
		case VIDEO_PLAYBACK_BOUNCE :

		if( mVideoCheckBounce )
		{
			if( mVideoPlayTypePrevious == VIDEO_PLAYBACK_NORMAL ) 
			{ 
				mVideoSwitchBounce = true ; 
				mVideoCheckBounce = false ;
			}
			else if ( mVideoPlayTypePrevious == VIDEO_PLAYBACK_REVERSE )
			{
				mVideoSwitchBounce = false ; 
				mVideoCheckBounce = false ;
			}
			else
			{ 
				mVideoSwitchBounce = true ; 
				mVideoCheckBounce = false ;
			}
		}// bounce_switch

		if( mVideoSwitchBounce == true ) // we playback normal way
		{  
			mFrame = mFrame + 1 ;
			if( mFrame >= Frames_Length - 1 ){ mVideoSwitchBounce = false ; }
		}
		else // we play reverse way
		{
			 mFrame = mFrame - 1 ;
			 if( mFrame <= Frame_Start ){ mVideoSwitchBounce = true ; }
		}
	break;
	case  VIDEO_PLAYBACK_RANDOM :
		{
			mFrame = NxVideoUtils::RandNumber( Frame_Start, Frames_Length );
			mVideoCheckBounce = true ;
		}
	break;
		
	default :
	break;
	 }//switch

	//UpdateListeners :
 
	//for( std::list< NxVideoPlayerListener *  >::iterator iter =  mListenersPlayerList.begin(); iter !=  mListenersPlayerList.end(); iter++)	//update listeners if any
	//{
	//	NxVideoPlayerListener *  Listener = *iter; 
	//	Listener->NxVideoPlayerUpdated( mFrame );
	//}
}

	 

void NxVideo_Player::SetPlayBackType( NxVideo_PlayBack_Type type )
{
	mVideoPlayTypePrevious = mVideoPlayType;
	mVideoPlayType = type;
}

NxVideo_PlayBack_Type NxVideo_Player::GetPlayBackType()
{
	return mVideoPlayType ;
}



float NxVideo_Player::GetFramerate()
{
	return mVideoFrameRate ;
}

#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
void AccurateSleep(DWORD milliSeconds)
{
	static LARGE_INTEGER s_freq = {0,0};
	if (s_freq.QuadPart == 0)
		QueryPerformanceFrequency(&s_freq);

	LARGE_INTEGER from,now;
	QueryPerformanceCounter(&from);
	int ticks_to_wait = (int)s_freq.QuadPart / (1000/milliSeconds);
	bool done = false;
	int ticks_passed;
	int ticks_left;
	do
	{
		QueryPerformanceCounter(&now);
		ticks_passed = (int)((__int64)now.QuadPart - (__int64)from.QuadPart);
		ticks_left = ticks_to_wait - ticks_passed;

		if (now.QuadPart < from.QuadPart)    // time wrap
			done = true;
		if (ticks_passed >= ticks_to_wait)
			done = true;

		if (!done)
		{
			if (ticks_left > (int)s_freq.QuadPart*2/1000)
				Sleep(1);
			else                       
				for (int i=0; i<10; i++)
					Sleep(0);
		}
	}
	while (!done);           
}


static DWORD VideoUpdateThread( LPVOID lpParameter )
{
	NxVideo_Player * pointer = ( NxVideo_Player * ) lpParameter  ;

	while( true )
	{
		pointer->StepFrame( 0.0f );
		AccurateSleep( 1000 / pointer->GetFramerate() );
	} 

	return true ;
}
#endif

void NxVideo_Player::StartRendering()
{
#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
	 DWORD Video_Thread_ID;
	 HANDLE mVideoLoopHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) VideoUpdateThread, this, 0, &Video_Thread_ID );
	 SetThreadPriority ( mVideoLoopHandle , THREAD_PRIORITY_TIME_CRITICAL );
#endif
}

void NxVideo_Player::StopRendering()
{
	////mVideoLoop = false ;
	//WaitForSingleObject( mVideoLoopHandle, INFINITE );
	//TerminateThread( mVideoLoopHandle, 0 );
	//mVideoLoopHandle  = NULL ;
}

 


NxVideo_PlayBack_Status NxVideo_Player::GetPlayBackState()
{
	return mVideoPlayState ; 
}

void NxVideo_Player::SetPlayBackState( NxVideo_PlayBack_Status Status )
{
	mVideoPlayState = Status ;
}

unsigned long NxVideo_Player::GetVideoDurationframes()
{
	return mFramesLength;
}

unsigned long NxVideo_Player::GetWidth( int plane )
{
	return mVideoWidth[plane];
}
	 
unsigned long NxVideo_Player::GetHeight( int plane )
{
	return mVideoHeight[plane];
}
	 
unsigned int NxVideo_Player::GetBpp() {
	return mVideoBpp;
}

short NxVideo_Player::GetNumPlanes(){ 
	return mNumPlanes;
}

unsigned int NxVideo_Player::GetVideoHours()
{
	int Milliseconds = ( GetVideoDurationframes() / GetFramerate() ) * 1000 ;
	return Milliseconds / (1000*60*60);
}

unsigned int NxVideo_Player::GetVideoMinutes()
{
	int Milliseconds = ( GetVideoDurationframes() / GetFramerate() ) * 1000 ;
	return ( Milliseconds % (1000*60*60)) / (1000*60);
}

unsigned int NxVideo_Player::GetVideoSeconds()
{
	int Milliseconds = ( GetVideoDurationframes() / GetFramerate() ) * 1000 ;
	return   (( Milliseconds % (1000*60*60)) % (1000*60)) / 1000; 
}
 
const std::string NxVideo_Player::GetCodecName()
{
	return mVideoCodecName;
}

const std::string & NxVideo_Player::GetVideoFullPath() const
{ 
	return mVideoFullPath;
}

const std::string & NxVideo_Player::GetVideoName() const
{
	return mVideoFullName;
}

const std::string & NxVideo_Player::GetVideoExtension() const
{
	return mVideoExtension;
}

void NxVideo_Player::SetInboundFrame( unsigned long Frame )
{
	mFrameInbound = Frame ;
}

void NxVideo_Player::SetOutboundFrame( unsigned long Frame )
{
	mFrameOutbound = Frame ;
}

unsigned long NxVideo_Player::GetInboundFrame()
{
	return mFrameInbound;
}

unsigned long NxVideo_Player::GetOutboundFrame()
{
	return mFrameOutbound;
}

void NxVideo_Player::SetVolume( float Volume )
{


}

void NxVideo_Player::Rewind()
{
	mFrame = mFrameInbound ;
}

//unsigned char * NxVideo_Player::GetPixelData()
//{
//	return mVideoData;
//}

void NxVideo_Player::AddListener( NxVideoPlayerListener * listener )
{
	mListeners.push_back( listener );
}

void NxVideo_Player::RemoveListener( NxVideoPlayerListener * listener )
{
 
}

bool NxVideo_Player::IsLooping()
{
	return mLooping ;
}

void NxVideo_Player::SetLooping( bool Loop )
{
	mLooping = Loop;
}



}//namespace