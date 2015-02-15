#include "NxGraphicsPreCompiled.h"

#define POSITION_BINDING 0
#define NORMAL_BINDING 1
#define TEXCOORD_BINDING 2

namespace Nx {

    class NxLine2D : public MovableObject, public Renderable // public SimpleRenderable
    {
    protected:

		//Ogre::TextureUnitState * mState;



		void setMaterial( const String& matName )
		{
			mMatName = matName;
			mMaterial = MaterialManager::getSingleton().getByName(mMatName);
			if (mMaterial.isNull())
				OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + mMatName,
				"SimpleRenderable::setMaterial" );

			// Won't load twice anyway
			mMaterial->load();
		}

		const MaterialPtr& getMaterial(void) const
		{
			return mMaterial;
		}

		const String& getMovableType(void) const
		{
			static String movType = "NxGuiElement";
			return movType;
		}
		//-----------------------------------------------------------------------
		const LightList& getLights(void) const
		{
			// Use movable query lights
			return queryLights();
		}

		const AxisAlignedBox& getBoundingBox(void) const
		{
			return mBox;
		}

		void _updateRenderQueue(RenderQueue* queue)
		{
			if( this->isVisible() ) {
				queue->addRenderable( this, mRenderQueueID, mZorder ); 
			}
		}

		void visitRenderables(Renderable::Visitor* visitor,  bool debugRenderables)
		{
			visitor->visit(this, 0, false); 
		}

		void getRenderOperation( RenderOperation& op )
		{
			op = mRenderOp;
		}

		void setRenderOperation( const RenderOperation& rend )
		{
			mRenderOp = rend;
		}

		void getWorldTransforms( Ogre::Matrix4* xform ) const
		{
			// return identity matrix to prevent parent transforms
			//*xform = Matrix4::IDENTITY;	

			float X =  ( mMain->getX() / mScreenWidth ) * 2.0f ;
			float Y =  ( mMain->getY() / -mScreenHeight ) * 2.0f ;

			Ogre::Matrix3 rot3x3, scale3x3;
			rot3x3.FromEulerAnglesXYZ(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian(0));
			scale3x3 = Ogre::Matrix3::ZERO;
			scale3x3[0][0] = 1.0f;
			scale3x3[1][1] = 1.0f;
			scale3x3[2][2] = 1.0f;

			Ogre::Matrix4 mTransform = Ogre::Matrix4::IDENTITY;
			mTransform = rot3x3 * scale3x3;
			mTransform.setTrans(Ogre::Vector3( X, Y, 0));

			*xform = mTransform;

		}

