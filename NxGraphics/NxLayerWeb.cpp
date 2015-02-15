#include "NxGraphicsPreCompiled.h"

namespace Nx {


NxTextureWeb::NxTextureWeb() : NxTexture()
{

}

NxTextureWeb::~NxTextureWeb()
{

}

NxVideo_Player * NxTextureWeb::GetWeb()
{
	return 0;//mNxVideoFile; //mLayer.
}

//------------------------------------

NxLayerWeb::NxLayerWeb( int Width, int Height ) : NxLayer( Width, Height )//Mixing_layers & MixLayer ) : //, NxTextureWeb() 
{

}

NxLayerWeb::~NxLayerWeb()
{

}

}


 