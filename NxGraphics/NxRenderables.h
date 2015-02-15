#include "NxPrerequisites.h"
#include "../nxdeps/include/OGRE/OgreSimpleRenderable.h"
//using namespace Ogre;

namespace Nx {

enum 
{
	POSITION_BINDING,
	TEXCOORD_BINDING
};

/// Abstract base class providing mechanisms for dynamically growing hardware buffers.
class DynamicRenderable : public Ogre::SimpleRenderable
{
	public:
	/// Constructor
	DynamicRenderable();
	/// Virtual destructor
	virtual ~DynamicRenderable();

	/** Initializes the dynamic renderable.
	@remarks
	This function should only be called once. It initializes the
	render operation, and calls the abstract function
	createVertexDeclaration().
	@param operationType The type of render operation to perform.
	@param useIndices Specifies whether to use indices to determine the
	vertices to use as input. */
	void initialize( Ogre::RenderOperation::OperationType operationType, bool useIndices);

	/// Implementation of Ogre::SimpleRenderable
	virtual Ogre::Real getBoundingRadius(void) const;
	/// Implementation of Ogre::SimpleRenderable
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;

	protected:
	/// Maximum capacity of the currently allocated vertex buffer.
	size_t mVertexBufferCapacity;
	/// Maximum capacity of the currently allocated index buffer.
	size_t mIndexBufferCapacity;

	/** Creates the vertex declaration.
	@remarks
	Override and set mRenderOp.vertexData->vertexDeclaration here.
	mRenderOp.vertexData will be created for you before this method
	is called. */
	virtual void createVertexDeclaration() = 0;

	/** Prepares the hardware buffers for the requested vertex and index counts.
	@remarks
	This function must be called before locking the buffers in
	fillHardwareBuffers(). It guarantees that the hardware buffers
	are large enough to hold at least the requested number of
	vertices and indices (if using indices). The buffers are
	possibly reallocated to achieve this.
	@par
	The vertex and index count in the render operation are set to
	the values of vertexCount and indexCount respectively.
	@param vertexCount The number of vertices the buffer must hold.

	@param indexCount The number of indices the buffer must hold. This
	parameter is ignored if not using indices. */
	virtual void prepareHardwareBuffers(size_t vertexCount, size_t indexCount,  bool UseVertexColour);

	/** Fills the hardware vertex and index buffers with data.
	@remarks
	This function must call prepareHardwareBuffers() before locking
	the buffers to ensure the they are large enough for the data to
	be written. Afterwards the vertex and index buffers (if using
	indices) can be locked, and data can be written to them. */
	virtual void fillHardwareBuffers() = 0;
};

class NxLine : public DynamicRenderable
{
	typedef Ogre::Vector3 Vector3;
	typedef Ogre::Quaternion Quaternion;
	typedef Ogre::Camera Camera;
	typedef Ogre::Real Real;
	typedef Ogre::RenderOperation::OperationType OperationType;

	public:
	/// Constructor - see setOperationType() for description of argument.
	NxLine( OperationType opType = Ogre::RenderOperation::OT_LINE_STRIP, bool UseVertexColour = false  );
	~NxLine();

	/// Add a point to the point list
	void addPoint(const Nx::Vector3 &p, const NxColourValue & Col );
	/// Add a point to the point list
	void addPoint(Real x, Real y, Real z, const NxColourValue & Col  );

	/// Change the location of an existing point in the point list
	void setPoint(unsigned short index, const Nx::Vector3 &value);

	void setPointColour( unsigned short Index, const NxColourValue &Value );

	/// Return the location of an existing point in the point list
	const Nx::Vector3& getPoint(unsigned short index) const;

	/// Return the total number of points in the point list
	unsigned short getNumPoints(void) const;
	//! Set Material Name.
	void SetMaterial( const std::string & MaterialName );

	/// Remove all points from the point list
	void clear();

	/// Call this to update the hardware buffer after making changes.  
	void update();

	/** Set the type of operation to draw with.
	* @param opType Can be one of 
	*    - RenderOperation::OT_LINE_STRIP
	*    - RenderOperation::OT_LINE_LIST
	*    - RenderOperation::OT_POINT_LIST
	*    - RenderOperation::OT_TRIANGLE_LIST
	*    - RenderOperation::OT_TRIANGLE_STRIP
	*    - RenderOperation::OT_TRIANGLE_FAN
	*    The default is OT_LINE_STRIP.
	*/
	void setOperationType(OperationType opType);
	OperationType getOperationType() const;


