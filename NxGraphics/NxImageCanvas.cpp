#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

#ifndef SK_IGNORE_STDINT_DOT_H
#define SK_IGNORE_STDINT_DOT_H
#endif

#include <core/SkBitmap.h>
#include <core/SkDevice.h>
#include <core/SkPaint.h>
#include <core/SkRect.h>
#include <core/SkPath.h>
#include <effects/SkGradientShader.h>

namespace Nx {
 
NxImageCanvas::NxImageCanvas() : NxImage(), mContext(0), mCallBack(0)
{

}

NxImageCanvas::~NxImageCanvas()
{
	mImg->~Image();
}
 
bool NxImageCanvas::createImageFile( int width, int height, int bpp ) { 

	mContext = ( NxImageCanvasContext * ) new NxImageCanvasContext( width, height, bpp == 4 ? true : false );
	bpp == 4 ? mFormat =  NXPF_BYTE_RGBA : NXPF_BYTE_RGB;
	mWidth = width;
	mHeight = height;
	mBpp = bpp;

	mImg = new Image();
	mImg->loadDynamicImage( mContext->getData(), mWidth, mHeight, (Ogre::PixelFormat)mFormat );
	return true;

}

bool NxImageCanvas::openImageFile( const std::string & fileName ) {
	// load ogre image
	NxImage::openImageFile( fileName );
	// draw ogre image data into canvas.
	mContext->drawImage(*mImg, 0,0, mImg->getWidth(), mImg->getHeight() );


	return true;
} 

void NxImageCanvas::CloseImageFile()
{

}

void NxImageCanvas::SetColour( NxColourValue const &val )
{
	mContext->fillStyle( val );
	mContext->fillRect(0, 0, mWidth, mHeight);
	mIsDirty = true; 
}

unsigned char * NxImageCanvas::GetBuffer()
{
	return mImg->getData() ;//  mContext->getData();//
}

NxImageCanvasContext * NxImageCanvas::GetContext() {
	return mContext;
}

void NxImageCanvas::AddCallBack( NxImageCanvasCallBack * CallBack ) {
	mCallBack = CallBack;
}
 
class NxGraphics_Export TextMetrics
{
	public:
		TextMetrics(float xBearing, float yBearing, float width, float height, float xAdvance, float yAdvance);
		float getFullWidth();
		float getFullHeight();

		Ogre::Real xBearing;
		Ogre::Real yBearing;
		Ogre::Real width;
		Ogre::Real height;
		Ogre::Real xAdvance;
		Ogre::Real yAdvance;
};

class NxGraphics_Export Colour //unused
{
	public:
		Colour(const Ogre::ColourValue& color);
		Colour(unsigned int r, unsigned int g, unsigned int b, Ogre::Real a = 1.0);
		Colour(const std::string& hexa);

		Ogre::Real r;
		Ogre::Real g;
		Ogre::Real b;
		Ogre::Real a;
};



typedef std::pair<Ogre::Real, Ogre::ColourValue> ColorStop;

class NxGraphics_Export Gradient
{	
public:
	Gradient(float x0, float y0, float x1, float y1);
	Gradient(float x0, float y0, float radius0, float x1, float y1, float radius1);
	~Gradient();

	void addColorStop(float offset, Ogre::ColourValue color);
	friend class NxImageCanvasContext;

protected:
	SkShader * getShader();
	void createShader();
	void deleteShader();

	static bool colorStopsSorting(ColorStop a, ColorStop b);

