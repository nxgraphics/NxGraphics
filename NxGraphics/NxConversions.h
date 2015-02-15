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
#ifndef __NXGRAPHICSOGRECONVERSION_H__
#define __NXGRAPHICSOGRECONVERSION_H__

#include <OgreQuaternion.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>

#include <OgreMatrix3.h>
#include <OgreMatrix4.h>

#include <OgreColourValue.h>
#include "NxColourValue.h"

using namespace Ogre;
using namespace Nx;

// Quaternion 
inline const Ogre::Quaternion & NxQuatToOgre( const Nx::Quaternion & quat ) {
	return Ogre::Quaternion( quat.w,quat.x,quat.y,quat.z );
}

inline void NxQuatToOgre( Ogre::Quaternion & quat1, const Nx::Quaternion & quat2 ) {
	quat1.w = quat2.w;
	quat1.x = quat2.x;
	quat1.y = quat2.y;
	quat1.z = quat2.z;
}

// vector2
inline const Ogre::Vector2 & NxVec2ToOgre( const Nx::Vector2 & vec ) {
	return Ogre::Vector2(vec.x, vec.y );
}

inline void NxVec2ToOgre( Ogre::Vector2 & vec1, const Nx::Vector2 & vec2 ) {
	vec1.x = vec2.x;
	vec1.y = vec2.y;
}

// vector3
inline const Ogre::Vector3 & NxVec3ToOgre( const Nx::Vector3 & vec ) {
	return Ogre::Vector3(vec.x, vec.y, vec.z);
}

inline void NxVec3ToOgre( Ogre::Vector3 & vec1, const Nx::Vector3 & vec2 ) {
	vec1.x = vec2.x;
	vec1.y = vec2.y;
	vec1.z = vec2.z;
}

// should be equal operator nx = ogre in Vector3 File
inline const Nx::Vector3 & OgreToNxVec3( const Ogre::Vector3 & vec ) {
	return Nx::Vector3(vec.x, vec.y, vec.z);
}


//vector4
inline const Ogre::Vector4 & NxVec4ToOgre( const Nx::Vector4 & vec )  {
	return Ogre::Vector4( vec.x,vec.y,vec.z,vec.w );
}

inline void NxVec4ToOgre( Ogre::Vector4 & vec1, const Nx::Vector4 & vec2 )  {
	vec1.x = vec2.x;
	vec1.y = vec2.y;
	vec1.z = vec2.z;
	vec1.w = vec2.w;
}

//matrix3
inline const Ogre::Matrix3 & NxMat3toOgre( const Nx::Matrix3 & mat ) {
	return Ogre::Matrix3( mat.m );
}

inline void NxMat3toOgre(  Ogre::Matrix3 & mat1, const Nx::Matrix3 & mat2 ) {

	for( int i = 0; i < 3 ; i++ ) {
		mat1[i][0] = mat2[i][0];
		mat1[i][1] = mat2[i][1];
		mat1[i][2] = mat2[i][2]; 
	} 
}


//matrix4
inline const Ogre::Matrix4 & NxMat4toOgre( const Nx::Matrix4 & mat)  { 
	return Ogre::Matrix4( mat.m[0][0],mat.m[0][1],mat.m[0][2],mat.m[0][3],mat.m[1][0],mat.m[1][1],mat.m[1][2],mat.m[1][3],mat.m[2][0],mat.m[2][1],mat.m[2][2],mat.m[2][3],mat.m[3][0],mat.m[3][1],mat.m[3][2],mat.m[3][3]);
}


inline void NxMat4toOgre( Ogre::Matrix4 & mat1, const Nx::Matrix4 & mat2 ) {

	for( int i = 0; i < 4 ; i++ ) {
		mat1[i][0] = mat2[i][0];
		mat1[i][1] = mat2[i][1];
		mat1[i][2] = mat2[i][2];
		mat1[i][3] = mat2[i][3];
	}


	//return Ogre::Matrix4( mat.m[0][0],mat.m[0][1],mat.m[0][2],mat.m[0][3],mat.m[1][0],mat.m[1][1],mat.m[1][2],mat.m[1][3],mat.m[2][0],mat.m[2][1],mat.m[2][2],mat.m[2][3],mat.m[3][0],mat.m[3][1],mat.m[3][2],mat.m[3][3]);
}

//nxcolour
inline const Ogre::ColourValue & NxColourtoOgre( const NxColourValue & colour)  { 
	return Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a );
}

inline void NxColourtoOgre( Ogre::ColourValue & col1, const NxColourValue & col2 ) {
	col1.r = col2.r;
	col1.g = col2.g;
	col1.b = col2.b;
	col1.a = col2.a;
}

// should be copy operator in NxColourValue
inline const NxColourValue & OgretoNxColour( const Ogre::ColourValue & colour)  { 
	return NxColourValue( colour.r, colour.g, colour.b, colour.a );
}

//radian
inline const Ogre::Radian & NxRadiantoOgre( const Nx::Radian & rad )  { 
	return Ogre::Radian( (Ogre::Radian) rad.valueRadians() );
}

inline void NxRadiantoOgre( Ogre::Radian & radian1, const Nx::Radian & radian2 )  { 
	radian1 = (Ogre::Radian) radian2.valueRadians() ;
}


//degrees
inline const Ogre::Degree & NxDegreeToOgre( const Nx::Degree & deg )  { 
	return Ogre::Degree( (Ogre::Degree) deg.valueDegrees()  );
}

inline void NxDegreeToOgre( Ogre::Degree & deg1, const Nx::Degree & deg2 )  { 
	deg1 = (Ogre::Degree) deg2.valueDegrees() ;
	 
}




 

#endif