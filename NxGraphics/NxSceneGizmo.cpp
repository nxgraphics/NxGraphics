#include "NxGraphicsPreCompiled.h"
#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
#include "NxPhysicsInclude.h"
#endif
#include "../../nxdeps/include/OIS/OIS.h"

namespace Nx {

NxSceneTools::NxSceneTools( Nx3D_Scene * Scene, Ogre::Camera * Camera ) : mScene( Scene ), mEditionActive( false )
{
	mEntitySelection = NULL;
	CreateGizmo();
}

NxSceneTools::~NxSceneTools()
{
	 
}

const NXTOOLS_MODE NxSceneTools::GetGizmoMode() const 
{
	return mCurrentGizmoMode;
}

const NXTOOL_AXIS NxSceneTools::GetActiveAxis() const 
{
	return mCurrentAxis;
}

void NxSceneTools::SetActiveAxis( const std::string & AxisMeshName )
{
	if ( AxisMeshName == "scbwx" ) {
		mCurrentAxis = AXIS_X;//NXTOOL_AXIS::AXIS_X
	}
	else if ( AxisMeshName == "scbwy" ) {
		mCurrentAxis = AXIS_Y;
	}
	else if ( AxisMeshName == "scbwz" ) {
		mCurrentAxis = AXIS_Z;
	}
	else {
		//mCurrentAxis = NXTOOL_AXIS::AXIS_NONE;
	}
}

void createPlaneMesh( Ogre::SceneManager *manager, Ogre::String name )
{
	Ogre::ManualObject *mMesh = manager->createManualObject("OgitorAxisPlaneGizmoManualObject");
	mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);
	mMesh->position( 0, 1, 0);
	mMesh->position( 1, 1, 0);
	mMesh->position( 1, 0, 0);
	mMesh->position( 0, 0, 0);
	mMesh->index(0);
	mMesh->index(1);
	mMesh->index(2);
	mMesh->index(0);
	mMesh->index(2);
	mMesh->index(3);
	mMesh->end();
	mMesh->convertToMesh(name, "EditorResources");
	manager->destroyManualObject(mMesh);
}