	SkShader* mShader;
	GradientType  mType;
	Ogre::Vector2 mP0;
	Ogre::Vector2 mP1;
	Ogre::Real    mRadius0;
	Ogre::Real    mRadius1;
	std::vector<ColorStop> mColorStops;

};

Gradient::Gradient(float x0, float y0, float x1, float y1)
{	
	mP0 = Ogre::Vector2(x0, y0);
	mP1 = Ogre::Vector2(x1, y1);
	mType = GradientType_Linear;
	mShader = NULL;
}

Gradient::Gradient(float x0, float y0, float radius0, float x1, float y1, float radius1)
{
	mP0 = Ogre::Vector2(x0, y0);
	mP1 = Ogre::Vector2(x1, y1);
	mRadius0 = radius0;
	mRadius1 = radius1;
	if (mP0 == mP1)
		mType = GradientType_Radial;
	else
		mType = GradientType_RadialWithFocalPoint;
	mShader = NULL;
}

Gradient::~Gradient()
{
	deleteShader();
}

void Gradient::addColorStop(float offset, Ogre::ColourValue color)
{	
	mColorStops.push_back(ColorStop(offset, color));
	deleteShader();
}

SkShader* Gradient::getShader()
{
	if (mShader == NULL)
		createShader();

	return mShader;
}
void Gradient::createShader()
{
	//FIXME : how is std::sort behaving with two identical values ?
	//std::sort(mColorStops.begin(), mColorStops.end(), colorStopsSorting);

	SkPoint pts[2];
	pts[0].set(mP0.x, mP0.y);
	pts[1].set(mP1.x, mP1.y);

	SkColor* colors = new SkColor[mColorStops.size()];
	SkScalar* pos   = new SkScalar[mColorStops.size()];
	for (unsigned int i=0; i<mColorStops.size(); ++i)
	{
		Ogre::ColourValue color = mColorStops[i].second;
		colors[i] = SkColorSetARGB((U8CPU)(color.a*255), (U8CPU)(color.r*255), (U8CPU)(color.g*255), (U8CPU)(color.b*255));
		pos[i]    = mColorStops[i].first;
	}

	if (mType == GradientType_Linear)
		mShader = SkGradientShader::CreateLinear(pts, colors, pos, mColorStops.size(), SkShader::kClamp_TileMode);
	else if (mType == GradientType_Radial)
		mShader = SkGradientShader::CreateRadial(pts[1], mRadius1, colors, pos, mColorStops.size(), SkShader::kClamp_TileMode);
	else
		mShader = SkGradientShader::CreateTwoPointRadial(pts[0], mRadius0, pts[1], mRadius1, colors, pos, mColorStops.size(), SkShader::kClamp_TileMode);
}

void Gradient::deleteShader()
{
	mShader->safeUnref();
	mShader = NULL;
}

bool Gradient::colorStopsSorting(ColorStop a, ColorStop b)
{ 
	return a.first < b.first;
}
 
 

class NxGraphics_Export Pattern
{
	public:
		Pattern(const Ogre::Image& img, Repetition repeat);
		~Pattern();

		friend class NxImageCanvasContext;

	protected:
		SkShader* getShader() { return mShader; }
		SkShader* mShader;
};

Pattern::Pattern(const Ogre::Image& img, Repetition repeat)
{	
	SkBitmap bitmap;// = new SkBitmap();
	bitmap.setConfig(SkBitmap::kARGB_8888_Config, img.getWidth(), img.getHeight());
	bitmap.allocPixels();
	bitmap.eraseARGB(0x00, 0x00, 0x00, 0x00);	

	Ogre::PixelBox src = img.getPixelBox();
	Ogre::PixelBox dst = Ogre::PixelBox(src.getWidth(), src.getHeight(), src.getDepth(), Ogre::PF_A8R8G8B8); //PF_A8B8G8R8

	dst.data = bitmap.getPixels();
	Ogre::PixelUtil::bulkPixelConversion(src, dst);

	if (repeat == Repetition_Repeat)
		mShader = SkShader::CreateBitmapShader(bitmap, SkShader::kRepeat_TileMode, SkShader::kRepeat_TileMode);
	else
	{
		bool repeatX = repeat == Repetition_RepeatX;
		bool repeatY = repeat == Repetition_RepeatY;
		SkShader::TileMode tileModeX = repeatX ? SkShader::kRepeat_TileMode : SkShader::kClamp_TileMode;
		SkShader::TileMode tileModeY = repeatY ? SkShader::kRepeat_TileMode : SkShader::kClamp_TileMode;
		int expandW = repeatX ? 0 : 1;
		int expandH = repeatY ? 0 : 1;	

		SkBitmap bitmapTmp;
		bitmapTmp.setConfig(bitmap.config(), bitmap.width() + expandW, bitmap.height() + expandH);
		bitmapTmp.allocPixels();
		bitmapTmp.eraseARGB(0x00, 0x00, 0x00, 0x00);
		SkCanvas canvas(bitmapTmp);
		canvas.drawBitmap(bitmap, 0, 0);
		mShader = SkShader::CreateBitmapShader(bitmapTmp, tileModeX, tileModeY);
	}
}

Pattern::~Pattern()
{
	mShader->safeUnref();
	mShader = NULL;
}

class NxGraphics_Export ImageData
{
	public:
		ImageData();

