#include "NxVideoPreCompiled.h"

#define forwardNormalSpeed 0x00010000 //normal speed (1) in fixed 
 
namespace NxVideo_Namespace {
 
struct NxVideoQuickTime
{
public:
    Movie MovieQT;
	Media mVideoMedia;
	Media mAudioMedia;
	short movieResFile;
	//ICMDecompressionSessionRef decompressionSession;
	unsigned char * Buffer;
	bool FrameReady;

	PixMapHandle pixmap;
	GWorldPtr gw;

	NxVideoQuickTime() : Buffer( 0 ), MovieQT( 0 ) { //decompressionSession( 0 ), 
		FrameReady = false;
	}

	~NxVideoQuickTime(){

	}

} ; 

///////////////////
enum NxVideoLoopOptions
{
	NxLoopOneWay,
	NxLoopBounce,
	NxLoopNone
};

void setLoopState( NxVideo_Mov_Player * Ptr, NxVideoLoopOptions state )
{
	TimeBase myTimeBase;
	long myFlags = 0L;
	myTimeBase = GetMovieTimeBase( Ptr->m_Video->MovieQT );
	myFlags = GetTimeBaseFlags(myTimeBase);
	switch (state) {
case NxLoopOneWay:
	myFlags |= loopTimeBase;
	myFlags &= ~palindromeLoopTimeBase;
	SetMoviePlayHints(Ptr->m_Video->MovieQT, hintsLoop, hintsLoop);
	SetMoviePlayHints(Ptr->m_Video->MovieQT, 0L, hintsPalindrome);
	break;
case NxLoopBounce:
	myFlags |= loopTimeBase;
	myFlags |= palindromeLoopTimeBase;
	SetMoviePlayHints(Ptr->m_Video->MovieQT, hintsLoop, hintsLoop);
	SetMoviePlayHints(Ptr->m_Video->MovieQT, hintsPalindrome, hintsPalindrome);
	break;
case NxLoopNone:
default:
	myFlags &= ~loopTimeBase;
	myFlags &= ~palindromeLoopTimeBase;
	SetMoviePlayHints(Ptr->m_Video->MovieQT, 0L, hintsLoop | hintsPalindrome);
	break;
	}

	SetTimeBaseFlags(myTimeBase, myFlags);
	SetMoviePlayHints( Ptr->m_Video->MovieQT, hintsOffscreen | hintsDontUseVideoOverlaySurface, hintsOffscreen | hintsDontUseVideoOverlaySurface ); 

}

//////////////////



NxVideo_Mov_Player::NxVideo_Mov_Player() : NxVideo_Player()
{
	mVideoType = NxVideoMov;
}

NxVideo_Mov_Player::~NxVideo_Mov_Player()  
{
 
}

bool NxVideo_Mov_Player::CloseVideoFile()
{
	Log( "NxVideo_Mov_Player::CloseVideoFile() ... " );
	ClearMoviesStickyError();
	mVideoPlayState = VIDEO_STATUS_STOP;
	 
	DisposeMovie( m_Video->MovieQT );
	StopMovie( m_Video->MovieQT );
	 Log("Closing Quicktime video File" ) ;
	CloseMovieFile( m_Video->movieResFile );
	m_Video->Buffer ? delete m_Video->Buffer : Log("No release m_Video->Buffer" ) ;
	delete m_Video;
	Log( "NxVideo_Mov_Player::CloseVideoFile() : DONE " );
	return true;
}

void NxVideo_Mov_Player::Play()
{


	GoToBeginningOfMovie( m_Video->MovieQT );

 

StartMovie( m_Video->MovieQT );

	MoviesTask( m_Video->MovieQT, 0 );



	NxVideo_Player::Play();



	//setLoopState( this, NxVideoLoopOptions::NxLoopBounce );


	//SetMoviePreferredRate(m_Video->MovieQT, FloatToFixed( 3.0 ) );
	//SetMovieRate( m_Video->MovieQT, FloatToFixed( 3.0 ) );

	for( std::vector< NxVideoPlayerListener * >::iterator Iter = mListeners.begin(); Iter != mListeners.end(); Iter++ ) {
		(*Iter)->OnPlay();
	}


}

void NxVideo_Mov_Player::Stop()
{
	StopMovie( m_Video->MovieQT );
	SetMovieActive( m_Video->MovieQT, false );
	SetPlayBackState( VIDEO_STATUS_STOP ); 

	for( std::vector< NxVideoPlayerListener * >::iterator Iter = mListeners.begin(); Iter != mListeners.end(); Iter++ ) {
		(*Iter)->OnStop();
	}
}

//! Rewind And Play Video.
void NxVideo_Mov_Player::Rewind()
{
	GoToBeginningOfMovie( m_Video->MovieQT );
}



void NxVideo_Mov_Player::SetVolume( float Volume )
{
	short VolumeOut = 255.0f * Volume;
	//Log("Volume Out : " + Ogre::StringConverter::toString( VolumeOut ));
	SetMovieVolume( m_Video->MovieQT, VolumeOut );
}



 

unsigned long NxVideo_Mov_Player::GetPositionFrame()
{
	TimeValue inTime = GetMovieTime( m_Video->MovieQT, nil );
	//OSType whichMediaType =  1;//VIDEO_TYPE;
	TimeValue newTimeValue = 0;
	GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeEdgeOK, inTime, 1, &newTimeValue, nil);
	if(newTimeValue == -1) {
		// Search backwards for the next available time
		GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeMediaSample,  inTime, -1, &newTimeValue, NULL);
		if(newTimeValue == -1) {
			//[NSException raise:@"CantFindTime" format:@"Cannot find a video frame time close to %ld", time.timeValue];
			Log("Cannot find a video frame time close");
		}
	}
	
	TimeValue aMediaTime, aDuration1;
	long aSampleIndex;
	MediaTimeToSampleNum( m_Video->mVideoMedia, TrackTimeToMediaTime( newTimeValue, GetMovieTrack( m_Video->MovieQT,1 ) ), &aSampleIndex, &aMediaTime, &aDuration1);
	return aSampleIndex;
}

