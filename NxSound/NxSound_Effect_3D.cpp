#include "NxSoundPreCompiled.h"

#ifdef USE_OPENAL

#include "../nxdeps/include/AL/al.h" 
#include "../nxdeps/include/AL/alc.h" 
#include "../nxdeps/include/AL/alext.h"  

/* Notes 
OPENAL uses a right handed coordinate system, 
- where in a frontal default view X (thumb) points right, 
- Y points up (index finger), 
- and Z points towards the viewer/camera (middle finger). 

!To switch from a left handed coordinate system, flip the sign on the Z coordinate.
- Listener position is always in the world coordinate system.
*/

// we only provide a simple interface in mono 32 bits sample buffers.
// we dont use stereo spatialization, as it given from the multichannel mixer.


namespace NxSound_Namespace {

#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_WIN32
CRITICAL_SECTION criticalSection;
#endif
void check()
{
	int error = alGetError();
    switch(error)
    {
        case AL_NO_ERROR:
            //Log("AL_NO_ERROR");
        break;
        case AL_INVALID_NAME:
            Log("AL_INVALID_NAME");
        break;
        case AL_INVALID_ENUM:
            Log("AL_INVALID_ENUM");
        break;
        case AL_INVALID_VALUE:
            Log("AL_INVALID_VALUE");
        break;
        case AL_INVALID_OPERATION:
            Log("AL_INVALID_OPERATION");
        break;
        case AL_OUT_OF_MEMORY:
            Log("AL_OUT_OF_MEMORY");
        break;
    };
}

NxSoundEffect3D::NxSoundEffect3D()
{
	mType = Effect_3D ;


#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_WIN32
	InitializeCriticalSection(&criticalSection);
	EnterCriticalSection(&criticalSection);
#endif


	mStreamFormat = AL_FORMAT_MONO_FLOAT32;
    alGenBuffers(NUMBUFFERS, mBuffers);
    check();
	mSource = 0 ;
    alGenSources(1, &mSource);
    check();
	int channels = 1;
 
	// Fill all the Buffers with dummy audio data
	for (int iLoop = 0; iLoop < NUMBUFFERS; iLoop++)
	{
		float * data = new float[SND_BUFFER_SIZE*channels];
		memset( &data[0], 0, SND_BUFFER_SIZE * sizeof(float) *channels);
		alBufferData(mBuffers[iLoop], mStreamFormat, &data[0], SND_BUFFER_SIZE * sizeof(float)*channels, 44100 );			 
	}

	alSourceQueueBuffers(mSource, NUMBUFFERS, mBuffers );
    alSource3f(mSource, AL_POSITION, 0.0, 0.0, 0.0);
    alSource3f(mSource, AL_VELOCITY, 0.0, 0.0, 0.0);
    alSource3f(mSource, AL_DIRECTION, 0.0, 0.0, 1.0);
	// Sound Cones - refer to spotlight + falloff kind.
	alSourcef (mSource, AL_CONE_INNER_ANGLE, 360 ); // inner cone angle, in degrees.
	alSourcef (mSource, AL_CONE_OUTER_ANGLE, 360 ); // outer cone angle, in degrees.
	alSourcef (mSource, AL_CONE_OUTER_GAIN, 1.0 ); // outer angle gain.
	alSourcef( mSource, AL_REFERENCE_DISTANCE, 1.0f); 
	alSourcef( mSource, AL_MAX_DISTANCE, 4.0f);
	alSourcef (mSource, AL_MIN_GAIN, 0);
	alSourcef (mSource, AL_MAX_GAIN, 1);
    alSourcef (mSource, AL_ROLLOFF_FACTOR, 1.0 );
	//alSourcei (source, AL_SOURCE_RELATIVE, AL_FALSE ); walk, speak, sounds are relative to listener.
 
	alSourcePlay(mSource);
	check();

#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_WIN32
	LeaveCriticalSection(&criticalSection);
#endif

	 
}

NxSoundEffect3D::~NxSoundEffect3D()
{
	alDeleteBuffers( NUMBUFFERS, mBuffers );
}

void NxSoundEffect3D::SetRelative( bool Relative )
{
	alSourcei(mSource,AL_SOURCE_RELATIVE,Relative); 
}

bool NxSoundEffect3D::GetRelative()
{
	ALint Relative;
	alGetSourcei(mSource,AL_SOURCE_RELATIVE,&Relative);
	return Relative;
}

void NxSoundEffect3D::SetPosition( float X, float Y, float Z)
{
	alSource3f(mSource, AL_POSITION, X ,Y ,Z );
}

void NxSoundEffect3D::GetPosition( float &X, float &Y, float &Z )
{
    float vec[3];
    alGetSourcefv(mSource, AL_POSITION, vec);
    X = vec[0]; Y = vec[1]; Z = vec[2];
}

void NxSoundEffect3D::SetDirection( float X, float Y, float Z)
{
	alSource3f(mSource, AL_DIRECTION, X ,Y ,Z );
}

void NxSoundEffect3D::GetDirection( float &X, float &Y, float &Z )
{
    float vec[3];
    alGetSourcefv(mSource, AL_DIRECTION, vec);
    X = vec[0]; Y = vec[1]; Z = vec[2];
}

void NxSoundEffect3D::SetVelocity( float X, float Y, float Z)
{
	/* Calculate Velocity
	Vector3 velocity = (m_vecPosition - m_vecLastPos) / (float)delta;
	m_pSound->setVelocity(velocity.x, velocity.y, velocity.z);
	*/


	alSource3f(mSource, AL_VELOCITY, X ,Y ,Z );
}

void NxSoundEffect3D::GetVelocity( float &X, float &Y, float &Z )
{
    float vec[3];
    alGetSourcefv(mSource, AL_VELOCITY, vec);
    X = vec[0]; Y = vec[1]; Z = vec[2];
}

void NxSoundEffect3D::SetRange( float Min , float Max )
{
	alSourcef( mSource, AL_REFERENCE_DISTANCE, Min );          
	alSourcef( mSource, AL_MAX_DISTANCE, Max);
}

void NxSoundEffect3D::GetRange( float &Min, float &Max )
{
    alGetSourcef(mSource, AL_REFERENCE_DISTANCE, &Min );
    alGetSourcef(mSource, AL_MAX_DISTANCE, &Max );
}

void NxSoundEffect3D::SetSoundCone( float Inner, float Outer, float OuterGain )
{
	alSourcef (mSource, AL_CONE_INNER_ANGLE, Inner ); // inner cone angle, in degrees.
	alSourcef (mSource, AL_CONE_OUTER_ANGLE, Outer ); // outer cone angle, in degrees.
	alSourcef (mSource, AL_CONE_OUTER_GAIN, OuterGain ); // outer angle gain.
}

bool NxSoundEffect3D::playing()
{
    ALenum state;
    alGetSourcei(mSource, AL_SOURCE_STATE, &state);
	check();

	if(state != AL_PLAYING)
	{
		alSourcePlay(mSource);
	}

	alGetSourcei(mSource, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

void NxSoundEffect3D::Process( float ** BufIn , float ** BufOut, int BlockSize )
{
	float * left  = BufIn[0];
	float * right = BufIn[1];

	/* Note :Stereo formats are interleaved,
	so the first sample is for the left speaker, the second is for the right
	Openal doesnt spatialize 3d stereo formats*/

	
#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_WIN32
	EnterCriticalSection(&criticalSection);
#endif


    ALint processed = 0 ;
    bool active = false;
    alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed); 
	check();

	int channels = 1 ; 
	ALsizei  size = SND_BUFFER_SIZE * sizeof(float) * channels ;  

	// while(processed--)
	//{
	if(processed)
	{
        ALuint buffer = 0;
		ALsizei Freq = 44100 ;
		alSourceUnqueueBuffers(mSource, 1, &buffer);
		check();
		alBufferData( buffer, mStreamFormat, left  ,size, Freq );
		check();
		alSourceQueueBuffers(mSource, 1, &buffer);
		check();
	}

	ALenum state;
	alGetSourcei(mSource, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING)
	{
		//isPlaying = true;
	}
	else
	{
		ALint iQueuedBuffers;
		alGetError();
		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);// nb buffers queued

		if(alGetError() != AL_NO_ERROR)
		{
			Log( "NxSound : 3D sound error" );
		}

		if (iQueuedBuffers)
		{
			alSourcePlay(mSource);	 
		} 
	}

#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_WIN32
	LeaveCriticalSection(&criticalSection);
#endif
	
}

void NxSoundEffect3D::empty()
{
    int queued;
    
    alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
    
    while(queued--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(mSource, 1, &buffer);
        check();
    }
}

}//namespace

#endif