	/** Returns an interpolated point based on a parametric value over the whole series.
	@remarks
	Given a t value between 0 and 1 representing the parametric distance along the
	whole length of the spline, this method returns an interpolated point.
	@param t Parametric value.
	*/
	Vector3 Interpolate(Real t) const;

	/** Interpolates a single segment of the spline given a parametric value.
	@param fromIndex The point index to treat as t=0. fromIndex + 1 is deemed to be t=1
	@param t Parametric value
	*/
	Vector3 Interpolate(unsigned int fromIndex, Real t) const;


	protected:
	/// Implementation DynamicRenderable, creates a simple vertex-only decl
	void createVertexDeclaration();
	/// Implementation DynamicRenderable, pushes point list out to hardware memory
	void fillHardwareBuffers();

	private:
	std::vector<Nx::Vector3> mPoints;
	std::vector<int> mPointsColor;
	bool mDirty;
	bool mUseVertexColour;

	Ogre::SimpleSpline * mAnimSpline;
};


struct NxPoint
{
	Ogre::Vector3 Position;
	Ogre::Vector3 Colour;

	NxPoint( Ogre::Vector3 Pos, Ogre::Vector3 Col) : Position(Pos), Colour(Col)
	{
	
	}
};

class NxParticles : public DynamicRenderable
{
	typedef Ogre::RenderOperation::OperationType OperationType;
public :
	/// Constructor - see setOperationType() for description of argument.
	NxParticles( OperationType opType = Ogre::RenderOperation::OT_POINT_LIST, bool UseVertexColour = false );
	// destructor
	~NxParticles();
	//! Draw Particles.
	void Draw( const float * Data, const unsigned int * Colors, unsigned long ParticlesNum );
	//! Set Material Name.
	void SetMaterial( const std::string & MaterialName );
protected:
	/// Implementation DynamicRenderable, creates a simple vertex-only decl
	void createVertexDeclaration();
	/// Implementation DynamicRenderable, pushes point list out to hardware memory
	void fillHardwareBuffers();
private:
	bool mDirty;
	bool mUseVertexColour;
};

////////////
#include <iostream>
#include <stdlib.h> // for C qsort 
#include <cmath>
#include <time.h> // for random

const int MaxVertices = 500;
const int MaxTriangles = 1000;
//const int n_MaxPoints = 10; // for the test programm
const double EPSILON = 0.000001;

struct ITRIANGLE{
	int p1, p2, p3;
};

struct IEDGE{
	int p1, p2;
};

struct XYZ{
	double x, y, z;
};

int XYZCompare(const void *v1, const void *v2);
int Triangulate(int nv, XYZ pxyz[], ITRIANGLE v[], int &ntri);
int CircumCircle(double, double, double, double, double, double, double, double, double&, double&, double&);
///////////////////////

#define DEFAULT_MAX_POINTS 500

class NxDelaunay{
private:
	int maxPoints;
	ITRIANGLE *v;
	XYZ *p;
	int nv;
	int ntri;

public:
	NxDelaunay(int maxPoints=DEFAULT_MAX_POINTS);
	~NxDelaunay();

	void init(int maxPoints=DEFAULT_MAX_POINTS);
	void reset();

	int addPoint(float x=0.0, float y=0.0, float z=0.0);
	int triangulate();

	int getNumTriangles();
	ITRIANGLE* getTriangles();
	XYZ* getPoints();

	void outputTriangles();
	void drawTriangles();	
};




//////


class NxTriangles : public DynamicRenderable
{
	typedef Ogre::RenderOperation::OperationType OperationType;
public :
	/// Constructor - see setOperationType() for description of argument.
	NxTriangles( OperationType opType = Ogre::RenderOperation::OT_TRIANGLE_LIST, bool UseVertexColour = false, bool UVW = false );
	// destructor
	~NxTriangles();

	void prepareHardwareBuffers(size_t vertexCount, size_t indexCount,  bool UseVertexColour);
	//! Draw Particles.
	void Draw( const float * TriangleData, const float * TriangleNormals, const unsigned int * Colors, unsigned long NumTriangles );
	//! Set Material Name.
	void SetMaterial( const std::string & MaterialName );

	void SetTextureCoordinates( const float * Coordinates );

	void _updateRenderQueue(Ogre::RenderQueue* queue);
	const Ogre::String& getMovableType(void) const;
	Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;

