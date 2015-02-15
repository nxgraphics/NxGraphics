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
#ifndef __NXIMAGE_H__
#define __NXIMAGE_H__

#include "NxPrerequisites.h"
#include <stack>

class SkPath;
class SkBitmap;
class SkDevice;
class SkCanvas;
class SkPaint;
class SkPaint;

namespace Nx {

class Gradient;
class Pattern;
class TextMetrics;
class ImageData;
class NxImageCanvasState;
class NxImageCanvasContext;

class NxGraphics_Export NxImage
{
public :
	NxImage();
	~NxImage();
	virtual bool openImageFile( const std::string & FileName );
	
	virtual void CloseImageFile();
	virtual unsigned char * GetBuffer();
	size_t GetHeight();
	size_t GetWidth();
	size_t GetBpp();
	void Resize( unsigned short width, unsigned short height );
	bool IsDirty();
	void SetDirty(bool dirty);
	virtual void SetColour( NxColourValue const &val  );
	const NxColourValue GetImageAverageColorAtPos( int x1, int y1, int x2, int y2 ) const;
	const NxColourValue GetImageAverageColor() const;
	NxColourValue GetColourAt( int X, int Y, int Z ) const;
	void FlipY();
	void FlipX();
	NxPixelFormat GetPixelFormat();
	const Ogre::Image & GetImage();

protected:
	Ogre::Image * mImg;
	NxPixelFormat mFormat;
	size_t mWidth;
	size_t mHeight;
	size_t mBpp;
	unsigned char * mBuffer;
	bool mIsDirty;
};

//! this code from Ogre::Canvas 
// http://www.visual-experiments.com/demos/ogrecanvas/
enum SkXfermodeModes 
{
    kClear_Mode,    //!< [0, 0]
    kSrc_Mode,      //!< [Sa, Sc]
    kDst_Mode,      //!< [Da, Dc]
    kSrcOver_Mode,  //!< [Sa + Da - Sa*Da, Rc = Sc + (1 - Sa)*Dc]
    kDstOver_Mode,  //!< [Sa + Da - Sa*Da, Rc = Dc + (1 - Da)*Sc]
    kSrcIn_Mode,    //!< [Sa * Da, Sc * Da]
    kDstIn_Mode,    //!< [Sa * Da, Sa * Dc]
    kSrcOut_Mode,   //!< [Sa * (1 - Da), Sc * (1 - Da)]
    kDstOut_Mode,   //!< [Da * (1 - Sa), Dc * (1 - Sa)]
    kSrcATop_Mode,  //!< [Da, Sc * Da + (1 - Sa) * Dc]
    kDstATop_Mode,  //!< [Sa, Sa * Dc + Sc * (1 - Da)]
    kXor_Mode,      //!< [Sa + Da - 2 * Sa * Da, Sc * (1 - Da) + (1 - Sa) * Dc]
    // these modes are defined in the SVG Compositing standard
    // http://www.w3.org/TR/2009/WD-SVGCompositing-20090430/
    kPlus_Mode,
    kMultiply_Mode,
    kScreen_Mode,
    kOverlay_Mode,
    kDarken_Mode,
    kLighten_Mode,
    kColorDodge_Mode,
    kColorBurn_Mode,
    kHardLight_Mode,
    kSoftLight_Mode,
    kDifference_Mode,
    kExclusion_Mode,
    kLastMode = kExclusion_Mode
};

enum DrawingOperator 
{
	DrawingOperator_Copy        = kSrcOver_Mode, //dont't known which one it is
	DrawingOperator_SourceOver  = kSrcOver_Mode,
	DrawingOperator_SourceIn    = kSrcIn_Mode,
	DrawingOperator_SourceOut   = kSrc_Mode,
	DrawingOperator_SourceATop  = kSrcATop_Mode,
	DrawingOperator_DestOver    = kDstOver_Mode,
	DrawingOperator_DestIn      = kDstIn_Mode,
	DrawingOperator_DestOut     = kDst_Mode,
	DrawingOperator_DestATop    = kDstATop_Mode,
	DrawingOperator_Xor         = kXor_Mode,
	DrawingOperator_PlusDarker  = kDarken_Mode,  //dont't known which one it is
	DrawingOperator_Highlight   = kSrcOver_Mode, //dont't known which one it is
	DrawingOperator_PlusLighter = kLighten_Mode, //dont't known which one it is
	DrawingOperator_Clear       = kClear_Mode
};

enum SkPaintCap 
{
    kButt_Cap,      //!< begin/end contours with no extension
    kRound_Cap,     //!< begin/end contours with a semi-circle extension
    kSquare_Cap,    //!< begin/end contours with a half square extension
    kCapCount,
    kDefault_Cap = kButt_Cap
};

enum SkPaintJoin 
{
    kMiter_Join,    //!< connect path segments with a sharp join
    kRound_Join,    //!< connect path segments with a round join
    kBevel_Join,    //!< connect path segments with a flat bevel join
    kJoinCount,
    kDefault_Join = kMiter_Join
};

enum LineCap
{
	LineCap_Butt   = kButt_Cap,
	LineCap_Round  = kRound_Cap,
	LineCap_Square = kSquare_Cap
};

enum LineJoin
{
	LineJoin_Round = kRound_Join,
	LineJoin_Bevel = kBevel_Join,
	LineJoin_Miter = kMiter_Join
};

enum LineDash
{
	LineDash_Solid,
	LineDash_Dotted,
	LineDash_Dashed
};

enum Repetition
{
	Repetition_Repeat,
	Repetition_RepeatX,
	Repetition_RepeatY,
	Repetition_NoRepeat
};

enum SkPaintAlign 
{
    kLeft_Align,
    kCenter_Align,
    kRight_Align,

