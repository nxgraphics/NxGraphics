#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

//Terrain
#include "../nxdeps/include/OGRE/Terrain/OgreTerrain.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainGroup.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainMaterialGeneratorA.h"
using namespace Ogre;

//Terrain Paging Geometry
#include "../nxdeps/include/OGRE/Paging/OgrePaging.h"
#include "../nxdeps/include/OGRE/Paging/OgrePagedWorldSection.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainPagedWorldSection.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainPaging.h"

//Paged Geometry
#include "../nxdeps/include/PagedGeometry/include/PagedGeometry.h"
#include "../nxdeps/include/PagedGeometry/include/BatchPage.h"
#include "../nxdeps/include/PagedGeometry/include/ImpostorPage.h"
#include "../nxdeps/include/PagedGeometry/include/TreeLoader2D.h"
#include "../nxdeps/include/PagedGeometry/include/WindBatchPage.h"
#include "../nxdeps/include/PagedGeometry/include/TreeLoader3D.h"
#include "../nxdeps/include/PagedGeometry/include/GrassLoader.h"

using namespace Forests;

#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

// [ Terrain Paging ]
//If you use, for example 0,0,1,1 you will have four pages (0,0),(0,1),(1,0),(1,1).
//#define TERRAIN_PAGE_MIN_X 0
//#define TERRAIN_PAGE_MIN_Y 0
//#define TERRAIN_PAGE_MAX_X 1
//#define TERRAIN_PAGE_MAX_Y 1

#define TERRAIN_FILE_PREFIX String("TerrainGen")
#define TERRAIN_FILE_SUFFIX String("dat")
bool mTerrainsImported = false;

void initBlendMaps( Terrain* terrain , const NxTerrainDesc & TerrainDesc )
{
	if( TerrainDesc.mTextureLayerList.size() == 3  )
	{
		//layerIndex The layer index, which should be 1 or higher (since the bottom layer has no blending).
		LogMsg("Get layer blend map 1");
		TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(1);
		LogMsg("Get layer blend map 2");
		TerrainLayerBlendMap* blendMap2 = terrain->getLayerBlendMap(2);
		LogMsg("Get layer pointers");
		float* pBlend1 = blendMap1->getBlendPointer();
		float* pBlend2 = blendMap2->getBlendPointer();
		for ( Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y ) {
			for ( Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x ) {
				Ogre::Real tx, ty;
				blendMap1->convertImageToTerrainSpace(x, y, &tx, &ty);
				Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);

				Ogre::Real val = (height - TerrainDesc.mTextureLayerList[1].mMinHeight ) / TerrainDesc.mTextureLayerList[1].mFadeDistance;
				val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
				*pBlend1++ = val;

				val = (height - TerrainDesc.mTextureLayerList[2].mMinHeight ) / TerrainDesc.mTextureLayerList[2].mFadeDistance;
				val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
				*pBlend2++ = val;
			}
		}
		blendMap1->dirty();
		blendMap2->dirty();
		blendMap1->update();
		blendMap2->update();
	}
 
}

void getTerrainHeightMapImage( bool flipX, bool flipY, Image& img, const NxTerrainDesc & TerrainDesc )
{
	LogMsg("Loading Heightmap Image ..."  );
	
	img.load( TerrainDesc.mHeightMap, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 

	LogMsg("Heightmap Img Width : " + Ogre::StringConverter::toString( img.getWidth()  )  );
	LogMsg("Heightmap Img Height : " + Ogre::StringConverter::toString( img.getHeight()  )  );

	if (flipX) {
		img.flipAroundY();
		LogMsg( "HeightMap Was Flipped in Y" );
	}
	
	if (flipY) {
		img.flipAroundX();
		LogMsg( "HeightMap Was Flipped in X" );
	}

}

void defineTerrain( TerrainGroup * mTerrainGroup, long x, long y, bool flat , const NxTerrainDesc & TerrainDesc )
{
	// if a file is available, use it
	// if not, generate file from import

	// Usually in a real project you'll know whether the compact terrain data is
	// available or not; I'm doing it this way to save distribution size

	if (flat) {
		mTerrainGroup->defineTerrain(x, y, 0.0f);
		LogMsg("Terrain Is Flat");
	}
	else {
		String filename = mTerrainGroup->generateFilename(x, y);

		// terrain exists ( .DAT )
		if (ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename)) {
			LogMsg("Terrain Is from Filename" );
			mTerrainGroup->defineTerrain(x, y);
		}
		else {// create the terrain frmo heightmap
		
			LogMsg("Terrain Is from Provided Image "  );
			Image img;
			getTerrainHeightMapImage(x % 2 != 0, y % 2 != 0, img, TerrainDesc );
			mTerrainGroup->defineTerrain(x, y, &img);
			mTerrainsImported = true;
		}
	}
}

