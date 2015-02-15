#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"

#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_APPLE
#include <Carbon/Carbon.h>
#endif

namespace NxSound_Namespace {

class NxSoundVstModule
{
public :
	#if NXSOUND_PLATFORM == NXSOUND_PLATFORM_WIN32

	NxSoundVstModule() : mEffect(NULL), hModule(NULL) {

	}

	bool OpenModule( const std::string & FilePath ) {

		AEffect *(*pMain)(long (*audioMaster)(AEffect *effect, long opcode, long index, long value, void *ptr, float opt)) = 0;

		try {
			hModule = ::LoadLibrary( FilePath.c_str() );          
		}
		catch(...) {
			hModule = NULL;
			return false;
		}

		if (hModule) {
			pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))::GetProcAddress(hModule, "VSTPluginMain");
			if (!pMain){ pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))::GetProcAddress(hModule, "main"); }
			if (!pMain)  Log("Could not find valid vst dll entry point, tried VSTPluginMain and main");
		}

		if (pMain) {
			try {
				mEffect = pMain(  NxSoundEffectVSTHost::getSingleton().AudioMasterCallback );
			}
			catch(...) {
				mEffect = NULL;
				return false;
			}
		}

		return true;
	}

	bool CloseModule() {

		if( hModule ) {
			::FreeLibrary(hModule);                
			hModule = NULL;
		}
		return !hModule;
	}

	HMODULE hModule;


	#elif NXSOUND_PLATFORM == NXSOUND_PLATFORM_ANDROID

	NxSoundVstModule() : mEffect(NULL)  {

	}

	bool OpenModule( const std::string & FilePath ) {

		 

		return true;
	}

	bool CloseModule() {

		return false;
	}


	#else // Machintosh



	NxSoundVstModule() : mEffect(NULL),  resHandle(0), bundleRef(0), resFileId(0)
	{

	}

	bool OpenModule( const std::string & FilePath )
	{
		AEffect *(*pMain)(long (*audioMaster)(AEffect *effect, long opcode, long index, long value, void *ptr, float opt)) = 0;
        
        bool ok = false;
		std::string filename = FilePath;
		NxSoundUtils::ToLower( filename );

		if(filename.substr(filename.find_last_of(".") + 1) == "vst")  {

			const char* const utf8 = filename.c_str();
			CFURLRef url = CFURLCreateFromFileSystemRepresentation (0, (const UInt8*) utf8,strlen (utf8), false );

			if (url != 0) {
				bundleRef = CFBundleCreate (kCFAllocatorDefault, url);
				CFRelease (url);

				if (bundleRef != 0) {
					if (CFBundleLoadExecutable (bundleRef)) {

                        pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))CFBundleGetFunctionPointerForName ((CFBundleRef)bundleRef, CFSTR("VSTPluginMain"));
						
						if (pMain == 0)
							pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))CFBundleGetFunctionPointerForName (bundleRef, CFSTR("main_macho"));

						if (pMain != 0) {

							try {
								mEffect = pMain(  NxSoundEffectVSTHost::getSingleton().AudioMasterCallback );
							}
							catch(...) {
								mEffect = NULL;
								return false;
							}

							CFTypeRef name = CFBundleGetValueForInfoDictionaryKey (bundleRef, CFSTR("CFBundleName"));

							if (name != 0) {
								if (CFGetTypeID (name) == CFStringGetTypeID()) {
									char buffer[1024];
									//if (CFStringGetCString ((CFStringRef) name, buffer, sizeof (buffer), CFStringGetSystemEncoding()))
										//pluginName = buffer;
								}
							}

							//if (pluginName.isEmpty()) pluginName = "NoNamePlugin";

							resFileId = CFBundleOpenBundleResourceMap (bundleRef);
							ok = true;
						}
					}

					if (! ok) {
						CFBundleUnloadExecutable (bundleRef);
						CFRelease (bundleRef);
						bundleRef = 0;
					}
				}
			}
		}

		return ok;
	}

	bool CloseModule() 
	{

		return true;
	}

	Handle resHandle;
	CFBundleRef bundleRef;
	FSSpec parentDirFSSpec;
	short resFileId;

	#endif	


	AEffect * GetEffect() { return mEffect; }

