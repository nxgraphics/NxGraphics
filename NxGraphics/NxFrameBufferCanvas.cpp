#include "NxGraphicsPreCompiled.h"
#include "../nxvideo/NxVideo_Headers.h"

namespace Nx {
 
NxFrameBufferCanvas::NxFrameBufferCanvas( NxTexture * Layer ) : NxFrameBuffer( Layer )
{


}

NxFrameBufferCanvas::~NxFrameBufferCanvas()
{

}

bool NxFrameBufferCanvas::UpdateTexture( float time )
{
	 /*
	NxBuffer->GetPixelData( &LayerBuffer[0] );

	//LayerPixelBox.data = NxBuffer->GetPixelData();
	if( LayerPixelBox->data != 0 )  
	{
		ProcessVerticalPixelBufferFlip();
		ProcessVideoPlugins();
	}	*/

	return true;
}

}//namespace
 