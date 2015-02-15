#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "NxRenderables.h"

namespace Nx {

NxLine::NxLine( OperationType opType, bool UseVertexColour ) : mUseVertexColour(UseVertexColour)
{
	initialize(opType,false);
	setMaterial("BaseWhiteNoLighting");
	mDirty = true;
	setRenderQueueGroup( RENDER_QUEUE_OVERLAY - 1);

	// animation spline
	mAnimSpline = new SimpleSpline();
	mAnimSpline->setAutoCalculate(true);
}

NxLine::~NxLine()
{
	delete mAnimSpline;
}

void NxLine::setOperationType(OperationType opType)
{
	mRenderOp.operationType = opType;
}

RenderOperation::OperationType NxLine::getOperationType() const
{
	return mRenderOp.operationType;
}

void NxLine::SetMaterial( const std::string & MaterialName )
{
	setMaterial(MaterialName);
}

static unsigned int packData( const NxColourValue & Col )
{
	unsigned char val8;
	unsigned int val32 = 0;
	// Alpha
	val8 = static_cast<unsigned char>( Col.a * 255 );
	val32 = val8 << 24;
	// Blue
	val8 = static_cast<unsigned char>( Col.b * 255  );
	val32 += val8 << 16;
	// Green
	val8 = static_cast<unsigned char>( Col.g * 255 );
	val32 += val8 << 8;
	// Red
	val8 = static_cast<unsigned char>( Col.r * 255 );
	val32 += val8;

	return val32;

}

void NxLine::addPoint(const Nx::Vector3 &p, const NxColourValue & Col )
{
	mPoints.push_back(p);
	if(mUseVertexColour){ mPointsColor.push_back( packData( Col )); } 
	mDirty = true;

	mAnimSpline->addPoint(  NxVec3ToOgre( p ) );
}
void NxLine::addPoint(float x, float y, float z, const NxColourValue & Col )
{
	mPoints.push_back(Nx::Vector3(x,y,z));
	if(mUseVertexColour){ mPointsColor.push_back( packData( Col )); } 
	mDirty = true;

	mAnimSpline->addPoint(Vector3(x,y,z));

}
const Nx::Vector3& NxLine::getPoint(unsigned short index) const
{
	assert(index < mPoints.size() && "Point index is out of bounds!!");
	return mPoints[index];
}

unsigned short NxLine::getNumPoints(void) const
{
	return (unsigned short)mPoints.size();
}

void NxLine::setPoint(unsigned short index, const Nx::Vector3 &value)
{
	assert(index < mPoints.size() && "Point index is out of bounds!!");
	mPoints[index] = value;
	mDirty = true;

	mAnimSpline->updatePoint( index,  NxVec3ToOgre( value ) );
}

void NxLine::setPointColour( unsigned short index, const NxColourValue &Value )
{
	assert(index < mPointsColor.size() && "Point index is out of bounds!!");
	if(mUseVertexColour){
		mPointsColor[index] = packData( Value );
		mDirty = true;
	} 
}

void NxLine::clear()
{
	mPoints.clear();
	mPointsColor.clear();
	mDirty = true;

	mAnimSpline->clear();
}

void NxLine::update()
{
	if (mDirty) fillHardwareBuffers();
}

Ogre::Vector3 NxLine::Interpolate(Real t) const
{
	return mAnimSpline->interpolate( t );
}

Ogre::Vector3 NxLine::Interpolate(unsigned int fromIndex, Real t) const
{
	return mAnimSpline->interpolate( fromIndex,t );
}

void NxLine::createVertexDeclaration()
{
	VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
	if(mUseVertexColour)decl->addElement( TEXCOORD_BINDING, 0, VET_COLOUR, VES_DIFFUSE );
}

void NxLine::fillHardwareBuffers()
{
	int size = mPoints.size();
	prepareHardwareBuffers(size,0, mUseVertexColour);
 
	if (!size) 
	{ 
		mBox.setExtents(Vector3::ZERO,Vector3::ZERO);
		mDirty=false;
		return;
	}

	Nx::Vector3 vaabMin = mPoints[0];
	Nx::Vector3 vaabMax = mPoints[0];

	HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
	{
		for(int i = 0; i < size; i++)
		{
			*prPos++ = mPoints[i].x;
			*prPos++ = mPoints[i].y;
			*prPos++ = mPoints[i].z;

			if(mPoints[i].x < vaabMin.x)
				vaabMin.x = mPoints[i].x;
			if(mPoints[i].y < vaabMin.y)
				vaabMin.y = mPoints[i].y;
			if(mPoints[i].z < vaabMin.z)
				vaabMin.z = mPoints[i].z;

			if(mPoints[i].x > vaabMax.x)
				vaabMax.x = mPoints[i].x;
			if(mPoints[i].y > vaabMax.y)
				vaabMax.y = mPoints[i].y;
			if(mPoints[i].z > vaabMax.z)
				vaabMax.z = mPoints[i].z;
		}
	}

	vbuf->unlock();


	if( mUseVertexColour ) {
		HardwareVertexBufferSharedPtr cbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
		cbuf->writeData(0, size * sizeof(unsigned int), &mPointsColor[0], true );
	}


	mBox.setExtents( NxVec3ToOgre( vaabMin ), NxVec3ToOgre(  vaabMax ) );
	mDirty = false;

}

}//namespace