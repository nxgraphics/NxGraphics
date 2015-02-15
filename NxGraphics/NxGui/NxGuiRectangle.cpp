#include "NxGraphicsPreCompiled.h"

#define POSITION_BINDING 0
#define NORMAL_BINDING 1
#define TEXCOORD_BINDING 2
#define COLOR_BINDING 3

namespace Nx {

    class NxGuiRectangle2D : public MovableObject, public Renderable
    {
    protected:

		Ogre::TextureUnitState * mState;
		String mMatName;
		MaterialPtr mMaterial;
		AxisAlignedBox mBox;
		bool mHasTextureCoordinates;
		Ogre::HardwareBuffer::Usage mBufferUsage;
		Nx::Vector2 mTextureCoordinates[4];

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

		const MaterialPtr& getMaterial() const {
			return mMaterial;
		}

		const String& getMovableType() const {
			static String movType = "NxGuiElement";
			return movType;
		}
		//-----------------------------------------------------------------------
		const LightList& getLights() const {
			// Use movable query lights
			return queryLights();
		}

		const AxisAlignedBox& getBoundingBox(void) const {
			 return mBox;
		}

		void _updateRenderQueue( RenderQueue* queue ) {
			if( this->isVisible() ) {
				queue->addRenderable( this, mRenderQueueID, mZorder ); 
			}
		}

		void visitRenderables(Renderable::Visitor* visitor,  bool debugRenderables) {
			visitor->visit(this, 0, false);  
		}

		void getRenderOperation(RenderOperation& op)
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
			// *xform = Matrix4::IDENTITY;	
//
		//	 return;
			/*
			float X =  ( mMain->getX() / mScreenWidth ) * 2.0f ;
			float Y =  ( mMain->getY() / -mScreenHeight ) * 2.0f;*/

			float X =  ( mMain->getPositionDerivedX() / mScreenWidth ) * 2.0f ;
			float Y =  ( mMain->getPositionDerivedY() / -mScreenHeight ) * 2.0f;

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

		void _initRectangle2D( bool includeTextureCoords )
		{
			// use identity projection and view matrices
			// mUseIdentityProjection = true;
			// mUseIdentityView = true;

			//Ogre::AxisAlignedBox aabb;
			//aabb.setInfinite();
			//setBoundingBox(aabb);

			//setUseIdentityProjection(true);
			//setUseIdentityView(true);

			//AxisAlignedBox aabBounds;
			//aabBounds.setInfinite();	
			//this->setBoundingBox( aabBounds );//  _setBounds( aabBounds );

			

			mRenderOp.vertexData = OGRE_NEW VertexData();

			mRenderOp.indexData = 0;
			mRenderOp.vertexData->vertexCount = 4; 
			mRenderOp.vertexData->vertexStart = 0; 
			// No indexes & issue as a strip 
			mRenderOp.operationType = RenderOperation::OT_TRIANGLE_STRIP; 
			mRenderOp.useIndexes = false; 
			mRenderOp.useGlobalInstancingVertexBufferIsAvailable = false;

			VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

			decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
			HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer( decl->getVertexSize(POSITION_BINDING), mRenderOp.vertexData->vertexCount, mBufferUsage);
			bind->setBinding(POSITION_BINDING, vbuf);

			decl->addElement(NORMAL_BINDING, 0, VET_FLOAT3, VES_NORMAL);
			vbuf = HardwareBufferManager::getSingleton().createVertexBuffer( decl->getVertexSize(NORMAL_BINDING), mRenderOp.vertexData->vertexCount, mBufferUsage);
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



			Ogre::MaterialPtr Mat = Ogre::MaterialManager::getSingleton().getByName( "ManualColours" );
			static int Index = 0;
			Ogre::String Name = "HudRectangle"+Ogre::StringConverter::toString( Index++ );
			RectangleMaterial = Mat->clone( Name );
			this->setMaterial( RectangleMaterial->getName() );
 
			if( includeTextureCoords ) {
				buildTextureCoordinates();
				//setBackgroundTexture("NxLogo.jpg");
			} else { // Colours
				buildColorCoordinates();
				setBackgroundColor( NxColourValue(1.0,0.0,0.0,1)  );
			}


		}

