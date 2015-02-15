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
#ifndef __NXVIDEOAVIMANAGER_H__
#define __NXVIDEOAVIMANAGER_H__

#include "NxVideo_Prerequisites.h"
#include "NxVideo_Player.h"
 
namespace NxVideo_Namespace {

//!  A Class Buffer Utility For NxVideo
/*!
	NxVideoBuffer Class Implementation.
*/
class NxVideo_Export NxVideoBuffer
{
public :
	NxVideoBuffer( int Width, int Height, int bpp );
	~NxVideoBuffer();
	//virtuals
	void GetPixelData( unsigned char * data ); 

private :
	unsigned char * mVideoData;
	unsigned long mVideoWidth;
	unsigned long mVideoHeight;
	int mVideoBpp;

};


class NxVideo_Export NxVideo_Theora_Player : public NxVideo_Player
{
public :
	NxVideo_Theora_Player();
	~NxVideo_Theora_Player();

	bool OpenVideoFile( const std::string& FilePath, NxVideoPixelFormatInfo & info );
	//! Close Video File.
	bool CloseVideoFile();
	bool NewFrame();
	//! Render a Frame.
	bool RenderFrame();
	bool GetAudioData( int64 audioSampleindex, char * data, unsigned int * numSamples, NxVideoStreamFlags * OutFlags );
	void GetPixelData( unsigned char * data  );
	void SetFramerate( float framerate );
	void SetOpenGLID( int plane, unsigned int ID );
	NxVideoTheora * m_video;
};


#if NXVIDEO_PLATFORM == NXVIDEO_PLATFORM_WIN32
//!  A Video Avi Player Class Utility For NxVideo
/*!
	NxVideo_Avi_Player Class Implementation.
*/
class NxVideo_Export NxVideo_Avi_Player : public NxVideo_Player
{
public :
	NxVideo_Avi_Player();
	~NxVideo_Avi_Player();
	bool OpenVideoFile( const std::string & szFileName, NxVideoPixelFormatInfo & info );
	bool CloseVideoFile();
	bool RenderFrame();
	//virtuals
	void GetPixelData( unsigned char * data ); 
	bool GetAudioData( int64 audioSampleindex, char * data, unsigned int * numSamples, NxVideoStreamFlags * OutFlags ){  return false; };
	bool NewFrame();
	void ResetFrame();
	void SetFramerate( float FrameRate ) ;
	//void Get_Video_Codec_Info( std::string file );
	//void Video_Pause_Rendering( bool value );
	//bool FoundCorrectDecompressor();
	//NxVideo_Info * VIDEO_GLOBAL_INFO ;
	NxVideoVFW * m_Video;
};

//!  A Video Avi Recorder Class Utility For NxVideo
/*!
	NxVideo_Avi_Recorder Class Implementation.
*/
class NxVideo_Export NxVideo_Avi_Recorder : public NxVideo_Recorder
{
public :
	NxVideo_Avi_Recorder();
	~NxVideo_Avi_Recorder();
	bool CreateVideoFile( const std::string & szFileName, int width, int height, int Bpp, int Fps );
	bool CloseVideoFile();
	void AddVideoFrame( unsigned char * Buffer );
private:
	NxVideoRecorderVFW * mVideo;
};
#endif

//!  A Video Mov Player Class Utility For NxVideo
/*!
	NxVideo_Mov_Player Class Implementation.
*/
class NxVideo_Export NxVideo_Mov_Player : public NxVideo_Player
{
public :
	NxVideo_Mov_Player();
	~NxVideo_Mov_Player();
	bool OpenVideoFile( const std::string & szFileName, NxVideoPixelFormatInfo & info );
	bool CloseVideoFile();
	bool RenderFrame();
	//virtuals
	void GetPixelData( unsigned char * data );
	void SetOpenGLID( int plane, unsigned int ID ); 
	bool GetAudioData( int64 audioSampleindex, char * data, unsigned int * numSamples, NxVideoStreamFlags * flags );
	void SetFramerate( float FrameRate );
	void StepTime( double TimePos );
	void SeekPosition( unsigned long Frame );
	float GetFramerate();
	unsigned long GetVideoDurationframes();
	unsigned long GetPositionFrame();
	//! Rewind And Play Video.
	void Rewind();
	//virtuals
	void SetInboundFrame( unsigned long Frame );
	void SetOutboundFrame( unsigned long Frame );
	void SetPlayBackType( NxVideo_PlayBack_Type type );
	void SetLooping( bool Loop );
	void SetVolume( float Volume );
	bool NewFrame();
	void ResetFrame();
	void Play();
	void Stop();
	void Pause();
	NxVideoQuickTime * m_Video;

};

 



} 

#endif
