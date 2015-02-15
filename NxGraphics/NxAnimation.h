/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
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
#ifndef __NXANIMATION_H__
#define __NXANIMATION_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxKeyframe
{
public:
	//! NxKeyframe Constructor.
	NxKeyframe( float Time, float Value = 0.0f );
	//! NxKeyframe Destructor.
	virtual ~NxKeyframe();
	//! Get Keyframe Time.
	float GetTime() const { return mTime; }
	//! Set Keyframe Time.
	void SetTime( float Time );
	//! Get Value.
	float GetValue() const;
	//! Set Value.
	void SetValue( float Val );

protected :
	float mTime;
	float mValue;
};

class NxGraphics_Export NxEvent : public NxKeyframe
{
public:
	NxEvent( float Time, unsigned int Value = 0 ) : NxKeyframe( Time, Value )
	{
	
	}
};

class NxGraphics_Export NxAnimation  
{
	enum AnimationTypes
	{
		AnimationNone,
		AnimationPlayForward,
		AnimationPlayBackward,
		AnimationBounce,
		AnimationRandom
	};

public :
	//! NxAnimation Constructor.
	NxAnimation( const std::string & AnimationName );
	//! NxAnimation Destructor.
	virtual ~NxAnimation();
	//! Set Animation Type.
	void SetAnimationType( AnimationTypes Type );
	//! Get Animation Type.
	AnimationTypes GetAnimationType();
	//! Get Animation Name.
	const std::string GetAnimationName();
	//! Add a Keyframe To Animation.
	void AddKeyframe( NxKeyframe * Key );
	//! Get Number keyframes.
	size_t GetNumKeyframes();
	//! Update By Time.
	void Update( unsigned long elapsedMS );
	//! Set Length.
	void SetLength( float Time );
	//! Get Length.
	float GetLength();
	//! Set Animation Speed.
	void SetSpeed( float Speed );
	//! Enable Animation.
	void SetEnabled( bool Enabled );
	//! Delete all KeyFrames.
	void DeleteKeyframes();
	//! Add a Event To Animation.
	void AddEvent( NxEvent * Event );
	//! Delete all events.
	void DeleteEvents();
	//! Virtuals
	virtual void OnValue( const float Time, const float Value ) = 0;
	//! Virtuals
	virtual void OnKeyframe( const NxKeyframe* Key ) = 0;
	//! Virtuals
	virtual void OnEvent( const NxEvent* Event ) = 0;
private:
	void GetInterpolatedKeyFrame( const float timeIndex, NxKeyframe* kf );
	float GetKeyFramesAtTime( float timeIndex, NxKeyframe** keyFrame1, NxKeyframe** keyFrame2, unsigned short* firstKeyIndex = 0) const;
	typedef std::vector<NxKeyframe*> KeyFrameList;
	KeyFrameList mKeyFrames;
	AnimationTypes mType;
	float mTotalAnimationLength;
	float mTotalElapsedTime;
	bool mEnabled;
	float mSpeed;
	NxKeyframe * mLastKey;
	std::string mAnimationName;
	//! Events
	void GetInterpolatedEvent( const float timeIndex, NxEvent * kf );
	//float GetEventsAtTime( float timeIndex, NxEvent** keyFrame1, NxEvent** keyFrame2, unsigned short* firstKeyIndex) const;
	void GetEventsAtTime( float timeIndex, NxEvent** keyFrame1 ) const;
	typedef std::vector<NxEvent*> EventList;
	EventList mEvents;
	NxEvent * mLastEvent;
	float mTotalEventElapsedTime;
};

}//namespace

#endif