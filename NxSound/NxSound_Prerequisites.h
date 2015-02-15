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
#ifndef __NXSOUNDPREREQUISITES_H__
#define __NXSOUNDPREREQUISITES_H__

#include <NxSoundConfig.h>
#include "NxSound_Plateform.h"

 
typedef double NxDouble;

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

#include <time.h>
#include <fstream>

using namespace std;

//#define VST_2_4_EXTENSIONS 1
//#define VST_FORCE_DEPRECATED 0

#define SND_BUFFER_SIZE 2048


/*
#ifdef __ANDROID__
#define NxMediaPath "/sdcard/NxSound.log"
#else
#define NxMediaPath "NxSound.log"
#endif
*/

enum NxSoundApi 
{
	NX_UNSPECIFIED,    /*!< Search for a working compiled API. */
	NX_LINUX_ALSA,     /*!< The Advanced Linux Sound Architecture API. */
	NX_LINUX_OSS,      /*!< The Linux Open Sound System API. */
	NX_UNIX_JACK,      /*!< The Jack Low-Latency Audio Server API. */
	NX_MACOSX_CORE,    /*!< Macintosh OS-X Core Audio API. */
	NX_WIN_ASIO,   /*!< The Steinberg Audio Stream I/O API. */
	NX_WIN_DS,     /*!< The Microsoft Direct Sound API. */
	NX_OPEN_SL,     /*!< The ANDROID OPENSL Sound API. */
	NX_DUMMY   /*!< A compilable but non-functional API. */
};


enum NxSound_Effects_Library_Types 
{ 
	Effect_VST_Instrument,
	Effect_VST_Effect,
	Effect_PitShift, 
	Effect_3D,
	Effect_Beat,
	Effect_OggBroadCast,
	Effect_Granulate,
	Effect_Chorus,
	Effect_Echo
};

enum NxSoundMediaType
{ 
	TYPE_WAV,
	TYPE_MP3,
	TYPE_VST_INSTRUMENT,
	TYPE_OGG,
	TYPE_FLAC,
	TYPE_BUFF,
	TYPE_STK_INSTRUMENT 
};

typedef unsigned long NxAudioFormat;
static const NxAudioFormat NX_SINT8 = 0x1;    // 8-bit signed integer.
static const NxAudioFormat NX_SINT16 = 0x2;   // 16-bit signed integer.
static const NxAudioFormat NX_SINT24 = 0x4;   // Lower 3 bytes of 32-bit signed integer.
static const NxAudioFormat NX_SINT32 = 0x8;   // 32-bit signed integer.
static const NxAudioFormat NX_FLOAT32 = 0x10; // Normalized between plus/minus 1.0.
static const NxAudioFormat NX_FLOAT64 = 0x20; // Normalized between plus/minus 1.0.



typedef unsigned int NxAudioStreamStatus;
static const NxAudioStreamStatus NX_INPUT_OVERFLOW = 0x1;    // Input data was discarded because of an overflow condition at the driver.
static const NxAudioStreamStatus NX_OUTPUT_UNDERFLOW = 0x2;  // The output buffer ran low, likely causing a gap in the output sound.

//! The public device information structure for returning queried values.
struct NxDeviceInfo
{
	bool probed;                  /*!< true if the device capabilities were successfully probed. */
	std::string name;             /*!< Character string device identifier. */
	unsigned int outputChannels;  /*!< Maximum output channels supported by device. */
	unsigned int inputChannels;   /*!< Maximum input channels supported by device. */
	unsigned int duplexChannels;  /*!< Maximum simultaneous input/output channels supported by device. */
	bool isDefaultOutput;         /*!< true if this is the default output device. */
	bool isDefaultInput;          /*!< true if this is the default input device. */
	std::vector<unsigned int> sampleRates; /*!< Supported sample rates (queried from list of standard rates). */
	NxAudioFormat nativeFormats;  /*!< Bit mask of supported data formats. */

	// Default constructor.
	NxDeviceInfo() :probed(false), outputChannels(0), inputChannels(0), duplexChannels(0),
	isDefaultOutput(false), isDefaultInput(false), nativeFormats(0) {}
};