		int width();
		int height();
		unsigned char* data();

	protected:
		unsigned char* mData;
};

class NxGraphics_Export NxImageCanvasState 
{
	public:
		NxImageCanvasState();
		
		//Compositing
		Ogre::Real      globalAlpha;
		DrawingOperator globalCompositeOperation;
		
		//Line styles
		Ogre::Real  lineWidth;
		LineCap     lineCap;
		LineJoin    lineJoin;
		Ogre::Real  miterLimit;
		LineDash    lineDash;
		
		//Stroke and fill style
		ColourSource      strokeColourSource;			
		Ogre::ColourValue strokeColour;
		Gradient*         strokeGradient;
		Pattern*          strokePattern;

		ColourSource      fillColourSource;
		Ogre::ColourValue fillColour;
		Gradient*         fillGradient;
		Pattern*          fillPattern;
		
		//Shadow
		Ogre::Real        shadowOffsetX;
		Ogre::Real        shadowOffsetY;
		Ogre::Real        shadowBlur;
		Ogre::ColourValue shadowColor;

		//TransformationMatrix m_transform;
		//bool m_invertibleCTM;

		// Text
		Ogre::Real   textSize;
		TextAlign    textAlign;
		TextBaseline textBaseline;

		//String m_unparsedFont;
		//Font m_font;
		//bool m_realizedFont;
};

class NxGraphics_Export ColourConverter
{
	public:
		static Ogre::ColourValue fromHexa(const std::string& text);
		static Ogre::ColourValue fromRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);
};

//------------------------------------------------
NxImageCanvasState::NxImageCanvasState()
{
	globalAlpha = 1.0;
	globalCompositeOperation = DrawingOperator_SourceOver;
	
	lineWidth  = 1.0;
	lineCap    = LineCap_Butt;
	lineJoin   = LineJoin_Miter;
	miterLimit = 10.0;
	lineDash   = LineDash_Solid;

	strokeColourSource = ColourSource_Uniform;
	strokeColour       = Ogre::ColourValue::Black;
	strokeGradient     = NULL;
	strokePattern      = NULL;

	fillColourSource = ColourSource_Uniform;
	fillColour       = Ogre::ColourValue::Black;
	fillGradient     = NULL;
	fillPattern      = NULL;

	shadowOffsetX = 0.0;
	shadowOffsetY = 0.0;
	shadowBlur    = 0.0;
	shadowColor   = Ogre::ColourValue::Black;

	textSize     = 10;
	textAlign    = TextAlign_Left;
	textBaseline = TextBaseline_Alphabetic;

	//String m_unparsedFont;
	//Font m_font;
	//bool m_realizedFont;
}

NxImageCanvasContext::NxImageCanvasContext( unsigned int width, unsigned int height, bool alphaEnable )
{
	mWidth       = width;
	mHeight      = height;
	mAlphaEnable = alphaEnable;

	mBitmap = new SkBitmap();
	mBitmap->setConfig(SkBitmap::kARGB_8888_Config, width, height ); 
	//mAlphaEnable ? mBitmap->setConfig(SkBitmap::kARGB_8888_Config, width, height) : mBitmap->setConfig(SkBitmap::kIndex8_Config, width, height);

	mBitmap->allocPixels();
	mBitmap->eraseARGB(0x00, 0x00, 0x00, 0x00);

	mDevice = new SkDevice(*mBitmap);
	mCanvas = new SkCanvas(mDevice);
 
	mFillStyle = new SkPaint;
	mFillStyle->setAntiAlias(true);

	mStrokeStyle = new SkPaint;
	mStrokeStyle->setAntiAlias(true);
	mStrokeStyle->setStyle(SkPaint::kStroke_Style);	

	mPath = new SkPath;

	

	mStates.push( new NxImageCanvasState() );
}

NxImageCanvasContext::~NxImageCanvasContext()
{
/*
	delete mDevice;
	mDevice = NULL;

	delete mBitmap;
	mBitmap = NULL;

	delete mCanvas;
	mCanvas = NULL;	
*/
}


//2D Context
void NxImageCanvasContext::save()
{	
	NxImageCanvasState state = getCurrentState();
	mStates.push(&state);
	mCanvas->save();
}

