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
#ifndef __NXOBJECT_H__
#define __NXOBJECT_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxDelaunayTriangulator
{
public :
	NxDelaunayTriangulator( unsigned int MaxPoints );
	~NxDelaunayTriangulator();
	void Reset();
	int AddPoint( float x , float y , float z );
	int Triangulate();
	unsigned int GetNumTriangles();
	void GetTriangles( float * Triangles );
	void GetPoints( float * Points );
private : 
	NxDelaunay * mDelaunay;

};

class NxGraphics_Export Nx3DSceneObject
{
public :
	Nx3DSceneObject();
	~Nx3DSceneObject();
	Nx3DSceneObjectTypes GetType();
	/*virtual Ogre::Mesh * GetObject();*/

protected :
	Nx3DSceneObjectTypes Type ;
};

enum NxGraphicsQuadPositions
{	
	QuadVertexTopLeft,
	QuadVertexTopMid,
	QuadVertexTopRight,
	QuadVertexMidLeft,
	QuadVertexMidMid,
	QuadVertexMidRight,
	QuadVertexBottomLeft,
	QuadVertexBottomMid,
	QuadVertexBottomRight 
};

class NxGraphics_Export Nx3DSceneObjectPatch : public Nx3DSceneObject
{
public :
	//! Nx3DSceneObjectPatch class Constructor.
	Nx3DSceneObjectPatch();
	//! Nx3DSceneObjectPatch class Deconstructor.
	~Nx3DSceneObjectPatch();
	//! Create the patch.
	void  Create( const std::string& MeshName, const std::string& ResourceGroup, float Width, float Height );
	//! Update Points
	void Update( float * ControlPoints );
	//!Set Vertex position.
	void SetVertexPosition( NxGraphicsQuadPositions Vertex, Ogre::Vector3 Positions );
	//! Get Vertex Position.
	Ogre::Vector3 & GetVertexPosition( NxGraphicsQuadPositions Vertex );
	//!Reset.
	void Reset();
	//! Update point from settings file.
	void SetCalibrationScript( std::string CalibScript );
	//! Get Mesh.
	/*Ogre::MeshPtr GetObject();*/
	//! Get Num Rows
	int GetNumRows();
	//! Get Num Colums.
	int GetNumCols();

	size_t GetVerticesWidth();
	size_t GetVerticesHeight();

private:
	struct PatchVertex
	{
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

	int m_row ;
	int m_col ;
	size_t m_MaxSubdivisionU ;
	size_t m_MaxSubdivisionV ;
	size_t PatchMeshVerticesWidth ;
	size_t PatchMeshVerticesHeight ;
	float * m_patchCtlPoints ;

	Ogre::VertexDeclaration* m_patchDecl;	
	Ogre::PatchMesh * NxPatchMesh;
	std::string CalibrationScript;
	Ogre::HardwareVertexBuffer * VertexBuffer;
	
};


}

#endif