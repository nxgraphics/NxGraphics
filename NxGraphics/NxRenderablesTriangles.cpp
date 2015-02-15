#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"

namespace Nx {

NxTriangles::NxTriangles( OperationType opType, bool UseVertexColour , bool UVW ) : mUseVertexColour(UseVertexColour), mZorder(100), mUVW(UVW)
{
	initialize( opType, false );
	mDirty = true;
	//setRenderQueueGroup( RENDER_QUEUE_MAIN );//RENDER_QUEUE_OVERLAY - 1 );

}

NxTriangles::~NxTriangles()
{
	LogMsg("~NxTriangles()");
}

void NxTriangles::Draw( const float * TriangleData, const float * TriangleNormals, const unsigned int * Colors, unsigned long NumTriangles )
{
	this->prepareHardwareBuffers( NumTriangles, 0, mUseVertexColour );
 
	HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
	vbuf->writeData( 0, 3 * NumTriangles * sizeof(float), TriangleData, true );
 
	HardwareVertexBufferSharedPtr nbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
 
	nbuf->writeData(0,  NumTriangles * sizeof(float), TriangleNormals, true );
 

	if( mUseVertexColour ) {
 

		HardwareVertexBufferSharedPtr cbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(2);
		cbuf->writeData(0, NumTriangles * sizeof(unsigned int), Colors, true );
	}
 
}

const String& NxTriangles::getMovableType(void) const
{
	static Ogre::String movType = "NxGeometry";
	return movType;
}

void NxTriangles::_updateRenderQueue( RenderQueue * queue )
{
	if( this->isVisible() ) {
		queue->addRenderable( this, mRenderQueueID, mZorder ); 
	}
}

Real NxTriangles::getSquaredViewDepth(const Camera* cam) const
{ 

	return (10000.0f - (Ogre::Real)GetZOrder());
	// return (20.0f - (Ogre::Real)GetZOrder());
	//(void)cam;
	//return 0; 

 /*
		Ogre::Vector3 min, max, mid, dist;

		min = mBox.getMinimum();
		max = mBox.getMaximum();
		mid = ((max + min) * 0.5);
		dist = getParentNode()->_getDerivedPosition() - cam->getDerivedPosition() - mid;

		return dist.squaredLength();
 */



}


void NxTriangles::SetZOrder( unsigned short Priority )
{
	mZorder = Priority;
}

unsigned short  NxTriangles::GetZOrder() const
{
	return mZorder;
}



void NxTriangles::SetTextureCoordinates( const float * Coordinates )
{
 
	HardwareVertexBufferSharedPtr cbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(2);

	 
	cbuf->writeData(0, cbuf->getNumVertices() * sizeof(float) * (mUVW ? 3 : 2), Coordinates, true );

 

}

void NxTriangles::SetMaterial( const std::string & MaterialName )
{
	setMaterial(MaterialName);
}

void NxTriangles::createVertexDeclaration()
{
	Ogre::VertexDeclaration* vdecl =  mRenderOp.vertexData-> vertexDeclaration;
	vdecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	vdecl->addElement(1, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	if(mUseVertexColour) {
		vdecl->addElement( 2, 0, VET_COLOUR, VES_DIFFUSE );
	}
	else {
	 
		vdecl->addElement( 2, 0, mUVW ? VET_FLOAT3 : VET_FLOAT2, VES_TEXTURE_COORDINATES );
	}
	 
}

void NxTriangles::fillHardwareBuffers()
{

}

void NxTriangles::prepareHardwareBuffers( size_t vertexCount, size_t indexCount,  bool UseVertexColour )
{
	// Prepare vertex buffer
	size_t newVertCapacity = mVertexBufferCapacity;

	 

	if ((vertexCount > mVertexBufferCapacity) || (!mVertexBufferCapacity)) { // vertexCount exceeds current capacity! It is necessary to reallocate the buffer.

		// Check if this is the first call
		if (!newVertCapacity)
			newVertCapacity = 1;

		// Make capacity the next power of two
		while (newVertCapacity < vertexCount)
			newVertCapacity <<= 1;
	}
	else if (vertexCount < mVertexBufferCapacity>>1) {
		// Make capacity the previous power of two
		while (vertexCount < newVertCapacity>>1)
			newVertCapacity >>= 1;
	}


	if( newVertCapacity != mVertexBufferCapacity)  {
		mVertexBufferCapacity = newVertCapacity;

		// Create new vertex buffer
		HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
		mRenderOp.vertexData->vertexDeclaration->getVertexSize(0), mVertexBufferCapacity,
		HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_? HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE !!!???
		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
 
		// Prepare buffer for normals
		HardwareVertexBufferSharedPtr nbuf =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		3*sizeof(float), mVertexBufferCapacity,
		HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );
		mRenderOp.vertexData->vertexBufferBinding->setBinding(1, nbuf);


		if(UseVertexColour) {

		 
			//colour buffer
			HardwareVertexBufferSharedPtr cbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR), mVertexBufferCapacity,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
			// Bind buffer
			mRenderOp.vertexData->vertexBufferBinding->setBinding(2, cbuf);
		}else
		{
	 

			

			HardwareVertexBufferSharedPtr texcoordsVertexBuffer = 
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( ( mUVW ? 3 : 2)  *sizeof(float), mVertexBufferCapacity, Ogre::HardwareBuffer::HBU_WRITE_ONLY);

			mRenderOp.vertexData->vertexBufferBinding->setBinding(2, texcoordsVertexBuffer);

			if( mRenderOp.vertexData->vertexBufferBinding->isBufferBound( 2 ))
			{
				 
			}
			else
			{
				 
			
			}


			 
		}

	}
	// Update vertex count in the render operation
	mRenderOp.vertexData->vertexCount = vertexCount;

	if( mRenderOp.useIndexes )
	{
		OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(), "indexCount exceeds 16 bit");
		size_t newIndexCapacity = mIndexBufferCapacity;
		// Prepare index buffer
		if ((indexCount > newIndexCapacity) || (!newIndexCapacity))
		{
			// indexCount exceeds current capacity! It is necessary to reallocate the buffer.

			// Check if this is the first call
			if (!newIndexCapacity)
				newIndexCapacity = 1;

			// Make capacity the next power of two
			while (newIndexCapacity < indexCount)
				newIndexCapacity <<= 1;

		}
		else if (indexCount < newIndexCapacity>>1) 
		{
			// Make capacity the previous power of two
			while (indexCount < newIndexCapacity>>1)
				newIndexCapacity >>= 1;
		}

		if (newIndexCapacity != mIndexBufferCapacity)
		{
			mIndexBufferCapacity = newIndexCapacity;
			// Create new index buffer
			mRenderOp.indexData->indexBuffer =
			HardwareBufferManager::getSingleton().
			createIndexBuffer( HardwareIndexBuffer::IT_16BIT, mIndexBufferCapacity,
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
		}

		// Update index count in the render operation
		mRenderOp.indexData->indexCount = indexCount;
	}

	 
}

}//namespace