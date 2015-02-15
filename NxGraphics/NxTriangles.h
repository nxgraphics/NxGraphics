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
#ifndef __NXTRIANGLES_H__
#define __NXTRIANGLES_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage a 3D Physic Fluid Object.
class NxGraphics_Export Nx3DObjectTriangles : public NxEntity 
{
public:
	//! Nx3DObjectPhysicFluid class Constructor.
	Nx3DObjectTriangles( NxNode * parentNode, bool UseVertexColour, bool IsUVW = false);
	//! Nx3DObjectPhysicFluid class Destructor.
	~Nx3DObjectTriangles();
	//! Update Fluids.
	void Update( const float * Triangles, const float * Normals, const unsigned int * Colors, unsigned long NumTriangles );
	//! Set Material Name.
	void SetMaterialName( const std::string & MaterialName );
	void SetVisibilityFlags( int Flags );
	const std::string & GetMaterialName();
	void SetVisible( bool Visible );
	void SetWorldTransform( const Nx::Matrix4 & Trans );
	void SetTextureCoordinates( const float * Coordinates );
	void SetRenderQueueGroup( unsigned char Group );
	void SetZOrder( unsigned short Priority );
	unsigned short GetZOrder() const;
private :
	//! NxTriangles
	NxTriangles * mTriangleObject;
};

}
#endif