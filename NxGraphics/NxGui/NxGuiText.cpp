
#include "NxGraphicsPreCompiled.h"

#include "../nxdeps/include/OGRE/Overlay/OgreFont.h"
#include "../nxdeps/include/OGRE/Overlay/OgreFontManager.h"
//#include <OgreFont.h>
//#include <OgreFontManager.h>
// 
#define POS_TEX_BINDING    0
#define COLOUR_BINDING     1

namespace Nx {

class NxText2D :  public MovableObject, public Renderable // public SimpleRenderable //
{
public:

	NxText2D( NxText * ptr, const String &caption, const String &fontName, Real charHeight, const NxColourValue &color = NxColourValue::White  )
		: mpFont(NULL), mCaption(caption), mFontName(fontName), mCharHeight(charHeight), mColor(color), mZorder(100),//  mType("MovableText"),
		mTimeUntilNextToggle(0), mSpaceWidth(0), mUpdateColors(true), mOffsetX(0), mOffsetY(0), mNumLines(0),
		mAlignV( /*NxTextVerticalAlignment::*/NxTextVMiddle ), mAlignH( /*NxTextHorizontalAlignment::*/NxTextHMiddle )	
	{

		mMain = ptr;
		mRenderOp.vertexData = NULL;
		this->setFontName(mFontName);
		this->_setupGeometry();
	}

	~NxText2D()
	{
		if (mRenderOp.vertexData){ delete mRenderOp.vertexData; }
		if (!mpMaterial.isNull()) { MaterialManager::getSingletonPtr()->remove(mpMaterial->getName()); }
	}

	void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false)
	{
		visitor->visit(this, 0, false);
	}

	void SetViewportSize( int ScreenWidth, int ScreenHeight )
	{
		mScreenWidth = ScreenWidth;
		mScreenHeight = ScreenHeight;
	}

	void setFontName( const String &fontName ) {
 
		if((Ogre::MaterialManager::getSingletonPtr()->resourceExists(mName + "Material"))) { 
			Ogre::MaterialManager::getSingleton().remove(mName + "Material"); 
		}

		if (mFontName != fontName || mpMaterial.isNull() || !mpFont) {
			mFontName = fontName;
			mpFont = (Font *)FontManager::getSingleton().getByName(mFontName).getPointer();
			if (!mpFont)
				throw Exception(Exception::ERR_ITEM_NOT_FOUND, "Could not find font " + fontName, "MovableText::setFontName");

			mpFont->load();



			float TTFSize = mpFont->getTrueTypeSize( );
			unsigned int TTFResolution = mpFont->getTrueTypeResolution();
			LogManager::getSingleton().logMessage( "NxGuiText: TTF Size : " + Ogre::StringConverter::toString( TTFSize ) );
			LogManager::getSingleton().logMessage( "NxGuiText: TTF Resolution : " + Ogre::StringConverter::toString( TTFResolution ) );

			/*
			if( mpFont->isLoaded() ){
				LogManager::getSingleton().logMessage( "NxGuiText-NxText2D: loaded : " +  fontName );
			}else{  
				LogManager::getSingleton().logMessage( "NxGuiText-NxText2D: could not load : " +  fontName );
			}*/


			if (!mpMaterial.isNull())
			{
				MaterialManager::getSingletonPtr()->remove(mpMaterial->getName());
				mpMaterial.setNull();
			}

			mpMaterial = mpFont->getMaterial()->clone(mName + "Material");
			if (!mpMaterial->isLoaded())
				mpMaterial->load();

			mpMaterial->setDepthCheckEnabled(false);
			mpMaterial->setDepthBias(1.0,1.0);
			mpMaterial->setDepthWriteEnabled(true);
			mpMaterial->setLightingEnabled(false);
		}
		 
	}

	Ogre::TexturePtr GetFontTexture() {
		Ogre::String Name = mpFont->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();
		return Ogre::TextureManager::getSingleton().getByName( Name );
	}

	Ogre::Font * GetFont() {
		return mpFont;
	}

	void SetCaption(const String &caption) {
		if( caption != mCaption) {
			mCaption = caption;
		}
	}

	const std::string & GetCaption() const {
		return mCaption;
	}