private :
	AEffect * mEffect;



};

NxSoundEffectVSTPlugin::NxSoundEffectVSTPlugin( NxSoundEffectVSTHost  * pHost ) : pEffect(0), NumIn(0), NumOut(0), VstWantsMidi(false),
VstCanReceiveEvents(false), VstCanMidiProgramNames(false) , VstHasEditor(false) , VstIsSynth(false), mHost( pHost )
{
	mType = Effect_VST_Effect ;
	mModule = new NxSoundVstModule();
	sDir = NULL;
}

NxSoundEffectVSTPlugin::~NxSoundEffectVSTPlugin()
{
	Log( "NxSound : Deleting VST Plugin" + VstModuleName + "\r\n" );
	Unload();
}

bool NxSoundEffectVSTPlugin::Load(const char *name)
{
	if (!mHost) return false;                         

	Unload();
	if( !mModule->OpenModule( name )) return false;
	pEffect = mModule->GetEffect();
	if(!pEffect) return false;
	if (pEffect && (pEffect->magic != kEffectMagic)) { // vst plugin ?
        Log("vst magic incorrect...");
		pEffect = NULL;
		return false ;
	}
    // Create window if any editor.
	mWindow = new NxSoundEffectVSTWindow( this );

	return true;
}
 
bool NxSoundEffectVSTPlugin::Unload()
{
	if (!mHost){ return false; } 
	if (!pEffect ){ return false ; }
	if(pEffect){ 
		//pEffect->dispatcher(pEffect, effMainsChanged, 0, 0, (void *)0, 0.0f); 
		Log( "NxSoundEffectVSTPlugin::Unload() : effStopProcess" );
		int res = pEffect->dispatcher(pEffect,effStopProcess, 0, 0, NULL, 0.0);
		Log( "NxSoundEffectVSTPlugin::Unload() : effMainsChanged" );
		pEffect->dispatcher(pEffect, effMainsChanged, 0, 0, NULL, 0);
	}//pause

	if(pEffect){
		Log( "NxSoundEffectVSTPlugin::Unload() : effClose" );
		pEffect->dispatcher (pEffect, effClose, 0, 0, NULL, 0.0); 
		Log( "NxSoundEffectVSTPlugin::Unload() : effClose Done" );
	}

	pEffect = NULL; 

	mModule->CloseModule();

	return true;
}

AEffect * NxSoundEffectVSTPlugin::GetEffect()
{
	if(!pEffect){ return 0; }
	return pEffect;
}

VstInt32 NxSoundEffectVSTPlugin::GetNumInputs()
{
	if(!pEffect){ return 0; }
	return NumIn ;
}

VstInt32 NxSoundEffectVSTPlugin::GetNumOutputs()
{
	if(!pEffect){ return 0; }
	return NumOut ;
}

VstInt32 NxSoundEffectVSTPlugin::GetNumPrograms()
{
	return NumPrograms ;
}

VstInt32 NxSoundEffectVSTPlugin::GetNumParams()
{
	return NumParams ;
}

VstInt32 NxSoundEffectVSTPlugin::GetUniqueID()
{
	return VstuniqueID;		 
}

VstInt32 NxSoundEffectVSTPlugin::GetVersion()
{
	return Vstversion;
}

bool NxSoundEffectVSTPlugin::PropertiesWantsMidi()
{
	return VstWantsMidi ;
}

bool NxSoundEffectVSTPlugin::PropertiesCanReceiveEvents()
{
	return VstCanReceiveEvents ;
}

bool NxSoundEffectVSTPlugin::PropertiesCanMidiProgramNames()
{
	return VstCanMidiProgramNames;
}

bool NxSoundEffectVSTPlugin::PropertiesHasEditor()
{
	return 	VstHasEditor ;
}