void NxVideo_Mov_Player::SeekPosition( unsigned long Frame )
{
	// get duration and sample count  
	TimeValue duration = GetMovieDuration( m_Video->MovieQT );
	long samples = GetMediaSampleCount( m_Video->mVideoMedia);  
	// calculate time values per frame  
	double timeValuesPerFrame = (double)duration / (double)samples;  
	double t = (double)Frame * (double)timeValuesPerFrame;  
	// jump to specific time
	SetMovieTimeValue( m_Video->MovieQT, t);

	MoviesTask( m_Video->MovieQT, 0);

	//unsigned long res = GetPositionFrame();
	//if( Frame != res ) {
	//	Log("Error SetPosition : Frame not requested !");
	//	Log("movie duration : " + Ogre::StringConverter::toString( duration ));
	//	Log("Current Movie Time: " + Ogre::StringConverter::toString( GetMovieTime( m_Video->MovieQT, nil) ));
	//	Log("Wanted  Movie Time : " + Ogre::StringConverter::toString( (float)t ));
	//	Log("Asked Frame: " + Ogre::StringConverter::toString( Frame ));
	//	Log("Got Frame : " + Ogre::StringConverter::toString( res ));
	//	Log("Movie Framerate : " + Ogre::StringConverter::toString( GetFramerate() ));
	//	Log("timeValuesPerFrame : " + Ogre::StringConverter::toString( (float)timeValuesPerFrame ));
	//}
 
}

float NxVideo_Mov_Player::GetFramerate()
{
	long sampleCount = GetMediaSampleCount( m_Video->mVideoMedia );
	TimeValue64 duration = GetMediaDisplayDuration( m_Video->mVideoMedia );
	TimeValue64 timeScale = GetMediaTimeScale( m_Video->mVideoMedia );
	double outFPS = (float)sampleCount * (float)timeScale / (float)duration;
	return outFPS;

}

unsigned long NxVideo_Mov_Player::GetVideoDurationframes()
{
	return GetMediaSampleCount( m_Video->mVideoMedia );
}

void NxVideo_Mov_Player::StepTime( double TimePos )
{

	//TimeScale ts = GetMovieTimeScale( m_Video->MovieQT );

	TimeValue tv = GetMovieDuration( m_Video->MovieQT );
	 
	//TimeValue64 display_timeAudio;
	//SampleNumToMediaDisplayTime(  m_Video->mAudioMedia, mFrame+1, &display_timeAudio, NULL );
	SetMovieTimeValue( m_Video->MovieQT,  tv * TimePos );//(TimeValue)(mFrame*ts/1000/10000i64));

	//regler set time


	// http://www.gamasutra.com/view/feature/131606/a_c_platform_class_for_.php?print=1
	//et

	// http://www.openscenegraph.org/svn/osg/OpenSceneGraph/tags/OpenSceneGraph-3.1.0/src/osgPlugins/quicktime/MovieData.cpp
}

