#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxTextureImage::NxTextureImage() : NxTexture() {

}

NxTextureImage::~NxTextureImage() {

}

bool NxTextureImage::openImageFile( const std::string & TextureName, const std::string& filename ) {
 
	mImage = new NxImage();
	if( mImage->openImageFile( filename ) ) {

		Type = TYPE_IMAGE;
		NxTextureName = TextureName;

		create( TextureName, "NxMaterialVideo",  mImage->GetWidth(),  mImage->GetHeight(), 0, NXTEX_TYPE_2D, (NxPixelFormat )  mImage->GetBpp() == 4 ? NXPF_A8R8G8B8 : NXPF_BYTE_BGR, NXTU_STATIC );
 
		// NxPixelBuffer = mTexture[0]->getBuffer(0,0).get();
		mTexture[0]->loadImage( mImage->GetImage() );
		mFrameBuffer = new NxFrameBufferImage( this );

		//NxPixelBuffer = mTexture[0]->getBuffer(0,0).get();
	}
	
	mTexture[0]->loadImage( mImage->GetImage() );
	return true;
}

bool NxTextureImage::createCanvas( const std::string & TextureName, unsigned int width, unsigned int height, unsigned int bpp ) {

	mImage = new NxImageCanvas();
	((NxImageCanvas*)mImage)->createImageFile(  width, height, bpp );
	Type = TYPE_CANVAS;
	return true;
}

/*
void NxTextureImage::loadImage( NxImage * img, int plane ) {
	mTexture[plane]->loadImage( img->GetImage() );
}*/

unsigned int NxTextureImage::GetImageWidth() {
	return mImage->GetWidth();
}

unsigned int NxTextureImage::GetImageHeight() {
	return mImage->GetHeight();
}

unsigned int NxTextureImage::GetImageBpp() {
	return mImage->GetBpp();
}
 
NxImage * NxTextureImage::GetImage() {
	return mImage;//.get();
}

//--------------------------------------------------------

NxLayerImage::NxLayerImage( int Width, int Height ) : NxLayer( Width, Height ) 
{

}

NxLayerImage::~NxLayerImage()
{

}

}