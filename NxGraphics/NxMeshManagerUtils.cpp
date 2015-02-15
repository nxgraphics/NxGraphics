#include "NxGraphicsPreCompiled.h"
#include "NxMeshManager.h"
#include "NxConversions.h"

#include "../nxdeps/include/OgreProcedurals/include/Procedural.h"
using namespace Ogre;



namespace Nx {

bool mFlipVertexWinding = false;
Ogre::Matrix4 mTransform ;
AxisAlignedBox mBoundingBox;
bool mNormaliseNormals = false;
bool mUpdateBoundingBox = true;

void processIndexData(IndexData* indexData)
{
	if (!mFlipVertexWinding)
	{
		// Nothing to do.
		return;
	}

	if (indexData->indexCount % 3 != 0)
	{
        printf("Index number is not a multiple of 3, no vertex winding flipping possible. Skipped.");
        return;
	}

	//print("Flipping index order for vertex winding flipping.", V_HIGH);
	Ogre::HardwareIndexBufferSharedPtr buffer = indexData->indexBuffer;
	unsigned char* data = static_cast<unsigned char*>(buffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

	if(buffer->getType() == Ogre::HardwareIndexBuffer::IT_16BIT)
	{
		// 16 bit
		//print("using 16bit indices", V_HIGH);

		for (size_t i = 0; i < indexData->indexCount; i+=3)
		{
			Ogre::uint16 * i0 = (Ogre::uint16*)(data+0 * buffer->getIndexSize());
			Ogre::uint16* i2 = (Ogre::uint16*)(data+2 * buffer->getIndexSize());

			// flip
			Ogre::uint16 tmp = *i0;
			*i0 = *i2;
			*i2 = tmp;

			data += 3 * buffer->getIndexSize();
		}
	}
	else
	{
		// 32 bit
		//print("using 32bit indices", V_HIGH);

		for (size_t i = 0; i < indexData->indexCount; i+=3)
		{
			Ogre::uint32* i0 = (Ogre::uint32*)(data+0 * buffer->getIndexSize());
			Ogre::uint32* i2 = (Ogre::uint32*)(data+2 * buffer->getIndexSize());

			// flip
			Ogre::uint32 tmp = *i0;
			*i0 = *i2;
			*i2 = tmp;

			data += 3 * buffer->getIndexSize();
		}
	}

	buffer->unlock();
}

void processPositionElement( VertexData* vertexData, const VertexElement* vertexElem )
{
	int nMaxVert= vertexData->vertexCount ;
	//const Ogre::VertexElement* VertexEle_POS = vertexData->vertexDeclaration->findElementBySemantic( Ogre::VES_POSITION );
 
	// get vertex buffer info via the input element
	Ogre::HardwareVertexBufferSharedPtr VertexBufPOS = vertexData->vertexBufferBinding->getBuffer( vertexElem->getSource() );
 
	//LOCK BUFFER
	unsigned char* VertexPtrPOS = static_cast<unsigned char*>( VertexBufPOS->lock( Ogre::HardwareBuffer::HBL_NORMAL)   );
	int VertSizePOS=VertexBufPOS->getVertexSize();

	float * pElementPOS=NULL;
   
	//A vector of every vertices position
	std::vector<Ogre::Vector3> positions(nMaxVert);
	//Copy each position into position vector
	for(int nVert=0 ; nVert<nMaxVert ; nVert++)
	{
		vertexElem->baseVertexPointerToElement( VertexPtrPOS, &pElementPOS );
		Ogre::Vector3 vertex(pElementPOS);
		vertex = mTransform * vertex;
		pElementPOS[0] = vertex.x;
		pElementPOS[1] = vertex.y;
		pElementPOS[2] = vertex.z;
		mBoundingBox.merge(vertex);
		VertexPtrPOS+=VertSizePOS ;
	}
	//UNLOCK BUFFER
	if(VertexBufPOS->isLocked()){VertexBufPOS->unlock();}
}

void processDirectionElement(VertexData* vertexData, const VertexElement* vertexElem )
{

	Ogre::Quaternion rotation = mTransform.extractQuaternion();
	rotation.normalise();

	int nMaxVert= vertexData->vertexCount ;
	//const Ogre::VertexElement* VertexEle_POS = vertexData->vertexDeclaration->findElementBySemantic( Ogre::VES_POSITION );

	// get vertex buffer info via the input element
	Ogre::HardwareVertexBufferSharedPtr VertexBufPOS = vertexData->vertexBufferBinding->getBuffer( vertexElem->getSource() );

	//LOCK BUFFER
	unsigned char* VertexPtrPOS = static_cast<unsigned char*>( VertexBufPOS->lock( Ogre::HardwareBuffer::HBL_NORMAL ) );
	int VertSizePOS=VertexBufPOS->getVertexSize();

	float * pElementPOS=NULL;

	//A vector of every vertices position
	std::vector<Ogre::Vector3> positions(nMaxVert);
	//Copy each position into position vector
	for(int nVert=0 ; nVert<nMaxVert ; nVert++)
	{
		vertexElem->baseVertexPointerToElement( VertexPtrPOS, &pElementPOS );
		Ogre::Vector3 vertex(pElementPOS);
		vertex = rotation * vertex;
		if (mNormaliseNormals)
		{
			vertex.normalise();
		}

		pElementPOS[0] = vertex.x;
		pElementPOS[1] = vertex.y;
		pElementPOS[2] = vertex.z;
		//mBoundingBox.merge(vertex);
		VertexPtrPOS+=VertSizePOS ;
	}
	//UNLOCK BUFFER
	if(VertexBufPOS->isLocked()){VertexBufPOS->unlock();}

 
}

void processVertexData(VertexData* vertexData)
{
    const VertexElement* position = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
    if (position != NULL) {
        processPositionElement(vertexData, position);
    }else {
		LogMsg("processVertexData : CANT Process POSITION");
	}

    const VertexElement* normal = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
    if (normal != NULL) {
        processDirectionElement(vertexData, normal);
    }

    const VertexElement* binormal = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_BINORMAL);
    if (binormal != NULL) {
        processDirectionElement(vertexData, binormal);
    }

    const VertexElement* tangent = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TANGENT);
    if (tangent != NULL) {
        processDirectionElement(vertexData, tangent);
    }
}