void configureTerrainDefaults( Nx::NxScene3DObjectTerrain * ptr, Ogre::SceneManager * mSceneMgr, const NxTerrainDesc & TerrainDesc )
	{
		// MaxPixelError decides how precise our terrain is going to be.
		// A lower number will mean a more accurate terrain, at the cost of performance (because of more vertices).
		ptr->GetTerrainOptions()->setMaxPixelError(8);

		ptr->GetTerrainOptions()->setCompositeMapDistance( TerrainDesc.mCompositeMapDistance ); //view texture up to.
 

		 Ogre::TerrainMaterialGeneratorA::SM2Profile* pMatProfile =
		 static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile*>(ptr->GetTerrainOptions()->getDefaultMaterialGenerator()->getActiveProfile());

		 /*
		 pMatProfile->setCompositeMapEnabled(true);
		 pMatProfile->setLayerNormalMappingEnabled(false);
		 pMatProfile->setLayerParallaxMappingEnabled(false);
		 pMatProfile->setLayerSpecularMappingEnabled(true);
		 pMatProfile->setLightmapEnabled(true);

		 */


		//getDefaultTerrainMaterialGenerator

		//dynamic lighting
		//ptr->mTerrainGlobals->setCastsDynamicShadows(true);
		//Ogre::TerrainMaterialGeneratorA::SM2Profile* pMatProfile =
		//static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile*>(ptr->mTerrainGlobals->getDefaultMaterialGenerator()->getActiveProfile());
		//pMatProfile->setLightmapEnabled(false);
		//pMatProfile->setReceiveDynamicShadowsEnabled(true);   
		//pMatProfile->setReceiveDynamicShadowsLowLod(false);
		//pMatProfile->setReceiveDynamicShadowsDepth(true); 
		//Ogre::PSSMShadowCameraSetup* pPSSMShadowCameraSetup = static_cast<PSSMShadowCameraSetup*>(mSceneMgr->getShadowCameraSetup().get()); 
		//pMatProfile->setReceiveDynamicShadowsPSSM(pPSSMShadowCameraSetup);



	//Nx3D_Scene * Scene = NxScene3DManager::getSingleton().GetNx3DSceneFromOgreSceneManager( SceneMgr );

	//Scene->ShadowsPrepare();

	//Ogre::PSSMShadowCameraSetup* pPSSMShadowCameraSetup = static_cast<PSSMShadowCameraSetup*>(SceneMgr->getShadowCameraSetup().get()); 
	//Ogre::TerrainMaterialGeneratorA::SM2Profile* pMatProfile =
	//static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile*>(mTerrainGlobals->getDefaultMaterialGenerator()->getActiveProfile());
	//pMatProfile->setReceiveDynamicShadowsPSSM(pPSSMShadowCameraSetup);
	//mTerrainGlobals->setCastsDynamicShadows(true);

	////pMatProfile->setLightmapEnabled(false);
	//pMatProfile->setReceiveDynamicShadowsEnabled(true);   
	//pMatProfile->setReceiveDynamicShadowsLowLod(false);
	//pMatProfile->setReceiveDynamicShadowsDepth(true); 

	//Scene->ShadersRefresh() ;



		//ptr->mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);

		//void setLayerTextureName(uint8 layerIndex, uint8 samplerIndex, const String& textureName);

		
		//mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
		// ptr->mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
		//ptr->mTerrainGlobals->setLightMapSize(256);

		//matProfile->setLightmapEnabled(false);
		// Important to set these so that the terrain knows what to use for derived (non-realtime) data
		//CompositeMapDistance decides how far the Ogre terrain will render the lightmapped terrain. 

		
		ptr->GetTerrainOptions()->setLightMapDirection( NxVec3ToOgre( TerrainDesc.mLightDirection ) ); 
		ptr->GetTerrainOptions()->setCompositeMapAmbient(  NxColourtoOgre( TerrainDesc.mCompositeMapAmbient )  );
		ptr->GetTerrainOptions()->setCompositeMapDiffuse(  NxColourtoOgre(  TerrainDesc.mCompositeMapDiffuse ) ); 

		// Configure default import settings for if we use imported image
		Terrain::ImportData& defaultimp = ptr->GetTerrainGroup()->getDefaultImportSettings();
 
		defaultimp.terrainSize = TerrainDesc.mTerrainSize;
		defaultimp.worldSize = TerrainDesc.mWorldSize;
		defaultimp.inputScale = TerrainDesc.mInputScale;
		defaultimp.minBatchSize = TerrainDesc.mMinBatch;
		defaultimp.maxBatchSize = TerrainDesc.mMaxBatch;

		defaultimp.layerList.resize( TerrainDesc.mTextureLayerList.size() );

		for( int i =0; i < TerrainDesc.mTextureLayerList.size(); i++ ) {

			defaultimp.layerList[i].worldSize = TerrainDesc.mTextureLayerList[i].mWorldSize;

			for( int y = 0 ; y < TerrainDesc.mTextureLayerList[i].mTextures.size(); y++   ) {
				defaultimp.layerList[i].textureNames.push_back(TerrainDesc.mTextureLayerList[i].mTextures[y]);
				//Log("Added to layer : " + Ogre::StringConverter::toString( i ) + " texturesname :  " + TerrainDesc.mTextureLayerList[i].mTextures[y]  );
			}
		}
}

