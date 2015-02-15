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
#ifndef __NXSOUNDPLUGINEFFECTGRANULATE_H__
#define __NXSOUNDPLUGINEFFECTGRANULATE_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSound_Export NxSoundEffectGranulate : public NxSoundEffect
{
public:
	NxSoundEffectGranulate();
	~NxSoundEffectGranulate();
	//virtuals
	void Process( float ** BufIn, float ** BufOut, int BlockSize );

	/*Set the number of simultaneous grain "voices" to use.
    Multiple grains are offset from one another in time by grain
    duration / nVoices.  For this reason, it is best to set the grain
    parameters before calling this function (during initialization).*/
	void SetVoices( unsigned int nVoices );

	/*Set the stretch factor used for grain playback (1 - 1000).
    Granular synthesis allows for time-stetching without affecting
    the original pitch of a sound.  A stretch factor of 4 will produce
    a resulting sound of length 4 times the orignal sound.  The
    default parameter of 1 produces no stretching.*/
	void SetStretch( unsigned int stretchFactor );

	/*Set global grain parameters used to determine individual grain settings.
    Each grain is defined as having a length of \e duration
    milliseconds which must be greater than zero.  For values of \e
    rampPercent (0 - 100) greater than zero, a linear envelope will be
    applied to each grain.  If \e rampPercent = 100, the resultant
    grain "window" is triangular while \e rampPercent = 50 produces a
    trapezoidal window.  In addition, each grain can have a time delay
    of length \e delay and a grain pointer increment of length \e
    offset, which can be negative, before the next ramp onset (in
    milliseconds).  The \e offset parameter controls grain pointer
    jumps between enveloped grain segments, while the \e delay
    parameter causes grain calculations to pause between grains.  The
    actual values calculated for each grain will be randomized by a
    factor set using the setRandomFactor() function.*/
	void SetGrainParameters( unsigned int duration, unsigned int rampPercent, int offset, unsigned int delay );

    /*This random factor is applied when all grain state durations
    are calculated.  If set to 0.0, no randomness occurs.  When
    randomness = 1.0, a grain segment of length \e duration will be
    randomly augmented by up to +- \e duration seconds (i.e., a 30
    millisecond length will be augmented by an extra length of up to
    +30 or -30 milliseconds).*/
	void SetRandomFactor( double randomness );
	void SetSoundFile( std::string path );
	void Reset();
private :
	stk::Granulate * granule ;
};

}//namespace

#endif