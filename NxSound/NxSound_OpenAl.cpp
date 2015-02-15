#include "NxSoundPreCompiled.h"
#ifdef USE_OPENAL
#include "../nxdeps/include/AL/al.h" 
#include "../nxdeps/include/AL/alc.h" 
#include "../nxdeps/include/AL/alext.h"  
#endif
namespace NxSound_Namespace {

#ifdef USE_OPENAL
static void checkForErrors(void)
{
    {
        ALCdevice *device = alcGetContextsDevice(alcGetCurrentContext());
        ALCenum error = alcGetError(device);
        if(error != ALC_NO_ERROR)
		{
            //die("ALC", (const char*)alcGetString(device, error));
			Log( "OpenAl error " + std::string(  (const char*)alcGetString(device, error) )  );
		}
    }
    {
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
		{
			Log( "OpenAl error " + std::string(  (const char*)alGetString(error) )  );
            //die("AL", (const char*)alGetString(error));
		}
    }
}

static void printALInfo(void)
{
	Log( "OpenAL vendor string:  " + std::string(  alGetString(AL_VENDOR)  )   );
	Log( "OpenAL renderer string:  " + std::string(  alGetString(AL_RENDERER)  )   );
	Log( "OpenAL version string:  " + std::string(  alGetString(AL_VERSION)  )   );
	// printExtensions("OpenAL extensions", ' ', alGetString(AL_EXTENSIONS));
	checkForErrors();
}

static void printDevices( ALCenum which, const char *kind)
{
    const char *s = alcGetString( NULL, which );
    while(*s != '\0')
    {
		Log( "OpenAL found Device : "+ std::string( kind) +": " + std::string( s ) );
        while(*s++ != '\0')
            ;
    }
}
#endif

bool NxSoundManager::InitializeOpenAL()
{

#ifdef USE_OPENAL

	ALCdevice* myALDevice;
	ALCcontext* myALContext;
	myALDevice = NULL;

	myALDevice = alcOpenDevice( NULL );
	if( myALDevice == NULL )  {
		Log( "alcOpenDevice failed, Sound Spatialization Disabled" );
		return false  ;
	}
 
	const ALCint attr[] =
	{
		ALC_REFRESH, 22050, // How often do we update the mixahead buffer (hz).
		ALC_SYNC, AL_TRUE, // We want an synchronous context.
		ALC_STEREO_SOURCES, 1,
		ALC_MONO_SOURCES, 255,
		0
	};
 
	myALContext = NULL;
	myALContext = alcCreateContext(myALDevice, attr);
	alcMakeContextCurrent(myALContext);

	ALCint MaxMonoSources;
	alcGetIntegerv(myALDevice, ALC_MONO_SOURCES, 1, &MaxMonoSources);

	ALCint MaxStereoSources;
	alcGetIntegerv(myALDevice, ALC_STEREO_SOURCES, 1, &MaxStereoSources);

	ALCint Frequency;
	alcGetIntegerv(myALDevice, ALC_FREQUENCY, 1, &Frequency);

	ALCint Refresh;
	alcGetIntegerv(myALDevice, ALC_REFRESH, 1, &Refresh);

	ALCint Sync;
	alcGetIntegerv(myALDevice, ALC_SYNC, 1, &Sync);
    
	Log( "OpenAL Max Mono Sources : " + NxSoundUtils::ToString( MaxMonoSources ) );
	Log( "OpenAL Max Stereo Sources : " + NxSoundUtils::ToString( MaxStereoSources ) );
	Log( "OpenAL Device Frequency : " + NxSoundUtils::ToString( Frequency ) );
	Log( "OpenAL Device Refresh rate : " + NxSoundUtils::ToString( Refresh ) );

	/* crashing on adelin computer
    if( alcIsExtensionPresent(NULL, (const ALCchar*)"ALC_ENUMERATION_EXT") == AL_TRUE ) {
        if( alcIsExtensionPresent(NULL, (const ALCchar*)"ALC_ENUMERATE_ALL_EXT") == AL_TRUE ) {
            printDevices(ALC_ALL_DEVICES_SPECIFIER, "playback ");
		}
        else {
            printDevices(ALC_DEVICE_SPECIFIER, "playback ");
		}
        printDevices(ALC_CAPTURE_DEVICE_SPECIFIER, "capture ");
    }
	else { Log("No device enumeration available"); }
	*/

	Log( "OpenAL print devices : DONE " );

	const ALenum xRamAuto = alGetEnumValue("AL_STORAGE_AUTO");
	const ALenum xRamHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
	const ALenum xRamAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");


	Log( "OpenAL checking extension : EAX2.0" );
	ALboolean isEAXPresent = alIsExtensionPresent( "EAX2.0" );
	if ( isEAXPresent ) {
	  Log( "OpenAl EAX 2.0 Extension available" );
	}

	Log( "OpenAL checking extension : EAX2.0 : DONE" );

	if( alIsExtensionPresent("AL_EXT_float32") != AL_TRUE ) {
		Log( "OpenAl AL_EXT_float32 not supported" ); return false;
	}

	alListener3f(AL_POSITION,0.0,0.0,0.0);
	alListener3f(AL_VELOCITY,0.0,0.0,0.0);

	ALfloat orientation[6];
    orientation[0] = 0.0;
    orientation[1] = 0.0;
    orientation[2] = -1.0;
    orientation[3] = 0.0;
    orientation[4] = 1.0;
    orientation[5] = 0.0;
	alListenerfv( AL_ORIENTATION, orientation );

	// Initialise Doppler

	Log( "OpenAL creating doppler..." );

	alDopplerFactor( 1.0 ); // 1.2 = exaggerate the pitch shift by 20%
	alDopplerVelocity( 343.0f ); // m/s this may need to be scaled at some point

	Log( "OpenAL creating doppler : DONE" );


	alListenerf (AL_GAIN, 1.0);
	alDistanceModel( AL_LINEAR_DISTANCE_CLAMPED );//AL_LINEAR_DISTANCE ); //AL_LINEAR_DISTANCE_CLAMPED



//#define AL_DISTANCE_MODEL                         0xD000
//#define AL_INVERSE_DISTANCE                       0xD001
//#define AL_INVERSE_DISTANCE_CLAMPED               0xD002
//#define AL_LINEAR_DISTANCE                        0xD003
//#define AL_LINEAR_DISTANCE_CLAMPED                0xD004
//#define AL_EXPONENT_DISTANCE                      0xD005
//#define AL_EXPONENT_DISTANCE_CLAMPED              0xD006


	UseOpenAL = true;
	Log( "OpenAL Sound Spatialization Enabled");
	return true;

#endif
	return false;
}

void NxSoundManager::SetListenerPosition( float X , float Y, float Z ) {
	#ifdef USE_OPENAL
	if(!UseOpenAL) return ;
	alListener3f(AL_POSITION, X, Y, Z );
	#endif
}

void NxSoundManager::SetListenerOrientation( float X , float Y, float Z ) {
	#ifdef USE_OPENAL
	if(!UseOpenAL) return ;
	ALfloat dir[] = { X, Y, Z }; //{ X, Y, Z, 0, 1, 0 };
	alListenerfv( AL_ORIENTATION, dir );
	#endif
}

}