	void SetColour( const NxColourValue &color ) {
		if (color != mColor) {
			mColor = color;
			mUpdateColors = true;
		}
	}

	const NxColourValue & GetColour() {
		return mColor;
	}

	void SetCharacterHeight( float height ) {
		if (height != mCharHeight) {
			mCharHeight = height;
		}
	}

	float GetCharacterHeight() {
		return mCharHeight;
	}

	void SetSpaceWidth(Real width) {
		if ( width != mSpaceWidth) {
			mSpaceWidth = width;
		}
	}

	float GetSpaceWidth() {
		return mSpaceWidth;
	}

	float GetCharacterAspectRatio( unsigned char Char ) {
		Ogre::Font::GlyphInfo gi = mpFont->getGlyphInfo(Char);
		return gi.aspectRatio;
	}

	float GetCharacterWidth( unsigned char Char ) {
		Ogre::Font::GlyphInfo gi = mpFont->getGlyphInfo(Char);
		float UVwidth = gi.uvRect.right - gi.uvRect.left;
		return UVwidth; 
	}

	float GetCharacterHeight( unsigned char Char ) {
		Ogre::Font::GlyphInfo gi = mpFont->getGlyphInfo(Char);
		float UVheight = gi.uvRect.bottom - gi.uvRect.top;
		return UVheight; 
	}

	unsigned int GetNumCharSpaces() {
		int NumChars = 0;
		String::iterator i, iend;
		i = mCaption.begin();
		iend = mCaption.end();
		for (i = mCaption.begin(); i != iend; ++i) {
			if( (*i) == ' ' ) NumChars++;
		}

		return NumChars;
	}

	int GetNumLines() {
		return mNumLines;
	}

	const float GetTextPixelHeight()
	{
		return GetCharacterHeight() * mScreenHeight; // * GetNumLines() 
	}

	const float GetTextPixelLenght( std::string text )
	{
		Ogre::String DstText = text;
		String::iterator i, iend;
		i = DstText.begin();
		iend = DstText.end();
		float len = 0.0f;
		for (String::iterator j = i; j != iend && *j != '\n'; j++) {
			if (*j == ' ')
				len += mSpaceWidth;
			else
				len += mpFont->getGlyphAspectRatio((unsigned char)*j) * mCharHeight * 2.0 * (float)( (float)mScreenHeight / (float)mScreenWidth );	
		}

		float textPixelsLength = mScreenWidth * len;
		textPixelsLength = textPixelsLength / 2;
		return textPixelsLength;
	}

	const float GetTextLengthWithoutSpace() {

		String::iterator i, iend;
		i = mCaption.begin();
		iend = mCaption.end();
		// get character length
		float len = 0.0f;
		for (String::iterator j = i; j != iend && *j != '\n'; j++)
		{
			if (*j != ' ')
				len += mpFont->getGlyphAspectRatio((unsigned char)*j) * mCharHeight * 2.0 * (float)( (float)mScreenHeight / (float)mScreenWidth );	
		}

		return len;
	}

	NxTextVerticalAlignment GetVerticalAlignment() {
		return mAlignV;
	}

	NxTextHorizontalAlignment GetHorizontalAlignment() {
		return mAlignH;
	}

	void SetVerticalAlignment( NxTextVerticalAlignment VAlign )
	{
		 float HeightPixelLen = GetTextPixelHeight();
		 float HeightDiffPixels = mMain->getHeight() - HeightPixelLen;
		 if( VAlign == NxTextVTop ) mOffsetY = 0;
		 else if( VAlign == NxTextVMiddle ) mOffsetY = HeightDiffPixels / 2;
		 else if( VAlign == NxTextVBottom ) mOffsetY = HeightDiffPixels;
		 mAlignV = VAlign;
	}

	void SetHorizontalAlignment( NxTextHorizontalAlignment HAlign )
	{
		 float WidthPixelLen = GetTextPixelLenght( GetCaption() );
		 float WidthDiffPixels = mMain->getWidth() - WidthPixelLen;
		 if( HAlign == NxTextHLeft ) mOffsetX = 0;
		 else if( HAlign == NxTextHMiddle) mOffsetX = WidthDiffPixels / 2;
		 else if( HAlign == NxTextHRight ) mOffsetX = WidthDiffPixels;
		 mAlignH = HAlign;
	}

