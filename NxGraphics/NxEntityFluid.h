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
#ifndef __NXENTITYFLUID_H__
#define __NXENTITYFLUID_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage a 3D Physic Fluid Object.
class NxGraphics_Export Nx3DObjectPhysicFluid : public NxEntity 
{
public:
	//! Nx3DObjectPhysicFluid class Constructor.
	Nx3DObjectPhysicFluid( NxNode * parentNode, unsigned int NumParticles = 256, bool UseVertexColor = false );
	//! Nx3DObjectPhysicFluid class Destructor.
	~Nx3DObjectPhysicFluid();
	//! Update
	void Update( float time );
	//! Update Fluids.
	void Update( float time, const float * Positions, const unsigned int * Colors, unsigned long NumParticles );
	//! Set visible.
	void SetVisible( bool Visible );
	//! Set Particle size.
	void SetParticlesSize( float SizeX, float SizeY );
	//! Set Material Name.
	void SetMaterialName( std::string MaterialName );
	//! Set NxFluids to Particles.
	void SetNxFluids( NxPhysicsFluid * Fluid );
	//! Set Particle Positions.
	void SetPositions( const float * Positions );
private :
	//! NxFluids if any.
	NxPhysicsFluid * mFluids;
	//! Physx Fluid.
	NxParticles * mFluidObject;
	//! num particles
	unsigned long mNumParticles;
};

}

#endif