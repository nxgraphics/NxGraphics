#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"

namespace Nx {

NxParticles::NxParticles( OperationType opType, bool UseVertexColour ) : mUseVertexColour(UseVertexColour)
{
	initialize( opType, false );
	mDirty = true;
	setRenderQueueGroup( RENDER_QUEUE_OVERLAY - 1 );
}

NxParticles::~NxParticles()
{
	LogMsg("~NxParticles()");
}

void NxParticles::Draw( const float * Data, const unsigned int * Colors, unsigned long ParticlesNum )
{
	prepareHardwareBuffers( ParticlesNum, 0, mUseVertexColour );

	HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
	vbuf->writeData(0, 3 * ParticlesNum * sizeof(float), Data, true );

	if( mUseVertexColour ) {
		HardwareVertexBufferSharedPtr cbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
		cbuf->writeData(0, ParticlesNum * sizeof(unsigned int), Colors, true );
	}
}

void NxParticles::SetMaterial( const std::string & MaterialName )
{
	setMaterial(MaterialName);
}

void NxParticles::createVertexDeclaration()
{
	VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	decl->addElement( POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION );
	if(mUseVertexColour)decl->addElement( TEXCOORD_BINDING, 0, VET_COLOUR, VES_DIFFUSE );
}

void NxParticles::fillHardwareBuffers()
{

}

}//namespace