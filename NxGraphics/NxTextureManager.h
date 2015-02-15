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
#ifndef __NXTEXTUREMANAGER_H__
#define __NXTEXTUREMANAGER_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxTextureManager : public NxSingleton<NxTextureManager>
{
	typedef map<std::string, NxTexture*>::type NxTextureMap;
	typedef NxMapIterator<NxTextureMap> NxTextureMapIterator;
public :
	//! NxOverlayManager class constructor
	NxTextureManager();
	//! NxOverlayManager class destructor
	~NxTextureManager();
	//! NxOverlayManager Singleton.
	static NxTextureManager& getSingleton();
	//! NxOverlayManager Singleton.
	static NxTextureManager* getSingletonPtr();
	//! get Texture Iterator.
	NxTextureMapIterator GetTextureIterator();
	//! load texture
	bool LoadTexture( const std::string & TextureName, const std::string & Group );
	//! Texture exists
	bool TextureExists( const std::string & TextureName );
	//! Get Texture by name.
	NxTexture * GetTexture( const std::string & TextureName );
	//! create a texture
	//NxTexture * CreateNxTexture( const std::string & TexName, int Width, int Height, NxPixelFormat format );
	//! create a texture rtt
	//NxTextureImage * CreateRenderTexture(  const std::string & TexName, Ogre::Camera * Cam, int Width = 320, int Height = 240 );
	//! create a depth texture
	//NxTextureImage * CreateDepthTexture(  const std::string & TexName, Ogre::Camera * Cam, int Width = 320, int Height = 240 );
	//! create a video texture.
	NxTextureVideo * CreateTextureVideo( const std::string & TextureName, const std::string & FileName, NxVideoPixelFormatInfo & info, bool UseOpenGlUpdate = false );
	//! create a 3d scene rtt
	//NxTexture3D * CreateTexture3D( const std::string & TextureName, const std::string & FileName, NxGraphics_3D_Scene_Types SceneType, int Width = 320, int Height = 240, int FSAA = 8 );
	//! create a camera texture.
	NxTextureCamera * CreateTextureCamera( const std::string & TextureName, const std::string & DeviceName, unsigned int DevType, int Width, int Height, int Bpp );
	//! create a image texture
	NxTextureImage * CreateTextureImage( const std::string & TextureName, const std::string & FileName );
	//! create a canvas texture.
	NxTextureImage * CreateTextureCanvas( const std::string & TextureName, int Width = 320, int Height = 240 , int Bpp = 4 );
	//! create a simple texture
	NxTexture * CreateTexture( const std::string & TexName, const std::string & textureGroupName, int Width = 256, int Height = 256, int numMips = 0, NxTextureType type = NXTEX_TYPE_2D, NxPixelFormat format = NXPF_R8G8B8, NxTextureUsage usage = NXTU_DEFAULT );
	//! create a web texture.
	NxTextureWeb * CreateWebTexture( const std::string & TextureName, const std::string & Link, int Width, int Height );	
	//! unload texture.
	void Unload( const std::string & TextureName );
	//! remove texture
	void Remove( const std::string & TextureName );
	//! update texture.
	void Update( float Time );

private :
	NxTextureMap mTextures;
};

}//namespace

#endif