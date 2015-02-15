#include "NxVideoPreCompiled.h"

#include <algorithm> 
#include <string.h>

namespace NxVideo_Namespace {

	std::string NxVideoUtils::ToString( double Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( float Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();	
	}

	std::string NxVideoUtils::ToString( int Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( unsigned int Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( int64 Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( uint64 Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( unsigned long Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( long Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	std::string NxVideoUtils::ToString( bool Value )
	{
		ostringstream str;
		str << Value ;
		return str.str();
	}

	void NxVideoUtils::ToLower( std::string& Str )
	{
		std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
	}

	void NxVideoUtils::ToUpper( std::string & Str )
	{
		std::transform(Str.begin(), Str.end(), Str.begin(), ::toupper );
	}

	void NxVideoUtils::SplitFileName( std::string& Filename )
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

	void NxVideoUtils::SplitFileExtension( std::string& FileExtension )
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

	unsigned long NxVideoUtils::RandNumber( unsigned long Min , unsigned long Max )
	{
		return ( rand() % ( Max - Min + 1 ) + Min ) ;
	}

	unsigned long NxVideoUtils::Str2FOURCC( const std::string & src )
	{
		unsigned long Four = 0;
		char st[4];
		strcpy(st, src.c_str());
		Four = st[0] | (st[1] << 8) | (st[2] << 16) | (st[3] << 24);
		return Four;
	}

	std::string NxVideoUtils::FOURCC2Str( unsigned long fcc )
	{
		std::string FOUR ;
		FOUR.resize(6);
		//#if BYTE_ORDER == BIG_ENDIAN

		FOUR[0]=(fcc >> 24) & 0xFF;
		FOUR[1]=(fcc >> 16) & 0xFF;
		FOUR[2]=(fcc >>  8) & 0xFF;
		FOUR[3]=(fcc      ) & 0xFF;

		//#else

		//FOUR[0]=(fcc      ) & 0xFF;
		//FOUR[1]=(fcc >>  8) & 0xFF;
		//FOUR[2]=(fcc >> 16) & 0xFF;
		//FOUR[3]=(fcc >> 24) & 0xFF;

		//#endif
		FOUR[4]=0;

		return FOUR;
	}

	unsigned char * NxVideoUtils::YUVToRGB( const unsigned char *yuv )
	{ 
	  /* // floating point 
	  int Y = yuv[0] - 16;
	  int U = yuv[1] - 128;
	  int V = yuv[2] - 128;

	  int R = 1.164*Y + 1.596*V           + 0.5;
	  int G = 1.164*Y - 0.813*V - 0.391*U + 0.5;
	  int B = 1.164*Y           + 2.018*U + 0.5;
	  */

	  // integer math
	  int Y = (yuv[0] - 16)*76284;
	  int U = yuv[1] - 128;
	  int V = yuv[2] - 128;

	  int R = Y + 104595*V           ;
	  int G = Y -  53281*V -  25625*U;
	  int B = Y            + 132252*U;

	  // round
	  R += 32768;
	  G += 32768;
	  B += 32768;

	  // shift
	  R >>= 16;
	  G >>= 16;
	  B >>= 16;

	  // clamp
	  if (R < 0) { R = 0; }
	  if (G < 0) { G = 0; }
	  if (B < 0) { B = 0; }

	  if (R > 255) { R = 255; };
	  if (G > 255) { G = 255; };
	  if (B > 255) { B = 255; };

	  // output

	  unsigned char * rgb = new unsigned char[3];
	  rgb[0] = R;
	  rgb[1] = G;
	  rgb[2] = B;

	return rgb ;

	}



/*
std::string NxVideoUtils::GetFullPath( std::string Path )
{
char full[_MAX_PATH];
if( _fullpath( full, ( const char * ) Path.c_str(), _MAX_PATH ) != NULL )
printf( "Full path is: %s\n", full );
else
printf( "Invalid path\n" );

return std::string( full );	
}

std::string GetCurrentWorkingDirectory()
{
std::string workingDirectory = "";
char currentPath[_MAX_PATH];
getcwd(currentPath, _MAX_PATH);
workingDirectory = currentPath;
return workingDirectory + "\\";
}

*/



}