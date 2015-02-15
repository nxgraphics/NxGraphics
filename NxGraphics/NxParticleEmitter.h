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
#ifndef __NXPARTICLEEMITTER_H__
#define __NXPARTICLEEMITTER_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage a 3D Physic Fluid Object.
class NxGraphics_Export NxParticleEmitter : public NxEntity 
{
public:
	//! Nx3DObjectPhysicFluid class Constructor.
	NxParticleEmitter( NxParticleSystem * Parent, const std::string & ParticleSystemName, NxParticleEmitterType Type );
	//! Nx3DObjectPhysicFluid class Destructor.
	~NxParticleEmitter();
	void SetColourFades( const NxColourValue& colourStart, const NxColourValue& colourEnd );
	void SetEnabled(bool enabled);
	void SetStartTime( float startTime);
	void SetDuration( float duration);
	void SetRepeatDelay( float duration);
	void SetAngle( float Degree );
	void SetTimeToLive( float SecondsTime );
	void SetEmissionRate( float particlesPerSecond );
	void SetParticleVelocity( float Velocity );
	void SetDirection( const Ogre::Vector3 & Direction );
	void SetPosition( const Ogre::Vector3 & Position );
	void SetParameter( const std::string & Param,  const std::string & Value );
private :
	Ogre::ParticleEmitter* mParticleEmitter;
};

}

#endif