    kAlignCount
};

enum TextAlign
{
	TextAlign_Left   = kLeft_Align,
	TextAlign_Center = kCenter_Align,
	TextAlign_Right  = kRight_Align
};

enum TextBaseline
{
	TextBaseline_Top,
	TextBaseline_Hanging,
	TextBaseline_Middle,
	TextBaseline_Alphabetic,
	TextBaseline_Ideographic, 
	TextBaseline_Bottom
};

enum ColourSource
{
	ColourSource_Uniform,
	ColourSource_Gradient,
	ColourSource_Pattern
};

enum GradientType
{
	GradientType_Linear,
	GradientType_Radial,
	GradientType_RadialWithFocalPoint
};
 
class NxGraphics_Export NxImageCanvasContext
{
public:
	NxImageCanvasContext( unsigned int Width, unsigned int Height, bool Alpha = true );
	~NxImageCanvasContext();
	//2D Context			
	void save();
	void restore();
	void reset();
	//Transformation					
	void scale( float x, float y );
	void scale( const Nx::Vector2& scaling);			
	void rotate( float radian);
	void rotate( const Nx::Radian& angle);
	void translate(float x, float y);
	void translate(const Nx::Vector2& translation);
	void transform(float m11, float m12, float m21, float m22, float dx,  float dy);
	void transform(const Nx::Matrix3& transform);
	void setTransform(float m11, float m12, float m21, float m22, float dx,  float dy);
	void setTransform(const Nx::Matrix3& transform);
	//Image drawing			
	void drawImage(const Ogre::Image& image, float dstX, float dstY, float dstWidth = 0, float dstHeight = 0);
	void drawImage(const Ogre::Image& image, const Nx::Vector2& pos, const Nx::Vector2& dim = Nx::Vector2::ZERO);
	void drawImage(const Ogre::Image& image, float srcX, float srcY, float srcWidth, float srcHeight, float dstX, float dstY, float dstWidth, float dstHeight);
	void drawImage(const Ogre::Image& image, const Nx::Vector2& srcPos, const Nx::Vector2& srcDim, const Nx::Vector2& dstPos, const Nx::Vector2& dstDim);
	//Compositing
	void globalAlpha(float alpha);
	float globalAlpha();
	void globalCompositeOperation(DrawingOperator op);
	DrawingOperator globalCompositeOperation();
	//Line styles
	void lineWidth(float width);
	float lineWidth();
	void lineCap(LineCap lineCap);
	LineCap lineCap();
	void lineJoin(LineJoin lineJoin);
	LineJoin lineJoin();
	void miterLimit(float limit);
	float miterLimit();
	void lineDash(LineDash lineDash);
	LineDash lineDash();
	//Colors, styles and shadows
	void strokeStyle(NxColourValue color);
	void strokeStyle( Gradient * gradient);
	void strokeStyle( Pattern * pattern);
	void fillStyle(NxColourValue color);
	void fillStyle(Gradient * gradient);
	void fillStyle(Pattern * pattern);
	void shadowOffsetX(float x);
	void shadowOffsetY(float y);
	void shadowBlur(float blur);
	void shadowColor(NxColourValue color);
	Gradient* createLinearGradient(float x0, float y0, float x1, float y1);
	Gradient* createLinearGradient(const Nx::Vector2& pos0, const Nx::Vector2& pos1);
	Gradient* createRadialGradient(float x0, float y0, float radius0, float x1, float y1, float radius1);
	Gradient* createRadialGradient(const Nx::Vector2& pos0, float radius0, const Nx::Vector2& pos1, float radius1);
	Pattern* createPattern(const Ogre::Image& image, Repetition = Repetition_Repeat);
	//Paths
	void beginPath();
	void closePath();
	void fill();
	void stroke();
	void clip();
	void moveTo(float x, float y);
	void moveTo(const Nx::Vector2& pos);
	void lineTo(float x, float y);
	void lineTo(const Nx::Vector2& pos);
	void Circle( float x, float y, float Radius );
	void quadraticCurveTo(float cpx, float cpy, float x, float y);
	void quadraticCurveTo(const Nx::Vector2& controlPoint, const Nx::Vector2& pos);
	void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);
	void bezierCurveTo(const Nx::Vector2& controlPoint1, const Nx::Vector2& controlPoint2, const Nx::Vector2& pos);
	void arcTo(float x1, float y1, float x2, float y2, float radius);
	void arcTo(const Nx::Vector2& pos1, const Nx::Vector2& pos2, float radius);
	void arc(float x, float y, float radius, float startRadian, float endRadian, bool anticlockwise);
	void arc(const Nx::Vector2& pos, float radius, Nx::Radian start, Nx::Radian end, bool anticlockwise);
	void rect(float x, float y, float w, float h);
	void rect(const Nx::Vector2& pos, const Nx::Vector2& dim);
	bool isPointInPath(float x, float y);
	bool isPointInPath(const Nx::Vector2& pos);	
	//Text
	void SetTextSize( float Size );
	float GetTextSize();
	void textFont(const std::string& family);
	std::string textFont();
	void textAlign(TextAlign align);
	TextAlign textAlign();
	void textBaseline(TextBaseline baseline);
	TextBaseline textBaseline();
	void fillText(const std::string& text, float x, float y, float  maxWidth = -1);
	void fillText(const std::string& text, const Nx::Vector2& pos, float maxWidth = -1);
	void strokeText(const std::string& text, float x, float y, float maxWidth = -1);
	void strokeText(const std::string& text, const Nx::Vector2& pos, float maxWidth = -1);			
	TextMetrics measureText(const std::string& text);
	//Rectangles			
	void clearRect(float x, float y, float w, float h);
	void clearRect(const Nx::Vector2& pos, const Nx::Vector2& dim);
	void fillRect(float x, float y, float w, float h);
	void fillRect(const Nx::Vector2& pos, const Nx::Vector2& dim);
	void strokeRect(float x, float y, float w, float h);
	void strokeRect(const Nx::Vector2& pos, const Nx::Vector2& dim);
	ImageData createImageData(float width, float height);
	ImageData createImageData(const Nx::Vector2& dim);
	ImageData createImageData(ImageData imageData);
	ImageData getImageData(float x, float y, float width, float height);
	ImageData getImageData(const Nx::Vector2& pos, const Nx::Vector2& dim);
	void putImageData(ImageData imageData, float x, float y, float dirtyX = 0, float dirtyY = 0);
	void putImageData(ImageData imageData, const Nx::Vector2& pos, const Nx::Vector2& dirty = Nx::Vector2::ZERO);
	unsigned char* getData();
	void saveToFile(const std::string& _filename);
	unsigned int GetWidth()  { return mWidth;  }
	unsigned int GetHeight() { return mHeight; }



protected:						
	void applyFillStyle();
	void applyStrokeStyle();
	bool pathHasCurrentPoint(SkPath& path);

