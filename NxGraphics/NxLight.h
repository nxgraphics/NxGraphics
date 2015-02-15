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
#ifndef __NXLIGHT_H__
#define __NXLIGHT_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage Lights in the 3d scene
class NxGraphics_Export NxScene3DObjectLight : public NxEntity 
{
public :
	//! NxScene3DObjectLight Class Constructor.
	NxScene3DObjectLight( NxNode * ParentNode, const std::string & LightName, NxLightType Type );
	//! NxScene3DObjectLight Class Destructor.
	~NxScene3DObjectLight();
	//! Update Light;
	void Update( float time );
	//! Set Light Position.
	void SetPosition( const Nx::Vector3 & Position );
	//! Set Light Intensity if Applicable.
	void SetIntensity( float Intensity );
	//! Set Light Diffuse Colour.
	void SetDiffuseColour( const NxColourValue & Colour );
	//! Set light Specular Colour.
	void SetSpecularColour( const NxColourValue & Colour );
	//! Set Light Type.
	void SetLightType( const NxLightType & Type );
	//! Get Light Type.
	const NxLightType & GetLightType() const;
	//! Set Light Direction If Applicable.
	void SetDirection( const Nx::Vector3 & Direction );
	//! Set Light Cone Angle If Applicable.
	void SetConeAngle( float Angle );
	//! Set Frustrum Near And Far Plane.
	void SetFrustum( float NearPlane, float FarPlane );
	//! Set Projective Texture.
	void SetTexture( const std::string & TextureName );
	//! View Light Debug.
	void SetDebugEnabled( bool Debug );
private :
	Ogre::Light * mLight;
};

}

#endif