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
#ifndef __NXGRAPHICSTEXTURE_H__
#define __NXGRAPHICSTEXTURE_H__

#include "NxPrerequisites.h"
#include "../NxVideo/NxVideo_Format.h"

using namespace NxVideo_Namespace;

namespace Nx {

class NxGraphics_Export NxTexture
{
protected :

	friend class NxTextureManager;
	//! constructor
	NxTexture();
	//! internal create
	void create( const std::string & TexName, const std::string & textureGroupName, int Width, int Height, int numMips, NxTextureType type, NxPixelFormat format, NxTextureUsage usage );
public:
	//! destructor
	virtual ~NxTexture();
	//! get texture internal ID.
	unsigned int GetTextureID( int plane = 0 );
	//! Get width
	unsigned int GetWidth( int plane = 0 );
	//! Get height
	unsigned int GetHeight( int plane = 0 );
	//! get total pixel size.
	unsigned long GetPixelSize( int plane = 0 );
	//! get Bpp
	unsigned int GetBpp( int plane = 0 );
	//! get num planes
	size_t getNumPlanes();
	//! pixel buffer
	Ogre::HardwarePixelBuffer * GetPixelBuffer( int plane = 0 ) const;
	//! get texture name.
	const std::string & GetTextureName( int plane = 0 ) const;
	//! set blank
	void SetMuted( bool Muted );
	//! prepare texture
	void Prepare( int plane = 0, bool BackgroundThread = false);
	//! Returns true if the Resource has been loaded, false otherwise. 
	bool isLoaded( int plane = 0  ) const;
	//!Returns whether the resource is currently in the process of background loading. 
	bool isLoading( int plane = 0  ) const;
	//!Is this resource manually loaded? 
	bool isManuallyLoaded( int plane = 0 ) const;
	//!Returns true if the Resource has been prepared, false otherwise. 
	bool isPrepared( int plane = 0  ) const;
	//! Returns true if the Resource is reloadable, false otherwise.
	bool isReloadable( int plane = 0 ) const ;
	//! load texture
	void Load( int plane = 0, bool BackgroundThread = false );
	//! reload texture
	void Reload( int plane = 0 );
	//! unload texture
	void Unload(int plane = 0 );
	//! free texture internal resources.
	void FreeInternalResources(int plane = 0 );
	//! create texture internal resources.
	void CreateInternalResources(int plane = 0 );


	virtual void updateTexture( float time );

	const int getType() const {
		return Type;
	}

	const int getFormat() const {
		return FormatType;
	}

	const int GetIndex() const {
		return LayerNumber;
	}

	bool hasAudio() {
		return mHasAudio;
	}

	unsigned int getAudioSampleRate() {
		return mAudioSampleRate;
	}

	unsigned int getAudioChannels() {
		return mAudioChannels;
	}

	unsigned int getAudioBufferSize() {
		return mAudioBufferSize;
	}

	unsigned int getAudioBitsPerSamples() {
		return mAudioBitsPerSamples;
	}
 
 
protected: 

	//! Layer Name.
	std::string LayerName  ;

	//! Layer Texture Unit.
	std::vector<Ogre::Texture *>  mTexture;

	//! Layer Texture Unit Name.
	std::string NxTextureName;
	//! texture pixel format
	int FormatType;


	//! Layer Type.
	int Type ;
	//! layer Z Order.
	int Depth;
	float mOpacity;
	bool mMuted;
	//! layer FrameBuffer.
	NxFrameBuffer * mFrameBuffer;
	//! Layer Effects Container.
	std::list< NxPlugin * > EffectPluginList;
	int LayerNumber ;
	shared_ptr <NxVideoBuffer> NxBuffer;

	bool mHasAudio;
	unsigned int mAudioSampleRate;
	unsigned int mAudioChannels;
	unsigned int mAudioBufferSize;
	unsigned int mAudioBitsPerSamples;

};

class NxGraphics_Export NxTextureVideo : public NxTexture
{
public:
	NxTextureVideo();
	~NxTextureVideo();
	bool OpenVideoFile( const std::string & filename,  const std::string & TextureName, bool useOpenGL, NxVideoPixelFormatInfo & info );
	unsigned int GetVideoWidth();
	unsigned int GetVideoHeight();
	unsigned int GetVideoBpp();
	const string & GetVideoFileName() const;
	void Stop();
	void Play();
	void Rewind();
	bool IsVideoFormatPlanar();
	NxVideoPixelFormat GetVideoFormat();
	NxVideo_Player * GetVideo();
private :

	NxVideo_Player * mNxVideoFile;
};

class NxGraphics_Export NxTextureCamera : public NxTexture
{
public:
	NxTextureCamera();
	~NxTextureCamera();
	bool openVideoDevice( const std::string & textureName, int index, int width, int height, int bpp, int devType );
	unsigned int GetVideoWidth();
	unsigned int GetVideoHeight();
	unsigned int GetVideoBpp();
	NxVideo_Grabber * GetCamera();
private :
 
	NxVideo_Grabber * mNxVideoDevice;
};

class NxGraphics_Export NxTexture3D : public NxTexture
{
public:
	NxTexture3D();
	~NxTexture3D();
	bool CreateScene( const Nx3DSceneDesc & desc );
	Nx3D_Scene * GetScene();
private :
	Nx3D_Scene * mNx3DScene;
};

class NxGraphics_Export NxTextureWeb : public NxTexture
{
public:
	NxTextureWeb();
	~NxTextureWeb();
	NxVideo_Player * GetWeb();
};

class NxGraphics_Export NxTextureImage : public NxTexture
{
public:
	NxTextureImage();
	~NxTextureImage();
	//! load nximage one texture.
	//void loadImage( NxImage * img, int plane = 0 );
	//! open image from path
	bool openImageFile( const std::string & textureName, const std::string& filename );
	
	bool createCanvas( const std::string & textureName, unsigned int width, unsigned int height, unsigned int bpp );
 

	unsigned int GetImageWidth();
	unsigned int GetImageHeight();
	unsigned int GetImageBpp();
	NxImage * GetImage();
private :
	//NxImagePtr mNxImage;
	NxImage * mImage;
};

class NxGraphics_Export NxTextureStream : public NxTexture
{
public:
	NxTextureStream();
	~NxTextureStream();
	NxVideo_Player * GetStream();
};

} // namespace

#endif