#include "NxGraphicsPreCompiled.h"

namespace Nx {

struct KeyFrameTimeLess
{
    bool operator()( const NxKeyframe* kf, const NxKeyframe* kf2 ) const
    {
        return kf->GetTime() < kf2->GetTime();
    }
};

NxAnimation::NxAnimation( const std::string & AnimationName ) : mType( AnimationNone ), mAnimationName(AnimationName), mTotalAnimationLength(0.0f), mEnabled( true )
{
	mSpeed = 1.0f;
	mTotalElapsedTime = 0;
	mTotalEventElapsedTime = 0;
	mLastKey = 0;
	mLastEvent = 0;
}

NxAnimation::~NxAnimation()
{
	DeleteKeyframes();
}
 
const std::string NxAnimation::GetAnimationName()
{
	return mAnimationName;
}

void NxAnimation::GetInterpolatedKeyFrame( const float timeIndex, NxKeyframe * kf ) 
{
	NxKeyframe * kret = static_cast<NxKeyframe*>(kf);
    NxKeyframe *k1, *k2;// Keyframe pointers
    unsigned short firstKeyIndex;
    Nx::Real t = this->GetKeyFramesAtTime( timeIndex, &k1, &k2, &firstKeyIndex);

    if( t == 0.0 )
    {
        kret->SetValue( k1->GetValue() );// Just use k1
    }
    else // k1 & k2
    {
		float diff = k2->GetValue() - k1->GetValue();// Interpolate by t
		kret->SetValue( k1->GetValue() + diff * t);
    }

	if( k1 != mLastKey )
	{  
		this->OnKeyframe( k1 );
		mLastKey = k1;
	}
}

void NxAnimation::GetInterpolatedEvent( const float timeIndex, NxEvent * kf ) 
{
	NxEvent * k1 = NULL ;
	unsigned short firstEventIndex;
	this->GetEventsAtTime( timeIndex, &k1 );

	if( k1 != mLastEvent ) {  

		this->OnEvent( k1 );
		mLastEvent = k1;
	}
}

void NxAnimation::GetEventsAtTime( float timeIndex, NxEvent** keyFrame1 ) const
{
	EventList::const_iterator i;
	NxEvent timeKey( timeIndex );
	i = std::lower_bound( mEvents.begin(), mEvents.end(), &timeKey, KeyFrameTimeLess());

	if (i == mEvents.end()){
		--i;
	}
	else{
		if (i != mEvents.begin() && timeIndex < (*i)->GetTime()){
			--i;
		}
	}

	*keyFrame1 = *i;
}

float NxAnimation::GetKeyFramesAtTime( float timeIndex, NxKeyframe** keyFrame1, NxKeyframe** keyFrame2, unsigned short* firstKeyIndex) const
{
    // Parametric time
    // t1 = time of previous keyframe
    // t2 = time of next keyframe
    Nx::Real t1, t2;

    // Wrap time
	assert(mTotalAnimationLength > 0.0f && "Invalid animation length!");

    while( timeIndex > mTotalAnimationLength && mTotalAnimationLength > 0.0f )
    {
        timeIndex -= mTotalAnimationLength;
    }

	KeyFrameList::const_iterator i;
    // No global keyframe index, need to search with local keyframes.
    NxKeyframe timeKey( timeIndex );
    i = std::lower_bound(mKeyFrames.begin(), mKeyFrames.end(), &timeKey, KeyFrameTimeLess());
  
    if (i == mKeyFrames.end())
    {
        // There is no keyframe after this time, wrap back to first
        *keyFrame2 = mKeyFrames.front();
		t2 = mTotalAnimationLength + (*keyFrame2)->GetTime();
        // Use last keyframe as previous keyframe
        --i;
    }
    else
    {
        *keyFrame2 = *i;
        t2 = (*keyFrame2)->GetTime();
        // Find last keyframe before or on current time
        if (i != mKeyFrames.begin() && timeIndex < (*i)->GetTime())
        {
            --i;
        }
    }

    // Fill index of the first key
    if (firstKeyIndex)
    {
        *firstKeyIndex = static_cast<unsigned short>(std::distance(mKeyFrames.begin(), i));
    }

    *keyFrame1 = *i;

    t1 = (*keyFrame1)->GetTime();

    if (t1 == t2)
    {
        return 0.0;// Same KeyFrame (only one)
    }
    else
    {
        return (timeIndex - t1) / (t2 - t1);
    }
}

void NxAnimation::Update( unsigned long elapsedMS )
{
	if( !mEnabled ) return;

	float divideBy1000 = 0.001f;
	float elapsedS = static_cast<float>(elapsedMS) * divideBy1000;

	//Keyframes
	mTotalElapsedTime += ( elapsedS * mSpeed );

	if( mKeyFrames.size() ){
		NxKeyframe kf( mTotalElapsedTime );
		GetInterpolatedKeyFrame( mTotalElapsedTime, &kf);
		OnValue( mTotalElapsedTime, kf.GetValue() );
	}


	// Events
	mTotalEventElapsedTime += ( elapsedS * mSpeed );

	if( mEvents.size() ) {
		NxEvent ev( mTotalEventElapsedTime );
		GetInterpolatedEvent( mTotalEventElapsedTime, &ev ) ;
	}
	 
}

void NxAnimation::SetLength( float Time )
{
	mTotalAnimationLength = Time;
}

float NxAnimation::GetLength()
{
	return mTotalAnimationLength;
}

void NxAnimation::SetEnabled( bool Enabled )
{
	mEnabled = Enabled;
}

void NxAnimation::SetAnimationType( NxAnimation::AnimationTypes Type )
{
	mType = Type;
}

NxAnimation::AnimationTypes NxAnimation::GetAnimationType()
{
	return mType;
}

void NxAnimation::SetSpeed( float Speed )
{
	mSpeed = Speed;
}

size_t NxAnimation::GetNumKeyframes()
{
	return mKeyFrames.size();
}

void NxAnimation::AddKeyframe( NxKeyframe * Key )
{
	// Insert just before upper bound
	KeyFrameList::iterator i = std::upper_bound(mKeyFrames.begin(), mKeyFrames.end(), Key, KeyFrameTimeLess());
	mKeyFrames.insert(i, Key);
}

void NxAnimation::DeleteKeyframes(){
	for( KeyFrameList::reverse_iterator It = mKeyFrames.rbegin();It != mKeyFrames.rend(); ++It){
		delete *It;
	}
	mKeyFrames.clear();
}

 
// events
 
void NxAnimation::AddEvent( NxEvent * Event )
{
	EventList::iterator i = std::upper_bound(mEvents.begin(), mEvents.end(), Event, KeyFrameTimeLess());
	mEvents.insert(i, Event);
}

void NxAnimation::DeleteEvents(){
	for( EventList::reverse_iterator It = mEvents.rbegin();It != mEvents.rend(); ++It){
		delete *It;
	}
	mEvents.clear();
	mTotalEventElapsedTime = 0;
}






}