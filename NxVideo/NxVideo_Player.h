/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __      _    __ _      __           
			   / | / /_  __| |  / /(_)____/ /___   ____ 
			  /  |/ /| |/_/| | / // // __  // _ \ / __ \
			 / /|  /_>  <  | |/ // // /_/ //  __// /_/ /
			/_/ |_//_/|_|  |___//_/ \__,_/ \___/ \____/ 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __NXVIDEOPLAYER_H__
#define __NXVIDEOPLAYER_H__
 
#include "NxVideo_Prerequisites.h"

namespace NxVideo_Namespace {

class NxVideo_Export NxVideoPlayerListener
{
	public :
	virtual ~NxVideoPlayerListener(){};
	virtual void OnFrameChanged( unsigned long NewFrameNb ) = 0;
	virtual void OnStop() = 0;
	virtual void OnPlay() = 0;
};

//!  A Class To Play Video Files / Avi / MOV / SWF / FLV / WEB.
/*!
	NxVideo_Player Class Implementation.
*/
class NxVideo_Export NxVideo_Player
{
	public : 
	//! NxVideo_Player Class Constructor.
	NxVideo_Player();
	//! NxVideo_Player Class Deconstructor.
	virtual ~NxVideo_Player();
	//! Open File With Specified Parameters.
	virtual bool OpenVideoFile( const std::string& FilePath, NxVideoPixelFormatInfo & info ) = 0;
	//! Close Video File.
	virtual bool CloseVideoFile() = 0;
	//! Render a Frame.
	virtual bool RenderFrame() = 0;
	//! Media has audio ?
	bool HasAudio();
	//! Media has Video ?
	bool HasVideo();
	//! Get bpp from pixel format
	unsigned short GetVideoBppFromPixelFormat( NxVideoPixelFormat format );
	//! get audio data.
	virtual bool GetAudioData( int64 audioSampleindex, char * data, unsigned int * numSamples, NxVideoStreamFlags * OutFlags ) = 0;
	//! get Audio buffer size.
	unsigned long GetAudioBufferSize();
	//! get Audio channels number.
	unsigned short GetAudioNumChannels();

	unsigned long GetAudioSampleRate() ;
 
	unsigned short GetAudioBitsperSamples();

	bool IsPlanar();
	NxVideoPixelFormat GetVideoFormat();
	//! get pixel data.
	virtual void GetPixelData( unsigned char * data  ) = 0;
	//! set opengl texture ID
	virtual void SetOpenGLID( int plane, unsigned int ID ); 
	//! get opengl texture ID
	unsigned int GetOpenGLID( int plane = 0); 
	//! Set Video Framerate.
	virtual void SetFramerate( float framerate ) = 0;
	//! Get Video Framerate.
	virtual float GetFramerate();
	//! Play Video.
	virtual void Play();
	//! Pause Video;
	virtual void Pause();
	//! Stop Video.
	virtual void Stop();
	//! Rewind And Play Video.
	virtual void Rewind();
	//! Set Video Position.
	virtual void SeekPosition( unsigned long Frame );
	//! Step frame Number
	void StepFrame( unsigned long FrameNum );
	//! Get Current Video Frame Position.
	virtual unsigned long GetPositionFrame();
	//! Step to video Position ( Time )
	virtual void StepTime( double TimePos );
	//! Get Video Current Position / Hour.
	unsigned int GetVideoHours();
	//! Get Video Current Position / Minutes.
	unsigned int GetVideoMinutes();
	//! Get Video Current Position / Seconds.
	unsigned int GetVideoSeconds();
	//! Get Video Width.
	unsigned long GetWidth( int plane = 0 );
	//! Get Video Height.
	unsigned long GetHeight( int plane = 0 );
	//! Get Video Bits Per Pixel.
	unsigned int GetBpp();
	//! get num planes.
	short GetNumPlanes();

	//! Get Video Duration / Frames.	
	virtual unsigned long GetVideoDurationframes();
	//! Get Video Codec Name.
	const std::string GetCodecName();
	//! Get Video Stream Type. see NxVideoStreamType.
	NxVideoStreamType GetType();
	//! Get Video Full Path.
	const std::string & GetVideoFullPath() const;
	//! Get Video Name.
	const std::string & GetVideoName() const;
	//! Get Video Extension.
	const std::string & GetVideoExtension() const;

	bool IsLooping();
	virtual void SetLooping( bool Loop );

	// ! Audio
	virtual void SetVolume( float Volume );

	virtual void SetInboundFrame( unsigned long Frame );
	virtual unsigned long GetInboundFrame();
	virtual void SetOutboundFrame( unsigned long Frame );
	virtual unsigned long GetOutboundFrame();

	virtual bool NewFrame();
	virtual void ResetFrame();
 
	void SetManualUpdate( bool Manual );
	bool IsManualUpdate();

	virtual void SetPlayBackType( NxVideo_PlayBack_Type type );
	NxVideo_PlayBack_Type GetPlayBackType();

	NxVideo_PlayBack_Status GetPlayBackState();
	void SetPlayBackState( NxVideo_PlayBack_Status Status );

	//friend DWORD VideoUpdateThread( LPVOID lpParameter );
	void StartRendering();
	void StopRendering();


	void AddListener( NxVideoPlayerListener * listener );
	void RemoveListener( NxVideoPlayerListener * listener );
 
	bool mNewFrame;

	bool mVideoSwitchBounce ;
	bool mVideoCheckBounce ;
	unsigned char * mVideoData;
protected:
	short mNumPlanes;
	unsigned long mVideoWidth[3];
	unsigned long mVideoHeight[3];
	int mVideoBpp;
	
	bool mManualUpdate ;
	unsigned long mFrameInbound;
	unsigned long mFrameOutbound;
	unsigned long mFrameOffset;
	unsigned long mFramesLength;
	unsigned long mFrame;
	unsigned long mFramePrevious;
	float mVideoFrameRate;
	std::string mVideoFullPath ;
	std::string mVideoFullName ;
	std::string mVideoExtension ;
	std::string mVideoCodecName;
	NxVideoStreamType mVideoType;

	bool mLooping;

 
	bool mHasAudio;
	unsigned long mAudioBufferSize;
	unsigned short mAudioNumChannels;
	unsigned long  mAudioSampleRate;
	unsigned short mAudioBitsperSamples;


	NxVideoPixelFormat mVideoFormat;
	bool mHasVideo;
	unsigned int mOpengGlID[3];

	NxVideo_PlayBack_Type mVideoPlayType;
	NxVideo_PlayBack_Type mVideoPlayTypePrevious;
	NxVideo_PlayBack_Status mVideoPlayState;

	std::vector<NxVideoPlayerListener*> mListeners;
	
};


//!  A Class To Record Video Files / Avi / MOV.
/*!
	NxVideo_Recorder Class Implementation.
*/
class NxVideo_Export NxVideo_Recorder
{
public:
	NxVideo_Recorder();
	~NxVideo_Recorder();
};

}

#endif