		void prepareHardwareBuffers( size_t vertexCount, size_t indexCount )
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

		 
				//colour buffer
				HardwareVertexBufferSharedPtr cbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR), mVertexBufferCapacity,
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
				// Bind buffer
				mRenderOp.vertexData->vertexBufferBinding->setBinding(1, cbuf);
			 

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



		void _initRectangle2D(  Ogre::HardwareBuffer::Usage vBufUsage)
		{

			mRenderOp.vertexData = OGRE_NEW VertexData();

			// Initialize render operation
			mRenderOp.operationType = RenderOperation::OT_LINE_STRIP;
			mRenderOp.useIndexes = false;
			//mRenderOp.vertexData = new VertexData;
			if (mRenderOp.useIndexes)
				mRenderOp.indexData = new IndexData;

			// Reset buffer capacities
			mVertexBufferCapacity = 0;
			mIndexBufferCapacity = 0;

			// Create vertex declaration
			VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
			decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
			decl->addElement(1, 0, VET_COLOUR, VES_DIFFUSE);


			fillHardwareBuffers();


			Ogre::MaterialPtr Mat = Ogre::MaterialManager::getSingleton().getByName( "ManualColours" );

			static int Index = 0;
			Ogre::String Name = "HudLine"+Ogre::StringConverter::toString( Index++ );
			LineMaterial = Mat->clone( Name );
			this->setMaterial( LineMaterial->getName() );
			UpdateColors( mLineColour ) ;





			/*/
			mRenderOp.indexData = 0;
			mRenderOp.vertexData->vertexCount = 4; 
			mRenderOp.vertexData->vertexStart = 0; 
			mRenderOp.operationType = RenderOperation::OT_LINE_STRIP; 
			mRenderOp.useIndexes = false; 
			mRenderOp.useGlobalInstancingVertexBufferIsAvailable = false;

			VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

			decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
			HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer( decl->getVertexSize(POSITION_BINDING), mRenderOp.vertexData->vertexCount, vBufUsage);
			bind->setBinding(POSITION_BINDING, vbuf);

			decl->addElement(NORMAL_BINDING, 0, VET_FLOAT3, VES_NORMAL);
			vbuf = HardwareBufferManager::getSingleton().createVertexBuffer( decl->getVertexSize(NORMAL_BINDING), mRenderOp.vertexData->vertexCount, vBufUsage);
			bind->setBinding(NORMAL_BINDING, vbuf);

			float *pNorm = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
			*pNorm++ = 0.0f;
			*pNorm++ = 0.0f;
			*pNorm++ = 1.0f;

			*pNorm++ = 0.0f;
			*pNorm++ = 0.0f;
			*pNorm++ = 1.0f;

			*pNorm++ = 0.0f;
			*pNorm++ = 0.0f;
			*pNorm++ = 1.0f;

			*pNorm++ = 0.0f;
			*pNorm++ = 0.0f;
			*pNorm++ = 1.0f;

			vbuf->unlock();

			if (includeTextureCoords)
			{
				decl->addElement(TEXCOORD_BINDING, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);

				HardwareVertexBufferSharedPtr tvbuf = HardwareBufferManager::getSingleton().createVertexBuffer( decl->getVertexSize(TEXCOORD_BINDING), mRenderOp.vertexData->vertexCount, vBufUsage);

				// Bind buffer
				bind->setBinding(TEXCOORD_BINDING, tvbuf);

				// Set up basic tex coordinates
				//setDefaultUVs();
			}

			// Colours
			decl->addElement(3, 0, VET_COLOUR, VES_DIFFUSE);
			HardwareVertexBufferSharedPtr cbuf = HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(3), mRenderOp.vertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
			bind->setBinding(3, cbuf);
 
			// set basic white material
			//this->setMaterial(  "BaseWhiteNoLighting");
			//setCastShadows(true);



			Ogre::MaterialPtr Mat = Ogre::MaterialManager::getSingleton().getByName( "LineColours" );

			static int Index = 0;
			Ogre::String Name = "NxLine"+Ogre::StringConverter::toString( Index++ );
			RectangleMaterial = Mat->clone( Name );

			 this->setMaterial( RectangleMaterial->getName() );

			 UpdateColors(   Ogre::ColourValue(0.2,0.2,0.2,1) ) ;
			 */






 
		}

    public:

		

 

		NxLine2D( NxGuiLine * ptr, bool includeTextureCoordinates, HardwareBuffer::Usage vBufUsage = HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY )
			:  mMain( ptr ) , MovableObject() , mZorder(100), mLineColour( NxColourValue::White )// ,,mMatName("BaseWhite"), mMaterial(MaterialManager::getSingleton().getByName("BaseWhite"), SimpleRenderable()
		{
			_initRectangle2D( vBufUsage);
		}

		~NxLine2D()
		{ 
			OGRE_DELETE mRenderOp.vertexData;
		}

		void setBoundingBox( const AxisAlignedBox& box )
		{
			mBox = box;
		}
		void addPoint(const Vector3 &p)
		{
			mPoints.push_back(p);
			mDirty = true;
		}
		void addPoint(Real x, Real y, Real z)
		{
			mPoints.push_back(Vector3(x,y,z));
			mDirty = true;
		}
		const Vector3& getPoint(unsigned short index) const
		{
			assert(index < mPoints.size() && "Point index is out of bounds!!");
			return mPoints[index];
		}

		unsigned short getNumPoints(void) const
		{
			return (unsigned short)mPoints.size();
		}

		void setPoint(unsigned short index, const Vector3 &value)
		{
			assert(index < mPoints.size() && "Point index is out of bounds!!");
			mPoints[index] = value;
			mDirty = true;
		}

		void fillHardwareBuffers()
		{
			int size = mPoints.size();
			prepareHardwareBuffers(size,0);

			if (!size) 
			{ 
				mBox.setExtents(Ogre::Vector3::ZERO,Ogre::Vector3::ZERO);
				mDirty=false;
				return;
			}

			Ogre::Vector3 vaabMin = Ogre::Vector3(mPoints[0].x,mPoints[0].y,mPoints[0].z);// mPoints[0];
			Ogre::Vector3 vaabMax = Ogre::Vector3(mPoints[0].x,mPoints[0].y,mPoints[0].z);//mPoints[0];

			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

			Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
			{
				for(int i = 0; i < size; i++)
				{
					*prPos++ = mPoints[i].x;
					*prPos++ = mPoints[i].y;
					*prPos++ = mPoints[i].z;

					if(mPoints[i].x < vaabMin.x)
						vaabMin.x = mPoints[i].x;
					if(mPoints[i].y < vaabMin.y)
						vaabMin.y = mPoints[i].y;
					if(mPoints[i].z < vaabMin.z)
						vaabMin.z = mPoints[i].z;

					if(mPoints[i].x > vaabMax.x)
						vaabMax.x = mPoints[i].x;
					if(mPoints[i].y > vaabMax.y)
						vaabMax.y = mPoints[i].y;
					if(mPoints[i].z > vaabMax.z)
						vaabMax.z = mPoints[i].z;
				}
			}

			vbuf->unlock();
			mBox.setExtents(vaabMin, vaabMax);
			mDirty = false;

			UpdateColors( mLineColour );

		}
 

		void SetViewportSize( float ScreenWidth, float ScreenHeight )
		{
			mScreenWidth = ScreenWidth;
			mScreenHeight = ScreenHeight;
			//LogManager::getSingleton().logMessage( " Rectangle set viewport Width :  " + Ogre::StringConverter::toString(mScreenWidth) +"  Height : " + Ogre::StringConverter::toString(mScreenHeight)    );
		
		}

        Real getSquaredViewDepth(const Camera* cam) const
        { 
			return (10000.0f - (Ogre::Real)GetZOrder());
		}

		void SetZOrder( unsigned short Priority )
		{
			mZorder = Priority;
		}

		unsigned short GetZOrder() const
		{
			return mZorder;
		}

        Real getBoundingRadius(void) const 
		{ 
			return 0;
		}

		void UpdateColors( const NxColourValue & Value ) {

			RGBA color;
			Root::getSingleton().convertColourValue(  Ogre::ColourValue( Value.r, Value.g, Value.b, Value.a ) , &color);
			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
			RGBA *pDest = static_cast<RGBA*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
			for (int i = 0; i < (int)mRenderOp.vertexData->vertexCount; ++i)
				*pDest++ = color;
			vbuf->unlock();

			mLineColour = Value;

		}

		NxGuiLine * mMain;
		float mScreenWidth, mScreenHeight;
		Ogre::MaterialPtr LineMaterial;
		NxColourValue mLineColour;
		unsigned short mZorder;
		RenderOperation mRenderOp;
		String mMatName;
		MaterialPtr mMaterial;
		AxisAlignedBox mBox;

		std::vector<Vector3> mPoints;
		bool mDirty;
		/// Maximum capacity of the currently allocated vertex buffer.
		size_t mVertexBufferCapacity;
		/// Maximum capacity of the currently allocated index buffer.
		size_t mIndexBufferCapacity;


    };
 


