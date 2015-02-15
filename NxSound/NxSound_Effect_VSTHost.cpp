#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"

namespace NxSound_Namespace {

	template<> NxSoundEffectVSTHost* Singleton<NxSoundEffectVSTHost>::msSingleton = 0;


	VstTimeInfo * vstTimeInfo;
	std::list< NxSoundEffectVSTPlugin *  > VstPlugins ;
	int kBlockSize ; 
	float kSampleRate ; 
	
	
	VstTimeInfo * OnGetTime( long lMask ){ return vstTimeInfo; }
	bool OnSetTime(  long filter, VstTimeInfo * timeInfo ) { return false; }


	

NxSoundEffectVSTHost::NxSoundEffectVSTHost()
{
	vstTimeInfo = new VstTimeInfo();
	float GeneralSampleRate = 44100.0f ;          
	vstTimeInfo->samplePos = 0.0;
	vstTimeInfo->sampleRate = GeneralSampleRate;
	vstTimeInfo->nanoSeconds = 0.0;
	vstTimeInfo->ppqPos = 0.0;
	vstTimeInfo->tempo = 120.0;
	vstTimeInfo->barStartPos = 0.0;
	vstTimeInfo->cycleStartPos = 0.0;
	vstTimeInfo->cycleEndPos = 0.0;
	vstTimeInfo->timeSigNumerator = 4;
	vstTimeInfo->timeSigDenominator = 4;
	vstTimeInfo->smpteOffset = 0;
	vstTimeInfo->smpteFrameRate = 1;
	vstTimeInfo->samplesToNextClock = 0;
	vstTimeInfo->flags = 0;
}

NxSoundEffectVSTPlugin * NxSoundEffectVSTHost::LoadVstPlugin( const std::string & Path )
{
	
	NxSoundEffectVSTPlugin * Plugin = new NxSoundEffectVSTPlugin( this );	

	Log( "Loading Vst Plugin : " + Path  );
	if (!Plugin->Load( Path.c_str() ))              
	{
		Log( "Could Not Load Vst Plugin" );
		delete Plugin;  
		Plugin = NULL ;
		return 0;                            
	}

	Log( "Loading Vst Plugin Loaded" );
	Log( "Loading parameters..." );
	Plugin->SetupParameters();
	Log( "Loading parameters : Done" );
	VstPlugins.push_back( Plugin );

	return Plugin ;
}

bool NxSoundEffectVSTHost::UnloadVstPlugin( NxSoundEffectVSTPlugin * Plugin )
{
	VstPlugins.remove( Plugin );
	if(!Plugin->Unload()){ Log( "NxSound : Could not unload VST plugin" ); return false;  }
	return true ;
}

NxSoundEffectVSTHost& NxSoundEffectVSTHost::getSingleton(void)
{
	assert( msSingleton );  return ( *msSingleton );
}

NxSoundEffectVSTPlugin * NxSoundEffectVSTHost::GetVSTPluginFromEffect( AEffect * effect  )  
{
	NxSoundEffectVSTPlugin * NxPlugin = 0 ;
	for( std::list< NxSoundEffectVSTPlugin *  >::iterator iter = VstPlugins.begin(); iter != VstPlugins.end(); iter++)
	{
		NxSoundEffectVSTPlugin  * VSTPlugin = *iter; 
		if ( VSTPlugin->GetEffect() == effect)
		{
			NxPlugin = VSTPlugin ;
			break;
		}
	}
	return NxPlugin ;
}


void NxSoundEffectVSTHost::SetTime( double Nanoseconds )
{
	vstTimeInfo->nanoSeconds = Nanoseconds;
	CalcTimeInfo();

	vstTimeInfo->samplePos += (float)SND_BUFFER_SIZE;
	vstTimeInfo->flags &= ~kVstTransportChanged;

}

void NxSoundEffectVSTHost::CalcTimeInfo(long lMask)
{
	// we don't care for the mask in here
	static double fSmpteDiv[] =
	{
		24.f,
		25.f,
		24.f,
		30.f,
		29.97f,
		30.f
	};

	double dPos = vstTimeInfo->samplePos / vstTimeInfo->sampleRate;
	vstTimeInfo->ppqPos = dPos * vstTimeInfo->tempo / 60.L;								 
	double dOffsetInSecond = dPos - floor(dPos);
	vstTimeInfo->smpteOffset = (long)(dOffsetInSecond * fSmpteDiv[vstTimeInfo->smpteFrameRate] * 80.L);

}

long VSTCALLBACK NxSoundEffectVSTHost::AudioMasterCallback( AEffect *effect, long opcode, long index, long value, void *ptr, float opt )
{
	NxSoundEffectVSTPlugin * nEffect = NxSoundEffectVSTHost::getSingleton().GetVSTPluginFromEffect( effect );
	VstIntPtr result = 0;

	// Filter idle calls...
	bool filtered = false;
	if (opcode == audioMasterIdle)
	{
		static bool wasIdle = false;
		if (wasIdle)
			filtered = true;
		else
		{
			printf ("(Future idle calls will not be displayed!)\n");
			wasIdle = true;
		}
	}

	if (!filtered)
		//printf ("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void> (value), ptr, opt);

	switch (opcode)
	{
		case audioMasterVersion :
		result = kVstVersion;
		break;
		case audioMasterCurrentId:
        break;
		case audioMasterGetNumAutomatableParameters:
        return 0;
		case audioMasterGetAutomationState:
        return 1;
		case audioMasterGetVendorVersion:
        return 0x0101;
		case audioMasterGetVendorString:
		case audioMasterGetProductString:
        break;
		case audioMasterGetSampleRate:
        return 44100;
		case audioMasterGetBlockSize:
		NxSoundLog::getSingleton().LogMessage( "NxSound : Host requested audioMasterGetBlockSize..." );
        return SND_BUFFER_SIZE;
		case audioMasterSetOutputSampleRate:
        return 0;
		case audioMasterGetTime:
        #ifdef _MSC_VER
        #pragma warning (push)
        #pragma warning (disable: 4311)
        #endif
		return (VstIntPtr) & vstTimeInfo;
        #ifdef _MSC_VER
        #pragma warning (pop)
        #endif
        break;

    default:
		//NxSoundLog::getSingleton().LogMessage( "NxSound : *** Unhandled VST Callback ***" );
        break;
	}

	return result;
  
}

}//namespace