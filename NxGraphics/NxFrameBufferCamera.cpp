#include "NxGraphicsPreCompiled.h"
#include "../NxVideo/NxVideo_Grabber.h"

namespace Nx {

	NxFrameBufferCamera::NxFrameBufferCamera( NxTexture * Layer ) : NxFrameBuffer( Layer )
	{

	}

	NxFrameBufferCamera::~NxFrameBufferCamera()
	{
		LogMsg("~NxFrameBufferCamera() ... ");

	}

	bool NxFrameBufferCamera::UpdateTexture( float time )
	{

		NxVideoCaptureDeviceType Type =  ((NxTextureCamera*)mLayer)->GetCamera()->GetCaptureDeviceType(); //mLayer->

		if( ((NxTextureCamera*)mLayer)->GetCamera()->NewFrame() )
		{

			//Log( "FORMAT : " + Ogre::StringConverter::toString( LayerPixelBuffer->getFormat() ) );

			LayerPixelBox->data = ((NxTextureCamera*)mLayer)->GetCamera()->GetBuffer();
			ProcessVerticalPixelBufferFlip();
			mLayer->GetPixelBuffer()->blitFromMemory( *LayerPixelBox );
	

			//memcpy( LayerPixelBox.data , mLayer->mNxVideoDevice->GetBuffer(),  LayerWidth * LayerHeight * 3 );

			//LayerBuffer = mLayer->mNxVideoDevice->GetBuffer();
			//LayerPixelBuffer->blitFromMemory()

			//	LayerPixelBuffer->writeData(0, LayerWidth * LayerHeight * 3, mLayer->mNxVideoDevice->GetBuffer(), true );

			//int width = LayerPixelBox.getWidth();
			//int height = LayerPixelBox.getHeight();

			//Log( "Width : " + Ogre::StringConverter::toString( width ) );
			//Log("NOW");
			//LayerPixelBuffer->getHeight();
			//LayerPixelBuffer->blitFromMemory( LayerPixelBox );

			//Log("OK");
		}
		//
		//if( LayerPixelBox.data  != 0 )
		//{
		//	ProcessVerticalPixelBufferFlip();
		//	ProcessVideoPlugins();
		//}

		return true;

	}

}