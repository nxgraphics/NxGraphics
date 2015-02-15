#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"
#include "STK/include/Stk.h"

using namespace stk;

namespace NxSound_Namespace {

NxSoundMediaFileVSTi::NxSoundMediaFileVSTi(): NxSoundMediaFile(), VSTInstrument(0)
{
	mType = TYPE_VST_INSTRUMENT;
	CreateEffectBuffers();

	Play();
}

NxSoundMediaFileVSTi::~NxSoundMediaFileVSTi()
{
	if( VSTInstrument ){
		delete VSTInstrument ;
		VSTInstrument = NULL ;
	}
}

void NxSoundMediaFileVSTi::SetResolution( NxAudioFormat Res )
{
	mDstResolution = Res;
}  

void NxSoundMediaFileVSTi::SendMidiMsg( NxSoundMidiMessage message )
{
	if (GetVSTInstrument()->PropertiesWantsMidi()) {

		VstMidiEvent * pevent;
		pevent = new VstMidiEvent(); // memory leak !
		pevent->type = kVstMidiType;
		pevent->byteSize = 24;
		pevent->deltaFrames = 0;
		pevent->flags = 0;
		pevent->detune = 0;
		pevent->noteLength = 0;
		pevent->noteOffset = 0;
		pevent->reserved1 = 0;
		pevent->reserved2 = 0;
		pevent->noteOffVelocity = 0;
		pevent->midiData[0] = message.type;//type ; //should be included channel inside
		pevent->midiData[1] = message.intValues[0] ;
		pevent->midiData[2] = message.intValues[1] ;
		pevent->midiData[3] = 0 ; ///< 1 to 3 MIDI bytes; midiData[3] is reserved (zero)

		VstEvents * eve = new VstEvents();
		eve->numEvents = 1 ;
		eve->events[0] = ( VstEvent * ) pevent ;

		GetVSTInstrument()->ProcessEvents( eve ) ;
	}

	/*
	stk::Skini::Message message;
	message.type = __SK_NoteOn_ ;
	message.channel = 0;
	message.time = 0.0f ;
	message.intValues[0] = 0;
	message.intValues[1] = 0;
	*/

}


void NxSoundMediaFileVSTi::SetVSTInstrument( NxSoundEffectVSTPlugin * Instrument ) {

	VSTInstrument = Instrument ;
	VSTInstrument->GetModuleName(mFileName) ;
	mNumChannels = VSTInstrument->GetNumOutputs();
	//mResolution = Instrument->GetResolution();
}

void NxSoundMediaFileVSTi::Rewind()
{

}

NxSoundEffectVSTPlugin * NxSoundMediaFileVSTi::GetVSTInstrument(){
	return VSTInstrument ;
}

void NxSoundMediaFileVSTi::SetRate( double Rate ){

}


StkFrames& NxSoundMediaFileVSTi::Update( StkFrames& Frame ) {

	if(!Frame.size()) Frame.resize( SND_BUFFER_SIZE, GetNumChannels() );

	if( isMuted() || isStopped() ) {

		for ( unsigned int i=0; i < Frame.size(); i++ ){ 
			Frame[i] = 0.0f;
		} 
		return Frame;
	}

	if( VSTInstrument ){

		VstInt32 numinputs = VSTInstrument->GetNumInputs() ;
		VstInt32 numoutputs = VSTInstrument->GetNumOutputs() ;

		if(( VSTInstrument->GetEffect()->flags & effFlagsCanReplacing) != 0) {

			try {
				VSTInstrument->GetEffect()->processReplacing (VSTInstrument->GetEffect(), NULL , outBufs , VSTInstrument->GetBlockSize() );
			}
            catch (...) {
				Log( "NxSound : Error processing NxSoundMediaFileVSTi::Update" );
			}

		 

			for ( unsigned int i=0; i< VSTInstrument->GetBlockSize() ; i++ ){
				float * buffer = outBufs[0];//VSTInstrument->GetOutputBuffer(0) ;
				Frame( i , 0 ) = buffer[i]  ;
			} 

			for ( unsigned int i=0; i< VSTInstrument->GetBlockSize() ; i++ ){
				float * buffer = outBufs[1];//VSTInstrument->GetOutputBuffer(1) ;
				Frame( i , 1 ) = buffer[i] ;
			} 
		}
	}

	return Frame ;
}



}