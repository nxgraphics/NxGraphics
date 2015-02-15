#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

#define TEAR_MEMORY_FACTOR 3

namespace Nx {

Nx3DObjectPhysicClothe::Nx3DObjectPhysicClothe( NxNode * parentNode, const NxClotheDesc & ClotheDesc ) : NxEntity()
{
	mType = NxTypeCloth ;

	Nx3D_Scene * CurrentScene = &parentNode->GetNxScene();
	mNode = CurrentScene->GetNxSceneManager()->createSceneNode();
	LogMsg("Clothe : Attaching manual object");

	mEntity =  ClotheDesc.mEntity;
	mNode->attachObject( mEntity );

	mNxNodeParent = parentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );

	Vector3 vScale (1,1,1);



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


   LogMsg("const Ogre::MeshPtr mesh = mEntity->getMesh();");

   const Ogre::MeshPtr mesh = mEntity->getMesh();

    LogMsg(" buildEdgeList ");
   mesh->buildEdgeList();


	unsigned short src, dest;
	if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)){
		mesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}




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

     LogMsg(" NxClothMeshDesc ... ");


	NxClothMeshDesc desc;
	desc.numVertices = mVertexCount;
	desc.numTriangles = mIndexCount / 3;
	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.triangleStrideBytes		= 3*sizeof(NxU32);
	desc.vertexMassStrideBytes		= sizeof(NxReal);
	desc.vertexFlagStrideBytes		= sizeof(NxU32);
	desc.points						= (NxVec3*)malloc(sizeof(NxVec3)*desc.numVertices);
	desc.triangles					= mMeshFaces;//(NxU32*)malloc(sizeof(NxU32)*desc.numTriangles*3);
	desc.vertexMasses				= 0;
	desc.vertexFlags				= 0;
	desc.flags						= NX_CLOTH_MESH_WELD_VERTICES;
	desc.weldingDistance			= 0.0001f;

	mMaxVertices = TEAR_MEMORY_FACTOR * desc.numVertices;
	mMaxIndices  = 3 * desc.numTriangles;

	mTriCount = desc.numTriangles;

	 LogMsg(" copy positions and indices ... ");
	// copy positions and indices
	NxVec3 *vSrc = (NxVec3*)mMeshVertices;//desc.numVertices;
	NxVec3 *vDest = (NxVec3*)desc.points;
	for (int i = 0; i < desc.numVertices; i++, vDest++, vSrc++)
	{
		*vDest = (*vSrc);//*vScale ;//+ myOffset;
	}

	
	 LogMsg("memcpy ... ");
	memcpy((NxU32*)desc.triangles, mMeshFaces, sizeof(NxU32)*desc.numTriangles*3);

 
	 mNumVertices = mVertexCount;
	 mNumIndices = mIndexCount;
	//memcpy((NxU32*)desc.triangles, wo.mIndices, sizeof(NxU32)*desc.numTriangles*3);
 
/*
	bool textured = true;
	if (textured)
	{
		mNumTempTexCoords = desc.numVertices;
		mTempTexCoords = (float *)malloc(sizeof(float) * mNumTempTexCoords * 2);
		memcpy((NxU32*)mTempTexCoords, wo.mTexCoords, sizeof(float)*mNumTempTexCoords*2);

		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
		for (NxU32 i = 0; i < desc.numTriangles; i++)
		{
			assert((desc.flags & NX_CF_16_BIT_INDICES) == 0);
			NxU32* tri = (NxU32*)(((char*)desc.triangles) + (desc.triangleStrideBytes * i));
			mIndexRenderBuffer[3*i+0] = tri[0];
			mIndexRenderBuffer[3*i+1] = tri[1];
			mIndexRenderBuffer[3*i+2] = tri[2];
		}
	}
	else
	{
		mNumTempTexCoords = 0;
		mTempTexCoords = NULL;
	}
	*/

			mNumTempTexCoords = 0;
		mTempTexCoords = NULL;


	if(!NxInitCooking())
	{
	
	   LogMsg("NxInitCooking failed");
	}
  MemoryWriteBuffer wb;

  	   LogMsg("NxCookClothMesh  ");
  NxCookClothMesh(desc, wb);
  MemoryReadBuffer rb(wb.data);
  NxClothMesh * mClothMesh = NxScene3DPhysicsManager::getSingleton().GetPhysicsSdk()->createClothMesh(rb);

  free((NxVec3*)desc.points); free((NxU32*)desc.triangles);
  free((NxReal*)desc.vertexMasses); free((NxU32*)desc.vertexFlags);



	 mVertexRenderBuffer = NULL;
 

	 mIndexRenderBuffer = NULL;


	allocateReceiveBuffers(desc.numVertices, desc.numTriangles); 


