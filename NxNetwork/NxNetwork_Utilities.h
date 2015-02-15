#ifndef __NXNETWORKUTILITIES_H__
#define __NXNETWORKUTILITIES_H__

#include "NxNetwork_Prerequisites.h"

namespace NxNetwork {

class NxNetwork_Export NxNetworkUtils
{
public :
	//! double value to string.
	static std::string ToString( double Value );
	//! float value to string.
	static std::string ToString( float Value );
	//! int value to string.
	static std::string ToString( int Value );
	//! unsigned int value to string.
	static std::string ToString( unsigned int Value );
	//! bool value to string.
	static std::string ToString( bool Value );
	//! __int64 value to string.
	static std::string ToString( int64 Value );
	//! unsigned __int64 value to string.
	static std::string ToString( uint64 Value );
	//! unsigned long value to string.
	static std::string ToString( unsigned long Value );
	//! Lower case a string.
	static void ToLower( std::string& Str );
	//! Upper case a string.
	static void ToUpper( std::string & Str );
};

}

#endif