void NxImageCanvasContext::restore()
{
	if (mStates.size() > 0)
		mStates.pop();
	mCanvas->restore();
}

void NxImageCanvasContext::reset()
{
	for (unsigned int i=0; i<mStates.size(); ++i)
		mStates.pop();
	//FIXME : no call to canvas->restore(); ?
	mStates.push(new NxImageCanvasState());
	beginPath();
}

//Image drawing
void NxImageCanvasContext::drawImage(const Ogre::Image& image, float dstX, float dstY, float dstWidth, float dstHeight)
{
	if (dstWidth == 0 || dstHeight == 0)
		return;

	Pattern* pattern = createPattern(image);

	save();
	translate(dstX, dstY);
	fillStyle(pattern);
	if (dstWidth != 0 && dstHeight != 0) scale(dstWidth/image.getWidth(), dstHeight/image.getHeight());
	fillRect(0.0f, 0.0f, (float) image.getWidth(), (float) image.getHeight());
	restore();

	delete pattern;
}

void NxImageCanvasContext::drawImage(const Ogre::Image& image, const Nx::Vector2& pos, const Nx::Vector2& dim)
{
	drawImage(image, pos.x, pos.y, dim.x, dim.y);
}

void NxImageCanvasContext::drawImage(const Ogre::Image& image, float srcX, float srcY, float srcWidth, float srcHeight, float dstX, float dstY, float dstWidth, float dstHeight)
{
	Pattern* pattern = createPattern(image);

	save();
	beginPath();
	rect(dstX, dstY, dstWidth, dstHeight);
	clip();

	Nx::Vector2 scaling(dstWidth/srcWidth, dstHeight/srcHeight);
	scale(scaling);

	beginPath();
	fillStyle(pattern);
	translate(dstX - srcX*scaling.x, dstY - srcY*scaling.y);
	fillRect(0.0f, 0.0f, (float) image.getWidth(), (float) image.getHeight());

	getCurrentState().fillPattern = NULL;

	restore();

	delete pattern;
}

void NxImageCanvasContext::drawImage(const Ogre::Image& image, const Nx::Vector2& srcPos, const Nx::Vector2& srcDim, const Nx::Vector2& dstPos, const Nx::Vector2& dstDim)
{
	drawImage(image, srcPos.x, srcPos.y, srcDim.x, srcDim.y, dstPos.x, dstPos.y, dstDim.x, dstDim.y);
}

//Compositing
void NxImageCanvasContext::globalAlpha(float alpha)
{	
	getCurrentState().globalAlpha = alpha;
}

float NxImageCanvasContext::globalAlpha()
{
	return getCurrentState().globalAlpha;
}

void NxImageCanvasContext::globalCompositeOperation(DrawingOperator op)
{
	getCurrentState().globalCompositeOperation = op;
}

DrawingOperator NxImageCanvasContext::globalCompositeOperation()
{
	return getCurrentState().globalCompositeOperation;
}

//Line styles
void NxImageCanvasContext::lineWidth(float width)
{
	getCurrentState().lineWidth = width;	
}

float NxImageCanvasContext::lineWidth()
{
	return getCurrentState().lineWidth;
}

void NxImageCanvasContext::lineCap(LineCap lineCap)
{
	getCurrentState().lineCap = lineCap;
}

LineCap NxImageCanvasContext::lineCap()
{
	return getCurrentState().lineCap;
}

void NxImageCanvasContext::lineJoin(LineJoin lineJoin)
{
	getCurrentState().lineJoin = lineJoin;
}

LineJoin NxImageCanvasContext::lineJoin()
{
	return getCurrentState().lineJoin;
}

void NxImageCanvasContext::lineDash(LineDash lineDash)
{	
	getCurrentState().lineDash = lineDash;
}

LineDash NxImageCanvasContext::lineDash()
{
	return getCurrentState().lineDash;
}

void NxImageCanvasContext::miterLimit(float limit)
{
	getCurrentState().miterLimit = limit;	
}

Ogre::Real NxImageCanvasContext::miterLimit()
{
	return getCurrentState().miterLimit;
}

//Colors, styles and shadows
void NxImageCanvasContext::strokeStyle(NxColourValue color)
{
	getCurrentState().strokeColourSource = ColourSource_Uniform;
	getCurrentState().strokeColour =  NxColourtoOgre( color );
}

