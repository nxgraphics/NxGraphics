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
#ifndef __NXSOUNDEFFECTVSTPLUGIN_H__
#define __NXSOUNDEFFECTVSTPLUGIN_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSoundVstModule;


class NxSoundEffectVSTWindow
{
public:
	NxSoundEffectVSTWindow( NxSoundEffectVSTPlugin * VstPlugin );
	~NxSoundEffectVSTWindow();

	void ShowDialog();

private:
	NxSoundEffectVSTPlugin * mPlugin;
};



//!  A class to manage Steinberg VST sound effects.
class NxSound_Export NxSoundEffectVSTPlugin : public NxSoundEffect
{
	public:
	//! NxSoundEffectVSTPlugin Class Constructor.
    NxSoundEffectVSTPlugin( NxSoundEffectVSTHost * pHost );
	//! NxSoundEffectVSTPlugin Class Deconstructor.
    ~NxSoundEffectVSTPlugin();
	//! Process Audio Data.
	void Process( float ** BufIn , float ** BufOut , int BlockSize );
	//! Load Plugin From Path.
    virtual bool Load(const char *name);
	//! Unload Plugin.
    virtual bool Unload();
	//! Show Editor if any;
	void ShowDialog();
	//! Get Vst Effect.
	AEffect * GetEffect();

	bool PropertiesWantsMidi();
	//! Does the plugin receive Events.
	bool PropertiesCanReceiveEvents();
	//! Does the plugin receive Midi.
	bool PropertiesCanMidiProgramNames();
	//! Does the plugin have an interface.
	bool PropertiesHasEditor();
	//! Is the plugin a Vst Instrument.
	bool PropertiesIsSynth();
	//! Get Input Number.
	int GetNumInputs();
	//! Get Output Number.
	int GetNumOutputs();
	//! Get Number of Programs.
	int GetNumPrograms();
	//! Get Parameter Number.
	int GetNumParams();
	//! Get Plugin Unique ID.
	int GetUniqueID();
	//! Get Plugin Version.
	int GetVersion();
	//! Get Plugin Module Name.
	void GetModuleName( std::string & ModuleName );
	//! Get Plugin Vendor Name.
	void GetModuleVendorName( std::string & ModuleVendorName );
	//! Get Plugin Product Name.
	void GetModuleProductName( std::string & ModuleProductName );
	//! Get Block Size.
	int GetBlockSize();
	//! Get BitsWidth
	unsigned int GetResolution();
	//! Set Program.
	void SetProgram( int ProgramIndex );
	//! Get Plugin Program List.
	void GetProgramNames( std::vector<string> & ProgramNames );
	//! Process Events.
	void ProcessEvents( VstEvents * Events );
	//! Get Parameters.
	void GetParametersProperties( std::vector<VstParameterProperties *> & ParameterProperties );
	//! Get number parameters.
	unsigned int GetNumParameters();
	//! Set Parameter.
	bool SetParameter( int parameter, float value );
	//! Set Parameter.
	bool SetParameter( int parameter, int value );
	//! Get Parameter Value.
	float GetParameterValue( int parameter );
	//! Get Parameter Name.
	const std::string GetParameterName( int parameter );
	//! Setup parameters
	void SetupParameters();

private:
 
	//! vst Plugin Window if any.
	NxSoundEffectVSTWindow * mWindow;
	//! Plugin Vst Host.
    NxSoundEffectVSTHost *mHost;
	//! Plugin Vst Effect.
    AEffect * pEffect;
	bool VstWantsMidi ;
	bool VstCanReceiveEvents  ;
	bool VstCanMidiProgramNames  ;
	bool VstHasEditor ;
	bool VstIsSynth ;
	int NumIn ;
	int NumOut ;
	int NumPrograms ;
	int NumParams ;
	int VstuniqueID;		 
	int Vstversion;
	std::string VstModuleName ;
	std::string VstModuleVendorName;
	std::string VstModuleProductName;

	int VstBlockSize ; 
	float VstSampleRate ; 
	int VstNumProcessCycles ; 

	NxSoundVstModule * mModule;

    //HMODULE hModule;
    char *sDir;

	void SetInputBuffer( int nBuf, float *pData );
    float ** GetOutputBuffers() { return outBufs; }
	float *  GetOutputBuffer( int nBuf );
	float ** outBufs;
	float ** inBufs;
    float fMax;
	unsigned short wChnMask;
	int nAllocatedOutbufs;
	int nAllocatedInbufs;
};

}//namespace

#endif