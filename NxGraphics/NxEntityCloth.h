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
#ifndef __NXENTITYCLOTH_H__
#define __NXENTITYCLOTH_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"


class NxCloth;
class NxMeshData;
class NxClothMesh;
class NxClothDesc;
class NxVec3;
class NxClothMeshDesc;

namespace Nx {
 
class NxGraphics_Export Nx3DObjectPhysicClothe : public NxEntity 
{
public :
	//! Nx3DObjectPhysicClothe Class Constructor.
	Nx3DObjectPhysicClothe( NxNode * Parent, const NxClotheDesc & ClotheDesc );
	//! Nx3DObjectPhysicClothe Class Destructor.
	~Nx3DObjectPhysicClothe();
	//! Update.
	void Update( float time );
	void allocateReceiveBuffers( int numVertices, int numTriangles );
	void AttachVertexToGlobalPosition( unsigned int Vertice, const Ogre::Vector3 & Position );
private :

	NxCloth * mCloth;
	Ogre::Entity * mEntity;
	struct RenderBufferVertexElement;
	std::string MatName ;
	Ogre::ManualObject* mManualObj;	
	//!The Recieve buffer that the PhysX SDK return the simulate result every step.
	NxMeshData * mReceiveBuffers;
	NxScene *mScene;
	//NxCloth *mCloth;
	NxClothMesh *mClothMesh;
	//! The rendering buffers,the most important thing for ogre rendering.
	RenderBufferVertexElement * mVertexRenderBuffer;	
	unsigned int * mIndexRenderBuffer;
	unsigned int mTexId;
	unsigned int mMaxVertices;
	unsigned int mMaxIndices;
	unsigned int mNumIndices;
	unsigned int mNumParentIndices;
	unsigned int mNumVertices;
	unsigned int mLastNumVertices;
	unsigned int mMeshDirtyFlags;
	bool mTeared;
	//! Temporary storage for tex coords
	float *mTempTexCoords;
	unsigned int mNumTempTexCoords;
	unsigned int mTriCount;
};

//!  A class to manage a 3D Physic Cloth Object.
class NxGraphics_Export Nx3DObjectPhysicCloth : public NxEntity 
{
public:
	//Nx3DObjectPhysicCloth Class Constructor.
	Nx3DObjectPhysicCloth( Ogre::SceneNode * EntitySceneNode );
	//Nx3DObjectPhysicCloth Class Destructor.
	~Nx3DObjectPhysicCloth();
	//Get Physx NxCloth.
	//NxCloth * GetNxCloth();
	//!Detach Cloth From Any Attached Object
	void DetachCloth();
	//! Set Cloth Material by Name.
	void SetMaterialName( const std::string & MaterialName );	
	//! Update Cloth.
	void Update( float time ); 
private:
	bool Create( NxClothDesc &desc, char *objFileName, float scale, NxVec3* offset, char *texFilename   );
	void render();	
	struct RenderBufferVertexElement;
	std::string MatName ;
	Ogre::ManualObject* mManualObj;	
	//!The Recieve buffer that the PhysX SDK return the simulate result every step.
	NxMeshData  * mReceiveBuffers;
	NxScene *mScene;
	NxCloth *mCloth;
	NxClothMesh *mClothMesh;
	//! The rendering buffers,the most important thing for ogre rendering.
	RenderBufferVertexElement * mVertexRenderBuffer;	
	unsigned int * mIndexRenderBuffer;
	unsigned int mTexId;
	unsigned int mMaxVertices;
	unsigned int mMaxIndices;
	unsigned int mNumIndices;
	unsigned int mNumParentIndices;
	unsigned int mNumVertices;
	unsigned int mLastNumVertices;
	unsigned int mMeshDirtyFlags;
	bool mTeared;
	//! Temporary storage for tex coords
	float *mTempTexCoords;
	unsigned int mNumTempTexCoords;
	unsigned int mTriCount;
	void init( NxScene * scene, NxClothDesc &desc, NxClothMeshDesc &meshDesc );
	bool generateObjMeshDesc( NxClothMeshDesc &desc, char *filename, float scale, NxVec3* offset, bool textured );
	void generateTearLines( NxClothMeshDesc& desc, unsigned int w, unsigned int h );
	void releaseMeshDescBuffers( const NxClothMeshDesc& desc );
	bool cookMesh(NxClothMeshDesc& desc);
	void allocateReceiveBuffers(int numVertices, int numTriangles);
	void releaseReceiveBuffers();
	bool mInitDone;
};


}
#endif