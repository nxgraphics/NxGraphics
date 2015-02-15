#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

void CreateTriangleMeshFromEntity( Entity* pEntity, NxTriangleMeshShapeDesc* pOutDesc, Ogre::Vector3 vScale )
{
   String sMeshName = pEntity->getMesh()->getName();

   unsigned int   mVertexCount = 0;
   unsigned int   mIndexCount  = 0;
   size_t         vertex_count;
   Ogre::Vector3*   vertices;
   size_t         index_count;
   unsigned long*   indices;

   bool added_shared = false;
   bool hasWarning = false;
   vertex_count = 0;
   index_count = 0;
   size_t current_offset = 0;
   size_t shared_offset = 0;
   size_t next_offset = 0;
   size_t index_offset = 0;

   const Ogre::MeshPtr mesh = pEntity->getMesh();

   mesh->buildEdgeList();

   unsigned short j = mesh->getNumSubMeshes();
   for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
   {
      Ogre::SubMesh* submesh = mesh->getSubMesh( i );

      if(submesh->useSharedVertices) 
	  {
         if( !added_shared )
		 {
            mVertexCount += (uint)mesh->sharedVertexData->vertexCount;
            added_shared = true;
         }
      }
      else 
	  {
         mVertexCount += (uint)submesh->vertexData->vertexCount;
      }

      mIndexCount += (uint)submesh->indexData->indexCount;
   }

   vertices = new Ogre::Vector3[mVertexCount];
   indices = new unsigned long[mIndexCount];

   NxVec3* mMeshVertices = new NxVec3[mVertexCount];
   NxU32* mMeshFaces = new NxU32[mIndexCount];
   NxMaterialIndex* mMaterials = new NxMaterialIndex[mIndexCount];

   NxMaterialIndex currentMaterialIndex = 0;
   added_shared = false;

   for ( unsigned short i = 0; i < mesh->getNumSubMeshes();i++) 
   {
      Ogre::SubMesh* submesh = mesh->getSubMesh(i);
      Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
      if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
      {
         if(submesh->useSharedVertices)
         {
            added_shared = true;
            shared_offset = current_offset;
         }

         const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
         Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
         unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

         Ogre::Real* pReal;

         for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		 {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            mMeshVertices[current_offset + j] = NxVec3(pReal[0] * vScale.x ,pReal[1] * vScale.y, pReal[2] * vScale.z);
         }

         vbuf->unlock();
         next_offset += vertex_data->vertexCount;
      }

      Ogre::IndexData* index_data = submesh->indexData;
      size_t numTris = index_data->indexCount / 3;
      Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

      bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

      if ( use32bitindexes ) 
	  {
         unsigned int*  pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
         size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

         for ( size_t k = 0; k < numTris*3; ++k) 
		 {
            mMeshFaces[index_offset] = pInt[k] + static_cast<unsigned int>(offset);
            mMaterials[index_offset++] = currentMaterialIndex;
         }
      }
      else 
	  {
         unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
         size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

         for ( size_t k = 0; k < numTris*3; ++k) 
		 {
            mMeshFaces[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
            mMaterials[index_offset++] = currentMaterialIndex;
         }
      }

      ibuf->unlock();
      current_offset = next_offset;
   }

	NxTriangleMeshDesc mTriangleMeshDescription;
	mTriangleMeshDescription.numVertices = mVertexCount;
	mTriangleMeshDescription.numTriangles = mIndexCount / 3;
	mTriangleMeshDescription.pointStrideBytes = sizeof(NxVec3);
	mTriangleMeshDescription.triangleStrideBytes = 3 * sizeof(NxU32);
	//if (smooth)
	mTriangleMeshDescription.points = mMeshVertices;                     
	//else
	//mTriangleMeshDescription.points = mMeshVertices_Smoothed;

	mTriangleMeshDescription.flags = NX_MF_HARDWARE_MESH;
	mTriangleMeshDescription.triangles = mMeshFaces;
	mTriangleMeshDescription.materialIndexStride = sizeof(NxMaterialIndex);
	mTriangleMeshDescription.materialIndices = mMaterials;

	MemoryWriteBuffer buf;

   if(1)
   {
      Ogre::String fn;
      if (1) 
	  {
         fn = mesh->getName();
         if (Ogre::StringUtil::endsWith(fn, ".mesh")) 
		 { fn = fn.substr(0, fn.length() - 5) + ".nxs"; }
         else{ fn  += ".nxs"; }
      }
      else 
	  { fn = "cooked.nxs"; }

      //Initialise the cooker
      NxInitCooking();

      if ( NxCookTriangleMesh( mTriangleMeshDescription, UserStream(std::string( "cooked/" + fn).c_str(), false) ) ) 
	  {
         pOutDesc->meshData = NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk()->createTriangleMesh(UserStream(std::string( "cooked/" + fn).c_str(), true));
      }
      else 
	  {
         hasWarning = true;
		#ifdef _DEBUG     
         //error::getSingleton().Throw("Failed meshShape cooking for Mesh '" + mesh->getName() + "'", error::WARNING, mesh->getName(), "meshShape");
		#endif   
      }
   }
   else 
   {
      if( NxCookTriangleMesh( mTriangleMeshDescription, buf ) )
	  {
         pOutDesc->meshData = NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk()->createTriangleMesh(MemoryReadBuffer(buf.data));
      }
      else 
	  {
         hasWarning = true;
      }
   }

   //Release the cooker
   NxCloseCooking();
   delete []vertices;
   delete []indices;
   delete []mMeshVertices;
   delete []mMeshFaces;
   delete []mMaterials;
}

void CreateConvexMeshFromEntity( Entity * pEntity, NxConvexShapeDesc * pOutDesc, Ogre::Vector3 vScale )
{  
	unsigned int mVertexCount = 0;
	unsigned int mIndexCount  = 0;
	size_t vertex_count;
	Ogre::Vector3 * vertices;
	size_t index_count;
	unsigned long * indices;

	bool added_shared = false;
	bool hasWarning = false;
	vertex_count = 0;
	index_count = 0;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	assert( pEntity );

	const Ogre::String EntityName = pEntity->getName();
	const Ogre::MeshPtr mesh = pEntity->getMesh();
	const String sMeshName = mesh->getName();
 
	unsigned short j = mesh->getNumSubMeshes();
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i){
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );
		if(submesh->useSharedVertices){
			if( !added_shared ){
				mVertexCount += (uint)mesh->sharedVertexData->vertexCount;
				added_shared = true;
				}
			}
		else{
			mVertexCount += (uint)submesh->vertexData->vertexCount;
		}
		mIndexCount += (uint)submesh->indexData->indexCount;
	}

	vertices = new Ogre::Vector3[mVertexCount];
	indices = new unsigned long[mIndexCount];

	NxVec3* mMeshVertices = new NxVec3[mVertexCount];
	NxU32* mMeshFaces = new NxU32[mIndexCount];
	NxMaterialIndex* mMaterials = new NxMaterialIndex[mIndexCount];

	NxMaterialIndex currentMaterialIndex = 0;
	added_shared = false;

   for ( unsigned short i = 0; i < mesh->getNumSubMeshes();i++) 
   {
      Ogre::SubMesh* submesh = mesh->getSubMesh(i);
      Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

      if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
      {
         if(submesh->useSharedVertices)
         {
            added_shared = true;
            shared_offset = current_offset;
         }

         const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
         Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
         unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

         Ogre::Real* pReal;

         for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		 {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            mMeshVertices[current_offset + j] = NxVec3(pReal[0] * vScale.x ,pReal[1] * vScale.y, pReal[2] * vScale.z);
         }

         vbuf->unlock();
         next_offset += vertex_data->vertexCount;
      }

      Ogre::IndexData* index_data = submesh->indexData;
      size_t numTris = index_data->indexCount / 3;
      Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

      bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

      if ( use32bitindexes ) 
	  {
         unsigned int*  pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
         size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

         for ( size_t k = 0; k < numTris*3; ++k) 
		 {
            mMeshFaces[index_offset] = pInt[k] + static_cast<unsigned int>(offset);
            mMaterials[index_offset++] = currentMaterialIndex;
         }
      }
      else 
	  {
         unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
         size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

         for ( size_t k = 0; k < numTris*3; ++k) 
		 {
            mMeshFaces[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
            mMaterials[index_offset++] = currentMaterialIndex;
         }
      }

      ibuf->unlock();
      current_offset = next_offset;
   }

	NxConvexMeshDesc convexMeshDesc;
	convexMeshDesc.numVertices = mVertexCount ;
	convexMeshDesc.numTriangles = mIndexCount / 3;
	convexMeshDesc.pointStrideBytes = sizeof(NxVec3);
	convexMeshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
	convexMeshDesc.points = mMeshVertices;
	convexMeshDesc.flags = NX_CF_COMPUTE_CONVEX;
	convexMeshDesc.triangles = mMeshFaces;

	MemoryWriteBuffer buf;

   if(1)
   {
      Ogre::String fn;
      if (1) 
	  {
         fn = mesh->getName();
         if (Ogre::StringUtil::endsWith(fn, ".mesh")) 
		 { 
			 fn = fn.substr(0, fn.length() - 5) + ".nxs";
		 }
         else
		 {
			 fn  += ".nxs";
		 }
      }
      else 
	  { 
		  fn = "cooked.nxs";
	  }

      //Initialise the cooker
	  LogMsg("NxGraphics: Initializing the Cooker.");
	  if(!NxInitCooking())
	  {
		  LogMsg("NxGraphics: Failed to Initialize Cooking for Convex Mesh");
		  return ;
	  }


	  LogMsg("NxGraphics: Initialize Cooking for Convex Mesh Done");

	  if( NxCookConvexMesh( convexMeshDesc, UserStream( std::string( "cooked/" + fn).c_str(), false ) ) ) 
	  {
         pOutDesc->meshData = NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk()->createConvexMesh( UserStream( std::string( "cooked/" + fn).c_str(), true ) );
		 LogMsg("NxGraphics: NxCookConvexMesh Done");
      }
      else 
	  {
		  LogMsg("NxCookConvexMesh failed, object has more than 255 faces !");
      }
   }
   else //memory
   {
      if( NxCookConvexMesh( convexMeshDesc, buf ) )
	  {
         pOutDesc->meshData = NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk()->createConvexMesh( MemoryReadBuffer( buf.data ) );
      }
      else 
	  {
         LogMsg("NxCookConvexMesh failed 2");
      }
   }

   if(  pOutDesc->meshData  ) 
   {
	   LogMsg("Created Convex Mesh For " + pEntity->getName() );
   }

   //Release the cooker
   NxCloseCooking();
   delete []vertices;
   delete []indices;
   delete []mMeshVertices;
   delete []mMeshFaces;
   delete []mMaterials;
}

