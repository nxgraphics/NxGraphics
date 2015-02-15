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

#ifndef __NXSOUND_H__
#define __NXSOUND_H__

#include "NxSound_Prerequisites.h"
#include "NxSound_Singleton.h"

namespace NxSound_Namespace 
{

//!  Listener class for NxSound.
class NxSound_Export NxSoundManagerListener
{
public :
	virtual ~NxSoundManagerListener(){}
	virtual void OnSoundStopped( const std::string SoundName ) = 0;
	virtual void OnSoundMuted( const std::string SoundName ) = 0;

};

//!  Sound manager class for NxSound.
class NxSound_Export NxSoundManager : public Singleton<NxSoundManager>  
{
	typedef map<std::string, NxSoundMediaFile* >::type NxSoundMap;
	typedef NxMapIterator<NxSoundMap> NxSoundMapIterator;

	typedef vector<NxDeviceInfo>::type NxDeviceInfoVector;
	typedef NxSoundVectorIterator<NxDeviceInfoVector> NxAudioDeviceIterator;

	public:
	//! NxSoundManager class Constructor. see NxSoundApi
	NxSoundManager( const NxSoundApi Api );
	//! NxSoundManager class Destructor.
	~NxSoundManager();
	//! NxSoundManager Singleton.
	static NxSoundManager& getSingleton();
	//! NxSoundManager Singleton.
	static NxSoundManager* getSingletonPtr();
	//! Get current audio driver type.
	const NxSoundApi GetNxSoundApiType();
	//! Create a Audio Ouput Device from index.
	NxSoundDeviceOutput * CreateDeviceOutput( int AudioDeviceId );
	//! Delete Audio Ouput Device.
	bool DeleteDeviceOutput( NxSoundDeviceOutput * Player );
	//! Delete All Audio Ouput Devices.
	bool DeleteAllDeviceOutputs();
	//! Get Audio Output Device by Index.
	NxSoundDeviceOutput * GetDeviceOutput( int index );
	//! Get Number Of Audio Outputs Created.
	size_t GetNumOutputDevices();
	//! Create an Audio Input Device by Index.
	NxSoundDeviceInput * CreateDeviceInput( int AudioDeviceId );
	//! Delete Audio Input Device.
	bool DeleteDeviceInput( NxSoundDeviceInput * Input );
	//! Delete all inputs
	bool DeleteAllDeviceInputs();
	//! get Audio Input Device by Index.
	NxSoundDeviceInput * GetDeviceInput( int Index );
	//! Get Number Of Audio Input Created.
	size_t GetNumInputDevices();
	//! Create the default Audio Output Device.
	NxSoundDeviceOutput * CreateDefaultDeviceOutput();
	//! Create the default Audio Input Device;
	NxSoundDeviceInput * CreateDefaultDeviceInput();
	//! Get Sound Output Device List.
	NxAudioDeviceIterator GetAudioOutputIterator();
	//! Get Sound Input Device List.
	NxAudioDeviceIterator GetAudioInputIterator();
	//! Get Mixed Sound Device List.	
	void GetSoundDeviceList( std::vector< NxDeviceInfo > & DevList );
	//! Get Output Device index by name.
	int GetOutputDeviceIndexID( const std::string DeviceName );
	//! Get Input Device index by name.
	int GetInputDeviceIndexID( const std::string DeviceName );
	//! Get Device index by name.
	int GetDeviceIndexID( const std::string DeviceName );
	//! Get Device Name From Id.
	std::string GetDeviceNameFromIndex( int ID );
	//! Get default input device index.
	unsigned int GetDefaultInputDeviceIndex();
	//! Get Default output device index.
	unsigned int GetDefaultOutputDeviceIndex();
	//! add media file from path.
	NxSoundMediaFile * AddMediaFile( const std::string & SoundName, NxSoundDeviceOutput * DeviceOutput, const std::string & FilePath, bool Normalize = false );
	//! add a vst Instrument.
	NxSoundMediaFileVSTi * AddMediaFile( const std::string & SoundName, NxSoundDeviceOutput * DeviceOutput, NxSoundEffectVSTPlugin * VstPlug );

	//! Get Sound Files Iterator.
	NxSoundMapIterator GetNxSoundIterator();

	//! get media file by index.
	NxSoundMediaFile * GetMediaFile( int Index );
	//! get media file by name.
	NxSoundMediaFile * GetMediaFile( const std::string & SoundName ) const;

	//! Play sound from name.
	void PlayNxSound( const std::string & SoundName );
	//! Stop sound from name.
	void StopNxSound( const std::string & SoundName );
	//! remove media file from mixer.
	bool RemoveMediaFile( NxSoundMediaFile * MediaFile );
	//! delete media file.
	bool DeleteMediaFile( const std::string & Name );
	//! Get media file Names.
	void GetMediaFileNames( std::vector<string> & FileNames );
	//! Get Media File from Name.
	NxSoundMediaFile * GetMediaFileFromName( const std::string& FileName );
	//! Set Vst plugins base path.
	void SetPluginBasePathVST( const std::string VstPath );
	//! Get Vst plugin base path.
	std::string GetPluginBasePathVST();
	//! 3D spatialization Listener position.
	void SetListenerPosition( float X , float Y, float Z );
	//! 3D spatialization Listener orientation.
	void SetListenerOrientation( float X , float Y, float Z );
	//! Add listener.
	void AddListener( NxSoundManagerListener * listener );
	//! Remove listener.
	void RemoveListener( NxSoundManagerListener * listener );

	//std::list< NxSoundPluginDescription * > GetAvailableVSTPluginList( std::string Path, bool ScanSubFolders );
	//std::list< NxSoundPluginDescription * > GetAvailableDefaultVSTPluginList( bool ScanSubFolders );
	//std::list< NxSoundPluginDescription * > GetAvailablePluginList();

	//unsigned long GenerateId();

private :
	std::list<NxSoundManagerListener *> Listeners_List;
	//int SearchDirectory( std::vector<std::string> &refvecFiles, const std::string &refcstrRootDirectory, const std::string &refcstrExtension, bool bSearchSubdirectories );
	//std::string getCurrentWorkingDirectory();
	std::string PluginBasePathVst;
	NxSoundApi m_NxSoundApi;
	bool InitializeOpenAL();
	bool UseOpenAL ;
	RtAudio * dac;
	NxAudioFormat format ;
	int DefaultOutputDeviceIndex ;
	int DefaultInputDeviceIndex ;
	// private data
	std::list<NxSoundDeviceOutput * > ListPlayers ;
	std::list<NxSoundDeviceInput * >  ListRecorders ;

	//std::list<NxSoundMediaFile *>  ListPlayersMediaFiles ;

	//! NxNode List
	NxSoundMap mSounds;


	// sound device listing variables
	NxDeviceInfoVector SoundCardOutputDeviceList ;
	NxDeviceInfoVector SoundCardInputDeviceList ;
	NxDeviceInfoVector SoundCardDeviceList ;

};

 
} // nxsound namespace

#endif

 