Ogre::MeshPtr createMesh( Ogre::SceneManager *manager, Ogre::String name )
{
	Ogre::ManualObject *mMesh = manager->createManualObject("OgitorAxisGizmoManualObject");
	mMesh->begin("AxisGizmo_Material", RenderOperation::OT_LINE_LIST);
	mMesh->position(0, 0, 0);
	mMesh->position(3, 0, 0);

	mMesh->index(0);
	mMesh->index(1);
	mMesh->end();

	float const radius = 0.22f;
	float const accuracy = 8;
	float MPI = Math::PI;

	float division = (MPI / 2.0f) / 16.0f;
	float start = division * 3;
	float end = division * 14;
	int index_pos = 0;

	mMesh->begin("AxisGizmo_Material", RenderOperation::OT_LINE_STRIP);

	for(float theta = start; theta < end; theta += division)  {
		mMesh->position(0, 3.0f * cos(theta), 3.0f * sin(theta));
		mMesh->index(index_pos++);
	}

	mMesh->end();

	mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

	mMesh->position(2.85f,     0,     0);

	for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy)  {
		mMesh->position(2.95f, radius * cos(theta), radius * sin(theta));
	}
	mMesh->position(3.45f,     0,     0);

	for(int inside = 1;inside < 16;inside++) {
		mMesh->index(0);
		mMesh->index(inside);
		mMesh->index(inside + 1);
	}
	mMesh->index(0);
	mMesh->index(16);
	mMesh->index(1);

	for(int outside = 1;outside < 16;outside++) {
		mMesh->index(17);
		mMesh->index(outside);
		mMesh->index(outside + 1);
	}
	mMesh->index(17);
	mMesh->index(16);
	mMesh->index(1);

	mMesh->end();

	//ROTATE GIZMO

	mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Quaternion q1;
	q1.FromAngleAxis(Ogre::Degree(-90), Ogre::Vector3(0,0,1));
	Ogre::Quaternion q2;
	q2.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0,1,0));

	Ogre::Vector3 translate1(0, 3.0f * cos(end), 3.0f * sin(end));
	Ogre::Vector3 translate2(0, 3.0f * cos(start), 3.0f * sin(start) - 0.25f);

	Ogre::Vector3 pos(-0.3f,     0,     0);
	mMesh->position(q1 * pos + translate1);

	for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy)  {
		pos = Ogre::Vector3(-0.3f, radius * cos(theta), radius * sin(theta));
		mMesh->position(q1 * pos + translate1);
	}
	pos = Ogre::Vector3(0.3f, 0 , 0);
	mMesh->position(q1 * pos + translate1);

	pos = Ogre::Vector3(-0.3f,     0,     0);
	mMesh->position(q2 * pos + translate2);

	for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy)  {
		pos = Ogre::Vector3(-0.3f, radius * cos(theta), radius * sin(theta));
		mMesh->position(q2 * pos + translate2);
	}
	pos = Ogre::Vector3(0.3f, 0 , 0);
	mMesh->position(q2 * pos + translate2);

	for(int inside = 1;inside < 16;inside++) {
		mMesh->index(0);
		mMesh->index(inside);
		mMesh->index(inside + 1);
	}
	mMesh->index(0);
	mMesh->index(16);
	mMesh->index(1);

	for(int outside = 1;outside < 16;outside++) {
		mMesh->index(17);
		mMesh->index(outside);
		mMesh->index(outside + 1);
	}
	mMesh->index(17);
	mMesh->index(16);
	mMesh->index(1);

	for(int inside = 19;inside < 34;inside++) {
		mMesh->index(18);
		mMesh->index(inside);
		mMesh->index(inside + 1);
	}
	mMesh->index(18);
	mMesh->index(34);
	mMesh->index(19);

	for(int outside = 19;outside < 34;outside++) {
		mMesh->index(35);
		mMesh->index(outside);
		mMesh->index(outside + 1);
	}
	mMesh->index(35);
	mMesh->index(34);
	mMesh->index(19);

	mMesh->end();

	//SCALE GIZMO

	mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

	mMesh->position( 2.85f, 0, 0 );

	for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy)  {
		mMesh->position(2.85f, radius * cos(theta), radius * sin(theta));
	}

	mMesh->position(3.45f,     0,     0);

	mMesh->position(3.40f,  0.20f,  0.20f);
	mMesh->position(3.40f,  0.20f, -0.20f);
	mMesh->position(3.40f, -0.20f, -0.20f);
	mMesh->position(3.40f, -0.20f,  0.20f);
	mMesh->position(3.50f,  0.20f,  0.20f);
	mMesh->position(3.50f,  0.20f, -0.20f);
	mMesh->position(3.50f, -0.20f, -0.20f);
	mMesh->position(3.50f, -0.20f,  0.20f);

	for(int inside = 1;inside < 16; inside++ ) {
		mMesh->index(0);
		mMesh->index(inside);
		mMesh->index(inside + 1);
	}
	mMesh->index(0);
	mMesh->index(16);
	mMesh->index(1);

	for(int outside = 1;outside < 16;outside++)
	{
		mMesh->index(17);
		mMesh->index(outside);
		mMesh->index(outside + 1);
	}
	mMesh->index(17);
	mMesh->index(16);
	mMesh->index(1);

	mMesh->index(18);
	mMesh->index(19);
	mMesh->index(20);
	mMesh->index(18);
	mMesh->index(20);
	mMesh->index(21);

	mMesh->index(22);
	mMesh->index(23);
	mMesh->index(24);
	mMesh->index(22);
	mMesh->index(24);
	mMesh->index(25);

	mMesh->index(18);
	mMesh->index(22);
	mMesh->index(25);
	mMesh->index(18);
	mMesh->index(25);
	mMesh->index(21);

	mMesh->index(19);
	mMesh->index(23);
	mMesh->index(24);
	mMesh->index(19);
	mMesh->index(24);
	mMesh->index(20);

	mMesh->index(18);
	mMesh->index(22);
	mMesh->index(23);
	mMesh->index(18);
	mMesh->index(23);
	mMesh->index(19);

	mMesh->index(21);
	mMesh->index(20);
	mMesh->index(24);
	mMesh->index(21);
	mMesh->index(24);
	mMesh->index(25);

	mMesh->end();

	Ogre::MeshPtr GizmoMesh =  mMesh->convertToMesh( name  );

	manager->destroyManualObject(mMesh);

	return GizmoMesh;
}

void NxSceneTools::SetEditionActive( bool EditActive )
{
	mEditionActive = EditActive;
}

bool NxSceneTools::GetEditionActive()
{
	return mEditionActive;
}