NxClothDesc clothDesc;
	clothDesc.clothMesh = mClothMesh;
	clothDesc.meshData = *mReceiveBuffers;

	if(!desc.isValid())
	{
		LogMsg("Physics Cloth Object Description Not Valid" ); 
		return ;
	}

	
	
	clothDesc.globalPose.t = NxVec3(parentNode->GetNxSceneNode()->getPosition().x, parentNode->GetNxSceneNode()->getPosition().y  , parentNode->GetNxSceneNode()->getPosition().z );
	clothDesc.globalPose.M.rotX(-NxHalfPiF32);

	clothDesc.thickness = 0.01;
	clothDesc.density = 1.80f; // was 1.0f / heavier / lighter
	//
	// Set the bendint stiffness (0-1) NX_CLF_BENDING must be set.
	clothDesc.bendingStiffness = 0.5f;//1.0;
	clothDesc.dampingCoefficient = 0.5;
	// Set the stretching stiffness (0-1)The difficulty to stretch the cloth.1 : Not allowed to stretch
	clothDesc.stretchingStiffness = 1;
	// Set the friction of the cloth// Defines how much energy is lost due to interior motion of the cloth.
	clothDesc.friction = 0.5;//   0.5; 
	clothDesc.pressure = 1.0;
	clothDesc.tearFactor = 1.5;
	clothDesc.attachmentTearFactor = 1.5;
	clothDesc.wakeUpCounter = 0.4 ;
	clothDesc.sleepLinearVelocity = -1 ;
    clothDesc.collisionResponseCoefficient = 0.2;
	clothDesc.attachmentResponseCoefficient = 0.2;
	//Number of solver iterations. Note: Small numbers make the simulation faster while the cloth gets less stiff.Default:</b> 5
	clothDesc.solverIterations = 15;//10;//5;
	//clothDesc.flags |= NX_CLF_STATIC;
	//clothDesc.flags |= NX_CLF_DISABLE_COLLISION;
	//clothDesc.flags |= NX_CLF_VISUALIZATION;

	//Enable/disable gravity. If off, the cloth is not subject to the gravitational forceof the rigid body scene.
	clothDesc.flags |= NX_CLF_GRAVITY;


	// Cloth bending is allowed.
	//clothDesc.flags |= NX_CLF_BENDING;
	//clothDesc.flags |= NX_CLF_BENDING_ORTHO;
	//clothDesc.flags |= NX_CLF_DAMPING;
	//clothDesc.flags |= NX_CLF_COMDAMPING;

	//Set 2 way collision (cloth <-> rigid)
	//Enable/disable two way collision of cloth with the rigid body scene.
	//In either case, cloth is influenced by colliding rigid bodies.
	//If NX_CLF_COLLISION_TWOWAY is not set, rigid bodies are not influenced by 
	//colliding pieces of cloth. Use NxClothDesc.collisionResponseCoefficient to
	//control the strength of the feedback force on rigid bodies.
	//When using two way interaction care should be taken when setting the density of the attached objects.
	//For example if an object with a very low or high density is attached to a cloth then the simulation 
	//may behave poorly. This is because impulses are only transfered between the cloth and rigid body solver
	//outside the solvers.
	//Two way interaction works best when NX_SF_SEQUENTIAL_PRIMARY is set in the primary scene. If not set,
	//collision and attachment artifacts may happen.

	clothDesc.flags |= NX_CLF_COLLISION_TWOWAY;

	//clothDesc.flags |= NX_CLF_DISABLE_COLLISION;

	


	/**
	\brief Uses the information provided by NxCloth.setConstrainPositions() and
	NxCloth.setConstrainCoefficients() to make the cloth less stretchy.

	If the flag is set, the solver pulls vertices 
	with a maxDistance greater zero towards vertices with maxDistance zero (attached). 
	This reduces the stretchiness even for low solver iteration counts. The process is
	non-physical and can yield small ghost forces. The NxCloth.hardStretchLimitationFactor
	defines by what factor the cloth is allowed to stretch.

	@see NxCloth.setConstrainPositions() NxCloth.setConstrainCoefficients() NxCloth.hardStretchLimitationFactor
	*/
	clothDesc.flags |= NX_CLF_HARD_STRETCH_LIMITATION;

	//brief Enable/disable self-collision handling within a single piece of cloth.
 	//clothDesc.flags |= NX_CLF_SELFCOLLISION;

	//Allows cloth to collide with soft bodies and with other cloth.
	//clothDesc.flags |= NX_CLF_INTER_COLLISION; // creashes

	// Set the hardware flag
	clothDesc.flags |= NX_CLF_HARDWARE;
	if (!NxScene3DPhysicsManager::getSingleton().HasPhysxHardwareAcceleration())
	{
		clothDesc.flags &= ~NX_FF_HARDWARE;
		LogMsg("Cloth Created in Software Mode.");
	}
	else
	{
		LogMsg("Cloth Created in Hardware Mode.");
	}

	mCloth = CurrentScene->GetPhysicScene()->GetPhysicsScene3D().createCloth(clothDesc);

	NxGroupsMask groupsMask;
	groupsMask.bits0 = 1; 
	groupsMask.bits1 = 1; 
	groupsMask.bits2 = 1; 
	groupsMask.bits3 = 1; 
	mCloth->setGroupsMask(groupsMask);

	mCloth->setGroup( GROUP_COLLIDABLE_PUSHABLE );





