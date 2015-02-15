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
#ifndef __NXPARTICLESYSTEM_H__
#define __NXPARTICLESYSTEM_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage a 3D Physic Fluid Object.
class NxGraphics_Export NxParticleSystem : public NxEntity 
{
public:
	//! Nx3DObjectPhysicFluid class Constructor.
	NxParticleSystem( NxNode * parentNode, unsigned int NumParticles = 256  );
	//! Nx3DObjectPhysicFluid class Destructor.
	~NxParticleSystem();
	//! Create Emitter
	NxParticleEmitter * CreateEmitter( NxParticleEmitterType Type );
	//! create affector
	NxParticleSystemAffector * CreateAffector( NxParticleSystemAffectorTypes Type );
	//! set particle system visible.
	void SetVisible( bool Visible );
	//! update
	void Update( float time );
	//! get particle number. 
	unsigned int GetNumParticles();
	//! empty 
	void Clear();
	//! set sorting enabled
	void SetSortingEnabled( bool sort  );
	//! set cull individualy
	void SetCullIndividually( bool Cull );
	//! fast forward.
	void FastForward( float time, float interval  );
	//! set speed factor.
	void SetSpeedFactor( float speed );
	//! set default particle size.
	void SetParticlesSize( float SizeX, float SizeY );
	//! create a particle.
	void CreateParticle( const Ogre::Vector3 & pos, const Ogre::Vector3 & dir, float Velocity, float TTL, float RotSpeed );
	//! set particle position
	void SetParticlePosition( unsigned int index, const Ogre::Vector3 & pos );
	//! set material name
	void SetMaterialName( std::string MaterialName );
	//! Get material name
	const std::string & GetMaterialName();
private :

	//! num particles
	unsigned long mNumParticles;

	Ogre::ParticleSystem* mParticleSystem;
};

}

#endif