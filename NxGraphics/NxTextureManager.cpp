#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "../nxvideo/NxVideo_Headers.h"

#ifdef __ANDROID__
#include <OgreGLES2Texture.h>
#include <android/log.h>
#define  LOG_TAG    "TEXTURE BASE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

namespace Nx 
{

template<> NxTextureManager* NxSingleton<NxTextureManager>::ms_Singleton = 0;

NxTextureManager::NxTextureManager()
{

}

NxTextureManager::~NxTextureManager()
{

}

NxTextureManager& NxTextureManager::getSingleton() {
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxTextureManager* NxTextureManager::getSingletonPtr() {
	return ms_Singleton;
}

bool NxTextureManager::LoadTexture( const std::string & TextureName, const std::string & Group ) {
	Ogre::TexturePtr Tex = TextureManager::getSingleton().load( TextureName, Group );
	return Tex->isLoaded();
}

NxTextureManager::NxTextureMapIterator NxTextureManager::GetTextureIterator()
{ 
	return NxTextureMapIterator( mTextures.begin(), mTextures.end() );
}

bool NxTextureManager::TextureExists( const std::string & TextureName ) {
	bool exists = true;
	NxTextureMap::const_iterator i = mTextures.find( TextureName );
	if (i == mTextures.end()){ exists = false; } 
	return exists;	
	//return TextureManager::getSingleton().resourceExists( TextureName );

}

NxTexture * NxTextureManager::GetTexture( const std::string & TextureName )
{
	NxTextureMap::const_iterator i = mTextures.find( TextureName );
	if (i == mTextures.end()){
		OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Cannot find Texture with name " + TextureName, "NxTextureManager::GetTexture");
	}
	else{
		return i->second;
	}
}

void NxTextureManager::Unload( const std::string & TextureName )
{
	TextureManager::getSingleton().unload( TextureName );
}

void NxTextureManager::Remove( const std::string & TextureName ) {
	if( TextureExists( TextureName ) ) {
		LogMsg("NxTextureManager::Remove: deleting texture...");
		NxTexture * t = GetTexture( TextureName );
		// remove texture from NxTextureManager
		NxTextureMap::iterator i = mTextures.find( TextureName );
		mTextures.erase( i );
		// remove texture from Ogre TextureManager, should be done in 
		TextureManager::getSingleton().remove( TextureName );
		//delete class, ex: delete NxTextureVideo
		delete t;
		LogMsg("NxTextureManager::Remove: deleting texture: Done !");
	}else {
		LogMsg("Texture doesnt exists : " + TextureName );
	}

}
 
NxTexture * NxTextureManager::CreateTexture( const std::string & texName, const std::string & textureGroupName, int width, int height, int numMips, NxTextureType type, NxPixelFormat format, NxTextureUsage usage ) {
	NxTexture * t = new NxTexture();
	t->create( texName, textureGroupName, width, height, numMips, type, format, usage );// permitted as friendship
	return t;
}

NxTextureImage * NxTextureManager::CreateTextureImage( const std::string & TextureName, const std::string & FileName ) {

	NxTextureImage * mImageTexture = new NxTextureImage();
	if( mImageTexture->openImageFile( TextureName, FileName )) {
		mTextures.insert( NxTextureMap::value_type( TextureName, mImageTexture ) );
		return mImageTexture;
	}
	delete mImageTexture;
	return NULL;
}

NxTextureImage * NxTextureManager::CreateTextureCanvas( const std::string & TextureName, int width, int height, int bpp ) {

	NxTextureImage * mImageTexture = new NxTextureImage();
	if( mImageTexture->createCanvas( TextureName, width, height, bpp ) ) {
		mTextures.insert( NxTextureMap::value_type( TextureName, mImageTexture ) );
		return mImageTexture;
	} 
	delete mImageTexture;
	return NULL;
}

 

/*
NxTexture3D * NxTextureManager::CreateTexture3D( const std::string & TextureName, const std::string & FileName, NxGraphics_3D_Scene_Types SceneType, int Width , int Height, int FSAA  )
{
	if( Root::getSingleton().getRenderSystem()->getCapabilities()->getNonPOW2TexturesLimited() ) { 
		LogMsg( "NxGraphics : Non Support of Non power of two textures, errors might occur" );
		FSAA = 0;
		Width = 512 ;
		Height = 512 ;
	}
	else { 
		LogMsg( "NxGraphics : Supporting Non power of two textures" );
		LogMsg( "NxGraphics : RenderTexture Size : " + Ogre::StringConverter::toString( Width ) + " X " + Ogre::StringConverter::toString( Height ) );
	}

	NxTexture3D * Tex3D = new NxTexture3D();
	Tex3D->Type =  TYPE_3D;
	//Tex3D->Width = Width;
	//Tex3D->Height = Height;
	//Tex3D->Bpp = 4;

	LogMsg("CreateNxLayer3D : Creating renderTarget...");
	Ogre::PixelFormat format = Ogre::PF_BYTE_BGRA;
	Ogre::TexturePtr tex = TextureManager::getSingleton().createManual( TextureName, "NxMaterialVideo", TEX_TYPE_2D,
	Tex3D->GetWidth(), Tex3D->GetHeight(), 0, format, TU_RENDERTARGET, 0, false , FSAA ); 

	//Tex3D->mTexture = tex.get();
		Tex3D->mTexture.push_back( tex.get() );


	Ogre::HardwarePixelBuffer * buff  =  Tex3D->mTexture[0]->getBuffer(0,0).get();
	RenderTexture * target = buff->getRenderTarget(0);

	Nx3DSceneDesc Desc;
	Desc.mType = SceneType;
	Desc.mScenePath = FileName;
	Desc.mTarget = target;

	Tex3D->CreateScene( Desc );

	Tex3D->mFrameBuffer = new NxFrameBuffer3D( Tex3D );
	if(  mTextures.find(TextureName) != mTextures.end() )  LogMsg( "Error : Texture name already exists !!!");
	mTextures.insert( NxTextureMap::value_type( TextureName, Tex3D ) );

	return Tex3D;
}
*/



NxTextureVideo * NxTextureManager::CreateTextureVideo( const std::string & TextureName, const std::string & Filename, NxVideoPixelFormatInfo & info, bool UseOpenGlUpdate )
{
	NxTextureVideo * mVideoTexture = new NxTextureVideo();

	LogMsg("NxVideoManager::getSingleton().OpenVideoFile");
 
	bool open = mVideoTexture->OpenVideoFile( Filename, TextureName, UseOpenGlUpdate, info );
	/*
	if( mVideoTexture->GetVideo()->HasAudio() ) {
		mVideoTexture->mAudioSampleRate = mVideoTexture->GetVideo()->GetAudioSampleRate() ;
		mVideoTexture->mAudioChannels = mVideoTexture->GetVideo()->GetAudioNumChannels();
		mVideoTexture->mAudioBufferSize = mVideoTexture->GetVideo()->GetAudioBufferSize();
		mVideoTexture->mAudioBitsPerSamples = mVideoTexture->GetVideo()->GetAudioBitsperSamples();
	}


	mVideoTexture->Type = GetVideoFileType( Filename );// avi, mov, flv, swf only.
	mVideoTexture->Width = mVideoTexture->GetVideoWidth();
	mVideoTexture->Height = mVideoTexture->GetVideoHeight();
	mVideoTexture->NxTextureName = TextureName;
	mVideoTexture->Bpp = mVideoTexture->GetVideoBpp();
	mVideoTexture->Bpp == 4 ? mVideoTexture->FormatType = NXPF_A8R8G8B8 : mVideoTexture->FormatType = NXPF_BYTE_BGR; // PixelFormat::PF_BYTE_BGRA // -> PF_A8R8G8B8


	 if( info.mDstVideoFormat == NxVideoPixelFormat2vuy ){
		 mVideoTexture->FormatType =  NXPF_A8R8G8B8;//NXPF_A8R8G8B8;//NXPF_R8G8B8A8;//<<faster
		 mVideoTexture->Bpp = 4;
		 mVideoTexture->Width =  mVideoTexture->GetVideoWidth() / 2 ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight();
	 } 
	 else if( info.mDstVideoFormat == NxVideoPixelFormaty420 ) { //theora
		 mVideoTexture->FormatType = NXPF_L8;//  NXPF_A8R8G8B8 ;    // NXPF_R8G8B8A8; <<== generates error on tegra : texture's gl states do not match with shader
		 mVideoTexture->Bpp = 1;
		 mVideoTexture->Width =  mVideoTexture->GetVideoWidth();/// 2;// / 2 ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight();

	 }
	 else if( info.mDstVideoFormat == NxVideoPixelFormat24RGB) {
		 mVideoTexture->FormatType = NXPF_BYTE_RGB;
		 mVideoTexture->Bpp = 3;
		 mVideoTexture->Width = mVideoTexture->GetVideoWidth()  ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight()  ;
	 } 
	 else if( info.mDstVideoFormat == NxVideoPixelFormat32ARGB) {
		 mVideoTexture->FormatType = NXPF_A8R8G8B8; 
		 mVideoTexture->Bpp = 4;
		 mVideoTexture->Width = mVideoTexture->GetVideoWidth()  ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight()  ;
	 } 

	 // no need to use ??
	 else if( info.mDstVideoFormat == NxVideoPixelFormat24BGR) {
		 mVideoTexture->FormatType = NXPF_X8R8G8B8;
		 mVideoTexture->Bpp = 4;
		 mVideoTexture->Width = mVideoTexture->GetVideoWidth()  ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight()  ;
	 } 
	 else if( info.mDstVideoFormat == NxVideoPixelFormat32BGRA) {
		 mVideoTexture->FormatType = NXPF_B8G8R8A8; 
		 mVideoTexture->Bpp = 4;
		 mVideoTexture->Width = mVideoTexture->GetVideoWidth()  ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight()  ;
	 } 
	 else if( info.mDstVideoFormat == NxVideoPixelFormat32RGBA) {
		 mVideoTexture->FormatType = NXPF_A8R8G8B8;//NXPF_R8G8B8A8; 
		 mVideoTexture->Bpp = 4;
		 mVideoTexture->Width = mVideoTexture->GetVideoWidth()  ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight()  ;
	 }
	 else if( info.mDstVideoFormat == NxVideoPixelFormat32ABGR) {
		 mVideoTexture->FormatType = NXPF_A8B8G8R8; 
		 mVideoTexture->Bpp = 4;
		 mVideoTexture->Width = mVideoTexture->GetVideoWidth()  ;
		 mVideoTexture->Height = mVideoTexture->GetVideoHeight()  ;
	 }	 
	 else{

		 LogMsg("--- WARNING NO TEXTURE SETTINGS !!!");
	 }
 
	 int nummaps = 0;
	Ogre::TexturePtr tex = TextureManager::getSingleton().createManual( TextureName , "NxMaterialVideo" , TEX_TYPE_2D,
	mVideoTexture->Width , mVideoTexture->Height, nummaps, (Ogre::PixelFormat)mVideoTexture->FormatType, TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );//TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );//Ogre::HardwareBuffer::HBU_DYNAMIC); //TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );  
 
	mVideoTexture->mTexture = tex.get();

	// GLuint
	if( UseOpenGlUpdate ) {
		unsigned int TextureID = -1;

#ifdef __ANDROID__
		TextureID = ((GLES2Texture*)tex.get())->getGLID();
		LOGD("---> ANDROID TEXTURE MANAGER GLID ID : %d" ,  TextureID );
#else
		(mVideoTexture->mTexture)->getCustomAttribute("GLID", &TextureID );
#endif
 
		//info.mOpenglID = TextureID;
		mVideoTexture->GetVideo()->SetOpenGLID( TextureID );
		LogMsg( "Using Gl Texture ID : " + NxVideoUtils::ToString( TextureID ) );
	}
 

	if( mVideoTexture->Bpp == 4 && mVideoTexture->mTexture->getFormat() != mVideoTexture->FormatType) LogMsg("----- ERROR texture depth INVALID format : " + Ogre::StringConverter::toString( mVideoTexture->mTexture->getFormat() ));
	mVideoTexture->NxPixelBuffer = mVideoTexture->mTexture->getBuffer(0,0).get();

 

	if( mVideoTexture->NxPixelBuffer->getFormat() != mVideoTexture->FormatType ) {
		LogMsg("----- ERROR texture depth INVALID2 !");
		// doesnt matter so much since when uploading to gpu, pixelbox will be converted...
		LogMsg("pixel buffer format : " + Ogre::StringConverter::toString( mVideoTexture->mTexture->getFormat() ) );
		LogMsg("texture format : " + Ogre::StringConverter::toString( mVideoTexture->FormatType ) );
	} 
	*/


	


	LogMsg("Creating Video Texture : " ); 
	LogMsg("Width : " + Ogre::StringConverter::toString( mVideoTexture->GetWidth() ) );  
	LogMsg("Height : " + Ogre::StringConverter::toString(mVideoTexture->GetHeight() ) );  
	LogMsg("Bpp : " + Ogre::StringConverter::toString( mVideoTexture->GetBpp() ) );  
 

	mTextures.insert( NxTextureMap::value_type( TextureName, mVideoTexture ) );



	return mVideoTexture;
}


NxTextureCamera * NxTextureManager::CreateTextureCamera( const std::string & TextureName, const std::string & DeviceName, unsigned int DevType, int Width, int Height, int Bpp )
{
	NxTextureCamera * mVideoTextureCamera = new NxTextureCamera(); 

	unsigned long DevIndex = -1;
	DevIndex = NxVideoManager::getSingleton().GetCameraIndexFromName( DeviceName, (NxVideoCaptureDeviceType)DevType );
	if( DevIndex == -1 ){ LogMsg("No Camera Found For this Name : " + DeviceName ); return NULL; }
 
	if( mVideoTextureCamera->openVideoDevice( TextureName, DevIndex, Width, Height, Bpp, (NxVideoCaptureDeviceType) DevType )) {
		mTextures.insert( NxTextureMap::value_type( TextureName, mVideoTextureCamera ) );
		return mVideoTextureCamera; 
	}
	delete mVideoTextureCamera;
	return NULL;
}


NxTextureWeb * NxTextureManager::CreateWebTexture( const std::string & TextureName, const std::string & Link, int Width, int Height )
{
	NxTextureWeb * mVideoTextureWeb = new NxTextureWeb (); 
	return mVideoTextureWeb;
}

void NxTextureManager::Update( float Time )
{
	NxTextureMapIterator Iter = GetTextureIterator();
	while( Iter.hasMoreElements() ) {
		NxTexture * Tex = Iter.getNext();
		Tex->updateTexture( Time );
	}
}

}//namespace