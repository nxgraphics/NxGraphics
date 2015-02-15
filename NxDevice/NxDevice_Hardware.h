/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       ____               _           
			   / | / /_  __ / __ \ ___  _   __ (_)_____ ___ 
			  /  |/ /| |/_// / / // _ \| | / // // ___// _ \
			 / /|  /_>  < / /_/ //  __/| |/ // // /__ /  __/
			/_/ |_//_/|_|/_____/ \___/ |___//_/ \___/ \___/ 
			                                                
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
#ifndef __NXDEVICEHARDWARE_H__
#define __NXDEVICEHARDWARE_H__

#include "NxDevice_Prerequisites.h"

namespace NxDevice_Namespace {

enum NxDevice_Hardware_Types
{ 
	NXDEVICE_MIDI_CONTROLLER,
	NXDEVICE_WIIMOTE,
	NXDEVICE_3DCONNEXION,
	NXDEVICE_TRACKIR,
	NXDEVICE_DMX,
	NXDEVICE_MOUSE_KEYBOARD,
	NXDEVICE_MOCAP_MYSQL_READER ,
	NXDEVICE_MOCAP_MYSQL_RECORDER,
	NXDEVICE_AUDIO,
	NXDEVICE_WAVERIDER,
	NXDEVICE_OSC,
	NXDEVICE_LASER
} ;

enum NxDevice_Hardware_SubTypes
{ 
	NXDEVICE_SUB_MIDI_IN,
	NXDEVICE_SUB_MIDI_OUT,
	NXDEVICE_SUB_MIDI_THRU,
	NXDEVICE_SUB_SOCKET_MOCAP,
	NXDEVICE_SOUND_INPUT ,
	NXDEVICE_SOUND_OUTPUT ,
	NXDEVICE_SUB_OSC_RECEIVER ,
	NXDEVICE_SUB_OSC_SENDER,
	NXDEVICE_SUB_DMX_IN,
	NXDEVICE_SUB_DMX_OUT

} ;

typedef struct DeviceHardwareParam
{
	std::string ParamName;
	int Datatype;
	double MinValue;
	double MaxValue;
	double CurrentValue;
} DeviceHardwareParam;

typedef struct DeviceHardwareMidiParam : public DeviceHardwareParam
{
	int Command;
	int Channel;
	int Note;
	int Velocity ;
};

class NxDevice_Export NxDevice_Hardware  
{
public :
	NxDevice_Hardware();
	~NxDevice_Hardware();
	virtual bool loadPropertySheet();
	virtual bool SavePropertySheet();
	virtual void BuildParamDictionary();
	virtual bool ConnectDevice() = 0;
	virtual bool DisconnectDevice() = 0;
	virtual void SetDeviceIndex( int Index );
	virtual int GetDeviceIndex();
	void GenerateID();
	unsigned long GetID();
	void SetID( unsigned long Id ) ;
	
	NxDevice_Hardware_Types GetHardwareDeviceType();
	void SetHardwareDeviceType( NxDevice_Hardware_Types Type);
	NxDevice_Hardware_SubTypes GetHardwareDeviceSubType();
	void SetHardwareDeviceSubType( NxDevice_Hardware_SubTypes SubType );
	std::string GetHardwareDeviceName();
	void SetHardwareDeviceName( std::string );
	std::list<std::string> GetParameterDictionaryPageList();
	typedef std::map< std::string, std::list< DeviceHardwareParam * > > DeviceHardwareParamType  ;
	DeviceHardwareParamType GetParameterDictionary();

	std::list< DeviceHardwareParam *> GetParameterDictionaryPage( std::string Page  );
	DeviceHardwareParam * GetParameterDictionaryParameter( std::string DictionaryName, long Index );

	virtual bool StartListening( int port );
	virtual bool StopListening();
	bool IsListening();
	unsigned long DeviceID  ;

protected :
	NxDevice_Hardware_Types mDeviceType ;
	NxDevice_Hardware_SubTypes mDeviceSubType;
	int mDeviceIndex;
	std::string mDeviceName;