namespace Nx {

// This class just pretends to provide procedural page content to avoid page loading
class DummyPageProvider : public PageProvider
{
public:
	bool prepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
	bool loadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
	bool unloadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
	bool unprepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
};
DummyPageProvider mDummyPageProvider;

float getTerrainHeight(const float x, const float z, void * userData )
{
	Terrain * t = ( Terrain * ) userData;
	return t->getHeightAtWorldPosition(x, 0, z);
}

NxScene3DObjectTerrain::NxScene3DObjectTerrain( Nx3D_Scene * mManager, const NxTerrainDesc & TerrainDesc ) : NxEntity(), mScene(mManager)
{
	mType = NxTypeTerrain;
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW TerrainGroup( mManager->GetNxSceneManager(), Terrain::ALIGN_X_Z, TerrainDesc.mTerrainSize, TerrainDesc.mWorldSize );
	mTerrainGroup->setFilenameConvention( TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX );
	mTerrainGroup->setOrigin( NxVec3ToOgre( TerrainDesc.mTerrainPosition ) );
	configureTerrainDefaults( this, mManager->GetNxSceneManager(), TerrainDesc );


	//bool Pagging = true;


	//if(Pagging)
	//{
	//	//PAGED TERRAIN GEOMETRY
	//	// Paging setup
	//	mPageManager = OGRE_NEW PageManager();
	//	// Since we're not loading any pages from .page files, we need a way just 
	//	// to say we've loaded them without them actually being loaded
	//	mPageManager->setPageProvider(&mDummyPageProvider);

	//	mPageManager->addCamera( mScene->GetNxNode("CameraEditorNode")->GetNxCamera("CameraEditor")->GetCameraFps() );
	//	mTerrainPaging = OGRE_NEW TerrainPaging(mPageManager);

	//	PagedWorld* world = mPageManager->createWorld();

	//	float UnloadDistance = 5000;
	//	float Display = 100; //2000
	//	mTerrainPaging->createWorldSection( world, mTerrainGroup, Display, UnloadDistance, TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y,  TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y );

	//}
	//else
	//{

		bool FlatTerrain = false;
		for( long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x ) {
			for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y) {
				defineTerrain( mTerrainGroup, x, y, FlatTerrain, TerrainDesc );
				//Log("--------- Terrain is defined");
			}	
		}


