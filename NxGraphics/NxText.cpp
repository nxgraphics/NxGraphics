#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"
#include "NxConversions.h"

namespace Nx {

	Nx3DObjectText::Nx3DObjectText( NxNode * parentNode, const std::string & TextName, const std::string &caption, const std::string & FontName , float charHeight  , const NxColourValue &color   ) : NxEntity()
	{
		mType = NxTypeText ;
		mText = new NxBillboardText( TextName, caption, FontName, charHeight, NxColourtoOgre( color ) );

		mNode = parentNode->GetNxScene().GetNxSceneManager()->createSceneNode( TextName );// "NxLine" + Ogre::StringConverter::toString( instance_count() ) );
		mNode->attachObject( mText );
		//mText->setBoundingBox(AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));

		mNxNodeParent = parentNode;
		mNxNodeParent->GetNxSceneNode()->addChild( mNode );
	}

	Nx3DObjectText::~Nx3DObjectText()
	{
		LogMsg("Erasing Nx3DObjectText ...." );

		mNode->detachObject( mText );
		delete mText;

		mNxNodeParent->GetNxSceneNode()->removeAndDestroyChild(mNode->getName());

		LogMsg("Erasing Nx3DObjectPhysicLine .. DONE " );

	}
 
	void Nx3DObjectText::Update( float time )
	{
		//mText->update();
	}
 
	void Nx3DObjectText::SetVisible( bool Visible )
	{
		mText->setVisible(  Visible );
	}


}