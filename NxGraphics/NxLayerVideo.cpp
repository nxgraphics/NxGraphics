#include "NxGraphicsPreCompiled.h"
#include "../nxvideo/NxVideo_Headers.h"

namespace Nx {

NxTextureVideo::NxTextureVideo() : NxTexture(), mNxVideoFile(0) {

	mAudioSampleRate = 0;
	mAudioChannels = 0;
	mAudioBufferSize = 0;
	mAudioBitsPerSamples = 0;

	Type = TYPE_VIDEO;

	

}

NxTextureVideo::~NxTextureVideo()
{
	LogMsg( "Deleting NxTextureVideo... " );
	for( int i = 0 ; i < mTexture.size(); i++ ) {
		Ogre::TextureManager::getSingleton().unload( mTexture[i]->getName() );
		Ogre::TextureManager::getSingleton().remove( mTexture[i]->getName() );
		Ogre::TextureManager::getSingleton().destroyResourcePool(  mTexture[i]->getName()  );

	}
	NxVideoManager::getSingleton().CloseVideoFile( mNxVideoFile );

	mFrameBuffer->Release();
	delete mFrameBuffer;
	mFrameBuffer = NULL;

	LogMsg( "Deleting NxTextureVideo : Done" );
 
}


struct VideoPlaneInfo {

