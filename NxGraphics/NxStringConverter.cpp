#include "NxGraphicsPreCompiled.h"

#include <algorithm> 
#include <string.h>

namespace Nx {

	std::string NxStringConverter::toString( double Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( float Value ) {
		ostringstream str;
		str << Value ;
		return str.str();	
	}

	std::string NxStringConverter::toString( int Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( unsigned int Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( int64 Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( uint64 Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( unsigned long Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( long Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxStringConverter::toString( bool Value ) {
		ostringstream str;
		str << Value ;
		return str.str();
	}

	void NxStringConverter::toLower( std::string& Str ) {
		std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
	}

	void NxStringConverter::toUpper( std::string & Str ) {
		std::transform(Str.begin(), Str.end(), Str.begin(), ::toupper );
	}
	 

	 

}