void NxSceneTools::CreateGizmo()
{

	Ogre::ResourceManager::ResourceCreateOrRetrieveResult giz  = MaterialManager::getSingletonPtr()->createOrRetrieve("AxisGizmo_Material", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 //Ogre::MaterialPtr gizmoMaterial = MaterialManager::getSingletonPtr()->createOrRetrieve("AxisGizmo_Material", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first->  staticCast<Material>();
	//Ogre::MaterialPtr gizmoMaterial = MaterialManager::getSingletonPtr()->createOrRetrieve("AxisGizmo_Material", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first;
	
	Ogre::MaterialPtr gizmoMaterial = giz.first.staticCast<Material>() ;
	
	gizmoMaterial->setReceiveShadows(false);
	gizmoMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false); 
	gizmoMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	gizmoMaterial->getTechnique(0)->getPass(0)->setCullingMode(CULL_NONE);
	gizmoMaterial->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_DIFFUSE);

	createMesh( mScene->GetNxSceneManager() , "OgitorAxisGizmoMesh");
	createPlaneMesh( mScene->GetNxSceneManager(), "OgitorAxisPlaneMesh");

	mGizmoNode = mScene->GetNxSceneManager()->getRootSceneNode()->createChildSceneNode("scbWidgetNode",Ogre::Vector3(0,0,0),Ogre::Quaternion::IDENTITY);

	mGizmoX = mGizmoNode->createChildSceneNode("scbnwx",Ogre::Vector3(0,0,0),Ogre::Quaternion::IDENTITY);
	mGizmoY = mGizmoNode->createChildSceneNode("scbnwy",Ogre::Vector3(0,0,0),Ogre::Quaternion::IDENTITY);
	mGizmoZ = mGizmoNode->createChildSceneNode("scbnwz",Ogre::Vector3(0,0,0),Ogre::Quaternion::IDENTITY);

	Ogre::Quaternion q1;
	Ogre::Quaternion q2;

	q1.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0,0,1));
	q2.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(1,0,0));
	mGizmoY->setOrientation(q1 * q2);

	q1.FromAngleAxis(Ogre::Degree(-90), Ogre::Vector3(0,1,0));
	q2.FromAngleAxis(Ogre::Degree(-90), Ogre::Vector3(1,0,0));
	mGizmoZ->setOrientation(q1 * q2);

	//Entities
	mGizmoEntities[0] = mScene->GetNxSceneManager()->createEntity("scbwx", "OgitorAxisGizmoMesh", "EditorResources");
	mGizmoEntities[1] = mScene->GetNxSceneManager()->createEntity("scbwy", "OgitorAxisGizmoMesh", "EditorResources");
	mGizmoEntities[2] = mScene->GetNxSceneManager()->createEntity("scbwz", "OgitorAxisGizmoMesh", "EditorResources");
	mGizmoEntities[3] = mScene->GetNxSceneManager()->createEntity("scbwt", "OgitorAxisPlaneMesh", "EditorResources");
	mGizmoEntities[4] = mScene->GetNxSceneManager()->createEntity("scbwu", "OgitorAxisPlaneMesh", "EditorResources");
	mGizmoEntities[5] = mScene->GetNxSceneManager()->createEntity("scbwv", "OgitorAxisPlaneMesh", "EditorResources");

	//XX arrows
	mGizmoEntities[0]->setCastShadows(false);
	mGizmoEntities[0]->setMaterialName("MAT_GIZMO_X");
	mGizmoEntities[0]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[0]->setQueryFlags( NXGIZMO ); //NxEntityQueryFlags::
	mGizmoX->attachObject(mGizmoEntities[0]);

	//YY arrows
	mGizmoEntities[1]->setCastShadows(false);
	mGizmoEntities[1]->setMaterialName("MAT_GIZMO_Y");
	mGizmoEntities[1]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[1]->setQueryFlags( NXGIZMO );//NxEntityQueryFlags::
	mGizmoY->attachObject(mGizmoEntities[1]);

	//ZZ arrows
	mGizmoEntities[2]->setCastShadows(false);
	mGizmoEntities[2]->setMaterialName("MAT_GIZMO_Z");
	mGizmoEntities[2]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[2]->setQueryFlags( NXGIZMO );//NxEntityQueryFlags::
	mGizmoZ->attachObject(mGizmoEntities[2]);

	//XY Plane
	mGizmoEntities[3]->setCastShadows(false);
	mGizmoEntities[3]->setMaterialName("MAT_GIZMO_XY");
	mGizmoEntities[3]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[3]->setQueryFlags( NXGIZMO );// NxEntityQueryFlags::
	mGizmoX->attachObject(mGizmoEntities[3]);

	//YZ Plane
	mGizmoEntities[4]->setCastShadows(false);
	mGizmoEntities[4]->setMaterialName("MAT_GIZMO_YZ");
	mGizmoEntities[4]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[4]->setQueryFlags( NXGIZMO );//NxEntityQueryFlags::
	mGizmoY->attachObject(mGizmoEntities[4]);

	//ZX Plane
	mGizmoEntities[5]->setCastShadows(false);
	mGizmoEntities[5]->setMaterialName("MAT_GIZMO_ZX");
	mGizmoEntities[5]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[5]->setQueryFlags( NXGIZMO );//NxEntityQueryFlags::
	mGizmoZ->attachObject(mGizmoEntities[5]);

	mGizmoNode->setVisible(false);

	SetGizmoMode( NXTOOL_SELECT );//NXTOOLS_MODE::

}

