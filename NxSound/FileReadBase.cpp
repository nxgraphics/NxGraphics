#include "NxSoundPreCompiled.h"
#include "STK/include/Stk.h"
#include "FileRead.h"

using namespace NxSound_Namespace;

namespace NxSound_Namespace { //stk

FileReadBase::FileReadBase( void ): mFile(0) , mLoop( true ), mFinished(false), mResolutionCoef(1.0)
{

}

FileReadBase::~FileReadBase( void )
{

}

unsigned long FileReadBase::GetFileSize() const 
{ 
	return mFileSize; 
}

unsigned int FileReadBase::GetChannels() const
{ 
	return mChannels;
}

NxDouble FileReadBase::GetFileRate( void ) const 
{ 
	return mFileRate;
}

int64 FileReadBase::GetFirstSamplePosition()
{ 
	return SampleFirstPosition ;
}

uint64 FileReadBase::GetSampleNum()
{
	return mSampleNum;
}

//int FileReadBase::GetBps()
//{ 
//	return Bps;
//}

unsigned int FileReadBase::GetBlockSize() const 
{ 
	return mBlockSize;
}

//unsigned int FileReadBase::GetResolution()
//{ 
//	return mResolution;
//}

void FileReadBase::SetOutputResolution( NxAudioFormat Res )
{
	NxAudioFormat FileFormat = mDataType;
	//Log( "File resolution : " + NxSoundUtils::ToString( FileFormat )  );

	switch( FileFormat ) {
		case NX_SINT8 : // 8-bit signed integer.
			mResolutionCoef = 1.0 / 128.0;
			break;
		case NX_SINT16 : // 16-bit signed integer.
			mResolutionCoef = 1.0 / 32768.0;
			break;
		case NX_SINT24 : // Lower 3 bytes of 32-bit signed integer.
			mResolutionCoef = 1.0 / 2147483648.0;
			break;
		case NX_SINT32 : break; // 32-bit signed integer.				 
		case NX_FLOAT32 : break; // Normalized between plus/minus 1.0.
		case NX_FLOAT64 : break; // Normalized between plus/minus 1.0.
		default: break;
	}
}


void  FileReadBase::SetLooping( bool Loop )
{ 
	mLoop = Loop;
}

bool FileReadBase::IsFinished()
{ 
	return mFinished;
} 


}//namespace