AxisAlignedBox getVertexDataAabb( VertexData* vd, const Ogre::Matrix4& transform)
{
	AxisAlignedBox aabb;

	const VertexElement* ve = vd->vertexDeclaration->findElementBySemantic(VES_POSITION);
	HardwareVertexBufferSharedPtr vb = vd->vertexBufferBinding->getBuffer(ve->getSource());

	unsigned char* data = static_cast<unsigned char*>(
	vb->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

	for (size_t i = 0; i < vd->vertexCount; ++i)
	{
		float* v;
		ve->baseVertexPointerToElement(data, &v);
		aabb.merge(transform * Ogre::Vector3(v[0], v[1], v[2]));

		data += vb->getVertexSize();
	}
	vb->unlock();

	return aabb;
}

AxisAlignedBox getMeshAabb(Mesh* mesh, const Ogre::Matrix4& transform)
{
	AxisAlignedBox aabb;
	if (mesh->sharedVertexData != 0)
	{
		aabb.merge(getVertexDataAabb(mesh->sharedVertexData, transform));
	}
	for (unsigned int i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		SubMesh* sm = mesh->getSubMesh(i);
		if (sm->vertexData != 0)
		{
			aabb.merge(getVertexDataAabb(sm->vertexData, transform));
		}
	}

	return aabb;
}

AxisAlignedBox getMeshAabb(MeshPtr mesh, const Nx::Matrix4& transform)
{
	Ogre::Matrix4 mat1;
	NxMat4toOgre( mat1, transform );
	return getMeshAabb(mesh.get(), mat1);
}

void NxMeshManager::SetPivotTransform( Ogre::MeshPtr mesh, const Nx::Vector3 & Position, const Nx::Quaternion & Rotation, const Nx::Vector3 & Scale )
{
	//from mesh magick / mit licence
	 Nx::Matrix4 transform = Nx::Matrix4::IDENTITY;
	 Nx::Vector3 translate =  Nx::Vector3::ZERO;
	// Apply current transform to the mesh, to get the bounding box to
	// base te translation on.
	AxisAlignedBox aabb = getMeshAabb( mesh, transform);
	//if (alignment == "left")
	//{
	// 	translate = Vector3(-aabb.getMinimum().x, 0, 0);
	//}
	//else if (alignment == "center")
	//{
	//	translate = Vector3(-aabb.getCenter().x, 0, 0);
	//}
	//else if (alignment == "right")
	//{
	//	translate = Vector3(-aabb.getMaximum().x, 0, 0);
	//}

	//Position .. only support pivot down / centered
	//translate = Vector3(0, -aabb.getMinimum().y, 0);// pivot down

	translate = Position;
	 
	transform = Nx::Matrix4::getTrans(translate) * transform;

	//rotation
	transform = Nx::Matrix4(Rotation) * transform;

	//scale
	transform = Nx::Matrix4::getScale(Scale) * transform;

    // Check whether we have to flip vertex winding.
    // We do have to, if we changed our right hand base.
    // We can test it by using the cross product from X and Y and see, if it is a non-negative
    // projection on Z. Actually it should be exactly Z, as we don't do non-uniform scaling yet,
    // but the test is cheap either way.
    Nx::Matrix3 m3;
    transform.extract3x3Matrix(m3);

    if (m3.GetColumn(0).crossProduct(m3.GetColumn(1)).dotProduct(m3.GetColumn(2)) < 0)
    {
		LogMsg("SetPivotPosition : Flipping vertex winding ... "   );
    	mFlipVertexWinding = true;
    }

	//mTransform = transform;

	NxMat4toOgre( mTransform, transform ) ;


	mBoundingBox.setNull();

    if( mesh->sharedVertexData != NULL)
    {
        processVertexData( mesh->sharedVertexData);
	}else
	{
		LogMsg("mesh->sharedVertexData NULL");
	}

    for( int i = 0; i < mesh->getNumSubMeshes(); i++ )
    {
        SubMesh* submesh = mesh->getSubMesh(i);
        if( submesh->vertexData != NULL )
        {
			LogMsg("SetPivotPosition : Processing vertex data ... "   );
            processVertexData(submesh->vertexData);
		}else
		{
			LogMsg("submesh->vertexData NULL");
		}

        if (submesh->indexData != NULL)
        {
			LogMsg("SetPivotPosition : Processing Index data .."   );
        	processIndexData(submesh->indexData);
		}else
		{
			LogMsg("submesh->indexData NULL");
		}
    }

	//process pose
    for( unsigned short i = 0; i < mesh->getPoseCount(); ++i )
    {
		Ogre::Pose * pose =  mesh->getPose(i);
		Ogre::Matrix3 m3x3;
		mTransform.extract3x3Matrix(m3x3);

		Pose::VertexOffsetIterator it = pose->getVertexOffsetIterator();
		while (it.hasMoreElements()) {
			Ogre::Vector3 offset = it.peekNextValue();
			Ogre::Vector3 newOffset = m3x3 * offset;
			*it.peekNextValuePtr() = newOffset;
			it.moveNext();
		}
    }

	mesh->_setBounds( mBoundingBox, false );
  
}

}