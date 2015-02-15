/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

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
#include "NxGraphicsPreCompiled.h"
#include "NxVector3.h"
#include "NxMath.h"

namespace Nx
{
	const Vector3 Vector3::ZERO( 0, 0, 0 );

	const Vector3 Vector3::UNIT_X( 1, 0, 0 );
	const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
	const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
	const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
	const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
	const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
	const Vector3 Vector3::UNIT_SCALE(1, 1, 1);


	Vector3::Vector3( const Ogre::Vector3 & rkVector ) {

		x = rkVector.x;
		y = rkVector.y;
		z = rkVector.z;

	}

	//Vector3& Vector3::operator = ( const Ogre::Vector3& rkVector ) {

	//	x = rkVector.x;
	//	y = rkVector.y;
	//	z = rkVector.z;

	//	return *this;

	//}

	/*const Ogre::Vector3 & Vector3::toOgre() const {
		Ogre::Vector3 * c = new Ogre::Vector3( x,y,z );
		return c;
	}*/

	/*
	const Ogre::Vector3 & Vector3::toOgre() const {
		return Ogre::Vector3( x,y,z );
	}*/
}
