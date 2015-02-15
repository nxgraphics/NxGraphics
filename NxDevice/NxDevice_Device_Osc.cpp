#include "NxDevicePreCompiled.h"

#include "../nxdeps/include/oscpack/osc/OscHostEndianness.h"
#include "../nxdeps/include/oscpack/osc/OscOutboundPacketStream.h"
#include "../nxdeps/include/oscpack/osc/OscReceivedElements.h"
#include "../nxdeps/include/oscpack/osc/OscPacketListener.h"
#include "../nxdeps/include/oscpack/ip/UdpSocket.h"

using namespace osc;

namespace NxDevice_Namespace {

#define PORT 7000

/*
class ExamplePacketListener : public osc::OscPacketListener {
protected:

virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
{
	try{
            // example of parsing single messages. osc::OsckPacketListener
            // handles the bundle traversal.
            
            if( strcmp( m.AddressPattern(), "/test1" ) == 0 )
			{
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                bool a1;
                osc::int32 a2;
                float a3;
                const char *a4;
                args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;
                
                std::cout << "received '/test1' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
                
            }else if( strcmp( m.AddressPattern(), "/test2" ) == 0 ){
                // example #2 -- argument iterator interface, supports
                // reflection for overloaded messages (eg you can call 
                // (*arg)->IsBool() to check if a bool was passed etc).
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                bool a1 = (arg++)->AsBool();
                int a2 = (arg++)->AsInt32();
                float a3 = (arg++)->AsFloat();
                const char *a4 = (arg++)->AsString();
                if( arg != m.ArgumentsEnd() )
                    throw osc::ExcessArgumentException();
                
                std::cout << "received '/test2' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
            }
        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};


*/


	NxDeviceOscBundleInitiator NxDeviceOscBeginBundleImmediate(1);
	NxDeviceOscBundleTerminator NxDeviceOscEndBundle;
	NxDeviceOscMessageTerminator NxDeviceOscEndMessage;
	NxDeviceOscNilType NxDeviceOscNil;
	NxDeviceOscInfinitumType NxDeviceOscInfinitum;

template <typename T>
struct BreakApart
{
	union
	{
		T asNative;
		char bytes[sizeof(T)];
	};

	BreakApart(const T& t) : asNative(t)
	{

	}

	unsigned int GetSize()
	{
		return sizeof(T);
	}