	void SetZOrder( unsigned short Priority );
	unsigned short GetZOrder() const;



protected:
	/// Implementation DynamicRenderable, creates a simple vertex-only decl
	void createVertexDeclaration();
	/// Implementation DynamicRenderable, pushes point list out to hardware memory
	void fillHardwareBuffers();
private:
	bool mDirty;
	bool mUseVertexColour;
	unsigned short mZorder;
	bool mUVW;
 
};

////
class NxBillboardText : public Ogre::MovableObject, public Ogre::Renderable
{
	/******************************** MovableText data ****************************/
public:
	enum HorizontalAlignment    {H_LEFT, H_CENTER};
	enum VerticalAlignment      {V_BELOW, V_ABOVE, V_CENTER};

protected:
	Ogre::String					mFontName;
	Ogre::String					mType;
	Ogre::String					mName;
	Ogre::String					mCaption;
	HorizontalAlignment		mHorizontalAlignment;
	VerticalAlignment		mVerticalAlignment;

	Ogre::ColourValue				mColor;
	Ogre::RenderOperation			mRenderOp;
	Ogre::AxisAlignedBox			mAABB;
	Ogre::LightList				mLList;

	Ogre::Real					mCharHeight;
	Ogre::Real					mSpaceWidth;

	bool					mNeedUpdate;
	bool					mUpdateColors;
	bool					mOnTop;

	Ogre::Real					mTimeUntilNextToggle;
	Ogre::Real					mRadius;

	Ogre::Vector3					mGlobalTranslation;
	Ogre::Vector3					mLocalTranslation;

	Ogre::Camera					*mpCam;
	Ogre::RenderWindow			*mpWin;
	Ogre::Font					*mpFont;
	Ogre::MaterialPtr				mpMaterial;
	Ogre::MaterialPtr				mpBackgroundMaterial;

	/******************************** public methods ******************************/
public:
	NxBillboardText(const Ogre::String &name, const Ogre::String &caption, const Ogre::String &fontName = "BlueHighway-8", Ogre::Real charHeight = 1.0, const Ogre::ColourValue &color = Ogre::ColourValue::White);
	virtual ~NxBillboardText();

	// Add to build on Shoggoth:
	virtual void				visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

	// Set settings
	void						setFontName(const Ogre::String &fontName);
	void						setCaption(const Ogre::String &caption);
	void						setColor(const Ogre::ColourValue &color);
	void						setCharacterHeight(Ogre::Real height);
	void						setSpaceWidth(Ogre::Real width);
	void						setTextAlignment(const HorizontalAlignment& horizontalAlignment, const VerticalAlignment& verticalAlignment);
	void						setGlobalTranslation( Ogre::Vector3 trans );
	void						setLocalTranslation( Ogre::Vector3 trans );
	void						showOnTop(bool show=true);

	// Get settings
	const   Ogre::String				&getFontName()	const {return mFontName;}
	const   Ogre::String				&getCaption()	const {return mCaption;}
	const   Ogre::ColourValue			&getColor()		const {return mColor;}

	Ogre::Real						getCharacterHeight() const {return mCharHeight;}
	Ogre::Real						getSpaceWidth() const {return mSpaceWidth;}
	Ogre::Vector3						getGlobalTranslation() const {return mGlobalTranslation;}
	Ogre::Vector3						getLocalTranslation() const {return mLocalTranslation;}
	bool						getShowOnTop() const {return mOnTop;}
	Ogre::AxisAlignedBox				GetAABB(void) { return mAABB; }

	/******************************** protected methods and overload **************/
protected:

	// from MovableText, create the object
	void						_setupGeometry();
	void						_updateColors();

	// from MovableObject
	void						getWorldTransforms(Ogre::Matrix4 *xform) const;
	Ogre::Real						getBoundingRadius(void) const {return mRadius;};
	Ogre::Real						getSquaredViewDepth(const Ogre::Camera *cam) const {return 0;};
	const   Ogre::Quaternion			&getWorldOrientation(void) const;
	const   Ogre::Vector3				&getWorldPosition(void) const;
	const   Ogre::AxisAlignedBox		&getBoundingBox(void) const {return mAABB;};
	const   Ogre::String				&getName(void) const {return mName;};
	const   Ogre::String				&getMovableType(void) const {static Ogre::String movType = "MovableText"; return movType;};

	void						_notifyCurrentCamera(Ogre::Camera *cam);
	void						_updateRenderQueue(Ogre::RenderQueue* queue);

	// from renderable
	void						getRenderOperation(Ogre::RenderOperation &op);
	const   Ogre::MaterialPtr			&getMaterial(void) const {assert(!mpMaterial.isNull());return mpMaterial;};
	const   Ogre::LightList			&getLights(void) const {return mLList;};
};


}