void NxVideo_Mov_Player::SetFramerate( float FrameRate )
{
	mVideoFrameRate = FrameRate;

	SetMovieRate( m_Video->MovieQT, FloatToFixed( 1.0f   ) ); // ! attention : https://developer.apple.com/quicktime/icefloe/dispatch013.html 
	SetMoviePreferredRate( m_Video->MovieQT, FloatToFixed( 1.0f  ) );

	//SetMoviePreferredRate( m_Video->MovieQT, FloatToFixed( FrameRate )  );
	//SetMovieRate( m_Video->MovieQT, FloatToFixed( FrameRate ) );
}


unsigned short GetVideoCodecTypeBpp( ImageDescriptionHandle imgDesc )
{
	unsigned short Bpp = -1;
	if( (*imgDesc)->cType == kJPEG2000CodecType ) {
		Log("Codec is : kJPEG2000CodecType");
		Bpp = 4;
	}
	else if( (*imgDesc)->cType == kH264CodecType ){
		Log("Codec is : kH264CodecType");
		Bpp = 3;
	}
	else if( (*imgDesc)->cType == kAnimationCodecType ) {
		Log("Codec is : kAnimationCodecType");
		Bpp = 4;
	}
	else if( (*imgDesc)->cType == kPNGCodecType ) {
		Log("Codec is : kPNGCodecType");
		Bpp = 4;
	}
	else if( (*imgDesc)->cType == kSorenson3CodecType ) {
		Log("Codec is : kSorenson3CodecType");
		Bpp = 3;
	}
	else if( (*imgDesc)->cType == kJPEGCodecType ) {
		Log("Codec is : kJPEGCodecType");
		Bpp = 3;
	}
	else if( (*imgDesc)->cType == kMotionJPEGACodecType ) {
		Log("Codec is : kMotionJPEGACodecType");
		Bpp = 3;
	}
	else if( (*imgDesc)->cType == kMotionJPEGBCodecType ) {
		Log("Codec is : kMotionJPEGBCodecType");
		Bpp = 3;
	}else
	{
		Bpp = 3;
		Log("Quicktime Bpp unimplemented");
	}

	return Bpp;

 
}

bool HasMediaTypeInTrack( Movie theMovie, OSType theMediaType)
{
	Track aTrack = NULL;
	long aTrackCount = 0;
	long index;
	OSType aMediaType;
	bool haveMediaType = false;

	aTrackCount = GetMovieTrackCount(theMovie);
	if(aTrackCount == 0)
		return false;				// no tracks in movie

	for(index = 1; index <= aTrackCount; index++)
	{
		aTrack = GetMovieIndTrack(theMovie, index);
		GetMediaHandlerDescription( GetTrackMedia(aTrack), &aMediaType, NULL, NULL);

		haveMediaType = ( aMediaType == theMediaType);
		if(haveMediaType == true)
			return true;
	}
	return false;
}




OSStatus AudioCall(void *inUserData, AudioUnitRenderActionFlags *ioRenderFlags,
				   const AudioTimeStamp *inTimeStamp,
				   UInt32 inNumberFrames,
				   AudioBufferList *inInputData,
				   AudioBufferList *outOutputData)
{

Log( "Processing audio...." );
return 0;
}

OSErr movieDrawingCompleteCallDelegate( Movie theMovie, long refCon)
{
	NxVideo_Mov_Player * ptr = ( NxVideo_Mov_Player * )refCon;

	//Log("Tick....");

	ptr->m_Video->FrameReady = true;

	//bool HasAlpha = ptr->GetBpp() == 4 ? true : false;

	//if ( LockPixels( ptr->m_Video->pixmap  ) )
	//{
	//	BYTE * pBits = ( BYTE *) GetPixBaseAddr( ptr->m_Video->pixmap );
	//	int boxsize = ptr->GetWidth() * ptr->GetHeight();
	//	// Swap alpha byte to the end, so ARGB become RGBA;

	//	//memcpy( &ptr->m_Video->Buffer[0], &pBits[0], ptr->GetWidth() * ptr->GetHeight() * ptr->GetBpp() );

	//	//unsigned char * from = (unsigned char *) pBits;
	//	//unsigned char * to = (unsigned char *) ptr->m_Video->Buffer;

	//	//for( unsigned long index = 0; index < boxsize; index++, from += ptr->GetBpp(), to += ptr->GetBpp()) {
	//	//	if(HasAlpha)to[3] = from[3];
	//	//	to[0] = from[2];
	//	//	to[1] = from[1];
	//	//	to[2] = from[0];
	//	//}

	//	ptr->m_Video->FrameReady = true;
	//	UnlockPixels( ptr->m_Video->pixmap );

	//	
	//}

	return noErr;
}