void NxSceneTools::SetGizmoMode( NXTOOLS_MODE Mode )
{
	if(mOldGizmoMode == Mode) 
		return;

	mOldGizmoMode = Mode;
	Entity* wx = mGizmoEntities[0];
	Entity* wy = mGizmoEntities[1];
	Entity* wz = mGizmoEntities[2];

	mGizmoEntities[3]->getSubEntity(0)->setVisible(false);
	mGizmoEntities[4]->getSubEntity(0)->setVisible(false);
	mGizmoEntities[5]->getSubEntity(0)->setVisible(false);

	wx->getSubEntity(0)->setVisible(true);
	wx->getSubEntity(1)->setVisible(false);
	wx->getSubEntity(2)->setVisible(false);
	wx->getSubEntity(3)->setVisible(false);
	wx->getSubEntity(4)->setVisible(false);
	wy->getSubEntity(0)->setVisible(true);
	wy->getSubEntity(1)->setVisible(false);
	wy->getSubEntity(2)->setVisible(false);
	wy->getSubEntity(3)->setVisible(false);
	wy->getSubEntity(4)->setVisible(false);
	wz->getSubEntity(0)->setVisible(true);
	wz->getSubEntity(1)->setVisible(false);
	wz->getSubEntity(2)->setVisible(false);
	wz->getSubEntity(3)->setVisible(false);
	wz->getSubEntity(4)->setVisible(false);

	switch( Mode )
	{
		case NXTOOL_MOVE :
		{
			wx->getSubEntity(2)->setVisible(true);
			wy->getSubEntity(2)->setVisible(true);
			wz->getSubEntity(2)->setVisible(true);
			mGizmoEntities[3]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[4]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[5]->getSubEntity(0)->setVisible(true);
			break;
		}

		case NXTOOL_ROTATE :
		{
			wx->getSubEntity(0)->setVisible(false);
			wy->getSubEntity(0)->setVisible(false);
			wz->getSubEntity(0)->setVisible(false);
			wx->getSubEntity(1)->setVisible(true);
			wy->getSubEntity(1)->setVisible(true);
			wz->getSubEntity(1)->setVisible(true);
			wx->getSubEntity(3)->setVisible(true);
			wy->getSubEntity(3)->setVisible(true);
			wz->getSubEntity(3)->setVisible(true);
			break;
		}

		case NXTOOL_SCALE :
		{
			wx->getSubEntity(4)->setVisible(true);
			wy->getSubEntity(4)->setVisible(true);
			wz->getSubEntity(4)->setVisible(true);
			mGizmoEntities[3]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[4]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[5]->getSubEntity(0)->setVisible(true);
			break;
		}
		
		case NXTOOL_NONE :
		{
			mGizmoEntities[0]->setVisible(false);
			mGizmoEntities[1]->setVisible(false);
			mGizmoEntities[2]->setVisible(false);
			mGizmoEntities[3]->setVisible(false);
			mGizmoEntities[4]->setVisible(false);
			mGizmoEntities[5]->setVisible(false);	
			break;
		
		}
	}

	mCurrentGizmoMode = Mode;
}

void NxSceneTools::UpdateGizmo()
{
	Ogre::Camera * Cam = mScene->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->GetCameraFps();  

	if(mGizmoEntities[0])
	{
		Ogre::Vector3 cameraPos = Cam->getPosition();
		float dist1 = (cameraPos - mGizmoEntities[0]->getWorldBoundingBox().getCenter()).length();
		float dist2 = (cameraPos - mGizmoEntities[1]->getWorldBoundingBox().getCenter()).length();
		float dist3 = (cameraPos - mGizmoEntities[2]->getWorldBoundingBox().getCenter()).length();

		int orders[3];
		int increment = -1;

		if( mCurrentGizmoMode == NXTOOL_ROTATE ){
			increment = 1;
			orders[0] = orders[1] = orders[2] = 93;
		}
		else{
			increment = -1;
			orders[0] = orders[1] = orders[2] = 95;
		}

		if(dist1 > dist2) 
			orders[0] += increment;
		else
			orders[1] += increment;

		if(dist1 > dist3) 
			orders[0] += increment;
		else
			orders[2] += increment;

		if(dist2 > dist3) 
			orders[1] += increment;
		else
			orders[2] += increment;

		mGizmoEntities[0]->setRenderQueueGroup(orders[0]);
		mGizmoEntities[1]->setRenderQueueGroup(orders[1]);
		mGizmoEntities[2]->setRenderQueueGroup(orders[2]);
	}

	if( mEntitySelection ){

		//Scale the gizmo to fit the screen View.
		Ogre::Vector3 position = mEntitySelection->getParentSceneNode()->_getDerivedPosition();
		int iLeft,iTop,iWidth,iHeight;
		mScene->GetNxViewport()->getActualDimensions(iLeft,iTop,iWidth,iHeight);
		float minsize =  std::min( iWidth, iHeight );   //std::min(rect.z, rect.w);
		float distance = ( position - Cam->getDerivedPosition() ).length();
		distance /= (minsize / 50.0f);
		distance *= 1.0f; // mGizmoScale;

		mGizmoNode->setPosition( mEntitySelection->getParentNode()->_getDerivedPosition() );
		mGizmoNode->setOrientation( mEntitySelection->getParentNode()->_getDerivedOrientation() );
		mGizmoNode->setScale( distance, distance, distance);
		mGizmoNode->setVisible(true);
	}
}

void NxSceneTools::DeleteGizmo()
{

}