	void getWorldTransforms(Ogre::Matrix4 *xform) const {

		//xform[0] = mParentNode->_getFullTransform();

		float X =  ( (mMain->getPositionDerivedX() + mOffsetX) / (float)mScreenWidth ) * 2.0f ;
		float Y =  ( (mMain->getPositionDerivedY() + mOffsetY) / (float)-mScreenHeight) * 2.0f ;


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



	const   String          &getFontName() const {return mFontName;}
	const   String          &getCaption() const {return mCaption;}
	const   NxColourValue     &getColor() const {return mColor;}
	Real    getCharacterHeight() const {return mCharHeight;}
	Real    getSpaceWidth() const {return mSpaceWidth;}
	AxisAlignedBox            GetAABB(void) { return mAABB; }


public:

	void    _setupGeometry()
	{

		//LogManager::getSingleton().logMessage("updating Text Geometry.... !!");

		assert(mpFont);
		assert(!mpMaterial.isNull());

		unsigned int vertexCount = static_cast<unsigned int>(mCaption.size() * 6);

		if (mRenderOp.vertexData)
		{
			// Removed this test as it causes problems when replacing a caption
			// of the same size: replacing "Hello" with "hello"
			// as well as when changing the text alignment
			//if (mRenderOp.vertexData->vertexCount != vertexCount)
			{
				delete mRenderOp.vertexData;
				mRenderOp.vertexData = NULL;
				mUpdateColors = true;
			}
		}

		if (!mRenderOp.vertexData)
			mRenderOp.vertexData = new VertexData();

		mRenderOp.indexData = 0;
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.vertexData->vertexCount = vertexCount;
		mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST; 
		mRenderOp.useIndexes = false; 
		mRenderOp.useGlobalInstancingVertexBufferIsAvailable = false;



		VertexDeclaration  *decl = mRenderOp.vertexData->vertexDeclaration;
		VertexBufferBinding   *bind = mRenderOp.vertexData->vertexBufferBinding;
		size_t offset = 0;

		// create/bind positions/tex.ccord. buffer
		if (!decl->findElementBySemantic(VES_POSITION))
			decl->addElement(POS_TEX_BINDING, offset, VET_FLOAT3, VES_POSITION);

		offset += VertexElement::getTypeSize(VET_FLOAT3);

		if (!decl->findElementBySemantic(VES_TEXTURE_COORDINATES))
			decl->addElement(POS_TEX_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);

		HardwareVertexBufferSharedPtr ptbuf = HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(POS_TEX_BINDING),
		mRenderOp.vertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(POS_TEX_BINDING, ptbuf);

		// Colours - store these in a separate buffer because they change less often
		if (!decl->findElementBySemantic(VES_DIFFUSE))
			decl->addElement(COLOUR_BINDING, 0, VET_COLOUR, VES_DIFFUSE);

		HardwareVertexBufferSharedPtr cbuf = HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(COLOUR_BINDING),
		mRenderOp.vertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(COLOUR_BINDING, cbuf);

		size_t charlen = mCaption.size();
		float *pPCBuff = static_cast<float*>(ptbuf->lock(HardwareBuffer::HBL_DISCARD));

		//////

 
		float largestWidth = 0;
		float left = -1; 
		float top  = 1; 

		Real spaceWidth = mSpaceWidth;
		// Derive space width from a capital A
		if (spaceWidth == 0)
			spaceWidth = mpFont->getGlyphAspectRatio('A') * mCharHeight * 2.0  ; // aDDED

		// for calculation of AABB
		Ogre::Vector3 min, max, currPos;
		Ogre::Real maxSquaredRadius;
		bool first = true;

		// Use iterator
		String::iterator i, iend;
		iend = mCaption.end();
		bool newLine = true;
		Real len = 0.0f;

		Real verticalOffset = 0;
		for (i = mCaption.begin(); i != iend; ++i) {
			if (*i == '\n')
				top += verticalOffset * 2.0;
		}


		/////////// Character loop /////////////

		mNumLines = 0;

		for (i = mCaption.begin(); i != iend; ++i) {




			// wrap text on width
			float textPointsLength = left + 1 + mpFont->getGlyphAspectRatio((unsigned char)* i + 1) * mCharHeight * 2.0  * (float)( (float)mScreenHeight / (float)mScreenWidth ); 
			float textPixelsLength = mScreenWidth * textPointsLength;
			textPixelsLength = textPixelsLength / 2;

			if( textPixelsLength > mMain->getWidth() ) {
		
				left = -1;
				top -= mCharHeight * 2.0;
				newLine = true;
				mNumLines++;
			}

			//LogManager::getSingleton().logMessage(  Ogre::StringConverter::toString( textPixelsLength ) );




			if (newLine){
				len = 0.0f;
				for (String::iterator j = i; j != iend && *j != '\n'; j++){
					if (*j == ' ')
						len += spaceWidth;
					else 
						len += mpFont->getGlyphAspectRatio((unsigned char)*j) * mCharHeight * 2.0  * (float)( (float)mScreenHeight / (float)mScreenWidth ); 
				}
				newLine = false;
			}

			if (*i == '\n'){  
				left = 0 * 2.0 - 1.0;
				top -= mCharHeight * 2.0;
				newLine = true;
				continue;
			}

			if (*i == ' '){
				// Just leave a gap, no tris
				left += spaceWidth;
				// Also reduce tri count
				mRenderOp.vertexData->vertexCount -= 6;
				continue;
			}

			Real horiz_height = mpFont->getGlyphAspectRatio((unsigned char)*i)  * (float)( (float)mScreenHeight / (float)mScreenWidth ); 
			Real u1, u2, v1, v2; 
			Ogre::Font::UVRect utmp;
			utmp = mpFont->getGlyphTexCoords((unsigned char)*i);
			u1 = utmp.left;
			u2 = utmp.right;
			v1 = utmp.top;
			v2 = utmp.bottom;

			float z = 0.0f;

			// each vert is (x, y, z, u, v)
			//-------------------------------------------------------------------------------------
			// First tri
			//
			// Upper left
			*pPCBuff++ = left;
			*pPCBuff++ = top;
			*pPCBuff++ = z;
			*pPCBuff++ = u1;
			*pPCBuff++ = v1;

			top -= mCharHeight * 2.0;

			// Bottom left
			*pPCBuff++ = left;
			*pPCBuff++ = top;
			*pPCBuff++ = z;
			*pPCBuff++ = u1;
			*pPCBuff++ = v2;

			top += mCharHeight * 2.0;
			left += horiz_height * mCharHeight * 2.0;

			// Top right
			*pPCBuff++ = left;
			*pPCBuff++ = top;
			*pPCBuff++ = z;
			*pPCBuff++ = u2;
			*pPCBuff++ = v1;
			//-------------------------------------------------------------------------------------
			// Second tri
			//
			// Top right (again)
			*pPCBuff++ = left;
			*pPCBuff++ = top;
			*pPCBuff++ = z;
			*pPCBuff++ = u2;
			*pPCBuff++ = v1;

			top -= mCharHeight * 2.0;
			left -= horiz_height  * mCharHeight * 2.0;

			// Bottom left (again)
			*pPCBuff++ = left;
			*pPCBuff++ = top;
			*pPCBuff++ = z;
			*pPCBuff++ = u1;
			*pPCBuff++ = v2;

			left += horiz_height  * mCharHeight * 2.0;

			// Bottom right
			*pPCBuff++ = left;
			*pPCBuff++ = top;
			*pPCBuff++ = z;
			*pPCBuff++ = u2;
			*pPCBuff++ = v2;
			//-------------------------------------------------------------------------------------

			currPos = Ogre::Vector3(left, top, z);
			min.makeFloor(currPos);
			max.makeCeil(currPos);
			maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());


			// Go back up with top
			top += mCharHeight * 2.0;

			float currentWidth = (left + 1)/2 - 0;
			if (currentWidth > largestWidth)
				largestWidth = currentWidth;
		}

		// Unlock vertex buffer
		ptbuf->unlock();

		//mAABB.setInfinite(); // ??

		//mAABB = Ogre::AxisAlignedBox(min, max);
	 


		mRadius = Ogre::Math::Sqrt(maxSquaredRadius);

		if (mUpdateColors) { this->_updateColors(); }

		SetVerticalAlignment( mAlignV ); 
		SetHorizontalAlignment( mAlignH );

	}

