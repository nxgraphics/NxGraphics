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
#ifndef __NXSOUNDDEVICE_H__
#define __NXSOUNDDEVICE_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

enum NxSoundDeviceTypes
{ 
	NxSoundTypeDeviceInput,
	NxSoundTypeDeviceOutput
};
	
//!  Sound Device class for NxSound. shouldnt be used directly.
class NxSound_Export NxSoundDevice
{
protected :
	//! NxSoundDevice constructor.
	NxSoundDevice( int AudioDeviceId );
public :
	//! NxSoundDevice destructor.
	virtual ~NxSoundDevice();
	//! Get device name.
	const std::string GetDeviceName() const;
	//! get Num Channels.
	unsigned int GetNumChannels() const ;
	//! Add effect plugin.
 	bool AddAudioPlugin( NxSoundEffect * Effect );
	//! Get FFT spectrum.
	//bool GetFFTSpectrum( float * Spectrum );
	//! Get FFT num bins.
	unsigned int GetFFTNumBins() const;


	//NxSoundFFT * GetFFT();
	float * GetFFTAmplitude();
	float * GetFFTData();
	float * GetFFTDecibels();

	//! add listener.
	void AddListener( NxSoundManagerListener * listener );
	//! Get Device Id.
	//unsigned long GetID();
	//! Compute final output audio buffer stream from audio plugins.
	void ProcessDeviceEffectPlugins( stk::StkFrames & Frame );
	//! Copy in Buffer helper.
	void CopyIn( stk::StkFrames & Frame );
	//! Copy out buffer helper.
	void CopyOut( stk::StkFrames & Frame );
protected :
	//! Generate internal audio buffers.
	void CreateAudioBuffers();
	//! Generate Id.
	//void GenerateID();
	//! Audio device plugin list.
	std::list< NxSoundEffect * > mDevicePluginList ;
	float ** outBufs;
	float ** inBufs;
	NxSoundDeviceTypes mDevType ;
	//unsigned long mDeviceID  ;
	NxSoundFFT * mDeviceFFT;
	int mAudioIndex;
	RtAudio * mAudioDevice;
	//! Device Audio Format in Use.
	NxAudioFormat mFormat;
	unsigned int mNumOutChannels;


};

}// namespace

#endif