	DeviceHardwareParamType DeviceHardwareParamList ;
	bool Thread_Should_Run;
	bool Listening ;
	int ListeningPort ;
};


enum NxDeviceOscValueType
{
	TYPEBOOL,
	TYPEFLOAT,
	TYPEINT32
};

class NxDevice_Export NxDeviceOscMessageValue
{
public:
	NxDeviceOscMessageValue();
	virtual ~NxDeviceOscMessageValue();
	NxDeviceOscValueType GetType();
	bool IsBool() const ;
	bool IsFloat() const;
	bool IsInt32() const;
	bool AsBool() const;
	float AsFloat() const;
	signed long AsInt32() const;


protected :
	NxDeviceOscValueType mType;
	const char * mValue;
};

class NxDevice_Export NxDeviceOscMessageValueBool : public NxDeviceOscMessageValue
{
public :
	explicit NxDeviceOscMessageValueBool( bool );
};

class NxDevice_Export NxDeviceOscMessageValueFloat : public NxDeviceOscMessageValue
{
public :
	explicit NxDeviceOscMessageValueFloat( float );
};

class NxDevice_Export NxDeviceOscMessageValueInt32 : public NxDeviceOscMessageValue
{
public :
	explicit NxDeviceOscMessageValueInt32( int32 );
};
////////////////////////

struct NxDevice_Export NxDeviceOscBundleInitiator{
	explicit NxDeviceOscBundleInitiator( uint64 timeTag_ ) : timeTag( timeTag_ ) {}
	uint64 timeTag;
};

extern NxDevice_Export NxDeviceOscBundleInitiator NxDeviceOscBeginBundleImmediate;

inline NxDevice_Export NxDeviceOscBundleInitiator NxDeviceOscBeginBundle( uint64 timeTag=1 )
{
	return NxDeviceOscBundleInitiator(timeTag);
}

struct NxDevice_Export NxDeviceOscBundleTerminator{
};

extern NxDevice_Export NxDeviceOscBundleTerminator NxDeviceOscEndBundle;

struct NxDevice_Export NxDeviceOscBeginMessage{
	explicit NxDeviceOscBeginMessage( const char *addressPattern_ ) : addressPattern( addressPattern_ ) {}
	const char *addressPattern;
};

struct NxDevice_Export NxDeviceOscMessageTerminator{
};

extern NxDevice_Export NxDeviceOscMessageTerminator NxDeviceOscEndMessage;

// osc specific types. they are defined as structs so they can be used
// as separately identifiable types with the streaming operators.

struct NxDevice_Export NxDeviceOscNilType{
};

extern NxDevice_Export NxDeviceOscNilType Nil;


struct NxDevice_Export NxDeviceOscInfinitumType{
};

extern NxDevice_Export NxDeviceOscInfinitumType NxDeviceOscInfinitum;

struct NxDevice_Export NxDeviceOscRgbaColor{
	NxDeviceOscRgbaColor() {}
	explicit NxDeviceOscRgbaColor( uint32 value_ ) : value( value_ ) {}
	uint32 value;

	operator uint32() const { return value; }
};


struct NxDevice_Export NxDeviceOscMidiMessage{
	NxDeviceOscMidiMessage() {}
	explicit NxDeviceOscMidiMessage( uint32 value_ ) : value( value_ ) {}
	uint32 value;

	operator uint32() const { return value; }
};


struct NxDevice_Export NxDeviceOscTimeTag{
	NxDeviceOscTimeTag() {}
	explicit NxDeviceOscTimeTag( uint64 value_ ) : value( value_ ) {}
	uint64 value;

	operator uint64() const { return value; }
};


struct NxDevice_Export NxDeviceOscSymbol{
	NxDeviceOscSymbol() {}
	explicit NxDeviceOscSymbol( const char* value_ ) : value( value_ ) {}
	const char* value;

	operator const char *() const { return value; }
};


struct NxDevice_Export NxDeviceOscBlob{
	NxDeviceOscBlob() {}
	explicit NxDeviceOscBlob( const void* data_, unsigned long size_ )
		: data( data_ ), size( size_ ) {}
	const void* data;
	unsigned long size;
};

#ifdef x86_64

typedef signed int oscint32;
typedef unsigned int oscuint32;

#else

typedef signed long oscint32;
typedef unsigned long oscuint32;

#endif

class NxDevice_Export NxDeviceOscOutputMessage{
public:
	NxDeviceOscOutputMessage( char *buffer, unsigned long capacity );
	~NxDeviceOscOutputMessage();

