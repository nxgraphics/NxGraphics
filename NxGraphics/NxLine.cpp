#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"
#include "NxConversions.h"

namespace Nx {

Nx3DObjectPhysicLine::Nx3DObjectPhysicLine( NxNode * parentNode, const std::string & name, bool UseVertexColor ) : NxEntity()
{
	mType = NxTypeLine ;
	mLines = new NxLine( RenderOperation::OT_LINE_STRIP, UseVertexColor );

	mNode = parentNode->GetNxScene().GetNxSceneManager()->createSceneNode( name );// "NxLine" + Ogre::StringConverter::toString( instance_count() ) );
	mNode->attachObject( mLines );
	mLines->setBoundingBox(AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));

	mNxNodeParent = parentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );
}

Nx3DObjectPhysicLine::~Nx3DObjectPhysicLine()
{
	LogMsg("Erasing Nx3DObjectPhysicLine ...." );

	mNode->detachObject( mLines );
	delete mLines;

	mNxNodeParent->GetNxSceneNode()->removeAndDestroyChild(mNode->getName());

	LogMsg("Erasing Nx3DObjectPhysicLine .. DONE " );
 
}

void Nx3DObjectPhysicLine::AddPoint( float x, float y, float z, const NxColourValue & Col )
{
	mLines->addPoint( x, y, z,   Col   );
}

void Nx3DObjectPhysicLine::AddPoint( const Nx::Vector3 & Pos, const NxColourValue & Col )
{
	mLines->addPoint( Pos,   Col  );
}

void Nx3DObjectPhysicLine::SetPoint( unsigned short Index, const Nx::Vector3 &Value )
{
	mLines->setPoint( Index,   Value  ); 
}

void Nx3DObjectPhysicLine::SetPointColour( unsigned short Index, const NxColourValue &Value )
{
	mLines->setPointColour( Index,  Value  );
}

const Nx::Vector3 & Nx3DObjectPhysicLine::GetPointPosition( unsigned short Index ) const
{
	return  mLines->getPoint( Index  );
}

unsigned short Nx3DObjectPhysicLine::GetNumPoints()
{
	return mLines->getNumPoints();
}

void Nx3DObjectPhysicLine::Update( float time )
{
	mLines->update();
}

Nx::Vector3 Nx3DObjectPhysicLine::Interpolate(float t) const
{
	 
	return OgreToNxVec3( mLines->Interpolate( t ) );
}

Nx::Vector3 Nx3DObjectPhysicLine::Interpolate(unsigned int fromIndex, float t) const
{
	return  OgreToNxVec3( mLines->Interpolate( fromIndex, t ));
}

void Nx3DObjectPhysicLine::Clear()
{
	mLines->clear();
}

void Nx3DObjectPhysicLine::SetMaterialName( const std::string & MaterialName )
{
	mLines->SetMaterial( MaterialName );
}

void Nx3DObjectPhysicLine::SetVisible( bool Visible )
{
	mLines->setVisible(  Visible );
}


}