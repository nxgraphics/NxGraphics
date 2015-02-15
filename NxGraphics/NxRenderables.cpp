#include "NxGraphicsPreCompiled.h"
#include "NxRenderables.h"

namespace Nx {

DynamicRenderable::DynamicRenderable()
{

}

DynamicRenderable::~DynamicRenderable()
{
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
}

void DynamicRenderable::initialize( RenderOperation::OperationType operationType, bool useIndices)
{
	// Initialize render operation
	mRenderOp.operationType = operationType;
	mRenderOp.useIndexes = useIndices;
	mRenderOp.vertexData = new VertexData;
	if (mRenderOp.useIndexes)
	mRenderOp.indexData = new IndexData;

	// Reset buffer capacities
	mVertexBufferCapacity = 0;
	mIndexBufferCapacity = 0;

	// Create vertex declaration
	createVertexDeclaration();
}

void DynamicRenderable::prepareHardwareBuffers( size_t vertexCount, size_t indexCount, bool UseVertexColour )
{
	// Prepare vertex buffer
	size_t newVertCapacity = mVertexBufferCapacity;
	if ((vertexCount > mVertexBufferCapacity) || (!mVertexBufferCapacity))
	{
		// vertexCount exceeds current capacity!
		// It is necessary to reallocate the buffer.

		// Check if this is the first call
		if (!newVertCapacity)
		newVertCapacity = 1;

		// Make capacity the next power of two
		while (newVertCapacity < vertexCount)
		newVertCapacity <<= 1;
	}
	else if (vertexCount < mVertexBufferCapacity>>1)
	{
		// Make capacity the previous power of two
		while (vertexCount < newVertCapacity>>1)
		newVertCapacity >>= 1;
	}


	if( newVertCapacity != mVertexBufferCapacity) 
	{
		mVertexBufferCapacity = newVertCapacity;
		// Create new vertex buffer
		HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
		mRenderOp.vertexData->vertexDeclaration->getVertexSize(0), mVertexBufferCapacity,
		HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);

		if(UseVertexColour){
		//colour buffer
		HardwareVertexBufferSharedPtr cbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR), mVertexBufferCapacity,
		Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(1, cbuf);
		}
 
	}
	// Update vertex count in the render operation
	mRenderOp.vertexData->vertexCount = vertexCount;

	if( mRenderOp.useIndexes )
	{
		OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(), "indexCount exceeds 16 bit");
		size_t newIndexCapacity = mIndexBufferCapacity;
		// Prepare index buffer
		if ((indexCount > newIndexCapacity) || (!newIndexCapacity))
		{
			// indexCount exceeds current capacity!
			// It is necessary to reallocate the buffer.

			// Check if this is the first call
			if (!newIndexCapacity)
			newIndexCapacity = 1;

			// Make capacity the next power of two
			while (newIndexCapacity < indexCount)
			newIndexCapacity <<= 1;

		}
		else if (indexCount < newIndexCapacity>>1) 
		{
			// Make capacity the previous power of two
			while (indexCount < newIndexCapacity>>1)
			newIndexCapacity >>= 1;
		}

		if (newIndexCapacity != mIndexBufferCapacity)
		{
			mIndexBufferCapacity = newIndexCapacity;
			// Create new index buffer
			mRenderOp.indexData->indexBuffer =
			HardwareBufferManager::getSingleton().createIndexBuffer( HardwareIndexBuffer::IT_16BIT, mIndexBufferCapacity, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
		}

		// Update index count in the render operation
		mRenderOp.indexData->indexCount = indexCount;
	}
}

Real DynamicRenderable::getBoundingRadius(void) const
{
	return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
}

Real DynamicRenderable::getSquaredViewDepth( const Ogre::Camera * cam ) const
{
	Ogre::Vector3 vMin, vMax, vMid, vDist;
	vMin = mBox.getMinimum();
	vMax = mBox.getMaximum();
	vMid = ((vMax - vMin) * 0.5) + vMin;
	vDist = cam->getDerivedPosition() - vMid;
	return vDist.squaredLength();
}