bool NxSoundEffectVSTPlugin::PropertiesIsSynth()
{
	return VstIsSynth ;
}

void NxSoundEffectVSTPlugin::GetModuleName( std::string & ModuleName )
{
	ModuleName = VstModuleName;
}

void NxSoundEffectVSTPlugin::GetModuleVendorName( std::string & ModuleVendorName )
{
	ModuleVendorName = VstModuleVendorName ;
}

void NxSoundEffectVSTPlugin::GetModuleProductName( std::string & ModuleProductName )
{
	ModuleProductName = VstModuleProductName ;
}

VstInt32 NxSoundEffectVSTPlugin::GetBlockSize()
{
	return VstBlockSize ;
}

unsigned int NxSoundEffectVSTPlugin::GetResolution()
{
	return NX_FLOAT32; // NxAudioFormat 
}

void NxSoundEffectVSTPlugin::Process( float ** BufIn , float ** BufOut , int BlockSize )
{
	if (( GetEffect()->flags & effFlagsCanReplacing) != 0)
    {
        try {
			GetEffect()->processReplacing( GetEffect(), BufIn, BufOut, GetBlockSize() );
        }
        catch (...) {
			Log("GetEffect()->processReplacing failed for vst effect");
		}

		for (int f = 0; f < GetNumOutputs(); f++) {
			for ( unsigned int i=0; i< GetBlockSize() ; i++ ) { 
				outBufs[f][i] = BufIn[f][i];
			} 
		}
    }
    else {
		// not Vst 2.4 compliant. 
		GetEffect()->process( GetEffect(), BufIn, BufOut, GetBlockSize() );
		Log( "VST non 2.4 Compliant." );
    }
}

