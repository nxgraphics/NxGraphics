#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxTextureStream::NxTextureStream() : NxTexture()
{

}

NxTextureStream::~NxTextureStream()
{

}

NxVideo_Player * NxTextureStream::GetStream()
{
	return 0;//mNxVideoFile;//mLayer.
}

//--------------------------------------------------------

NxLayerStream::NxLayerStream( int Width, int Height ) : NxLayer( Width, Height )
{

}

NxLayerStream::~NxLayerStream()
{

}

}
 