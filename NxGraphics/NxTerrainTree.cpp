#include "NxGraphicsPreCompiled.h"
//#include "NxGraphics_3D_Physics_Include.h"
using namespace Ogre;

#include "../nxdeps/include/CaduneTree/CTPrerequisites.h"
#include "../nxdeps/include/CaduneTree/CTParameters.h"
#include "../nxdeps/include/CaduneTree/CTSerializer.h"
//#include "CTSection.h"
#include "../nxdeps/include/CaduneTree/CTStem.h"

using namespace CaduneTree;

namespace Nx {


NxScene3DObjectTree::NxScene3DObjectTree( NxNode * TreeSceneNode ) : NxEntity()
{
	mType = NxTypeEntity;

	static int Index = 0;

	//Materials: "BarkNoLighting", "Leaves" and "Frond" are default ones.

	Parameters * mParameters = new Parameters();

		//void mParameters->setNumBranches( unsigned int level, unsigned char numBranches ) {
 
	Stem * mTrunk = new Stem( mParameters );
	mTrunk->grow( Ogre::Quaternion::IDENTITY, Ogre::Vector3::ZERO );

	Ogre::ManualObject * manualObject = new Ogre::ManualObject("Tree"+ Ogre::StringConverter::toString( Index ));
	mTrunk->createGeometry( manualObject );

	BillboardSet * Set = TreeSceneNode->GetNxSceneNode()->getCreator()->createBillboardSet("Leaves"+ Ogre::StringConverter::toString( Index ));
	mTrunk->createLeaves( Set );

	TreeSceneNode->GetNxSceneNode()->attachObject(manualObject);

	TreeSceneNode->GetNxSceneNode()->attachObject( Set );

	TreeSceneNode->GetNxSceneNode()->setVisible(true);

	Index++;



}

NxScene3DObjectTree::~NxScene3DObjectTree()
{

}

void NxScene3DObjectTree::Update( float time )
{
	NxEntity::Update(time);
 
}

 
 


}