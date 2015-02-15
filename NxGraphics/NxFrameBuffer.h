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
#ifndef __NXFRAMEBUFFER_H__
#define __NXFRAMEBUFFER_H__

#include "NxPrerequisites.h"
 
namespace Nx {

class NxGraphics_Export NxFrameBuffer
{	
protected :	
	//! NxFrameBuffer class Constructor.
	NxFrameBuffer( NxTexture * Layer );  
public :
	//! NxFrameBuffer class Deconstructor.
	virtual~NxFrameBuffer();
	//! Release Frame Buffer Resource.
	bool Release();
	//! Update Dynamic Texture.
	virtual bool UpdateTexture( float time ) = 0;
	//! Set Vertex Position
	void SetVertexPos( NxQuadVertex Type, float PosX, float PosY );
	//! save pixel buffer to file
	void SaveToFile( const std::string & FileName );
protected :

	NxTexture * mLayer;

	void ProcessVideoPlugins();
	void Process3DPlugins();
	bool UpdateVertexBuffer();
	void ProcessVerticalPixelBufferFlip();
	int AssociatedLayerNumber;
	std::vector<Nx::Vector2> NxVertexList;
	Ogre::PixelBox * LayerPixelBox;
	int mWidth ;
	int mHeight ;
	int mBpp ;
	int mType;
	NxPixelFormat mFormat;
	unsigned char * LayerBuffer;
	char * mAudioBuffer;
	int64 mAudioSampleindex;
	NxSoundMediaFileBuffer * mSound ;
};

class NxFrameBufferVideo : public NxFrameBuffer
{
public :
	NxFrameBufferVideo( NxTexture * Layer );
	~NxFrameBufferVideo();
	bool UpdateTexture( float time );
 
};

class NxFrameBufferCamera : public NxFrameBuffer
{
public :
	NxFrameBufferCamera( NxTexture * Layer );
	~NxFrameBufferCamera();
	bool UpdateTexture( float time );
 
};

class NxFrameBufferImage : public NxFrameBuffer
{
public :
	NxFrameBufferImage( NxTexture * Layer );
	~NxFrameBufferImage();
	bool UpdateTexture( float time );
 
};


class NxFrameBuffer3D : public NxFrameBuffer
{
public :
	NxFrameBuffer3D( NxTexture * Layer );
	~NxFrameBuffer3D();
	bool UpdateTexture( float time );
 
};

class NxFrameBufferCanvas : public NxFrameBuffer
{
public :
	NxFrameBufferCanvas( NxTexture * Layer );
	~NxFrameBufferCanvas();
	bool UpdateTexture( float time );
 
};


}  
#endif