	void _updateColors() {
		assert(mpFont);
		assert(!mpMaterial.isNull());

		// Convert to system-specific
		RGBA color;
		Root::getSingleton().convertColourValue( Ogre::ColourValue( mColor.r, mColor.g, mColor.b, mColor.a )  , &color);
		HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);
		RGBA *pDest = static_cast<RGBA*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
		for (int i = 0; i < (int)mRenderOp.vertexData->vertexCount; ++i)
			*pDest++ = color;
		vbuf->unlock();
		mUpdateColors = false;
	}



	Real getBoundingRadius(void) const {
		return mRadius;
	} 

	Real getSquaredViewDepth( const Camera * cam ) const {
		return (10000.0f - (Ogre::Real)GetZOrder());
	};
 
	 const AxisAlignedBox &getBoundingBox(void) const {
		 return mAABB;
	 }

	 void setBoundingBox( const AxisAlignedBox& box )
	 {
		 mAABB = box;
	 } 
 
	 const String &getMovableType(void) const {
		 static Ogre::String movType = "NxGuiElement";
		 return movType;
	 }

	void _notifyCurrentCamera(Camera *cam){}

	void SetZOrder( unsigned short Priority )
	{
		mZorder = Priority;
	}

	unsigned short GetZOrder() const
	{
		return mZorder;
	}

	void _updateRenderQueue(RenderQueue* queue) {

	

		if (this->isVisible()) {
			 queue->addRenderable(this, mRenderQueueID, mZorder );//OGRE_RENDERABLE_DEFAULT_PRIORITY); //  RENDER_QUEUE_OVERLAY ); //

			
		}
	}

	// from renderable
	void getRenderOperation(RenderOperation &op) {
 
		if (this->isVisible()) {
			op = mRenderOp;
		}
	}

	const   MaterialPtr       &getMaterial(void) const { assert(!mpMaterial.isNull());return mpMaterial;}
	const   LightList         &getLights(void) const {return mLList;}

