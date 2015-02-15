/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __NXTERRAIN_H__
#define __NXTERRAIN_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

struct NxGraphics_Export NxTerrainLayerDesc {

	//! worldSize = 100, terrainsize 1000, 10 tiles.
	float mWorldSize;
	//! diffuse and normal map.
	std::vector<std::string> mTextures;
	float mMinHeight;
	float mFadeDistance;
};

class NxGraphics_Export NxTerrainDesc  
{
public:
	NxTerrainDesc(){}
	float mWorldSize;
	short mTerrainSize;
	short mMinBatch; 
	short mMaxBatch;
	float mInputScale;
	Nx::Vector3 mTerrainPosition;
	std::string mHeightMap;
	std::string mGlobalColourMap;
	std::string mCompositeMap;
	float mCompositeMapDistance;
	std::string mLightMap;
	Nx::Vector3 mLightDirection;
	NxColourValue mCompositeMapAmbient;
	NxColourValue mCompositeMapDiffuse;
	std::vector<NxTerrainLayerDesc> mTextureLayerList;
};

class NxGraphics_Export NxScene3DObjectTerrain : public NxEntity
{
public:
	//! NxScene3DObjectTerrain Constructor.
	NxScene3DObjectTerrain( Nx3D_Scene * mManager, const NxTerrainDesc & TerrainDesc );
	//! NxScene3DObjectTerrain Destructor.
	~NxScene3DObjectTerrain();
	//! Create Paged Grass.
	NxScene3DObjectPagedGrass * CreatePagedGrass( const NxPagedGrassDesc & GrassDesc );
	//! Create Paged Entities.
	NxScene3DObjectPagedEntities * CreatePagedEntities( const NxPagedEntitiesDesc & EntitiesDesc );
	//! get Height at 2D position.
	float GetHeightAtPosition( float X, float Z );
	//! Get Terrain Group.
	Ogre::TerrainGroup * GetTerrainGroup();
	//! Get Terrain Options.
	Ogre::TerrainGlobalOptions * GetTerrainOptions();
	//! Update If Needed.
	void Update( float time );
private :
	Ogre::TerrainGlobalOptions * mTerrainGlobals;
	Ogre::TerrainGroup * mTerrainGroup;
	Nx3D_Scene * mScene;
	std::vector<NxScene3DObjectPagedGrass*> mPagedGrass;
	std::vector<NxScene3DObjectPagedEntities*> mPagedEntities;
};

}

#endif