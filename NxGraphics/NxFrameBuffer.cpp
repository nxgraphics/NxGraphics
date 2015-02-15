#include "NxGraphicsPreCompiled.h"

#include "../NxSound/NxSound_Headers.h"
using namespace NxSound_Namespace;

namespace Nx {
 
NxFrameBuffer::NxFrameBuffer( NxTexture * Layer ) : mLayer( Layer ), mAudioBuffer(0), mSound(0)
{ 
	LogMsg("Creating FrameBuffer ..." ); 
	mType = mLayer->getType();
 
	mWidth = mLayer->GetWidth(); 
	mHeight = mLayer->GetHeight();
	mBpp = mLayer->GetBpp();

	NxVertexList.push_back( Nx::Vector2(0,0) ); // quad upper left
	NxVertexList.push_back( Nx::Vector2(0,-mHeight) ); // quad lower left
 	NxVertexList.push_back( Nx::Vector2(mWidth,0) ); // quad upper right
	NxVertexList.push_back( Nx::Vector2(mWidth,-mHeight) ); // quad lower right
 	NxVertexList.push_back( Nx::Vector2(mWidth/2,-mHeight/2) ); // quad middle

	LogMsg( "FrameBuffer PixelBox Width : " + Ogre::StringConverter::toString( mWidth ));
	LogMsg( "FrameBuffer PixelBox Height : " + Ogre::StringConverter::toString( mHeight ));
	//LogMsg( "FrameBuffer PixelBox Format : " + Ogre::StringConverter::toString( Layer->mTexture[0]->getFormat() ));

	LayerBuffer = new unsigned char[ mWidth * mHeight * mBpp ];
	LayerPixelBox = new PixelBox( mWidth, mHeight, 1, (Ogre::PixelFormat)Layer->getFormat(), LayerBuffer );

	//Audio
	if( Layer->hasAudio() ) {
		LogMsg( "Preparing audio stream..");
		mAudioSampleindex = 0;
		double destSampleRate = 44100.0;
		double ratio = destSampleRate/Layer->getAudioSampleRate();
		unsigned long size = Layer->getAudioBufferSize() / ratio;
		const unsigned long BufferSize =  (size+1) * Layer->getAudioChannels() * (Layer->getAudioBitsPerSamples()  /8);
		LogMsg( "Creating file buffer size : "  + NxSoundUtils::ToString(  BufferSize ));
		LogMsg( "Creating file buffer with num channels : " + NxSoundUtils::ToString(  Layer->getAudioChannels() ));
		LogMsg( "Creating file buffer sample rate : "  + NxSoundUtils::ToString(  Layer->getAudioSampleRate() ));
		mAudioBuffer = new char[ BufferSize ];// 1/21 second 
		mSound = new NxSoundMediaFileBuffer( Layer->getAudioChannels(), Layer->getAudioSampleRate() , destSampleRate );
		NxSoundManager::getSingleton().GetDeviceOutput(0)->AddMediaFile( mSound );

		 
	}else{
		LogMsg( "No sound to prepare.");

	}
 
 
	LogMsg("Creating FrameBuffer : Done" );
}

NxFrameBuffer::~NxFrameBuffer()
{
	LogMsg("~NxFrameBuffer() ... ");


	if( mSound ) {
		NxSoundManager::getSingleton().GetDeviceOutput(0)->RemoveMediaFile( mSound );
		delete mSound;
		mSound = 0;
	}

	if( LayerBuffer ) delete LayerBuffer;
	LayerBuffer = 0;

	if( mAudioBuffer ) delete mAudioBuffer;
	mAudioBuffer = 0;

	NxVertexList.clear();
	LogMsg("~NxFrameBuffer() DONE ");

		
 
}

void NxFrameBuffer::SaveToFile( const std::string & FileName )
{
	Image finalImage;
	finalImage = finalImage.loadDynamicImage(static_cast<unsigned char*>(LayerPixelBox->data), LayerPixelBox->getWidth(),LayerPixelBox->getHeight(),Ogre::PF_R8G8B8);
	finalImage.save(FileName);
}

void NxFrameBuffer::SetVertexPos( NxQuadVertex Type, float PosX, float PosY )
{
	NxVertexList[Type].x = PosX;
	NxVertexList[Type].y = PosY;
}

bool NxFrameBuffer::Release()
{
	NxVertexList.clear();
	if( LayerBuffer ) delete LayerBuffer;
	LayerBuffer = NULL;
	return true;
}

void NxFrameBuffer::Process3DPlugins()
{
	//for( int i = 0 ; i < MixerManager::getSingleton().GetNumPlugins( mLayer->LayerNumber ); i++ )
	//{
	//	NxPlugin* NxPluginInst = MixerManager::getSingleton().GetVideoPlugin( mLayer->LayerNumber,  i );
	//	if( NxPluginInst )
	//	{
	//		if( NxGraphics_Plugin_Effect_Types::TYPE_PLUGIN_3D == NxPluginInst->GetPluginType() )
	//		{
	//			//NxPluginInst->ProcessFrame( ( BYTE * ) LayerPixelBox.data );
	//		}
	//	}
	//} 
}

void NxFrameBuffer::ProcessVideoPlugins()
{
	/*
	int NumPlugs = MixerManager::getSingleton().GetNumPlugins( mLayer.LayerNumber );
	NxGraphics_Layer_Types LayerType = ( NxGraphics_Layer_Types ) mLayer.Type ;
	if( LayerType == TYPE_3D && 0 == NumPlugs )
	{
		return ;
	}

	if( LayerType == TYPE_3D )
	{
		LayerPixelBuffer->blitToMemory( LayerPixelBox ); // copy the buffer to pixelbox
	}
	
	for( int i = 0 ; i < NumPlugs; i++ )
	{
		NxPlugin * NxPluginInst = MixerManager::getSingleton().GetVideoPlugin( mLayer.LayerNumber, i );
		if( NxPluginInst )
		{
			if( NxGraphics_Plugin_Effect_Types::TYPE_VIDEO == NxPluginInst->GetPluginType() )  
			{
				//NxPluginInst->ProcessFrame( ( BYTE * ) LayerPixelBox.data );
			}
		}
	}
	*/

	 

	mLayer->GetPixelBuffer()->blitFromMemory( *LayerPixelBox );
}

void NxFrameBuffer::ProcessVerticalPixelBufferFlip()
{
	size_t rowSpan = LayerPixelBox->getWidth() * PixelUtil::getNumElemBytes( LayerPixelBox->format );
	size_t height = LayerPixelBox->getHeight();
	Ogre::uchar *tmpData = new Ogre::uchar[rowSpan * height];
	Ogre::uchar *srcRow = (Ogre::uchar *) LayerPixelBox->data, *tmpRow = tmpData + (height - 1) * rowSpan;

	while (tmpRow >= tmpData)
	{
		memcpy(tmpRow, srcRow, rowSpan);
		srcRow += rowSpan;
		tmpRow -= rowSpan;
	}

	memcpy(LayerPixelBox->data, tmpData, rowSpan * height);
	delete [] tmpData;
}

bool NxFrameBuffer::UpdateVertexBuffer()
{
/*
	 Vertex_ID = static_cast<float*>(layer_vertex_buffer->lock(HardwareBuffer::HBL_DISCARD));

   //for(int i = 0; i < 4; i++){
  // ---  assign values to the vertex buffer data --- //
  // position & U,V assignments
  { // VERT #0
    // position assignments
    *(Vertex_ID) = QUAD_UPPER_LEFT.x;    Vertex_ID++;
    *(Vertex_ID) = QUAD_UPPER_LEFT.y;    Vertex_ID++;
    // uv assignments
    *(Vertex_ID) = 0.;    Vertex_ID++;
    *(Vertex_ID) = 0.;    Vertex_ID++;
  }

  { // VERT #1
    // position assignments
    *(Vertex_ID) = QUAD_LOWER_LEFT.x;    Vertex_ID++;
    *(Vertex_ID) = QUAD_LOWER_LEFT.y;    Vertex_ID++;
    // uv assignments
    *(Vertex_ID) = 0.;    Vertex_ID++;
    *(Vertex_ID) = 1.;    Vertex_ID++;
  }

  { // VERT #2
    // position assignments
    *(Vertex_ID) = QUAD_LOWER_RIGHT.x;    Vertex_ID++;
    *(Vertex_ID) = QUAD_LOWER_RIGHT.y;    Vertex_ID++;
    // uv assignments
    *(Vertex_ID) = 1.;    Vertex_ID++;
    *(Vertex_ID) = 1.;    Vertex_ID++;
  }

  { // VERT #3
    // position assignments
    *(Vertex_ID) = QUAD_UPPER_RIGHT.x;    Vertex_ID++;
    *(Vertex_ID) = QUAD_UPPER_RIGHT.y;    Vertex_ID++;
    // uv assignments
    *(Vertex_ID) = 1.;    Vertex_ID++;
    *(Vertex_ID) = 0.;    Vertex_ID++;
  }

   { // VERT #4 MIDDLE
    // position assignments
    *(Vertex_ID) = QUAD_LOWER_MIDDLE.x;    Vertex_ID++;
    *(Vertex_ID) = QUAD_LOWER_MIDDLE.y;    Vertex_ID++;
    // uv assignments
    *(Vertex_ID) = 0.5;    Vertex_ID++;
    *(Vertex_ID) = 0.5;    Vertex_ID++;
  }

   layer_vertex_buffer->unlock(); 
   // end vertex buffer
   */

	return true ;
}

} 