/////////

NxGuiLine::NxGuiLine( float PosX, float PosY, float Width, float Height, NxGuiManager * Manager )
: NxElement( PosX, PosY, Width, Height, Manager )
{
	mLine = new NxLine2D( this, false ) ;
	mLine->setUseIdentityProjection(true);
	mLine->setUseIdentityView(true);
	mLine->SetViewportSize( Manager->GetViewportWidth(), Manager->GetViewportHeight() );
	mLine->setRenderQueueGroup( RENDER_QUEUE_OVERLAY );
	//setRenderQueueGroupAndPriority(uint8 queueID, ushort priority);

	Manager->GetContentManager()->GetContentManager()->getRootSceneNode()->createChildSceneNode()->attachObject(mLine);

	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mLine->setBoundingBox(aabInf);

 
}

NxGuiLine::~NxGuiLine()
{

}

void NxGuiLine::SetVisibilityFlags( int Flags )
{
	mLine->setVisibilityFlags( Flags );
}

void NxGuiLine::SetRenderQueueGroup( unsigned int Group )
{
	mLine->setRenderQueueGroup( Group );
}

void NxGuiLine::SetZOrder( unsigned short Priority )
{
	mLine->SetZOrder( Priority );
}

unsigned short NxGuiLine::GetZOrder()
{
	return mLine->GetZOrder();
}

void NxGuiLine::OnViewportResized( float ScreenWidth, float ScreenHeight )
{
	mScreenWidth = ScreenWidth; mScreenHeight = ScreenHeight; 
	//mRectangle->SetViewportSize( ScreenWidth, ScreenHeight );
}

void NxGuiLine::Draw()
{
	if( !mNeedsUpate ) return;
 
	mLine->fillHardwareBuffers();

	mNeedsUpate = false;
}

void NxGuiLine::Repaint()
{
	mNeedsUpate = true;
}

void NxGuiLine::SetColour( const NxColourValue & Colour )
{
	mLine->fillHardwareBuffers();
	mLine->UpdateColors( Colour );
}

void NxGuiLine::SetVisible( bool Visible )
{
	mLine->setVisible( Visible );	
}

void NxGuiLine::addPoint(const Vector3 &p)
{
	mLine->addPoint(p);
}

void NxGuiLine::addPoint(Real x, Real y, Real z)
{
	float dx, dy;
	ConvertToPixelCoordinates( x, y, dx, dy );
	mLine->addPoint(dx,dy,z);
}

const Vector3& NxGuiLine::getPoint(unsigned short index) const
{
	return mLine->getPoint( index );
}

unsigned short NxGuiLine::getNumPoints(void) const
{
	return mLine->getNumPoints();
}

void NxGuiLine::setPoint(unsigned short index, const Vector3 &value)
{
	mLine->setPoint( index, value);
}

}