#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"
#include "NxConversions.h"

namespace Nx {

Nx3DObjectTriangles::Nx3DObjectTriangles( NxNode * parentNode, bool UseVertexColour, bool IsUVW  ) : NxEntity()
{
	mType = NxTypeEntity;
	Nx3D_Scene * CurrentScene = &parentNode->GetNxScene(); 

	mTriangleObject = new NxTriangles( Ogre::RenderOperation::OT_TRIANGLE_LIST, UseVertexColour, IsUVW );

	//mTriangleObject = new NxTriangles( Ogre::RenderOperation::OT_TRIANGLE_STRIP, UseVertexColour );

	mTriangleObject->setQueryFlags( NXENTITY );  

	mNode = CurrentScene->GetNxSceneManager()->createSceneNode(); 
	LogMsg("Creating Node : "+ mNode->getName());
	mNode->attachObject( mTriangleObject );
	mNxNodeParent = parentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );
	mTriangleObject->setBoundingBox( Ogre::AxisAlignedBox( -1000, -1000, -1000, 1000, 1000, 1000 ) );
}

Nx3DObjectTriangles::~Nx3DObjectTriangles()
{
	LogMsg("Erasing Nx3DObjectPhysicFluid");
	mNxNodeParent->GetNxSceneNode()->removeChild( mNode );
	mNode->detachObject( mTriangleObject->getName() );
	//delete Object;
	mNxNodeParent->GetNxScene().GetNxSceneManager()->destroySceneNode( mNode );
	LogMsg("~Nx3DObjectPhysicFluid() DONE ");
}

void Nx3DObjectTriangles::Update( const float * Triangles, const float * Normals, const unsigned int * Colors, unsigned long NumTriangles )
{
	mTriangleObject->Draw( Triangles, Normals, Colors, NumTriangles ); 
 
	mNode->needUpdate(true); 
 
}

void Nx3DObjectTriangles::SetMaterialName( const std::string & MaterialName )
{
	mTriangleObject->SetMaterial( MaterialName );	 
}

const std::string & Nx3DObjectTriangles::GetMaterialName()
{
	return mTriangleObject->getMaterial()->getName(); 
}

void Nx3DObjectTriangles::SetWorldTransform( const Nx::Matrix4 & Trans )
{
	mTriangleObject->setWorldTransform( NxMat4toOgre( Trans ));
}

void Nx3DObjectTriangles::SetRenderQueueGroup( unsigned char Group )
{
	mTriangleObject->setRenderQueueGroup( Group );
}

void Nx3DObjectTriangles::SetVisible( bool Visible )
{
	mTriangleObject->setVisible( Visible );
}

void Nx3DObjectTriangles::SetVisibilityFlags( int Flags )
{
	mTriangleObject->setVisibilityFlags( Flags );
}

void Nx3DObjectTriangles::SetZOrder( unsigned short Priority )
{
	mTriangleObject->SetZOrder(Priority );
}

unsigned short Nx3DObjectTriangles::GetZOrder() const
{
	return mTriangleObject->GetZOrder();
}

void Nx3DObjectTriangles::SetTextureCoordinates( const float * Coordinates )
{
	mTriangleObject->SetTextureCoordinates( Coordinates );
}

}