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
#ifndef __NXMATERIAL_H__
#define __NXMATERIAL_H__

#include "NxPrerequisites.h"

namespace Nx {

//!  A class to manage materials
/*!
	Technique Creation, management.
*/
class NxGraphics_Export MaterialNx
{
private :
    typedef vector<NxTechnique*>::type Techniques;
	//! All techniques supported.
    Techniques mTechniques;
public :
	//! MaterialNx Class Constructor.
	MaterialNx( const std::string & MaterialName );
	//! MaterialNx Class Destructor.
	~MaterialNx();
	//! load material.
	void Load( bool backgroundThread = false );
	//! reload material
	void Reload();
	//! compile
	void Compile( bool autoManageTextureUnits = true );
	//! prepare
	void Prepare( bool backgroundThread = false );
	//! Get Material Name.
	const std::string & GetName() const;
	//! Set Ambient.
	void SetAmbient( const NxColourValue & ambient );
	//! Set Diffuse.
	void SetDiffuse( const NxColourValue & diffuse );
	//! Set Specular.
	void SetSpecular( const NxColourValue & specular );
	//! material affected by light ?
	void SetLightingEnabled( bool Enabled );
	//! Set material self illumination colour.
	void SetSelfIllumination( float red, float green, float blue );
	//! Set material receive shadows.
	void SetReceiveShadows( bool ReceiveShadows );
	//! Set Culling Mode.
	void SetCullingMode( NxCullingMode Mode );
	//! create a technique
	NxTechnique * CreateTechnique();
	//! Create a Technique with a specified name.
	NxTechnique * CreateTechnique( const std::string & TechniqueName );
	//! Get Technique by Index.
	NxTechnique * GetTechnique( unsigned short index );
	//! Get Technique by Name.
	NxTechnique * GetTechnique( const std::string & name );
	//! Get total number of techniques.
	unsigned short GetNumTechniques() const;
	//! Get Ogre Material
	Ogre::Material * GetMaterial();
private :
	Ogre::Material * mMaterial;
};

}//namespace

#endif