void NxImageCanvasContext::strokeStyle(Gradient* gradient)
{
	getCurrentState().strokeColourSource = ColourSource_Gradient;
	getCurrentState().strokeGradient = gradient;
}

void NxImageCanvasContext::strokeStyle(Pattern* pattern)
{
	getCurrentState().strokeColourSource = ColourSource_Pattern;
	getCurrentState().strokePattern = pattern;
}

void NxImageCanvasContext::fillStyle( NxColourValue color)
{
	getCurrentState().fillColourSource = ColourSource_Uniform;
	getCurrentState().fillColour = NxColourtoOgre( color );
}

void NxImageCanvasContext::fillStyle(Gradient* gradient)
{
	getCurrentState().fillColourSource = ColourSource_Gradient;
	getCurrentState().fillGradient = gradient;
}

void NxImageCanvasContext::fillStyle(Pattern* pattern)
{
	getCurrentState().fillColourSource = ColourSource_Pattern;
	getCurrentState().fillPattern = pattern;
}

void NxImageCanvasContext::shadowOffsetX(float x)
{
	getCurrentState().shadowOffsetX = x;
}

void NxImageCanvasContext::shadowOffsetY(float y)
{
	getCurrentState().shadowOffsetY = y;
}

void NxImageCanvasContext::shadowBlur(float blur)
{
	getCurrentState().shadowBlur = blur;
}

void NxImageCanvasContext::shadowColor(NxColourValue color)
{
	getCurrentState().shadowColor = NxColourtoOgre( color );
}

void NxImageCanvasContext::textBaseline( TextBaseline baseline )
{
	getCurrentState().textBaseline = baseline;
}

void NxImageCanvasContext::textFont(const std::string& family)
{

}

void NxImageCanvasContext::SetTextSize( float Size )
{
	getCurrentState().textSize = Size;
}

float NxImageCanvasContext::GetTextSize()
{
	return getCurrentState().textSize;
}

Gradient* NxImageCanvasContext::createLinearGradient(float x0, float y0, float x1, float y1)
{
	return new Gradient(x0, y0, x1, y1);
}

Gradient* NxImageCanvasContext::createLinearGradient(const Nx::Vector2& pos0, const Nx::Vector2& pos1)
{
	return createLinearGradient(pos0.x, pos0.y, pos1.x, pos1.y);
}

Gradient* NxImageCanvasContext::createRadialGradient(float x0, float y0, float radius0, float x1, float y1, float radius1)
{
	return new Gradient(x0, y0, radius0, x1, y1, radius1);
}

Gradient* NxImageCanvasContext::createRadialGradient(const Nx::Vector2& pos0, float radius0, const Nx::Vector2& pos1, float radius1)
{
	return createRadialGradient(pos0.x, pos0.y, radius0, pos1.x, pos1.y, radius1);
}

Pattern* NxImageCanvasContext::createPattern(const Ogre::Image& image, Repetition repeat)
{
	return new Pattern(image, repeat);
}

//Rectangles
void NxImageCanvasContext::clearRect(float x,  float y, float w, float h)
{
	save();
	globalCompositeOperation( DrawingOperator_Clear);
	fillRect(x, y, w, h);
	restore();
}

void NxImageCanvasContext::clearRect(const Nx::Vector2& pos,  const Nx::Vector2& dim)
{
	clearRect(pos.x, pos.y, dim.x, dim.y);
}

void NxImageCanvasContext::fillRect(float x, float y, float w, float h)
{
	rect(x, y, w, h);
	fill();
	beginPath();
}

void NxImageCanvasContext::fillRect(const Nx::Vector2& pos, const Nx::Vector2& dim)
{
	fillRect(pos.x, pos.y, dim.x, dim.y);
}

void NxImageCanvasContext::strokeRect(float x, float y, float w, float h)
{
	rect(x, y, w, h);
	stroke();
	beginPath();
}

void NxImageCanvasContext::strokeRect(const Nx::Vector2& pos, const Nx::Vector2& dim)
{
	strokeRect(pos.x, pos.y, dim.x, dim.y);
}

//New
unsigned char* NxImageCanvasContext::getData()
{
	//SkAutoLockPixels image_lock(*mBitmap);
	return (unsigned char*) mBitmap->getPixels();
}

