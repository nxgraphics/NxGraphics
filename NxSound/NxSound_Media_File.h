/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __      _____                           __
			   / | / /_  __/ ___/ ____   __  __ ____   ____/ /
			  /  |/ /| |/_/\__ \ / __ \ / / / // __ \ / __  / 
			 / /|  /_>  < ___/ // /_/ // /_/ // / / // /_/ /  
			/_/ |_//_/|_|/____/ \____/ \__,_//_/ /_/ \__,_/   
			                                                  
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
#ifndef __NXSOUNDMEDIAFILE_H__
#define __NXSOUNDMEDIAFILE_H__

#include "NxSound_Prerequisites.h"
#include "NxSound_Media_DataTypes.h"

namespace NxSound_Namespace {

//!  A class to manage a sound media files.
/*!
	this is the base class for all media files and shouldnt be used directly.
*/
class NxSound_Export NxSoundMediaFile
{
protected :	
	//! NxSoundMediaFile Class Constructor.
	NxSoundMediaFile();
public :
	//! NxSoundMediaFile Class Destructor.
	virtual ~NxSoundMediaFile();
	//! Open File.
	virtual bool OpenFile( std::string Filename, bool Normalize = true );
	//! Get File Path.
	const std::string& GetFilePath();
	//! Get File Name.
	const std::string& GetFileName();
	//! Get File extension.
	const std::string& GetFileExtension();
	//! Get File Stream Type.
	NxSoundMediaType GetMediaType();
	//! Get number of total samples in File.
	virtual unsigned long GetNumSamples();
	//! Get number of channels in file.
	unsigned int GetNumChannels();
	//! get audio resolution
	NxAudioFormat GetResolution();
	//! Set Output Resolution
	virtual void SetResolution( NxAudioFormat Res ) = 0;
	//! Get duration in seconds.
	virtual unsigned long GetLengthSeconds();
	//! Get duration in Ms.
	virtual unsigned long GetLengthMilliSeconds();
	//! Set playback rate. from 0.0 to 1.0.
	virtual void SetRate( double Rate ) = 0;
	//! Get playback rate. from 0.0 to 1.0.
	float GetRate();
	//! Set volume rate. from 0.0 to 1.0.
	void SetVolume( float Volume );
	//! Get volume rate. from 0.0 to 1.0.
	float GetVolume();
	//! Set pan.
	void SetPan( float Pan );
	//! Get pan.
	float GetPan();
	//! mute file sound output.
	void SetMute( bool Mute );
	//! Is Muted.
	bool isMuted();
	//! Set medial file looping.
	virtual void SetLooping( bool Loop );
	//! is media file in looping mode.
	bool GetLooping();
	//! Play the media file.
	virtual void Play();
	//! Stop media file playback.
	virtual void Stop();
	//! Pause media file playback.
	virtual void Pause();
	//! rewind file to inbound start time;
	virtual void Rewind() = 0;
	//! Is playback stopped.
	virtual bool isStopped();
	//! Is playback Paused.
	virtual bool isPaused();
	//! Is playback Playing.
	virtual bool IsPlaying();
	//! Is media file stereo.
	bool IsStereo();
	//! is media file mono.	
	bool IsMono();
	//! Set playback cursor position.
	virtual void SetPosition( unsigned long position, NXSOUNDTIMEUNIT postype );
	//! Get playback cursor position.	
	virtual double GetPosition( NXSOUNDTIMEUNIT postype );
	//! get total duration.	
	virtual unsigned long GetTotalDuration( NXSOUNDTIMEUNIT postype );
	//! Add Audio Effect Plugin to stream.
	bool AddAudioPlugin( NxSoundEffect * Effect );
	//! Remove Audio Effect Plugin to stream.
	bool RemoveAudioPlugin( NxSoundEffect * Effect );


	virtual stk::StkFrames& Update( stk::StkFrames& frames ) = 0;
	virtual std::vector< NxSoundFrames *> GetSampleDictionary();  
	virtual unsigned int GetFirstSamplePosition();


	NxSoundEffect * GetAudioPlugin( int index );
	virtual void ProcessEffectPlugins( stk::StkFrames & Frame );
	void CreateEffectBuffers();


//protected :
	float mVolume;
	float mPan ;
	bool mMuted ;
	bool mLoop;
	bool mPlaying;
	bool mPaused;
	bool mStopped;
	unsigned int mNumChannels ;
	string mFilePath ;
	string mFileName ;
	string mFileExtension ;
	NxSoundMediaType mType ;
	float ** outBufs;
	float ** inBufs;

	bool chunking_ ;
	unsigned long chunkPointer_ ;
	bool normalizing_  ; 
	NxSoundFrames firstFrame_;
	double phaseOffset_;
	bool finished_;
	bool interpolate_;
	double time_;
	double rate_;
	unsigned long chunkThreshold_;
	unsigned long chunkSize_;
	NxSoundFrames data_;
	NxSoundFrames lastFrame_;
 
protected :
	//NxAudioFormat mResolution;
	NxAudioFormat mDstResolution;
	
private :
	void CopyIn( stk::StkFrames & Frame );
	void CopyOut( stk::StkFrames & Frame );
	void CopyOutUntouched( stk::StkFrames & Frame );
	//! Audio plugin List.
	std::list< NxSoundEffect * > mAudioPluginList ;
	

};

}//namespace

#endif