		void buildTextureCoordinates() {
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration; 
			decl->addElement( TEXCOORD_BINDING, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES );
			HardwareVertexBufferSharedPtr tvbuf = HardwareBufferManager::getSingleton().createVertexBuffer( decl->getVertexSize(TEXCOORD_BINDING), mRenderOp.vertexData->vertexCount, mBufferUsage );
			mRenderOp.vertexData->vertexBufferBinding->setBinding( TEXCOORD_BINDING, tvbuf );
			setDefaultUVs();
		}

		void buildColorCoordinates() {
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration; 
			decl->addElement( COLOR_BINDING, 0, VET_COLOUR, VES_DIFFUSE );
			HardwareVertexBufferSharedPtr cbuf = HardwareBufferManager::getSingleton().createVertexBuffer( /*decl->getVertexSize(3)*/ VertexElement::getTypeSize(VET_COLOUR), mRenderOp.vertexData->vertexCount, mBufferUsage );
			mRenderOp.vertexData->vertexBufferBinding->setBinding( COLOR_BINDING, cbuf );
		}

		
		void rebuildCoordinates( bool textureCoordinates ) {

			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration; 
 
			if( !textureCoordinates ) { // destroy previous texture coordinates
				decl->removeElement( VES_TEXTURE_COORDINATES ); 
				if( mRenderOp.vertexData->vertexBufferBinding->isBufferBound( TEXCOORD_BINDING ) )
					mRenderOp.vertexData->vertexBufferBinding->unsetBinding( TEXCOORD_BINDING );
				buildColorCoordinates();
				mHasTextureCoordinates = false;
				//setBackgroundColor( mBackgroundColour, true );
			} else { // destroy color
				decl->removeElement( VES_DIFFUSE ); 
				if( mRenderOp.vertexData->vertexBufferBinding->isBufferBound( COLOR_BINDING ) )
					mRenderOp.vertexData->vertexBufferBinding->unsetBinding( COLOR_BINDING );
				buildTextureCoordinates();
				mHasTextureCoordinates = true;
			}
 
		}

    public:
 
		NxGuiRectangle2D( NxRectangle * ptr, bool includeTextureCoordinates, HardwareBuffer::Usage vBufUsage ) : MovableObject(), mMain( ptr ), mState( NULL ), mZorder(100) {
			mMatName = "BaseWhite";
			mMaterial = MaterialManager::getSingleton().getByName( mMatName );
			mHasTextureCoordinates = includeTextureCoordinates;
			mBufferUsage = vBufUsage;
			_initRectangle2D( includeTextureCoordinates );
		}

		~NxGuiRectangle2D() { 
			OGRE_DELETE mRenderOp.vertexData;
		}

		void setBoundingBox( const AxisAlignedBox& box ) {
			mBox = box;
		} 

        /** Sets the corners of the rectangle, in relative coordinates.
        @param
        left Left position in screen relative coordinates, -1 = left edge, 1.0 = right edge
        @param top Top position in screen relative coordinates, 1 = top edge, -1 = bottom edge
        @param right Right position in screen relative coordinates
        @param bottom Bottom position in screen relative coordinates
		@param updateAABB Tells if you want to recalculate the AABB according to 
		the new corners. If false, the axis aligned bounding box will remain identical.
        */
		void setCorners( Real left, Real top, Real right, Real bottom, bool updateAABB = true ) {
 
			HardwareVertexBufferSharedPtr vbuf =  mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);

			float* pFloat = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

			float Z = -1;

			*pFloat++ = left;
			*pFloat++ = top;
			*pFloat++ = Z;

			*pFloat++ = left;
			*pFloat++ = bottom;
			*pFloat++ = Z;

			*pFloat++ = right;
			*pFloat++ = top;
			*pFloat++ = Z;

			*pFloat++ = right;
			*pFloat++ = bottom;
			*pFloat++ = Z;

			vbuf->unlock();

			if(updateAABB) // should not box be updated. is set to infinite
			{
				/*
				mBox.setExtents(
					std::min(left, right), std::min(top, bottom), 0,
					std::max(left, right), std::max(top, bottom), 0);
					*/
 	
			}		
		
		}

