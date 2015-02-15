/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __      _____                           __
			   / | / /_  __/ ___/ ____   __  __ ____   ____/ /
			  /  |/ /| |/_/\__ \ / __ \ / / / // __ \ / __  / 
			 / /|  /_>  < ___/ // /_/ // /_/ // / / // /_/ /  
			/_/ |_//_/|_|/____/ \____/ \__,_//_/ /_/ \__,_/   
			                                                  
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
#ifndef __NXSOUNDUTILITIES_H__
#define __NXSOUNDUTILITIES_H__

#include "NxSound_Prerequisites.h"

namespace NxSound_Namespace {

class NxSound_Export NxSoundUtils
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
	//! get file name.
	static void SplitFileName( std::string& FileName );
	//! get file extension.
	static void SplitFileExtension( std::string& FileExtension );
	//! get random float number
	static float GetRandom( float x, float y );
};
 
}

#endif