unsigned int firstPO2From( unsigned int  n)
{
	--n;            
	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	++n;
	return n;
}
/*
 Helper function to find the smallest power of two greater than or equal
 to the given number.
 */
static unsigned int NextLargerPowerOfTwo(unsigned int n) {
    unsigned int candidate = n;
    
    while((candidate & (candidate - 1)) != 0) {
        ++candidate;
    }
    
    return candidate;
}

bool NxVideo_Mov_Player::OpenVideoFile( const std::string & szFileName )
{
	if( !NxVideoManager::getSingleton().IsQuicktimeEnabled()){
		Log("NxVideo_Mov_Player::OpenVideoFile : failed, not quicktime support");
		return false;
	}

	m_Video = new NxVideoQuickTime();

	mVideoFullPath = std::string( szFileName );//save the video full path
	std::string Tmp = mVideoFullPath ;

	NxVideoUtils::SplitFileName( Tmp );
	mVideoFullName = Tmp;

	NxVideoUtils::SplitFileExtension( Tmp );
	mVideoExtension = Tmp;
	
    FSSpec sfFile;
#ifdef __APPLE__
	FSRef ref; // intermediate struct
	FSPathMakeRef( (const UInt8*)szFileName.c_str(), &ref, NULL );
	if ( FSGetCatalogInfo( &ref, kFSCatInfoNone , NULL, NULL, &sfFile, NULL) != noErr ){
		Log("FSGetCatalogInfo Failed!" );
		return false;
	}
#else
	if ( NativePathNameToFSSpec ((char*) (const char*) szFileName.c_str(), &sfFile, 0) != noErr ){
		Log("NativePathNameToFSSpec Failed!" );
		return false;
	}
#endif


	short movieResFile;
	OSErr err = OpenMovieFile (&sfFile, &movieResFile, fsRdPerm);
	if (err == noErr) {

		short movieResID = 0;
		err = NewMovieFromFile(&m_Video->MovieQT, movieResFile, &movieResID, (unsigned char *) 0, newMovieActive, (Boolean *) 0);
		if (err == noErr){
			CloseMovieFile (movieResFile);
		} else {
			Log( "NewMovieFromFile failed" );
			return false;
		}
	} else {
		Log("OpenMovieFile failed " );
		return false;
	}

	Rect r;
	GetMovieBox(m_Video->MovieQT , &r);
	
	//SetMovieBox(m_Video->MovieQT , &r);
	 
	mVideoWidth = r.right;
	mVideoHeight = r.bottom;
	mVideoBpp = 4;

	int texture_width = ((mVideoWidth + 7) >> 3) << 3;
	int texture_height = mVideoHeight;

	char * pointer = (char*)malloc(mVideoBpp * texture_width * texture_height + 32);

	m_Video->Buffer  = (unsigned char*)(((unsigned long)(pointer + 31) >> 5) << 5);

	mVideoWidth = texture_width ;
	mVideoHeight = texture_height ;


	Rect texture_bounds;
	texture_bounds.left = 0;
	texture_bounds.top = 0;
	texture_bounds.right = texture_width;
	texture_bounds.bottom = texture_height;

 

  /*
	https://developer.apple.com/quicktime/icefloe/dispatch020.html
   Create a GWorld to draw our movie into.  We can use our buffer to
   hold the pixel data for the movie.  k32ARGBPixelFormat is the
   native pixel format for the Mac, and appears to be the only available
   32-bit pixel format on the Mac.
   */ //kYUVSPixelFormat
	err = QTNewGWorldFromPtr(&m_Video->gw, k32BGRAPixelFormat , &texture_bounds, NULL, NULL, 0, m_Video->Buffer, mVideoBpp * texture_width); // k32BGRAPixelFormat
	//GDHandle origDevice;
	//CGrafPtr origPort;
	//GetGWorld (&origPort, &origDevice);
	//SetGWorld(m_Video->gw, NULL); // set current graphics port to offscreen
	SetMovieGWorld( m_Video->MovieQT, (CGrafPtr)m_Video->gw, NULL);
	SetMovieDrawingCompleteProc( m_Video->MovieQT, movieDrawingCallWhenChanged, movieDrawingCompleteCallDelegate , (long)this );

	SetMovieActive( m_Video->MovieQT, true );
	SetMovieAudioMute( m_Video->MovieQT, FALSE, 0);
	SetMovieVolume( m_Video->MovieQT, 255 );



 
	//SetMoviePreviewTime( m_Video->MovieQT , 0 , 2 );
	/*
	Log("set selection");
	
	SetMovieSelection(
		m_Video->MovieQT,
		0, GetMovieDuration( m_Video->MovieQT )
		);

	Log("set selection OK");
	*/


	//err = PrePrerollMovie( m_Video->MovieQT, 0, GetMoviePreferredRate(m_Video->MovieQT),NewMoviePrePrerollCompleteProc(MyMoviePrePrerollCompleteProc),(void *)0L);


	//m_Video->pixmap = GetGWorldPixMap (m_Video->gw);
	//if (m_Video->pixmap) {
	//	if (!LockPixels (m_Video->pixmap)) {  // lock offscreen pixel map
	//		ExitToShell ();
	//	}
	//}
	//else {
	//	ExitToShell ();
	//}

	//SetGWorld(origPort, origDevice);


 /*

	// Video
	Track VideoTrack = 0;
	//VideoTrack = GetMovieIndTrackType( m_Video->MovieQT, 1, VideoMediaType, movieTrackMediaType );

	VideoTrack = GetMovieIndTrackType(m_Video->MovieQT, 1, VisualMediaCharacteristic, movieTrackCharacteristic | movieTrackEnabledOnly);

	
	if (!VideoTrack){
		Log("mov file has no VideoMediaType");
		return false;
	} 

	//SetTrackEnabled(  VideoTrack, false );

	m_Video->mVideoMedia = GetTrackMedia( VideoTrack );

	//////////////////////////////////////////
 
	// Sound
	Log( "Looking for quicktime audio track info..." );
	Track AudioTrack = 0;
	AudioTrack = GetMovieIndTrackType( m_Video->MovieQT, 1, SoundMediaType, movieTrackMediaType | movieTrackEnabledOnly );
 
	if ( AudioTrack  != NULL )
	{
		// Get the sound track's media
		m_Video->mAudioMedia = GetTrackMedia( AudioTrack );
		TimeValue AudiomediaEnd = GetMediaDuration( m_Video->mAudioMedia  );
		TimeValue AudiotrackEnd = GetTrackDuration( AudioTrack );
		TimeValue VideomediaEnd = GetMediaDuration( m_Video->mVideoMedia  );
		TimeValue VideotrackEnd = GetTrackDuration( VideoTrack );

		//float trackDuration = (float) AudiotrackEnd  / (float)  1000; // make it milliseconds so it looks like an innocent oversight. 

		Log( "Audio media duration :   " + NxVideoUtils::ToString( (unsigned long) AudiomediaEnd ) );
		Log( "Audio track duration :   " + NxVideoUtils::ToString(  (unsigned long)AudiotrackEnd ) );
		Log( "Video media duration :   " + NxVideoUtils::ToString(  (unsigned long)VideomediaEnd ) );
		Log( "Video track duration :   " + NxVideoUtils::ToString( (unsigned long) VideotrackEnd ) );

		OSType aTrackType;
		GetMediaHandlerDescription( m_Video->mAudioMedia, &aTrackType, 0, 0);
		if(aTrackType == SoundMediaType) {
			SampleDescriptionHandle aDesc = (SampleDescriptionHandle)NewHandle(sizeof(aDesc));
			GetMediaSampleDescription( m_Video->mAudioMedia, 1, aDesc);
			if(GetMoviesError() == noErr) {
				SoundDescription& desc = **(SoundDescriptionHandle)aDesc;
				Log( "Audio num Channels rate : " + NxVideoUtils::ToString( desc.numChannels ) );
			}
			DisposeHandle((Handle)aDesc);
		}

		// Start reading the movie from the beginning, and remember how long it is
		TimeScale mediaTimeScale = GetMediaTimeScale(m_Video->mAudioMedia);
		TimeValue mediaDuration = GetMediaDuration(m_Video->mAudioMedia);

		QTAudioContextRef Ref;
		OSStatus stat =  GetMovieAudioContext( m_Video->MovieQT, &Ref );
		if( stat != 0 ){ Log( "GetMovieAudioContext Error ");  }


	}else
	{
		Log("Mov Video : No AUdio");
	}


//////

	 
	OSType dwType;
	Str255 creatorName;
	OSType creatorManufacturer;
	GetMediaHandlerDescription( m_Video->mVideoMedia, &dwType, creatorName, &creatorManufacturer );

	//SampleDescriptionHandle sampleDesc = (SampleDescriptionHandle)NewHandleClear(sizeof(SampleDescriptionHandle));//(SampleDescriptionHandle) NewHandle(0);
	//GetMediaSampleDescription( m_Video->mVideoMedia, 1, sampleDesc);

	ImageDescriptionHandle sampleDesc = (ImageDescriptionHandle)NewHandleClear(sizeof(ImageDescription));
	GetMediaSampleDescription( m_Video->mVideoMedia, 1, (SampleDescriptionHandle)sampleDesc );
	//mVideoBpp = GetVideoCodecTypeBpp( sampleDesc );	

	ComponentDescription cd = { decompressorComponentType, 0, 0, 0, cmpIsMissing };
	Component decompressor = 0;
	cd.componentSubType = (*(ImageDescriptionHandle)sampleDesc)->cType;
	decompressor = FindNextComponent(0, &cd);
	if (!decompressor){ Log(" FindNextComponent Failed ! "); return false; }

	Fixed Rate = GetMoviePreferredRate( m_Video->MovieQT );
	long sampleCount = GetMediaSampleCount( m_Video->mVideoMedia );
	TimeValue64 duration = GetMediaDisplayDuration( m_Video->mVideoMedia );
	TimeValue64 timeScale = GetMediaTimeScale( m_Video->mVideoMedia );
	double outFPS = (double)sampleCount * (double)timeScale / (double)duration;
	SetFramerate( outFPS );
	// SetMoviePreferredRate( m_Video->MovieQT, 0x00010000 );
	
	TimeValue trackEnd = GetTrackDuration( VideoTrack );
	TimeValue trackOffset = GetTrackOffset( VideoTrack );//begining of track in movie time 	
	TimeValue inTime = trackOffset;
	Picture  * moviePict = (Picture *)GetTrackPict( VideoTrack, inTime );

	GetTrackNextInterestingTime( VideoTrack, nextTimeMediaSample, inTime, forwardNormalSpeed, &inTime, nil);
 
	mFrameOffset = inTime;
	mFrameInbound = 0;//mFrameOffset ;
	mFrame = mFrameOffset ;

	mFramesLength = sampleCount;
	mFrameOutbound = mFramesLength ;



	TimeValue timeNow;
	Fixed playRate;
	timeNow = GetMovieTime( m_Video->MovieQT, 0);
	playRate = GetMoviePreferredRate( m_Video->MovieQT );

	Log("PrePrerollMovie Movie");
	PrePrerollMovie( m_Video->MovieQT, timeNow, playRate, 0, 0);

	Log("PreRolling Movie");
	PrerollMovie( m_Video->MovieQT, timeNow, playRate);
	SetMovieRate( m_Video->MovieQT, playRate);

//////////////
	OSErr StickyErr = GetMoviesStickyError();
    if( err == noErr  && StickyErr == noErr){
		//CreateDecompressionSession( this );
		//StartRendering();

		//mVideoFrameRate = 25;

		//SetMovieRate( m_Video->MovieQT, FloatToFixed( 1  ) );
		//SetMoviePreferredRate( m_Video->MovieQT, FloatToFixed( 1  ) );



        return true;
    }
    else{
		OSErr LastError = GetMoviesError();
		Log("GetMoviesStickyError() Failed! : " + NxVideoUtils::ToString( StickyErr ) );
		Log("err Failed! : " + NxVideoUtils::ToString( err ) );
		Log("LastError Failed! : " + NxVideoUtils::ToString( LastError ) );
		ClearMoviesStickyError();
        return false;
	}
 
*/
	return true;

}