struct NxSoundMidiMessage
{ 
	long type;                         /*!< The message type, as defined in SKINI.msg. */
	long channel;                      /*!< The message channel (not limited to 16!). */
	NxDouble time;                     /*!< The message time stamp in seconds (delta or absolute). */
	std::vector<NxDouble> floatValues; /*!< The message values read as floats (values are type-specific). */
	std::vector<long> intValues;       /*!< The message values read as ints (number and values are type-specific). */
	std::string remainder;             /*!< Any remaining message data, read as ascii text. */

	// Default constructor.
	NxSoundMidiMessage() :type(0), channel(0), time(0.0), floatValues(2), intValues(2) {}
};

#define NX_TIMEUNIT_MS                0x00000001  /* Milliseconds. */
#define NX_TIMEUNIT_PCM               0x00000002  /* PCM samples, related to milliseconds * samplerate / 1000. */
#define NX_TIMEUNIT_S                 0x00000003  /* Seconds */
#define NX_TIMEUNIT_PCMBYTES          0x00000004  /* Bytes, related to PCM samples * channels * datawidth (ie 16bit = 2 bytes). */
#define NX_TIMEUNIT_RAWBYTES          0x00000008  /* Raw file bytes of (compressed) sound data (does not include headers).  Only used by Sound::getLength and Channel::getPosition. */
#define NX_TIMEUNIT_PCMFRACTION       0x00000010  /* Fractions of 1 PCM sample.  Unsigned int range 0 to 0xFFFFFFFF.  Used for sub-sample granularity for DSP purposes. */
#define NX_TIMEUNIT_MODORDER          0x00000100  /* MOD/S3M/XM/IT.  Order in a sequenced module format.  Use Sound::getFormat to determine the PCM format being decoded to. */
#define NX_TIMEUNIT_MODROW            0x00000200  /* MOD/S3M/XM/IT.  Current row in a sequenced module format.  Sound::getLength will return the number of rows in the currently playing or seeked to pattern. */
#define NX_TIMEUNIT_MODPATTERN        0x00000400  /* MOD/S3M/XM/IT.  Current pattern in a sequenced module format.  Sound::getLength will return the number of patterns in the song and Channel::getPosition will return the currently playing pattern. */
#define NX_TIMEUNIT_SENTENCE_MS       0x00010000  /* Currently playing subsound in a sentence time in milliseconds. */
#define NX_TIMEUNIT_SENTENCE_PCM      0x00020000  /* Currently playing subsound in a sentence time in PCM Samples, related to milliseconds * samplerate / 1000. */
#define NX_TIMEUNIT_SENTENCE_PCMBYTES 0x00040000  /* Currently playing subsound in a sentence time in bytes, related to PCM samples * channels * datawidth (ie 16bit = 2 bytes). */
#define NX_TIMEUNIT_SENTENCE          0x00080000  /* Currently playing sentence index according to the channel. */
#define NX_TIMEUNIT_SENTENCE_SUBSOUND 0x00100000  /* Currently playing subsound index in a sentence. */
#define NX_TIMEUNIT_BUFFERED          0x10000000  /* Time value as seen by buffered stream.  This is always ahead of audible time, and is only used for processing. */

typedef unsigned int NXSOUNDTIMEUNIT;


class MTRand;
class RtAudio;
struct AEffect;
struct VstTimeInfo;
struct VstTimeInfo;
struct VstEvents;
struct VstParameterProperties;

namespace stk
{
	class StkFrames;
	class ModalBar;
	class TcpServer;
	class Mutex;
	class FileReadWav;
	class Stk;
	class Granulate;
	class Chorus;
	class Echo;
	class PitShift;
	class Thread;
}

namespace NxSound_Namespace 
{