//////////
int CircumCircle(double xp, double yp, double x1, double y1, double x2, 
double y2, double x3, double y3, double &xc, double &yc, double &r){
  double m1, m2, mx1, mx2, my1, my2;
  double dx, dy, rsqr, drsqr;

/* Check for coincident points */
if(abs(y1 - y2) < EPSILON && abs(y2 - y3) < EPSILON)
  return(false);
if(abs(y2-y1) < EPSILON){ 
  m2 = - (x3 - x2) / (y3 - y2);
  mx2 = (x2 + x3) / 2.0;
  my2 = (y2 + y3) / 2.0;
  xc = (x2 + x1) / 2.0;
  yc = m2 * (xc - mx2) + my2;
}else if(abs(y3 - y2) < EPSILON){ 
        m1 = - (x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc = (x3 + x2) / 2.0;
        yc = m1 * (xc - mx1) + my1;
      }else{
         m1 = - (x2 - x1) / (y2 - y1); 
         m2 = - (x3 - x2) / (y3 - y2); 
         mx1 = (x1 + x2) / 2.0; 
         mx2 = (x2 + x3) / 2.0;
         my1 = (y1 + y2) / 2.0;
         my2 = (y2 + y3) / 2.0;
         xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2); 
         yc = m1 * (xc - mx1) + my1; 
       }
  dx = x2 - xc;
  dy = y2 - yc;
  rsqr = dx * dx + dy * dy;
  r = sqrt(rsqr); 
  dx = xp - xc;
  dy = yp - yc;
  drsqr = dx * dx + dy * dy;
  return((drsqr <= rsqr) ? true : false);
}
///////////////////////////////////////////////////////////////////////////////
// Triangulate() :
//   Triangulation subroutine
//   Takes as input NV vertices in array pxyz
//   Returned is a list of ntri triangular faces in the array v
//   These triangles are arranged in a consistent clockwise order.
//   The triangle array 'v' should be malloced to 3 * nv
//   The vertex array pxyz must be big enough to hold 3 more points
//   The vertex array must be sorted in increasing x values say
//
//   qsort(p,nv,sizeof(XYZ),XYZCompare);
///////////////////////////////////////////////////////////////////////////////

int Triangulate(int nv, XYZ pxyz[], ITRIANGLE v[], int &ntri){
  int *complete = NULL;
  IEDGE *edges = NULL; 
  IEDGE *p_EdgeTemp;
  int nedge = 0;
  int trimax, emax = 200;
  int status = 0;
  int inside;
  int i, j, k;
  double xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;
  double xmin, xmax, ymin, ymax, xmid, ymid;
  double dx, dy, dmax; 

/* Allocate memory for the completeness list, flag for each triangle */
  trimax = 4 * nv;
  complete = new int[trimax];
/* Allocate memory for the edge list */
  edges = new IEDGE[emax];
/*
      Find the maximum and minimum vertex bounds.
      This is to allow calculation of the bounding triangle
*/
  xmin = pxyz[0].x;
  ymin = pxyz[0].y;
  xmax = xmin;
  ymax = ymin;
  for(i = 1; i < nv; i++){
    if (pxyz[i].x < xmin) xmin = pxyz[i].x;
    if (pxyz[i].x > xmax) xmax = pxyz[i].x;
    if (pxyz[i].y < ymin) ymin = pxyz[i].y;
    if (pxyz[i].y > ymax) ymax = pxyz[i].y;
  }
  dx = xmax - xmin;
  dy = ymax - ymin;
  dmax = (dx > dy) ? dx : dy;
  xmid = (xmax + xmin) / 2.0;
  ymid = (ymax + ymin) / 2.0;
/*
   Set up the supertriangle
   his is a triangle which encompasses all the sample points.
   The supertriangle coordinates are added to the end of the
   vertex list. The supertriangle is the first triangle in
   the triangle list.
*/
  pxyz[nv+0].x = xmid - 20 * dmax;
  pxyz[nv+0].y = ymid - dmax;
  pxyz[nv+1].x = xmid;
  pxyz[nv+1].y = ymid + 20 * dmax;
  pxyz[nv+2].x = xmid + 20 * dmax;
  pxyz[nv+2].y = ymid - dmax;
  v[0].p1 = nv;
  v[0].p2 = nv+1;
  v[0].p3 = nv+2;
  complete[0] = false;
  ntri = 1;
/*
   Include each point one at a time into the existing mesh
*/
  for(i = 0; i < nv; i++){
    xp = pxyz[i].x;
    yp = pxyz[i].y;
    nedge = 0;
/*
     Set up the edge buffer.
     If the point (xp,yp) lies inside the circumcircle then the
     three edges of that triangle are added to the edge buffer
     and that triangle is removed.
*/
  for(j = 0; j < ntri; j++){
    if(complete[j])
      continue;
    x1 = pxyz[v[j].p1].x;
    y1 = pxyz[v[j].p1].y;
    x2 = pxyz[v[j].p2].x;
    y2 = pxyz[v[j].p2].y;
    x3 = pxyz[v[j].p3].x;
    y3 = pxyz[v[j].p3].y;
    inside = CircumCircle(xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r);
    if (xc + r < xp)
// Suggested
// if (xc + r + EPSILON < xp)
      complete[j] = true;
    if(inside){
/* Check that we haven't exceeded the edge list size */
      if(nedge + 3 >= emax){
        emax += 100;
        p_EdgeTemp = new IEDGE[emax];
        for (int i = 0; i < nedge; i++) { // Fix by John Bowman
          p_EdgeTemp[i] = edges[i];   
        }
        delete []edges;
        edges = p_EdgeTemp;
      }
      edges[nedge+0].p1 = v[j].p1;
      edges[nedge+0].p2 = v[j].p2;
      edges[nedge+1].p1 = v[j].p2;
      edges[nedge+1].p2 = v[j].p3;
      edges[nedge+2].p1 = v[j].p3;
      edges[nedge+2].p2 = v[j].p1;
      nedge += 3;
      v[j] = v[ntri-1];
      complete[j] = complete[ntri-1];
      ntri--;
      j--;
    }
  }
/*
  Tag multiple edges
  Note: if all triangles are specified anticlockwise then all
  interior edges are opposite pointing in direction.
*/
  for(j = 0; j < nedge - 1; j++){
    for(k = j + 1; k < nedge; k++){
      if((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1)){
        edges[j].p1 = -1;
        edges[j].p2 = -1;
        edges[k].p1 = -1;
        edges[k].p2 = -1;
      }
       /* Shouldn't need the following, see note above */
      if((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2)){
        edges[j].p1 = -1;
        edges[j].p2 = -1;
        edges[k].p1 = -1;
        edges[k].p2 = -1;
      }
    }
  }
/*
     Form new triangles for the current point
     Skipping over any tagged edges.
     All edges are arranged in clockwise order.
*/
  for(j = 0; j < nedge; j++) {
    if(edges[j].p1 < 0 || edges[j].p2 < 0)
      continue;
    v[ntri].p1 = edges[j].p1;
    v[ntri].p2 = edges[j].p2;
    v[ntri].p3 = i;
    complete[ntri] = false;
    ntri++;
  }
}
/*
      Remove triangles with supertriangle vertices
      These are triangles which have a vertex number greater than nv
*/
  for(i = 0; i < ntri; i++) {
    if(v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv) {
      v[i] = v[ntri-1];
      ntri--;
      i--;
    }
  }
  delete[] edges;
  delete[] complete;
  return 0;
} 

