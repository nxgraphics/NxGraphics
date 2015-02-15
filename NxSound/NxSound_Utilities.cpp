#include "NxSoundPreCompiled.h"

#include <algorithm> 
#include <string.h>

namespace NxSound_Namespace {

std::string NxSoundUtils::ToString( double Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxSoundUtils::ToString( float Value )
{
	ostringstream str;
	str << Value ;
	return str.str();	
}
	
std::string NxSoundUtils::ToString( int Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxSoundUtils::ToString( unsigned int Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxSoundUtils::ToString( int64 Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxSoundUtils::ToString( uint64 Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

std::string NxSoundUtils::ToString( unsigned long Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}
	
std::string NxSoundUtils::ToString( bool Value )
{
	ostringstream str;
	str << Value ;
	return str.str();
}

void NxSoundUtils::ToLower( std::string & Str )
{
	std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
}

void NxSoundUtils::ToUpper( std::string & Str )
{
	std::transform(Str.begin(), Str.end(), Str.begin(), ::toupper );
}

void NxSoundUtils::SplitFileName( std::string& Filename )
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

void NxSoundUtils::SplitFileExtension( std::string& FileExtension )
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

float NxSoundUtils::GetRandom(float x, float y) 
{
	float high = 0;
	float low = 0;
	float randNum = 0;
	if (x == y) return x;
	high = max(x,y);
	low = min(x,y);
	randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
	return randNum;
}


}