void GetMeshInformationEx( const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices,
										const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );
		if(submesh->useSharedVertices){ // We only need to add the shared vertices once
			if( !added_shared ) {
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		} else {
			vertex_count += submesh->vertexData->vertexCount;
		}
		index_count += submesh->indexData->indexCount;// Add the indices
	}
	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) {
			if(submesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			float* pReal;
			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes ){
			for ( size_t k = 0; k < numTris*3; ++k){
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else{
			for ( size_t k = 0; k < numTris*3; ++k){
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
} 

/*
if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("NxGeometry") == 0 ))
{
	Log( "FOUND NXGEOMETRY !!!!" );

	Ogre::MovableObject * Object = static_cast<Ogre::MovableObject*>(query_result[qr_idx].movable);    

	why show alway on click ??

		Log( "Name : " );

	Log( "NXGEOMETRY Name : " + Object->getName() );


}
*/
 

Ogre::Entity * CreateRaycastForType( Nx3D_Scene * Scene, const OIS::MouseEvent &e, NxEntityQueryFlags Mask )
{
 
	RaySceneQuery * raySceneQuery = Scene->GetNxSceneManager()->createRayQuery( Ray() );
	float normX = (float)e.state.X.abs  / (float)Scene->GetNxViewport()->getActualWidth() ;
	float normY = (float)e.state.Y.abs  / (float)Scene->GetNxViewport()->getActualHeight();
	Ogre::Camera * Cam =  Scene->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->GetCameraFps();

	Ray mouseRay = Cam->getCameraToViewportRay( normX, normY );

	raySceneQuery->setRay(mouseRay);
	raySceneQuery->setSortByDistance( true );
	raySceneQuery->setQueryMask( Mask );//NxEntityQueryFlags::NXENTITY | NxEntityQueryFlags::NXTERRAIN );

	// Execute query
	RaySceneQueryResult result = raySceneQuery->execute();

	if( result.size() == 0 ) { 
		Scene->GetNxSceneManager()->destroyQuery(raySceneQuery); 
		LogMsg("No Selection");
		return 0;
	} 



	// at this point we have raycast to a series of different objects bounding boxes.
	// we need to test these different objects to see which is the first polygon hit.
	// there are some minor optimizations (distance based) that mean we wont have to
	// check all of the objects most of the time, but the worst case scenario is that
	// we need to test every triangle of every object.
	Ogre::Entity *pentity = NULL;
	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result;
	Ogre::RaySceneQueryResult &query_result = raySceneQuery->getLastResults();

	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{

		if (query_result[qr_idx].movable != NULL)
		{
			LogMsg( " RAYCAST OBJECT TYPE :  "+ query_result[qr_idx].movable->getMovableType() );
			LogMsg( " RAYCAST OBJECT NAME :  "+ query_result[qr_idx].movable->getParentNode()->getName() );
		}

		


		// stop checking if we have found a raycast hit that is closer
		// than all remaining entities
		if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance)) {
			break;
		}

		// only check this result if its a hit against an entity
		if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0 ))
		{
			// get the entity to check
			pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);           

			if(!pentity->getVisible()) continue;
			// mesh data to retrieve         
			size_t vertex_count;
			size_t index_count;
			Ogre::Vector3 *vertices;
			unsigned long *indices;

			LogMsg("get MEsh info...");

			// get the mesh information
			GetMeshInformationEx(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
				pentity->getParentNode()->_getDerivedPosition(),
				pentity->getParentNode()->_getDerivedOrientation(),
				pentity->getParentNode()->_getDerivedScale());

			printf("Clicked ENTITY: ");
			printf("%s\n",pentity->getName( ).c_str());
			printf("SCENENODE : %s\n",pentity->getParentSceneNode()->getName( ).c_str());
			
			// test for hitting individual triangles on the mesh
			bool new_closest_found = false;
			for (int i = 0; i < static_cast<int>(index_count); i += 3 ) { // check for a hit against this triangle
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects( mouseRay, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false);
				if( hit.first ) { // if it was a hit check if its the closest
					if ((closest_distance < 0.0f) || (hit.second < closest_distance)) {
						closest_distance = hit.second;// this is the closest so far, save it off
						new_closest_found = true;
					}
				}
			}

			LogMsg("Deleting vertices... ");

			// free the verticies and indicies memory
			delete[] vertices;
			delete[] indices;

			// if we found a new closest raycast for this object, update the
			// closest_result before moving on to the next object.
			if( new_closest_found )
			{
				LogMsg("new_closest_found ... ");
				closest_result = mouseRay.getPoint(closest_distance);
			}
		}   
	}

	Scene->GetNxSceneManager()->destroyQuery(raySceneQuery);

	return pentity;
}

void NxSceneTools::OnKeyPressed( const OIS::KeyEvent &e )
{

}

void NxSceneTools::OnKeyReleased( const OIS::KeyEvent &e )
{

}



unsigned int    mVertexBufferSize = 0;
unsigned int    mIndexBufferSize = 0;
Ogre::Vector3 * mVertexBuffer = 0;
unsigned long * mIndexBuffer = 0;
Ogre::Vector3 mLastTranslationDelta;
Ogre::Quaternion mLastDerivedOrient  ;
Ogre::Vector3 mLastDerivedPosition  ;
Ogre::Vector3 mLastDerivedScale  ;
Ogre::Plane mLastUsedPlane  ;
Ogre::Vector3 mLast3DDelta;
bool mLeftDown = false;
Ogre::Vector2 mLastMouse ;

