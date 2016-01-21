#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

NxRectangle2D::NxRectangle2D( NxNode * RectangleSceneNode, bool includeTextureCoordinates ) : NxEntity()
{
	mType = NxTypeEntity;

	static int index = 0;
	mRectangle = new Ogre::Rectangle2D(includeTextureCoordinates); 
	mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);

	// Render the background before everything else
	mRectangle->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
 
	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);

	mNode = RectangleSceneNode->GetNxScene().GetNxSceneManager()->createSceneNode( RectangleSceneNode->GetName() + Ogre::StringConverter::toString( index ) );
	mNode->attachObject( mRectangle );
	mNxNodeParent = RectangleSceneNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );

	index++;
}

NxRectangle2D::~NxRectangle2D()
{

}

void NxRectangle2D::SetRenderQueueGroup( unsigned char renderGroup )
{
	mRectangle->setRenderQueueGroup( renderGroup );
}

void NxRectangle2D::Update( float time )
{
	//NxScene3DObjectEntity::Update(time);
}

void NxRectangle2D::SetMaterialName( const std::string & MaterialName ) {
	mRectangle->setMaterial( MaterialName );
}
 
void NxRectangle2D::SetUVs( const Nx::Vector2 & topLeft, const Nx::Vector2 & bottomLeft, const Nx::Vector2 & topRight, const Nx::Vector2 & bottomRight ) {
	mRectangle->setUVs( 
		Ogre::Vector2( topLeft.x, topLeft.y ),
		Ogre::Vector2( bottomLeft.x, bottomLeft.y ),
		Ogre::Vector2( topRight.x, topRight.y ),
		Ogre::Vector2( bottomRight.x, bottomRight.y ) );
}



 
 


}