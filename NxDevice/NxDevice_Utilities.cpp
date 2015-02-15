#include "NxDevicePreCompiled.h"

#include <algorithm> 
#include <string.h>

namespace NxDevice_Namespace {

std::string NxDeviceUtils::ToString( double Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxDeviceUtils::ToString( float Value )
{
	ostringstream str;
	str << Value ;
	return str.str();	
}
	
std::string NxDeviceUtils::ToString( int Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxDeviceUtils::ToString( unsigned int Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxDeviceUtils::ToString( int64 Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxDeviceUtils::ToString( uint64 Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxDeviceUtils::ToString( unsigned long Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}
	
std::string NxDeviceUtils::ToString( bool Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

void NxDeviceUtils::ToLower( std::string & Str )
{
	std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
}

void NxDeviceUtils::ToUpper( std::string & Str )
{
	std::transform(Str.begin(), Str.end(), Str.begin(), ::toupper );
}

void NxDeviceUtils::SplitFileName( std::string& Filename )
{
	string fullPath(Filename);
	replace(fullPath.begin(),fullPath.end(),'\\','/');
	string::size_type lastSlashPos = fullPath.find_last_of('/');
	if (lastSlashPos==std::string::npos)//didnt find a slach
	{ Filename=""; }
	else// found a slach
	{
		//path=fullPathr.substr(0,lastSlashPos);
		Filename=fullPath.substr(lastSlashPos+1,fullPath.size()-lastSlashPos-1);
	}
}

void NxDeviceUtils::SplitFileExtension( std::string& FileExtension )
{
	string fullPath(FileExtension);
	string::size_type lastSlashPos=fullPath.find_last_of('.');
	if (lastSlashPos==std::string::npos)
	{ FileExtension="";}
	else// found a dot
	{
		FileExtension=fullPath.substr(lastSlashPos+1,fullPath.size()-lastSlashPos-1);
	}
}


}