protected:
	String mFontName;
	//String mType;
	//String mName;
	String mCaption;
	NxTextVerticalAlignment mAlignV;
	NxTextHorizontalAlignment mAlignH;
	NxColourValue mColor;
	RenderOperation mRenderOp;
	AxisAlignedBox mAABB;
	LightList mLList;
	Real mCharHeight;
	Real mSpaceWidth;
	//bool mNeedUpdate;
	bool mUpdateColors;
	Real mTimeUntilNextToggle;
	Real mRadius;
	Font * mpFont;
	MaterialPtr mpMaterial;
	MaterialPtr mpBackgroundMaterial;

	NxText * mMain;
	int mScreenWidth, mScreenHeight;
	float mOffsetX, mOffsetY; 
	int mNumLines;

	unsigned short mZorder;

	//Ogre::Vector3 mPosition;
};
 
NxText::NxText( const std::string & CaptionText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager, const std::string & FontName) 
: NxElement( PosX, PosY, Width, Height, Manager ), mTextWrap(false)
{
 
	mText = CaptionText;
	//float mDefaultCharHeight =  32.0f / (float)Manager->GetViewportHeight();

	float mDefaultCharHeight  = Manager->getDefaultFontSize();

	LogManager::getSingleton().logMessage( "NxGuiText: Creating with font name : " +  FontName + " char default height : " + Ogre::StringConverter::toString( mDefaultCharHeight )   );

	mTextStruct = new NxText2D( this, CaptionText, FontName, mDefaultCharHeight );
	mTextStruct->setUseIdentityProjection(true);
	mTextStruct->setUseIdentityView(true);
	mTextStruct->setVisible( true );
	mTextStruct->setRenderQueueGroup( RENDER_QUEUE_OVERLAY ); 

	//mTextStruct->SetColour( ColourValue::Blue );

	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mTextStruct->setBoundingBox(aabInf);


	mTextStruct->SetViewportSize( Manager->GetViewportWidth(), Manager->GetViewportHeight() );
	
	// Attach to scene
	static int id = 0;
	Ogre::SceneNode * Noder = Manager->GetContentManager()->GetContentManager()->getRootSceneNode()->createChildSceneNode("NxTextSceneNode"+Ogre::StringConverter::toString(id++));
	//Noder->showBoundingBox(true);
	Noder->attachObject(mTextStruct );
 
	float TTFSize = mTextStruct->GetFont()->getTrueTypeSize( );
	unsigned int TTFResolution = mTextStruct->GetFont()->getTrueTypeResolution();
	//LogManager::getSingleton().logMessage( "NxGuiText: TTF Size : " + Ogre::StringConverter::toString( TTFSize ) );
	//LogManager::getSingleton().logMessage( "NxGuiText: TTF Resolution : " + Ogre::StringConverter::toString( TTFResolution ) );
 
 

	mTextStruct->SetCharacterHeight( mDefaultCharHeight );
	mTextStruct->SetSpaceWidth( mDefaultCharHeight / 2 );


 
 

	//SetHorizontalAlignment( NxTextHorizontalAlignment::NxTextHMiddle );
	//SetVerticalAlignment( NxTextVerticalAlignment::NxTextVMiddle);

	//SetTextWidthWrap( true );

}