		mTerrainGroup->loadAllTerrains( true );	
	
	//}

		
   
	//mTerrainsImported = true;
	if ( mTerrainsImported ) {

		 

		TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while( ti.hasMoreElements()) {

			Terrain * t = ti.getNext()->instance;

			//t->setRenderQueueGroup (Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);


		//	t->setRenderQueueGroup()

			//Log("TERRAIN NUM LAYERS : " + Ogre::StringConverter::toString( t->getLayerBlendMapSize() )  );//	

			initBlendMaps( t, TerrainDesc );

			if( !TerrainDesc.mGlobalColourMap.empty() ) {  
				t->setGlobalColourMapEnabled(true);
				Image colourMap;
				colourMap.load( TerrainDesc.mGlobalColourMap , ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				t->getGlobalColourMap()->setNumMipmaps( 5 );
				t->getGlobalColourMap()->loadImage(colourMap);
			}

			if( !TerrainDesc.mCompositeMap.empty() ) {
				Image CompositeMap;
				CompositeMap.load( TerrainDesc.mCompositeMap, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				t->getCompositeMap()->setNumMipmaps( 5 );
				t->getCompositeMap()->loadImage( CompositeMap );
			}

			if( !TerrainDesc.mLightMap.empty() ) {
				Image LightMap;
				LightMap.load( TerrainDesc.mLightMap, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				t->getLightmap()->setNumMipmaps( 5 );
				t->getLightmap()->loadImage( LightMap );
			}

		}

		mTerrainGroup->saveAllTerrains(false);
	}

	Ogre::Terrain * LastTerrain = 0;
	TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	while( ti.hasMoreElements())
	{
		Terrain * t = ti.getNext()->instance;
		LogMsg( "Creating physx terrain.."); 
		mManager->GetPhysicScene()->CreateHeightFiled( t, Ogre::Vector3( TerrainDesc.mTerrainPosition.x, TerrainDesc.mTerrainPosition.y, TerrainDesc.mTerrainPosition.z  ) );
		LastTerrain = t;
	}

	mTerrainGroup->freeTemporaryResources();
	LogMsg("Creating Terrain Done");

}

NxScene3DObjectTerrain::~NxScene3DObjectTerrain()
{

}

float NxScene3DObjectTerrain::GetHeightAtPosition( float X, float Z )
{
	Ogre::Terrain * LastTerrain = 0;
	TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	while( ti.hasMoreElements()){
		Terrain * t = ti.getNext()->instance;
		LastTerrain = t;
	}

	return LastTerrain->getHeightAtWorldPosition( Ogre::Vector3(  X, 0 ,Z )  );
}

NxScene3DObjectPagedGrass * NxScene3DObjectTerrain::CreatePagedGrass( const NxPagedGrassDesc & GrassDesc )
{
	std::string CamName = mScene->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->GetCameraFps()->getName();
	NxScene3DObjectPagedGrass * c = new NxScene3DObjectPagedGrass( mTerrainGroup->getSceneManager()->getCamera( CamName ), this, GrassDesc );
	mPagedGrass.push_back( c );
	return c;
}

NxScene3DObjectPagedEntities * NxScene3DObjectTerrain::CreatePagedEntities( const NxPagedEntitiesDesc & EntitiesDesc )
{
	std::string CamName = mScene->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->GetCameraFps()->getName();
	NxScene3DObjectPagedEntities * c = new NxScene3DObjectPagedEntities( mTerrainGroup->getSceneManager()->getCamera( CamName ), this, EntitiesDesc );
	mPagedEntities.push_back( c );
	return c;
}

Ogre::TerrainGroup * NxScene3DObjectTerrain::GetTerrainGroup()
{
	return mTerrainGroup;
}

Ogre::TerrainGlobalOptions * NxScene3DObjectTerrain::GetTerrainOptions()
{
	return mTerrainGlobals;
}

void NxScene3DObjectTerrain::Update( float time )
{
	for( int i = 0 ; i < mPagedGrass.size(); i++ ){
		mPagedGrass[i]->Update( time );
	}

	for( int i = 0 ; i < mPagedEntities.size(); i++ ){
		mPagedEntities[i]->Update( time );
	}
}

} // namespace

 



