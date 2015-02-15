#include "NxGraphicsPreCompiled.h"

//Terrain
#include "../nxdeps/include/OGRE/Terrain/OgreTerrain.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainGroup.h"
#include "../nxdeps/include/OGRE/Terrain/OgreTerrainMaterialGeneratorA.h"

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


	MapChannel GetMapChannel( unsigned int ChannelId )
	{
		MapChannel Channel = Forests::CHANNEL_COLOR;
		switch( ChannelId)
		{
		case 0 :
			Channel = Forests::CHANNEL_COLOR;
			LogMsg( "GetMapChannel : COLOR");
			break;
		case 1 :
			Channel = Forests::CHANNEL_RED;
			LogMsg( "GetMapChannel : RED");
			break;
		case 2 :
			Channel = Forests::CHANNEL_GREEN;
			break;
		case 3 :
			Channel = Forests::CHANNEL_BLUE;
			break;
		case 4 :
			Channel = Forests::CHANNEL_ALPHA;
			LogMsg( "GetMapChannel : ALPHA");
			break;
		default :
			LogMsg("Error : Unknown Grass Channel");
			break;
		}
		return Channel;
	}


	NxScene3DObjectPagedGrassLayer::NxScene3DObjectPagedGrassLayer( NxScene3DObjectPagedGrass * Grass, const NxPagedGrassLayerDesc & GrassLayerDesc )
	{
		LogMsg("Creating Grass Layer...");
		GrassLoader * grassLoader =  (GrassLoader *)Grass->GetGrass()->getPageLoader();
		GrassLayer* gl = grassLoader->addLayer(  GrassLayerDesc.mMaterialName   );
		gl->setLightingEnabled(true);
		gl->setMinimumSize( GrassLayerDesc.mMinMaxSize.x , GrassLayerDesc.mMinMaxSize.x );
		gl->setMaximumSize( GrassLayerDesc.mMinMaxSize.y, GrassLayerDesc.mMinMaxSize.y  );
		gl->setAnimationEnabled(true);
		gl->setSwayDistribution( GrassLayerDesc.mSwayDistribution );	
		gl->setSwayLength( GrassLayerDesc.mSwayLength);
		gl->setSwaySpeed( GrassLayerDesc.mSwaySpeed );
		gl->setDensity( GrassLayerDesc.mDensity );
		gl->setFadeTechnique( FADETECH_ALPHA );
		gl->setRenderTechnique( GRASSTECH_CROSSQUADS);
		if( !GrassLayerDesc.mDensityMap.empty() ){ LogMsg("Set DensityMap ... "); gl->setDensityMap( GrassLayerDesc.mDensityMap, GetMapChannel( GrassLayerDesc.mDensityMapChannel ) ); }
		if( !GrassLayerDesc.mColorMap.empty() ){ LogMsg("Set ColorMap ... "); gl->setColorMap( GrassLayerDesc.mColorMap, GetMapChannel( GrassLayerDesc.mDensityMapChannel ) ); }
		gl->setMapBounds( TBounds( GrassLayerDesc.mBounds.x, GrassLayerDesc.mBounds.y, GrassLayerDesc.mBounds.z, GrassLayerDesc.mBounds.w ));

		LogMsg("Creating Grass Layer : Done");

		 
		


		
		//TexturePtr texture = TextureManager::getSingleton().createManual("Write Texture","General",TEX_TYPE_2D, gl->getDensityMap()->getPixelBox().getWidth(), gl->getDensityMap()->getPixelBox().getHeight(), 0 , PF_A8R8G8B8, Ogre::TU_STATIC );
		//texture->getBuffer()->blitFromMemory( gl->getDensityMap()->getPixelBox()   ) ;
		//Ogre::Image destImage;
		//texture->convertToImage( destImage, false );
		//destImage.save( "c:/test.jpg" );
		// 
		//Log( " Creating Grass With Bounds : X : " + Ogre::StringConverter::toString( GrassLayerDesc.mBounds.x  ));
		//Log( " Y : " + Ogre::StringConverter::toString( GrassLayerDesc.mBounds.y  ));
		//Log( " Width : " + Ogre::StringConverter::toString( GrassLayerDesc.mBounds.z  ));
		//Log( " Height : " + Ogre::StringConverter::toString( GrassLayerDesc.mBounds.w  ));

		 


	}
	
	NxScene3DObjectPagedGrassLayer::~NxScene3DObjectPagedGrassLayer()
	{
	
	
	}


}//namespace

 