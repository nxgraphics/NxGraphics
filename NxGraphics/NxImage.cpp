#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

namespace Nx {

NxImage::NxImage() : mIsDirty(false)
{

}

NxImage::~NxImage()
{
	mImg->~Image();
}


 


bool NxImage::openImageFile( const std::string & FileName )
{
	std::ifstream i, j;
	Ogre::FileStreamDataStream* pFS = 0;
	i.open(FileName.c_str(), std::ios::binary);
	if (i.is_open()) {
		pFS = new Ogre::FileStreamDataStream(&i, false);
	}
	else { return false ; }
	Ogre::DataStreamPtr strm(pFS);

	mImg = new Image();
	mImg->load(strm);
	mWidth = mImg->getWidth();
	mHeight = mImg->getHeight();

	if( mImg->getHasAlpha() ) { 
		mFormat = NXPF_BYTE_BGRA;
		mBpp =  4 ;
	}  
	else {
		mFormat = NXPF_BYTE_BGR;
		mBpp =  3 ;
	}

 

	return true ;
}

void NxImage::CloseImageFile()
{

}

void NxImage::Resize( unsigned short width, unsigned short height )
{
	mImg->resize( width, height, Ogre::Image::FILTER_BILINEAR );
}

size_t NxImage::GetHeight()
{
	return mHeight;
}

size_t NxImage::GetWidth()
{
	return mWidth;
}

size_t NxImage::GetBpp()
{
	return mBpp ;
}

bool NxImage::IsDirty()
{
	return mIsDirty;
}

void NxImage::SetDirty(bool dirty)
{
	mIsDirty = dirty;
}

void NxImage::SetColour( NxColourValue const &val )
{
	LogMsg("Passe NxImage::SetColour");
	unsigned long numPixels = mWidth * mHeight * mBpp;
	for( int x = 0; x < mWidth; x++ ) {
		for( int y = 0; y < mHeight; y++ ) {
			 mImg->setColourAt( NxColourtoOgre( val ) ,x,y,0); 
		}
	}
}
 
const NxColourValue NxImage::GetImageAverageColorAtPos( int x1, int y1, int x2, int y2 ) const
{
	float redBucket = 0;
	float greenBucket = 0;
	float blueBucket = 0;
	float alphaBucket = 0;
	unsigned long numPixels = 0;
	for( int y = y1; y < y2; y++  ) {
		for( int x = x1; x < x2; x++  ) {
			Ogre::ColourValue col = mImg->getColourAt(x,y,0);
			redBucket += col.r;
			greenBucket += col.g;
			blueBucket += col.b;
			alphaBucket += col.a;
			numPixels++;
		}
	}
	return NxColourValue( redBucket/(float)numPixels, greenBucket/(float)numPixels, blueBucket/(float)numPixels, alphaBucket/(float)numPixels );
}

const NxColourValue NxImage::GetImageAverageColor() const
{
	return GetImageAverageColorAtPos( 0, 0, mWidth, mHeight );
}
 
NxColourValue NxImage::GetColourAt( int X, int Y, int Z ) const
{
	Ogre::ColourValue val = mImg->getColourAt(X,Y,Z);
	return NxColourValue( val.r, val.g, val.b, val.a );
}

void NxImage::FlipY()
{
	mImg->flipAroundY();
}

void NxImage::FlipX()
{
	mImg->flipAroundX();
}

NxPixelFormat NxImage::GetPixelFormat()
{
	return mFormat;
}

//Ogre::Image * NxImage::GetImage()
const Ogre::Image & NxImage::GetImage()
{
	return *mImg  ;
}

unsigned char * NxImage::GetBuffer()
{
	return mImg->getData() ;
}

}//namespace