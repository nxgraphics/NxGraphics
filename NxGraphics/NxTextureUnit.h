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
#ifndef __NXTEXTUREUNIT_H__
#define __NXTEXTUREUNIT_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxTextureUnit
{
public :
	//! NxTextureUnit Class Constructor.
	NxTextureUnit( NxPass * parent, const std::string & TextureUnitName );
	//! NxTextureUnit Class Destructor.
	~NxTextureUnit();
	//! Get Material Name.
	const std::string & GetName() const;
	//! Set Colour Operation.
	void SetColourOperation( const NxLayerBlendOperation op );
	//! Set Texture Scale.
	void SetTextureScale( float ScaleX, float ScaleY );
	//! Set Texture Rotation.
	void SetTextureRotate( float degree );
	//! set texture transform
	void SetTextureTransform( const Nx::Matrix4 & Mat );
	//! Set Texture based on File( jpg. etc ... ).
	void SetTextureFile( const std::string & TextureFile );
	//! Set Texture based on Name.
	void SetTextureName( const std::string & TextureName );
	//! set texture name, with texture type.
	void SetTextureName( const std::string & TextureName, NxTextureType Type );
	//! Get Texture Unit.
	Ogre::TextureUnitState * GetTextureUnit();
	//! Set Alpha Operation.
	void SetAlphaOperation( NxLayerBlendOperationEx op, 
		NxLayerBlendSource source1 = NXLBS_TEXTURE,
		NxLayerBlendSource source2 = NXLBS_CURRENT,
		float arg1 = 1.0, float arg2 = 1.0, float manualBlend = 0.0 );

	//! Set Texture unit opacity( same as SetAlphaOperation ).
	void SetOpacity( float Value );
	//! set number of mipmaps
	void SetNumMipmaps( unsigned int numMips );
	//! set mip texture bias.
	void SetTextureMipmapBias( float bias );
	//! set texture anisotropy.
	void SetTextureAnisotropy( unsigned int MaxAnisotropy );
	void SetTextureBorderColour( const NxColourValue& BorderColour );
	void SetTextureAddressingMode( NxTextureAddressingMode tam );
	void SetTextureFiltering( NxTextureFilterOptions filterType );
	void SetTextureFiltering( NxFilterType ftype, NxFilterOptions opts );
	/** Set a the detailed filtering options on this texture unit. 
	@param minFilter
	The filtering to use when reducing the size of the texture. Can be NXFO_POINT, NXFO_LINEAR or NXFO_ANISOTROPIC.
	@param magFilter
	The filtering to use when increasing the size of the texture. Can be NXFO_POINT, NXFO_LINEAR or NXFO_ANISOTROPIC.
	@param mipFilter
	The filtering to use between mip levels. Can be NXFO_NONE (turns off mipmapping), NXFO_POINT or NXFO_LINEAR (trilinear filtering).
	*/
	void SetTextureFiltering( NxFilterOptions minFilter, NxFilterOptions magFilter, NxFilterOptions mipFilter);
	void SetTextureCoordSet( unsigned int setCoord );
	void SetCubicTextureName( const std::string & name, bool forUVW = false );
	void SetEnvironmentMap( bool enable, NxEnvMapType envMapType = ENV_CURVED);
	unsigned int GetTextureWidth();
	unsigned int GetTextureHeight();

private :
	Ogre::TextureUnitState * mTextureUnit;
};

}//namespace

#endif