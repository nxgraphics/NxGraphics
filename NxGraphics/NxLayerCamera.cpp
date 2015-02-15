#include "NxGraphicsPreCompiled.h"
#include "../nxvideo/NxVideo_Headers.h"

namespace Nx {

NxTextureCamera::NxTextureCamera() : NxTexture(), mNxVideoDevice(NULL) {

	mAudioSampleRate = 0;
	mAudioChannels = 0;
	mAudioBufferSize = 0;
	mAudioBitsPerSamples = 0;

	Type = TYPE_VIDEO_CAPTURE;
}

NxTextureCamera::~NxTextureCamera()
{
	if( mNxVideoDevice ) mNxVideoDevice->CloseVideoDevice();
}

bool NxTextureCamera::openVideoDevice( const std::string & textureName, int index, int width, int height, int bpp, int devType ) 
{
	mNxVideoDevice = NxVideoManager::getSingleton().OpenVideoDevice( index, width, height, bpp, (NxVideoCaptureDeviceType) devType );

	if( mNxVideoDevice ) {

		 NxTextureName = textureName ;	 
		 FormatType = Ogre::PF_R8G8B8;

		 create( 
			 textureName,
			 "NxMaterialVideo",
			 mNxVideoDevice->GetWidth(),
			 mNxVideoDevice->GetHeight(),
			 0,
			 NXTEX_TYPE_2D,
			 NXPF_R8G8B8,
			 NXTU_DYNAMIC_WRITE_ONLY_DISCARDABLE );

		 mFrameBuffer = new NxFrameBufferCamera( this );

		 LogMsg("NxTextureCamera: Creating Camera Texture " ); 
		 LogMsg("Width : " + Ogre::StringConverter::toString( mNxVideoDevice->GetWidth()) );  
		 LogMsg("Height : " + Ogre::StringConverter::toString(mNxVideoDevice->GetHeight() ) );  
		 LogMsg("Bpp : " + Ogre::StringConverter::toString( mNxVideoDevice->GetBpp() ));



	}

	return mNxVideoDevice != NULL;
}

unsigned int NxTextureCamera::GetVideoWidth() {
	return mNxVideoDevice->GetWidth();
}

unsigned int NxTextureCamera::GetVideoHeight() {
	return mNxVideoDevice->GetHeight();
}

unsigned int NxTextureCamera::GetVideoBpp() {
	return mNxVideoDevice->GetBpp();
}

NxVideo_Grabber * NxTextureCamera::GetCamera()
{
	return mNxVideoDevice;
}
//------------------------------------------------

NxLayerCamera::NxLayerCamera( int Width, int Height ) : NxLayer( Width, Height )
{

}

NxLayerCamera::~NxLayerCamera()
{

}


}