		//Sets the normals of the rectangle
		void setNormals(const Ogre::Vector3 &topLeft, const Ogre::Vector3 &bottomLeft, const Ogre::Vector3 &topRight, const Ogre::Vector3 &bottomRight)
		{
			HardwareVertexBufferSharedPtr vbuf =  mRenderOp.vertexData->vertexBufferBinding->getBuffer(NORMAL_BINDING);

			float* pFloat = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

			*pFloat++ = topLeft.x;
			*pFloat++ = topLeft.y;
			*pFloat++ = topLeft.z;

			*pFloat++ = bottomLeft.x;
			*pFloat++ = bottomLeft.y;
			*pFloat++ = bottomLeft.z;

			*pFloat++ = topRight.x;
			*pFloat++ = topRight.y;
			*pFloat++ = topRight.z;

			*pFloat++ = bottomRight.x;
			*pFloat++ = bottomRight.y;
			*pFloat++ = bottomRight.z;

			vbuf->unlock();	
		
		
		}


		void setUVs( const  Nx::Vector2 &topLeft, const  Nx::Vector2 &bottomLeft, const  Nx::Vector2 &topRight, const  Nx::Vector2 &bottomRight) {

			if( mRenderOp.vertexData->vertexDeclaration->getElementCount() <= TEXCOORD_BINDING ) return; //Vertex data wasn't built with UV buffer

			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
			float* pFloat = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

			*pFloat++ = topLeft.x;
			*pFloat++ = topLeft.y;

			*pFloat++ = bottomLeft.x;
			*pFloat++ = bottomLeft.y;

			*pFloat++ = topRight.x;
			*pFloat++ = topRight.y;

			*pFloat++ = bottomRight.x;
			*pFloat++ = bottomRight.y;

			vbuf->unlock();	

			mTextureCoordinates[0] = topLeft;
			mTextureCoordinates[1] = bottomLeft;
			mTextureCoordinates[2] = topRight;
			mTextureCoordinates[3] = bottomRight;
		
		}

	
		void getUVs( Vector2 & topLeft, Vector2 & bottomLeft, Vector2 & topRight, Vector2 & bottomRight ) {

			topLeft = mTextureCoordinates[0];
			bottomLeft = mTextureCoordinates[1];
			topRight = mTextureCoordinates[2];
			bottomRight = mTextureCoordinates[3];	 
		}

		void setDefaultUVs() {
			setUVs( Vector2::ZERO, Vector2::UNIT_Y, Vector2::UNIT_X, Vector2::UNIT_SCALE );
		}

		bool hasTextureCoordinates() {  
			return mHasTextureCoordinates;
		}
		

		void SetViewportSize( float ScreenWidth, float ScreenHeight ) {
			mScreenWidth = ScreenWidth;
			mScreenHeight = ScreenHeight; 
		}

        Real getSquaredViewDepth(const Camera* cam) const {
			return ( 10000.0f - ( Ogre::Real ) GetZOrder() );
		}

		void SetZOrder( unsigned short Priority ) {
			mZorder = Priority;
		}

		unsigned short GetZOrder() const {
			return mZorder;
		}

        Real getBoundingRadius(void) const { 
			return 0;
		}

		const NxColourValue & GetColour() const {
			return mBackgroundColour;
		}

		void setBackgroundColor( const NxColourValue & Value, bool force = false ) {

			if( mBackgroundColour == Value && !force ) return;

			if( mState ) {
				RectangleMaterial->getTechnique(0)->getPass(0)->removeAllTextureUnitStates(); 	
				mState = 0;
			}

			rebuildCoordinates( false );

			RGBA color;
			Root::getSingleton().convertColourValue( Ogre::ColourValue( Value.r, Value.g, Value.b, Value.a ), &color);
			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer( COLOR_BINDING );
			RGBA *pDest = static_cast<RGBA*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
			for (int i = 0; i < (int)mRenderOp.vertexData->vertexCount; ++i)
				*pDest++ = color;
			vbuf->unlock();
 
			RectangleMaterial->getTechnique(0)->getPass(0)->setVertexColourTracking( TVC_DIFFUSE | TVC_AMBIENT | TVC_SPECULAR );
	 
			mBackgroundColour = Value;
	 
		}

		void setBackgroundTexture( const std::string & TextureName ) {
 
			if( Ogre::TextureManager::getSingleton().resourceExists( TextureName ) ) {
 
				rebuildCoordinates( true );
 
				if( !mState )
				 	mState = RectangleMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();

				mState->getParent()->setLightingEnabled(false);

				mState->setTexture( Ogre::TextureManager::getSingleton().getByName( TextureName ) );

				RectangleMaterial->getTechnique(0)->getPass(0)->setVertexColourTracking( TVC_NONE );

			}
			else {
				LogManager::getSingleton().logMessage("NxGui : Cant load Texture Name : " + TextureName );
			}
		}

