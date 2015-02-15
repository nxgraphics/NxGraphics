#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxKeyframe::NxKeyframe( float Time, float Value ) : mTime( Time ), mValue( Value )
{

}

NxKeyframe::~NxKeyframe()
{

}

void NxKeyframe::SetTime( float Time )
{
	mTime = Time;
}

float NxKeyframe::GetValue() const
{
	return mValue;
}

void NxKeyframe::SetValue( float Val )
{
	mValue = Val;
}

}