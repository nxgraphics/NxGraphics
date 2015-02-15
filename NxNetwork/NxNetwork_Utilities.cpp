#include "NxNetworkPreCompiled.h"

#include <algorithm> 
#include <string.h>

namespace NxNetwork {

	std::string NxNetworkUtils::ToString( double Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxNetworkUtils::ToString( float Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();	
	}

	std::string NxNetworkUtils::ToString( int Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxNetworkUtils::ToString( unsigned int Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxNetworkUtils::ToString( int64 Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxNetworkUtils::ToString( uint64 Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxNetworkUtils::ToString( unsigned long Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxNetworkUtils::ToString( bool Value )
	{
		std::ostringstream str;
		str << Value ;
		return str.str();
	}

	void NxNetworkUtils::ToLower( std::string& Str )
	{
		std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
	}

	void NxNetworkUtils::ToUpper( std::string & Str )
	{
		std::transform(Str.begin(), Str.end(), Str.begin(), ::toupper );
	}

}