/*
	NxClothMeshDesc desc;

	NxVec3 myOffset(0.f);
	//if (offset != NULL){ myOffset = *offset; }


	desc.numVertices = mObject->getCurrentVertexCount();
	desc.numTriangles = mObject->getCurrentIndexCount()  / 3;
	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.triangleStrideBytes		= 3*sizeof(NxU32);
	desc.vertexMassStrideBytes		= sizeof(NxReal);
	desc.vertexFlagStrideBytes		= sizeof(NxU32);
	desc.points						= (NxVec3*)malloc(sizeof(NxVec3)*desc.numVertices);
	desc.triangles					= (NxU32*)malloc(sizeof(NxU32)*desc.numTriangles*3);
	desc.vertexMasses				= 0;
	desc.vertexFlags				= 0;
	desc.flags						= NX_CLOTH_MESH_WELD_VERTICES;
	desc.weldingDistance			= 0.0001f;

	mMaxVertices = TEAR_MEMORY_FACTOR * desc.numVertices;
	mMaxIndices  = 3 * desc.numTriangles;

	mTriCount = desc.numTriangles;

	// copy positions and indices
	NxVec3 *vSrc = (NxVec3*)desc.numVertices;
	NxVec3 *vDest = (NxVec3*)desc.points;
	for (int i = 0; i < desc.numVertices; i++, vDest++, vSrc++)
	{
		*vDest = (*vSrc)*scale + myOffset;
	}

	memcpy((NxU32*)desc.triangles, mTriangles, sizeof(NxU32)*desc.numTriangles*3);

	if (textured)
	{
		mNumTempTexCoords = desc.numVertices;
		mTempTexCoords = (float *)malloc(sizeof(float) * mNumTempTexCoords * 2);
		memcpy((NxU32*)mTempTexCoords, wo.mTexCoords, sizeof(float)*mNumTempTexCoords*2);

		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
		for (NxU32 i = 0; i < desc.numTriangles; i++)
		{
			assert((desc.flags & NX_CF_16_BIT_INDICES) == 0);
			NxU32* tri = (NxU32*)(((char*)desc.triangles) + (desc.triangleStrideBytes * i));
			mIndexRenderBuffer[3*i+0] = tri[0];
			mIndexRenderBuffer[3*i+1] = tri[1];
			mIndexRenderBuffer[3*i+2] = tri[2];
		}
	}
	else
	{
		mNumTempTexCoords = 0;
		mTempTexCoords = NULL;
	}


*/

	 
	
