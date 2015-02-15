/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
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
#ifndef __NXUTILS_H__
#define __NXUTILS_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxUtils
{
public :
	//! NxGraphicsUtilities Class Constructor.
	NxUtils();
	//! NxGraphicsUtilities Class Deconstructor.
	~NxUtils();
	//! Gets File Name.
	static void SplitFileName( std::string& Filename );
	//! Gets File Path.
	static void SplitFilepath( std::string& Path );
	//! Gets Full Path.
	static std::string GetFullPath( std::string Path );
	//! Lower case a string.
	static void ToLower( std::string& Str );
	//! Splits string with given delimiter.
	static std::vector<std::string> SplitString( const std::string& s, const std::string& delim, const bool keep_empty = true );
	//! List Directories.
	static int ListDirectories( std::vector<std::string> &refvecDirectory, const std::string &refcstrRootDirectory, bool bSearchSubdirectories );
	//! Unicode to Ansi string.
	static std::string UnicodeToAnsi( std::wstring str );
	//! Ansi to Unicode string.
	static std::wstring AnsiToUnicode( std::string str );
	//! Get Random Float between two values.
	static float GetRandom(float x, float y) ;
	//! Creates a File List based on extension.
	static int SearchDirectory(std::vector<std::string> &refvecFiles, const std::string &refcstrRootDirectory, const std::string &refcstrExtension, bool bSearchSubdirectories );
	//! Get current working directory.
	static std::string GetCurrentWorkingDirectory();
	// harris corner detector
	static void DetectCorners( float * Cotrners2D, const unsigned char * im, int xsize, int ysize, int stride, int b, int* ret_num_corners, bool nonmax_suppression, NxFastCornerDetectionType Type );

};

}

#endif