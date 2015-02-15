/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       __  ___                         
			   / | / /_  __ /  |/  /____   _____ ____ _ ____ 
			  /  |/ /| |/_// /|_/ // __ \ / ___// __ `// __ \
			 / /|  /_>  < / /  / // /_/ // /__ / /_/ // /_/ /
			/_/ |_//_/|_|/_/  /_/ \____/ \___/ \__,_// .___/ 
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
#ifndef __NXMOCAPPLATEFORM_H__
#define __NXMOCAPPLATEFORM_H__

namespace NxMocap_Namespace {

#define NXMOCAP_PLATFORM_WIN32 1
#define NXMOCAP_PLATFORM_LINUX 2
#define NXMOCAP_PLATFORM_APPLE 3
#define NXMOCAP_PLATFORM_SYMBIAN 4
#define NXMOCAP_PLATFORM_APPLE_IOS 5
#define NXMOCAP_PLATFORM_ANDROID 6
#define NXMOCAP_PLATFORM_TEGRA2 7
#define NXMOCAP_PLATFORM_NACL 8

#define NXMOCAP_COMPILER_MSVC 1
#define NXMOCAP_COMPILER_GNUC 2
#define NXMOCAP_COMPILER_BORL 3
#define NXMOCAP_COMPILER_WINSCW 4
#define NXMOCAP_COMPILER_GCCE 5

#define NXMOCAP_ENDIAN_LITTLE 1
#define NXMOCAP_ENDIAN_BIG 2

#define NXMOCAP_ARCHITECTURE_32 1
#define NXMOCAP_ARCHITECTURE_64 2

	/* Finds the compiler type and version.
	*/
#if defined( __GCCE__ )
#   define NXMOCAP_COMPILER NXMOCAP_COMPILER_GCCE
#   define NXMOCAP_COMP_VER _MSC_VER
	//#     include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE 
#elif defined( __WINSCW__ )
#   define NXMOCAP_COMPILER NXMOCAP_COMPILER_WINSCW
#   define NXMOCAP_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define NXMOCAP_COMPILER NXMOCAP_COMPILER_MSVC
#   define NXMOCAP_COMP_VER _MSC_VER
#elif defined( __clang__ )
#   define NXMOCAP_COMPILER NXMOCAP_COMPILER_CLANG
#   define NXMOCAP_COMP_VER (((__clang_major__)*100) + \
	(__clang_minor__*10) + \
	__clang_patchlevel__)
#elif defined( __GNUC__ )
#   define NXMOCAP_COMPILER NXMOCAP_COMPILER_GNUC
#   define NXMOCAP_COMP_VER (((__GNUC__)*100) + \
	(__GNUC_MINOR__*10) + \
	__GNUC_PATCHLEVEL__)
#elif defined( __BORLANDC__ )
#   define NXMOCAP_COMPILER NXMOCAP_COMPILER_BORL
#   define NXMOCAP_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__ 
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

	/* Finds the current platform */

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define NXMOCAP_PLATFORM NXMOCAP_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
	// Device                                                     Simulator
	// Both requiring OS version 4.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define NXMOCAP_PLATFORM NXMOCAP_PLATFORM_APPLE_IOS