/*
	desc.numVertices = mObject->getCurrentVertexCount();
	desc.numTriangles = mObject->getCurrentIndexCount()  / 3;
	desc.pointStrideBytes = sizeof(NxVec3);
	desc.triangleStrideBytes = 3*sizeof(NxU32);
	desc.vertexMassStrideBytes = sizeof(NxReal);
	desc.vertexFlagStrideBytes = sizeof(NxU32);
	desc.points = (NxVec3*)malloc(sizeof(NxVec3)*desc.numVertices);
	desc.triangles = (NxU32*)malloc(sizeof(NxU32)*desc.numTriangles*3);
	desc.vertexMasses = 0;
	desc.vertexFlags = 0;
	desc.flags = NX_CLOTH_MESH_WELD_VERTICES;
	desc.weldingDistance = 0.0001f;

	NxVec3 *vSrc = (NxVec3*)mVertices;
	NxVec3 *vDest = (NxVec3*)desc.points;
	for (int i = 0; i < mVertexCount; i++, vDest++, vSrc++)
	*vDest = (*vSrc);
	memcpy((NxU32*)desc.triangles, mTriangles, sizeof(NxU32)*desc.numTriangles*3);

	NxInitCooking();
	MemoryWriteBuffer wb;
	NxCookClothMesh(desc, wb);
	MemoryReadBuffer rb(wb.data);
	NxClothMesh *mClothMesh = 	NxScene3DManager::getSingleton().GetPhysicsSdk()->createClothMesh(rb);

	free((NxVec3*)desc.points); free((NxU32*)desc.triangles);
	free((NxReal*)desc.vertexMasses); free((NxU32*)desc.vertexFlags);

	NxU32 maxVertices = mVertexCount * 3;
	mReceiveBuffers.verticesPosBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	mReceiveBuffers.verticesNormalBegin= (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	mReceiveBuffers.verticesPosByteStride = sizeof(NxVec3);
	mReceiveBuffers.verticesNormalByteStride = sizeof(NxVec3);
	mReceiveBuffers.maxVertices = maxVertices;
	mReceiveBuffers.numVerticesPtr = (NxU32*)malloc(sizeof(NxU32));

	NxU32 maxIndices = 3 * mTriangleCount;
	mReceiveBuffers.indicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxIndices);
	mReceiveBuffers.indicesByteStride = sizeof(NxU32);
	mReceiveBuffers.maxIndices = maxIndices;
	mReceiveBuffers.numIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

	int maxParentIndices = maxVertices;
	mReceiveBuffers.parentIndicesBegin= (NxU32*)malloc(sizeof(NxU32)*maxParentIndices);
	mReceiveBuffers.parentIndicesByteStride = sizeof(NxU32);
	mReceiveBuffers.maxParentIndices = maxParentIndices;
	mReceiveBuffers.numParentIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

	mVertexCount = mTriangleCount = maxParentIndices = 0;

	ClothDesc.clothMesh = mClothMesh;
	ClothDesc.meshData = mReceiveBuffers;

	mCloth = mScene->createCloth(ClothDesc); 


	//bool res = generateObjMeshDesc( meshDesc, objFileName, scale, offset, texFilename != NULL );

	//if( !res ){ Log("Could not create Wavefront Physics Cloth Object !"  ); }
	//Nx3D_Scene * scener = NxScene3DManager::getSingleton(). GetNx3DSceneFromOgreSceneManager( mNode->getCreator()  );
	//init(  &scener->GetPhysicsScene3D() , desc, meshDesc);

	*/

/*

	static int index = 0;
 	mManualObj = Node->getCreator()->createManualObject("ClothObj_" + Ogre::StringConverter::toString( index ) );
	index++;
 
	SceneNode * mClothNode = Node->getCreator()->getRootSceneNode()->createChildSceneNode();
	mClothNode->attachObject(mManualObj);



	float ClothSize = 5.0f; // 10

	Create( clothDesc, "../../NxMedia/Video/OGRE/media/Physics/mycloth.obj", ClothSize , &VecToPhysx( Node->getPosition())  ,"text" );
	//scener->Unlock();


	//mCloth->setConstrainPositions()

	*/


}


Nx3DObjectPhysicClothe::~Nx3DObjectPhysicClothe()
{


}

void Nx3DObjectPhysicClothe::AttachVertexToGlobalPosition( unsigned int Vertice, const Ogre::Vector3 & Position )
{
 

	mCloth->attachVertexToGlobalPosition(Vertice , VecToPhysx( Position ) );


}

struct Nx3DObjectPhysicClothe::RenderBufferVertexElement
{
	NxVec3 position;
	NxVec3 normal;
	float texCoord[2];
};


