#include "NxSoundPreCompiled.h"
#include "FileReadWav.h"

namespace stk {

FileReadWav::FileReadWav() : FileLoop(), mLoop(true),  mResolutionCoef(1.0)
{

}

FileReadWav::~FileReadWav()
{
 
}
 
void FileReadWav::SetPosition( double TimeSeconds )
{
	time_ = TimeSeconds;
}

void FileReadWav::SetLooping( bool Loop )
{
	mLoop = Loop;
}

double FileReadWav::GetPosition()
{
	return time_;
}

double FileReadWav::GetRate()
{
	return rate_ ;
}

double FileReadWav::GetFrequency()
{
	return data_.dataRate() ;
}

unsigned long FileReadWav::GetFileSize()
{
	return file_.fileSize();
}

NxAudioFormat FileReadWav::GetNativeResolution()
{
	return file_.format(); 
}

void FileReadWav::SetOutputResolution( NxAudioFormat Res )
{
	NxAudioFormat FileFormat = file_.format();
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

bool FileReadWav::IsFinished()
{
return finished_;
}

StkFrames& FileReadWav::Tick( StkFrames& frames )
{
	if ( !file_.isOpen() ) {
		return frames;
	}

	 
	//if ( finished_ ) return frames;//0.0;

	 if ( finished_ ) {
	 	for ( unsigned int i=0; i<frames.size(); i++ ) frames[i] = 0.0;
	 	return frames;//0.0;
	 }


 //time_

	unsigned int nChannels = lastFrame_.channels();
 
	unsigned int j, counter = 0;
	for ( unsigned int i=0; i<frames.frames(); i++ ) {
		FileLoop::tick();

		if ( time_ < 0.0 || time_ > (StkFloat) ( file_.fileSize() - 1.0 ) && !mLoop ) 
		{
			for ( unsigned int i=0; i<frames.size(); i++ ) frames[i] = 0.0;
			finished_ = true; return frames;
		}
		
		for ( j=0; j<nChannels; j++ )
			frames[counter++] = lastFrame_[j] * mResolutionCoef ;
	}

	return frames;
}



}