		const std::string & getBackgroundTextureName() const {
			if( mState )
				return mState->getTextureName();
			else
				return "";

		}


		NxRectangle * mMain;
		float mScreenWidth, mScreenHeight;
		Ogre::MaterialPtr RectangleMaterial;

		NxColourValue mBackgroundColour;

		unsigned short mZorder;

		RenderOperation mRenderOp;

    };
 


/////////

NxRectangle::NxRectangle( float PosX, float PosY, float Width, float Height, NxGuiManager * Manager )
: NxElement( PosX, PosY, Width, Height, Manager ) {

	mRectangle = new NxGuiRectangle2D( this, false, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY ) ; // 
	mRectangle->setUseIdentityProjection(true);
	mRectangle->setUseIdentityView(true);
	mRectangle->SetViewportSize( Manager->GetViewportWidth(), Manager->GetViewportHeight() );
 
	float dx, dy;
	ConvertToPixelCoordinates( 0, 0, dx,dy  );
	float dw, dh;
	ConvertToPixelCoordinates( 0 + Width, 0 + Height, dw,dh  );
 
	mRectangle->setCorners( dx, dy, dw, dh, true );
	mRectangle->setRenderQueueGroup( RENDER_QUEUE_OVERLAY );

	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);
 
	static int id = 0;   //OGRE_NEW SceneNode(NULL); //
	mNode  = Manager->GetContentManager()->GetContentManager()->getRootSceneNode()->createChildSceneNode("NxRectangleSceneNode"+Ogre::StringConverter::toString( id++ )) ;//->attachObject(mRectangle);
	mNode->attachObject(mRectangle);
 
}

NxRectangle::~NxRectangle() {
	mNode->getParent()->removeChild(mNode);
	delete mRectangle;
	mRectangle = 0;
}

void NxRectangle::SetVisibilityFlags( int Flags ) {
	mRectangle->setVisibilityFlags( Flags );
}

void NxRectangle::SetRenderQueueGroup( unsigned int Group ) {
	mRectangle->setRenderQueueGroup( Group );
}

void NxRectangle::SetZOrder( unsigned short Priority ) {
	mRectangle->SetZOrder( Priority );
}

unsigned short NxRectangle::GetZOrder() {
	return mRectangle->GetZOrder();
}

void NxRectangle::OnViewportResized( float ScreenWidth, float ScreenHeight )
{
	mScreenWidth = ScreenWidth; mScreenHeight = ScreenHeight; 
	//mRectangle->SetViewportSize( ScreenWidth, ScreenHeight );
}

void NxRectangle::Draw()
{

	//mNode->_update(true, false);

	if( !mNeedsUpate ) return;

	float dx, dy;
	ConvertToPixelCoordinates( 0, 0, dx, dy );

	float dw, dh;
	ConvertToPixelCoordinates( 0 + mWidth, 0 + mHeight , dw, dh );

	mRectangle->setCorners( dx, dy, dw, dh, false ); // because is set to infinite

	mNeedsUpate = false;
}

void NxRectangle::Repaint()
{
	mNeedsUpate = true;
}

void NxRectangle::SetColour( const NxColourValue & Colour ) {
	mRectangle->setBackgroundColor( Colour );
}

const NxColourValue & NxRectangle::GetColour() const {
	return mRectangle->GetColour();
}

void NxRectangle::SetTexture( const std::string & TextureName ) {
	mRectangle->setBackgroundTexture( TextureName );	
}

const std::string & NxRectangle::getBackgroundTextureName() const {
	return mRectangle->getBackgroundTextureName();	
}

void NxRectangle::setTextureCoordinates( const  Nx::Vector2 &topLeft, const  Nx::Vector2 &bottomLeft, const  Nx::Vector2 &topRight, const  Nx::Vector2 &bottomRight) {
	mRectangle->setUVs( topLeft , bottomLeft, topRight, bottomRight );	
}

void NxRectangle::getTextureCoordinates( Vector2 & topLeft, Vector2 & bottomLeft, Vector2 & topRight, Vector2 & bottomRight ) {
	mRectangle->getUVs( topLeft , bottomLeft, topRight, bottomRight );	
}

bool NxRectangle::hasTextureCoordinates() {
	return mRectangle->hasTextureCoordinates();
}

void NxRectangle::SetVisible( bool Visible ) {
	mRectangle->setVisible( Visible );	
}
 


}