void NxSoundEffectVSTPlugin::SetupParameters()
{
	VstBlockSize =  SND_BUFFER_SIZE ;
	VstSampleRate = 44100.f;

    pEffect->dispatcher(pEffect, effOpen, 0, 0, (void *)0, 0.0f);
    pEffect->dispatcher(pEffect, effSetSampleRate, 0, 0, (void *)0, (float)VstSampleRate);
    pEffect->dispatcher(pEffect, effSetBlockSize, 0, VstBlockSize, (void *)0, 0.0f);
    pEffect->dispatcher(pEffect, effMainsChanged, 0, 1, (void *)0, 0.0f);//resume 

	char VsteffectName[256] = {0};
	char VstvendorString[256] = {0};
	char VstproductString[256] = {0};

	pEffect->dispatcher (pEffect, effGetEffectName, 0, 0, VsteffectName, 0);
	pEffect->dispatcher (pEffect, effGetVendorString, 0, 0, VstvendorString, 0);
	pEffect->dispatcher (pEffect, effGetProductString, 0, 0, VstproductString, 0);

	VstModuleName = VsteffectName ;
	VstModuleVendorName = VstvendorString ;
	VstModuleProductName = VstproductString ;

	Log( "VST Host - \r\n Info Name : " +  VstModuleName   +
	" \r\n Vendor name : " +  VstModuleVendorName + " \r\n Product name :  " + VstModuleProductName );

	NumPrograms = pEffect->numPrograms ;
	NumParams = pEffect->numParams ;

	NumIn  = pEffect->numInputs ;
	NumOut = pEffect->numOutputs ;

	Log( "VST Host - \r\n Num programs : " + NxSoundUtils::ToString( NumPrograms ) +
	"\r\n num params : " +   NxSoundUtils::ToString( NumParams )  + 
	"\r\n Num inputs :  " + NxSoundUtils::ToString( NumIn ) +
	"\r\n Num outputs :  " + NxSoundUtils::ToString( NumOut ) );
 
	VstuniqueID = pEffect->uniqueID ;  //< registered unique identifier (register it at Steinberg 3rd party support Web). This is used to identify a plug-in during save+load of preset and project.
	Vstversion = pEffect->version ;    //< plug-in version (example 1100 for version 1.1.0.0)

	if ((pEffect->flags & effFlagsHasEditor) != 0) {
		NxSoundLog::getSingleton().LogMessage( "VST Host - Has an editor" );
		VstHasEditor = true ;
	}
	else {
		Log( "VST Host - Has No editor" );
	}

	if ((pEffect->flags & effFlagsIsSynth) != 0) {
		Log( "VST Host - plugin is a synth \r\n switching to Effect_VST_Instrument" );
		VstIsSynth = true ;
	}
	else {
		Log( "VST Host - plugin is an effect" );
	}

	//connect inputs / outputs
    int i;
	for (i = NumIn; --i >= 0;) { pEffect->dispatcher (pEffect, effConnectInput, i, 1, 0, 0); }

	for (i = NumOut; --i >= 0;){ pEffect->dispatcher (pEffect, effConnectOutput, i, 1, 0, 0); }


	VstWantsMidi = pEffect->dispatcher (pEffect,effCanDo, 0, 0, (void*) "receiveVstMidiEvent", 0) > 0;
	VstCanReceiveEvents = pEffect->dispatcher (pEffect,effCanDo, 0, 0, (void*) "receiveVstEvents", 0) > 0;
	VstCanMidiProgramNames = pEffect->dispatcher (pEffect,effCanDo, 0, 0, (void*) "midiProgramNames", 0) > 0;

	// create VST(i) buffers
	int Buffer_Size =  24000 ; //????
	int  j;                                
	if ( NumIn )                  
	{
		inBufs = new float *[NumIn];
		if (!inBufs)
		{
			Log( "VST Host - Could not create input buffer - no memory " );
			return ; 
		}

		for (i = 0; i < NumIn; i++)
		{
			inBufs[i] = new float[Buffer_Size];
			if (!inBufs[i]){ return ; }
			for (j = 0; j < Buffer_Size; j++){ inBufs[i][j] = 0.f; }
		}

		for (i = 0; i < NumIn; i++){ inBufs[i] = 0; }
		nAllocatedInbufs = NumIn;

		Log( "VST Host - Allocated " + NxSoundUtils::ToString( nAllocatedInbufs )   + " Input Buffers ");

	}//NumIn
	else{ Log( "VST Host - No Input Buffers "); }

	if ( NumOut )                
	{
		int nAlloc;
		if (NumOut < 2){ nAlloc = 2; }
		else{ nAlloc = NumOut; }
		outBufs = new float *[nAlloc];
		if (!outBufs)
		{	
			Log( "NxSound : VST Host - Could not create output buffer - no memory " );
			return ;
		}
                                    
		for (i = 0; i < NumOut; i++)
		{
			outBufs[i] = new float[Buffer_Size];
			if (!outBufs[i]){ return ; }
			for (j = 0; j < Buffer_Size; j++){ outBufs[i][j] = 0.f; }
		}

		for (; i < nAlloc; i++){ outBufs[i] = outBufs[0]; }             
		nAllocatedOutbufs = nAlloc;

		Log( "VST Host - Allocated " + NxSoundUtils::ToString( nAllocatedOutbufs )   + " Output Buffers ");


	}//NumOut
	else{ Log( "VST Host - No Output Buffers "); }


	pEffect->dispatcher (pEffect,  effStartProcess , 0, 0, 0, 0);

}

void NxSoundEffectVSTPlugin::ShowDialog()
{
	mWindow->ShowDialog();
}

void NxSoundEffectVSTPlugin::SetInputBuffer(int nBuf, float *pData = 0)
{
	if (nBuf < pEffect->numInputs)
    // inBufs allocation isn't checked here; this is intentional.
    // We rely on a sensible implementation that doesn't set
    // input buffers before initialization is finished (which
    // would throw out an effect if buffer allocation fails).
	inBufs[nBuf] = pData;
}

