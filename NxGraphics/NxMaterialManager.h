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
#ifndef __NXMATERIALMANAGER_H__
#define __NXMATERIALMANAGER_H__

#include "NxPrerequisites.h"

namespace Nx {

//!  A class to manage a materials
/*!
	Material Creation, management.
*/
class NxGraphics_Export NxMaterialManager : public NxSingleton<NxMaterialManager>
{
private :
	typedef map<std::string, MaterialNx* >::type NxMaterialList;
	typedef NxMapIterator<NxMaterialList> NxMaterialMapIterator;
    NxMaterialList mMaterials;

	typedef map<std::string, NxShader* >::type NxShaderList;
	typedef NxMapIterator<NxShaderList> NxShaderMapIterator;
	NxShaderList mShaders;

public :
	//! NxMaterialManager Class Constructor.
	NxMaterialManager();
	//! NxMaterialManager Class Destructor.
	~NxMaterialManager();
	//! NxMaterialManager Singleton.
	static NxMaterialManager& getSingleton();
	//! NxMaterialManager Singleton.
	static NxMaterialManager* getSingletonPtr();
	//! Create a Material
	MaterialNx * CreateMaterial( const std::string & MaterialName );
	//! Destroy a Material
	void DestroyMaterial( const std::string & MaterialName );
	//! material already exists.
	bool MaterialExists( const std::string & materialname );
	//! get material by name
	MaterialNx * GetMaterial( const std::string & MaterialName );  
	//! Create a Vertex program definition.
	NxVertexShader * CreateVertexProgram( const std::string & VertexProgramName, const std::string & ShaderType = "glsl" );
	//! Create a Pixel program definition.
	NxPixelShader * CreateFragmentProgram( const std::string & FragmentProgramName, const std::string & ShaderType = "glsl" );
	//! Create a Geometry program definition.
	NxGeometryShader * CreateGeometryProgram( const std::string & GeometryProgramName, const std::string & ShaderType = "glsl" );
	//! Destroy program.
	void DestroyProgram( const std::string & ProgramName );
	//! Create a cubeMap Material.
	void CreateCubemapMaterial( const std::string & MaterialName, Ogre::Entity * Ent );
};


}//namespace

#endif