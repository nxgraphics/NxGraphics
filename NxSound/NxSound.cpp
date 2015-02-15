#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake android ?
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"
#include "STK/include/Stk.h" 
#include "STK/include/RtAudio.h"

namespace NxSound_Namespace 
{

template<> NxSoundManager* Singleton<NxSoundManager>::msSingleton = 0;

NxSoundManager::NxSoundManager( const NxSoundApi NxApiType ) : DefaultOutputDeviceIndex (-1), DefaultInputDeviceIndex(-1), UseOpenAL(false)
{
	new NxSoundLog();
	Log("Started NxSound.");

	m_NxSoundApi = NxApiType;
	stk::Stk::setSampleRate( 44100.0 );	
	//set path for stk instruments.
	stk::Stk::setRawwavePath( "../../NxMedia/Audio/RAW/" );

	Log( "NxSound : Initializing OpenAl..." );


	//initialize openal for 3d spatialization
	InitializeOpenAL() ? Log("NxSound : OpenAL Initialization Enabled.") : Log("NxSound : OpenAL Initialization Failed!");

	Log( "NxSound : Initializing OpenAl : DONE" );


	Log( "NxSound : Initializing VstHost..." );
	//create our main and only VST host
	NxSoundEffectVSTHost * VSTHost = new NxSoundEffectVSTHost();
	Log( "NxSound : Initializing VstHost : DONE" );

	Log( "NxSound : Initializing RtAudio..." );
	dac = NULL ;
	dac = new RtAudio( (RtAudio::Api) m_NxSoundApi ); 
	 
	if( !dac ) {	
		Log( "NxSound : Could not initialize Api Type !" );
		return ;
	}else {
		Log( "NxSound : Initialized" );
	}

 	int devices = dac->getDeviceCount();

	Log( "NxSound : Found num devices : " + NxSoundUtils::ToString( devices ) );

	RtAudio::DeviceInfo info;
	NxDeviceInfo * DeviceInfo;
	bool Output = false ;

	Log( "NxSound : Enumerating devices.. ");

	for (int i=0; i< devices; i++) {
		try {
			info = dac->getDeviceInfo(i);
			DeviceInfo = new NxDeviceInfo();
			DeviceInfo->name = info.name;
			DeviceInfo->isDefaultInput = info.isDefaultInput;
			DeviceInfo->isDefaultOutput = info.isDefaultOutput;
			DeviceInfo->inputChannels = info.inputChannels;
			DeviceInfo->outputChannels = info.outputChannels;	
		} 
		catch (RtError &error) {
			error.printMessage();
			break;
		}

		if( DeviceInfo->isDefaultOutput )// default output
		{
			Output = true ;
			DefaultOutputDeviceIndex = i ;
			Log( "Default Output Audio device : " + DeviceInfo->name );
			SoundCardDeviceList.push_back( *DeviceInfo );
			SoundCardOutputDeviceList.push_back( *DeviceInfo );
		}
		else if( DeviceInfo->isDefaultInput ) { // default input
			Output = false ;
			DefaultInputDeviceIndex = i ;
			Log( "Default Input Audio device : " +  DeviceInfo->name    );
			SoundCardDeviceList.push_back( *DeviceInfo );
			SoundCardInputDeviceList.push_back( *DeviceInfo );
		}
		else {
			if( Output ) { // output
				SoundCardDeviceList.push_back( *DeviceInfo );
				SoundCardOutputDeviceList.push_back( *DeviceInfo );
				Log( "Output Audio device " + DeviceInfo->name );
			}
			else { // input
				SoundCardDeviceList.push_back( *DeviceInfo );
				SoundCardInputDeviceList.push_back( *DeviceInfo );
				Log( "Input Audio device " + DeviceInfo->name );
			}
		}
	}

		Log( "NxSound : Enumerating devices : DONE");

	DefaultInputDeviceIndex = dac->getDefaultInputDevice();

	delete dac ;
	return  ;
}

NxSoundManager::~NxSoundManager()
{
	Log("Deleting NxSound Manager...");
	DeleteAllDeviceOutputs();
	DeleteAllDeviceInputs();
	Log("Deleting NxSound Manager : Done");

}

NxSoundManager& NxSoundManager::getSingleton()
{
	assert( msSingleton );  return ( *msSingleton );
}

NxSoundManager* NxSoundManager::getSingletonPtr()
{
	return msSingleton;
}

const NxSoundApi NxSoundManager::GetNxSoundApiType()
{
	return m_NxSoundApi ;
}

void NxSoundManager::SetPluginBasePathVST( const std::string VstPath )
{
	PluginBasePathVst = VstPath ;
}

std::string NxSoundManager::GetPluginBasePathVST()
{
	return PluginBasePathVst ;
}



/*
//VST default instrument and effects
std::list< NxSoundPluginDescription  * > NxSoundManager::GetAvailableDefaultVSTPluginList( bool ScanSubFolders )
{
	Log( "NxSound : GetAvailableDefaultVSTPluginList started." );
	std::list< NxSoundPluginDescription  * > Desc =  GetAvailableVSTPluginList( "C:\\Program Files/VstPlugins" , ScanSubFolders ); // windows specific
	Log( "NxSound : GetAvailableDefaultVSTPluginList ended." );
	return Desc ;
}

//VST instrument and effects
std::list< NxSoundPluginDescription * > NxSoundManager::GetAvailableVSTPluginList( std::string Path , bool ScanSubFolders )
{
	std::list< NxSoundPluginDescription  * > VSTList ;
	int result = 0;
	std::vector<std::string> VSTDllList;
	result = SearchDirectory(VSTDllList, Path , "dll", ScanSubFolders );
	if(!result)// did find plugins
	{
		//Print results
		for(std::vector<std::string>::iterator iterVstDll = VSTDllList.begin(); iterVstDll != VSTDllList.end(); ++iterVstDll)
		{
			std::string VstDllName  = *iterVstDll  ;
			char * plugPath ={ (char *) (LPCTSTR) VstDllName.c_str() }; 

			Log( "NxSound : Found VST Plugin : "  + VstDllName );

			NxSoundEffectVSTPlugin * plug = NxSoundEffectVSTHost::getSingleton().LoadVstPlugin( VstDllName );
			if( plug )// check compatibility
			{
				Log( "NxSound : VST Plugin Name : " + VstDllName + " Loaded." );

				if( plug->GetEffect() )
				{
					NxSoundPluginDescription * VstDesc = new NxSoundPluginDescription();
					plug->GetModuleName(VstDesc->Name);
					VstDesc->Type = ( plug->PropertiesIsSynth() ? Effect_VST_Instrument : Effect_VST_Effect );  
					VstDesc->Path = std::string( plugPath ) ;
					VSTList.push_back( VstDesc );
				}
				else
				{
					Log( "NxSound : Plugin doesnt have instance");
				}

				Log( "NxSound : VST Plugin Name : " + VstDllName + " Unloading..." );
				if(!NxSoundEffectVSTHost::getSingleton().UnloadVstPlugin( plug )){ Log( "NxSound : VST Plugin Name : " + VstDllName + " Could not unload" );  }
				Log( "NxSound : VST Plugin Name : " + VstDllName + " Unloading Done" );
				delete plug;
			}
			
		}

	}else
	{ 
		Log( "NxSound : Didnt find any VST Plugins");
	}
      
	return VSTList ;
}
*/

unsigned int NxSoundManager::GetDefaultInputDeviceIndex()
{
	return DefaultInputDeviceIndex ;
}

unsigned int NxSoundManager::GetDefaultOutputDeviceIndex()
{
	return DefaultOutputDeviceIndex;
}

NxSoundManager::NxAudioDeviceIterator NxSoundManager::GetAudioOutputIterator()
{
	return NxSoundManager::NxAudioDeviceIterator( SoundCardOutputDeviceList.begin(), SoundCardOutputDeviceList.end());
}
 
NxSoundManager::NxAudioDeviceIterator NxSoundManager::GetAudioInputIterator()
{
	return NxSoundManager::NxAudioDeviceIterator( SoundCardInputDeviceList.begin(), SoundCardInputDeviceList.end());
}

void NxSoundManager::GetSoundDeviceList( std::vector< NxDeviceInfo  > & DevList  )
{
	DevList.resize(SoundCardDeviceList.size());
	std::copy(SoundCardDeviceList.begin(),SoundCardDeviceList.end(),DevList.begin());
}

int NxSoundManager::GetOutputDeviceIndexID( const std::string name )
{
	int AudioIndex = -1 ;
	int i = 0;

	NxSoundManager::NxAudioDeviceIterator iter = GetAudioOutputIterator();
	while(!iter.hasMoreElements()) {
		NxDeviceInfo Device =  iter.getNext();
		if( name ==  Device.name){
			AudioIndex = i ;
			Log(  "NxSound : GetOutputDeviceIndexID found for device " + Device.name );
			break;
		}
		i++;
	}

	if(AudioIndex == -1){ 
		Log( "NxSound : GetOutputDeviceIndexID device not found : " + name );
	}

	return AudioIndex ;
}

int NxSoundManager::GetInputDeviceIndexID( const std::string name )
{
	int AudioIndex = -1 ;
	int i = 0;

	NxSoundManager::NxAudioDeviceIterator iter = GetAudioInputIterator();
	while(!iter.hasMoreElements()){
		NxDeviceInfo Device = iter.getNext(); 
		if( name ==  Device.name){
			AudioIndex = i ;
			//Ogre::LogManager::getSingleton().logMessage("NxSound : GetInputDeviceIndexID found for device " + Ogre::String( Device.name ) );
			break;
		}
	i++;
	}

	if(AudioIndex == -1){  
	//	Ogre::LogManager::getSingleton().logMessage("NxSound : GetInputDeviceIndexID device not found : " + Ogre::String( name ) ); 
	}

	return AudioIndex ;
}


int NxSoundManager::GetDeviceIndexID( const std::string name )
{
	int AudioIndex = -1 ;
	int i = 0;

	for( int i = 0; i < SoundCardDeviceList.size(); i++ )
	{
		NxDeviceInfo Device = SoundCardDeviceList[i]; 
		if( name ==  Device.name)
		{
			AudioIndex = i ;
			break;
		}
	}

	if(AudioIndex == -1)
	{ 
		//Log"NxSound : GetDeviceIndexID device not found : " + Ogre::String( name ) ); 
	}

	return AudioIndex ;
}

std::string NxSoundManager::GetDeviceNameFromIndex( int ID )
{
	std::string DevName("Empty") ;
	int i = 0;
	for( int i = 0; i < SoundCardDeviceList.size(); i++ )
	{
		NxDeviceInfo Device = SoundCardDeviceList[i]; 
		if( ID ==  i  ){ DevName = Device.name ; break; }
	}
	return DevName ;
}

NxSoundDeviceOutput * NxSoundManager::CreateDeviceOutput( int AudioDeviceId )
{
	NxSoundDeviceOutput * Player = new NxSoundDeviceOutput( AudioDeviceId );
	ListPlayers.push_back( Player );
	return Player ;
}

NxSoundDeviceOutput * NxSoundManager::CreateDefaultDeviceOutput()
{
	NxSoundDeviceOutput * Player = new NxSoundDeviceOutput( DefaultOutputDeviceIndex );
	ListPlayers.push_back( Player );
	return Player ;
}



bool NxSoundManager::DeleteDeviceOutput( NxSoundDeviceOutput * Player )
{
 
	ListPlayers.remove( Player );
	delete Player ;
	return true ;
}

bool NxSoundManager::DeleteAllDeviceOutputs()
{
	for( std::list< NxSoundDeviceOutput * >::iterator iter =  ListPlayers.begin(); iter !=  ListPlayers.end(); iter++)
	{
		Log( "Deleting Output Device : " + (*iter)->GetDeviceName() );
		delete *iter;
	}
	ListPlayers.clear();
	return true ;
}

bool NxSoundManager::DeleteAllDeviceInputs()
{
		Log("Deleting Audio Inputs...");
	for( std::list< NxSoundDeviceInput * >::iterator iter =  ListRecorders.begin(); iter !=  ListRecorders.end(); iter++)
	{
		Log( "Deleting Input Device : " + (*iter)->GetDeviceName() );
		delete *iter ; 
	}
	ListRecorders.clear();
	Log("Deleting Audio Inputs : Done");
	return true ;
}

NxSoundDeviceOutput * NxSoundManager::GetDeviceOutput( int index )
{
	if( !ListPlayers.size() ) return 0 ;
	std::list<NxSoundDeviceOutput *>::iterator i = ListPlayers.begin();
	std::advance(i, index );
	// now index points to the nth member of the list (counting from zero)
	return *i ;
}

NxSoundDeviceInput * NxSoundManager::GetDeviceInput( int index )
{
	if( !ListRecorders.size() ) return 0 ;
	std::list<NxSoundDeviceInput *>::iterator i = ListRecorders.begin();
	std::advance(i, index );
	// now index points to the nth member of the list (counting from zero)
	return *i ;
}

size_t NxSoundManager::GetNumOutputDevices()
{
	return ListPlayers.size();
}

size_t NxSoundManager::GetNumInputDevices()
{
	return ListRecorders.size();
}

NxSoundDeviceInput * NxSoundManager::CreateDeviceInput( int AudioDeviceId )
{
	NxSoundDeviceInput * Recorder = new NxSoundDeviceInput( AudioDeviceId );
	ListRecorders.push_back( Recorder );
	return Recorder ;
}

NxSoundDeviceInput * NxSoundManager::CreateDefaultDeviceInput()
{
	NxSoundDeviceInput * Recorder = new NxSoundDeviceInput( DefaultInputDeviceIndex );
	ListRecorders.push_back( Recorder );
	return Recorder;
}


bool NxSoundManager::DeleteDeviceInput( NxSoundDeviceInput * Recorder )
{
	ListRecorders.remove( Recorder );
	delete Recorder ;
	return true ;
}

NxSoundMediaFile * NxSoundManager::AddMediaFile( const std::string & SoundName, NxSoundDeviceOutput * DeviceOutput , const std::string & FilePath, bool Normalize )
{

	Log("opening file : " + FilePath );

	//set string to lower case
	std::string FilePathcpy = FilePath;
	NxSoundUtils::ToLower( FilePathcpy );

	

	if(FilePathcpy.substr(FilePathcpy.find_last_of(".") + 1) == "wav") // wav
	{
		NxSoundMediaFileWav * File = NULL ; 
		File = new NxSoundMediaFileWav() ; 
		File->OpenFile( FilePathcpy, Normalize );
		DeviceOutput->AddMediaFile( File ); // add media file to output device
		mSounds.insert( NxSoundMap::value_type( SoundName, File ) );
		return File ;
		 
	} 
	else if(FilePathcpy.substr(FilePathcpy.find_last_of(".") + 1) == "mp3") // mp3
	{
		NxSoundMediaFileMp3 * File = NULL ; 
		File = new NxSoundMediaFileMp3() ; 
		Log( "NxSound : File is a MP3" );
		File->OpenFile( FilePathcpy, Normalize  );
		DeviceOutput->AddMediaFile( File ); // add media file to output device
		mSounds.insert( NxSoundMap::value_type( SoundName, File ) );
		return File ;
	}
	else if(FilePathcpy.substr(FilePathcpy.find_last_of(".") + 1) == "ogg") // ogg
	{
		NxSoundMediaFileOgg * File = NULL ; 
		File = new NxSoundMediaFileOgg() ; 
		Log( "NxSound : File is a OGG" );
		File->OpenFile( FilePathcpy, Normalize  );
		DeviceOutput->AddMediaFile( File ); // add media file to output device
		mSounds.insert( NxSoundMap::value_type( SoundName, File ) );
		return File ;
	}
	else if(FilePathcpy.substr(FilePathcpy.find_last_of(".") + 1) == "flac") // flac
	{

		NxSoundMediaFileFlac * File = NULL ; 
		File = new NxSoundMediaFileFlac() ; 
		Log( "NxSound : File is a FLAC" );
		File->OpenFile( FilePathcpy, Normalize  );
		DeviceOutput->AddMediaFile( File ); // add media file to output device
		mSounds.insert( NxSoundMap::value_type( SoundName, File ) );
		return File ;
	}
	else
	{
		Log( "NxSound : File as no extension" );
	}
}

NxSoundMediaFileVSTi * NxSoundManager::AddMediaFile( const std::string & SoundName, NxSoundDeviceOutput * DeviceOutput, NxSoundEffectVSTPlugin * VSTI )
{

	Log( "NxSound : adding VSTI" );

	NxSoundMediaFileVSTi * File = NULL ; 
	if( VSTI )
	{
		File = new NxSoundMediaFileVSTi();  
		File->SetVSTInstrument( VSTI ) ;
		Log( "NxSound : File is a VSTi" );
		DeviceOutput->AddMediaFile( File ); // add media file to output device
		mSounds.insert( NxSoundMap::value_type( SoundName, File ) );
	}
	return File ;
}

//! Get Sound Files Iterator.
NxSoundManager::NxSoundMapIterator NxSoundManager::GetNxSoundIterator()
{
	return NxSoundMapIterator( mSounds.begin(), mSounds.end() );
}

NxSoundMediaFile * NxSoundManager::GetMediaFile( int Index )
{
	//if( !ListPlayersMediaFiles.size() ) return 0 ;
	NxSoundMap::iterator i = mSounds.begin();
	std::advance( i, Index );
	// now index points to the nth member of the list (counting from zero)
	return i->second;
}

NxSoundMediaFile * NxSoundManager::GetMediaFile( const std::string & SoundName ) const
{
	NxSoundMap::const_iterator i = mSounds.find( SoundName );
	if (i == mSounds.end()){
		Log(   "Cannot find NxNode with name " + SoundName );
	}
	else{
		return i->second;
	}
}

void NxSoundManager::GetMediaFileNames( std::vector<string> & FileNames )
{
	NxSoundMapIterator Iter = GetNxSoundIterator();
	while( Iter.hasMoreElements() ) { 
		FileNames.push_back( Iter.getNext()->GetFileName() );
	}
}

NxSoundMediaFile * NxSoundManager::GetMediaFileFromName( const std::string& FileName  )
{
	NxSoundMediaFile * MediaFile = NULL ;
	NxSoundMapIterator Iter = GetNxSoundIterator();
	while( Iter.hasMoreElements() ) {
		NxSoundMediaFile * c = Iter.getNext();
		if( c->GetFileName() == FileName ){
			MediaFile = c ;
			break ;
		}
	}
	return MediaFile ;	
}

void NxSoundManager::PlayNxSound( const std::string & SoundName )
{
	mSounds[SoundName]->Play();
}

void NxSoundManager::StopNxSound( const std::string & SoundName )
{
	mSounds[SoundName]->Stop();
}

bool NxSoundManager::RemoveMediaFile( NxSoundMediaFile * MediaFile )
{
	// loop through all devices and find the media file(s)
	bool Find = false ;
	for( std::list< NxSoundDeviceOutput * >::iterator iter =  ListPlayers.begin(); iter !=  ListPlayers.end(); iter++) {
		NxSoundDeviceOutput * Device = *iter ; 
		if(Device->RemoveMediaFile( MediaFile )) {
			Find = true ;
		}
	}
	return Find ;
}

bool NxSoundManager::DeleteMediaFile( const std::string & Name )
{
	
	NxSoundMap::iterator i = mSounds.find( Name ); //MediaFile->GetName()
	bool Find = RemoveMediaFile( i->second );// remove media from any output device
	delete i->second;
	mSounds.erase(i);
	return Find ;	
}

void NxSoundManager::AddListener(  NxSoundManagerListener  * listener )
{
	 Listeners_List.push_back(listener); 
}

void NxSoundManager::RemoveListener( NxSoundManagerListener * listener )
{
	Listeners_List.remove( listener );
}

} // nxsound namespace