void NxVideo_Mov_Player::SetLooping( bool Loop ) 
{
	mLooping = Loop;
	setLoopState( this, mLooping ? NxLoopOneWay : NxLoopNone );
}

void NxVideo_Mov_Player::SetPlayBackType( NxVideo_PlayBack_Type type )
{
	mVideoPlayType = type;
}

bool NxVideo_Mov_Player::RenderFrame()
{

	if( GetPlayBackState() == VIDEO_STATUS_STOP  ) { //|| mFramePrevious == mFrame
		return false;
	}

	

	// activeStart = 0.0f;
	//TimeValue activeDuration = 120000.0f;
	 


	//MediaGSetActiveSegment(  GetMediaHandler(  m_Video->mVideoMedia ) , activeStart, activeDuration);

 

	

	//int FrameIndex = mFrame;
	//TimeRecord CurrentTime;
	//CurrentTime.base = GetMovieTimeBase( m_Video->MovieQT );
	//long long FrameCount = FrameIndex ;//* 100;
	//CurrentTime.value = SInt64ToWide(FrameCount);
	//CurrentTime.scale = 25;//2997;
	//SetMovieTime( m_Video->MovieQT, &CurrentTime);

	//TimeValue64 display_time;
	//SampleNumToMediaDisplayTime(  m_Video->mAudioMedia, mFrame+1, &display_time, NULL )

 

	//TimeValue inTime = GetMovieTime( m_Video->MovieQT, nil );
	//GetTrackNextInterestingTime( GetMovieTrack( m_Video->MovieQT, 1 ), nextTimeEdgeOK | nextTimeMediaSample, inTime, forwardNormalSpeed, &inTime, nil);
	//TimeValue aMediaTime, aDuration1;
	//long aSampleIndex;
	//MediaTimeToSampleNum( m_Video->mVideoMedia, TrackTimeToMediaTime( inTime, GetMovieTrack( m_Video->MovieQT,1 ) ), &aSampleIndex, &aMediaTime, &aDuration1); 
	//if( aMediaTime == -1 ) return false;
	//if( mFramePrevious == aSampleIndex  ) return false;
	//mFramePrevious = aSampleIndex;


	//Log("Playing Sample : "  + Ogre::StringConverter::toString( aSampleIndex ) + " TIME : " + Ogre::StringConverter::toString( inTime ) );

 




	MoviesTask( m_Video->MovieQT, 0 );

	if( !mLooping && IsMovieDone( m_Video->MovieQT ) ) Stop(); 

	bool Res = m_Video->FrameReady;
	if( Res )
	{
		

		//Log("Playing Sample : "  + Ogre::StringConverter::toString( GetPositionFrame() )   );

		m_Video->FrameReady = false;
	}
 




	//bool res = m_Video->FrameReady;
	//if( m_Video->FrameReady  )  m_Video->FrameReady = false;
	//return res;

	//[ seeks good, reverse also, but no sound ]
	//SetPlayBackType( NxVideo_PlayBack_Type::VIDEO_PLAYBACK_REVERSE );
	//int FrameIndex = mFrame;
	//TimeRecord CurrentTime;
	//CurrentTime.base = GetMovieTimeBase( m_Video->MovieQT );
	//long long FrameCount = FrameIndex ;//* 100;
	//CurrentTime.value = SInt64ToWide(FrameCount);
	//CurrentTime.scale = 25;//2997;
	//SetMovieTime( m_Video->MovieQT, &CurrentTime);


	//UpdateMovie( m_Video->MovieQT );
	//MoviesTask( 0,0);//m_Video->MovieQT, 0);

	return Res;//m_Video->FrameReady;//true;
}

bool NxVideo_Mov_Player::NewFrame()
{
	return RenderFrame();
}

void NxVideo_Mov_Player::ResetFrame()
{
	mNewFrame = false;
}

void NxVideo_Mov_Player::Pause(void){


}

void NxVideo_Mov_Player::SetInboundFrame(unsigned long){


}

void NxVideo_Mov_Player::SetOutboundFrame(unsigned long){


}

void NxVideo_Mov_Player::GetPixelData( unsigned char * data  )
{
	data = m_Video->Buffer;
	//GetArgbData( &data[0], mVideoBpp , m_Video );

}

//unsigned char * NxVideo_Mov_Player::GetPixelData()
//{
//	return m_Video->Buffer;
//}


}