	int width;
	int height;
	int bpp;
	int format;
};

NxPixelFormat GetFormat( int plane, NxVideoPixelFormat format, VideoPlaneInfo & info ) {
	
	// TODO info.bpp = 4;
	NxPixelFormat pixelformat;

	switch( format ) {
		// 32 bits
		case NxVideoPixelFormat32BGRA : pixelformat = NXPF_B8G8R8A8; break;
		case NxVideoPixelFormat32ARGB : pixelformat = NXPF_A8R8G8B8; break;
		case NxVideoPixelFormat32ABGR : pixelformat = NXPF_A8B8G8R8; break;
		case NxVideoPixelFormat32RGBA : pixelformat = NXPF_R8G8B8A8; break;
		// 24 bits
		case NxVideoPixelFormat24RGB  : pixelformat = NXPF_R8G8B8; break;
		case NxVideoPixelFormat24BGR  : pixelformat = NXPF_B8G8R8; break;

		// 16 bits
		case NxVideoPixelFormat2vuy  : pixelformat = NXPF_A8R8G8B8; info.width /= 2; break;

//  on adreno
			//http://fgiesen.wordpress.com/2013/09/25/texture-uploads-on-android/
		// 8 bits
		case NxVideoPixelFormaty420 : pixelformat = NXPF_L8; break;
 //case NxVideoPixelFormaty420 : pixelformat = NXPF_R8G8B8A8; info.width /= 4; break;


		default :
			LogMsg("NxGraphics_LayerVideo.cpp GetFormat : no format defined");
	}

	return pixelformat;
}

void GetVideoPlaneInfo( NxVideo_Player * player, std::vector<VideoPlaneInfo> & info ) {

	NxVideoPixelFormat format = player->GetVideoFormat();
	for( int i = 0 ; i < player->GetNumPlanes(); i++ ) {
		VideoPlaneInfo plane;
		plane.width = player->GetWidth( i );
		plane.height = player->GetHeight( i );
		plane.format = GetFormat( i, format, plane );
		info.push_back( plane );
	}

}

bool NxTextureVideo::OpenVideoFile( const std::string & filename, const std::string & TextureName, bool useOpenGL, NxVideoPixelFormatInfo & info ) { 

	mNxVideoFile = NxVideoManager::getSingleton().OpenVideoFile( filename, info );

	Type = TYPE_AVI;

	if( mNxVideoFile->HasAudio() ) {
		mHasAudio = true;
		mAudioSampleRate = mNxVideoFile->GetAudioSampleRate() ;
		mAudioChannels = mNxVideoFile->GetAudioNumChannels();
		mAudioBufferSize = mNxVideoFile->GetAudioBufferSize();
		mAudioBitsPerSamples = mNxVideoFile->GetAudioBitsperSamples();
	}

	if( mNxVideoFile->HasVideo() ) {

		NxVideoPixelFormat format = mNxVideoFile->GetVideoFormat();

		std::vector<VideoPlaneInfo> info;
		GetVideoPlaneInfo( mNxVideoFile, info );

		for( int i = 0 ; i < info.size(); i++ ) { 
			int numMipMaps = 0;
			Ogre::TexturePtr tex = TextureManager::getSingleton().createManual( 
				TextureName+"_"+Ogre::StringConverter::toString( i ), "NxMaterialVideo", TEX_TYPE_2D,
				info[i].width, info[i].height, numMipMaps, (Ogre::PixelFormat)info[i].format, TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );

			mTexture.push_back( tex.get() );
			mNxVideoFile->SetOpenGLID( i, GetTextureID( i ) );

		}

		mFrameBuffer = new NxFrameBufferVideo( this );
 
		//NxPixelBuffer = mVideoTexture->mTexture->getBuffer(0,0).get();



		//if( mVideoTexture->NxPixelBuffer->getFormat() != mVideoTexture->FormatType ) {
		//	LogMsg("----- ERROR texture depth INVALID2 !");
		//	// doesnt matter so much since when uploading to gpu, pixelbox will be converted...
		//	LogMsg("pixel buffer format : " + Ogre::StringConverter::toString( mVideoTexture->mTexture->getFormat() ) );
		//	LogMsg("texture format : " + Ogre::StringConverter::toString( mVideoTexture->FormatType ) );
		//} 




		//mVideoTexture->Width = mVideoTexture->GetVideoWidth();
		//mVideoTexture->Height = mVideoTexture->GetVideoHeight();
		//mVideoTexture->NxTextureName = TextureName;
		//mVideoTexture->Bpp = mVideoTexture->GetVideoBpp();
		//mVideoTexture->Bpp == 4 ? mVideoTexture->FormatType = NXPF_A8R8G8B8 : mVideoTexture->FormatType = NXPF_BYTE_BGR; // PixelFormat::PF_BYTE_BGRA // -> PF_A8R8G8B8




 
	}




	return mNxVideoFile != NULL;
}

unsigned int NxTextureVideo::GetVideoWidth() {
	return mNxVideoFile->GetWidth();
}

unsigned int NxTextureVideo::GetVideoHeight() {
	return mNxVideoFile->GetHeight();
}

unsigned int NxTextureVideo::GetVideoBpp() {
	return mNxVideoFile->GetBpp();
}

bool NxTextureVideo::IsVideoFormatPlanar() {
	return mNxVideoFile->IsPlanar();
}

NxVideoPixelFormat NxTextureVideo::GetVideoFormat() {
	return mNxVideoFile->GetVideoFormat();
}

NxVideo_Player * NxTextureVideo::GetVideo()
{
	return mNxVideoFile; //mLayer.
}

void NxTextureVideo::Stop()
{
	 mNxVideoFile->Stop();
}

void NxTextureVideo::Play()
{
	 mNxVideoFile->Play();
}

void NxTextureVideo::Rewind()
{
	 mNxVideoFile->Rewind();
}


const string & NxTextureVideo::GetVideoFileName() const
{
	return  mNxVideoFile->GetVideoName();
}

//------------------------------------

NxLayerVideo::NxLayerVideo( int Width, int Height ) : NxLayer( Width, Height )
{

}

NxLayerVideo::~NxLayerVideo()
{
	LogMsg( "Delete ~NxLayerVideo()");
}

NxTextureVideo * NxLayerVideo::GetTexture()
{

	return dynamic_cast<NxTextureVideo*>( NxTextureManager::getSingleton().GetTexture( NxEntity->getSubEntity(0)->getMaterialName() ));
 
	//NxEntity->getSubEntity(0)->getMaterialName()
}


}