void GetMeshDataEx(const Ogre::Entity *entity, size_t &vertex_count, size_t &index_count,
								 const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	Ogre::MeshPtr mesh = entity->getMesh();

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices) {
			if( !added_shared ) {
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}

	if(vertex_count > mVertexBufferSize) {
		OGRE_FREE(mVertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBuffer = OGRE_ALLOC_T(Ogre::Vector3,vertex_count, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBufferSize = vertex_count;
	}

	if(index_count > mIndexBufferSize) {
		OGRE_FREE(mIndexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBuffer = OGRE_ALLOC_T(unsigned long,index_count, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBufferSize = index_count;
	}

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		if(!entity->getSubEntity(i)->isVisible())
			continue;

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) {
			if(submesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			float* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				mVertexBuffer[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}

		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes ){
			for ( size_t k = 0; k < numTris*3; ++k) {
				mIndexBuffer[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else {
			for ( size_t k = 0; k < numTris*3; ++k) {
				mIndexBuffer[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
	index_count = index_offset;
}


static void GetBuffers(Ogre::Vector3 **VertexBuffer, unsigned long **IndexBuffer)
{
	*VertexBuffer = mVertexBuffer;
	*IndexBuffer = mIndexBuffer;
}

bool NxSceneTools::PickGizmos( Ogre::Ray &ray, int &Axis )
{
	if(!mGizmoEntities[0] || !mGizmoEntities[0]->isVisible())
		return false;

	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result;

	for(int widx = 0;widx < 6;widx++)
	{
		// get the entity to check
		Ogre::Entity *pentity = mGizmoEntities[widx];

		std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, pentity->getWorldBoundingBox());
		if(!hit.first)
			continue;

		size_t vertex_count;
		size_t index_count;
		Ogre::Vector3 *vertices;
		unsigned long *indices;

		// get the mesh information
		 GetMeshDataEx(pentity, vertex_count, index_count, 
			pentity->getParentNode()->_getDerivedPosition(),
			pentity->getParentNode()->_getDerivedOrientation(),
			pentity->getParentNode()->_getDerivedScale());

		 GetBuffers(&vertices, &indices);
		// test for hitting individual triangles on the mesh
		bool new_closest_found = false;
		for (int i = 0; i < static_cast<int>(index_count); i += 3) {
			
			hit = Ogre::Math::intersects(ray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, true);// check for a hit against this triangle
			
			if (hit.first) { // if it was a hit check if its the closest
				if ((closest_distance < 0.0f) || (hit.second < closest_distance)) {
					closest_distance = hit.second;
					new_closest_found = true;
				}
			}
		}

		if (new_closest_found) {
			closest_result = ray.getPoint(closest_distance);
			switch(widx) {
				case 0:Axis = AXIS_X;break;
				case 1:Axis = AXIS_Y;break;
				case 2:Axis = AXIS_Z;break;
				case 3:Axis = AXIS_XY;break;
				case 4:Axis = AXIS_YZ;break;
				case 5:Axis = AXIS_XZ;break;
			}
		}
	}

	return (closest_distance >= 0.0f);
}

bool GetMouseRay( Nx3D_Scene * Scene, Ogre::Vector2 point, Ogre::Ray &mRay )
{
	float width = Scene->GetNxViewport()->getActualWidth();
	float height = Scene->GetNxViewport()->getActualHeight();
	Ogre::Camera * Cam =  Scene->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->GetCameraFps();
	mRay = Cam->getCameraToViewportRay( point.x / width, point.y / height);
	return true;
}

Ogre::Plane NxSceneTools::FindGizmoTranslationPlane( Ogre::Ray &pickRay, int TranslationAxis )
{
	Ogre::Quaternion qOrient = mEntitySelection->getParentSceneNode()->_getDerivedOrientation();

	Ogre::Vector3 vPos = mEntitySelection->getParentSceneNode()->_getDerivedPosition();
	Ogre::Vector3 vCamBack = mScene->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->GetCameraFps()->getDerivedDirection();
	vCamBack = -vCamBack;
	if(!TranslationAxis) return Plane(vCamBack,vPos);

	Ogre::Plane planeX(qOrient.xAxis(), vPos);
	Ogre::Plane planeY(qOrient.yAxis(), vPos);
	Ogre::Plane planeZ(qOrient.zAxis(), vPos);

	float vX = planeX.projectVector(pickRay.getDirection()).length();
	float vY = planeY.projectVector(pickRay.getDirection()).length();
	float vZ = planeZ.projectVector(pickRay.getDirection()).length();

	if(TranslationAxis & AXIS_X) vX = 10000.0f;
	if(TranslationAxis & AXIS_Y) vY = 10000.0f;
	if(TranslationAxis & AXIS_Z) vZ = 10000.0f;

	if (vX < vY && vX < vZ){ return planeX; }
	else {
		if (vY < vX && vY < vZ ) { return planeY; }
		else { return planeZ; }
	}
}

Ogre::Vector3 NxSceneTools::GetGizmoIntersect( Ogre::Ray &pickRay, Ogre::Plane &planetouse, int TranslationAxis, Ogre::Vector3 &vLastPosition)
{
	std::pair<bool,Ogre::Real> result;
	result = pickRay.intersects(planetouse);

	if (result.first) {

		Ogre::Vector3 AxisX = Ogre::Vector3::ZERO;
		Ogre::Vector3 AxisY = Ogre::Vector3::ZERO;
		Ogre::Vector3 AxisZ = Ogre::Vector3::ZERO;
		Ogre::Quaternion qOrient = mEntitySelection->getParentSceneNode()->_getDerivedOrientation();

		if( TranslationAxis ) {
			if(TranslationAxis & AXIS_X) AxisX = qOrient.xAxis();
			if(TranslationAxis & AXIS_Y) AxisY = qOrient.yAxis();
			if(TranslationAxis & AXIS_Z) AxisZ = qOrient.zAxis();
		}
		else {
			AxisX = qOrient.xAxis();
			AxisY = qOrient.yAxis();
			AxisZ = qOrient.zAxis();
		}

		Ogre::Vector3 Proj = pickRay.getPoint(result.second) - vLastPosition;
		Ogre::Vector3 vPos1 = (AxisX.dotProduct(Proj) * AxisX);
		Ogre::Vector3 vPos2 = (AxisY.dotProduct(Proj) * AxisY);
		Ogre::Vector3 vPos3 = (AxisZ.dotProduct(Proj) * AxisZ);
		Ogre::Vector3 vPos = vPos1 + vPos2 + vPos3;

		mLastTranslationDelta = vPos;
		return vPos;
	}
	return mLastTranslationDelta;
}


void setDerivedPosition( Ogre::SceneNode * node, Ogre::Vector3 pos)
{
	if(node->getParentSceneNode()) {
		Ogre::Quaternion qParent = node->getParentSceneNode()->_getDerivedOrientation().Inverse();
		Ogre::Vector3 vParent = node->getParentSceneNode()->_getDerivedPosition();
		Ogre::Vector3 newPos = (pos - vParent);
		newPos /= node->getParentSceneNode()->_getDerivedScale();
		pos = qParent * newPos;
	}

	node->setPosition( pos );    
}

#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
NxActor * gMouseSphere = NULL;
NxActor * gSelectedActor = NULL;
NxDistanceJoint*  gMouseJoint = NULL;
#endif

void NxSceneTools::OnKeyMouseMoved( const OIS::MouseEvent &e )
{
	if (!mEditionActive) return;

	float DeltaX = ( e.state.X.abs  - mLastMouse.x) * 0.5f;
	float DeltaY = ( e.state.Y.abs  - mLastMouse.y) * 0.5f;

	if( GetGizmoMode() == NXTOOL_MOVE && mLeftDown && mEntitySelection ) { //NXTOOLS_MODE::
		Ogre::Ray mouseRay;
		GetMouseRay(  mScene, Ogre::Vector2( e.state.X.abs, e.state.Y.abs ), mouseRay);
		Ogre::Vector3 vNewPos = GetGizmoIntersect( mouseRay, mLastUsedPlane, mCurrentAxis, mLastDerivedPosition );
		vNewPos = vNewPos - mLast3DDelta + mLastDerivedPosition;
		setDerivedPosition( mEntitySelection->getParentSceneNode(), vNewPos  );
		UpdateGizmo();
	}

	else if( GetGizmoMode() == NXTOOL_ROTATE && mLeftDown && mEntitySelection )
	{
		if(mCurrentAxis == 0) mCurrentAxis = AXIS_Y;
		int axisSave = mCurrentAxis;
		if( mCurrentAxis ) {
			Ogre::Quaternion q1 = mLastDerivedOrient;
			switch( mCurrentAxis ) {
				case AXIS_X:q1 = q1 * Ogre::Quaternion(Ogre::Degree(-DeltaY),Ogre::Vector3(1,0,0));break;
				case AXIS_Y:q1 = q1 * Ogre::Quaternion(Ogre::Degree(-DeltaY),Ogre::Vector3(0,1,0));break;
				case AXIS_Z:q1 = q1 * Ogre::Quaternion(Ogre::Degree(-DeltaY),Ogre::Vector3(0,0,1));break;
			}
			mEntitySelection->getParentSceneNode()->setOrientation( q1 );

			//setDerivedOrientation( mEntitySelection->getParentSceneNode() ,  q1  );
		}
	}

	else if( GetGizmoMode() == NXTOOL_SCALE && mLeftDown && mEntitySelection )
	{
		Ogre::Ray mouseRay;
		GetMouseRay( mScene, Ogre::Vector2( e.state.X.abs, e.state.Y.abs ), mouseRay);
		Ogre::Vector3 vNewDist = GetGizmoIntersect(mouseRay,mLastUsedPlane,mCurrentAxis,mLastDerivedPosition);
		Ogre::Vector3 vScale = mLastDerivedScale;
		float fNewDist = vNewDist.length();
		float fLength = mLast3DDelta.length();

		if(mCurrentAxis & AXIS_X) vScale.x *= (fNewDist / fLength);
		if(mCurrentAxis & AXIS_Y) vScale.y *= (fNewDist / fLength);
		if(mCurrentAxis & AXIS_Z) vScale.z *= (fNewDist / fLength);
		 
		mEntitySelection->getParentSceneNode()->setScale( vScale );
	}


#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
	if (!gMouseSphere) return;
	NxVec3 pos;
	Ogre::Ray NxmouseRay;
	GetMouseRay( mScene, Ogre::Vector2( e.state.X.abs, e.state.Y.abs ), NxmouseRay) ;

	Plane t_plane = Plane( Ogre::Vector3(0,1,0), Ogre::Vector3( 0,0,0 ) ); // Plane X
	std::pair<bool, Real> t_result = NxmouseRay.intersects(t_plane);
	if(t_result.first) {
		Ogre::Vector3 t_hit = NxmouseRay.getPoint(t_result.second);
		gMouseSphere->setGlobalPosition(  VecToPhysx( t_hit ) );
		gSelectedActor->wakeUp();
	}
#endif




}

#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
NxActor* CreateSphere( NxScene  * scene, const NxVec3& pos, const NxReal radius, const NxReal density)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a sphere
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = radius;
	sphereDesc.localPose.t = NxVec3(0,radius,0);
	actorDesc.shapes.pushBack(&sphereDesc);

	if (density)
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.globalPose.t = pos;
	return scene->createActor(actorDesc);	
}
#endif

void NxSceneTools::OnKeyMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{

	if (!mEditionActive) return;

	if( id == OIS::MB_Left ) mLeftDown = true;

	Ogre::Entity * mEnt = NULL;
	mLastMouse = Ogre::Vector2( e.state.X.abs, e.state.Y.abs );

	Ogre::Ray NxmouseRay;
	GetMouseRay( mScene, Ogre::Vector2( e.state.X.abs, e.state.Y.abs ), NxmouseRay) ;


	// select physics objects
	#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
	NxRay ray;
	ray.dir =  VecToPhysx( NxmouseRay.getDirection() );
	ray.orig = VecToPhysx( NxmouseRay.getOrigin() );

	NxRaycastHit hit;
	const unsigned int COLLIDABLE_MASK = ( GROUP_COLLIDABLE_PUSHABLE << 1 )  ;
	NxShape* ClosestPhysicShape = mScene->GetPhysicScene()->GetPhysicsScene3D().raycastClosestShape(ray, NX_ALL_SHAPES, hit, COLLIDABLE_MASK );
	if( ClosestPhysicShape )
	{
		if(ClosestPhysicShape->getActor().isDynamic()) {

			int hitx, hity;
			gMouseSphere = CreateSphere( &mScene->GetPhysicScene()->GetPhysicsScene3D() , hit.worldImpact, 0.1f, 1.0f);

			gMouseSphere->raiseBodyFlag(NX_BF_KINEMATIC);
			gMouseSphere->raiseActorFlag(NX_AF_DISABLE_COLLISION);
			NxDistanceJointDesc desc;
			gSelectedActor = &ClosestPhysicShape->getActor();
			gSelectedActor->wakeUp();

			desc.actor[0] = gMouseSphere;
			desc.actor[1] = gSelectedActor;

			gMouseSphere->getGlobalPose().multiplyByInverseRT(hit.worldImpact, desc.localAnchor[0]);
			gSelectedActor->getGlobalPose().multiplyByInverseRT(hit.worldImpact, desc.localAnchor[1]);
			desc.maxDistance = 0.0f;
			desc.minDistance = 0.0f;
			desc.spring.damper = 1.0f;
			desc.spring.spring = 200.0f;
			desc.flags |= NX_DJF_MAX_DISTANCE_ENABLED | NX_DJF_SPRING_ENABLED;
			NxJoint* joint = mScene->GetPhysicScene()->GetPhysicsScene3D().createJoint(desc);
			gMouseJoint = (NxDistanceJoint*)joint->is(NX_JOINT_DISTANCE);
		}
	}

	#endif
 

	/////////////////////

	
 

	if( id == OIS::MB_Left && GetGizmoMode() == NXTOOL_SELECT )
	{
		if( mEnt = CreateRaycastForType( mScene, e, NXENTITY )) {//NxEntityQueryFlags::

			mEnt->getParentSceneNode()->showBoundingBox( true );
			mEntitySelection = mEnt;
			UpdateGizmo();
			//SetGizmoMode( NXTOOLS_MODE::NXTOOL_MOVE );
		} else {

			if( mEntitySelection ) {
				mEntitySelection->getParentSceneNode()->showBoundingBox( false );
				mEntitySelection = NULL;
			}
			SetGizmoMode( NXTOOL_SELECT );


		}
	}
	else if( id == OIS::MB_Left && GetGizmoMode() == NXTOOL_MOVE | NXTOOL_ROTATE| NXTOOL_SCALE  && mEntitySelection )
	{
		Ogre::Ray mouseRay;
		if(!GetMouseRay(  mScene, Ogre::Vector2(  e.state.X.abs, e.state.Y.abs ), mouseRay)) return;

		int Axis;
		if( PickGizmos( mouseRay,  Axis ) && mEntitySelection ) {
			mCurrentAxis = (NXTOOL_AXIS) Axis;
			mLastDerivedOrient =  mEntitySelection->getParentSceneNode()->getOrientation();//  _getDerivedOrientation();
			mLastDerivedPosition = mEntitySelection->getParentSceneNode()->_getDerivedPosition();
			mLastDerivedScale = mEntitySelection->getParentSceneNode()->getScale();
			mLastUsedPlane =  FindGizmoTranslationPlane( mouseRay, Axis );
			mLast3DDelta =  GetGizmoIntersect( mouseRay, mLastUsedPlane, Axis, mLastDerivedPosition );
		}
		else
		{
			if( mEntitySelection ) {
				mEntitySelection->getParentSceneNode()->showBoundingBox( false );
				mEntitySelection = NULL;
			}
			SetGizmoMode( NXTOOL_SELECT );
		}
	}
}

void NxSceneTools::OnKeyMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{
	if (!mEditionActive) return;

	if( id == OIS::MB_Left ){ UpdateGizmo(); mLeftDown = false; }
	#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
	if( gMouseSphere ) {
		mScene->GetPhysicScene()->GetPhysicsScene3D().releaseActor(  *gMouseSphere );
		mScene->GetPhysicScene()->GetPhysicsScene3D().releaseJoint( *gMouseJoint );
		gMouseSphere = NULL;
		gMouseJoint = NULL;
	}
#endif


}
 
 

} // namespace