NxPhysicsMeshShape::NxPhysicsMeshShape( NxPhysicsActor * Actor, const NxEntityDesc & Desc ) : NxPhysicsShape( Actor, Desc )
{
	Ogre::Vector3 Scale = Desc.mNxEntity->GetParentSceneNode()->getScale();
	NxMat33 Rot;
	Rot.fromQuat( NxQuatToPhysx( Desc.mlocalOrientation ) );
	NxVec3 Pos = NxVec3ToPhysx( Desc.mlocalPosition );

	if( Desc.mUseConvex )
	{
		NxConvexShapeDesc ShapeDesc;
		ShapeDesc.setToDefault();
		ShapeDesc.localPose.t = Pos;
		ShapeDesc.localPose.M = Rot;	
		CreateConvexMeshFromEntity( Desc.mNxEntity->GetEntity(), &ShapeDesc, Scale );
		mShape = Actor->GetNxActor()->createShape( ShapeDesc );
	}
	else
	{
		NxTriangleMeshShapeDesc ShapeDesc;
		ShapeDesc.setToDefault();
		ShapeDesc.localPose.t = Pos;
		ShapeDesc.localPose.M = Rot;	
		CreateTriangleMeshFromEntity( Desc.mNxEntity->GetEntity(), &ShapeDesc, Scale );
		mShape = Actor->GetNxActor()->createShape( ShapeDesc );
	}

	Actor->SetBodyProperties( Actor->GetBodyProperty() );
}

NxPhysicsMeshShape::~NxPhysicsMeshShape()
{

}


}//namespace