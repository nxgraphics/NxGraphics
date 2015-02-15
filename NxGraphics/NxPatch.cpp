#include "NxGraphicsPreCompiled.h"

#define LEVEL_WIDTH(lvl) ((1 << (lvl+1)) + 1)

namespace Nx {

Nx3DSceneObjectPatch::Nx3DSceneObjectPatch()
{
	Type = Nx3DObjectPatch ;
}

Nx3DSceneObjectPatch::~Nx3DSceneObjectPatch()
{

}
/*
Ogre::MeshPtr Nx3DSceneObjectPatch::GetObject()
{
	return NxPatchMesh ;
}*/

int Nx3DSceneObjectPatch::GetNumRows()
{
	return m_row;
}

int Nx3DSceneObjectPatch::GetNumCols()
{
	return m_col;
}

size_t Nx3DSceneObjectPatch::GetVerticesWidth()
{
	return PatchMeshVerticesWidth ;
}

size_t Nx3DSceneObjectPatch::GetVerticesHeight()
{
	return PatchMeshVerticesHeight;
}

void Nx3DSceneObjectPatch::Create( const Ogre::String& mesh_name, const Ogre::String& resource_group_name, float width, float height )
{
	//m_row = 7;
	//m_col = 7;

	//m_row = 5; //33 vertices
	//m_col = 5; // 33 vertices

	m_row = 3; // 17 vertices // minimum 3 X 3 control points
	m_col = 3; // 17 vertices
 
	m_MaxSubdivisionU = 3 ; // 3 X r  + 2 x r = 5 vertices
	m_MaxSubdivisionV = 3 ;

	PatchMeshVerticesWidth  =  (LEVEL_WIDTH( m_MaxSubdivisionU )-1) * (( m_row -1)/2) + 1;
	PatchMeshVerticesHeight  = (LEVEL_WIDTH( m_MaxSubdivisionV )-1) * ((m_col-1)/2) + 1;

	//Log( " ----- NUM VERTICE WIDTH ======= " + Ogre::StringConverter::toString( PatchMeshVerticesWidth ) );
	//Log( " ----- NUM VERTICE HEIGHT ======= " + Ogre::StringConverter::toString( PatchMeshVerticesHeight ) );

	m_patchCtlPoints=(float*)(new PatchVertex[m_row*m_col]);
	int cnt=0;
	PatchVertex *pVert=(PatchVertex*)m_patchCtlPoints;
	int i,j;

	for(j=0;j<m_row;j++) {
		for(i=0;i<m_col;i++) {
			pVert->x=((float)i)/(m_col-1)* width;
			pVert->y=((float)j)/(m_row-1)*-height;
			pVert->z=0;
			pVert->nx=1.0;
			pVert->ny=0.0;
			pVert->nz=0.0;
			pVert->u=((float)i)/(m_col-1);
			pVert->v=((float)j)/(m_row-1);
			pVert++;
			++cnt;
		}
	}

	m_patchDecl = Ogre::HardwareBufferManager::getSingleton().createVertexDeclaration(); // destroyVertexDeclaration

	VertexElement PosElement = m_patchDecl->addElement(0,0,Ogre::VET_FLOAT3,Ogre::VES_POSITION);
	m_patchDecl->addElement(0,sizeof(float)*3,Ogre::VET_FLOAT3,Ogre::VES_NORMAL);
	m_patchDecl->addElement(0,sizeof(float)*6,Ogre::VET_FLOAT2,Ogre::VES_TEXTURE_COORDINATES,0);

	NxPatchMesh = Ogre::MeshManager::getSingleton().
	createBezierPatch( mesh_name,resource_group_name, m_patchCtlPoints,m_patchDecl, m_col, m_row, m_MaxSubdivisionU , m_MaxSubdivisionV ,PatchSurface::VS_BOTH ).get();
	NxPatchMesh->setSubdivision(1.0f);

	return ;//NxPatchMesh ;
}

void Nx3DSceneObjectPatch::Update( float * controlPointBuffer )
{
	NxPatchMesh->update( controlPointBuffer, m_col, m_row, m_MaxSubdivisionU, m_MaxSubdivisionV, PatchSurface::VS_BOTH );//added function in ogre sources 
}

Ogre::Vector3 & Nx3DSceneObjectPatch::GetVertexPosition( NxGraphicsQuadPositions Vertex )
{
	PatchVertex * pVert = ( PatchVertex * ) m_patchCtlPoints;
	Ogre::Vector3 vec = Ogre::Vector3( pVert[ (int) Vertex ].x, pVert[ (int) Vertex ].y, pVert[ (int) Vertex ].z );
	return vec;
}

void Nx3DSceneObjectPatch::SetVertexPosition( NxGraphicsQuadPositions Vertex, Ogre::Vector3 Positions )
{
	PatchVertex * pVert = ( PatchVertex * ) m_patchCtlPoints;
	pVert[ (int) Vertex ].x = Positions.x ;
	pVert[ (int) Vertex ].y = Positions.y ;
	pVert[ (int) Vertex ].z = Positions.z ;
	Update( m_patchCtlPoints );
	return ;
}




}//namespace