	void Clear();

	unsigned int Capacity() const;

	// invariant: size() is valid even while building a message.
	unsigned int Size() const;

	const char *Data() const;

	// indicates that all messages have been closed with a matching EndMessage
	// and all bundles have been closed with a matching EndBundle
	bool IsReady() const;

	bool IsMessageInProgress() const;
	bool IsBundleInProgress() const;

	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscBundleInitiator& rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscBundleTerminator& rhs );

	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscBeginMessage& rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscMessageTerminator& rhs );

	NxDeviceOscOutputMessage& operator<<( bool rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscNilType& rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscInfinitumType& rhs );
	NxDeviceOscOutputMessage& operator<<( oscint32 rhs );

#ifndef x86_64
	NxDeviceOscOutputMessage& operator<<( int rhs )
	{ *this << (int32)rhs; return *this; }
#endif

	NxDeviceOscOutputMessage& operator<<( float rhs );
	NxDeviceOscOutputMessage& operator<<( char rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscRgbaColor& rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscMidiMessage& rhs );
	NxDeviceOscOutputMessage& operator<<( int64 rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscTimeTag& rhs );
	NxDeviceOscOutputMessage& operator<<( double rhs );
	NxDeviceOscOutputMessage& operator<<( const char* rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscSymbol& rhs );
	NxDeviceOscOutputMessage& operator<<( const NxDeviceOscBlob& rhs );

private:

	char *BeginElement( char *beginPtr );
	void EndElement( char *endPtr );

	bool ElementSizeSlotRequired() const;
	void CheckForAvailableBundleSpace();
	void CheckForAvailableMessageSpace( const char *addressPattern );
	void CheckForAvailableArgumentSpace( long argumentLength );

	char *data_;
	char *end_;

	char *typeTagsCurrent_; // stored in reverse order
	char *messageCursor_;
	char *argumentCurrent_;

	// elementSizePtr_ has two special values: 0 indicates that a bundle
	// isn't open, and elementSizePtr_==data_ indicates that a bundle is
	// open but that it doesn't have a size slot (ie the outermost bundle)
	uint32 *elementSizePtr_;

	bool messageIsInProgress_;
};


class NxDevice_Export NxDeviceOscMessages
{
public :
	NxDeviceOscMessages( const char *, unsigned long );
	~NxDeviceOscMessages();
	const unsigned long GetNumArguments() const;
	const char * GetPattern() const;
	void AddArgument( bool Val );
	void AddArgument( float Val );
	void AddArgument( char Val );
	void AddArgument( double Val );
	void AddArgument( int32 Val );
	void AddArgument( int64 Val);
	void AddArgument( uint32 Val );
	void AddArgument( const char* Val );
 
	typedef std::vector<NxDeviceOscMessageValue> MessageType;
	MessageType::const_iterator ArgumentsBegin() const;
	MessageType::const_iterator ArgumentsEnd() const;

	int32 GetArgAsInt32( int Index ) const ;
	float GetArgAsFloat( int Index ) const ;

private :
	const char * mPattern;
	unsigned long mNumArguments;
	MessageType mValues;
};

class NxDevice_Export NxDeviceOsc : public NxDevice_Hardware 
{
public :
	NxDeviceOsc();
	~NxDeviceOsc();
	bool loadPropertySheet();
	bool SavePropertySheet();
	bool ConnectDevice();
	bool DisconnectDevice();
	bool StartListening( int port );
	bool StopListening();
	void SendOscMessage( const char *addressName, int port, const NxDeviceOscOutputMessage & Message );
	NxDeviceOscListener * mOsc;
};

#ifdef USE_DMX
class NxDevice_Export NxDeviceDmx : public NxDevice_Hardware
{
public :
	NxDeviceDmx();
	~NxDeviceDmx();
	bool loadPropertySheet();
	bool SavePropertySheet();
	bool ConnectDevice();
	bool DisconnectDevice();
	void SendMessage( const unsigned int Channel, const unsigned char Value );
	void SetGrandMaster( const unsigned char Level );
	bool IsOpenDmxPro();
	static int GetNumDevices();
	NxDeviceDmxUtil * mDevice;
};
#endif



 

} // namespace

#endif