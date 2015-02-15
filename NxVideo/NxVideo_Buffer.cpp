#include "NxVideoPreCompiled.h"

namespace NxVideo_Namespace {

NxVideoBuffer::NxVideoBuffer( int Width , int Height, int Bpp )
{
	mVideoData = new unsigned char[ Width * Height * Bpp ];
	mVideoWidth = Width;
	mVideoHeight = Height ;
	mVideoBpp = Bpp;

	for ( int i = 0 ; i <  ( Width * Height * Bpp ) ; i++ ) {
		mVideoData[i] = 255;
	}
}

NxVideoBuffer::~NxVideoBuffer()
{
	delete mVideoData;
}

void NxVideoBuffer::GetPixelData( unsigned char * data  )
{
	memcpy( &data[0], mVideoData, mVideoWidth*mVideoHeight*mVideoBpp );
}
 

}