void NxImageCanvasContext::saveToFile(const std::string& filename)
{
	Ogre::Image image;
	image.loadDynamicImage(getData(), mWidth, mHeight, Ogre::PF_A8R8G8B8);
	image.save(filename);
}

void NxImageCanvasContext::applyFillStyle()
{
	NxImageCanvasState state = getCurrentState();
	mFillStyle->setXfermodeMode((SkXfermode::Mode)state.globalCompositeOperation);
	if (state.fillColourSource == ColourSource_Uniform) {
		Ogre::ColourValue color = state.fillColour;
		color.a *= state.globalAlpha;
		mFillStyle->setARGB((U8CPU)(color.a*255), (U8CPU)(color.r*255), (U8CPU)(color.g*255), (U8CPU)(color.b*255));
		mFillStyle->setShader(NULL);
		mFillStyle->setTextSize(state.textSize);
	}
	else if (state.fillColourSource == ColourSource_Gradient)
		mFillStyle->setShader(state.fillGradient->getShader());
	else //if (state.fillColourSource == ColourSource_Pattern)
		mFillStyle->setShader(state.fillPattern->getShader());
}

void NxImageCanvasContext::applyStrokeStyle()
{
	NxImageCanvasState state = getCurrentState();
	if (state.strokeColourSource == ColourSource_Uniform)
	{
		Ogre::ColourValue color = state.strokeColour;
		color.a *= state.globalAlpha;
		mStrokeStyle->setARGB((U8CPU)(color.a*255), (U8CPU)(color.r*255), (U8CPU)(color.g*255), (U8CPU)(color.b*255));
		mStrokeStyle->setStrokeMiter(state.miterLimit);
		mStrokeStyle->setStrokeJoin((SkPaint::Join) state.lineJoin);
		mStrokeStyle->setStrokeCap((SkPaint::Cap) state.lineCap);
		mStrokeStyle->setStrokeWidth(state.lineWidth);
		mStrokeStyle->setTextSize(state.textSize);
	}
	else if (state.strokeColourSource == ColourSource_Gradient)
		mStrokeStyle->setShader(state.strokeGradient->getShader());
	else //if (state.strokeColourSource == ColourSource_Pattern)
		mStrokeStyle->setShader(state.strokePattern->getShader());
}

bool NxImageCanvasContext::pathHasCurrentPoint(SkPath& path)
{
	return path.getPoints(NULL, 0) != 0;
}

NxImageCanvasState& NxImageCanvasContext::getCurrentState()
{
	return *mStates.top();
}

 



Ogre::ColourValue ColourConverter::fromHexa(const std::string& text)
{
	std::string c = text;
	if (c.substr(0,1) == "#")
		c = c.substr(1, c.length() - 1);

	int r,g,b;
	
	std::stringstream ss;
			
	if (c.length() == 3) 
	{
		ss << std::setbase(16) << "0x" << c[0] << c[0];
		ss >> r;
		ss.clear();
		ss << std::setbase(16) << "0x" << c[1] << c[1];
		ss >> g;
		ss.clear();
		ss << std::setbase(16) << "0x" << c[2] << c[2];
		ss >> b;
		ss.clear();
	}
	else if (c.length() == 6) 
	{
		ss << std::setbase(16) << "0x" << c[0] << c[1];
		ss >> r;
		ss.clear();
		ss << std::setbase(16) << "0x" << c[2] << c[3];
		ss >> g;
		ss.clear();
		ss << std::setbase(16) << "0x" << c[4] << c[5];
		ss >> b;
		ss.clear();
	} 
	else
		return Ogre::ColourValue::Black;

	return Ogre::ColourValue(r / 255.0f, g / 255.0f, b / 255.0f, 1);
}

Ogre::ColourValue ColourConverter::fromRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	return Ogre::ColourValue(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

TextMetrics::TextMetrics(float xBearing, float yBearing, float width, float height, float xAdvance, float yAdvance)
{
	this->xBearing = xBearing;
	this->yBearing = yBearing;
	this->width    = width;
	this->height   = height;
	this->xAdvance = xAdvance;
	this->yAdvance = yAdvance;
}

float TextMetrics::getFullWidth() 
{  
	return width + xBearing; 
}

float TextMetrics::getFullHeight() 
{ 
	return height; // + yBearing removed yBearing because for most fonts it returns a minus.}
}





}//namespace