#include "NxGraphicsPreCompiled.h"
#include "NxFrameBuffer.h"

namespace Nx {

NxFrameBuffer3D::NxFrameBuffer3D( NxTexture * Layer ) : NxFrameBuffer( Layer )
{

}

NxFrameBuffer3D::~NxFrameBuffer3D()
{

}

bool NxFrameBuffer3D::UpdateTexture( float time )
{


	//mLayer->mNx3DScene->GetNxViewport()->getTarget()->swapBuffers(false);
	//mLayer->mNxVideoFile->GetPixelData();


	return true;
}

}//namespace
 