float * NxSoundEffectVSTPlugin::GetOutputBuffer(int nBuf)
{
  if (nBuf < ((nAllocatedOutbufs) ? nAllocatedOutbufs : pEffect->numOutputs))
    // outBufs allocation isn't checked here; this is intentional.
    // We rely on a sensible implementation that doesn't set
    // input buffers before initialization is finished (which
    // would throw out an effect if buffer allocation fails).
    return outBufs[nBuf];
  else
    return 0;
}

bool NxSoundEffectVSTPlugin::SetParameter( int parameter, float value )
{
    if( pEffect )  {
      if (( parameter >= 0) && ( parameter <= pEffect->numParams )) {
        pEffect->setParameter(pEffect, parameter, value);
        return true;
      }
      else return false;
    }
    return false;
}

const std::string NxSoundEffectVSTPlugin::GetParameterName( int parameter )
{
    if( pEffect )  {
      if (( parameter >= 0) && ( parameter <= pEffect->numParams )) {
		char name[64];
		pEffect->dispatcher( pEffect, effGetParamName, parameter, 0, (void *) name, 0.0f);
		return std::string( name );
      }
    }		
	return std::string("Default Label");
}

float NxSoundEffectVSTPlugin::GetParameterValue( int parameter )
{
    if (pEffect) {
      if (( parameter >= 0) && ( parameter <= pEffect->numParams )) {
        return pEffect->getParameter( pEffect, parameter );
      }
    }
    return 0.0f;
}

bool NxSoundEffectVSTPlugin::SetParameter( int parameter, int value )
{
    //Debug("VSTPlugin::SetParameter(%d, %d).\n", parameter, value);
    return SetParameter(parameter,value/65535.0f);
}

void NxSoundEffectVSTPlugin::GetProgramNames( std::vector<string> & ProgramNames )
{
	if (!pEffect){ return;} 
	for (VstInt32 progIndex = 0; progIndex < pEffect->numPrograms; progIndex++)
	{
		char progName[256] = {0};
		if (!pEffect->dispatcher (pEffect, effGetProgramNameIndexed, progIndex, 0, progName, 0))
		{
			pEffect->dispatcher (pEffect, effSetProgram, 0, progIndex, 0, 0); // Note: old program not restored here!
			pEffect->dispatcher (pEffect, effGetProgramName, 0, 0, progName, 0);
		}
		ProgramNames.push_back(progName);
	}
}

unsigned int NxSoundEffectVSTPlugin::GetNumParameters()
{
	if(!pEffect){ return 0;}
	return pEffect->numParams ;
}

void NxSoundEffectVSTPlugin::GetParametersProperties( std::vector<VstParameterProperties *> & ParameterProperties )
{
	if (!pEffect){ return;} 
	for (VstInt32 i = 0; i < pEffect->numParams ; i++)
	{
		VstParameterProperties  * paramProps = new VstParameterProperties();
		if( pEffect->dispatcher( pEffect, effGetParameterProperties, i, 0, paramProps , 0) )//&& paramProps->flags & kVstParameterUsesIntegerMinMax
		{
			//mParamInfo[i].minValue = paramProps.minInteger;
			//mParamInfo[i].valueScale = 1.0 / (paramProps.maxInteger - paramProps.minInteger);
		}
		else
		{
			//mParamInfo[i].minValue = 0.0;
			//mParamInfo[i].valueScale = 1.0;
		}
		ParameterProperties.push_back(paramProps);
	}
}

void NxSoundEffectVSTPlugin::SetProgram( VstInt32 ProgramIndex )
{
	if (!pEffect){ return ;} 
	pEffect->dispatcher (pEffect, effSetProgram, 0, ProgramIndex, 0, 0);  
}

void NxSoundEffectVSTPlugin::ProcessEvents( VstEvents* Events )
{
	if (!pEffect){ return ;} 
	assert(Events != NULL);
	int res = pEffect->dispatcher(pEffect,effProcessEvents, 0, 0, Events, 0.0f);
    //"vst name:%s num events:%d  result:%d \n"),name().c_str(),p->numEvents,res);
}

 
}//namespace