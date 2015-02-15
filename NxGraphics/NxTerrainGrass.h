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
#ifndef __NXTERRAINGRASS_H__
#define __NXTERRAINGRASS_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

class NxGraphics_Export NxPagedGrassLayerDesc  
{
public:
	NxPagedGrassLayerDesc(){}
	//! Sets the boundaries of the density / color maps
	Nx::Vector4 mBounds;
	std::string mMaterialName;
	std::string mDensityMap;
	std::string mColorMap;
	//! Density Map Channel to Use( 0:Color, 1:Red, 2:Green, 3:Blue, 4:Alpha )
	unsigned int mDensityMapChannel;
	//! Sets the maximum density (measured in grass quads/billboards per square unit) of grass 
	float mDensity;
	Nx::Vector2 mMinMaxSize;
	float mSwayDistribution;	
	float mSwayLength;
	float mSwaySpeed;
};

class NxGraphics_Export NxScene3DObjectPagedGrassLayer
{
public :
	NxScene3DObjectPagedGrassLayer( NxScene3DObjectPagedGrass * Grass, const NxPagedGrassLayerDesc & GrassLayerDesc );
	~NxScene3DObjectPagedGrassLayer();

};

class NxGraphics_Export NxPagedGrassDesc  
{
public:
	NxPagedGrassDesc(){}
	float mPageSize;
	float mMaxRange; 
};

class NxGraphics_Export NxScene3DObjectPagedGrass
{
public :
	NxScene3DObjectPagedGrass( Ogre::Camera * Cam, NxScene3DObjectTerrain * Terrain, const NxPagedGrassDesc & GrassDesc );
	~NxScene3DObjectPagedGrass();
	NxScene3DObjectPagedGrassLayer * AddLayer( const NxPagedGrassLayerDesc & GrassLayer );
	void Update( float time );
	Forests::PagedGeometry * GetGrass();
private:
	Forests::PagedGeometry * mGrass;
};

class NxGraphics_Export NxPagedEntitiesDesc  
{
public:
	NxPagedEntitiesDesc(){}
	float mPageSize;
	float mPageDistance;
	float mPageDistanceFade;
	float mImpostorDistance;
	float mImpostorDistanceFade ;	 
	Nx::Vector4 mBounds;
	std::string mLightMap;
	std::string mMeshName;
	unsigned int mMeshAmount;
};

class NxGraphics_Export NxScene3DObjectPagedEntities
{
public :
	NxScene3DObjectPagedEntities( Ogre::Camera * Cam, NxScene3DObjectTerrain * DstTerrain, const NxPagedEntitiesDesc & EntitiesDesc );
	~NxScene3DObjectPagedEntities();
	void Update( float time );
private:
	Forests::PagedGeometry * mEntities;
};

}
#endif