NxText::~NxText()
{

}

void NxText::SetVisibilityFlags( int Flags )
{
	mTextStruct->setVisibilityFlags( Flags );
}

void NxText::SetRenderQueueGroup( unsigned int Group )
{
	mTextStruct->setRenderQueueGroup( Group );
}

void NxText::SetZOrder( unsigned short Priority )
{
	mTextStruct->SetZOrder( Priority );
}

unsigned short NxText::GetZOrder()
{
	return mTextStruct->GetZOrder();
}


void NxText::Draw( )
{
	if( !mNeedsUpate ) return;
	mTextStruct->_setupGeometry();
	if( mTextWrap ) SetTextWidthWrap( true );
	mNeedsUpate = false;
}

void NxText::SetTextPixelHeight( int PixelHeight )
{
	mTextStruct->SetCharacterHeight( (float)PixelHeight / (float)mManager->GetViewportHeight()  );
	mNeedsUpate = true;
}

void NxText::SetTextPixelSpacing( int PixelSpace )
{
	mTextStruct->SetSpaceWidth( ((float)PixelSpace / (float)mManager->GetViewportWidth()) * 2.0f  );
	mNeedsUpate = true;
}

bool NxText::SetTextWidthWrap( bool Wrap )
{
	float textPointsLength = mTextStruct->GetTextLengthWithoutSpace();
	float textPixelsLength = mManager->GetViewportWidth() * textPointsLength;
	textPixelsLength = textPixelsLength / 2;
	float spacepixels = mWidth - textPixelsLength;
	float spacepoints = spacepixels / mManager->GetViewportWidth();
	mTextStruct->SetSpaceWidth( ( spacepoints * 2 ) / mTextStruct->GetNumCharSpaces() );
	mTextWrap = true;
	mNeedsUpate = true;
	//LogManager::getSingleton().logMessage(  "text pixel width size : " + Ogre::StringConverter::toString( textPixelsLength ) );

	return true;
}

bool NxText::IsTextWrapping()
{
	return mTextWrap;
}

void NxText::Repaint()
{
	mNeedsUpate = true;
}

void NxText::OnViewportResized( float ScreenWidth, float ScreenHeight )
{
	mScreenWidth = ScreenWidth; mScreenHeight = ScreenHeight; 
}

void NxText::SetVisible( bool Visible )
{
	mTextStruct->setVisible( Visible );
}

void NxText::SetCaption( const std::string & CaptionText )
{
	mText = CaptionText;
	mTextStruct->SetCaption( CaptionText );
	mNeedsUpate = true;
}

const std::string & NxText::GetCaption() const
{
	return mText;
}
 
void NxText::SetColour( const NxColourValue & Colour )
{
	mTextStruct->SetColour( Colour );
	mNeedsUpate = true;
}

void NxText::SetVerticalAlignment( NxTextVerticalAlignment VAlign )
{
	mTextStruct->SetVerticalAlignment( VAlign );
}

void NxText::SetHorizontalAlignment( NxTextHorizontalAlignment HAlign )
{
	mTextStruct->SetHorizontalAlignment( HAlign );
}

}