void Nx3DObjectPhysicClothe::allocateReceiveBuffers( int numVertices, int numTriangles )
{
	// here we setup the buffers through which the SDK returns the dynamic cloth data
	// we reserve more memory for vertices than the initial mesh takes
	// because tearing creates new vertices
	// the SDK only tears cloth as long as there is room in these buffers

	//NxU32 maxVertices = TEAR_MEMORY_FACTOR * numVertices;
	//NxU32 maxIndices = 3*numTriangles;

	if (mVertexRenderBuffer == NULL)
	{
		// Allocate Render Buffer for Vertices if it hasn't been done before
		mVertexRenderBuffer = ( RenderBufferVertexElement*)malloc(sizeof(RenderBufferVertexElement) * mMaxVertices);
		memset(mVertexRenderBuffer, 0, sizeof(RenderBufferVertexElement) * mMaxVertices);
	}

	if (mIndexRenderBuffer == NULL)
	{
		// Allocate Render Buffer for Indices if it hasn't been done before
		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
	}

	mReceiveBuffers = new NxMeshData();

	mReceiveBuffers->verticesPosBegin         = &(mVertexRenderBuffer[0].position.x);
	mReceiveBuffers->verticesNormalBegin      = &(mVertexRenderBuffer[0].normal.x);
	mReceiveBuffers->verticesPosByteStride    = sizeof(RenderBufferVertexElement);
	mReceiveBuffers->verticesNormalByteStride = sizeof(RenderBufferVertexElement);
	mReceiveBuffers->maxVertices              = mMaxVertices;
	mReceiveBuffers->numVerticesPtr           = &mNumVertices;

	// the number of triangles is constant, even if the cloth is torn
	NxU32 maxIndices = 3*numTriangles;
	mReceiveBuffers->indicesBegin             = mIndexRenderBuffer;
	mReceiveBuffers->indicesByteStride        = sizeof(NxU32);
	mReceiveBuffers->maxIndices               = maxIndices;
	mReceiveBuffers->numIndicesPtr            = &mNumIndices;

	if (mNumTempTexCoords > 0)
	{
		// Copy Tex Coords from temp buffers to graphics buffer
		assert(mNumTempTexCoords == numVertices);

		for (NxU32 i = 0; i < mNumTempTexCoords; i++)
		{
			mVertexRenderBuffer[i].texCoord[0] = mTempTexCoords[2*i+0];
			mVertexRenderBuffer[i].texCoord[1] = mTempTexCoords[2*i+1];
		}

		// Get rid of temp buffer
		mNumTempTexCoords = 0;
		free (mTempTexCoords);
		mTempTexCoords = NULL;
	}

	// the parent index information would be needed if we used textured cloth
	mReceiveBuffers->parentIndicesBegin       = (NxU32*)malloc(sizeof(NxU32)*mMaxVertices);
	mReceiveBuffers->parentIndicesByteStride  = sizeof(NxU32);
	mReceiveBuffers->maxParentIndices         = mMaxVertices;
	mReceiveBuffers->numParentIndicesPtr      = &mNumParentIndices;

	mReceiveBuffers->dirtyBufferFlagsPtr = &mMeshDirtyFlags;

	// init the buffers in case we want to draw the mesh 
	// before the SDK as filled in the correct values
	mMeshDirtyFlags = 0;
	mNumVertices = 0;
	mNumIndices = 0;
	mNumParentIndices = 0;
}



void Nx3DObjectPhysicClothe::Update( float time )
{

	//Log("Clalled");
 
 
	NxU32 mVec3Buffsize = mNumVertices * sizeof(NxVec3);
 

	// Create Vertices/Normal Binding.
	Ogre::VertexBufferBinding* bind = mEntity->getMesh()->getSubMesh(0)->vertexData->vertexBufferBinding;

	Ogre::HardwareVertexBufferSharedPtr HardBuffer = bind->getBuffer(0);
    HardBuffer->writeData(0, mVec3Buffsize, mReceiveBuffers->verticesPosBegin, true);

  
 




	/*
	// Copy Normals
	//memcpy(mNormals,mReceiveBuffers.verticesNormalBegin, mVec3Buffsize);

	// Send to Hardware.
	HardBuffer = bind->getBuffer (1);
    HardBuffer->writeData(0, mVec3Buffsize, mReceiveBuffers->verticesNormalBegin, true);

	*/

/*
 
 

	mObject->clear();//Before rendering,clear buffer.
	mObject->begin( MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST );

	for( NxU32 i=0; i<numVertices; i++ )//First,Copy vertex buffer
	{
		Ogre::Vector3 pos = Ogre::Vector3(mVertexRenderBuffer[i].position.x, mVertexRenderBuffer[i].position.y, mVertexRenderBuffer[i].position.z);
		mObject->position(pos);
		Ogre::Vector3 normal = Ogre::Vector3(mVertexRenderBuffer[i].normal.x, mVertexRenderBuffer[i].normal.y, mVertexRenderBuffer[i].normal.z);
		mObject->normal(normal);
		mObject->textureCoord(mVertexRenderBuffer[i].texCoord[0],mVertexRenderBuffer[i].texCoord[1]);
	}

	for(NxU32 j=0;j<mTriCount;j++)	//Second copy Index buffer
	{
		mObject->triangle(mIndexRenderBuffer[j*3],mIndexRenderBuffer[j*3+1],mIndexRenderBuffer[j*3+2]);
	}

	mObject->end();

	*/

	 
}


}