#include "NxGraphicsPreCompiled.h"
#include "../nxvideo/NxVideo_Headers.h"

 
namespace Nx {
 
NxGraphicsRecorder::NxGraphicsRecorder()
{
	//mRecorder = new NxVideo_Avi_Recorder();
}

NxGraphicsRecorder::~NxGraphicsRecorder()
{
	//delete mRecorder;
	//mRecorder = 0;
}

void NxGraphicsRecorder::StartRecording( const std::string & Filename, int Width, int Height, int Bpp )
{
/*
	int Fps = 25;

	date today = day_clock::local_day();
	int day =  today.day() ;
	int month = today.month() ;
	int year = today.year() ;

	ptime now = second_clock::local_time(); //use the clock
	int seconds = now.time_of_day().seconds() ;
	int minutes = now.time_of_day().minutes();
	int hours = now.time_of_day().hours();

	ostringstream Timestamp;
	Timestamp.setf( ios::fixed, ios::floatfield );//set fixed double value
	Timestamp << "Records/" << year << "-" << month << "-" << day << "-" << hours << "-" << minutes << "-" <<  seconds << ".avi" ;
	string buffer_timestamp( Timestamp.str() );


	PixelFormat PixFormat ;
	if(	Bpp == 3 )
	{ 
		PixFormat = PF_BYTE_BGR ;
		Log("NxGraphics : Recording Pixel Depth is 24 bits");
	}
	else if(Bpp == 4 ) 
	{ 
		PixFormat = PF_BYTE_BGRA ;
		Log("NxGraphics : Recording Pixel Depth is 32 bits");
	}
	else
	{ 
		Log("NxGraphics : Pixel Depth not supported for recording");
	}

	TexturePtr RecordingTexture = TextureManager::getSingleton().createManual( "RecordingTexture", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, Width ,Height ,0, PixFormat, TU_RENDERTARGET ); 
	
	Log("NxGraphics : Video Recorder Texture Created.");
	RecordingTextureRT = RecordingTexture->getBuffer(0,0);
	RecordingViewport =  RecordingTextureRT->getRenderTarget()->addViewport( NxEngine::getSingleton().GetNxCamera()  );
	Log("NxGraphics : Video Recorder Viewport Created.");

	RecordingViewport->setBackgroundColour( Ogre::ColourValue(0,0,0,0) ); // set the background transparent
	RecordingViewport->setClearEveryFrame(true);
	RecordingViewport->getTarget()->setAutoUpdated(false);
	RecordingVideoBuffer = new BYTE[ Width * Height * Bpp ];
	RecordingPixelBox = PixelBox( Width, Height, 1, PixFormat, RecordingVideoBuffer );
	VideoRecord = true ;
	mRecorder->CreateVideoFile( buffer_timestamp.c_str(), Width, Height, Bpp, Fps );
	RecordingFps = Fps ;
	Log("NxGraphics : Video Recorder Started.");
	Ogre::Root::getSingleton().addFrameListener( this );
	*/
	
}

void NxGraphicsRecorder::StopRecording()
{
	/*
	mRecorder->CloseVideoFile();

	RecordingViewport->setCamera(NULL); 
	RecordingTextureRT->getRenderTarget()->setActive(false);
	RecordingTextureRT->getRenderTarget()->removeAllViewports();
	//RecordingTextureRT.setNull();

	TexturePtr Tex =  TextureManager::getSingleton().getByName("RecordingTexture");
	if( !Tex.isNull() )
	{
		Tex->unload();
		TextureManager::getSingleton().remove( Tex->getName() );
		Tex.setNull();
		Log("NxGraphics : Video Recorder Texture Released.");
	}

	delete RecordingVideoBuffer ;
	Log("NxGraphics : Video Recorder Stopped.");
	*/
}

void NxGraphicsRecorder::AddFrame( unsigned long elapsedMS )
{
	/*
	if(!VideoRecord){ return ; }

	RecordingViewport->getTarget()->update(true);
	RecordingTextureRT->blitToMemory(*RecordingPixelBox);

	//vertical flip
	{
		size_t rowSpan = RecordingPixelBox->getWidth() * PixelUtil::getNumElemBytes(RecordingPixelBox->format);
		size_t height = RecordingPixelBox->getHeight();
		Ogre::uchar *tmpData = new Ogre::uchar[rowSpan * height];
		Ogre::uchar *srcRow = (Ogre::uchar *)RecordingPixelBox->data, *tmpRow = tmpData + (height - 1) * rowSpan;

		while( tmpRow >= tmpData )
		{
			memcpy(tmpRow, srcRow, rowSpan);
			srcRow += rowSpan;
			tmpRow -= rowSpan;
		}
		memcpy(RecordingPixelBox->data, tmpData, rowSpan * height);

		delete [] tmpData;
	}

	mRecorder->AddVideoFrame( ( unsigned char * ) RecordingPixelBox->data );
	*/

}

bool NxGraphicsRecorder::frameRenderingQueued( const NxFrameEvent& evt )
{
	/*
	static float mTotalTime = 0;
	mTotalTime += evt.timeSinceLastFrame;
 
	if( mTotalTime > 0.1  )// time check should be managed in NxVideo
	{
		AddFrame( evt.timeSinceLastFrame  );
		mTotalTime = 0;
	} 
	*/
	return true;
}

}