#include "NxSoundPreCompiled.h"

#include "../nxdeps/include/kissfft/kiss_fftr.h"

#define FFTPI 3.141592653589793238462643383279502884197169399

//check how ot display :
//https://github.com/aaronleese/FFT-Plugin/blob/master/Source/accelerateFFT.cpp

struct NxFFtStruct
{
public:
	kiss_fft_cpx * mCxIn;
	kiss_fft_cpx * mCxOut;
	kiss_fftr_cfg mFftCfg;
	kiss_fftr_cfg mIfftCfg;

	NxFFtStruct() {
 
	}

	~NxFFtStruct(){

	}

} ;


namespace NxSound_Namespace {

NxSoundFFT::NxSoundFFT()
{
	mFFt = new NxFFtStruct();

	int dataSize = SND_BUFFER_SIZE; // numSamples
	// Set running flag
	mRunning = true;

	// Set data size
	mDataSize = dataSize;
	setDataSize( dataSize );
}

NxSoundFFT::~NxSoundFFT()
{


}

unsigned int NxSoundFFT::GetNumBins()
{ 
	return getBinSize();
}

void NxSoundFFT::GetSpectrum( float * Spectrum )
{
	//memcpy( &Spectrum[0], &Magnitudes[0], (TotalNumbins/2) * sizeof(float) );
}


void NxSoundFFT::Initialize( int NumSamplesIn, int BinsOut )
{


}

bool NxSoundFFT::PerformFFT( float * PcmAudio  )
{


	setData( PcmAudio );


	return true;
}

void NxSoundFFT::dispose()
{

	// Free KISS resources
	if ( mFFt->mFftCfg ) {
		kiss_fftr_free( mFFt->mFftCfg );
	}
	if ( mFFt->mIfftCfg ) {
		kiss_fftr_free( mFFt->mIfftCfg );
	}

	// Delete arrays
	if ( mAmplitude != 0 ) {
		delete [] mAmplitude;
	}
	if ( mFFt->mCxIn != 0 ) {
		delete [] mFFt->mCxIn;
	}
	if ( mFFt->mCxOut != 0 ) {
		delete [] mFFt->mCxOut;
	}
	if ( mData != 0 ) {
		delete [] mData;
	}
	if ( mImag != 0 ) {
		delete [] mImag;
	}
	if ( mInverseWindow != 0 ) {
		delete [] mInverseWindow;
	}
	if ( mPhase != 0 ) {
		delete [] mPhase;
	}
	if ( mReal != 0 ) {
		delete [] mReal;
	}
	if ( mWindow != 0 ) {
		delete [] mWindow;
	}
	if ( mWindowedData != 0 ) {
		delete [] mWindowedData;
	}

}


void NxSoundFFT::setData( float * data )
{
	// Set all flags to false
	mCartesianUpdated = false;
	mPolarUpdated = false;
	mCartesianNormalized = false;
	mPolarNormalized = false;
	mDataUpdated = false;
	mDataNormalized = false;

	// Copy incoming data
	memcpy( mData, data, sizeof( float ) * mDataSize );

	// Set data flag
	mDataUpdated = true;
	mDataNormalized = true;
}

void NxSoundFFT::setDataSize( int dataSize )
{
	// Dispose is size has changed
	if ( dataSize != mDataSize ) {
		dispose();
	}

	// Set dimensions
	mDataSize = dataSize;
	mBinSize =  ( mDataSize / 2 ) + 1;
	mWindowSum = 0.0f;

	// Set flags
	mCartesianNormalized = true;
	mCartesianUpdated = true;
	mDataNormalized = true;
	mPolarNormalized = true;
	mPolarUpdated = true;

	// Allocate arrays
	mAmplitude = new float[ mBinSize ];
	mData = new float[ mDataSize ];
	mImag = new float[ mBinSize ];
	mInverseWindow = new float[ mDataSize ];
	mReal = new float[ mBinSize ];
	mPhase = new float[ mBinSize ];
	mWindow = new float[ mDataSize ];
	mWindowedData = new float[ mDataSize ];

	// Set frequencies
	mFrequencyHigh = 1.0f;
	mFrequencyLow = 0.0f;

	// Initialize array values
	memset( mData, 0, sizeof( float ) * mDataSize );
	memset( mReal, 0, sizeof( float ) * mBinSize );
	memset( mImag, 0, sizeof( float ) * mBinSize );
	memset( mAmplitude, 0, sizeof( float ) * mBinSize );
	memset( mPhase, 0, sizeof( float ) * mBinSize );

	//calculate the window
	for ( unsigned int i = 0; i < mDataSize; i++ ) {
		mWindow[ i ] = sin( (float) ( (float)FFTPI * i) / ( mDataSize - 1 ) );
		mWindowSum += mWindow[ i ];
		mInverseWindow[ i ] = 1.0f / mWindow[ i ];
	}


	// Hamming Window
	//for ( unsigned int i = 0; i < mDataSize; i++ ) {

	//	mWindow[ i ] = (0.54f - 0.46f * cos( ( 2.0f * (float)FFTPI ) * i / (mDataSize - 1)));
	//	mWindowSum += mWindow[ i ];
	//	mInverseWindow[ i ] = 1.0f / mWindow[ i ];
	//}

 

	 


	// Set up KISS
	mFFt->mFftCfg = kiss_fftr_alloc( mDataSize, 0, 0, 0 );
	mFFt->mIfftCfg = kiss_fftr_alloc( mDataSize, 1, 0, 0 );
	mFFt->mCxIn = new kiss_fft_cpx[ mBinSize ];
	mFFt->mCxOut = new kiss_fft_cpx[ mBinSize ];
}

float* NxSoundFFT::getAmplitude()
{
	cartesianToPolar();
	return mAmplitude;
}
 
// Get input data // INVERSE FFT
float * NxSoundFFT::getData()
{
	// Data has not been updated
	if ( !mDataUpdated ) {
		transform();

		// Perform inverse FFT
		for ( unsigned int i = 0; i < mBinSize; i++ ) {
			mFFt->mCxIn[ i ].r = mReal[ i ];
			mFFt->mCxIn[ i ].i = mImag[ i ];
		}

		kiss_fftri( mFFt->mIfftCfg, mFFt->mCxIn, mData );

		// Populate data array
		for ( unsigned int i = 0; i < mDataSize; i++ ) {
			mData[ i ] *= mInverseWindow[i];
		}

		// Update flags
		mDataUpdated = true;
		mDataUpdated = true;
		mDataNormalized = false;

	}

	// Data has not been normalized
	if ( !mDataNormalized ) {

		// Normalize data
		float mNormalizer = (float)mWindowSum / (float)( 2 * mDataSize );
		for ( unsigned int i = 0; i < mDataSize; i++ ) {
			mData[ i ] *= mNormalizer;
		}

		mDataNormalized = true;

	}

	// Return data
	return mData;

}
 
float * NxSoundFFT::getImaginary()
{
	// Perform FFT and return imaginary data
	transform();
	return mImag;
}

float * NxSoundFFT::getPhase()
{
	// Prepare and return phase data
	cartesianToPolar();
	return mPhase;
}

float * NxSoundFFT::getReal()
{
	// Perform FFT and return real data
	transform();
	return mReal;
}

// Performs FFT
void NxSoundFFT::transform()
{
	// Check flag
	if ( !mRunning ) {
		return;
	}

	// Cartesian values need updating
	if ( !mCartesianUpdated ) {

		// Polar values need updating
		if ( !mPolarUpdated ) {

			// Copy data to windowed array
			memcpy( mWindowedData, mData, sizeof(float) * mDataSize );
 
			for ( unsigned int i = 0; i < mDataSize; i++ ) {
				mWindowedData[i] = mData[ i ] * mWindow[ i ];
			}
 
			// Perform FFT
			kiss_fftr( mFFt->mFftCfg, mWindowedData, mFFt->mCxOut );

			// Iterate through complex values
			for ( unsigned int i = 0; i < mBinSize; i++ ) {
				// Bail if running flag turns off
				if ( !mRunning ) {
					return;
				}

				// Apply complex value if index is within filter range
				bool mApplyComlex = i >= mFrequencyLow * mBinSize && i <= mFrequencyHigh * mBinSize;

				// Extract complex values
				mReal[ i ] = mApplyComlex ? mFFt->mCxOut[ i ].r : 0.0f;
				mImag[ i ] = mApplyComlex ? mFFt->mCxOut[ i ].i : 0.0f;
			}
 
			// Update flag
			mCartesianUpdated = true;

		} else {

			// Apply phase and amplitude to values
			for ( unsigned int i = 0; i < mBinSize; i++ ) {
				mReal[ i ] =  cos( (float)mPhase[i] ) * mAmplitude[ i ];
				mImag[ i ] =  sin( (float)mPhase[i] ) * mAmplitude[ i ];
			}
 
			mCartesianUpdated = true;
			mCartesianNormalized = mPolarNormalized;
		}
	}

	// Complex values are not yet normalized
	if ( !mCartesianNormalized ) {

		// Normalize values
		float mNormalizer = 2.0f / mWindowSum;
		for ( unsigned int i = 0; i < mBinSize; i++ ) {
			mReal[ i ] *= mNormalizer;
			mImag[ i ] *= mNormalizer;
		}
 
		mCartesianNormalized = true;
	}
}
// Sets amplitude and phase arrays
void NxSoundFFT::cartesianToPolar()
{
	// Check if polar data is current
	if ( !mPolarUpdated ) {

		// Perform FFT
		transform();

		// Find absolute maximums of and angles between values
		// and use them to set the amplitude and phase data, respectively
		for ( unsigned int i = 0; i < mBinSize; i++ ) {
			mAmplitude[ i ] = sqrtf( pow( mReal[ i ], 2.0f ) + pow( mImag[ i ], 2.0f ) );
			// http://code.google.com/p/libgdx/source/browse/trunk/gdx/jni/KissFFT.cpp?r=253
			mPhase[ i ] = atan2( mImag[ i ], mReal[ i ] );
		}
 
		// Set flags
		mPolarUpdated = true;
		mPolarNormalized = mCartesianNormalized;

	}

	// Polar data is not yet normalized
	if ( !mPolarNormalized ) {

		// Normalize values
		float mNormalizer = 2.0f / mWindowSum;
		for ( unsigned int i = 0; i < mBinSize; i++ ) {
			mAmplitude[ i ] *= mNormalizer;
		}
 
		mPolarNormalized = true;

	}

}



 


}