#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"

namespace Nx {

	NxDelaunayTriangulator::NxDelaunayTriangulator( unsigned int MaxPoints )
	{
		 mDelaunay = new NxDelaunay( MaxPoints );
	}

	NxDelaunayTriangulator::~NxDelaunayTriangulator()
	{
	
	}

	void NxDelaunayTriangulator::Reset()
	{
		mDelaunay->reset();
	}

	int NxDelaunayTriangulator::AddPoint(float x , float y , float z )
	{
		return mDelaunay->addPoint( x,y,z);
	}

	int NxDelaunayTriangulator::Triangulate()
	{
		return mDelaunay->triangulate();
	}

	unsigned int NxDelaunayTriangulator::GetNumTriangles()
	{
		return mDelaunay->getNumTriangles();
	}

	void NxDelaunayTriangulator::GetTriangles( float * Triangles )
	{
		ITRIANGLE* v = mDelaunay->getTriangles();
		XYZ* p = mDelaunay->getPoints();
		int y = 0;
		for( unsigned int i = 0 ; i < mDelaunay->getNumTriangles()* 3  *3; i += 9 ) {

			// Clockwise
			Triangles[i]   = p[v[y].p3].x;
			Triangles[i+1] = p[v[y].p3].y;
			Triangles[i+2] = 0.0f;

			Triangles[i+3] = p[v[y].p2].x;
			Triangles[i+4] = p[v[y].p2].y;
			Triangles[i+5] = 0.0f;

			Triangles[i+6] = p[v[y].p1].x;
			Triangles[i+7] = p[v[y].p1].y;
			Triangles[i+8] = 0.0f;
			y++;
		}
	}

	void NxDelaunayTriangulator::GetPoints( float * Points )
	{
	
	
	}

}