	NxImageCanvasState& getCurrentState();

	unsigned int      mWidth;
	unsigned int      mHeight;
	bool              mAlphaEnable;
	SkPath*           mPath;
	SkBitmap*         mBitmap;
	SkDevice*         mDevice;
	SkCanvas*         mCanvas;
	SkPaint*          mFillStyle;
	SkPaint*          mStrokeStyle;

	std::stack<NxImageCanvasState*> mStates;
};

class NxGraphics_Export NxImageCanvasCallBack
{
public:
	NxImageCanvasCallBack(){};
	virtual ~NxImageCanvasCallBack(){};
	virtual void OnCanvasUpdate( NxImageCanvasContext & Graphics ) = 0;

};

class NxGraphics_Export NxImageCanvas : public NxImage
{
public :
	NxImageCanvas();
	~NxImageCanvas();
	//! create empty image file
	bool createImageFile(  int Width = 320 , int Height = 240 , int Bpp = 4 );
	//! open image file into the canvas
	bool openImageFile( const std::string & FileName );
	//! close and release image.
	void CloseImageFile();
	
	
	void SetColour( NxColourValue const &val  );
	unsigned char * GetBuffer();
	NxImageCanvasContext * GetContext();
	void AddCallBack( NxImageCanvasCallBack * CallBack );
	NxImageCanvasCallBack * mCallBack;
private:
	NxImageCanvasContext * mContext;
	
};
 
class NxGraphics_Export NxImagePtr : public shared_ptr<NxImage> 
{
public:
	NxImagePtr() : shared_ptr<NxImage>(){ }
    explicit NxImagePtr( NxImage * rep ) : shared_ptr<NxImage>(rep){ }
    NxImagePtr( const NxImagePtr& r ) : shared_ptr<NxImage>(r){ } 
};
 

}//namespace

#endif

 
 