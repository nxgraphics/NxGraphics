#include "NxGraphicsPreCompiled.h"

namespace Nx {
 
NxFrameBufferImage::NxFrameBufferImage( NxTexture * Layer ) : NxFrameBuffer( Layer )
{


}

NxFrameBufferImage::~NxFrameBufferImage()
{


}


bool NxFrameBufferImage::UpdateTexture( float time )
{
 
	if( ((NxTextureImage*)mLayer)->GetImage()->IsDirty() ) { // canvas image update
		 mLayer->GetPixelBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		 const Ogre::PixelBox& pixelBox=mLayer->GetPixelBuffer()->getCurrentLock();
		 unsigned char * pDest=static_cast<unsigned char*>(pixelBox.data);
		 bool IsAlpha = ((NxTextureImage*)mLayer)->GetImage()->GetBpp() == 4 ? true : false;
		 unsigned int widthStep = (mBpp * mWidth) ;
		 uchar * src = ((NxTextureImage*)mLayer)->GetImage()->GetBuffer();

		 for(size_t i = 0; i < mHeight; i++)
			 for (size_t j = 0; j < mWidth; j++) {
				 int blue,green,red,alpha;
				 red = ((uchar*)(src + i * widthStep))[j*mBpp];
				 green = ((uchar*)(src + i * widthStep))[j*mBpp+1];
				 blue = ((uchar*)(src + i * widthStep))[j*mBpp+2] ;
				 alpha = ((uchar*)(src + i * widthStep))[j*mBpp+3];
				 *pDest++ = (Ogre::uint8) blue;// blue
				 *pDest++ = (Ogre::uint8) green;//green
				 *pDest++ = (Ogre::uint8) red;//red
				 *pDest++ = (Ogre::uint8) 0;
			 }
		 mLayer->GetPixelBuffer()->unlock();
 
		 ((NxTextureImage*)mLayer)->GetImage()->SetDirty(false);


	}
 
	//NxImageCanvasContext * ctx =  ( NxImageCanvasContext * ) ((NxImageCanvas*)mNxImage.get())->GetContext()  ;
	//if( ((NxImageCanvas*)mNxImage.get())->mCallBack ) ((NxImageCanvas*)mNxImage.get())->mCallBack->OnCanvasUpdate( *ctx );
	//LayerPixelBox.data = ctx->getData();
	//ProcessVideoPlugins();

	return true;
}

}//namespace
 