#   else
#       define NXMOCAP_PLATFORM NXMOCAP_PLATFORM_APPLE
#   endif
#elif defined(__ANDROID__)
#       define NXMOCAP_PLATFORM NXMOCAP_PLATFORM_ANDROID
#else
#   define NXMOCAP_PLATFORM NXMOCAP_PLATFORM_LINUX
#endif

	/* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define NXMOCAP_ARCH_TYPE NXMOCAP_ARCHITECTURE_64
#else
#   define NXMOCAP_ARCH_TYPE NXMOCAP_ARCHITECTURE_32
#endif

	//// Windows Settings
#if NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_WIN32

	// If we're not including this from a client build, specify that the stuff should get exported. Otherwise, import it.
#       if defined( NXMOCAP_STATIC_LIB )
	// Linux compilers don't have symbol import/export directives.
#       define NxMocap_Export
#       define NxMocap_Private
#   else
#       if defined( NXMOCAP_NONCLIENT_BUILD )
#               define NxMocap_Export __declspec( dllexport )
#       else
#           if defined( __MINGW32__ )
#               define NxMocap_Export
#           else
#                   define NxMocap_Export __declspec( dllimport )
#           endif
#       endif
#       define NxMocap_Private
#       endif


	// Win32 compilers use _DEBUG for specifying debug builds. for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define NXMOCAP_DEBUG_MODE 1
#   else
#       define NXMOCAP_DEBUG_MODE 0
#   endif


	// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
	// STLPORT fixes this though so allow if found
	// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
	// GCC 4 is also fine
#if defined(__MINGW32__)
# if NXMOCAP_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || NXMOCAP_COMP_VER > 345
#    define NXMOCAP_UNICODE_SUPPORT 1
#   else
#    define NXMOCAP_UNICODE_SUPPORT 0
#   endif
#  else
#   define NXMOCAP_UNICODE_SUPPORT 1
#  endif
# else
#  define NXMOCAP_UNICODE_SUPPORT 1
# endif
#else
#  define NXMOCAP_UNICODE_SUPPORT 1
#endif

#endif // NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_WIN32

	// Linux/Apple/iOs/Android/NaCl Settings
#if NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_LINUX || NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_APPLE || NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_APPLE_IOS || \
	NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_ANDROID || NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_NACL

	// Enable GCC symbol visibility
#   if defined( NXMOCAP_GCC_VISIBILITY )
#       define NxMocap_Export  __attribute__ ((visibility("default")))
#       define NxMocap_Private __attribute__ ((visibility("hidden")))
#   else
#       define NxMocap_Export
#       define NxMocap_Private
#   endif

	// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

#   ifdef DEBUG
#       define NXMOCAP_DEBUG_MODE 1
#   else
#       define NXMOCAP_DEBUG_MODE 0
#   endif

#if NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_APPLE
#define NXMOCAP_PLATFORM_LIB "NxMocapPlatform.bundle"
#elif NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_APPLE_IOS
#define NXMOCAP_PLATFORM_LIB "NxMocapPlatform.a"
#else //NXMOCAP_PLATFORM_LINUX
#define NXMOCAP_PLATFORM_LIB "libNxMocapPlatform.so"
#endif

	// Always enable unicode support for the moment
	// Perhaps disable in old versions of gcc if necessary
#define NXMOCAP_UNICODE_SUPPORT 1

#endif

	//----------------------------------------------------------------------------
	// Android Settings
#if NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_ANDROID
#   ifdef NXMOCAP_UNICODE_SUPPORT
#       undef NXMOCAP_UNICODE_SUPPORT
#   endif
#       define NXMOCAP_UNICODE_SUPPORT 0
#   define NXMOCAP_DEBUG_MODE 0
#         define CLOCKS_PER_SEC  1000
	// A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifdef DEBUG
#       define NXMOCAP_DEBUG_MODE 1
#   else
#       define NXMOCAP_DEBUG_MODE 0
#   endif
#endif
	//----------------------------------------------------------------------------
	// Endian Settings
	// check for BIG_ENDIAN config flag, set OGRE_ENDIAN correctly
#ifdef NXMOCAP_CONFIG_BIG_ENDIAN
#    define NXMOCAP_ENDIAN OGRE_ENDIAN_BIG
#else
#    define NXMOCAP_ENDIAN OGRE_ENDIAN_LITTLE
#endif

	// Integer formats of fixed bit width
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef int int32;
	typedef short int16;
	typedef char int8;
	// define uint64 type
#if NXMOCAP_COMPILER == NXMOCAP_COMPILER_MSVC
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#else
	typedef unsigned long long uint64;
	typedef long long int64;
#endif


}//namespace




#endif

