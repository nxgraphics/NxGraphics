#include "NxMocapPreCompiled.h"

#include <algorithm> 
#include <string.h>

namespace NxMocap_Namespace {

std::string NxMocapUtils::ToString( double Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxMocapUtils::ToString( float Value )
{
	ostringstream str;
	str << Value ;
	return str.str();	
}
	
std::string NxMocapUtils::ToString( int Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxMocapUtils::ToString( unsigned int Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxMocapUtils::ToString( long long Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}
	
std::string NxMocapUtils::ToString( bool Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

void NxMocapUtils::ToLower( std::string& Str )
{
	std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
}

void NxMocapUtils::SplitFileName( std::string& Filename )
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

void NxMocapUtils::SplitFileExtension( std::string& FileExtension )
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