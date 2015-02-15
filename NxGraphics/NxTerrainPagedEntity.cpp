#include "NxGraphicsPreCompiled.h"

//Terrain
#include "../nxdeps/include/OGRE/Terrain/OgreTerrain.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainGroup.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainMaterialGeneratorA.h"
using namespace Ogre;

//Paged Geometry
#include "../nxdeps/include/PagedGeometry/include/PagedGeometry.h"
#include "../nxdeps/include/PagedGeometry/include/BatchPage.h"
#include "../nxdeps/include/PagedGeometry/include/ImpostorPage.h"
#include "../nxdeps/include/PagedGeometry/include/TreeLoader2D.h"
#include "../nxdeps/include/PagedGeometry/include/WindBatchPage.h"
#include "../nxdeps/include/PagedGeometry/include/TreeLoader3D.h"
#include "../nxdeps/include/PagedGeometry/include/GrassLoader.h"

using namespace Forests;

namespace Nx {

float getEntityTerrainHeight(const float x, const float z, void * userData )
{
	Terrain * t = ( Terrain * ) userData;
	return t->getHeightAtWorldPosition(x, 0, z);
}

NxScene3DObjectPagedEntities::NxScene3DObjectPagedEntities( Ogre::Camera * Cam, NxScene3DObjectTerrain * DstTerrain, const NxPagedEntitiesDesc & EntitiesDesc )
{

	TerrainGroup::TerrainIterator ti = DstTerrain->GetTerrainGroup()->getTerrainIterator();
	Terrain * LastTerrain; 
	while( ti.hasMoreElements()){
		LastTerrain = ti.getNext()->instance; 
	}

	//Create and configure a new PagedGeometry instance
	mEntities = new PagedGeometry( Cam, EntitiesDesc.mPageSize );
	mEntities->setInfinite();
	//trees->setShadersEnabled(false); // because of the shadow refresh function


	mEntities->addDetailLevel<BatchPage>( EntitiesDesc.mPageDistance , EntitiesDesc.mPageDistanceFade );		//Use batches up to 150 units away, and fade for 30 more units
	mEntities->addDetailLevel<ImpostorPage>( EntitiesDesc.mImpostorDistance , EntitiesDesc.mImpostorDistanceFade);	//Use impostors up to 400 units, and for for 50 more units
	
	TreeLoader2D * treeLoader = new TreeLoader2D( mEntities, TBounds( EntitiesDesc.mBounds.x, EntitiesDesc.mBounds.y, EntitiesDesc.mBounds.z, EntitiesDesc.mBounds.w ));
	treeLoader->setHeightFunction(&getEntityTerrainHeight, LastTerrain );
	treeLoader->setMaximumScale(4.0f);


	//[NOTE] This sets the color map, or lightmap to be used for trees. All trees will be colored according
	//to this texture. In this case, the shading of the terrain is used so trees will be shadowed
	//just as the terrain is (this should appear like the terrain is casting shadows on the trees).
	//You may notice that TreeLoader2D / TreeLoader3D doesn't have a setMapBounds() function as GrassLoader
	//does. This is because the bounds you specify in the TreeLoader2D constructor are used to apply
	//the color map.
	if( !EntitiesDesc.mLightMap.empty() ) treeLoader->setColorMap(  EntitiesDesc.mLightMap );



	//Load a tree entity
	//Entity *tree1 = SceneMgr->createEntity("Tree1", "fir05_30.mesh");

	Entity *tree2 = DstTerrain->GetTerrainGroup()->getSceneManager()->createEntity("Tree2", EntitiesDesc.mMeshName);

	//tree2->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAIN);



	//Randomly place 10000 copies of the tree on the terrain
	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	Ogre::Radian yaw;
	Real scale;
	for (int i = 0; i < EntitiesDesc.mMeshAmount ; i++)
	{
		yaw = Ogre::Degree(Math::RangeRandom(0, 360));

		position.x = Math::RangeRandom( EntitiesDesc.mBounds.x, EntitiesDesc.mBounds.z);
		position.z = Math::RangeRandom( EntitiesDesc.mBounds.y, EntitiesDesc.mBounds.w);

		//scale = Math::RangeRandom(0.07f, 1.00f);

		float rnd = Math::UnitRandom();

		treeLoader->addTree(tree2, position, yaw, 1.00f);

		//if (rnd < 0.5f)
		//{
		////[NOTE] Unlike TreeLoader3D, TreeLoader2D's addTree() function accepts a Vector2D position (x/z)
		////The Y value is calculated during runtime (to save memory) from the height function supplied (above)
		//if (Math::UnitRandom() < 0.5f)
		//	treeLoader->addTree(tree1, position, yaw, scale);
		////else
		////	treeLoader->addTree(tree2, position, yaw, scale);
		//}
		//else
		//	treeLoader->addTree(tree2, position, yaw, scale);
	}

	mEntities->setPageLoader(treeLoader); 



/*
//Load a bush entity
Entity *fern = SceneMgr->createEntity("Fern", "farn1.mesh");

Entity *plant = SceneMgr->createEntity("Plant", "plant2.mesh");

Entity *mushroom = SceneMgr->createEntity("Mushroom", "shroom1_1.mesh");

Ogre::Vector3 positionBush = Ogre::Vector3::ZERO;
Radian yawBush;
Real scaleBush;

//Randomly place 20,000 bushes on the terrain
for (int i = 0; i < 20000; i++)
{
yawBush = Degree(Math::RangeRandom(0, 360));
positionBush.x = Math::RangeRandom(0, 1000);
positionBush.z = Math::RangeRandom(0, 1000);

float rnd = Math::UnitRandom();
if (rnd < 0.8f) 
{
scaleBush = Math::RangeRandom(0.3f, 0.4f);
bushLoader->addTree(fern, positionBush, yawBush, scaleBush);
}
else if (rnd < 0.9)
{
scaleBush = Math::RangeRandom(0.2f, 0.6f);
bushLoader->addTree(mushroom, positionBush, yawBush, scaleBush);
} 
else 
{
scaleBush = Math::RangeRandom(0.3f, 0.5f);
bushLoader->addTree(plant, positionBush, yawBush, scaleBush);
}
}
*/





}

NxScene3DObjectPagedEntities::~NxScene3DObjectPagedEntities()
{

}

void NxScene3DObjectPagedEntities::Update( float time )
{
	mEntities->update();
}

}//namespace