	char& operator[](int n) 
	{ 
		return bytes[n];
	}

};


static inline const char * FromFloat( float Val )
{
	union
	{
		float f;
		char c[4];
	} u;

	u.f = Val;

	char * k = new char[4];
	k[0] = u.c[3];
	k[1] = u.c[2];
	k[2] = u.c[1];
	k[3] = u.c[0];
	return k;
}

static inline const char * FromBool( bool Val )
{
	union
	{
		bool b;
		char c[1];
	} u;

	u.b = Val;
	char * k = new char[1];
	k[0] = u.c[0];
	return k;
}

static const char * FromInt32( signed long x )
{
	union{
		osc::int32 i;
		char c[4];
	} u;

	u.i = x;
	char * k = new char[4];
	k[3] = u.c[0];
	k[2] = u.c[1];
	k[1] = u.c[2];
	k[0] = u.c[3];
	return k;
}

static const char * FromUInt32( uint32 x )
{
	union{
		osc::uint32 i;
		char c[4];
	} u;

	u.i = x;
	char * k = new char[4];
	k[3] = u.c[0];
	k[2] = u.c[1];
	k[1] = u.c[2];
	k[0] = u.c[3];
	return k;
}




NxDeviceOscMessageValue::NxDeviceOscMessageValue()
{

}

NxDeviceOscMessageValue::~NxDeviceOscMessageValue()
{


}


NxDeviceOscValueType NxDeviceOscMessageValue::GetType()
{
	return mType;
}

bool NxDeviceOscMessageValue::IsBool() const
{
	if( mType == TYPEBOOL ) return true;
	return false;
}

bool NxDeviceOscMessageValue::IsFloat() const
{
	if( mType == TYPEFLOAT ) return true;
	return false;
}

bool NxDeviceOscMessageValue::IsInt32() const
{
	if( mType == TYPEINT32) return true;
	return false;
}

bool NxDeviceOscMessageValue::AsBool() const
{
	#ifdef OSC_HOST_LITTLE_ENDIAN
	union
	{
		bool b;
		char c[1];
	} u;

	u.c[0] = mValue[0];
	return u.b;
	#else
		return *(bool*)mValue;
	#endif
}
 
float NxDeviceOscMessageValue::AsFloat() const
{
	#ifdef OSC_HOST_LITTLE_ENDIAN
		union{
			float f;
			char c[4];
		} u;

		u.c[0] = mValue[3];
		u.c[1] = mValue[2];
		u.c[2] = mValue[1];
		u.c[3] = mValue[0];

		return u.f;
	#else
		return *(float*)mValue;
	#endif
}


signed long NxDeviceOscMessageValue::AsInt32() const
{
#ifdef OSC_HOST_LITTLE_ENDIAN
	union{
		signed long i;
		char c[4];
	} u;

	u.c[0] = mValue[3];
	u.c[1] = mValue[2];
	u.c[2] = mValue[1];
	u.c[3] = mValue[0];

	return u.i;
#else
	return *(int32*)mValue;
#endif
}


 

NxDeviceOscMessageValueBool::NxDeviceOscMessageValueBool( bool Value  ) : NxDeviceOscMessageValue()
{
	 mType = TYPEBOOL;
	 mValue = FromBool(Value); 
}

NxDeviceOscMessageValueFloat::NxDeviceOscMessageValueFloat( float Value  ) : NxDeviceOscMessageValue() 
{
	 mType = TYPEFLOAT;
	 mValue = FromFloat(Value);
}

NxDeviceOscMessageValueInt32::NxDeviceOscMessageValueInt32( int32 Value  ) : NxDeviceOscMessageValue() 
{
	mType = TYPEINT32;
	mValue = FromInt32(Value);
}


//--------------------------------------------------------------

NxDeviceOscMessages::NxDeviceOscMessages( const char * Pattern, unsigned long Arguments ) : mPattern( Pattern ), mNumArguments( Arguments ) 
{

}

NxDeviceOscMessages::~NxDeviceOscMessages()
{

}

const unsigned long NxDeviceOscMessages::GetNumArguments() const
{
	return mNumArguments;
}

const char * NxDeviceOscMessages::GetPattern() const
{
	return mPattern;
}

NxDeviceOscMessages::MessageType::const_iterator NxDeviceOscMessages::ArgumentsBegin() const {
	return mValues.begin();
}

int32 NxDeviceOscMessages::GetArgAsInt32( int Index ) const {
	return mValues[Index].AsInt32();
}

float NxDeviceOscMessages::GetArgAsFloat( int Index ) const {
	return mValues[Index].AsFloat();
}
 
NxDeviceOscMessages::MessageType::const_iterator NxDeviceOscMessages::ArgumentsEnd() const {
    return mValues.end();
}

void NxDeviceOscMessages::AddArgument( bool Val ) 
{
	mValues.push_back( NxDeviceOscMessageValueBool( Val ) ) ;
}

void NxDeviceOscMessages::AddArgument( float Val ) 
{
	mValues.push_back( NxDeviceOscMessageValueFloat( Val ) );
}	

void NxDeviceOscMessages::AddArgument( char Val ){

}

void NxDeviceOscMessages::AddArgument( double Val ){

}

void NxDeviceOscMessages::AddArgument( int32 Val ) {
	mValues.push_back( NxDeviceOscMessageValueInt32( Val ) );
}

void NxDeviceOscMessages::AddArgument( uint32 Val ){
}


void NxDeviceOscMessages::AddArgument( int64 Val ){
}


void NxDeviceOscMessages::AddArgument( const char* Val ){
}





const NxDeviceOscMessages & ConvertMessage( const osc::ReceivedMessage& m )
{
	// ????? should be passed as argument, memory leak
	NxDeviceOscMessages * msg = new NxDeviceOscMessages( m.AddressPattern(), m.ArgumentCount() );

	for( osc::ReceivedMessage::const_iterator iter = m.ArgumentsBegin(); iter != m.ArgumentsEnd(); iter++)
	{
		osc::ReceivedMessageArgument message = *iter; 
		if( message.IsBool() ) { 
			msg->AddArgument( message.AsBoolUnchecked() );
		}
		else if( message.IsFloat() ) { 
			msg->AddArgument( message.AsFloatUnchecked() );
		}
		else if( message.IsChar() ) msg->AddArgument( message.AsCharUnchecked() );
		else if( message.IsDouble() ) msg->AddArgument( message.AsDoubleUnchecked() ); 
		else if( message.IsInt32() )
		{ 
			//Log("VAL : " + nxdeviceu  ( message.AsInt32Unchecked()  ));
			msg->AddArgument( (int32)message.AsInt32Unchecked() );
		}
		else if( message.IsInt64() ) msg->AddArgument( message.AsInt64Unchecked() );
		//else if( message.IsMidiMessage() ) msg->AddArgument( message.AsMidiMessageUnchecked() ); 
		//else if( message.IsRgbaColor() ) msg->AddArgument( message.AsRgbaColorUnchecked() ); 
		else if( message.IsString() ) msg->AddArgument( message.AsStringUnchecked() ); 
		else if( message.IsSymbol() ) msg->AddArgument( message.AsSymbolUnchecked() ); 
		//else if( message.IsTimeTag() ) msg->AddArgument( message.AsTimeTagUnchecked() );
		else
		{ 

			Log("Unknown osc Type in NxDevice");
		}
	}

	return *msg;
}

//--------------------------------------------------------------

class NxDevice_Export NxDeviceOscListener : public osc::OscPacketListener
{
public:

	NxDeviceOscListener( NxDeviceOsc * Host ) : mHost( Host )
	{
	
	}

	~NxDeviceOscListener()
	{
	
	}

	void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
	{
		try
		{
			for( list<NxDeviceManagerListener * >::iterator iter = NxDeviceManager::getSingleton().Listeners_List.begin(); iter != NxDeviceManager::getSingleton().Listeners_List.end(); iter++)
			{
				NxDeviceManagerListener  * listen = *iter; 
				const NxDeviceOscMessages msg = ConvertMessage( m );

			 

				listen->NxDeviceReceiveOscMessage( mHost->GetID(), msg);
				//delete msg;
			}
		}
		catch( osc::Exception& e )
		{
				// any parsing errors such as unexpected argument types, or 
				// missing arguments get thrown as exceptions.
				//std::cout << "error while parsing message: "<< m.AddressPattern() << ": " << e.what() << "\n";
		}	
	}

	NxDeviceOsc * mHost;
	#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
	HANDLE thread_OSC;
#endif
	UdpListeningReceiveSocket * OscReceiver ;
};

void NxDeviceOsc::SendOscMessage( const char *addressName, int port, const NxDeviceOscOutputMessage & Message )
{
	IpEndpointName Connection( addressName, port );
	UdpTransmitSocket transmitSocket( Connection );
	transmitSocket.Send( Message.Data(), Message.Size() );


    /*
	const int size = 1024 ;
	char buffer[size];
	NxDeviceOscOutputMessage Mess( buffer, size );

	Mess << NxDeviceOscBeginBundleImmediate
	<< NxDeviceOscBeginMessage( "/test1" ) 
	<< true << 23 << (float)3.1415 << "hello" << NxDeviceOscEndMessage
	<< NxDeviceOscBeginMessage( "/test2" ) 
	<< true << 24 << (float)10.8 << "world" << NxDeviceOscEndMessage
	<< NxDeviceOscEndBundle;
		*/
 

}
 
NxDeviceOsc::NxDeviceOsc() : mOsc(0)
{
	mDeviceType = NXDEVICE_OSC ;
	mDeviceSubType = NXDEVICE_SUB_OSC_RECEIVER ;
	Listening = false ;
	mOsc = new NxDeviceOscListener(this);
	GenerateID();
}

NxDeviceOsc::~NxDeviceOsc()
{

}

bool NxDeviceOsc::loadPropertySheet()
{
	return true ;
}

bool NxDeviceOsc::SavePropertySheet()
{
	return true ;
}

bool NxDeviceOsc::ConnectDevice()
{
	return true ;
}

bool NxDeviceOsc::DisconnectDevice()
{
	return true ;
}

#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
DWORD  NxDevice_OSC_Update(LPVOID lpParameter)
{
	NxDeviceOsc * pointer =  ( NxDeviceOsc * ) lpParameter;
	pointer->mOsc->OscReceiver->RunUntilSigInt();

	return 0;
}	
#endif

bool NxDeviceOsc::StartListening( int Port )
{
	ListeningPort = Port;
	mOsc->OscReceiver = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, Port ), mOsc );
	if( mOsc->OscReceiver )
	{
		Thread_Should_Run = true ;
#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
		DWORD OSC_thread_id;
		mOsc->thread_OSC = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) NxDevice_OSC_Update, this, 0, &OSC_thread_id);
#endif
		Listening = true ;
		Log( "Started listening for osc messages." ) ;
		return true ;
	}
	Log( "StartListening Osc Failed" ) ;
	return false ;
}

bool NxDeviceOsc::StopListening()
{
	//OscReceiver->Break();
	mOsc->OscReceiver->AsynchronousBreak();
	#if NXDEVICE_PLATFORM == NXDEVICE_PLATFORM_WIN32
	DWORD dwRet = WaitForSingleObject( mOsc->thread_OSC, INFINITE ); 
	if (dwRet == WAIT_TIMEOUT)
	{
		DWORD dwForcefulExitCode = 1;        
		::TerminateThread( mOsc->thread_OSC , dwForcefulExitCode);
	} 
#endif

	if(mOsc->OscReceiver)
	{
		delete mOsc->OscReceiver ;
		mOsc->OscReceiver = NULL ;
	}

	Listening = false ;

	return true ;
}



}