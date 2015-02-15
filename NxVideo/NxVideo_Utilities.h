#ifndef __NXVIDEOUTILITIES_H__
#define __NXVIDEOUTILITIES_H__

#include "NxVideo_Prerequisites.h"

namespace NxVideo_Namespace {

class NxVideo_Export NxVideoUtils
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
	//! unsigned long value to string.
	static std::string ToString( long Value );
	//! Lower case a string.
	static void ToLower( std::string& Str );
	//! Upper case a string.
	static void ToUpper( std::string & Str );
	//! get file name.
	static void SplitFileName( std::string& FileName );
	//! get file extension.
	static void SplitFileExtension( std::string& FileExtension );
	//! Give a random number between two values.
	static unsigned long RandNumber( unsigned long Min, unsigned long Max );
	//! convert a string to Four.
	unsigned long Str2FOURCC( const std::string & src );
	//! convert a Four to string.
	static std::string FOURCC2Str( unsigned long fcc );
	//! Convert pixel buffer Yuv to Rgb
	static unsigned char * YUVToRGB( const unsigned char *yuv );
};

}

#endif