int XYZCompare(const void *v1, const void *v2){
  XYZ *p1, *p2;
    
  p1 = (XYZ*)v1;
  p2 = (XYZ*)v2;
  if(p1->x < p2->x)
    return(-1);
  else if(p1->x > p2->x)
         return(1);
       else
         return(0);
}

/////

NxDelaunay::NxDelaunay(int maxPoints){
	p = NULL;
	v = NULL;
	this->init(maxPoints);
}

NxDelaunay::~NxDelaunay(){
	if(p != NULL){
		delete []p;
		p = NULL;
	}
	if(v != NULL){
		delete []v;
		v = NULL;
	}
}

void NxDelaunay::init(int maxPoints){
	this->maxPoints = maxPoints;
	this->reset();
}

void NxDelaunay::reset(){
	if(p != NULL)
		delete []p;
	p = new XYZ[this->maxPoints];
	if(v != NULL)
		delete []v;
	v = NULL;
	nv = 0;
	ntri = 0;

}

int NxDelaunay::addPoint(float x, float y, float z){
	if (nv >= this->maxPoints){
		this->maxPoints = this->maxPoints * 2;            // double the size of the array if necessary
		XYZ *p_Temp = new XYZ[this->maxPoints];
		for (int i = 0; i < nv; i++)
			p_Temp[i] = p[i];
		delete []p;
		p = p_Temp; 
	}
	p[nv].x = x;
	p[nv].y = y;
	p[nv].z = z;
	nv++;
	return nv;
}

int NxDelaunay::triangulate(){
	XYZ *p_Temp = new XYZ[nv + 3]; 
	for (int i = 0; i < nv; i++)
		p_Temp[i] = p[i];      
	delete []p;
	p = p_Temp;
	v = new ITRIANGLE[3 * nv]; 
	qsort(p, nv, sizeof(XYZ), XYZCompare);
	Triangulate(nv, p, v, ntri);
	return ntri;
}

int NxDelaunay::getNumTriangles(){
	return this->ntri;
}

ITRIANGLE* NxDelaunay::getTriangles(){
	return this->v;
}

XYZ* NxDelaunay::getPoints(){
	return this->p;
}

void NxDelaunay::outputTriangles(){
	cout << "triangle count: " << ntri << endl;
	for(int i = 0; i < ntri; i++){
		cout << "triangle #" << i << endl;;
		cout << "\t" << (int)p[v[i].p1].x << "," << (int)p[v[i].p1].y << " " << (int)p[v[i].p2].x << "," << (int)p[v[i].p2].y << endl;
		cout << "\t" << (int)p[v[i].p2].x << "," << (int)p[v[i].p2].y << " " << (int)p[v[i].p3].x << "," << (int)p[v[i].p3].y << endl;
		cout << "\t" << (int)p[v[i].p3].x << "," << (int)p[v[i].p3].y << " " << (int)p[v[i].p1].x << "," << (int)p[v[i].p1].y << endl;
		cout << endl;
	}
}

void NxDelaunay::drawTriangles(){
	//for(int i = 0; i < ntri; i++){
	//	ofSetColor(0, 255, 0);
	//	ofFill();
	//	ofTriangle(p[v[i].p1].x, p[v[i].p1].y, p[v[i].p2].x, p[v[i].p2].y, p[v[i].p3].x, p[v[i].p3].y);
	//	ofSetColor(0, 0, 255);
	//	ofNoFill();
	//	ofTriangle(p[v[i].p1].x, p[v[i].p1].y, p[v[i].p2].x, p[v[i].p2].y, p[v[i].p3].x, p[v[i].p3].y);
	//}
}



}//namespace