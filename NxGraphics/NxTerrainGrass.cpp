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

float getGrassTerrainHeight(const float x, const float z, void * userData )
{
	Terrain * t = ( Terrain * ) userData;
	return t->getHeightAtWorldPosition(x, 0, z);
}

NxScene3DObjectPagedGrass::NxScene3DObjectPagedGrass( Ogre::Camera * Cam, NxScene3DObjectTerrain * TerrainPtr, const NxPagedGrassDesc & GrassDesc )
{
	TerrainGroup::TerrainIterator ti = TerrainPtr->GetTerrainGroup()->getTerrainIterator();
	Terrain * LastTerrain; int i = 0;
	while( ti.hasMoreElements()){
		i++;
		LastTerrain = ti.getNext()->instance; 
	}

	mGrass = new PagedGeometry( Cam, GrassDesc.mPageSize );
	mGrass->addDetailLevel<GrassPage>( GrassDesc.mMaxRange );

	//Create a GrassLoader object
	GrassLoader * grassLoader = new GrassLoader(mGrass);
	mGrass->setPageLoader(grassLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
	grassLoader->setHeightFunction(&getGrassTerrainHeight, LastTerrain );
	mGrass->setShadersEnabled(true);
}

NxScene3DObjectPagedGrass::~NxScene3DObjectPagedGrass()
{

}

NxScene3DObjectPagedGrassLayer * NxScene3DObjectPagedGrass::AddLayer( const NxPagedGrassLayerDesc & GrassLayer )
{
	return new NxScene3DObjectPagedGrassLayer( this, GrassLayer );
}

Forests::PagedGeometry * NxScene3DObjectPagedGrass::GetGrass()
{
	return mGrass;
}

void NxScene3DObjectPagedGrass::Update( float time )
{
	mGrass->update();
}


}//namespca