	class FileReadFlac;
	class FileReadMp3;
	class FileReadOgg;
	class NxSoundManagerListener ;
	class NxSoundManager;
	class NxSoundUtils;
	class NxSoundLog;
	class NxSoundDevice;
	class NxSoundDeviceInput;
	class NxSoundDeviceOutput;
	class NxSoundFrames;
	class NxSoundEffect;
	class NxSoundMediaFile;
	class NxSoundMediaFileMp3;
	class NxSoundMediaFileVSTi;
	class NxSoundMediaFileWav;
	class NxSoundMediaFileFlac;
	class NxSoundMediaFileOgg;
	class NxSoundMediaFileBuffer;
	struct NxSoundMediaFileBufferStruct;
	class NxSoundMediaInstModal;
	class NxSoundEffectVSTHost;
	class NxSoundEffectVSTPlugin;
	class NxSoundEffectPitShift;
	class NxSoundEffectGranulate;
	class NxSoundEffectBeatDetector;
	class NxSoundEffectOggBroadCaster;
	class NxSoundEffect3D;
	class NxSoundPluginDescription;
	class NxSoundFFT;


template < typename T > 
struct vector 
{ 
	typedef typename std::vector<T> type;    
}; 

template <typename K, typename V, typename P = std::less<K>  > 
struct map  { 
	typedef typename std::map<K, V, P> type; 
};

template <typename T, typename IteratorType, typename ValType>
class IteratorWrapper {
	private:
		IteratorWrapper();
	protected:
		IteratorType mBegin;
		IteratorType mCurrent;
		IteratorType mEnd;
	public:
		typedef ValType ValueType;
		typedef ValType* PointerType;
		typedef IteratorType iterator;
		typedef IteratorType const_iterator;
		IteratorWrapper ( IteratorType start, IteratorType last ) : mBegin( start ), mCurrent ( start ), mEnd ( last ){}
		bool hasMoreElements ( ) const { return mCurrent != mEnd; }
		void moveNext() { ++mCurrent; }
		const IteratorType& begin() {return mBegin;}
		IteratorType& current(){return mCurrent;}
		const IteratorType& end() {return mEnd;}
		
};

template <typename T, typename IteratorType>
class NxSoundVectorIteratorWrapper : public IteratorWrapper<T, IteratorType, typename  T::value_type>
{
public:
	typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::ValueType ValueType ; 
	typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::PointerType PointerType ;
	NxSoundVectorIteratorWrapper ( IteratorType start, IteratorType last ) : IteratorWrapper<T, IteratorType, typename T::value_type>( start, last ){ }
	ValueType peekNext() const { return *(this->mCurrent); }
	PointerType peekNextPtr()  const { return &(*(this->mCurrent) ); }
	ValueType getNext() { return *(this->mCurrent++); }	
};

template <typename T>
class NxSoundVectorIterator : public NxSoundVectorIteratorWrapper< T , typename T::iterator > {
public:
	NxSoundVectorIterator( typename T::iterator start, typename T::iterator last ) : NxSoundVectorIteratorWrapper<T,  typename T::iterator>( start , last ){}
	explicit NxSoundVectorIterator( T& c ) : NxSoundVectorIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() ){}	
};

template <typename T, typename IteratorType>
class MapIteratorWrapper  : public IteratorWrapper<T, IteratorType, typename T::mapped_type>
{
	public:
		typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::ValueType ValueType ; 
		typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::PointerType PointerType ;	
		typedef typename T::value_type PairType ; 
		typedef typename T::key_type KeyType;
		MapIteratorWrapper ( IteratorType start, IteratorType last ) : IteratorWrapper<T, IteratorType, typename T::mapped_type>( start, last ) { }
        KeyType peekNextKey(void) const { return this->mCurrent->first; }
		ValueType peekNextValue (  ) const { return this->mCurrent->second; }
		const PointerType peekNextValuePtr (  )  const { return &(this->mCurrent->second); }
        ValueType getNext() { return ((this->mCurrent++)->second) ; }
};

template <typename T>
class NxMapIterator : public MapIteratorWrapper<T,  typename T::iterator>{
	public:	
		NxMapIterator( typename T::iterator start, typename T::iterator last ) : MapIteratorWrapper<T,  typename T::iterator>(start , last ){}
		explicit NxMapIterator( T& c ): MapIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() ){}	
};





} 

#endif